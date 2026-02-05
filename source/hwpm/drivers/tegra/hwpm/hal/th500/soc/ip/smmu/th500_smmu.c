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

#include "th500_smmu.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture th500_smmu_inst0_perfmon_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_smmu0",
		.device_index = TH500_SMMU0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_smmu0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_smmu0_limit_r(),
		.start_pa = addr_map_rpg_pm_smmu0_base_r(),
		.end_pa = addr_map_rpg_pm_smmu0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst1_perfmon_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_smmu1",
		.device_index = TH500_SMMU1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_smmu1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_smmu1_limit_r(),
		.start_pa = addr_map_rpg_pm_smmu1_base_r(),
		.end_pa = addr_map_rpg_pm_smmu1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst2_perfmon_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_smmu2",
		.device_index = TH500_SMMU2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_smmu2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_smmu2_limit_r(),
		.start_pa = addr_map_rpg_pm_smmu2_base_r(),
		.end_pa = addr_map_rpg_pm_smmu2_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst3_perfmon_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_smmu3",
		.device_index = TH500_SMMU3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_smmu3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_smmu3_limit_r(),
		.start_pa = addr_map_rpg_pm_smmu3_base_r(),
		.end_pa = addr_map_rpg_pm_smmu3_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst4_perfmon_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_smmu4",
		.device_index = TH500_SMMU4_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_smmu4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_smmu4_limit_r(),
		.start_pa = addr_map_rpg_pm_smmu4_base_r(),
		.end_pa = addr_map_rpg_pm_smmu4_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_smmu0_base_r(),
		.end_abs_pa = addr_map_smmu0_limit_r(),
		.start_pa = addr_map_smmu0_base_r(),
		.end_pa = addr_map_smmu0_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_smmu_alist,
		.alist_size = ARRAY_SIZE(th500_smmu_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst1_perfmux_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_smmu1_base_r(),
		.end_abs_pa = addr_map_smmu1_limit_r(),
		.start_pa = addr_map_smmu1_base_r(),
		.end_pa = addr_map_smmu1_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_smmu_alist,
		.alist_size = ARRAY_SIZE(th500_smmu_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst2_perfmux_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_smmu2_base_r(),
		.end_abs_pa = addr_map_smmu2_limit_r(),
		.start_pa = addr_map_smmu2_base_r(),
		.end_pa = addr_map_smmu2_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_smmu_alist,
		.alist_size = ARRAY_SIZE(th500_smmu_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst3_perfmux_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_smmu3_base_r(),
		.end_abs_pa = addr_map_smmu3_limit_r(),
		.start_pa = addr_map_smmu3_base_r(),
		.end_pa = addr_map_smmu3_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_smmu_alist,
		.alist_size = ARRAY_SIZE(th500_smmu_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_smmu_inst4_perfmux_element_static_array[
	TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_smmu4_base_r(),
		.end_abs_pa = addr_map_smmu4_limit_r(),
		.start_pa = addr_map_smmu4_base_r(),
		.end_pa = addr_map_smmu4_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_smmu_alist,
		.alist_size = ARRAY_SIZE(th500_smmu_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
struct hwpm_ip_inst th500_smmu_inst_static_array[
	TH500_HWPM_IP_SMMU_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_SMMU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_smmu_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_smmu0_base_r(),
				.range_end = addr_map_smmu0_limit_r(),
				.element_stride = addr_map_smmu0_limit_r() -
					addr_map_smmu0_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_smmu_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_smmu0_base_r(),
				.range_end = addr_map_rpg_pm_smmu0_limit_r(),
				.element_stride = addr_map_rpg_pm_smmu0_limit_r() -
					addr_map_rpg_pm_smmu0_base_r() + 1ULL,
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
			TH500_HWPM_IP_SMMU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_smmu_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_smmu1_base_r(),
				.range_end = addr_map_smmu1_limit_r(),
				.element_stride = addr_map_smmu1_limit_r() -
					addr_map_smmu1_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_smmu_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_smmu1_base_r(),
				.range_end = addr_map_rpg_pm_smmu1_limit_r(),
				.element_stride = addr_map_rpg_pm_smmu1_limit_r() -
					addr_map_rpg_pm_smmu1_base_r() + 1ULL,
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
			TH500_HWPM_IP_SMMU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_smmu_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_smmu2_base_r(),
				.range_end = addr_map_smmu2_limit_r(),
				.element_stride = addr_map_smmu2_limit_r() -
					addr_map_smmu2_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_smmu_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_smmu2_base_r(),
				.range_end = addr_map_rpg_pm_smmu2_limit_r(),
				.element_stride = addr_map_rpg_pm_smmu2_limit_r() -
					addr_map_rpg_pm_smmu2_base_r() + 1ULL,
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
			TH500_HWPM_IP_SMMU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_smmu_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_smmu3_base_r(),
				.range_end = addr_map_smmu3_limit_r(),
				.element_stride = addr_map_smmu3_limit_r() -
					addr_map_smmu3_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_smmu_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_smmu3_base_r(),
				.range_end = addr_map_rpg_pm_smmu3_limit_r(),
				.element_stride = addr_map_rpg_pm_smmu3_limit_r() -
					addr_map_rpg_pm_smmu3_base_r() + 1ULL,
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
			TH500_HWPM_IP_SMMU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_smmu_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_smmu4_base_r(),
				.range_end = addr_map_smmu4_limit_r(),
				.element_stride = addr_map_smmu4_limit_r() -
					addr_map_smmu4_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_SMMU_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_SMMU_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_smmu_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_smmu4_base_r(),
				.range_end = addr_map_rpg_pm_smmu4_limit_r(),
				.element_stride = addr_map_rpg_pm_smmu4_limit_r() -
					addr_map_rpg_pm_smmu4_base_r() + 1ULL,
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
struct hwpm_ip th500_hwpm_ip_smmu = {
	.num_instances = TH500_HWPM_IP_SMMU_NUM_INSTANCES,
	.ip_inst_static_array = th500_smmu_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_smmu4_base_r(),
			.range_end = addr_map_smmu3_limit_r(),
			.inst_stride = addr_map_smmu0_limit_r() -
				addr_map_smmu0_base_r() + 1ULL,
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
			.range_start = addr_map_rpg_pm_smmu0_base_r(),
			.range_end = addr_map_rpg_pm_smmu4_limit_r(),
			.inst_stride = addr_map_rpg_pm_smmu0_limit_r() -
				addr_map_rpg_pm_smmu0_base_r() + 1ULL,
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
