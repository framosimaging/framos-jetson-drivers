// SPDX-License-Identifier: MIT
/* SPDX-FileCopyrightText: Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
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

#include "th500_rtr.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

/* RTR aperture should be placed in instance TH500_HWPM_IP_RTR_STATIC_RTR_INST */
static struct hwpm_ip_aperture th500_rtr_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_RTR_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "rtr",
		.device_index = TH500_RTR_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rtr_base_r(),
		.end_abs_pa = addr_map_rtr_limit_r(),
		.start_pa = addr_map_rtr_base_r(),
		.end_pa = addr_map_rtr_limit_r(),
		.base_pa = addr_map_rtr_base_r(),
		.alist = th500_rtr_alist,
		.alist_size = ARRAY_SIZE(th500_rtr_alist),
		.fake_registers = NULL,
	},
};

/* PMA from RTR perspective */
/* PMA aperture should be placed in instance TH500_HWPM_IP_RTR_STATIC_PMA_INST */
static struct hwpm_ip_aperture th500_rtr_inst1_perfmux_element_static_array[
	TH500_HWPM_IP_RTR_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "pma",
		.device_index = TH500_PMA_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_pma_base_r(),
		.end_abs_pa = addr_map_pma_limit_r(),
		.start_pa = addr_map_pma_base_r(),
		.end_pa = addr_map_pma_limit_r(),
		.base_pa = addr_map_pma_base_r(),
		.alist = th500_pma_res_cmd_slice_rtr_alist,
		.alist_size = ARRAY_SIZE(th500_pma_res_cmd_slice_rtr_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
static struct hwpm_ip_inst th500_rtr_inst_static_array[
	TH500_HWPM_IP_RTR_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_RTR_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_RTR_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_rtr_inst0_perfmux_element_static_array,
				.range_start = addr_map_rtr_base_r(),
				.range_end = addr_map_rtr_limit_r(),
				.element_stride = addr_map_rtr_limit_r() -
					addr_map_rtr_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_RTR_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_RTR_NUM_PERFMON_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},

		.element_fs_mask = 0x1U,
	},
	{
		.hw_inst_mask = BIT(1),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_RTR_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_RTR_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_rtr_inst1_perfmux_element_static_array,
				.range_start = addr_map_pma_base_r(),
				.range_end = addr_map_pma_limit_r(),
				.element_stride = addr_map_pma_limit_r() -
					addr_map_pma_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_RTR_NUM_BROADCAST_PER_INST,
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
					TH500_HWPM_IP_RTR_NUM_PERFMON_PER_INST,
				.element_static_array = NULL,
				.range_start = 0ULL,
				.range_end = 0ULL,
				.element_stride = 0ULL,

				.element_slots = 0U,
				.element_arr = NULL,
			},
		},

		.ip_ops = {
			.ip_dev = NULL,
			.hwpm_ip_pm = NULL,
			.hwpm_ip_reg_op = NULL,
		},

		.element_fs_mask = 0x1U,
	},
};

/* IP structure */
struct hwpm_ip th500_hwpm_ip_rtr = {
	.num_instances = TH500_HWPM_IP_RTR_NUM_INSTANCES,
	.ip_inst_static_array = th500_rtr_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/*
			 * PMA block is 0x2000 wide and RTR block is 0x1000 wide
			 * Expected facts:
			 * - PMA should be referred as a single entity
			 * - RTR IP instance array should have 2 slots(PMA, RTR)
			 *
			 * To ensure that the inst_slots are computed correctly
			 * as 2 slots, the instance range for perfmux aperture
			 * needs to be twice the PMA block.
			 */
			.range_start = addr_map_pma_base_r(),
			.range_end = addr_map_pma_limit_r() +
				(addr_map_pma_limit_r() -
					addr_map_pma_base_r() + 1ULL),
			/* Use PMA stride as it is larger block than RTR */
			.inst_stride = addr_map_pma_limit_r() -
				addr_map_pma_base_r() + 1ULL,
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
			.range_start = 0ULL,
			.range_end = 0ULL,
			.inst_stride = 0ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = 0U,
	.override_enable = false,
	/* RTR is defined as 2 instance IP corresponding to router and pma */
	/* Set this mask to indicate that instances are available */
	.inst_fs_mask = 0x3U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_VALID,
	.reserved = false,
};
