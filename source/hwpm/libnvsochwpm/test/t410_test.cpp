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

#include "t410_test.h"
#include "soc_mode_e_buffer.h"
#include "common/register_util.h"
#include "tb500/nv_ref_dev_perf.h"
#include "tb500/address_map_new.h"
#include "ip_names.h"
#include <unistd.h>

T410Tests::T410Tests() : NvSocHwpmTests(), t410_dev_count(0)
{
}

T410Tests::~T410Tests()
{
}

void T410Tests::SetUp(void)
{
	NvSocHwpmTests::SetUp();
	ASSERT_EQ(0, api_table.nv_soc_hwpm_init_fn());
}

void T410Tests::TearDown(void)
{
	api_table.nv_soc_hwpm_exit_fn();
	NvSocHwpmTests::TearDown();
}

void T410Tests::GetDevices(void)
{
	t410_dev_count = 0;

	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t410_dev_count, NULL));
	ASSERT_EQ(1U, t410_dev_count);

	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t410_dev_count, t410_dev));
}

TEST_F(T410Tests, EnumerateDevices)
{
	uint32_t chip_id, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		dev = t410_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_CHIP_ID;

		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(chip_id), &chip_id));
		ASSERT_EQ(TEGRA_SOC_HWPM_CHIP_ID_T410, chip_id);

	}
}

TEST_F(T410Tests, EnumerateDevicesNegative)
{
	t410_dev_count = 0;

	// Should fail with invalid dev_count ptr.
	ASSERT_NE(0, api_table.nv_soc_hwpm_get_devices_fn(NULL, t410_dev));

	// Get valid count.
	ASSERT_EQ(0, api_table.nv_soc_hwpm_get_devices_fn(&t410_dev_count, NULL));
	ASSERT_EQ(1U, t410_dev_count);

	// Should fail with invalid buffer ptr.
	ASSERT_NE(0, api_table.nv_soc_hwpm_get_devices_fn(&t410_dev_count, NULL));
}

TEST_F(T410Tests, EnumerateIPs)
{
	uint32_t ip_count, i, j;
	uint64_t fs_mask;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_ip ip[NV_SOC_HWPM_NUM_IPS], cur_ip;
	nv_soc_hwpm_ip_attribute ip_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, EnumerateIPsNegative)
{
	uint32_t ip_count, i, j;
	uint64_t fs_mask;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_ip ip[NV_SOC_HWPM_NUM_IPS], cur_ip;
	nv_soc_hwpm_ip_attribute ip_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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
			printf("\t%d - ip id = %u, name: %s\n", j, cur_ip, kIpNames[cur_ip]);

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
					dev, cur_ip, ip_attr, 0, &fs_mask));

			// Should fail with invalid buffer ptr.
			ASSERT_NE(0,
				api_table.nv_soc_hwpm_ip_get_info_fn(
					dev, cur_ip, ip_attr, sizeof(fs_mask), NULL));
		}
	}
}

TEST_F(T410Tests, EnumerateResources)
{
	uint32_t res_count, res_available, i, j;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES], cur_res;
	nv_soc_hwpm_resource_attribute res_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, EnumerateResourcesNegative)
{
	uint32_t res_count, res_available, i, j;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES], cur_res;
	nv_soc_hwpm_resource_attribute res_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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
			printf("\t%d - res id = %u, name: %s\n",
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

TEST_F(T410Tests, SessionAlloc)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, SessionAllocNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	// Should fail with invalid device handle.
	dev.handle = 0xffffffff;
	ASSERT_NE(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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


TEST_F(T410Tests, SessionReserveResources)
{
	uint32_t res_count, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES];
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, SessionReserveResourcesNegative)
{
	uint32_t res_count, i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	nv_soc_hwpm_resource res[NV_SOC_HWPM_NUM_RESOURCES];
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, SessionAllocPma)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	tegra_soc_hwpm_platform platform;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(platform), &platform));

		// Maximum CMA size for NVIDIA config is 1024MB.
		static const uint32_t kNumSizes = 3;
		static const uint32_t kSiliconSizes[kNumSizes] = {64 *1024, 256 * 1024, 768 * 1024};
		static const uint32_t kPresiSizes[kNumSizes] = {16 *1024, 32 * 1024, 64 * 1024};

		const uint32_t* kSizes =
			(platform == TEGRA_SOC_HWPM_PLATFORM_SILICON) ?
				kSiliconSizes : kPresiSizes;

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

TEST_F(T410Tests, SessionAllocPmaNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, SessionSetGetPmaState)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, SessionSetGetPmaStateNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, DISABLED_SessionGetSetCredits)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

TEST_F(T410Tests, DISABLED_SessionGetSetCreditsNegative)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

void T410Tests::TestRegopsRead(nv_soc_hwpm_session session,
	uint64_t pma_record_buffer_pma_va,
	size_t record_buffer_size)
{
	int all_reg_ops_passed = 0;
	static const uint32_t kRegOpsCount = 4;
	nv_soc_hwpm_reg_ops_params reg_ops_params[kRegOpsCount];

	// streaming cap
	reg_ops_params[0].in_offset = NV_PERF_PMASYS_STREAMING_CAPABILITIES0;
	reg_ops_params[0].in_out_val32 = 0;
	reg_ops_params[0].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[0].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[0].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS; // Initialize with def. value

	// stream buf base lo
	reg_ops_params[1].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASE(0, 0);
	reg_ops_params[1].in_out_val32 = 0;
	reg_ops_params[1].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[1].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[1].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf base hi
	reg_ops_params[2].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASEUPPER(0, 0);
	reg_ops_params[2].in_out_val32 = 0;
	reg_ops_params[2].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[2].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[2].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf size
	reg_ops_params[3].in_offset = NV_PERF_PMASYS_CHANNEL_OUTSIZE(0, 0);
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
	printf("streaming cap: 0x%x\n", reg_ops_params[0].in_out_val32);
	// TODO: update this value from Rahul.
	// My understanding this should be 0x224 (2 BLOCKs), but it is 0x424 (4 BLOCKs) on FPGA.
	EXPECT_EQ(0x0000424U, reg_ops_params[0].in_out_val32);

	printf("pma_record_buffer_pma_va: 0x%lx\n", pma_record_buffer_pma_va);
	printf("stream buf base lo: 0x%x\n", reg_ops_params[1].in_out_val32);
	EXPECT_EQ(pma_record_buffer_pma_va & 0xffffffffULL, reg_ops_params[1].in_out_val32);

	printf("stream buf base hi: 0x%x\n", reg_ops_params[2].in_out_val32);
	EXPECT_EQ(pma_record_buffer_pma_va >> 32, reg_ops_params[2].in_out_val32);

	printf("stream buf size: 0x%x\n", reg_ops_params[3].in_out_val32);
	EXPECT_EQ(record_buffer_size, reg_ops_params[3].in_out_val32);
}

