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

#include "t241_test.h"
#include "soc_mode_e_buffer.h"
#include "common/register_util.h"
#include "th500/nv_ref_dev_perf.h"
#include "ip_names.h"
#include <unistd.h>

T241Tests::T241Tests() : NvSocHwpmTests(), t241_dev_count(0)
{
}

T241Tests::~T241Tests()
{
}

void T241Tests::SetUp()
{
	NvSocHwpmTests::SetUp();
	ASSERT_EQ(0, api_table.nv_soc_hwpm_init_fn());
}

void T241Tests::TearDown()
{
	api_table.nv_soc_hwpm_exit_fn();
	NvSocHwpmTests::TearDown();
}

void T241Tests::GetDevices()
{
	t241_dev_count = 0;

	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t241_dev_count, NULL));
	ASSERT_EQ(1U, t241_dev_count);

	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t241_dev_count, t241_dev));
}

TEST_F(T241Tests, EnumerateDevices)
{
	uint32_t chip_id, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		dev = t241_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_CHIP_ID;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(chip_id), &chip_id));
		ASSERT_EQ(TEGRA_SOC_HWPM_CHIP_ID_T241, chip_id);

	}
}

TEST_F(T241Tests, EnumerateDevicesNegative)
{
	t241_dev_count = 0;

	// Should fail with invalid dev_count ptr.
	ASSERT_NE(0, api_table.nv_soc_hwpm_get_devices_fn(NULL, t241_dev));

	// Get valid count.
	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t241_dev_count, NULL));
	ASSERT_EQ(1U, t241_dev_count);

	// Should fail with invalid buffer ptr.
	ASSERT_NE(0, api_table.nv_soc_hwpm_get_devices_fn(&t241_dev_count, NULL));
}

TEST_F(T241Tests, EnumerateIPs)
{
	uint32_t ip_count, i, j;
	uint64_t fs_mask;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_ip ip[NV_SOC_HWPM_NUM_IPS], cur_ip;
	nv_soc_hwpm_ip_attribute ip_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(ip_count), &ip_count));
		ASSERT_GT(ip_count, 0U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_LIST;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*ip) * ip_count, ip));

		for (j = 0; j < ip_count; j++) {
			cur_ip = ip[j];
			printf("\t%d - ip id = %u, name: %s", j, cur_ip, kIpNames[cur_ip]);

			ip_attr = NV_SOC_HWPM_IP_ATTRIBUTE_FS_MASK;

			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_ip_get_info_fn(
					dev, cur_ip, ip_attr, sizeof(fs_mask), &fs_mask));
			printf(", fs_mask = 0x%lx\n", fs_mask);
			ASSERT_GT(fs_mask, 0U);
		}
	}
}

TEST_F(T241Tests, EnumerateIPsNegative)
{
	uint32_t ip_count, i, j;
	uint64_t fs_mask;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_ip ip[NV_SOC_HWPM_NUM_IPS], cur_ip;
	nv_soc_hwpm_ip_attribute ip_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT;

		// Should fail with invalid buffer size.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(uint8_t), &ip_count));

		// Should fail with invalid buffer ptr.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(ip_count), NULL));

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(ip_count), &ip_count));
		ASSERT_GT(ip_count, 0U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_LIST;

		// Should fail with invalid buffer size.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(uint8_t) * ip_count, ip));
				
		// Should fail with invalid buffer ptr.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*ip) * ip_count, NULL));

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*ip) * ip_count, ip));

		for (j = 0; j < ip_count; j++) {
			cur_ip = ip[j];
			printf("\t%d - ip id = %u, name: %s", j, cur_ip, kIpNames[cur_ip]);

			// Should fail with invalid attribute.
			ip_attr = (nv_soc_hwpm_ip_attribute)0xffffffff;
			size_t dummy;
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_ip_get_info_fn(
					dev, cur_ip, ip_attr, sizeof(dummy), &dummy));

			ip_attr = NV_SOC_HWPM_IP_ATTRIBUTE_FS_MASK;

			// Should fail with invalid buffer size.
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_ip_get_info_fn(
					dev, cur_ip, ip_attr, sizeof(uint8_t), &fs_mask));

			// Should fail with invalid buffer ptr.
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_ip_get_info_fn(
					dev, cur_ip, ip_attr, sizeof(fs_mask), NULL));
		}
	}
	
}

TEST_F(T241Tests, EnumerateResources)
{
	uint32_t res_count, res_available, i, j;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES], cur_res;
	nv_soc_hwpm_resource_attribute res_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(res_count), &res_count));
		ASSERT_GT(res_count, 0U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*res) * res_count, res));

		for (j = 0; j < res_count; j++) {
			cur_res = res[j];
			printf("\t%d - res id = %u, name: %s",
				j, cur_res, kResourceNames[cur_res]);

			res_attr = NV_SOC_HWPM_RESOURCE_ATTRIBUTE_IS_AVAILABLE;

			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_resource_get_info_fn(
					dev, cur_res, res_attr, sizeof(res_available),
					&res_available));
			ASSERT_EQ(1U, res_available);
			printf(", available = %u\n", res_available);
		}
	}
}

