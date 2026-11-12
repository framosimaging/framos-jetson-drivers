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

#include "nv_soc_hwpm_test.h"

NvSocHwpmTests::NvSocHwpmTests() : testing::Test(), api_table()
{}

NvSocHwpmTests::~NvSocHwpmTests()
{}

// Called once before the tests start to run.
void NvSocHwpmTests::SetUpTestCase()
{
}

// Called after all tests are done.
void NvSocHwpmTests::TearDownTestCase()
{
}

// Called before each test is run.
void NvSocHwpmTests::SetUp()
{
	LoadNvSocHwpm();
}

// Called after each test is run.
void NvSocHwpmTests::TearDown()
{
	UnloadNvSocHwpm();
}

void NvSocHwpmTests::LoadNvSocHwpm()
{
	static const char* kLibName = "libnvsochwpm.so";
	ASSERT_EQ(0, load_nv_soc_hwpm(kLibName, &api_table));
}

void NvSocHwpmTests::UnloadNvSocHwpm()
{
	ASSERT_EQ(0, unload_nv_soc_hwpm(&api_table));
}

TEST_F(NvSocHwpmTests, MultipleInitExit)
{
	ASSERT_EQ(0, api_table.nv_soc_hwpm_init_fn());
	ASSERT_EQ(0, api_table.nv_soc_hwpm_init_fn());
	api_table.nv_soc_hwpm_exit_fn();
	api_table.nv_soc_hwpm_exit_fn();
}


// Make sure we can get the lib major/minor version without calling init/exit
TEST_F(NvSocHwpmTests, EnumerateSystemInfo)
{
	uint32_t major, minor;
	nv_soc_hwpm_system_attribute attr = NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MAJOR;
	ASSERT_EQ(0, api_table.nv_soc_hwpm_system_get_info_fn(attr, sizeof(major), &major));
	attr = NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MINOR;
	ASSERT_EQ(0, api_table.nv_soc_hwpm_system_get_info_fn(attr, sizeof(minor), &minor));
}

TEST_F(NvSocHwpmTests, EnumerateSystemInfoNegative)
{
	uint32_t dummy, minor;

	nv_soc_hwpm_system_attribute attr;

	// Should fail with invalid attribute.
	attr = (nv_soc_hwpm_system_attribute)0xffffffff;
	ASSERT_NE(0, api_table.nv_soc_hwpm_system_get_info_fn(attr, sizeof(dummy), &dummy));

	attr = NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MAJOR;

	// Should fail with invalid buffer size.
	ASSERT_NE(0, api_table.nv_soc_hwpm_system_get_info_fn(attr, sizeof(uint8_t), &minor));

	// Should fail with invalid buffer ptr.
	ASSERT_NE(0, api_table.nv_soc_hwpm_system_get_info_fn(attr, sizeof(minor), NULL));
}