void T410Tests::TestRegopsWrite(nv_soc_hwpm_session session)
{
	uint32_t i;
	int all_reg_ops_passed = 0;
	static const uint32_t kRegOpsCount = 3;
	uint32_t reg_values[kRegOpsCount] = {};
	nv_soc_hwpm_reg_ops_params reg_ops_params[kRegOpsCount];

	// stream buf base lo
	reg_ops_params[0].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASE(0, 0);
	reg_ops_params[0].in_out_val32 = 0;
	reg_ops_params[0].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[0].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[0].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf base hi
	reg_ops_params[1].in_offset = NV_PERF_PMASYS_CHANNEL_OUTBASEUPPER(0, 0);
	reg_ops_params[1].in_out_val32 = 0;
	reg_ops_params[1].in_mask32 = 0xFFFFFFFF;
	reg_ops_params[1].in_cmd = NV_SOC_HWPM_REG_OPS_CMD_READ32;
	reg_ops_params[1].out_status = NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS;

	// stream buf size
	reg_ops_params[2].in_offset = NV_PERF_PMASYS_CHANNEL_OUTSIZE(0, 0);
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

TEST_F(T410Tests, SessionRegOps)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

void T410Tests::RegOpWrite32(
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

void T410Tests::RegOpRead32(
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

void T410Tests::SetupPma(nv_soc_hwpm_session session, const PmaConfigurationParams &params)
{
	// Taken from Perfkit\Shared\Perfkit\Tests\Emulation\SOC\Tests\SocSignalTest\Src\TH500HwpmHal.cpp
	nv_soc_hwpm_config_hs_credit_params credit_params;
	uint32_t config_credits = 0;
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_get_hs_credits_fn(
			session, TEGRA_SOC_HWPM_GET_TYPE_TOTAL_HS_CREDITS, &config_credits));
	ASSERT_NE(0U, config_credits);

	credit_params.cblock_idx = 0;
	credit_params.num_credits_per_chiplet = config_credits;
	ASSERT_EQ(0,
		api_table.nv_soc_hwpm_session_config_hs_credits_fn(
			session, 1, &credit_params));

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

void T410Tests::EnablePmaStreaming(nv_soc_hwpm_session session, const PmaConfigurationParams &params)
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
		NV_PERF_PMASYS_CHANNEL_CONFIG_USER(0, 0),
		channel_config_user,
		0xFFFFFFFF);
}

// FIXME: Import pm_signals.h
#define NV_PERF_PMMSYS_SYS0_SIGVAL_ZERO                                                   0
#define NV_PERF_PMMSYS_SYS0_SIGVAL_ONE                                                    1
#define NV_PERF_PMMSYS_SYS0_SIGVAL_PMA_TRIGGER                                            2

#define PM_DOMAIN_BASE(domain) (uint64_t)((0)?NV_PERF_##domain)
#define PM_REG(domain, reg, i) (uint64_t)(NV_PERF_##domain##_##reg(i))
#define PM_OFF(domain, reg) \
	(uint64_t)(PM_REG(domain, reg, 0) - PM_DOMAIN_BASE(domain))
#define PM_BASE(domain, reg, perfmon_idx) \
	(uint64_t)(PM_REG(domain, reg, perfmon_idx) - PM_OFF(domain, reg))
#define PM_ADDR(domain, reg, base) \
	(uint64_t)(base + PM_OFF(domain, reg))

void T410Tests::SetupPmm(nv_soc_hwpm_session session, const PmmConfigurationParams &params)
{
	const uint32_t perfmon_idx = params.perfmon_idx;
	const uint64_t perfmon_base = params.perfmon_base;
	uint32_t mode;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	}

	// Set ENGINE_SEL to pma_trigger
	RegOpWrite32(
		session,
		PM_ADDR(PMMSYS, ENGINE_SEL, perfmon_base),
		NV_PERF_PMMSYS_SYS0_SIGVAL_PMA_TRIGGER,
		0xFFFFFFFF);

	// Enable GCM, local triggering, user data mode.
	uint32_t control_d = 0;
	uint32_t control_b = 0;
	if (params.mode != PmmConfigurationParams::Mode::MODE_E_USERDATA) {
		control_b =
		REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLB_COUNTING_MODE,
			NV_PERF_PMMSYS_CONTROLB_COUNTING_MODE_GENERAL);
	}
	printf("counting mode control_b: %x\n", control_b);

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
	if (params.mode == PmmConfigurationParams::Mode::MODE_E_USERDATA)
	{
		control_b |= REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLB_MODEE_USERDATA,
			NV_PERF_PMMSYS_CONTROLB_MODEE_USERDATA_ENABLED);

		control_d |= REG32_WR(
			0,
			NV_PERF_PMMSYS_CONTROLD_MODEE_USERDATA_WINDOW_MODE,
			NV_PERF_PMMSYS_CONTROLD_MODEE_USERDATA_WINDOW_MODE_DISABLED);
	}
	RegOpWrite32(session, PM_ADDR(PMMSYS, CONTROLB, perfmon_base), control_b, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, CONTROLD, perfmon_base), control_d, 0xFFFFFFFF);

	// Set perfmon id
	const uint32_t soc_perfmon_prefix = 0x700;  // TODO: Temporary identifier
	const uint32_t perfmon_id = perfmon_idx | soc_perfmon_prefix;
	RegOpWrite32(session, PM_ADDR(PMMSYS, PERFMONID, perfmon_base), perfmon_id, 0xFFFFFFFF);

	// Reset CYA control
	RegOpWrite32(session, PM_ADDR(PMMSYS, CLAMP_CYA_CONTROL, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, WBSKEW0, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, WBSKEW1, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, WBSKEW2, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, WBSKEW_CHAIN0, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, WBSKEW_CHAIN1, perfmon_base), 0x0, 0xFFFFFFFF);

	// Do not use truth tables
	RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_OP, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_OP, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_OP, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_OP, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, FT0_INPUTSEL, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, FT1_INPUTSEL, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, FT2_INPUTSEL, perfmon_base), 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, FT3_INPUTSEL, perfmon_base), 0x0, 0xFFFFFFFF);

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
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR0_INC, perfmon_base), counter0_inc, 0xFFFFFFFF);
		const uint32_t counter1_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCBASE, 6)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR1_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR1_INC_INCFN, counter_mode);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR1_INC, perfmon_base), counter1_inc, 0xFFFFFFFF);
		const uint32_t counter2_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCBASE, 10)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR2_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR2_INC_INCFN, counter_mode);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR2_INC, perfmon_base), counter2_inc, 0xFFFFFFFF);
		const uint32_t counter3_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCMASK, 0xF)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCBASE, 14)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR3_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR3_INC_INCFN, counter_mode);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR3_INC, perfmon_base), counter3_inc, 0xFFFFFFFF);
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		mode = NV_PERF_PMMSYS_CONTROL_MODE_E;

		// Reset RAWCNT, except if they are to be primed for overflow (useful in Mode E)
		uint32_t rawCntVal = params.enable_overflow_priming ? 0xEFFFFF00 : 0;
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT0, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT1, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT2, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT3, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT4, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT5, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT6, perfmon_base), rawCntVal, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, RAWCNT7, perfmon_base), rawCntVal, 0xFFFFFFFF);

		// Configure counters by INC
		const uint32_t signal_sel = (params.collect_one) ? 1 : 0;
		const uint32_t counter_inc = 0
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCMASK, 0x1)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCBASE, signal_sel)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL, NV_PERF_PMMSYS_CNTR0_INC_FUNCSEL_TRUE)
			| REG32_WR(0, NV_PERF_PMMSYS_CNTR0_INC_INCFN, NV_PERF_PMMSYS_CNTR0_INC_INCFN_NOP);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR0_INC, perfmon_base), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR1_INC, perfmon_base), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR2_INC, perfmon_base), counter_inc, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, CNTR3_INC, perfmon_base), counter_inc, 0xFFFFFFFF);
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E_USERDATA) {
		mode = NV_PERF_PMMSYS_CONTROL_MODE_E;
	}

	// Finally, program CONTROL register
	uint32_t pmm_control = 0
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_MODE, mode)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_ADDTOCTR, NV_PERF_PMMSYS_CONTROL_ADDTOCTR_INCR)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_CLEAR_EVENT_ONCE, NV_PERF_PMMSYS_CONTROL_CLEAR_EVENT_ONCE_DISABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_EVENT_SYNC_MODE, NV_PERF_PMMSYS_CONTROL_EVENT_SYNC_MODE_LEVEL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_FLAG_SYNC_MODE, NV_PERF_PMMSYS_CONTROL_FLAG_SYNC_MODE_LEVEL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_MODEC_4X16_4X32, NV_PERF_PMMSYS_CONTROL_MODEC_4X16_4X32_DISABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_TIMEBASE_CYCLES, NV_PERF_PMMSYS_CONTROL_TIMEBASE_CYCLES_DISABLED)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_SHADOW_STATE, NV_PERF_PMMSYS_CONTROL_SHADOW_STATE_INVALID)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_DRIVE_DEBUG_PORT, NV_PERF_PMMSYS_CONTROL_DRIVE_DEBUG_PORT_NORMAL)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_CTXSW_TIMER, NV_PERF_PMMSYS_CONTROL_CTXSW_TIMER_ENABLE)
		| REG32_WR(0, NV_PERF_PMMSYS_CONTROL_WBMASK, NV_PERF_PMMSYS_CONTROL_WBMASK_DISABLE);
	RegOpWrite32(session, PM_ADDR(PMMSYS, CONTROL, perfmon_base), pmm_control, 0xFFFFFFFF);
}

