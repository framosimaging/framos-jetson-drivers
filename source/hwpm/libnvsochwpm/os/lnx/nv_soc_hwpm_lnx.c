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

#if !defined (__QNX__)

#include <linux/dma-heap.h>
#include <linux/dma-buf.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uapi/linux/tegra-soc-hwpm-uapi.h"
#include "os/nv_soc_hwpm_os.h"
#include "common/log.h"

#ifdef __cplusplus
extern "C" {
#endif

static int get_device_info(int fd, nv_soc_hwpm_device_info* dev_info)
{
	struct tegra_soc_hwpm_device_info params;
	int ret;
	ret = ioctl(fd, TEGRA_CTRL_CMD_SOC_HWPM_DEVICE_INFO, &params);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_SOC_HWPM_DEVICE_INFO, ret: %d, errno: %s\n",
			ret,
			strerror(errno));
		ret = -EIO;
		return ret;
	}

	dev_info->soc_chip_id = (params.chip << 4) | params.chip_revision;
	dev_info->soc_revision = params.revision;
	dev_info->soc_platform = params.platform;
	dev_info->soc_socket = 0; /* TODO: only single socket support for now. */

	return ret;
}

static int get_ip_floorsweep_info(
	int fd, nv_soc_hwpm_ip_info* ip_info, uint32_t* ip_available_count,
	nv_soc_hwpm_ip* ip_available_list)
{
	uint32_t ip, available_count, query_count, q;
	int ret = 0;

	/* Query all IPs. */
	ip = 0;
	available_count = 0;
	while (ip < TERGA_SOC_HWPM_NUM_IPS) {
		struct tegra_soc_hwpm_ip_floorsweep_info param = {};
		query_count = MIN((TERGA_SOC_HWPM_NUM_IPS - ip),
					TEGRA_SOC_HWPM_IP_QUERIES_MAX);
		param.num_queries = query_count;
		for (q = 0; q < query_count; ++q) {
			param.ip_fsinfo[q].ip = ip + q;
			param.ip_fsinfo[q].ip_inst_mask = 0;
		}

		ret = ioctl(fd, TEGRA_CTRL_CMD_SOC_HWPM_IP_FLOORSWEEP_INFO,
			&param);
		if (ret) {
			log_error(
				"Failed TEGRA_CTRL_CMD_SOC_HWPM_IP_FLOORSWEEP_INFO, ip: %d, ret: %d, errno: %s\n",
				ip, ret, strerror(errno));
			ret = -EIO;
			return ret;
		}

		for (q = 0; q < query_count; ++q, ++ip) {
			ip_info[ip].is_available =
				(param.ip_fsinfo[q].status ==
				 TEGRA_SOC_HWPM_IP_STATUS_VALID) ? 1 : 0;
			*((uint64_t*)ip_info[ip].fs_mask) = param.ip_fsinfo[q].ip_inst_mask;

			if (ip_info[ip].is_available)
				ip_available_list[available_count++] = ip;
		}
	}

	*ip_available_count = available_count;

	return ret;
}

