/* SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include "nv_soc_hwpm.h"
#include "common/log.h"
#include "os/nv_soc_hwpm_os.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIB_VERSION_MAJOR 0
#define LIB_VERSION_MINOR 1

/* Internal tracker of the number of enumerated NV_SOC_HWPM devices. */
static uint32_t g_device_count = 0;

/* Internal NV_SOC_HWPM device array. */
static nv_soc_hwpm_device_int g_devices_int[MAX_DEVICE_COUNT] = {{ 0 }};

/* The nv_soc_hwpm_device handle contains the index in g_devices_int array. */
nv_soc_hwpm_device_int* to_device_int(uint64_t handle) {
	const uint64_t dev_idx = to_dev_idx(handle);
	if (dev_idx >= g_device_count) {
		log_error("Invalid device handle: %llu\n", handle);
		return NULL;
	}

	return &g_devices_int[dev_idx];
}

/**
 * The nv_soc_hwpm_session handle contains the index in
 * nv_soc_hwpm_device_int::session array.
 */
nv_soc_hwpm_session_int* to_session_int(uint64_t handle) {
	nv_soc_hwpm_device_int* dev_int = to_device_int(handle);
	if (dev_int == NULL) {
		log_error("Invalid device session: %llu\n", handle);
		return NULL;
	}

	const uint32_t session_idx = to_session_idx(handle);
	if (session_idx >= dev_int->session_count_max) {
		log_error("Invalid session: %llu\n", handle);
		return NULL;
	}

	return dev_int->session_int[session_idx];
}

int nv_soc_hwpm_init(void) {
        int ret = 0;

	if (g_device_count == 0) {
		ret = nv_soc_hwpm_os_enumerate_device(
			MAX_DEVICE_COUNT, &g_device_count, g_devices_int);
	}

	return ret;
}

void nv_soc_hwpm_exit(void) {
        uint32_t i, s;

	for (i = 0; i < g_device_count; ++i) {
		for (s = 0; s < g_devices_int[i].session_count_max; ++s) {
			if (g_devices_int[i].session_int[s]) {
				nv_soc_hwpm_session session =
					{.handle = to_session_handle(i, s)};
				nv_soc_hwpm_session_free(session);
			}
		}
	}

	g_device_count = 0;
	memset(&g_devices_int, 0, sizeof(g_devices_int));
}