TEST_F(T241Tests, EnumerateResourcesNegative)
{
	uint32_t res_count, res_available, i, j;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES], cur_res;
	nv_soc_hwpm_resource_attribute res_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT;

		// Should fail with invalid buffer size.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(uint8_t), &res_count));

		// Should fail with invalid buffer ptr.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(res_count), NULL));

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(res_count), &res_count));
		ASSERT_GT(res_count, 0U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST;

		// Should fail with invalid buffer size.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(uint8_t) * res_count, res));

		// Should fail with invalid buffer ptr.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*res) * res_count, NULL));

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*res) * res_count, res));

		for (j = 0; j < res_count; j++) {
			cur_res = res[j];
			printf("\t%d - res id = %u, name: %s",
				j, cur_res, kResourceNames[cur_res]);

			// Should fail with invalid attribute.
			res_attr = (nv_soc_hwpm_resource_attribute)0xffffffff;
			size_t dummy;
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_resource_get_info_fn(
					dev, cur_res, res_attr, sizeof(dummy), &dummy));

			res_attr = NV_SOC_HWPM_RESOURCE_ATTRIBUTE_IS_AVAILABLE;

			// Should fail with invalid buffer size.
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_resource_get_info_fn(
					dev, cur_res, res_attr, sizeof(uint8_t), &res_available));

			// Should fail with invalid buffer ptr.
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_resource_get_info_fn(
					dev, cur_res, res_attr, sizeof(res_available), NULL));
		}
	}
}

TEST_F(T241Tests, SessionAlloc)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_DEVICE;
		nv_soc_hwpm_device dev_test;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(dev_test), &dev_test));
		ASSERT_EQ(dev.handle, dev_test.handle);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_SESSION_STARTED;
		uint32_t session_started;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(session_started), &session_started));
		ASSERT_EQ(0U, session_started);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_BUFFER_ALLOCATED;
		uint32_t pma_buffer_allocated;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_buffer_allocated),
				&pma_buffer_allocated));
		ASSERT_EQ(0U, pma_buffer_allocated);
		
		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE;
		size_t pma_record_buffer_size;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_record_buffer_size),
				&pma_record_buffer_size));
		ASSERT_EQ(0U, pma_record_buffer_size);
		
		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA;
		void* pma_record_buffer_cpu_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_record_buffer_cpu_va),
				&pma_record_buffer_cpu_va));
		ASSERT_TRUE(NULL == pma_record_buffer_cpu_va);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA;
		uint64_t pma_record_buffer_pma_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_record_buffer_pma_va),
				&pma_record_buffer_pma_va));
		ASSERT_EQ(0U, pma_record_buffer_pma_va);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_HANDLE;
		uint32_t pma_record_buffer_handle;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_record_buffer_handle),
				&pma_record_buffer_handle));
		ASSERT_EQ(0U, pma_record_buffer_handle);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_SIZE;
		size_t pma_mem_bytes_buffer_size;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_mem_bytes_buffer_size),
				&pma_mem_bytes_buffer_size));
		ASSERT_EQ(0U, pma_mem_bytes_buffer_size);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA;
		void* pma_mem_bytes_buffer_cpu_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_mem_bytes_buffer_cpu_va),
				&pma_mem_bytes_buffer_cpu_va));
		ASSERT_TRUE(NULL == pma_mem_bytes_buffer_cpu_va);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_PMA_VA;
		uint64_t pma_mem_bytes_buffer_pma_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_mem_bytes_buffer_pma_va),
				&pma_mem_bytes_buffer_pma_va));
		ASSERT_EQ(0U, pma_mem_bytes_buffer_pma_va);

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_HANDLE;
		uint32_t pma_mem_bytes_buffer_handle;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(pma_mem_bytes_buffer_handle),
				&pma_mem_bytes_buffer_handle));

		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionAllocNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	// Should fail with invalid device handle.
	dev.handle = 0xffffffff;
	ASSERT_NE(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));
	
	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Should fail with invalid attribute.
		session_attr = (nv_soc_hwpm_session_attribute)0xffffffff;
		size_t dummy;
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(dummy), &dummy));

		// Should fail with invalid buffer size.
		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_DEVICE;
		nv_soc_hwpm_device dev_test;
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(uint8_t), &dev_test));

		// Should fail with invalid buffer ptr.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session, session_attr, sizeof(dev_test), NULL));

		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}