/* TODO: get the actual IP instance/element max count. */
static uint32_t get_ip_max_instances(
	tegra_soc_hwpm_chip_id chip_id, nv_soc_hwpm_ip ip)
{
	switch (chip_id) {
	case TEGRA_SOC_HWPM_CHIP_ID_T241:
		switch (ip) {
		case NV_SOC_HWPM_IP_MSS_CHANNEL:
			return 64;
		case NV_SOC_HWPM_IP_PCIE_XALRC:
		case NV_SOC_HWPM_IP_PCIE_XTLRC:
		case NV_SOC_HWPM_IP_PCIE_XTLQ:
			return 4;
		default:
			break;
		}
		break;
	case TEGRA_SOC_HWPM_CHIP_ID_T410:
		switch (ip) {
		case NV_SOC_HWPM_IP_CPU:
			return 32;
		case NV_SOC_HWPM_IP_CPU_EXT_0:
			return 32;
		case NV_SOC_HWPM_IP_CPU_EXT_1:
			return 32;
		case NV_SOC_HWPM_IP_CPU_EXT_2:
			return 2;
		case NV_SOC_HWPM_IP_NVTHERM:
			return 1;
		case NV_SOC_HWPM_IP_CSN:
			return 32;
		case NV_SOC_HWPM_IP_CSN_EXT_0:
			return 10;
		case NV_SOC_HWPM_IP_CSNH:
			return 7;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return 0;
}

static int get_resource_info(
	int fd, nv_soc_hwpm_resource_info* res_info, uint32_t* res_available_count,
	nv_soc_hwpm_resource* res_available_list)
{
	uint32_t res, available_count, query_count, q;
	int ret = 0;

	/* Query all resources. */
	res = 0;
	available_count = 0;
	while (res < TERGA_SOC_HWPM_NUM_RESOURCES) {
		struct tegra_soc_hwpm_resource_info param = {};
		query_count = MIN((TERGA_SOC_HWPM_NUM_RESOURCES - res),
				TEGRA_SOC_HWPM_RESOURCE_QUERIES_MAX);
		param.num_queries = query_count;
		for (q = 0; q < query_count; ++q) {
			param.resource_info[q].resource = res + q;
			param.resource_info[q].status = TEGRA_SOC_HWPM_RESOURCE_STATUS_INVALID;
		}

		ret = ioctl(fd, TEGRA_CTRL_CMD_SOC_HWPM_RESOURCE_INFO, &param);
		if (ret) {
			log_error(
				"Failed TEGRA_CTRL_CMD_SOC_HWPM_RESOURCE_INFO, res: %d, ret: %d, errno: %s\n",
				res, ret, strerror(errno));
			ret = -EIO;
			return ret;
		}

		for (q = 0; q < query_count; ++q, ++res) {
			res_info[res].is_available =
				(param.resource_info[q].status ==
				 TEGRA_SOC_HWPM_RESOURCE_STATUS_VALID) ? 1 : 0;

			if (res_info[res].is_available)
				res_available_list[available_count++] = res;
		}
	}

	*res_available_count = available_count;

	return ret;
}

int nv_soc_hwpm_os_open_hwpm_device(const char* dev_path)
{
	int fd;

	fd = open(dev_path, O_RDWR);
	return fd;
}

int nv_soc_hwpm_os_close_hwpm_device(int fd)
{
	close(fd);
	return 0;
}

static int enumerate_socket(uint32_t socket, nv_soc_hwpm_device_int* device_int)
{
	int fd, ret = 0;
	uint32_t i;
  	const char* hwpm_path;

	hwpm_path = TEGRA_SOC_HWPM_DEV_NODE;
	fd = nv_soc_hwpm_os_open_hwpm_device(hwpm_path);
	if (fd < 0) {
		log_error("Failed opening HWPM socket: %u, device: %s, errno: %s\n",
			socket, hwpm_path, strerror(errno));
		ret = -errno;
		goto done;
	}

	nv_soc_hwpm_device_info* dev_info = &device_int->dev_info;
	ret = get_device_info(fd, dev_info);
	if (ret) {
		log_error("Failed get_device_info, dev: %s\n", hwpm_path);
		goto fail;
	}

	nv_soc_hwpm_ip_info* ip_info = device_int->ip_info;
	ret = get_ip_floorsweep_info(fd, ip_info, &dev_info->ip_available_count,
		dev_info->ip_available_list);
	if (ret) {
		log_error("Failed get_ip_floorsweep_info, dev: %s\n", hwpm_path);
		goto fail;
	}

	for (i = 0; i < dev_info->ip_available_count; i++) {
		nv_soc_hwpm_ip cur_ip = dev_info->ip_available_list[i];
		ip_info[cur_ip].inst_max_count =
			get_ip_max_instances(dev_info->soc_chip_id, cur_ip);
	}

	nv_soc_hwpm_resource_info* res_info = device_int->res_info;
	ret = get_resource_info(fd, res_info, &dev_info->res_available_count,
		dev_info->res_available_list);
	if (ret) {
		log_error("Failed get_resource_info, dev: %s\n", hwpm_path);
		goto fail;
	}

	device_int->dev_path = hwpm_path;
	device_int->session_count_max = 1; /* TODO: only single session now */
	device_int->session_count = 0;
	memset(device_int->session_int, 0, sizeof(device_int->session_int));

	if (device_int->session_count_max > MAX_SESSION_COUNT) {
		log_error("Session count max: %u is greater than MAX_SESSION_COUNT: %u\n",
			device_int->session_count_max, MAX_SESSION_COUNT);
		ret = -EINVAL;
		goto fail;
	}

fail:
	nv_soc_hwpm_os_close_hwpm_device(fd);

done:
	return ret;
}

/**
 * @brief Enumerate NV_SOC_HWPM device in the system.
 *
 * @return 0 if successful.
 *
 */
int nv_soc_hwpm_os_enumerate_device(
	uint32_t max_count, uint32_t* actual_count,
	nv_soc_hwpm_device_int* devices_int)
{
	int ret = 0;
	uint32_t socket;

	memset(devices_int, 0, sizeof(nv_soc_hwpm_device_int) * max_count);

	/**
	 * TODO: only single socket support for now.
	 */
	socket = 0;
	ret = enumerate_socket(socket, &devices_int[socket]);
	if (ret) {
		log_error("Failed enumerate device on socket: %u, ret: %d\n", socket, ret);
		goto fail;
	}

	*actual_count = 1;
	goto done;

fail:
	*actual_count = 0;

done:
	return ret;
}

int nv_soc_hwpm_os_session_reserve_resources(
        nv_soc_hwpm_session_int* session_int, nv_soc_hwpm_resource res_id)
{
	int ret;

	struct tegra_soc_hwpm_reserve_resource params = {};
	params.resource = (enum tegra_soc_hwpm_resource)res_id;
	ret = ioctl(session_int->fd,
		TEGRA_CTRL_CMD_SOC_HWPM_RESERVE_RESOURCE,
		&params);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_SOC_HWPM_RESERVE_RESOURCE res id: %u, ret: %d, "
			"errno: %s\n",
			params.resource,
			ret,
			strerror(errno));
		return -EIO;
	}

	return 0;
}