int nv_soc_hwpm_system_get_info(
	nv_soc_hwpm_system_attribute attribute, uint32_t info_size, void* info)
{
	if (info == NULL) {
		log_error("Invalid info param\n");
		return -EINVAL;
	}

	switch (attribute) {
	case NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MAJOR:
		if (info_size < sizeof(uint16_t)) {
			log_error("Invalid info size. Expected %d, got %d\n",
				sizeof(uint16_t), info_size);
			return -EINVAL;
		}
		*((uint16_t*)info) = LIB_VERSION_MAJOR;
		break;
	case NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MINOR:
		if (info_size < sizeof(uint16_t)) {
			log_error("Invalid info size. Expected %d, got %d\n",
				sizeof(uint16_t), info_size);
			return -EINVAL;
		}
		*((uint16_t*)info) = LIB_VERSION_MINOR;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

int nv_soc_hwpm_get_devices(uint32_t *count, nv_soc_hwpm_device *devices)
{
	uint32_t i, dev_count;

	if (count == NULL) {
		log_error("Invalid count param\n");
		return -EINVAL;
	}

	if (*count == 0) {
		*count = g_device_count;
		return 0;
	}

	if (devices == NULL) {
		log_error("Invalid devices param\n");
		return -EINVAL;
	}

	dev_count = (*count < g_device_count) ? *count : g_device_count;
	for (i = 0; i < dev_count; ++i) {
		devices[i].handle = to_dev_handle(i);
	}

	return 0;
}

int nv_soc_hwpm_device_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_device_attribute attribute,
	uint32_t info_size, void* info)
{
	uint32_t size_expected;
	nv_soc_hwpm_device_int* dev_int = to_device_int(device.handle);

	if (dev_int == NULL) {
		log_error("Invalid device handle: %llu\n", device.handle);
		return -EINVAL;
	}

	if (info == NULL) {
		log_error("Invalid info param\n");
		return -EINVAL;
	}

	memset(info, 0, info_size);

	switch (attribute) {
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_CHIP_ID:
		size_expected = sizeof(tegra_soc_hwpm_chip_id);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((tegra_soc_hwpm_chip_id*)info) =
			dev_int->dev_info.soc_chip_id;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_REVISION:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->dev_info.soc_revision;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM:
		size_expected = sizeof(tegra_soc_hwpm_platform);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((tegra_soc_hwpm_platform*)info) =
			dev_int->dev_info.soc_platform;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_SOCKET:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->dev_info.soc_socket;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->dev_info.ip_available_count;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_LIST:
		size_expected = sizeof(nv_soc_hwpm_ip) * dev_int->dev_info.ip_available_count;
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		memcpy(info, dev_int->dev_info.ip_available_list, size_expected);
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT:
		if (info_size < sizeof(uint32_t)) {
			log_error("Invalid info size. Expected %d, got %d\n",
				sizeof(uint32_t), info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->dev_info.res_available_count;
		break;
	case NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST:
		size_expected =
			sizeof(nv_soc_hwpm_resource) * dev_int->dev_info.res_available_count;
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		memcpy(info, dev_int->dev_info.res_available_list, size_expected);
		break;
	default:
		log_error("Invalid attribute: %d\n", attribute);
		return -EINVAL;
	}

	return 0;
}

int nv_soc_hwpm_ip_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_ip ip,
	nv_soc_hwpm_ip_attribute attribute, uint32_t info_size, void* info)
{
	uint32_t size_expected;
	nv_soc_hwpm_device_int* dev_int = to_device_int(device.handle);

	if (dev_int == NULL) {
		log_error("Invalid device handle: %llu\n", device.handle);
		return -EINVAL;
	}

	if (info == NULL) {
		log_error("Invalid info param\n");
		return -EINVAL;
	}

	if (ip >= NV_SOC_HWPM_NUM_IPS) {
		log_error("Invalid IP ID: %d\n", ip);
		return -EINVAL;
	}

	memset(info, 0, info_size);

	switch (attribute) {
	case NV_SOC_HWPM_IP_ATTRIBUTE_IS_AVAILABLE:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->ip_info[ip].is_available;
		break;
	case NV_SOC_HWPM_IP_ATTRIBUTE_INST_MAX_COUNT:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->ip_info[ip].inst_max_count;
		break;
	case NV_SOC_HWPM_IP_ATTRIBUTE_FS_MASK:
		/* Get the minimal byte size to fit the fs_mask. */
		size_expected =
			ALIGN_UP(dev_int->ip_info[ip].inst_max_count, 8) / 8;
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		memcpy(info, dev_int->ip_info[ip].fs_mask, size_expected);
		break;
	default:
		log_error("Invalid attribute: %d\n", attribute);
		return -EINVAL;
	}

	return 0;
}

int nv_soc_hwpm_resource_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_resource resource,
	nv_soc_hwpm_resource_attribute attribute, uint32_t info_size, void* info)
{
	uint32_t size_expected;
	nv_soc_hwpm_device_int* dev_int = to_device_int(device.handle);

	if (dev_int == NULL) {
		log_error("Invalid device handle: %llu\n", device.handle);
		return -EINVAL;
	}

	if (info == NULL) {
		log_error("Invalid info param\n");
		return -EINVAL;
	}

	if (resource >= NV_SOC_HWPM_NUM_RESOURCES) {
		log_error("Invalid resource ID: %d\n", resource);
		return -EINVAL;
	}

	memset(info, 0, info_size);

	switch (attribute) {
	case NV_SOC_HWPM_RESOURCE_ATTRIBUTE_IS_AVAILABLE:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) =
			dev_int->res_info[resource].is_available;
		break;
	default:
		log_error("Invalid attribute: %d\n", attribute);
		return -EINVAL;
	}

	return 0;
}