void T410Tests::SetupWatchbusPma(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint64_t perfmon_base = params.perfmon_base;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		// PMA perfmux.
		// SIGNAL(name/width/domain/instancetype):--/sys0.pma2pm_0_static_pattern_11111111_32/32/sys0/sys/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// Source: //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_0_GRP,
				NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_0_GRP_STATIC_PATTERN_EEEEEEEE_32_GROUP)
			| REG32_WR(0, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE_PMCONTROL_ENABLE, 0x1);
		RegOpWrite32(session, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE, channel_perfmux_sel, 0xFFFFFFFF);

		// Map 16-bit signals onto reduced watchbus by SEL
		// See above comment, the start of the signal is targeted to watchbus bit 22.
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x21201F1E, 0xFFFFFFFF);  // 'E' from EEEE
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x25242322, 0xFFFFFFFF); // 'E' from EEEE
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		// PMA perfmon true bit.
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
	}
}

// TODO: remove hardcoded values
// Based on https://sc.talos.nvidia.com/serve;file-limit=none/home/tegra_manuals/html/manuals/tb500c/dev_therm.html:
#define NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE 31:31
#define NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE_ENABLE 0x1
#define NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A 5:0
#define NV_THERM_PERFMUX 0xFE0001010768

TEST_F(T410Tests, SessionRegOpsNvtherm)
{
	uint32_t i, channel_perfmux_sel;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_resource res_ids[1] = { NV_SOC_HWPM_RESOURCE_NVTHERM };

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve resource.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res_ids));

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// The perfmux value should be initialized to 0.
		RegOpRead32(session, NV_THERM_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(0x0U, channel_perfmux_sel);

		// Set the perfmux to static pattern 1.
		const uint32_t mux_sel = 0x4;
		const uint32_t write_val = 0
		| REG32_WR(
			0,
			NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A,
			mux_sel)
		| REG32_WR(
			0,
			NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE,
			NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE_ENABLE);
		RegOpWrite32(session, NV_THERM_PERFMUX, write_val, 0xFFFFFFFF);

		// Read back the perfmux value.
		RegOpRead32(session, NV_THERM_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(write_val, channel_perfmux_sel);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

void T410Tests::SetupWatchbusNvtherm(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint64_t perfmon_base = params.perfmon_base;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		// NVTHERM perfmux.
		// SIGNAL(name/width/domain/instancetype):--/nvtherm0.therm_flex_a_static_pattern_5555_16/16/nvtherm0/tjv/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE/279275970299752/2147483648/2147483648/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A/279275970299752/4/63/0/0/1/none/
		// Source: //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		const uint32_t mux_sel = 0x4;
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A,
				mux_sel)
			| REG32_WR(
				0,
				NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE,
				NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE_ENABLE);
		RegOpWrite32(session, NV_THERM_PERFMUX, channel_perfmux_sel, 0xFFFFFFFF);

		// Map 16-bit signals onto reduced watchbus by SEL
		// See above comment, the start of the signal is targeted to watchbus bit 22.
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF);  // '5' from 5555
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF);  // '5' from 5555
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x21201F1E, 0xFFFFFFFF);  // '5' from 5555
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x25242322, 0xFFFFFFFF); // '5' from 5555
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		// PMA perfmon true bit.
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E_USERDATA) {
		// PRIVATE SIGNAL (see mode B).
		// ACTUAL SIGNAL.
		// SIGNAL(name/width/domain/instancetype):--/nvtherm0.user_data_group/11/nvtherm0/tjv/
		// ROUTE(index/registers):--/0/3/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE/279275970299752/2147483648/2147483648/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A/279275970299752/0/63/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_B/279275970299752/0/16128/0/0/1/none/
		const char *use_actual_signal = getenv("USE_ACTUAL_SIGNAL");
		printf("USE_ACTUAL_SIGNAL: %s\n", (use_actual_signal ? use_actual_signal : "0"));
		bool use_static_signal = (!use_actual_signal || strcmp(use_actual_signal, "0") == 0);

		// Need to use the _SC (self clear data) according to Nathan/Alex
		uint32_t mux_sel = (use_static_signal) ? 0x5 : 0x1;
		const char *use_guide = getenv("USE_GUIDE");
		printf("USE_GUIDE: %s\n", (use_guide ? use_guide : "0"));
		if (use_guide && strcmp(use_guide, "1") == 0)
			mux_sel = (use_static_signal) ? 0x4 : 0x0;
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_HWPM_GLOBAL_0_THERM_PM_SELECT_FLEX_A,
				mux_sel)
			| REG32_WR(
				0,
				NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE,
				NV_HWPM_GLOBAL_0_THERM_PM_CTRL_ENABLE_ENABLE);
		RegOpWrite32(session, NV_THERM_PERFMUX, channel_perfmux_sel, 0xFFFFFFFF);

		if (use_static_signal) {
			printf("Setup userdata mode, capture '5' from 5555.\n");
			RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF); // '5' from 5555
			RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF); // '5' from 5555

			// Force start and valid bit to true.
			printf("Force start and valid bit to true.\n");
			RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x00010001, 0xFFFFFFFF);
		} else {
			const char *capture_valid = getenv("CAPTURE_VALID");
			bool capture_valid_bit = (capture_valid && strcmp(capture_valid, "1") == 0);
			if (!capture_valid_bit) {
				printf("Capture nvtherm debug data [7:0]\n");
				// From Nathan:
				// userdata_data_d                                   (user_data_group[7:0])                             //|> w
				// ,.userdata_flush_d                                  (user_data_group[10])                              //|> w
				// ,.userdata_start_d                                  (user_data_group[8])                               //|> w
				// ,.userdata_valid_d                                  (user_data_group[9])                               //|> w
				// );
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF); // user data signal[3:0]
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF); // user data signal[7:4]
			} else {
				printf("Capture nvtherm debug data [10:8]\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x201F1E, 0xFFFFFFFF); // user data signal[3:0]
			}

			const char *use_actual_start_valid = getenv("USE_ACTUAL_START_VALID");
			printf("USE_ACTUAL_START_VALID: %s\n", (use_actual_start_valid ? use_actual_start_valid : "0"));
			if (!use_actual_start_valid || strcmp(use_actual_start_valid, "0") == 0) {
				// Force start and valid bit to true.
				printf("Force start and valid bit to true.\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x00010001, 0xFFFFFFFF);
			} else {
				// Use actual start and valid bit.
				// Userdata comes in over watchbus --
				//     userdata[7:4]  = trig1_sel[3:0]
				//     userdata[3:0]  = trig0_sel[3:0]
				//     userdata_start = event_sel[2]
				//     userdata_flush = event_sel[1]
				//     userdata_vld   = event_sel[0]

				// From Nathan:
				// userdata_data_d                                   (user_data_group[7:0])                             //|> w
				// ,.userdata_flush_d                                  (user_data_group[10])                              //|> w
				// ,.userdata_start_d                                  (user_data_group[8])                               //|> w
				// ,.userdata_valid_d                                  (user_data_group[9])                               //|> w
				// );
				printf("Use actual start and valid bit.\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x001E201F, 0xFFFFFFFF);
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_OP, perfmon_base), 0xFFFF, 0xFFFFFFFF);
			}
		}
	}
}