TEST_F(T241Tests, SessionReserveResources)
{
	uint32_t res_count, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES];
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Get available resources.
		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(res_count), &res_count));
		ASSERT_GT(res_count, 1U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*res) * res_count, res));

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve one resource.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionReserveResourcesNegative)
{
	uint32_t res_count, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES];
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Get available resources.
		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(res_count), &res_count));
		ASSERT_GT(res_count, 1U);

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(*res) * res_count, res));

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Should fail with invalid resource count.
		ASSERT_NE(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 0, res));

		// Should fail with invalid resource buffer ptr.
		ASSERT_NE(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, NULL));

		// Should fail with invalid resource id.
		res[0] = (nv_soc_hwpm_resource)0xffffffff;
		ASSERT_NE(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res));

		// Should fail with invalid session handle.
		res[0] = NV_SOC_HWPM_RESOURCE_PMA; // Use a guaranteed available resource.
		uint64_t session_handle = session.handle;
		session.handle = 0xffffffff;
		ASSERT_NE(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res));

		// Should be successful now.
		session.handle = session_handle;
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionAllocPma)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Maximum CMA size for NVIDIA config is 1024MB.
		static const uint32_t kNumSizes = 3;
		static const uint32_t kSizes[kNumSizes] = {64 *1024, 256 * 1024, 768 * 1024};
		for (uint32_t j = 0; j < kNumSizes; j++) {
			printf("PMA size: %u\n", kSizes[j]);

			// Allocate session.
			ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

			// Allocate PMA buffers.
			nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
			record_buffer_params.size = kSizes[j] * 1024;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_alloc_pma_fn(
					session, &record_buffer_params));

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_BUFFER_ALLOCATED;
			uint32_t pma_buffer_allocated;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_buffer_allocated),
					&pma_buffer_allocated));
			ASSERT_EQ(1U, pma_buffer_allocated);
			
			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE;
			size_t pma_record_buffer_size;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_record_buffer_size),
					&pma_record_buffer_size));
			ASSERT_EQ(record_buffer_params.size, pma_record_buffer_size);
			
			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA;
			void* pma_record_buffer_cpu_va;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_record_buffer_cpu_va),
					&pma_record_buffer_cpu_va));
			ASSERT_TRUE(NULL != pma_record_buffer_cpu_va);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA;
			uint64_t pma_record_buffer_pma_va;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_record_buffer_pma_va),
					&pma_record_buffer_pma_va));
			ASSERT_NE(0U, pma_record_buffer_pma_va);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_HANDLE;
			uint32_t pma_record_buffer_handle;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_record_buffer_handle),
					&pma_record_buffer_handle));
			ASSERT_NE(0U, pma_record_buffer_handle);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_SIZE;
			size_t pma_mem_bytes_buffer_size;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_mem_bytes_buffer_size),
					&pma_mem_bytes_buffer_size));
			ASSERT_EQ(4096U, pma_mem_bytes_buffer_size);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA;
			void* pma_mem_bytes_buffer_cpu_va;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_mem_bytes_buffer_cpu_va),
					&pma_mem_bytes_buffer_cpu_va));
			ASSERT_TRUE(NULL != pma_mem_bytes_buffer_cpu_va);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_PMA_VA;
			uint64_t pma_mem_bytes_buffer_pma_va;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_mem_bytes_buffer_pma_va),
					&pma_mem_bytes_buffer_pma_va));
			// Kernel doesnt populate this for mem bytes buffer.
			ASSERT_EQ(0U, pma_mem_bytes_buffer_pma_va);

			session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_HANDLE;
			uint32_t pma_mem_bytes_buffer_handle;
			ASSERT_EQ(0,
				api_table.nv_soc_hwpm_session_get_info_fn(
					session,
					session_attr,
					sizeof(pma_mem_bytes_buffer_handle),
					&pma_mem_bytes_buffer_handle));
			ASSERT_NE(0U, pma_mem_bytes_buffer_handle);

			// Free session.
			ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
		}
	}
}

TEST_F(T241Tests, SessionAllocPmaNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 1024 * 1024;

		////// Multiple PMA allocations. //////
		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));
		// Allocate PMA buffers.
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));
		// Should fail second time.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));

		////// PMA allocation after session start. //////
		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));
		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));
		// Allocate PMA buffers.
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));
		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionSetGetPmaState)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 1024 * 1024;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA;
		void* pma_mem_bytes_buffer_cpu_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session,
				session_attr,
				sizeof(pma_mem_bytes_buffer_cpu_va),
				&pma_mem_bytes_buffer_cpu_va));
		ASSERT_TRUE(NULL != pma_mem_bytes_buffer_cpu_va);

		*(uint64_t*)pma_mem_bytes_buffer_cpu_va = 0x1234;

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// Set and get PMA state.
		nv_soc_hwpm_pma_channel_state_params param = {};
		param.in_check_overflow = 1;
		param.in_mem_bump = 0;
		param.in_stream_mem_bytes = 1;
		param.in_read_mem_head = 1;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_set_get_pma_state_fn(
				session, &param));
		EXPECT_EQ(0U, param.out_overflowed);
		EXPECT_NE(0U, param.out_mem_head);

		EXPECT_EQ(0U, *(uint64_t*)pma_mem_bytes_buffer_cpu_va);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionSetGetPmaStateNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Should fail since session not started.
		nv_soc_hwpm_pma_channel_state_params param = {};
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_set_get_pma_state_fn(
				session, &param));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, DISABLED_SessionGetSetCredits)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 1024 * 1024;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// Get HS credits.
		uint32_t total_hs_credits;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_hs_credits_fn(
				session, TEGRA_SOC_HWPM_GET_TYPE_TOTAL_HS_CREDITS, &total_hs_credits));
		EXPECT_GT(total_hs_credits, 0U);

		nv_soc_hwpm_config_hs_credit_params config_hs_credit_params = {};
		config_hs_credit_params.num_credits_per_chiplet = total_hs_credits;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_config_hs_credits_fn(
				session, 1, &config_hs_credit_params));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, DISABLED_SessionGetSetCreditsNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Get HS credits.
		uint32_t total_hs_credits = 0;
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_get_hs_credits_fn(
				session, TEGRA_SOC_HWPM_GET_TYPE_TOTAL_HS_CREDITS,
				&total_hs_credits));

		nv_soc_hwpm_config_hs_credit_params config_hs_credit_params = {};
		config_hs_credit_params.num_credits_per_chiplet = total_hs_credits;
		ASSERT_NE(0,
			api_table.nv_soc_hwpm_session_config_hs_credits_fn(
				session, 1, &config_hs_credit_params));

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