int nv_soc_hwpm_session_alloc(
        nv_soc_hwpm_device device, nv_soc_hwpm_session *session)
{
	uint32_t session_idx;
	int fd, ret;

	if (session == NULL) {
		log_error("Invalid null session\n");
		return -EINVAL;
	}

	nv_soc_hwpm_device_int* dev_int = to_device_int(device.handle);
	if (dev_int == NULL) {
		return -EINVAL;
	}

	/* Find empty session slot. */
	for (session_idx = 0; session_idx < dev_int->session_count_max; ++session_idx) {
		if (!dev_int->session_int[session_idx]) {
			break;
		}
	}

	if (session_idx >= dev_int->session_count_max) {
		log_error("Maximum session count is reached, dev: %s\n",
			dev_int->dev_path);
		return -ENOMEM;
	}

	/* Open connection to driver. */
	fd = nv_soc_hwpm_os_open_hwpm_device(dev_int->dev_path);
	if (fd < 0) {
		log_error("Failed opening HWPM device: %s, errno: %s\n",
			dev_int->dev_path, strerror(errno));
		ret = -errno;
		return ret;
	}

	nv_soc_hwpm_session_int* session_int = malloc(sizeof(nv_soc_hwpm_session_int));
	memset(session_int, 0, sizeof(nv_soc_hwpm_session_int));
	session_int->fd = fd;
	session_int->dev_int = (void*)dev_int;
	session_int->info.device = device;

	dev_int->session_int[session_idx] = session_int;
	dev_int->session_count += 1;

	session->handle = to_session_handle(to_dev_idx(device.handle), session_idx);
	return 0;
}

static void update_resource_reservation_status(
	nv_soc_hwpm_session_int* session_int, uint8_t is_reserved)
{
	uint32_t i;
	nv_soc_hwpm_device_int* dev_int = (void*)session_int->dev_int;
	nv_soc_hwpm_resource_info *res_info = dev_int->res_info;
	nv_soc_hwpm_session_info *session_info = &session_int->info;

	for (i = 0; i < session_info->resource_count; ++i) {
		const uint32_t res_id = session_info->resource_ids[i];
		res_info[res_id].is_reserved = is_reserved;
	}
}

int nv_soc_hwpm_session_free(
	nv_soc_hwpm_session session)
{
	nv_soc_hwpm_device_int* dev_int;
	nv_soc_hwpm_session_int *session_int;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return 0;
	}

	/* Mark the resources as unreserved. */
	update_resource_reservation_status(session_int, 0);

	/* Release the buffers.*/
	nv_soc_hwpm_os_session_free_pma(session_int);

	/* Close driver connection. */
	nv_soc_hwpm_os_close_hwpm_device(session_int->fd);

	/* Cleanup session object. */
	dev_int = (void*)session_int->dev_int;
	const uint32_t session_idx = to_session_idx(session.handle);

	free(session_int);
	dev_int->session_int[session_idx] = NULL;
	dev_int->session_count -= 1;

	return 0;
}