static int unmap_dma_heap(int handle, void *va, size_t size)
{
    int ret;

    if (!handle || !va || !size) {
	log_error("Invalid handle: %d, va: %p, size: %llu\n", handle, va, size);
	return -EINVAL;
    }

    ret = munmap(va, size);
    if (ret < 0) {
        log_error("munmap failed : %s\n", strerror(errno));
        return -EINVAL;
    }

    return 0;
}

static int alloc_dma_heap(
	const char* heap_path,
	size_t size,
	tegra_soc_hwpm_coherency_type coherency,
	uint32_t flags,
	void* cpu_va_user,
	void** cpu_va,
	uint32_t* mem_handle,
	uint32_t* mem_fd,
	uint32_t* heap_fd)
{
	int ret, fd;

	/* Coherency flag not used in DMA heap implementation. */
	(void)coherency;
	(void)flags;

	if (size == 0 || cpu_va == NULL || mem_handle == NULL || mem_fd == NULL || heap_fd == NULL) {
		log_error("Invalid size: %llu, cpu_va: %p, mem_handle: %p, mem_fd: %p, heap_fd: %p\n",
			size, cpu_va, mem_handle, mem_fd, heap_fd);
		return -EINVAL;
	}
	
	*heap_fd = 0;
	fd = open(heap_path, O_RDWR | O_SYNC | O_CLOEXEC);
	if (fd < 0) {
		log_error("open %s failed [%s]\n", heap_path, strerror(errno));
		return -ENOMEM;
	}

	struct dma_heap_allocation_data data = {
		.len = size,
		.fd = 0,
		.fd_flags = O_RDWR | O_CLOEXEC,
		.heap_flags = 0,
	};

	ret = ioctl(fd, DMA_HEAP_IOCTL_ALLOC, &data);
	if (ret) {
		log_error("dma heap Alloc failed [%s]\n", strerror(errno));
		ret = -ENOMEM;
		goto error;
	}

	*mem_handle = (uint32_t)data.fd;

	*cpu_va = mmap(*cpu_va,
			size,
			(PROT_READ | PROT_WRITE),
			MAP_SHARED,
			data.fd,
			0);
	if (*cpu_va == MAP_FAILED) {
		log_error("mmap failed : %s\n", strerror(errno));
		*cpu_va = NULL;
		ret = -ENOMEM;
		goto error;
	}

	memset(*cpu_va, 0, size);

	if ((cpu_va_user != NULL) && (*cpu_va != cpu_va_user)) {
		ret = -EINVAL;
		goto error_unmap;
	}

	/* Retrieve memHandle's FD */
        *mem_fd = data.fd;
	*heap_fd = fd;

	return 0;

error_unmap:
	unmap_dma_heap(data.fd, *cpu_va, size);

error:
	close(fd);

	return ret;
}