void T241Tests::TestRegopsRead(nv_soc_hwpm_session session,
	uint64_t pma_record_buffer_pma_va,
	size_t record_buffer_size)
{
	int all_reg_ops_passed = 0;
	static const uint32_t kRegOpsCount = 4;
	nv_soc_hwpm_reg_ops_params reg_ops_params[kRegOpsCount];
	
	// profiler cap
	reg_ops_params[0].in_offset = NV_PERF_PMASYS_PROFILER_CAPABILITIES;
	reg_ops_params[0].in_out_val32 = 0;
	reg_ops_params[0].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[0].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[0].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS; // Initialize with def. value

	// stream buf base lo
	reg_ops_params[1].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASE(0);
	reg_ops_params[1].in_out_val32 = 0;
	reg_ops_params[1].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[1].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[1].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf base hi
	reg_ops_params[2].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASEUPPER(0);
	reg_ops_params[2].in_out_val32 = 0;
	reg_ops_params[2].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[2].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[2].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf size
	reg_ops_params[3].in_offset = NV_PERF_PMASYS_CHANNEL_OUTSIZE(0);
	reg_ops_params[3].in_out_val32 = 0;
	reg_ops_params[3].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[3].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[3].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// Read registers.
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			kRegOpsCount,
			reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);

	// Verify the register value.
	printf("profiler cap: 0x%x\n", reg_ops_params[0].in_out_val32);
	ASSERT_EQ(0x1000101U, reg_ops_params[0].in_out_val32);

	printf("pma_record_buffer_pma_va: 0x%lx\n", pma_record_buffer_pma_va);
	printf("stream buf base lo: 0x%x\n", reg_ops_params[1].in_out_val32);
	ASSERT_EQ(pma_record_buffer_pma_va & 0xffffffffULL, reg_ops_params[1].in_out_val32);

	printf("stream buf base hi: 0x%x\n", reg_ops_params[2].in_out_val32);
	ASSERT_EQ(pma_record_buffer_pma_va >> 32, reg_ops_params[2].in_out_val32);

	printf("stream buf size: 0x%x\n", reg_ops_params[3].in_out_val32);
	ASSERT_EQ(record_buffer_size, reg_ops_params[3].in_out_val32);
}

void T241Tests::TestRegopsWrite(nv_soc_hwpm_session session)
{
	uint32_t i;
	int all_reg_ops_passed = 0;
	static const uint32_t kRegOpsCount = 3;
	uint32_t reg_values[kRegOpsCount] = {};
	nv_soc_hwpm_reg_ops_params reg_ops_params[kRegOpsCount];

	// stream buf base lo
	reg_ops_params[0].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASE(0);
	reg_ops_params[0].in_out_val32 = 0;
	reg_ops_params[0].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[0].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[0].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf base hi
	reg_ops_params[1].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASEUPPER(0);
	reg_ops_params[1].in_out_val32 = 0;
	reg_ops_params[1].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[1].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[1].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf size
	reg_ops_params[2].in_offset = NV_PERF_PMASYS_CHANNEL_OUTSIZE(0);
	reg_ops_params[2].in_out_val32 = 0;
	reg_ops_params[2].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[2].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[2].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// Read registers.
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			kRegOpsCount,
			reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);
	
	for (i = 0; i < kRegOpsCount; i++) {
		reg_values[i] = reg_ops_params[i].in_out_val32;

		// Change command to write.
		reg_ops_params[i].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_WRITE32;
		// Set an arbitrary value.
		// Some of the registers are writable from bit 5 onwards.
		reg_ops_params[i].in_out_val32 = (i + 1) * 32;
	}

	// Write registers.
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			kRegOpsCount,
			reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);

	// Set ops command to read.
	for (i = 0; i < kRegOpsCount; i++) {
		reg_ops_params[i].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	}

	// Read registers again.
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			kRegOpsCount,
			reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);
	
	for (i = 0; i < kRegOpsCount; i++) {
		EXPECT_EQ((i + 1) * 32, reg_ops_params[i].in_out_val32);

		// Change command to write.
		reg_ops_params[i].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_WRITE32;
		// Restore original value.
		reg_ops_params[i].in_out_val32 = reg_values[i];
	}

	// Write registers again.
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			kRegOpsCount,
			reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);
}

TEST_F(T241Tests, SessionRegOps)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 1024 * 1024;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA;
		uint64_t pma_record_buffer_pma_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session,
				session_attr,
				sizeof(pma_record_buffer_pma_va),
				&pma_record_buffer_pma_va));
		ASSERT_NE(0U, pma_record_buffer_pma_va);

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		TestRegopsRead(session, pma_record_buffer_pma_va, record_buffer_params.size);
		TestRegopsWrite(session);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

void T241Tests::RegOpWrite32(
	nv_soc_hwpm_session session, uint64_t address, uint32_t value, uint32_t mask)
{
	nv_soc_hwpm_reg_ops_params reg_ops_params = {};
	reg_ops_params.in_offset = address;
	reg_ops_params.in_out_val32 = value;
	reg_ops_params.in_mask32 = mask;
	reg_ops_params.in_cmd = NV_SOC_HWPM_REG_OPS_CMD_WRITE32;

	int all_reg_ops_passed = 0;
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			1,
			&reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);
	EXPECT_EQ(NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS, reg_ops_params.out_status);
}