int nv_soc_hwpm_session_get_info(
	nv_soc_hwpm_session session, nv_soc_hwpm_session_attribute attribute,
	uint32_t info_size, void* info)
{
	uint32_t size_expected;
	nv_soc_hwpm_session_int *session_int;
	
	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		log_error("Invalid session handle: %llu\n", session.handle);
		return -EINVAL;
	}

	if (info == NULL) {
		log_error("Invalid info param\n");
		return -EINVAL;
	}

	memset(info, 0, info_size);

	switch (attribute) {
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_DEVICE:
		size_expected = sizeof(nv_soc_hwpm_device);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		memcpy(info, &session_int->info.device, size_expected);
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_SESSION_STARTED:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) = session_int->info.is_session_started;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_BUFFER_ALLOCATED:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) = session_int->info.is_pma_buffer_allocated;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE:
		size_expected = sizeof(size_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((size_t*)info) = session_int->info.record_buffer.size;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA:
		size_expected = sizeof(void*);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((void**)info) = session_int->info.record_buffer.buffer;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA:
		size_expected = sizeof(uint64_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint64_t*)info) = session_int->info.record_buffer.pma_va;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_HANDLE:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) = session_int->info.record_buffer.handle;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_SIZE:
		size_expected = sizeof(size_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((size_t*)info) = session_int->info.mem_bytes_buffer.size;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA:
		size_expected = sizeof(void*);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((void**)info) = session_int->info.mem_bytes_buffer.buffer;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_PMA_VA:
		size_expected = sizeof(uint64_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint64_t*)info) = session_int->info.mem_bytes_buffer.pma_va;
		break;
	case NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_HANDLE:
		size_expected = sizeof(uint32_t);
		if (info_size < size_expected) {
			log_error("Invalid info size. Expected %d, got %d\n",
				size_expected, info_size);
			return -EINVAL;
		}
		*((uint32_t*)info) = session_int->info.mem_bytes_buffer.handle;
		break;
	default:
		log_error("Invalid attribute: %d\n", attribute);
		return -EINVAL;
	}

	return 0;
}

int nv_soc_hwpm_session_reserve_resources(
        nv_soc_hwpm_session session, uint32_t res_count,
        const nv_soc_hwpm_resource *res_ids)
{
	int ret;
	uint32_t i;
	nv_soc_hwpm_device_int* dev_int;
	nv_soc_hwpm_resource_info* res_info;
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	if (res_count == 0 ||
		res_count > NV_SOC_HWPM_NUM_RESOURCES ||
		res_ids == NULL) {
		log_error("Invalid resource count: %u or resource id array: %p\n",
			res_count, res_ids);
		return -EINVAL;
	}

	/* Check if the session is already started. */
	session_info = &session_int->info;
	if (session_info->is_session_started) {
		log_error("Unable to reserve resources. Session is already started\n");
		return -EINVAL;
	}

	dev_int = (void*)session_int->dev_int;
	res_info = dev_int->res_info;

	/* Check the status of the requested resource(s) */
	for (i = 0; i < res_count; ++i) {
		const uint32_t res_id = res_ids[i];
		if (res_id >= NV_SOC_HWPM_NUM_RESOURCES) {
			log_error("Invalid resource id: %u on index: %u\n",
				res_id, i);
			return -EINVAL;
		}

		if (res_info[res_id].is_available != 1) {
			log_error("Resource id: %u is not available\n", res_id);
			return -EINVAL;
		}
	}

	/* Make the request to the driver to reserve the resources. */
	for (i = 0; i < res_count; ++i) {
		const uint32_t res_id = res_ids[i];
		ret = nv_soc_hwpm_os_session_reserve_resources(session_int, res_id);
		if (ret) {
			log_error("Failed to reserve resource id: %u\n", res_id);
			return ret;
		}
	}

	/* Update the session's resource list. */
	memset(session_info->resource_ids, 0, sizeof(session_info->resource_ids));
	memcpy(session_info->resource_ids, res_ids, res_count * sizeof(*session_info->resource_ids));
	session_info->resource_count = res_count;

	/* Update the reservation status of the resources. */
	update_resource_reservation_status(session_int, 1);
	session_info->is_resource_reserved = 1;

	return 0;
}

int nv_soc_hwpm_session_reserve_all_resources(
        nv_soc_hwpm_session session)
{
	nv_soc_hwpm_device_int* dev_int;
	const nv_soc_hwpm_device_info* dev_info;

	dev_int = to_device_int(session.handle);
	if (dev_int == NULL) {
		return -EINVAL;
	}

	dev_info = &dev_int->dev_info;
	return nv_soc_hwpm_session_reserve_resources(
		session, dev_info->res_available_count, dev_info->res_available_list);
}