// TODO: remove hardcoded values
#define NV_HWPM_CSN_0_MBN_PERFMUX (NV_ADDRESS_MAP_COMPUTE_0_MMCTLP_COMP_TYPE_CSN0_CSN_MBN_HWPM_BASE + 0x8ULL)
#define NV_HWPM_CSN_0_MBN_ENABLE 7:7
#define NV_HWPM_CSN_0_MBN_ENABLE_ENABLE 0x1
#define NV_HWPM_CSN_0_MBN_MUX_SEL 6:0

TEST_F(T410Tests, SessionRegOpsCsnMbn)
{
	uint32_t i, channel_perfmux_sel;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_resource res_ids[1] = { NV_SOC_HWPM_RESOURCE_CSN };

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve resource.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res_ids));

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// The perfmux value should be initialized to 0.
		RegOpRead32(session, NV_HWPM_CSN_0_MBN_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(0x0U, channel_perfmux_sel);

		// Set the perfmux to static pattern 1.
		const uint32_t mux_sel = 0x4;
		const uint32_t write_val = 0
		| REG32_WR(
			0,
			NV_HWPM_CSN_0_MBN_MUX_SEL,
			mux_sel)
		| REG32_WR(
			0,
			NV_HWPM_CSN_0_MBN_ENABLE,
			NV_HWPM_CSN_0_MBN_ENABLE_ENABLE);
		RegOpWrite32(session, NV_HWPM_CSN_0_MBN_PERFMUX, write_val, 0xFFFFFFFF);

		// Read back the perfmux value.
		RegOpRead32(session, NV_HWPM_CSN_0_MBN_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(write_val, channel_perfmux_sel);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

void T410Tests::SetupWatchbusCsnMbn(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint64_t perfmon_base = params.perfmon_base;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		// CSN-MBN perfmux.
		// SIGNAL(name/width/domain/instancetype):--/ucfcsn7p0.csn_mbn02pm_static_pattern_4a4a_16/16/ucfcsn7p0/tjv/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/99/2/13/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CSN0_CSN_MBN_HWPM_PMC_HWPM_PERF_MUX_0_ENABLE_0/69206024/128/128/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CSN0_CSN_MBN_HWPM_PMC_HWPM_PERF_MUX_0_MUX_SEL_0/69206024/0/127/0/0/1/none/
		// SIGNAL(name/width/domain/instancetype):--/ucfcsn7p0.csn_mbn02pm_static_pattern_a4a4_16/16/ucfcsn7p0/tjv/
		// Source: //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		const uint32_t mux_sel = 0x0;
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_HWPM_CSN_0_MBN_MUX_SEL,
				mux_sel)
			| REG32_WR(
				0,
				NV_HWPM_CSN_0_MBN_ENABLE,
				NV_HWPM_CSN_0_MBN_ENABLE_ENABLE);
		RegOpWrite32(session, NV_HWPM_CSN_0_MBN_PERFMUX, channel_perfmux_sel, 0xFFFFFFFF);

		// Map 16-bit signals onto reduced watchbus by SEL
		// See above comment, the start of the signal is targeted to watchbus bit 99.
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x66656463, 0xFFFFFFFF);  // 'a' from 4a4a
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x6A696867, 0xFFFFFFFF);  // '4' from 4a4a
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x6E6D6C6B, 0xFFFFFFFF);  // 'a' from 4a4a
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x7271706F, 0xFFFFFFFF); // '4' from 4a4a
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		// PMA perfmon true bit.
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
	}
}