void T241Tests::RegOpRead32(
	nv_soc_hwpm_session session, uint64_t address, uint32_t *value)
{
	nv_soc_hwpm_reg_ops_params reg_ops_params = {};
	reg_ops_params.in_offset = address;
	reg_ops_params.in_out_val32 = 0;
	reg_ops_params.in_mask32 = 0xFFFFFFFF;
	reg_ops_params.in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params.out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	int all_reg_ops_passed = 0;
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_regops_fn(
			session,
			1,
			&reg_ops_params,
			NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
			&all_reg_ops_passed));
	EXPECT_EQ(1, all_reg_ops_passed);
	EXPECT_EQ(NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS, reg_ops_params.out_status);

	*value = reg_ops_params.in_out_val32;
}

void T241Tests::SetupPma(nv_soc_hwpm_session session, const PmaConfigurationParams& params)
{
	// Taken from Perfkit\Shared\Perfkit\Tests\Emulation\SOC\Tests\SocSignalTest\Src\TH500HwpmHal.cpp

	const uint32_t enable_hs = NV_PERF_PMMSYSROUTER_GLOBAL_SECURE_CONFIG_HS_STREAM_ENABLE;
	const uint32_t config_credits = 0xFF; // FIXME: credit count must be queried!
	RegOpWrite32(session, NV_PERF_PMMSYSROUTER_GLOBAL_SECURE_CONFIG, enable_hs, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYSROUTER_USER_CHANNEL_CONFIG_SECURE(0), config_credits, 0xFFFFFFFF);

	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONFIG_TESLA_MODE0(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONFIG_TESLA_MODE1(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONFIG_MIXED_MODE0(0), 0x00000000, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONFIG_MIXED_MODE1(0), 0x00000000, 0xFFFFFFFF);

	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_MASK_SECURE0(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_MASK_SECURE1(0), 0xFFFFFFFF, 0xFFFFFFFF);

	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_START_MASK0(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_START_MASK1(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_STOP_MASK0(0), 0xFFFFFFFF, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_STOP_MASK1(0), 0xFFFFFFFF, 0xFFFFFFFF);

	if (params.enable_streaming) {
		EnablePmaStreaming(session, params);
	}

	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_STATUS0(0), 0x00000000, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_STATUS1(0), 0x00000000, 0xFFFFFFFF);
}

void T241Tests::EnablePmaStreaming(nv_soc_hwpm_session session, const PmaConfigurationParams& params)
{
	uint32_t keep_latest_config =
		(params.keep_latest) ?
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_KEEP_LATEST_ENABLE :
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_KEEP_LATEST_DISABLE;
        uint32_t channel_config_user =
		0
		| REG32_WR(
			0,
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_STREAM,
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_STREAM_ENABLE)
		| REG32_WR(
			0,
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_KEEP_LATEST,
			keep_latest_config)
		| REG32_WR(
			0,
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_COALESCE_TIMEOUT_CYCLES,
			NV_PERF_PMASYS_CHANNEL_CONFIG_USER_COALESCE_TIMEOUT_CYCLES_1K);

        RegOpWrite32(
		session,
		NV_PERF_PMASYS_CHANNEL_CONFIG_USER(0),
		channel_config_user,
		0xFFFFFFFF);
}

// FIXME: Import pm_signals.h
#define NV_PERF_PMMSYS_SYS0_SIGVAL_ZERO                                                   0
#define NV_PERF_PMMSYS_SYS0_SIGVAL_ONE                                                    1
#define NV_PERF_PMMSYS_SYS0_SIGVAL_PMA_TRIGGER                                            2

// armss.h
#define MC_MCC_CTL_PERFMUX_0                                             0x0000000000008914
#define MC_MCC_CTL_PERFMUX_0_MCC_CTL_PERFMUX_EN                          31:31
#define MC_MCC_CTL_PERFMUX_0_MCC_CTL_PERFMUX_SEL                         7:0
#define NV_ADDRESS_MAP_MC0_BASE                                          0x0000000004040000

void T241Tests::SetupPmm(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint32_t perfmon_idx = params.perfmon_idx;
	uint32_t mode;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	}

	// Enable command packet encoding. Seems like this is needed so perfmon
	// can process trigger command from PMA.
	const uint32_t secure_config =
		REG32_WR(
			0,
			NV_PERF_PMMSYS_SECURE_CONFIG_COMMAND_PKT_DECODER,
			NV_PERF_PMMSYS_SECURE_CONFIG_COMMAND_PKT_DECODER_ENABLE);
	RegOpWrite32(
		session, NV_PERF_PMMSYS_SECURE_CONFIG(perfmon_idx), secure_config, 0xFFFFFFFF);

	// Set ENGINE_SEL to pma_trigger
	RegOpWrite32(
		session,
		NV_PERF_PMMSYS_ENGINE_SEL(perfmon_idx),
		NV_PERF_PMMSYS_SYS0_SIGVAL_PMA_TRIGGER,
		0xFFFFFFFF);

	// Enable GCM, local triggering.
	uint32_t control_b =
		REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLB_COUNTING_MODE,
			NV_PERF_PMMSYS_CONTROLB_COUNTING_MODE_GENERAL);
	if (params.enable_local_triggering)
	{
		control_b |= REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLB_PMLOCALTRIGA_EN,
			NV_PERF_PMMSYS_CONTROLB_PMLOCALTRIGA_EN_ENABLE);
		control_b |= REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLB_PMLOCALTRIGB_EN,
			NV_PERF_PMMSYS_CONTROLB_PMLOCALTRIGB_EN_ENABLE);
	}
	RegOpWrite32(session, NV_PERF_PMMSYS_CONTROLB(perfmon_idx), control_b, 0xFFFFFFFF);

	// Set perfmon id
	const uint32_t soc_perfmon_prefix = 0x700;  // TODO: Temporary identifier
	const uint32_t perfmon_id = perfmon_idx | soc_perfmon_prefix;
	RegOpWrite32(session, NV_PERF_PMMSYS_PERFMONID(perfmon_idx), perfmon_id, 0xFFFFFFFF);

	// Reset CYA control
	RegOpWrite32(session, NV_PERF_PMMSYS_CLAMP_CYA_CONTROL(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_WBSKEW0(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_WBSKEW1(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_WBSKEW2(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_WBSKEW_CHAIN0(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_WBSKEW_CHAIN1(perfmon_idx), 0x0, 0xFFFFFFFF);

	// Do not use truth tables
	RegOpWrite32(session, NV_PERF_PMMSYS_TRIG0_OP(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_TRIG1_OP(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_EVENT_OP(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_SAMPLE_OP(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_FT0_INPUTSEL(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_FT1_INPUTSEL(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_FT2_INPUTSEL(perfmon_idx), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_FT3_INPUTSEL(perfmon_idx), 0x0, 0xFFFFFFFF);

	if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		mode = NV_PERF_PMMSYS_CONTROL_MODE_B;

		// Configure counters by INC and assuming the signals are at least 16-bits.
		// This will increment the counter by 0xE on each cycle
		const bool use_pop_count = false;
		uint32_t counter_mode =
			(use_pop_count) ?
				NV_PERF_PMMSYS_CNTR0_INC_INCFN_POPCOUNT :
				NV_PERF_PMMSYS_CNTR0_INC_INCFN_NOP;
		const uint32_t counter0_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCBASE, 2)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCFN, counter_mode);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR0_INC(perfmon_idx), counter0_inc, 0xFFFFFFFF);
		const uint32_t counter1_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCBASE, 6)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR1_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCFN, counter_mode);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR1_INC(perfmon_idx), counter1_inc, 0xFFFFFFFF);
		const uint32_t counter2_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCBASE, 10)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR2_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCFN, counter_mode);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR2_INC(perfmon_idx), counter2_inc, 0xFFFFFFFF);
		const uint32_t counter3_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCBASE, 14)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR3_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCFN, counter_mode);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR3_INC(perfmon_idx), counter3_inc, 0xFFFFFFFF);
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		mode = NV_PERF_PMMSYS_CONTROL_MODE_E;

		// Reset RAWCNT, except if they are to be primed for overflow (useful in Mode E)
		uint32_t rawCntVal = params.enable_overflow_priming ? 0xEFFFFF00 : 0;
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT0(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT1(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT2(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT3(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT4(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT5(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT6(perfmon_idx), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_RAWCNT7(perfmon_idx), rawCntVal, 0xFFFFFFFF);

		// Configure counters by INC
		const uint32_t signal_sel = (params.collect_one) ? 1 : 0;
		const uint32_t counter_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCMASK, 0x1)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCBASE, signal_sel)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCFN, NV_PERF_PMMSYS_CNTR0_INC_INCFN_NOP);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR0_INC(perfmon_idx), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR1_INC(perfmon_idx), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR2_INC(perfmon_idx), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_CNTR3_INC(perfmon_idx), counter_inc, 0xFFFFFFFF);
	}

	// Finally, program CONTROL register
	uint32_t pmm_control = 0
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_MODE, mode)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_ADDTOCTR, NV_PERF_PMMSYS_CONTROL_ADDTOCTR_INCR)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_CLEAR_EVENT_ONCE, NV_PERF_PMMSYS_CONTROL_CLEAR_EVENT_ONCE_DISABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_EVENT_SYNC_MODE, NV_PERF_PMMSYS_CONTROL_EVENT_SYNC_MODE_LEVEL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_FLAG_SYNC_MODE, NV_PERF_PMMSYS_CONTROL_FLAG_SYNC_MODE_LEVEL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_MODEC_4X16_4X32, NV_PERF_PMMSYS_CONTROL_MODEC_4X16_4X32_DISABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_CTXSW_MODE, NV_PERF_PMMSYS_CONTROL_CTXSW_MODE_DISABLED)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_TIMEBASE_CYCLES, NV_PERF_PMMSYS_CONTROL_TIMEBASE_CYCLES_DISABLED)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_SHADOW_STATE, NV_PERF_PMMSYS_CONTROL_SHADOW_STATE_INVALID)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_DRIVE_DEBUG_PORT, NV_PERF_PMMSYS_CONTROL_DRIVE_DEBUG_PORT_NORMAL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_CTXSW_TIMER, NV_PERF_PMMSYS_CONTROL_CTXSW_TIMER_ENABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_WBMASK, NV_PERF_PMMSYS_CONTROL_WBMASK_DISABLE);
	RegOpWrite32(session, NV_PERF_PMMSYS_CONTROL(perfmon_idx), pmm_control, 0xFFFFFFFF);
}

