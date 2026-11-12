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

#ifndef __TYPES_H__
#define __TYPES_H__

#include "nv_soc_hwpm.h"

#include "common/bit.h"

#define MAX_SOCKET_COUNT 1
#define MAX_DEVICE_COUNT 1
#define MAX_SESSION_COUNT 1
#define MAX_PATH_LENGTH 256
#define MAX_FS_MASK_SIZE 256

#define DEV_HANDLE_SHIFT 0U
#define DEV_HANDLE_MASK 0xFFULL /* Up to 256 HWPM devices */

#define SESSION_HANDLE_SHIFT 16U
#define SESSION_HANDLE_MASK 0xFF00ULL /* Up to 256 HWPM sessions per device */

static inline uint32_t to_dev_idx(uint64_t handle) {
	return FIELD_GET(handle, DEV_HANDLE_MASK, DEV_HANDLE_SHIFT);
}

static inline uint64_t to_dev_handle(uint32_t dev_idx) {
	uint64_t handle = FIELD_SET(dev_idx, DEV_HANDLE_MASK, DEV_HANDLE_SHIFT);
	return handle;
}

static inline uint32_t to_session_idx(uint64_t handle) {
	return FIELD_GET(handle, SESSION_HANDLE_MASK, SESSION_HANDLE_SHIFT);
}

static inline uint64_t to_session_handle(uint32_t dev_idx, uint32_t session_idx) {
	uint64_t handle = 0;
	handle |= FIELD_SET(dev_idx, DEV_HANDLE_MASK, DEV_HANDLE_SHIFT);
	handle |= FIELD_SET(session_idx, SESSION_HANDLE_MASK, SESSION_HANDLE_SHIFT);
	return handle;
}

/**
 * @brief Struct containing NV_SOC_HWPM allocated PMA buffer.
 */
typedef struct {
	size_t size;
	uint8_t* buffer;
	uint64_t pma_va;
	uint32_t handle;
	uint32_t heap_fd;
} nv_soc_hwpm_pma_buffer;

/**
 * @brief Struct containing NV_SOC_HWPM session attributes.
 */
typedef struct {
	nv_soc_hwpm_device device;

	nv_soc_hwpm_pma_buffer record_buffer;
	nv_soc_hwpm_pma_buffer mem_bytes_buffer;

	uint32_t resource_count;
	nv_soc_hwpm_resource resource_ids[NV_SOC_HWPM_NUM_RESOURCES];

	uint8_t is_resource_reserved;
	uint8_t is_pma_buffer_allocated;
	uint8_t is_session_started;
} nv_soc_hwpm_session_info;

/**
 * @brief Internal NV_SOC_HWPM session structure.
 */
typedef struct
{
	int fd;
	nv_soc_hwpm_session_info info;
	struct nv_soc_hwpm_device_int* dev_int;
} nv_soc_hwpm_session_int;

/**
 * @brief Struct containing NV_SOC_HWPM device attributes.
 */
typedef struct {
	uint32_t soc_chip_id;
	uint32_t soc_revision;
	uint32_t soc_platform;
	uint32_t soc_socket;

	uint32_t ip_available_count;
	nv_soc_hwpm_ip ip_available_list[NV_SOC_HWPM_NUM_IPS];

	uint32_t res_available_count;
	nv_soc_hwpm_resource res_available_list[NV_SOC_HWPM_NUM_RESOURCES];
} nv_soc_hwpm_device_info;

/**
 * @brief Struct containing NV_SOC_HWPM IP attributes.
 */
typedef struct {
	/* IP availability status. */
	uint8_t is_available;

	/* IP instance/element max count. */
	uint32_t inst_max_count;

	/* IP floorsweeping mask. */
	uint8_t fs_mask[MAX_FS_MASK_SIZE/sizeof(uint8_t)];
} nv_soc_hwpm_ip_info;

/**
 * @brief Struct containing NV_SOC_HWPM resource attributes.
 */
typedef struct {
	/* Resource availability status. */
	uint8_t is_available;

	/* Resource reservation status. */
	uint8_t is_reserved;
} nv_soc_hwpm_resource_info;

/**
 * @brief Internal NV_SOC_HWPM device structure for Linux.
 */
typedef struct
{
	const char* dev_path;

	nv_soc_hwpm_device_info dev_info;
	nv_soc_hwpm_ip_info ip_info[NV_SOC_HWPM_NUM_IPS];
	nv_soc_hwpm_resource_info res_info[NV_SOC_HWPM_NUM_RESOURCES];

	nv_soc_hwpm_session_int* session_int[MAX_SESSION_COUNT];
	uint32_t session_count;
	uint32_t session_count_max;
} nv_soc_hwpm_device_int;

nv_soc_hwpm_device_int* to_device_int(uint64_t handle);

nv_soc_hwpm_session_int* to_session_int(uint64_t handle);

#endif /*__TYPES_H__*/