// TODO: remove hardcoded values
#define NV_HWPM_CORE_0_IPMU_PERFMUX (NV_ADDRESS_MAP_COMPUTE_0_MMCTLP_COMP_TYPE_CORE0_CORE_HWPM_BASE + 0x30ULL)
#define NV_HWPM_CORE_0_IPMU_ENABLE 8:8
#define NV_HWPM_CORE_0_IPMU_ENABLE_ENABLE 0x1
#define NV_HWPM_CORE_0_IPMU_MUX_SEL 7:0

TEST_F(T410Tests, SessionRegOpsIpmu)
{
	uint32_t i, channel_perfmux_sel;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_resource res_ids[1] = { NV_SOC_HWPM_RESOURCE_CPU };

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve resource.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_resources_fn(session, 1, res_ids));

		// Start session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_start_fn(session));

		// The perfmux value should be initialized to 0.
		RegOpRead32(session, NV_HWPM_CORE_0_IPMU_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(0x0U, channel_perfmux_sel);

		// Set the perfmux to static pattern 1.
		const uint32_t mux_sel = 0x1;
		const uint32_t write_val = 0
		| REG32_WR(
			0,
			NV_HWPM_CORE_0_IPMU_MUX_SEL,
			mux_sel)
		| REG32_WR(
			0,
			NV_HWPM_CORE_0_IPMU_ENABLE,
			NV_HWPM_CORE_0_IPMU_ENABLE_ENABLE);
		RegOpWrite32(session, NV_HWPM_CORE_0_IPMU_PERFMUX, write_val, 0xFFFFFFFF);

		// Read back the perfmux value.
		RegOpRead32(session, NV_HWPM_CORE_0_IPMU_PERFMUX, &channel_perfmux_sel);
		EXPECT_EQ(write_val, channel_perfmux_sel);

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

void T410Tests::SetupWatchbusIpmu(nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint64_t perfmon_base = params.perfmon_base;

	if (params.mode == PmmConfigurationParams::Mode::MODE_C) {
		// Not supporting mode C testing for now.
		ASSERT_TRUE(false);
		return;
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_B) {
		// Core-0 IPMU perfmux.
		// Source: //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		// SIGNAL(name/width/domain/instancetype):--/ucfcsnh0p0.ipmu02pm_static_pattern_a4a4_16/16/ucfcsnh0p0/tjv/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CORE0_IPMU_PERFMUX_CONTROL_PM_EN/4718640/256/256/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CORE0_IPMU_PERFMUX_CONTROL_PM_SEL/4718640/1/255/0/0/1/none/
		const uint32_t mux_sel = 0x1;
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_HWPM_CORE_0_IPMU_MUX_SEL,
				mux_sel)
			| REG32_WR(
				0,
				NV_HWPM_CORE_0_IPMU_ENABLE,
				NV_HWPM_CORE_0_IPMU_ENABLE_ENABLE);
		RegOpWrite32(session, NV_HWPM_CORE_0_IPMU_PERFMUX, channel_perfmux_sel, 0xFFFFFFFF);

		// Map 16-bit signals onto reduced watchbus by SEL
		// See above comment, the start of the signal is targeted to watchbus bit 22.
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF);  // '4' from a4a4
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF);  // 'a' from a4a4
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x21201F1E, 0xFFFFFFFF);  // '4' from a4a4
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x25242322, 0xFFFFFFFF); // 'a' from a4a4
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E) {
		// PMA perfmon true bit.
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
		RegOpWrite32(session, PM_ADDR(PMMSYS, SAMPLE_SEL, perfmon_base), 0x0, 0xFFFFFFFF);
	} else if (params.mode == PmmConfigurationParams::Mode::MODE_E_USERDATA) {
		// PRIVATE SIGNAL (see mode B).
		// ACTUAL SIGNAL.
		// SIGNAL(name/width/domain/instancetype):--/ucfcsnh0p0.ipmu02pm_debug_event/16/ucfcsnh0p0/tjv/
		// ROUTE(index/registers):--/0/2/
		// DESTINATION(lsb_bitposition/watchbus_readback_index/watchbus_readback_lsb):--/22/0/0/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CORE0_IPMU_PERFMUX_CONTROL_PM_EN/4718640/256/256/0/0/1/none/
		// REGWRITE(field/addr/val/mask/chipletoffset/instanceoffset/instancecount/instancetype):--/NV_HWPM_GLOBAL_CORE0_IPMU_PERFMUX_CONTROL_PM_SEL/4718640/2/255/0/0/1/none/
		const char *use_actual_signal = getenv("USE_ACTUAL_SIGNAL");
		printf("USE_ACTUAL_SIGNAL: %s\n", (use_actual_signal ? use_actual_signal : "0"));
		bool use_static_signal = (!use_actual_signal || strcmp(use_actual_signal, "0") == 0);
		const uint32_t mux_sel = (use_static_signal) ? 0x1 : 0x2;
		const uint32_t channel_perfmux_sel = 0
			| REG32_WR(
				0,
				NV_HWPM_CORE_0_IPMU_MUX_SEL,
				mux_sel)
			| REG32_WR(
				0,
				NV_HWPM_CORE_0_IPMU_ENABLE,
				NV_HWPM_CORE_0_IPMU_ENABLE_ENABLE);
		RegOpWrite32(session, NV_HWPM_CORE_0_IPMU_PERFMUX, channel_perfmux_sel, 0xFFFFFFFF);

		if (use_static_signal) {
			printf("4/29 Setup userdata mode, capture 'a' from a4a4.\n");
			RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF); // '4' from a4a4
			RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF); // 'a' from a4a4

			// Force start and valid bit to true.
			printf("Force start and valid bit to true.\n");
			RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x00010001, 0xFFFFFFFF);
		} else {
			const char *capture_valid = getenv("CAPTURE_VALID");
			bool capture_valid_bit = (capture_valid && strcmp(capture_valid, "1") == 0);
			if (!capture_valid_bit) {
				printf("Capture ipmu debug data [10:3]\n");
				// Data layout: https://p4hw-swarm.nvidia.com/files/hw/doc/soc/tb50x/sysarch/iPMU/iPMU%20IAS.docx#view
				// valid: bit 0
				// start: bit 1
				// flush: bit 2
				// data: bit 3 to 10
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x1C1B1A19, 0xFFFFFFFF); // user data signal[3:0]
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x201F1E1D, 0xFFFFFFFF); // user data signal[7:4]
			} else {
				printf("Capture ipmu debug data [7:0]\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG0_SEL, perfmon_base), 0x19181716, 0xFFFFFFFF); // user data signal[3:0]
				RegOpWrite32(session, PM_ADDR(PMMSYS, TRIG1_SEL, perfmon_base), 0x1D1C1B1A, 0xFFFFFFFF); // user data signal[7:4]
			}

			const char *use_actual_start_valid = getenv("USE_ACTUAL_START_VALID");
			printf("USE_ACTUAL_START_VALID: %s\n", (use_actual_start_valid ? use_actual_start_valid : "0"));
			if (!use_actual_start_valid || strcmp(use_actual_start_valid, "0") == 0) {
				// Force start and valid bit to true.
				printf("Force start and valid bit to true.\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x00010001, 0xFFFFFFFF);
			} else {
				// Use actual start and valid bit.
				// Userdata comes in over watchbus --
				//     userdata[7:4]  = trig1_sel[3:0]
				//     userdata[3:0]  = trig0_sel[3:0]
				//     userdata_start = event_sel[2]
				//     userdata_flush = event_sel[1]
				//     userdata_vld   = event_sel[0]
				printf("Use actual start and valid bit.\n");
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_SEL, perfmon_base), 0x00171816, 0xFFFFFFFF);
				RegOpWrite32(session, PM_ADDR(PMMSYS, EVENT_OP, perfmon_base), 0xFFFF, 0xFFFFFFFF);
			}
		}
	}
}