void T241Tests::SetupWatchbus(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint32_t perfmon_idx = params.perfmon_idx;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		// PMA perfmux.
		// SIGNAL(name/width/domain/instancetype):--/sys0.pma2pm_0_static_pattern_11111111_32/32/sys0/sys/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// Source: //hw/tools/perfalyze/chips/th500/pml_files/soc_perf/pm_programming_guide.txt
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_0_GRP,
				NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_0_GRP_STATIC_PATTERN_EEEEEEEE_32_GROUP)
			| REG32_WR(0, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_ENABLE, 0x1);
		RegOpWrite32(session, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE, channel_perfmux_sel, 0xFFFFFFFF);

		// Map 16-bit signals onto reduced watchbus by SEL
		// See above comment, the start of the signal is targeted to watchbus bit 22.
		RegOpWrite32(session, NV_PERF_PMMSYS_EVENT_SEL(perfmon_idx), 0x19181716, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, NV_PERF_PMMSYS_TRIG0_SEL(perfmon_idx), 0x1D1C1B1A, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, NV_PERF_PMMSYS_TRIG1_SEL(perfmon_idx), 0x21201F1E, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, NV_PERF_PMMSYS_SAMPLE_SEL(perfmon_idx), 0x25242322, 0xFFFFFFFF); // 'E' from EEEE
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		RegOpWrite32(session, NV_PERF_PMMSYS_TRIG0_SEL(perfmon_idx), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_TRIG1_SEL(perfmon_idx), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_EVENT_SEL(perfmon_idx), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, NV_PERF_PMMSYS_SAMPLE_SEL(perfmon_idx), 0x0, 0xFFFFFFFF);
	}
}