int nv_soc_hwpm_session_alloc_pma(
        nv_soc_hwpm_session session,
        const nv_soc_hwpm_pma_buffer_params *record_buffer_params)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	if (record_buffer_params == NULL) {
		log_error("Invalid record buffer param: %p\n", record_buffer_params);
		return -EINVAL;
	}

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (session_info->is_session_started) {
		log_error("Session is already started\n");
		return -EINVAL;
	}

	if (session_info->is_pma_buffer_allocated == 1) {
		log_error("PMA buffers are already allocated\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_alloc_pma(session_int, record_buffer_params);
}

int nv_soc_hwpm_session_set_get_pma_state(
	nv_soc_hwpm_session session, nv_soc_hwpm_pma_channel_state_params* param)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (!session_info->is_session_started) {
		log_error("Session is not started\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_set_get_pma_state(session_int, param);
}

int nv_soc_hwpm_session_get_hs_credits(
	nv_soc_hwpm_session session,
	tegra_soc_hwpm_get_type type,
	uint32_t* num_hs_credits)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (!session_info->is_session_started) {
		log_error("Session is not started\n");
		return -EINVAL;
	}

	if (num_hs_credits == NULL) {
		log_error("Invalid null num_hs_credits\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_get_hs_credits(
		session_int, type, num_hs_credits);
}

int nv_soc_hwpm_session_config_hs_credits(
	nv_soc_hwpm_session session,
	uint32_t param_count,
	const nv_soc_hwpm_config_hs_credit_params* params)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (!session_info->is_session_started) {
		log_error("Session is not started\n");
		return -EINVAL;
	}

	if (param_count == 0) {
		log_error("Invalid 0 param_count\n");
		return -EINVAL;
	}

	if (params == NULL) {
		log_error("Invalid null params\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_config_hs_credits(
		session_int, param_count, params);
}

int nv_soc_hwpm_session_start(nv_soc_hwpm_session session)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (session_info->is_session_started) {
		log_error("Session is already started\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_start(session_int);
}

int nv_soc_hwpm_session_regops(
	nv_soc_hwpm_session session,
	const size_t param_count,
	nv_soc_hwpm_reg_ops_params* params,
	nv_soc_hwpm_reg_ops_validation_mode mode,
	int* all_reg_ops_passed)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	if (param_count == 0) {
		log_error("Invalid 0 param_count\n");
		return -EINVAL;
	}

	if (params == NULL) {
		log_error("Invalid null params\n");
		return -EINVAL;
	}

	if (mode >= NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_COUNT) {
		log_error("Invalid mode: %u\n", mode);
		return -EINVAL;
	}

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (!session_info->is_session_started) {
		log_error("Session is not started\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_regops(
		session_int, param_count, params, mode, all_reg_ops_passed);
}

int nv_soc_hwpm_session_setup_trigger(
	nv_soc_hwpm_session session,
	int enable_cross_trigger,
	nv_soc_hwpm_trigger_session_type session_type)
{
	nv_soc_hwpm_session_int *session_int;
	nv_soc_hwpm_session_info *session_info;

	if (session_type >= NV_SOC_HWPM_TRIGGER_SESSION_TYPE_COUNT) {
		log_error("Invalid trigger session type: %u\n", session_type);
		return -EINVAL;
	}

	session_int = to_session_int(session.handle);
	if (session_int == NULL) {
		return -EINVAL;
	}

	session_info = &session_int->info;
	if (!session_info->is_session_started) {
		log_error("Session is not started\n");
		return -EINVAL;
	}

	return nv_soc_hwpm_os_session_setup_trigger(
		session_int, enable_cross_trigger, session_type);
}

#ifdef __cplusplus
} /* extern "C" */
#endif