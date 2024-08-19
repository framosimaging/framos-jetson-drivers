// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
 */

#include "t234_pcie.h"

#include <tegra_hwpm.h>
#include <hal/t234/t234_regops_allowlist.h>
#include <hal/t234/hw/t234_addr_map_soc_hwpm.h>
#include <hal/t234/t234_perfmon_device_index.h>

static struct hwpm_ip_aperture t234_pcie_inst0_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie0",
		.device_index = T234_PCIE0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c0_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c0_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst1_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie1",
		.device_index = T234_PCIE1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c1_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c1_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst2_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie2",
		.device_index = T234_PCIE2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c2_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c2_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c2_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst3_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie3",
		.device_index = T234_PCIE3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c3_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c3_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c3_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst4_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie4",
		.device_index = T234_PCIE4_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c4_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c4_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c4_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst5_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie5",
		.device_index = T234_PCIE5_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c5_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c5_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c5_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c5_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst6_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie6",
		.device_index = T234_PCIE6_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c6_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c6_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c6_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c6_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst7_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie7",
		.device_index = T234_PCIE7_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c7_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c7_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c7_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c7_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst8_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie8",
		.device_index = T234_PCIE8_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c8_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c8_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c8_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c8_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst9_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie9",
		.device_index = T234_PCIE9_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c9_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c9_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c9_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c9_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst10_perfmon_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(10),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_pcie10",
		.device_index = T234_PCIE10_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_pcie_c10_base_r(),
		.end_abs_pa = addr_map_rpg_pm_pcie_c10_limit_r(),
		.start_pa = addr_map_rpg_pm_pcie_c10_base_r(),
		.end_pa = addr_map_rpg_pm_pcie_c10_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = t234_perfmon_alist,
		.alist_size = ARRAY_SIZE(t234_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst0_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c0_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c0_ctl_limit_r(),
		.start_pa = addr_map_pcie_c0_ctl_base_r(),
		.end_pa = addr_map_pcie_c0_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst1_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c1_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c1_ctl_limit_r(),
		.start_pa = addr_map_pcie_c1_ctl_base_r(),
		.end_pa = addr_map_pcie_c1_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst2_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(10),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c2_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c2_ctl_limit_r(),
		.start_pa = addr_map_pcie_c2_ctl_base_r(),
		.end_pa = addr_map_pcie_c2_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst3_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c3_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c3_ctl_limit_r(),
		.start_pa = addr_map_pcie_c3_ctl_base_r(),
		.end_pa = addr_map_pcie_c3_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst4_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c4_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c4_ctl_limit_r(),
		.start_pa = addr_map_pcie_c4_ctl_base_r(),
		.end_pa = addr_map_pcie_c4_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst5_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c5_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c5_ctl_limit_r(),
		.start_pa = addr_map_pcie_c5_ctl_base_r(),
		.end_pa = addr_map_pcie_c5_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst6_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c6_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c6_ctl_limit_r(),
		.start_pa = addr_map_pcie_c6_ctl_base_r(),
		.end_pa = addr_map_pcie_c6_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst7_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c7_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c7_ctl_limit_r(),
		.start_pa = addr_map_pcie_c7_ctl_base_r(),
		.end_pa = addr_map_pcie_c7_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst8_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c8_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c8_ctl_limit_r(),
		.start_pa = addr_map_pcie_c8_ctl_base_r(),
		.end_pa = addr_map_pcie_c8_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst9_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c9_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c9_ctl_limit_r(),
		.start_pa = addr_map_pcie_c9_ctl_base_r(),
		.end_pa = addr_map_pcie_c9_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t234_pcie_inst10_perfmux_element_static_array[
	T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_pcie_c10_ctl_base_r(),
		.end_abs_pa = addr_map_pcie_c10_ctl_limit_r(),
		.start_pa = addr_map_pcie_c10_ctl_base_r(),
		.end_pa = addr_map_pcie_c10_ctl_limit_r(),
		.base_pa = 0ULL,
		.alist = t234_pcie_ctl_alist,
		.alist_size = ARRAY_SIZE(t234_pcie_ctl_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
static struct hwpm_ip_inst t234_pcie_inst_static_array[
	T234_HWPM_IP_PCIE_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst0_perfmux_element_static_array,
				.range_start = addr_map_pcie_c0_ctl_base_r(),
				.range_end = addr_map_pcie_c0_ctl_limit_r(),
				.element_stride = addr_map_pcie_c0_ctl_limit_r() -
					addr_map_pcie_c0_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c0_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c0_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c0_limit_r() -
					addr_map_rpg_pm_pcie_c0_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(1),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst1_perfmux_element_static_array,
				.range_start = addr_map_pcie_c1_ctl_base_r(),
				.range_end = addr_map_pcie_c1_ctl_limit_r(),
				.element_stride = addr_map_pcie_c1_ctl_limit_r() -
					addr_map_pcie_c1_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c1_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c1_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c1_limit_r() -
					addr_map_rpg_pm_pcie_c1_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(2),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst2_perfmux_element_static_array,
				.range_start = addr_map_pcie_c2_ctl_base_r(),
				.range_end = addr_map_pcie_c2_ctl_limit_r(),
				.element_stride = addr_map_pcie_c2_ctl_limit_r() -
					addr_map_pcie_c2_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c2_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c2_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c2_limit_r() -
					addr_map_rpg_pm_pcie_c2_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(3),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst3_perfmux_element_static_array,
				.range_start = addr_map_pcie_c3_ctl_base_r(),
				.range_end = addr_map_pcie_c3_ctl_limit_r(),
				.element_stride = addr_map_pcie_c3_ctl_limit_r() -
					addr_map_pcie_c3_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c3_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c3_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c3_limit_r() -
					addr_map_rpg_pm_pcie_c3_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(4),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst4_perfmux_element_static_array,
				.range_start = addr_map_pcie_c4_ctl_base_r(),
				.range_end = addr_map_pcie_c4_ctl_limit_r(),
				.element_stride = addr_map_pcie_c4_ctl_limit_r() -
					addr_map_pcie_c4_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c4_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c4_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c4_limit_r() -
					addr_map_rpg_pm_pcie_c4_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(5),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst5_perfmux_element_static_array,
				.range_start = addr_map_pcie_c5_ctl_base_r(),
				.range_end = addr_map_pcie_c5_ctl_limit_r(),
				.element_stride = addr_map_pcie_c5_ctl_limit_r() -
					addr_map_pcie_c5_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c5_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c5_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c5_limit_r() -
					addr_map_rpg_pm_pcie_c5_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(6),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst6_perfmux_element_static_array,
				.range_start = addr_map_pcie_c6_ctl_base_r(),
				.range_end = addr_map_pcie_c6_ctl_limit_r(),
				.element_stride = addr_map_pcie_c6_ctl_limit_r() -
					addr_map_pcie_c6_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c6_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c6_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c6_limit_r() -
					addr_map_rpg_pm_pcie_c6_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(7),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst7_perfmux_element_static_array,
				.range_start = addr_map_pcie_c7_ctl_base_r(),
				.range_end = addr_map_pcie_c7_ctl_limit_r(),
				.element_stride = addr_map_pcie_c7_ctl_limit_r() -
					addr_map_pcie_c7_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c7_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c7_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c7_limit_r() -
					addr_map_rpg_pm_pcie_c7_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(8),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst8_perfmux_element_static_array,
				.range_start = addr_map_pcie_c8_ctl_base_r(),
				.range_end = addr_map_pcie_c8_ctl_limit_r(),
				.element_stride = addr_map_pcie_c8_ctl_limit_r() -
					addr_map_pcie_c8_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst8_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c8_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c8_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c8_limit_r() -
					addr_map_rpg_pm_pcie_c8_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(9),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst9_perfmux_element_static_array,
				.range_start = addr_map_pcie_c9_ctl_base_r(),
				.range_end = addr_map_pcie_c9_ctl_limit_r(),
				.element_stride = addr_map_pcie_c9_ctl_limit_r() -
					addr_map_pcie_c9_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst9_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c9_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c9_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c9_limit_r() -
					addr_map_rpg_pm_pcie_c9_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
	{
		.hw_inst_mask = BIT(10),
		.num_core_elements_per_inst =
			T234_HWPM_IP_PCIE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t234_pcie_inst10_perfmux_element_static_array,
				.range_start = addr_map_pcie_c10_ctl_base_r(),
				.range_end = addr_map_pcie_c10_ctl_limit_r(),
				.element_stride = addr_map_pcie_c10_ctl_limit_r() -
					addr_map_pcie_c10_ctl_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T234_HWPM_IP_PCIE_NUM_BROADCAST_PER_INST,
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
					T234_HWPM_IP_PCIE_NUM_PERFMON_PER_INST,
				.element_static_array =
					t234_pcie_inst10_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_pcie_c10_base_r(),
				.range_end = addr_map_rpg_pm_pcie_c10_limit_r(),
				.element_stride = addr_map_rpg_pm_pcie_c10_limit_r() -
					addr_map_rpg_pm_pcie_c10_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
			.fd = -1,
		},

		.element_fs_mask = 0U,
		.dev_name = "",
	},
};

/* IP structure */
struct hwpm_ip t234_hwpm_ip_pcie = {
	.num_instances = T234_HWPM_IP_PCIE_NUM_INSTANCES,
	.ip_inst_static_array = t234_pcie_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_pcie_c8_ctl_base_r(),
			.range_end = addr_map_pcie_c7_ctl_limit_r(),
			.inst_stride = addr_map_pcie_c8_ctl_limit_r() -
				addr_map_pcie_c8_ctl_base_r() + 1ULL,
			.inst_slots = 0U,
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
			.range_start = addr_map_rpg_pm_pcie_c0_base_r(),
			.range_end = addr_map_rpg_pm_pcie_c10_limit_r(),
			.inst_stride = addr_map_rpg_pm_pcie_c0_limit_r() -
				addr_map_rpg_pm_pcie_c0_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = TEGRA_HWPM_FUSE_SECURITY_MODE_MASK |
		TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK,
	.override_enable = false,
	.inst_fs_mask = 0U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_INVALID,
	.reserved = false,
};
