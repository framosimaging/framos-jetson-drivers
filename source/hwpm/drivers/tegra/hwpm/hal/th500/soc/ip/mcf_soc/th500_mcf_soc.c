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

#include "th500_mcf_soc.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture th500_mcf_soc_inst0_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_SOC_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "permon_mcfsoc0_soc",
		.device_index = TH500_MCFSOC0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfsoc0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfsoc0_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfsoc0_base_r(),
		.end_pa = addr_map_rpg_pm_mcfsoc0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_soc_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_SOC_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc0_base_r(),
		.end_abs_pa = addr_map_mc0_limit_r(),
		.start_pa = addr_map_mc0_base_r(),
		.end_pa = addr_map_mc0_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_soc_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_soc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(1),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc1_base_r(),
		.end_abs_pa = addr_map_mc1_limit_r(),
		.start_pa = addr_map_mc1_base_r(),
		.end_pa = addr_map_mc1_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_soc_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_soc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 2U,
		.element_index_mask = BIT(2),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc2_base_r(),
		.end_abs_pa = addr_map_mc2_limit_r(),
		.start_pa = addr_map_mc2_base_r(),
		.end_pa = addr_map_mc2_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_soc_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_soc_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(3),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc16_base_r(),
		.end_abs_pa = addr_map_mc16_limit_r(),
		.start_pa = addr_map_mc16_base_r(),
		.end_pa = addr_map_mc16_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_soc_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_soc_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_soc_inst0_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_SOC_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_soc_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_soc_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
struct hwpm_ip_inst th500_mcf_soc_inst_static_array[
	TH500_HWPM_IP_MCF_SOC_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MCF_SOC_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_SOC_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_soc_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc0_base_r(),
				.range_end = addr_map_mc16_limit_r(),
				.element_stride = addr_map_mc0_limit_r() -
					addr_map_mc0_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_SOC_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_soc_inst0_broadcast_element_static_array,
				.range_start = addr_map_mcb_base_r(),
				.range_end = addr_map_mcb_limit_r(),
				.element_stride = addr_map_mcb_limit_r() -
					addr_map_mcb_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_SOC_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_soc_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfsoc0_base_r(),
				.range_end = addr_map_rpg_pm_mcfsoc0_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfsoc0_limit_r() -
					addr_map_rpg_pm_mcfsoc0_base_r() + 1ULL,
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
struct hwpm_ip th500_hwpm_ip_mcf_soc = {
	.num_instances = TH500_HWPM_IP_MCF_SOC_NUM_INSTANCES,
	.ip_inst_static_array = th500_mcf_soc_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_mc0_base_r(),
			.range_end = addr_map_mc16_limit_r(),
			.inst_stride = addr_map_mc16_limit_r() -
				addr_map_mc0_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
		 */
		{
			.range_start = addr_map_mcb_base_r(),
			.range_end = addr_map_mcb_limit_r(),
			.inst_stride = addr_map_mcb_limit_r() -
				addr_map_mcb_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMON
		 */
		{
			.range_start = addr_map_rpg_pm_mcfsoc0_base_r(),
			.range_end = addr_map_rpg_pm_mcfsoc0_limit_r(),
			.inst_stride = addr_map_rpg_pm_mcfsoc0_limit_r() -
				addr_map_rpg_pm_mcfsoc0_base_r() + 1ULL,
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
