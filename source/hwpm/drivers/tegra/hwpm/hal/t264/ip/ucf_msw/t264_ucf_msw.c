// SPDX-License-Identifier: MIT
/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This is a generated file. Do not edit.
 *
 * Steps to regenerate:
 *     python3 ip_files_generator.py <soc_chip> <IP_name> [<dir_name>]
 */

#include "t264_ucf_msw.h"

#include <tegra_hwpm.h>
#include <hal/t264/t264_regops_allowlist.h>
#include <hal/t264/t264_perfmon_device_index.h>
#include <hal/t264/hw/t264_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture t264_ucf_msw_inst0_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw0",
		.device_index = T264_UCF_MSW0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw0_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw0_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw0_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw1",
		.device_index = T264_UCF_MSW1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw1_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw1_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw1_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst1_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw2",
		.device_index = T264_UCF_MSW2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw2_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw2_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw2_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw3",
		.device_index = T264_UCF_MSW3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw3_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw3_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw3_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst2_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw4",
		.device_index = T264_UCF_MSW4_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw4_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw4_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw4_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw5",
		.device_index = T264_UCF_MSW5_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw5_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw5_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw5_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw5_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst3_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw6",
		.device_index = T264_UCF_MSW6_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw6_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw6_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw6_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw6_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw7",
		.device_index = T264_UCF_MSW7_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw7_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw7_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw7_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw7_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst4_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw8",
		.device_index = T264_UCF_MSW8_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw8_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw8_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw8_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw8_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw9",
		.device_index = T264_UCF_MSW9_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw9_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw9_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw9_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw9_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst5_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw10",
		.device_index = T264_UCF_MSW10_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw10_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw10_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw10_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw10_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw11",
		.device_index = T264_UCF_MSW11_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw11_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw11_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw11_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw11_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst6_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw12",
		.device_index = T264_UCF_MSW12_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw12_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw12_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw12_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw12_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw13",
		.device_index = T264_UCF_MSW13_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw13_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw13_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw13_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw13_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst7_perfmon_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw14",
		.device_index = T264_UCF_MSW14_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw14_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw14_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw14_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw14_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_ucfmsw15",
		.device_index = T264_UCF_MSW15_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ucf_msw15_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ucf_msw15_limit_r(),
		.start_pa = addr_map_rpg_pm_ucf_msw15_base_r(),
		.end_pa = addr_map_rpg_pm_ucf_msw15_limit_r(),
		.base_pa = addr_map_rpg_grp_ucf_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst0_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc0_base_r(),
		.end_abs_pa = addr_map_mc0_limit_r(),
		.start_pa = addr_map_mc0_base_r(),
		.end_pa = addr_map_mc0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn0_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn0_msw_limit_r(),
		.start_pa = addr_map_ucf_msn0_msw_base_r(),
		.end_pa = addr_map_ucf_msn0_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn0_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn0_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn0_slice0_base_r(),
		.end_pa = addr_map_ucf_msn0_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc1_base_r(),
		.end_abs_pa = addr_map_mc1_limit_r(),
		.start_pa = addr_map_mc1_base_r(),
		.end_pa = addr_map_mc1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn0_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn0_msw_limit_r(),
		.start_pa = addr_map_ucf_msn0_msw_base_r(),
		.end_pa = addr_map_ucf_msn0_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn0_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn0_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn0_slice1_base_r(),
		.end_pa = addr_map_ucf_msn0_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst1_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc2_base_r(),
		.end_abs_pa = addr_map_mc2_limit_r(),
		.start_pa = addr_map_mc2_base_r(),
		.end_pa = addr_map_mc2_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn1_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn1_msw_limit_r(),
		.start_pa = addr_map_ucf_msn1_msw_base_r(),
		.end_pa = addr_map_ucf_msn1_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn1_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn1_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn1_slice0_base_r(),
		.end_pa = addr_map_ucf_msn1_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc3_base_r(),
		.end_abs_pa = addr_map_mc3_limit_r(),
		.start_pa = addr_map_mc3_base_r(),
		.end_pa = addr_map_mc3_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn1_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn1_msw_limit_r(),
		.start_pa = addr_map_ucf_msn1_msw_base_r(),
		.end_pa = addr_map_ucf_msn1_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn1_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn1_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn1_slice1_base_r(),
		.end_pa = addr_map_ucf_msn1_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst2_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc4_base_r(),
		.end_abs_pa = addr_map_mc4_limit_r(),
		.start_pa = addr_map_mc4_base_r(),
		.end_pa = addr_map_mc4_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn2_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn2_msw_limit_r(),
		.start_pa = addr_map_ucf_msn2_msw_base_r(),
		.end_pa = addr_map_ucf_msn2_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn2_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn2_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn2_slice0_base_r(),
		.end_pa = addr_map_ucf_msn2_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc5_base_r(),
		.end_abs_pa = addr_map_mc5_limit_r(),
		.start_pa = addr_map_mc5_base_r(),
		.end_pa = addr_map_mc5_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn2_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn2_msw_limit_r(),
		.start_pa = addr_map_ucf_msn2_msw_base_r(),
		.end_pa = addr_map_ucf_msn2_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn2_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn2_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn2_slice1_base_r(),
		.end_pa = addr_map_ucf_msn2_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst3_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc6_base_r(),
		.end_abs_pa = addr_map_mc6_limit_r(),
		.start_pa = addr_map_mc6_base_r(),
		.end_pa = addr_map_mc6_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn3_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn3_msw_limit_r(),
		.start_pa = addr_map_ucf_msn3_msw_base_r(),
		.end_pa = addr_map_ucf_msn3_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn3_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn3_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn3_slice0_base_r(),
		.end_pa = addr_map_ucf_msn3_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc7_base_r(),
		.end_abs_pa = addr_map_mc7_limit_r(),
		.start_pa = addr_map_mc7_base_r(),
		.end_pa = addr_map_mc7_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn3_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn3_msw_limit_r(),
		.start_pa = addr_map_ucf_msn3_msw_base_r(),
		.end_pa = addr_map_ucf_msn3_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn3_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn3_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn3_slice1_base_r(),
		.end_pa = addr_map_ucf_msn3_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst4_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc8_base_r(),
		.end_abs_pa = addr_map_mc8_limit_r(),
		.start_pa = addr_map_mc8_base_r(),
		.end_pa = addr_map_mc8_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn4_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn4_msw_limit_r(),
		.start_pa = addr_map_ucf_msn4_msw_base_r(),
		.end_pa = addr_map_ucf_msn4_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn4_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn4_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn4_slice0_base_r(),
		.end_pa = addr_map_ucf_msn4_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc9_base_r(),
		.end_abs_pa = addr_map_mc9_limit_r(),
		.start_pa = addr_map_mc9_base_r(),
		.end_pa = addr_map_mc9_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn4_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn4_msw_limit_r(),
		.start_pa = addr_map_ucf_msn4_msw_base_r(),
		.end_pa = addr_map_ucf_msn4_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn4_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn4_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn4_slice1_base_r(),
		.end_pa = addr_map_ucf_msn4_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst5_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc10_base_r(),
		.end_abs_pa = addr_map_mc10_limit_r(),
		.start_pa = addr_map_mc10_base_r(),
		.end_pa = addr_map_mc10_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn5_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn5_msw_limit_r(),
		.start_pa = addr_map_ucf_msn5_msw_base_r(),
		.end_pa = addr_map_ucf_msn5_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn5_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn5_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn5_slice0_base_r(),
		.end_pa = addr_map_ucf_msn5_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc11_base_r(),
		.end_abs_pa = addr_map_mc11_limit_r(),
		.start_pa = addr_map_mc11_base_r(),
		.end_pa = addr_map_mc11_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn5_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn5_msw_limit_r(),
		.start_pa = addr_map_ucf_msn5_msw_base_r(),
		.end_pa = addr_map_ucf_msn5_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn5_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn5_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn5_slice1_base_r(),
		.end_pa = addr_map_ucf_msn5_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst6_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc12_base_r(),
		.end_abs_pa = addr_map_mc12_limit_r(),
		.start_pa = addr_map_mc12_base_r(),
		.end_pa = addr_map_mc12_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn6_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn6_msw_limit_r(),
		.start_pa = addr_map_ucf_msn6_msw_base_r(),
		.end_pa = addr_map_ucf_msn6_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn6_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn6_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn6_slice0_base_r(),
		.end_pa = addr_map_ucf_msn6_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc13_base_r(),
		.end_abs_pa = addr_map_mc13_limit_r(),
		.start_pa = addr_map_mc13_base_r(),
		.end_pa = addr_map_mc13_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn6_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn6_msw_limit_r(),
		.start_pa = addr_map_ucf_msn6_msw_base_r(),
		.end_pa = addr_map_ucf_msn6_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn6_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn6_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn6_slice1_base_r(),
		.end_pa = addr_map_ucf_msn6_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_ucf_msw_inst7_perfmux_element_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc14_base_r(),
		.end_abs_pa = addr_map_mc14_limit_r(),
		.start_pa = addr_map_mc14_base_r(),
		.end_pa = addr_map_mc14_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn7_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn7_msw_limit_r(),
		.start_pa = addr_map_ucf_msn7_msw_base_r(),
		.end_pa = addr_map_ucf_msn7_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw0_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw0_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(0),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn7_slice0_base_r(),
		.end_abs_pa = addr_map_ucf_msn7_slice0_limit_r(),
		.start_pa = addr_map_ucf_msn7_slice0_base_r(),
		.end_pa = addr_map_ucf_msn7_slice0_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 3U,
		.element_index_mask = BIT(1),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc15_base_r(),
		.end_abs_pa = addr_map_mc15_limit_r(),
		.start_pa = addr_map_mc15_base_r(),
		.end_pa = addr_map_mc15_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_cbridge_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_cbridge_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 4U,
		.element_index_mask = BIT(1),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn7_msw_base_r(),
		.end_abs_pa = addr_map_ucf_msn7_msw_limit_r(),
		.start_pa = addr_map_ucf_msn7_msw_base_r(),
		.end_pa = addr_map_ucf_msn7_msw_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msn_msw1_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msn_msw1_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 5U,
		.element_index_mask = BIT(1),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ucf_msn7_slice1_base_r(),
		.end_abs_pa = addr_map_ucf_msn7_slice1_limit_r(),
		.start_pa = addr_map_ucf_msn7_slice1_base_r(),
		.end_pa = addr_map_ucf_msn7_slice1_limit_r(),
		.base_pa = 0ULL,
		.alist = t264_ucf_msw_slc_alist,
		.alist_size = ARRAY_SIZE(t264_ucf_msw_slc_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
static struct hwpm_ip_inst t264_ucf_msw_inst_static_array[
	T264_HWPM_IP_UCF_MSW_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc0_base_r(),
				.range_end = addr_map_ucf_msn0_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn0_slice0_limit_r() -
					addr_map_ucf_msn0_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw0_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw1_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw0_limit_r() -
					addr_map_rpg_pm_ucf_msw0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(1),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc2_base_r(),
				.range_end = addr_map_ucf_msn1_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn1_slice0_limit_r() -
					addr_map_ucf_msn1_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw2_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw3_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw2_limit_r() -
					addr_map_rpg_pm_ucf_msw2_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(2),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc4_base_r(),
				.range_end = addr_map_ucf_msn2_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn2_slice0_limit_r() -
					addr_map_ucf_msn2_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw4_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw5_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw4_limit_r() -
					addr_map_rpg_pm_ucf_msw4_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(3),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc6_base_r(),
				.range_end = addr_map_ucf_msn3_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn3_slice0_limit_r() -
					addr_map_ucf_msn3_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw6_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw7_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw6_limit_r() -
					addr_map_rpg_pm_ucf_msw6_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(4),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc8_base_r(),
				.range_end = addr_map_ucf_msn4_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn4_slice0_limit_r() -
					addr_map_ucf_msn4_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw8_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw9_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw8_limit_r() -
					addr_map_rpg_pm_ucf_msw8_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(5),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst5_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc10_base_r(),
				.range_end = addr_map_ucf_msn5_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn5_slice0_limit_r() -
					addr_map_ucf_msn5_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw10_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw11_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw10_limit_r() -
					addr_map_rpg_pm_ucf_msw10_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(6),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst6_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc12_base_r(),
				.range_end = addr_map_ucf_msn6_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn6_slice0_limit_r() -
					addr_map_ucf_msn6_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw12_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw13_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw12_limit_r() -
					addr_map_rpg_pm_ucf_msw12_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(7),
		.num_core_elements_per_inst =
			T264_HWPM_IP_UCF_MSW_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst7_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc14_base_r(),
				.range_end = addr_map_ucf_msn7_slice1_limit_r(),
				.element_stride = addr_map_ucf_msn7_slice0_limit_r() -
					addr_map_ucf_msn7_slice0_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_BROADCAST_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_UCF_MSW_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_ucf_msw_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ucf_msw14_base_r(),
				.range_end = addr_map_rpg_pm_ucf_msw15_limit_r(),
				.element_stride = addr_map_rpg_pm_ucf_msw14_limit_r() -
					addr_map_rpg_pm_ucf_msw14_base_r() + 1ULL,
				.element_slots = 0U,
				.eslots_overlimit = true,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = TEGRA_HWPM_IP_DEBUG_FD_INVALID,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
};

/* IP structure */
struct hwpm_ip t264_hwpm_ip_ucf_msw = {
	.num_instances = T264_HWPM_IP_UCF_MSW_NUM_INSTANCES,
	.ip_inst_static_array = t264_ucf_msw_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_mc0_base_r(),
			.range_end = addr_map_ucf_msn7_slice1_limit_r(),
			.inst_stride = addr_map_ucf_msn0_slice1_limit_r() -
				addr_map_mc0_base_r() + 1ULL,
			.inst_slots = 0U,
			.islots_overlimit = true,
			.inst_arr = NULL,
		},
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
		 */
		{
			.range_start = 0ULL,
			.range_end = 0ULL,
			.inst_stride = 0ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
		 */
		{
			.range_start = addr_map_rpg_pm_ucf_msw0_base_r(),
			.range_end = addr_map_rpg_pm_ucf_msw15_limit_r(),
			.inst_stride = addr_map_rpg_pm_ucf_msw1_limit_r() -
				addr_map_rpg_pm_ucf_msw0_base_r() + 1ULL,
			.inst_slots = 0U,
			.islots_overlimit = true,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = TEGRA_HWPM_FUSE_OPT_HWPM_DISABLE_MASK | TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK,
	.override_enable = false,
	.inst_fs_mask = 0U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_INVALID,
	.reserved = false,
};