static int free_dma_heap(
	uint64_t mem_handle, void *cpu_va, size_t size, uint32_t heap_fd)
{
	if ((cpu_va != NULL) && (mem_handle != 0)) {
		unmap_dma_heap((int)mem_handle, cpu_va, size);
	}

	if (mem_handle != 0) {
		close((int)mem_handle);
	}

	if (heap_fd != 0) {
		close(heap_fd);
	}

	return 0;
}

static int alloc_pma(
        nv_soc_hwpm_session_int* session_int,
	const char* heap_path,
        const nv_soc_hwpm_pma_buffer_params *record_buffer_params)
{
	int ret;
	nv_soc_hwpm_session_info *session_info = &session_int->info;

	/* Allocate PMA record buffer */

	void* record_buffer = NULL;
        uint32_t record_buffer_handle = 0;
        uint32_t record_buffer_fd = 0;
	uint32_t record_buffer_heap_fd = 0;
	const size_t record_buffer_size = record_buffer_params->size;
        void* record_buffer_cpu_va = record_buffer_params->cpu_va;
	tegra_soc_hwpm_coherency_type record_buffer_coherency = record_buffer_params->coherency_type;

	ret = alloc_dma_heap(
		heap_path,
		record_buffer_size,
		record_buffer_coherency,
		0, /* TODO: for other allocator may need to pass read/write property*/
		record_buffer_cpu_va,
		&record_buffer,
		&record_buffer_handle,
		&record_buffer_fd,
		&record_buffer_heap_fd);

        if (ret) {
		log_error("Failed allocating PMA buffer\n");
		goto error;
        }

	/* Allocate mem bytes buffer */

	void* mem_bytes_buffer = NULL;
        uint32_t mem_bytes_buffer_handle = 0;
        uint32_t mem_bytes_buffer_fd = 0;
	uint32_t mem_bytes_buffer_heap_fd = 0;
	const size_t mem_bytes_buffer_size = 4096; // ignore size passed by caller
	void* mem_bytes_buffer_cpu_va = NULL;
	tegra_soc_hwpm_coherency_type mem_bytes_buffer_coherency =
		record_buffer_coherency; // same with record buffer

        ret = alloc_dma_heap(
		heap_path,
		mem_bytes_buffer_size,
		mem_bytes_buffer_coherency,
		0, /* TODO: for other allocator may need to pass read/write property*/
		mem_bytes_buffer_cpu_va,
		&mem_bytes_buffer,
		&mem_bytes_buffer_handle,
		&mem_bytes_buffer_fd,
		&mem_bytes_buffer_heap_fd);

	if (ret) {
		log_error("Failed allocating mem bytes buffer\n");
		goto error_free_record;
        }

	struct tegra_soc_hwpm_alloc_pma_stream params = {};
        params.stream_buf_fd = record_buffer_fd;
        params.mem_bytes_buf_fd = mem_bytes_buffer_fd;
        params.stream_buf_size = record_buffer_size;

	/* Register the buffers to the driver */
        ret = ioctl(session_int->fd, TEGRA_CTRL_CMD_SOC_HWPM_ALLOC_PMA_STREAM, &params);
        if (ret) {
            log_error("Failed TEGRA_CTRL_CMD_SOC_HWPM_ALLOC_PMA_STREAM with errno=%s.\n",
	    	strerror(errno));
            ret = -EIO;
	    goto error_free_mem_bytes;
        }

        session_info->record_buffer.buffer = (uint8_t*)record_buffer;
        session_info->record_buffer.size = record_buffer_size;
        session_info->record_buffer.pma_va = params.stream_buf_pma_va;
        session_info->record_buffer.handle = record_buffer_handle;
	session_info->record_buffer.heap_fd = record_buffer_heap_fd;

        session_info->mem_bytes_buffer.buffer = (uint8_t*)mem_bytes_buffer;
        session_info->mem_bytes_buffer.size = mem_bytes_buffer_size;
        session_info->mem_bytes_buffer.pma_va = 0; /* TODO: currently, kernel doesn't provide this. */
        session_info->mem_bytes_buffer.handle = mem_bytes_buffer_handle;
	session_info->mem_bytes_buffer.heap_fd = mem_bytes_buffer_heap_fd;

	session_info->is_pma_buffer_allocated = 1;

	return 0;

error_free_mem_bytes:
	free_dma_heap(
		mem_bytes_buffer_handle, mem_bytes_buffer, mem_bytes_buffer_size,
		mem_bytes_buffer_heap_fd);

error_free_record:
	free_dma_heap(
		record_buffer_handle, record_buffer, record_buffer_size,
		record_buffer_heap_fd);

error:
	return ret;
}