void T241Tests::TeardownPma(nv_soc_hwpm_session session)
{
	// Clear NV_PERF_PMASYS_CHANNEL_STATUS_MEMBUF_STATUS
	const uint32_t pma_control_user = 0
		| REG32_WR(
			0,
			NV_PERF_PMASYS_CHANNEL_CONTROL_USER_MEMBUF_CLEAR_STATUS,
			NV_PERF_PMASYS_CHANNEL_CONTROL_USER_MEMBUF_CLEAR_STATUS_DOIT);
	RegOpWrite32(session, NV_PERF_PMASYS_CHANNEL_CONTROL_USER(0), pma_control_user, 0xFFFFFFFF);

	RegOpWrite32(session, NV_PERF_PMMSYSROUTER_GLOBAL_CNTRL, 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_CHANNEL_CONTROL_USER(0), 0x0, 0xFFFFFFFF);
}

void T241Tests::TeardownPmm(
	nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint32_t perfmon_idx = params.perfmon_idx;

	// Disable PMMs and reset ENGINE_SELs
	const uint32_t pmm_engine_sel = 0
		| REG32_WR(0, NV_PERF_PMMSYS_ENGINE_SEL_START, 0xFF);  // ZERO
	RegOpWrite32(session, NV_PERF_PMMSYS_ENGINE_SEL(perfmon_idx), pmm_engine_sel, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMMSYS_CONTROL(perfmon_idx), 0x0, 0xFFFFFFFF);

	// Disable command packet encoding. Seems like this is needed so perfmon
	// can process trigger command from PMA.
	const uint32_t secure_config =
		REG32_WR(
			0,
			NV_PERF_PMMSYS_SECURE_CONFIG_COMMAND_PKT_DECODER,
			NV_PERF_PMMSYS_SECURE_CONFIG_COMMAND_PKT_DECODER_DISABLE);
	RegOpWrite32(session, NV_PERF_PMMSYS_SECURE_CONFIG(perfmon_idx), secure_config, 0xFFFFFFFF);
}

void T241Tests::TeardownPerfmux(nv_soc_hwpm_session session)
{
	RegOpWrite32(session, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE, 0, 0xFFFFFFFF);
}

void T241Tests::IssuePmaTrigger(nv_soc_hwpm_session session)
{
	// This will issue PMA trigger to the perfmon.
	// The perfmon then will snapshot the counter value into shadow regiters
	uint32_t pma_global_trigger = 0
		| REG32_WR(
			0,
			NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL_GLOBAL_MANUAL_START,
			NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL_GLOBAL_MANUAL_START_PULSE);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL(0), pma_global_trigger, 0xFFFFFFFF);
}

void T241Tests::HarvestCounters(
	nv_soc_hwpm_session session, const PmmConfigurationParams& params, const uint32_t sig_val[4])
{
	const uint32_t perfmon_idx = params.perfmon_idx;

	uint32_t read_value = 0;

	RegOpRead32(session, NV_PERF_PMMSYS_EVENTCNT(perfmon_idx), &read_value);
	uint32_t event = read_value;

	RegOpRead32(session, NV_PERF_PMMSYS_TRIGGERCNT(perfmon_idx), &read_value);
	uint32_t trig0 = read_value;

	RegOpRead32(session, NV_PERF_PMMSYS_THRESHCNT(perfmon_idx), &read_value);
	uint32_t trig1 = read_value;

	RegOpRead32(session, NV_PERF_PMMSYS_SAMPLECNT(perfmon_idx), &read_value);
	uint32_t sample = read_value;

	RegOpRead32(session, NV_PERF_PMMSYS_ELAPSED(perfmon_idx), &read_value);
	uint32_t elapsed = read_value;

	printf("Event: %u, Trig0: %u, Trig1: %u, Sample: %u, Elapsed: %u\n",
		event, trig0, trig1, sample, elapsed);

	ASSERT_GT(sig_val[0], 0U);
	ASSERT_EQ((uint64_t)sig_val[0] * elapsed, event);

	ASSERT_GT(sig_val[1], 0U);
	ASSERT_EQ((uint64_t)sig_val[1] * elapsed, trig0);

	ASSERT_GT(sig_val[2], 0U);
	ASSERT_EQ((uint64_t)sig_val[2] * elapsed, trig1);

	ASSERT_GT(sig_val[3], 0U);
	ASSERT_EQ((uint64_t)sig_val[3] * elapsed, sample);
}