void T410Tests::TeardownPma(nv_soc_hwpm_session session)
{
	// Clear NV_PERF_PMASYS_CHANNEL_STATUS_MEMBUF_STATUS
	const uint32_t pma_control_user = 0
		| REG32_WR(
			0,
			NV_PERF_PMASYS_CHANNEL_CONTROL_USER_MEMBUF_CLEAR_STATUS,
			NV_PERF_PMASYS_CHANNEL_CONTROL_USER_MEMBUF_CLEAR_STATUS_DOIT);
	RegOpWrite32(session, NV_PERF_PMASYS_CHANNEL_CONTROL_USER(0, 0), pma_control_user, 0xFFFFFFFF);

	RegOpWrite32(session, NV_PERF_PMMSYSROUTER_GLOBAL_CNTRL, 0x0, 0xFFFFFFFF);
	RegOpWrite32(session, NV_PERF_PMASYS_CHANNEL_CONTROL_USER(0, 0), 0x0, 0xFFFFFFFF);
}

void T410Tests::TeardownPmm(
	nv_soc_hwpm_session session, const PmmConfigurationParams& params)
{
	const uint64_t perfmon_base = params.perfmon_base;

	// Disable PMMs and reset ENGINE_SELs
	const uint32_t pmm_engine_sel = 0
		| REG32_WR(0, NV_PERF_PMMSYS_ENGINE_SEL_START, 0xFF);  // ZERO
	RegOpWrite32(session, PM_ADDR(PMMSYS, ENGINE_SEL, perfmon_base), pmm_engine_sel, 0xFFFFFFFF);
	RegOpWrite32(session, PM_ADDR(PMMSYS, CONTROL, perfmon_base), 0x0, 0xFFFFFFFF);
}

void T410Tests::TeardownPerfmux(nv_soc_hwpm_session session)
{
	RegOpWrite32(session, NV_PERF_PMASYS_PERFMUX_CONFIG_SECURE, 0, 0xFFFFFFFF);
}

void T410Tests::IssuePmaTrigger(
	nv_soc_hwpm_session session,
	bool halt_before_trigger,
	bool halt_after_trigger,
	uint32_t delay_after_trigger)
{
	if (halt_before_trigger) {
		printf("Halting before trigger. Press enter to continue\n");
		getchar();
	}

	// This will issue PMA trigger to the perfmon.
	// The perfmon then will snapshot the counter value into shadow regiters
	uint32_t pma_global_trigger = 0
		| REG32_WR(
			0,
			NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL_GLOBAL_MANUAL_START,
			NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL_GLOBAL_MANUAL_START_PULSE);
	RegOpWrite32(session, NV_PERF_PMASYS_COMMAND_SLICE_TRIGGER_CONTROL(0), pma_global_trigger, 0xFFFFFFFF);

	if (halt_after_trigger) {
		printf("Halting after trigger. Press enter to continue\n");
		getchar();
	}

	if (delay_after_trigger) {
		usleep(delay_after_trigger);
	}
}

void T410Tests::HarvestCounters(
	nv_soc_hwpm_session session, const PmmConfigurationParams &params, const uint32_t sig_val[4])
{
	const uint64_t perfmon_base = params.perfmon_base;

	uint32_t read_value = 0;

	RegOpRead32(session, PM_ADDR(PMMSYS, EVENTCNT, perfmon_base), &read_value);
	uint32_t event = read_value;

	RegOpRead32(session, PM_ADDR(PMMSYS, TRIGGERCNT, perfmon_base), &read_value);
	uint32_t trig0 = read_value;

	RegOpRead32(session, PM_ADDR(PMMSYS, THRESHCNT, perfmon_base), &read_value);
	uint32_t trig1 = read_value;

	RegOpRead32(session, PM_ADDR(PMMSYS, SAMPLECNT, perfmon_base), &read_value);
	uint32_t sample = read_value;

	RegOpRead32(session, PM_ADDR(PMMSYS, ELAPSED, perfmon_base), &read_value);
	uint32_t elapsed = read_value;

	printf("Event: %u, Trig0: %u, Trig1: %u, Sample: %u, Elapsed: %u\n",
		event, trig0, trig1, sample, elapsed);

	EXPECT_GT(sig_val[0], 0U);
	EXPECT_EQ((uint64_t)sig_val[0] * elapsed, event);

	EXPECT_GT(sig_val[1], 0U);
	EXPECT_EQ((uint64_t)sig_val[1] * elapsed, trig0);

	EXPECT_GT(sig_val[2], 0U);
	EXPECT_EQ((uint64_t)sig_val[2] * elapsed, trig1);

	EXPECT_GT(sig_val[3], 0U);
	EXPECT_EQ((uint64_t)sig_val[3] * elapsed, sample);
}