static int path_exists(const char* filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0) ? 1 : 0;
}

int nv_soc_hwpm_os_session_alloc_pma(
        nv_soc_hwpm_session_int* session_int,
        const nv_soc_hwpm_pma_buffer_params *record_buffer_params)
{
	static const uint32_t kHeapPathCount = 3;
	static const char* kHeapPath[] = {
		"/dev/dma_heap/cma",
		"/dev/dma_heap/reserved",
		"/dev/dma_heap/system",
	};

	int ret;

	for (uint32_t i = 0; i < kHeapPathCount; i++) {
		log_debug("Trying to allocate PMA buffer from %s\n", kHeapPath[i]);
		if (path_exists(kHeapPath[i]) == 0) {
			log_debug("Heap %s does not exist\n", kHeapPath[i]);
			continue;
		}

		ret = alloc_pma(session_int, kHeapPath[i], record_buffer_params);
		if (!ret)
			return ret;
	}

	return ret;
}

void nv_soc_hwpm_os_session_free_pma(nv_soc_hwpm_session_int* session_int)
{
	nv_soc_hwpm_session_info *session_info = &session_int->info;

	/* Cleanup PMA buffer. */
	if (session_info->record_buffer.handle != 0) {
		free_dma_heap(
			session_info->record_buffer.handle,
			session_info->record_buffer.buffer,
			session_info->record_buffer.size,
			session_info->record_buffer.heap_fd);
	}

	if (session_info->mem_bytes_buffer.handle != 0) {
		free_dma_heap(
			session_info->mem_bytes_buffer.handle,
			session_info->mem_bytes_buffer.buffer,
			session_info->mem_bytes_buffer.size,
			session_info->mem_bytes_buffer.heap_fd);
	}
}