TEST_F(T241Tests, SessionSignalTestPmaPerfmux)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 1024 * 1024;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA;
		uint64_t pma_record_buffer_pma_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session,
				session_attr,
				sizeof(pma_record_buffer_pma_va),
				&pma_record_buffer_pma_va));
		ASSERT_NE(0U, pma_record_buffer_pma_va);

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// From //hw/tools/perfalyze/chips/th500/pml_files/soc_perf/pm_programming_guide.txt
		// Perfmon domain offset sys0
		const uint32_t perfmon_idx = 30;

		PmaConfigurationParams pma_params;
		SetupPma(session, pma_params);

		PmmConfigurationParams pmm_params;
		pmm_params.perfmon_idx = perfmon_idx;
		pmm_params.mode = PmmConfigurationParams::Mode::MODE_B;
		SetupPmm(session, pmm_params);
		SetupWatchbus(session, pmm_params);

		// http://nvbugs/3091420: Ignore the first snapshot as it could
		// contain some undesired noises between perfmux and perfmon.
		IssuePmaTrigger(session);
		const uint32_t sig_val[4] = { 0xE, 0xE, 0xE, 0xE };
		for (int i = 0; i < 5; i++) {
			IssuePmaTrigger(session);
			HarvestCounters(session, pmm_params, sig_val);
		}

		TeardownPerfmux(session);
		TeardownPmm(session, pmm_params);
		TeardownPma(session);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T241Tests, SessionStreamoutTestModeEBasicStreaming)
{
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	uint32_t i, num_mem_bytes, num_triggers, num_perfmons;
	
	// From //hw/tools/perfalyze/chips/th500/pml_files/soc_perf/pm_programming_guide.txt
	// Perfmon domain offset sys0
	const uint32_t perfmon_idx = 30;
	num_perfmons = 1;

	GetDevices();

	for (i = 0; i < t241_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t241_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size = 100 * 1024 * 1024;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_alloc_pma_fn(
				session, &record_buffer_params));

		session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA;
		uint64_t pma_record_buffer_pma_va;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_session_get_info_fn(
				session,
				session_attr,
				sizeof(pma_record_buffer_pma_va),
				&pma_record_buffer_pma_va));
		ASSERT_NE(0U, pma_record_buffer_pma_va);

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// Flush leftover records at the beginning of each subtest
		nv_soc_hwpm_pma_channel_state_params set_get_state_param = {};
		set_get_state_param.in_mem_bump = 0;
		set_get_state_param.in_stream_mem_bytes = 1;
		set_get_state_param.in_check_overflow = 1;
		set_get_state_param.in_read_mem_head = 1;
		ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_set_get_pma_state_fn(
			session, &set_get_state_param));
		SocModeEBuffer soc_mode_e_buffer(api_table, session);
		soc_mode_e_buffer.Initialize();
		soc_mode_e_buffer.SetRecordFormat(RecordFormatType::ModeE);
		num_mem_bytes = soc_mode_e_buffer.GetMemBytes();
		soc_mode_e_buffer.FlushRecordsInBuffer(num_mem_bytes);

		PmaConfigurationParams pma_params;
		pma_params.enable_streaming = true;
		SetupPma(session, pma_params);

		PmmConfigurationParams pmm_params;
		pmm_params.perfmon_idx = perfmon_idx;
		pmm_params.mode = PmmConfigurationParams::Mode::MODE_E;
		pmm_params.collect_one = true;
		SetupPmm(session, pmm_params);
		SetupWatchbus(session, pmm_params);

		num_triggers = 5;
		usleep(1000000); // 1 second
		for (i = 0; i < num_triggers; i++) {
			IssuePmaTrigger(session);
		}
		usleep(100000); // 100 milisecond

		TeardownPerfmux(session);
		TeardownPmm(session, pmm_params);
		TeardownPma(session);

		ASSERT_EQ(num_triggers * num_perfmons, soc_mode_e_buffer.GetNumValidRecords());
		ASSERT_EQ(num_triggers * num_perfmons, soc_mode_e_buffer.GetNumSamples());
		ASSERT_EQ(num_perfmons, soc_mode_e_buffer.GetNumUniquePerfmonID());
		ASSERT_GT(soc_mode_e_buffer.GetCounterValueSum(), 0U);
		ASSERT_EQ(0, soc_mode_e_buffer.IsZeroTimestampDetected());
		ASSERT_EQ(0, soc_mode_e_buffer.IsReversedTriggerCountDetected());

		// Stream & verify membytes
		set_get_state_param.in_mem_bump = 0;
		set_get_state_param.in_stream_mem_bytes = 1;
		set_get_state_param.in_check_overflow = 1;
		set_get_state_param.in_read_mem_head = 1;
		ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_set_get_pma_state_fn(
			session, &set_get_state_param));
		num_mem_bytes = soc_mode_e_buffer.GetMemBytes();
		printf("num_mem_bytes: %u\n", num_mem_bytes);
		ASSERT_GT(num_mem_bytes, 0U);
		ASSERT_EQ(num_mem_bytes,
			soc_mode_e_buffer.GetNumValidRecords() * sizeof(ModeERecordRaw));

		soc_mode_e_buffer.PrintRecords(100);

		printf("================ BEGIN BUFFER DUMP ================\n");
		soc_mode_e_buffer.DumpBuffer();
		printf("================ END BUFFER DUMP ================\n");

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}