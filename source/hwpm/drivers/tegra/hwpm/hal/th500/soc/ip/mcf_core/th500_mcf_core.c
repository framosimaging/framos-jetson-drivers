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

#include "th500_mcf_core.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture th500_mcf_core_inst0_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core0",
		.device_index = TH500_MCFCORE0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore0_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore0_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore0_limit_r(),
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
		.name = "perfmon_mcf_core1",
		.device_index = TH500_MCFCORE1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore1_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore1_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst1_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(2),
		.element_index = 2U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core2",
		.device_index = TH500_MCFCORE2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore2_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore2_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore2_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(3),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core3",
		.device_index = TH500_MCFCORE3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore3_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore3_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore3_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst2_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(4),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core4",
		.device_index = TH500_MCFCORE4_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore4_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore4_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore4_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(5),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core5",
		.device_index = TH500_MCFCORE5_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore5_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore5_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore5_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore5_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst3_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(6),
		.element_index = 6U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core6",
		.device_index = TH500_MCFCORE6_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore6_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore6_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore6_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore6_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(7),
		.element_index = 7U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core7",
		.device_index = TH500_MCFCORE7_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore7_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore7_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore7_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore7_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst4_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(8),
		.element_index = 8U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core8",
		.device_index = TH500_MCFCORE8_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore8_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore8_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore8_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore8_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(9),
		.element_index = 9U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core9",
		.device_index = TH500_MCFCORE9_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore9_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore9_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore9_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore9_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst5_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(10),
		.element_index = 10U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core10",
		.device_index = TH500_MCFCORE10_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore10_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore10_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore10_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore10_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(11),
		.element_index = 11U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core11",
		.device_index = TH500_MCFCORE11_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore11_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore11_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore11_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore11_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst6_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(12),
		.element_index = 12U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core12",
		.device_index = TH500_MCFCORE12_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore12_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore12_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore12_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore12_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(13),
		.element_index = 13U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core13",
		.device_index = TH500_MCFCORE13_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore13_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore13_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore13_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore13_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst7_perfmon_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(14),
		.element_index = 14U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core14",
		.device_index = TH500_MCFCORE14_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore14_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore14_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore14_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore14_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 1U,
		.element_index_mask = BIT(15),
		.element_index = 15U,
		.dt_mmio = NULL,
		.name = "perfmon_mcf_core15",
		.device_index = TH500_MCFCORE15_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_mcfcore15_base_r(),
		.end_abs_pa = addr_map_rpg_pm_mcfcore15_limit_r(),
		.start_pa = addr_map_rpg_pm_mcfcore15_base_r(),
		.end_pa = addr_map_rpg_pm_mcfcore15_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst1_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(2),
		.element_index = 3U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc2_base_r(),
		.end_abs_pa = addr_map_mc2_limit_r(),
		.start_pa = addr_map_mc2_base_r(),
		.end_pa = addr_map_mc2_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(3),
		.element_index = 4U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc3_base_r(),
		.end_abs_pa = addr_map_mc3_limit_r(),
		.start_pa = addr_map_mc3_base_r(),
		.end_pa = addr_map_mc3_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst2_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(4),
		.element_index = 5U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc4_base_r(),
		.end_abs_pa = addr_map_mc4_limit_r(),
		.start_pa = addr_map_mc4_base_r(),
		.end_pa = addr_map_mc4_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(5),
		.element_index = 6U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc5_base_r(),
		.end_abs_pa = addr_map_mc5_limit_r(),
		.start_pa = addr_map_mc5_base_r(),
		.end_pa = addr_map_mc5_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst3_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(6),
		.element_index = 7U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc6_base_r(),
		.end_abs_pa = addr_map_mc6_limit_r(),
		.start_pa = addr_map_mc6_base_r(),
		.end_pa = addr_map_mc6_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(7),
		.element_index = 8U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc7_base_r(),
		.end_abs_pa = addr_map_mc7_limit_r(),
		.start_pa = addr_map_mc7_base_r(),
		.end_pa = addr_map_mc7_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst4_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(8),
		.element_index = 9U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc8_base_r(),
		.end_abs_pa = addr_map_mc8_limit_r(),
		.start_pa = addr_map_mc8_base_r(),
		.end_pa = addr_map_mc8_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(9),
		.element_index = 10U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc9_base_r(),
		.end_abs_pa = addr_map_mc9_limit_r(),
		.start_pa = addr_map_mc9_base_r(),
		.end_pa = addr_map_mc9_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst5_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(10),
		.element_index = 11U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc10_base_r(),
		.end_abs_pa = addr_map_mc10_limit_r(),
		.start_pa = addr_map_mc10_base_r(),
		.end_pa = addr_map_mc10_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(11),
		.element_index = 12U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc11_base_r(),
		.end_abs_pa = addr_map_mc11_limit_r(),
		.start_pa = addr_map_mc11_base_r(),
		.end_pa = addr_map_mc11_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst6_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(12),
		.element_index = 13U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc12_base_r(),
		.end_abs_pa = addr_map_mc12_limit_r(),
		.start_pa = addr_map_mc12_base_r(),
		.end_pa = addr_map_mc12_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(13),
		.element_index = 14U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc13_base_r(),
		.end_abs_pa = addr_map_mc13_limit_r(),
		.start_pa = addr_map_mc13_base_r(),
		.end_pa = addr_map_mc13_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst7_perfmux_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(14),
		.element_index = 15U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc14_base_r(),
		.end_abs_pa = addr_map_mc14_limit_r(),
		.start_pa = addr_map_mc14_base_r(),
		.end_pa = addr_map_mc14_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 1U,
		.element_index_mask = BIT(15),
		.element_index = 16U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc15_base_r(),
		.end_abs_pa = addr_map_mc15_limit_r(),
		.start_pa = addr_map_mc15_base_r(),
		.end_pa = addr_map_mc15_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst0_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst1_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst2_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst3_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst4_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst5_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst6_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mcf_core_inst7_broadcast_element_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mcf_core_alist,
		.alist_size = ARRAY_SIZE(th500_mcf_core_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
struct hwpm_ip_inst th500_mcf_core_inst_static_array[
	TH500_HWPM_IP_MCF_CORE_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc0_base_r(),
				.range_end = addr_map_mc1_limit_r(),
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
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst0_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore0_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore1_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore0_limit_r() -
					addr_map_rpg_pm_mcfcore0_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc2_base_r(),
				.range_end = addr_map_mc3_limit_r(),
				.element_stride = addr_map_mc2_limit_r() -
					addr_map_mc2_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst1_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore2_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore3_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore1_limit_r() -
					addr_map_rpg_pm_mcfcore1_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc4_base_r(),
				.range_end = addr_map_mc5_limit_r(),
				.element_stride = addr_map_mc4_limit_r() -
					addr_map_mc4_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst2_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore4_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore5_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore4_limit_r() -
					addr_map_rpg_pm_mcfcore4_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc6_base_r(),
				.range_end = addr_map_mc7_limit_r(),
				.element_stride = addr_map_mc6_limit_r() -
					addr_map_mc6_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst3_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore6_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore7_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore6_limit_r() -
					addr_map_rpg_pm_mcfcore6_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc8_base_r(),
				.range_end = addr_map_mc9_limit_r(),
				.element_stride = addr_map_mc8_limit_r() -
					addr_map_mc8_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst4_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore8_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore9_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore8_limit_r() -
					addr_map_rpg_pm_mcfcore8_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst5_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc10_base_r(),
				.range_end = addr_map_mc11_limit_r(),
				.element_stride = addr_map_mc10_limit_r() -
					addr_map_mc10_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst5_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore10_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore11_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore10_limit_r() -
					addr_map_rpg_pm_mcfcore10_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst6_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc12_base_r(),
				.range_end = addr_map_mc13_limit_r(),
				.element_stride = addr_map_mc12_limit_r() -
					addr_map_mc12_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst6_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore12_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore13_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore12_limit_r() -
					addr_map_rpg_pm_mcfcore12_base_r() + 1ULL,
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
			TH500_HWPM_IP_MCF_CORE_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mcf_core_inst7_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc14_base_r(),
				.range_end = addr_map_mc15_limit_r(),
				.element_stride = addr_map_mc14_limit_r() -
					addr_map_mc14_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MCF_CORE_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mcf_core_inst7_broadcast_element_static_array,
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
					TH500_HWPM_IP_MCF_CORE_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mcf_core_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_mcfcore14_base_r(),
				.range_end = addr_map_rpg_pm_mcfcore15_limit_r(),
				.element_stride = addr_map_rpg_pm_mcfcore14_limit_r() -
					addr_map_rpg_pm_mcfcore14_base_r() + 1ULL,
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
struct hwpm_ip th500_hwpm_ip_mcf_core = {
	.num_instances = TH500_HWPM_IP_MCF_CORE_NUM_INSTANCES,
	.ip_inst_static_array = th500_mcf_core_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_mc0_base_r(),
			.range_end = addr_map_mc15_limit_r(),
			.inst_stride = addr_map_mc1_limit_r() -
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
			.range_start = addr_map_rpg_pm_mcfcore0_base_r(),
			.range_end = addr_map_rpg_pm_mcfcore15_limit_r(),
			.inst_stride = addr_map_rpg_pm_mcfcore1_limit_r() -
				addr_map_rpg_pm_mcfcore0_base_r() + 1ULL,
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