int nv_soc_hwpm_os_session_set_get_pma_state(
	nv_soc_hwpm_session_int *session_int,
	nv_soc_hwpm_pma_channel_state_params* params)
{
	int ret;
	struct tegra_soc_hwpm_update_get_put io_params = {};

        io_params.mem_bump = params->in_mem_bump;

        if (params->in_check_overflow)
        {
            io_params.b_check_overflow = 1;
        }

        if (params->in_read_mem_head)
        {
            io_params.b_read_mem_head = true;
        }

	io_params.b_stream_mem_bytes = (params->in_stream_mem_bytes != 0);

	ret = ioctl(session_int->fd,
			TEGRA_CTRL_CMD_SOC_HWPM_UPDATE_GET_PUT,
			&io_params);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_SOC_HWPM_UPDATE_GET_PUT, ret: %d, errno: %s\n",
			ret, strerror(errno));
		return -EIO;
	}

	if (params->in_check_overflow)
        {
            params->out_overflowed = !!io_params.b_overflowed;
        }

        if (params->in_read_mem_head)
        {
            params->out_mem_head = io_params.mem_head;
        }

	return 0;
}

int nv_soc_hwpm_os_session_get_hs_credits(
	nv_soc_hwpm_session_int* session_int,
	tegra_soc_hwpm_get_type type,
	uint32_t* num_hs_credits)
{
	int ret;
	uint16_t credit_cmd;

	switch (type) {
	case TEGRA_SOC_HWPM_GET_TYPE_HS_CREDITS:
		credit_cmd = TEGRA_SOC_HWPM_CMD_GET_HS_CREDITS;
		break;
	case TEGRA_SOC_HWPM_GET_TYPE_TOTAL_HS_CREDITS:
		credit_cmd = TEGRA_SOC_HWPM_CMD_GET_TOTAL_HS_CREDITS;
		break;
	default:
		log_error("Invalid credit type: %u\n", type);
		return -EINVAL;
	}

	struct tegra_soc_hwpm_exec_credit_program params = { 0 };

	params.num_entries = 1;
        params.credit_cmd = credit_cmd;
        params.credit_info[0].num_credits = ~0;

	ret = ioctl(session_int->fd,
			TEGRA_CTRL_CMD_SOC_HWPM_CREDIT_PROGRAM,
			&params);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_SOC_HWPM_CREDIT_PROGRAM, ret: %d, errno: %s\n",
			ret, strerror(errno));
		return -EIO;
	}

	/* Total credits are available in first entry */
        *num_hs_credits = params.credit_info[0].num_credits;

	return 0;
}

int nv_soc_hwpm_os_session_config_hs_credits(
	nv_soc_hwpm_session_int* session_int,
	uint32_t param_count,
	const nv_soc_hwpm_config_hs_credit_params* params)
{
	int ret;
	uint32_t i;

	i = 0;
	while (i < param_count)
        {
		/* TODO set params.pmaChannelIdx for multi-channel support (currently 0) */
		uint32_t b, batch_count;
		struct tegra_soc_hwpm_exec_credit_program io_params = { 0 };

		batch_count = MIN((param_count - i),
				TEGRA_SOC_HWPM_MAX_CREDIT_INFO_ENTRIES);

		io_params.credit_cmd = TEGRA_SOC_HWPM_CMD_SET_HS_CREDITS;
		io_params.num_entries = batch_count;

		for (b = 0; b < batch_count; ++b) {
			const uint32_t param_idx = i + b;;
			io_params.credit_info[b].cblock_idx = params[param_idx].cblock_idx;
			io_params.credit_info[b].num_credits = params[param_idx].num_credits_per_chiplet;
		}

		ret = ioctl(session_int->fd,
			TEGRA_CTRL_CMD_SOC_HWPM_CREDIT_PROGRAM,
			&io_params);
		if (ret) {
			log_error(
				"Failed TEGRA_CTRL_CMD_SOC_HWPM_CREDIT_PROGRAM, ret: %d, errno: %s\n",
				ret, strerror(errno));
			return -EIO;
		}

		/* TODO get info regarding failed credit programming from params.statusInfo (currently unused) */

		i += batch_count;
	}

	return 0;
}

