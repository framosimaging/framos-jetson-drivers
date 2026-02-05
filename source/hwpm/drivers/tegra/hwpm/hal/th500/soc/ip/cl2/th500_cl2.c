// SPDX-License-Identifier: MIT
/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include "th500_cl2.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture th500_cl2_inst0_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_0",
		.device_index = TH500_LTC0S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc0s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc0s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc0s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc0s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_1",
		.device_index = TH500_LTC0S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc0s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc0s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc0s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc0s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst1_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_2",
		.device_index = TH500_LTC1S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc1s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc1s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc1s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc1s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_3",
		.device_index = TH500_LTC1S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc1s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc1s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc1s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc1s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst2_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_4",
		.device_index = TH500_LTC2S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc2s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc2s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc2s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc2s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_5",
		.device_index = TH500_LTC2S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc2s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc2s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc2s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc2s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst3_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_6",
		.device_index = TH500_LTC3S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc3s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc3s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc3s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc3s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_7",
		.device_index = TH500_LTC3S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc3s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc3s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc3s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc3s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst4_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_8",
		.device_index = TH500_LTC4S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc4s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc4s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc4s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc4s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_9",
		.device_index = TH500_LTC4S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc4s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc4s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc4s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc4s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst5_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_10",
		.device_index = TH500_LTC5S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc5s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc5s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc5s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc5s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_11",
		.device_index = TH500_LTC5S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc5s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc5s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc5s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc5s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst6_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_12",
		.device_index = TH500_LTC6S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc6s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc6s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc6s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc6s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_13",
		.device_index = TH500_LTC6S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc6s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc6s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc6s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc6s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst7_perfmon_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_14",
		.device_index = TH500_LTC7S0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc7s0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc7s0_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc7s0_base_r(),
		.end_pa = addr_map_rpg_pm_ltc7s0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_cl2_15",
		.device_index = TH500_LTC7S1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_ltc7s1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_ltc7s1_limit_r(),
		.start_pa = addr_map_rpg_pm_ltc7s1_base_r(),
		.end_pa = addr_map_rpg_pm_ltc7s1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc0_base_r(),
		.end_abs_pa = addr_map_ltc0_limit_r(),
		.start_pa = addr_map_ltc0_base_r(),
		.end_pa = addr_map_ltc0_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst1_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc1_base_r(),
		.end_abs_pa = addr_map_ltc1_limit_r(),
		.start_pa = addr_map_ltc1_base_r(),
		.end_pa = addr_map_ltc1_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst2_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc2_base_r(),
		.end_abs_pa = addr_map_ltc2_limit_r(),
		.start_pa = addr_map_ltc2_base_r(),
		.end_pa = addr_map_ltc2_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst3_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc3_base_r(),
		.end_abs_pa = addr_map_ltc3_limit_r(),
		.start_pa = addr_map_ltc3_base_r(),
		.end_pa = addr_map_ltc3_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst4_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc4_base_r(),
		.end_abs_pa = addr_map_ltc4_limit_r(),
		.start_pa = addr_map_ltc4_base_r(),
		.end_pa = addr_map_ltc4_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst5_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc5_base_r(),
		.end_abs_pa = addr_map_ltc5_limit_r(),
		.start_pa = addr_map_ltc5_base_r(),
		.end_pa = addr_map_ltc5_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst6_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc6_base_r(),
		.end_abs_pa = addr_map_ltc6_limit_r(),
		.start_pa = addr_map_ltc6_base_r(),
		.end_pa = addr_map_ltc6_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_cl2_inst7_perfmux_element_static_array[
	TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_ltc7_base_r(),
		.end_abs_pa = addr_map_ltc7_limit_r(),
		.start_pa = addr_map_ltc7_base_r(),
		.end_pa = addr_map_ltc7_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_cl2_alist,
		.alist_size = ARRAY_SIZE(th500_cl2_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
struct hwpm_ip_inst th500_cl2_inst_static_array[
	TH500_HWPM_IP_CL2_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc0_base_r(),
				.range_end = addr_map_ltc0_limit_r(),
				.element_stride = addr_map_ltc0_limit_r() -
					addr_map_ltc0_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc0s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc0s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc0s0_limit_r() -
					addr_map_rpg_pm_ltc0s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc1_base_r(),
				.range_end = addr_map_ltc1_limit_r(),
				.element_stride = addr_map_ltc1_limit_r() -
					addr_map_ltc1_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc1s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc1s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc1s0_limit_r() -
					addr_map_rpg_pm_ltc1s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc2_base_r(),
				.range_end = addr_map_ltc2_limit_r(),
				.element_stride = addr_map_ltc2_limit_r() -
					addr_map_ltc2_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc2s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc2s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc2s0_limit_r() -
					addr_map_rpg_pm_ltc2s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc3_base_r(),
				.range_end = addr_map_ltc3_limit_r(),
				.element_stride = addr_map_ltc3_limit_r() -
					addr_map_ltc3_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc3s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc3s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc3s0_limit_r() -
					addr_map_rpg_pm_ltc3s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc4_base_r(),
				.range_end = addr_map_ltc4_limit_r(),
				.element_stride = addr_map_ltc4_limit_r() -
					addr_map_ltc4_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc4s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc4s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc4s0_limit_r() -
					addr_map_rpg_pm_ltc4s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst5_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc5_base_r(),
				.range_end = addr_map_ltc5_limit_r(),
				.element_stride = addr_map_ltc5_limit_r() -
					addr_map_ltc5_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc5s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc5s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc5s0_limit_r() -
					addr_map_rpg_pm_ltc5s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst6_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc6_base_r(),
				.range_end = addr_map_ltc6_limit_r(),
				.element_stride = addr_map_ltc6_limit_r() -
					addr_map_ltc6_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc6s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc6s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc6s0_limit_r() -
					addr_map_rpg_pm_ltc6s0_base_r() + 1ULL,
				.element_slots = 0U,
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
			TH500_HWPM_IP_CL2_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_cl2_inst7_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_ltc7_base_r(),
				.range_end = addr_map_ltc7_limit_r(),
				.element_stride = addr_map_ltc7_limit_r() -
					addr_map_ltc7_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_CL2_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_CL2_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_cl2_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_ltc7s0_base_r(),
				.range_end = addr_map_rpg_pm_ltc7s1_limit_r(),
				.element_stride = addr_map_rpg_pm_ltc7s0_limit_r() -
					addr_map_rpg_pm_ltc7s0_base_r() + 1ULL,
				.element_slots = 0U,
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
struct hwpm_ip th500_hwpm_ip_cl2 = {
	.num_instances = TH500_HWPM_IP_CL2_NUM_INSTANCES,
	.ip_inst_static_array = th500_cl2_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_ltc0_base_r(),
			.range_end = addr_map_ltc7_limit_r(),
			.inst_stride = addr_map_ltc0_limit_r() -
				addr_map_ltc0_base_r() + 1ULL,
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
			.range_start = addr_map_rpg_pm_ltc0s0_base_r(),
			.range_end = addr_map_rpg_pm_ltc7s1_limit_r(),
			.inst_stride = addr_map_rpg_pm_ltc0s0_limit_r() -
				addr_map_rpg_pm_ltc0s0_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = TEGRA_HWPM_FUSE_SECURITY_MODE_MASK | TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK,
	.override_enable = false,
	.inst_fs_mask = 0U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_INVALID,
	.reserved = false,
};