void T410Tests::InitPmmParams(nv_soc_hwpm_resource resource, PmmConfigurationParams &params)
{
	switch (resource) {
	case NV_SOC_HWPM_RESOURCE_PMA:
		// From //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		// PERFMON(domainame/chiplet/index/chipletoffset/regprefix/offsetfromPMMSYS):--/pmasys0/perfmon_sys/1/262144/NV_PERF_PMMSYS_/0/
		// Perfmon domain offset pmasys0
		params.perfmon_idx = 1;
		params.perfmon_base = PM_BASE(PMMSYS, ENGINE_SEL, params.perfmon_idx);
		params.expected_sig_val = { 0xE, 0xE, 0xE, 0xE };
		break;
	case NV_SOC_HWPM_RESOURCE_NVTHERM:
		// From //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		// PERFMON(domainame/chiplet/index/chipletoffset/regprefix/offsetfromPMMSYS):--/nvtherm0/perfmon_tjv/27264/1048576/NV_PERF_PMMTJV_/-85899345920/
		// Perfmon domain offset nvtherm0
		params.perfmon_idx = 27264;
		params.perfmon_base = PM_BASE(PMMTJV, ENGINE_SEL, params.perfmon_idx);
		params.expected_sig_val = { 0x5, 0x5, 0x5, 0x5 };
		break;
	case NV_SOC_HWPM_RESOURCE_CSN:
		// From //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		// PERFMON(domainame/chiplet/index/chipletoffset/regprefix/offsetfromPMMSYS):--/ucfcsn7p0/perfmon_tjv/146432/2097152/NV_PERF_PMMTJV_/-85899345920/
		// Perfmon domain offset csn0
		params.perfmon_idx = 146432;
		params.perfmon_base = NV_ADDRESS_MAP_COMPUTE_0_MMCTLP_COMP_TYPE_CSN0_CSN_HWPM_PRI0_BASE;
		params.expected_sig_val = { 0xa, 0x4, 0xa, 0x4 };
		break;
	case NV_SOC_HWPM_RESOURCE_CPU:
		// From //hw/nvmobile_tb50x/ip/perf/hwpm_soc/2.2/dvlib/specs/src_tb500/pm_programming_guide.txt
		// PERFMON(domainame/chiplet/index/chipletoffset/regprefix/offsetfromPMMSYS):--/ucfcsnh0p0/perfmon_tjv/158720/2097152/NV_PERF_PMMTJV_/-85899345920/
		// Perfmon domain offset csnh0
		params.perfmon_idx = 158720;
		params.perfmon_base = NV_ADDRESS_MAP_COMPUTE_0_MMCTLP_COMP_TYPE_CSNH0_CSN_HWPM_PRI0_BASE;
		params.expected_sig_val = { 0x4, 0xa, 0x4, 0xa };
		break;
	default:
		ASSERT_TRUE(false);
		break;
	}

	printf("Perfmon idx: %x, base: %lx\n", params.perfmon_idx, params.perfmon_base);
}