int nv_soc_hwpm_os_session_start(nv_soc_hwpm_session_int* session_int)
{
	int ret;
	nv_soc_hwpm_session_info *session_info;

	ret = ioctl(session_int->fd, TEGRA_CTRL_CMD_BIND);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_BIND, ret: %d, errno: %s\n",
			ret, strerror(errno));
		return -EIO;
	}

	session_info = &session_int->info;
	session_info->is_session_started = 1;

	return 0;
}

int nv_soc_hwpm_os_session_regops(
	nv_soc_hwpm_session_int* session_int,
	const size_t param_count,
	nv_soc_hwpm_reg_ops_params* params,
	nv_soc_hwpm_reg_ops_validation_mode mode,
	int* all_reg_ops_passed)
{
	int ret, all_passed;
	uint32_t i;

	all_passed = 1;
	i = 0;
	while (i < param_count)
        {
		/* TODO set params.pmaChannelIdx for multi-channel support (currently 0) */
		uint32_t b, batch_count;
		struct tegra_soc_hwpm_exec_reg_ops io_params = { 0 };

		batch_count = MIN((param_count - i),
				TEGRA_SOC_HWPM_REG_OPS_SIZE);

		io_params.mode = (uint8_t)mode;
		io_params.op_count = batch_count;

		for (b = 0; b < batch_count; ++b) {
			const uint32_t param_idx = i + b;;
			io_params.ops[b].phys_addr = params[param_idx].in_offset;
			io_params.ops[b].reg_val_lo = VAL_LO64(params[param_idx].in_out_val64);
			io_params.ops[b].reg_val_hi = VAL_HI64(params[param_idx].in_out_val64);
			io_params.ops[b].mask_lo = VAL_LO64(params[param_idx].in_mask64);
			io_params.ops[b].mask_hi = VAL_HI64(params[param_idx].in_mask64);
			io_params.ops[b].cmd = params[param_idx].in_cmd;
		}

		ret = ioctl(session_int->fd,
			TEGRA_CTRL_CMD_SOC_HWPM_EXEC_REG_OPS,
			&io_params);
		if (ret) {
			log_error(
				"Failed TEGRA_CTRL_CMD_SOC_HWPM_EXEC_REG_OPS, ret: %d, errno: %s\n",
				ret, strerror(errno));
			return -EIO;
		}

		all_passed &= !!io_params.b_all_reg_ops_passed;

		// Read back the result.
		for (b = 0; b < batch_count; ++b) {
			const uint32_t param_idx = i + b;
			params[param_idx].in_out_val64 =
				VAL_64(io_params.ops[b].reg_val_lo,
					io_params.ops[b].reg_val_hi);
			params[param_idx].out_status = io_params.ops[b].status;
		}

		i += batch_count;
	}

	if (all_reg_ops_passed != NULL)
            *all_reg_ops_passed = all_passed;

	return 0;
}

int nv_soc_hwpm_os_session_setup_trigger(
	nv_soc_hwpm_session_int* session_int,
	int enable_cross_trigger,
	nv_soc_hwpm_trigger_session_type session_type)
{
	int ret;

	/**
	 * TODO:
	 * set params.cblockIdx and params.pmaChannelIdx for multi-channel
	 * support (currently 0)
	 */
        struct tegra_soc_hwpm_setup_trigger params = { 0 };
	params.enable_cross_trigger = (uint8_t)enable_cross_trigger;
        params.session_type = (uint8_t)session_type;

	ret = ioctl(session_int->fd,
		TEGRA_CTRL_CMD_SOC_HWPM_SETUP_TRIGGER,
		&params);
	if (ret) {
		log_error(
			"Failed TEGRA_CTRL_CMD_SOC_HWPM_SETUP_TRIGGER, ret: %d, errno: %s\n",
			ret, strerror(errno));
		return -EIO;
	}

	return 0;

}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !__QNX__ */