void T410Tests::ModeBTest(nv_soc_hwpm_resource resource)
{
	uint32_t i;
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

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

		printf("Session started\n");

		PmaConfigurationParams pma_params;
		SetupPma(session, pma_params);

		printf("PMA setup done\n");

		PmmConfigurationParams pmm_params;
		InitPmmParams(resource, pmm_params);
		SetupPmm(session, pmm_params);

		printf("PMM setup done\n");

		switch (resource) {
		case NV_SOC_HWPM_RESOURCE_PMA:
			SetupWatchbusPma(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_NVTHERM:
			SetupWatchbusNvtherm(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_CSN:
			SetupWatchbusCsnMbn(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_CPU:
			SetupWatchbusIpmu(session, pmm_params);
			break;
		default:
			ASSERT_TRUE(false);
			break;
		}

		printf("Watchbus setup done\n");

		// http://nvbugs/3091420: Ignore the first snapshot as it could
		// contain some undesired noises between perfmux and perfmon.
		IssuePmaTrigger(session);
		printf("PMA trigger issued\n");

		for (int i = 0; i < 5; i++) {
			IssuePmaTrigger(session);
			printf("PMA trigger issued\n");

			HarvestCounters(session, pmm_params, pmm_params.expected_sig_val.data());
			printf("Harvested counters\n");
		}

		TeardownPerfmux(session);
		printf("Perfmux teardown done\n");

		TeardownPmm(session, pmm_params);
		printf("PMM teardown done\n");

		TeardownPma(session);
		printf("PMA teardown done\n");

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T410Tests, SessionSignalTestPmaPerfmux)
{
	ModeBTest(NV_SOC_HWPM_RESOURCE_PMA);
}

TEST_F(T410Tests, SessionSignalTestNvthermPerfmux)
{
	ModeBTest(NV_SOC_HWPM_RESOURCE_NVTHERM);
}

TEST_F(T410Tests, SessionSignalTestCsnMbnPerfmux)
{
	ModeBTest(NV_SOC_HWPM_RESOURCE_CSN);
}

TEST_F(T410Tests, SessionSignalTestIpmuPerfmux)
{
	ModeBTest(NV_SOC_HWPM_RESOURCE_CPU);
}

void T410Tests::ModeETest(nv_soc_hwpm_resource resource)
{
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	tegra_soc_hwpm_platform platform;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	uint32_t i, num_mem_bytes, num_triggers, num_perfmons;
	const char *var_halt_before_trigger, *var_halt_after_trigger,
		*var_delay_after_trigger;
	bool halt_before_trigger, halt_after_trigger;
	uint32_t delay_after_trigger;

	num_perfmons = 1;

	halt_before_trigger = false;
	halt_after_trigger = false;
	delay_after_trigger = 0;
	var_halt_before_trigger = getenv("HALT_BEFORE_TRIGGER");
	if (var_halt_before_trigger && strcmp(var_halt_before_trigger, "1") == 0)
		halt_before_trigger = true;
	var_halt_after_trigger = getenv("HALT_AFTER_TRIGGER");
	if (var_halt_after_trigger && strcmp(var_halt_after_trigger, "1") == 0)
		halt_after_trigger = true;
	var_delay_after_trigger = getenv("DELAY_AFTER_TRIGGER");
	if (var_delay_after_trigger)
		delay_after_trigger = atoi(var_delay_after_trigger);

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(platform), &platform));

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size =
			((platform == TEGRA_SOC_HWPM_PLATFORM_SILICON) ? 100 : 32) * 1024 * 1024;
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

		printf("Session started\n");

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

		printf("PMA setup done\n");

		PmmConfigurationParams pmm_params;
		InitPmmParams(resource, pmm_params);
		pmm_params.mode = PmmConfigurationParams::Mode::MODE_E;
		pmm_params.collect_one = true;
		SetupPmm(session, pmm_params);

		printf("PMM setup done\n");
		switch (resource) {
		case NV_SOC_HWPM_RESOURCE_PMA:
			SetupWatchbusPma(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_NVTHERM:
			SetupWatchbusNvtherm(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_CSN:
			SetupWatchbusCsnMbn(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_CPU:
			SetupWatchbusIpmu(session, pmm_params);
			break;
		default:
			ASSERT_TRUE(false);
			break;
		}

		printf("Watchbus setup done\n");

		const char *halt_to_override = getenv("HALT_TO_OVERRIDE");
		if (halt_to_override && strcmp(halt_to_override, "1") == 0) {
			printf("Halt to override programming before trigger\n");
			getchar();
		}

		const char *var_num_triggers = getenv("NUM_TRIGGERS");
		num_triggers = (var_num_triggers) ? atoi(var_num_triggers) : 5;
		usleep(1000000); // 1 second
		for (i = 0; i < num_triggers; i++) {
			IssuePmaTrigger(
				session,
				halt_before_trigger,
				halt_after_trigger,
				delay_after_trigger);
		}
		usleep(100000); // 100 milisecond

		printf("PMA trigger issued\n");

		TeardownPerfmux(session);
		printf("Perfmux teardown done\n");

		TeardownPmm(session, pmm_params);
		printf("PMM teardown done\n");

		TeardownPma(session);
		printf("PMA teardown done\n");

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

TEST_F(T410Tests, SessionStreamoutTestModeEBasicStreamingPma)
{
	ModeETest(NV_SOC_HWPM_RESOURCE_PMA);
}

TEST_F(T410Tests, SessionStreamoutTestModeEBasicStreamingNvtherm)
{
	ModeETest(NV_SOC_HWPM_RESOURCE_NVTHERM);
}

TEST_F(T410Tests, SessionStreamoutTestModeEBasicStreamingCsnMbn)
{
	ModeETest(NV_SOC_HWPM_RESOURCE_CSN);
}

TEST_F(T410Tests, SessionStreamoutTestModeEBasicStreamingIpmu)
{
	ModeETest(NV_SOC_HWPM_RESOURCE_CPU);
}

void T410Tests::ModeETestUserData(nv_soc_hwpm_resource resource)
{
	nv_soc_hwpm_device dev;
	nv_soc_hwpm_device_attribute dev_attr;
	tegra_soc_hwpm_platform platform;
	nv_soc_hwpm_session session;
	nv_soc_hwpm_session_attribute session_attr;
	uint32_t i, num_mem_bytes;

	printf("ModeETestUserData\n");

	GetDevices();

	for (i = 0; i < t410_dev_count; i++) {
		printf("Device %d:\n", i);
		dev = t410_dev[i];

		dev_attr = NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM;
		ASSERT_EQ(0,
			api_table.nv_soc_hwpm_device_get_info_fn(
				dev, dev_attr, sizeof(platform), &platform));

		// Allocate session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_alloc_fn(dev, &session));

		// Reserve all resources.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_reserve_all_resources_fn(session));

		// Allocate PMA buffers.
		nv_soc_hwpm_pma_buffer_params record_buffer_params = {};
		record_buffer_params.size =
			((platform == TEGRA_SOC_HWPM_PLATFORM_SILICON) ? 100 : 32) * 1024 * 1024;
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

		printf("Session started\n");

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
		soc_mode_e_buffer.SetRecordFormat(RecordFormatType::ModeE_userdata);
		num_mem_bytes = soc_mode_e_buffer.GetMemBytes();
		soc_mode_e_buffer.FlushRecordsInBuffer(num_mem_bytes);

		PmaConfigurationParams pma_params;
		pma_params.enable_streaming = true;
		SetupPma(session, pma_params);

		printf("PMA setup done\n");

		PmmConfigurationParams pmm_params;
		InitPmmParams(resource, pmm_params);
		pmm_params.mode = PmmConfigurationParams::Mode::MODE_E_USERDATA;
		SetupPmm(session, pmm_params);

		printf("PMM setup done\n");
		switch (resource) {
		case NV_SOC_HWPM_RESOURCE_NVTHERM:
			SetupWatchbusNvtherm(session, pmm_params);
			break;
		case NV_SOC_HWPM_RESOURCE_CPU:
			SetupWatchbusIpmu(session, pmm_params);
			break;
		default:
			ASSERT_TRUE(false);
			break;
		}

		printf("Watchbus setup done\n");

		const char *halt_to_override = getenv("HALT_TO_OVERRIDE");
		if (halt_to_override && strcmp(halt_to_override, "1") == 0) {
			printf("Halt to override programming before trigger\n");
			getchar();
		}

		usleep(100000); // 100 milisecond

		TeardownPerfmux(session);
		printf("Perfmux teardown done\n");

		TeardownPmm(session, pmm_params);
		printf("PMM teardown done\n");

		TeardownPma(session);
		printf("PMA teardown done\n");

		printf("num_valid_records: %u\n", soc_mode_e_buffer.GetNumValidRecords());
		printf("num_unique_perfmon_id: %u\n", soc_mode_e_buffer.GetNumUniquePerfmonID());

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
		EXPECT_GT(num_mem_bytes, 0U);
		EXPECT_EQ(num_mem_bytes,
			soc_mode_e_buffer.GetNumValidRecords() * sizeof(ModeERecordRaw));

		soc_mode_e_buffer.PrintRecords(100);

		printf("================ BEGIN BUFFER DUMP ================\n");
		soc_mode_e_buffer.DumpBuffer();
		printf("================ END BUFFER DUMP ================\n");

		// Free session.
		ASSERT_EQ(0, api_table.nv_soc_hwpm_session_free_fn(session));
	}
}

TEST_F(T410Tests, SessionStreamoutTestModeEUserDataNvtherm)
{
	ModeETestUserData(NV_SOC_HWPM_RESOURCE_NVTHERM);
}

TEST_F(T410Tests, SessionStreamoutTestModeEUserDataIpmu)
{
	ModeETestUserData(NV_SOC_HWPM_RESOURCE_CPU);
}