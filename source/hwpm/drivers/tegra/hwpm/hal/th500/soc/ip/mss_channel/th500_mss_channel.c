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

#include "th500_mss_channel.h"

#include <tegra_hwpm.h>
#include <hal/th500/soc/th500_soc_perfmon_device_index.h>
#include <hal/th500/soc/th500_soc_regops_allowlist.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture th500_mss_channel_inst0_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parta0",
		.device_index = TH500_MSS_CHANNEL_PARTA0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel0_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel0_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel0_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst1_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parta1",
		.device_index = TH500_MSS_CHANNEL_PARTA1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel1_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel1_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel1_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst2_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parta2",
		.device_index = TH500_MSS_CHANNEL_PARTA2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel2_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel2_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel2_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst3_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parta3",
		.device_index = TH500_MSS_CHANNEL_PARTA3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel3_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel3_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel3_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst4_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partb0",
		.device_index = TH500_MSS_CHANNEL_PARTB0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel4_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel4_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel4_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst5_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partb1",
		.device_index = TH500_MSS_CHANNEL_PARTB1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel5_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel5_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel5_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel5_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst6_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partb2",
		.device_index = TH500_MSS_CHANNEL_PARTB2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel6_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel6_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel6_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel6_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst7_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partb3",
		.device_index = TH500_MSS_CHANNEL_PARTB3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel7_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel7_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel7_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel7_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst8_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partc0",
		.device_index = TH500_MSS_CHANNEL_PARTC0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel8_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel8_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel8_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel8_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst9_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partc1",
		.device_index = TH500_MSS_CHANNEL_PARTC1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel9_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel9_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel9_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel9_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst10_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partc2",
		.device_index = TH500_MSS_CHANNEL_PARTC2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel10_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel10_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel10_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel10_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst11_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partc3",
		.device_index = TH500_MSS_CHANNEL_PARTC3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel11_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel11_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel11_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel11_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst12_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partd0",
		.device_index = TH500_MSS_CHANNEL_PARTD0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel12_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel12_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel12_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel12_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst13_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partd1",
		.device_index = TH500_MSS_CHANNEL_PARTD1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel13_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel13_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel13_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel13_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst14_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partd2",
		.device_index = TH500_MSS_CHANNEL_PARTD2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel14_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel14_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel14_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel14_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst15_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partd3",
		.device_index = TH500_MSS_CHANNEL_PARTD3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel15_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel15_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel15_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel15_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst16_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parte0",
		.device_index = TH500_MSS_CHANNEL_PARTE0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel16_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel16_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel16_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel16_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst17_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parte1",
		.device_index = TH500_MSS_CHANNEL_PARTE1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel17_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel17_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel17_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel17_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst18_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parte2",
		.device_index = TH500_MSS_CHANNEL_PARTE2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel18_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel18_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel18_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel18_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst19_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parte3",
		.device_index = TH500_MSS_CHANNEL_PARTE3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel19_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel19_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel19_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel19_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst20_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partf0",
		.device_index = TH500_MSS_CHANNEL_PARTF0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel20_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel20_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel20_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel20_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst21_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partf1",
		.device_index = TH500_MSS_CHANNEL_PARTF1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel21_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel21_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel21_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel21_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst22_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partf2",
		.device_index = TH500_MSS_CHANNEL_PARTF2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel22_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel22_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel22_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel22_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst23_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partf3",
		.device_index = TH500_MSS_CHANNEL_PARTF3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel23_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel23_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel23_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel23_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst24_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partg0",
		.device_index = TH500_MSS_CHANNEL_PARTG0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel24_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel24_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel24_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel24_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst25_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partg1",
		.device_index = TH500_MSS_CHANNEL_PARTG1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel25_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel25_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel25_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel25_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst26_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partg2",
		.device_index = TH500_MSS_CHANNEL_PARTG2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel26_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel26_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel26_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel26_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst27_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_partg3",
		.device_index = TH500_MSS_CHANNEL_PARTG3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel27_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel27_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel27_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel27_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst28_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(28),
		.element_index = 29U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parth0",
		.device_index = TH500_MSS_CHANNEL_PARTH0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel28_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel28_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel28_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel28_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst29_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parth1",
		.device_index = TH500_MSS_CHANNEL_PARTH1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel29_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel29_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel29_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel29_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst30_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parth2",
		.device_index = TH500_MSS_CHANNEL_PARTH2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel30_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel30_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel30_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel30_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst31_perfmon_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = "perfmon_msschannel_parth3",
		.device_index = TH500_MSS_CHANNEL_PARTH3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_msschannel31_base_r(),
		.end_abs_pa = addr_map_rpg_pm_msschannel31_limit_r(),
		.start_pa = addr_map_rpg_pm_msschannel31_base_r(),
		.end_pa = addr_map_rpg_pm_msschannel31_limit_r(),
		.base_pa = addr_map_rpg_pm_base_r(),
		.alist = th500_perfmon_alist,
		.alist_size = ARRAY_SIZE(th500_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst0_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst1_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc1_base_r(),
		.end_abs_pa = addr_map_mc1_limit_r(),
		.start_pa = addr_map_mc1_base_r(),
		.end_pa = addr_map_mc1_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst2_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc2_base_r(),
		.end_abs_pa = addr_map_mc2_limit_r(),
		.start_pa = addr_map_mc2_base_r(),
		.end_pa = addr_map_mc2_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst3_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc3_base_r(),
		.end_abs_pa = addr_map_mc3_limit_r(),
		.start_pa = addr_map_mc3_base_r(),
		.end_pa = addr_map_mc3_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst4_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc4_base_r(),
		.end_abs_pa = addr_map_mc4_limit_r(),
		.start_pa = addr_map_mc4_base_r(),
		.end_pa = addr_map_mc4_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst5_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.element_index_mask = BIT(5),
		.element_index = 6U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc5_base_r(),
		.end_abs_pa = addr_map_mc5_limit_r(),
		.start_pa = addr_map_mc5_base_r(),
		.end_pa = addr_map_mc5_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst6_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc6_base_r(),
		.end_abs_pa = addr_map_mc6_limit_r(),
		.start_pa = addr_map_mc6_base_r(),
		.end_pa = addr_map_mc6_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst7_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc7_base_r(),
		.end_abs_pa = addr_map_mc7_limit_r(),
		.start_pa = addr_map_mc7_base_r(),
		.end_pa = addr_map_mc7_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst8_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc8_base_r(),
		.end_abs_pa = addr_map_mc8_limit_r(),
		.start_pa = addr_map_mc8_base_r(),
		.end_pa = addr_map_mc8_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst9_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc9_base_r(),
		.end_abs_pa = addr_map_mc9_limit_r(),
		.start_pa = addr_map_mc9_base_r(),
		.end_pa = addr_map_mc9_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst10_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc10_base_r(),
		.end_abs_pa = addr_map_mc10_limit_r(),
		.start_pa = addr_map_mc10_base_r(),
		.end_pa = addr_map_mc10_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst11_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc11_base_r(),
		.end_abs_pa = addr_map_mc11_limit_r(),
		.start_pa = addr_map_mc11_base_r(),
		.end_pa = addr_map_mc11_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst12_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc12_base_r(),
		.end_abs_pa = addr_map_mc12_limit_r(),
		.start_pa = addr_map_mc12_base_r(),
		.end_pa = addr_map_mc12_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst13_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc13_base_r(),
		.end_abs_pa = addr_map_mc13_limit_r(),
		.start_pa = addr_map_mc13_base_r(),
		.end_pa = addr_map_mc13_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst14_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc14_base_r(),
		.end_abs_pa = addr_map_mc14_limit_r(),
		.start_pa = addr_map_mc14_base_r(),
		.end_pa = addr_map_mc14_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst15_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc15_base_r(),
		.end_abs_pa = addr_map_mc15_limit_r(),
		.start_pa = addr_map_mc15_base_r(),
		.end_pa = addr_map_mc15_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst16_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc16_base_r(),
		.end_abs_pa = addr_map_mc16_limit_r(),
		.start_pa = addr_map_mc16_base_r(),
		.end_pa = addr_map_mc16_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst17_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc17_base_r(),
		.end_abs_pa = addr_map_mc17_limit_r(),
		.start_pa = addr_map_mc17_base_r(),
		.end_pa = addr_map_mc17_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst18_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc18_base_r(),
		.end_abs_pa = addr_map_mc18_limit_r(),
		.start_pa = addr_map_mc18_base_r(),
		.end_pa = addr_map_mc18_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst19_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc19_base_r(),
		.end_abs_pa = addr_map_mc19_limit_r(),
		.start_pa = addr_map_mc19_base_r(),
		.end_pa = addr_map_mc19_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst20_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc20_base_r(),
		.end_abs_pa = addr_map_mc20_limit_r(),
		.start_pa = addr_map_mc20_base_r(),
		.end_pa = addr_map_mc20_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst21_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc21_base_r(),
		.end_abs_pa = addr_map_mc21_limit_r(),
		.start_pa = addr_map_mc21_base_r(),
		.end_pa = addr_map_mc21_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst22_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc22_base_r(),
		.end_abs_pa = addr_map_mc22_limit_r(),
		.start_pa = addr_map_mc22_base_r(),
		.end_pa = addr_map_mc22_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst23_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc23_base_r(),
		.end_abs_pa = addr_map_mc23_limit_r(),
		.start_pa = addr_map_mc23_base_r(),
		.end_pa = addr_map_mc23_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst24_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc24_base_r(),
		.end_abs_pa = addr_map_mc24_limit_r(),
		.start_pa = addr_map_mc24_base_r(),
		.end_pa = addr_map_mc24_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst25_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc25_base_r(),
		.end_abs_pa = addr_map_mc25_limit_r(),
		.start_pa = addr_map_mc25_base_r(),
		.end_pa = addr_map_mc25_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst26_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc26_base_r(),
		.end_abs_pa = addr_map_mc26_limit_r(),
		.start_pa = addr_map_mc26_base_r(),
		.end_pa = addr_map_mc26_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst27_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc27_base_r(),
		.end_abs_pa = addr_map_mc27_limit_r(),
		.start_pa = addr_map_mc27_base_r(),
		.end_pa = addr_map_mc27_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst28_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc28_base_r(),
		.end_abs_pa = addr_map_mc28_limit_r(),
		.start_pa = addr_map_mc28_base_r(),
		.end_pa = addr_map_mc28_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst29_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc29_base_r(),
		.end_abs_pa = addr_map_mc29_limit_r(),
		.start_pa = addr_map_mc29_base_r(),
		.end_pa = addr_map_mc29_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst30_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc30_base_r(),
		.end_abs_pa = addr_map_mc30_limit_r(),
		.start_pa = addr_map_mc30_base_r(),
		.end_pa = addr_map_mc30_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst31_perfmux_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 1U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mc31_base_r(),
		.end_abs_pa = addr_map_mc31_limit_r(),
		.start_pa = addr_map_mc31_base_r(),
		.end_pa = addr_map_mc31_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst0_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst1_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst2_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst3_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst4_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst5_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst6_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst7_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst8_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst9_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst10_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
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
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst11_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst12_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst13_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst14_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst15_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst16_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst17_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst18_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst19_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst20_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst21_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst22_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst23_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst24_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst25_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst26_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst27_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst28_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst29_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst30_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture th500_mss_channel_inst31_broadcast_element_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST] = {
	{
		.element_type = IP_ELEMENT_BROADCAST,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_mcb_base_r(),
		.end_abs_pa = addr_map_mcb_limit_r(),
		.start_pa = addr_map_mcb_base_r(),
		.end_pa = addr_map_mcb_limit_r(),
		.base_pa = 0ULL,
		.alist = th500_mss_channel_alist,
		.alist_size = ARRAY_SIZE(th500_mss_channel_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
struct hwpm_ip_inst th500_mss_channel_inst_static_array[
	TH500_HWPM_IP_MSS_CHANNEL_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc0_base_r(),
				.range_end = addr_map_mc0_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst0_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel0_base_r(),
				.range_end = addr_map_rpg_pm_msschannel0_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel0_limit_r() -
					addr_map_rpg_pm_msschannel0_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc1_base_r(),
				.range_end = addr_map_mc1_limit_r(),
				.element_stride = addr_map_mc1_limit_r() -
					addr_map_mc1_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst1_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel1_base_r(),
				.range_end = addr_map_rpg_pm_msschannel1_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel1_limit_r() -
					addr_map_rpg_pm_msschannel1_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc2_base_r(),
				.range_end = addr_map_mc2_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst2_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel2_base_r(),
				.range_end = addr_map_rpg_pm_msschannel2_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel2_limit_r() -
					addr_map_rpg_pm_msschannel2_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc3_base_r(),
				.range_end = addr_map_mc3_limit_r(),
				.element_stride = addr_map_mc3_limit_r() -
					addr_map_mc3_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst3_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel3_base_r(),
				.range_end = addr_map_rpg_pm_msschannel3_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel3_limit_r() -
					addr_map_rpg_pm_msschannel3_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc4_base_r(),
				.range_end = addr_map_mc4_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst4_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel4_base_r(),
				.range_end = addr_map_rpg_pm_msschannel4_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel4_limit_r() -
					addr_map_rpg_pm_msschannel4_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst5_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc5_base_r(),
				.range_end = addr_map_mc5_limit_r(),
				.element_stride = addr_map_mc5_limit_r() -
					addr_map_mc5_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst5_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel5_base_r(),
				.range_end = addr_map_rpg_pm_msschannel5_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel5_limit_r() -
					addr_map_rpg_pm_msschannel5_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst6_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc6_base_r(),
				.range_end = addr_map_mc6_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst6_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel6_base_r(),
				.range_end = addr_map_rpg_pm_msschannel6_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel6_limit_r() -
					addr_map_rpg_pm_msschannel6_base_r() + 1ULL,
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
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst7_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc7_base_r(),
				.range_end = addr_map_mc7_limit_r(),
				.element_stride = addr_map_mc7_limit_r() -
					addr_map_mc7_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst7_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel7_base_r(),
				.range_end = addr_map_rpg_pm_msschannel7_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel7_limit_r() -
					addr_map_rpg_pm_msschannel7_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(8),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst8_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc8_base_r(),
				.range_end = addr_map_mc8_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst8_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst8_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel8_base_r(),
				.range_end = addr_map_rpg_pm_msschannel8_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel8_limit_r() -
					addr_map_rpg_pm_msschannel8_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(9),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst9_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc9_base_r(),
				.range_end = addr_map_mc9_limit_r(),
				.element_stride = addr_map_mc9_limit_r() -
					addr_map_mc9_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst9_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst9_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel9_base_r(),
				.range_end = addr_map_rpg_pm_msschannel9_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel9_limit_r() -
					addr_map_rpg_pm_msschannel9_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(10),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst10_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc10_base_r(),
				.range_end = addr_map_mc10_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst10_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst10_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel10_base_r(),
				.range_end = addr_map_rpg_pm_msschannel10_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel10_limit_r() -
					addr_map_rpg_pm_msschannel10_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(11),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst11_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc11_base_r(),
				.range_end = addr_map_mc11_limit_r(),
				.element_stride = addr_map_mc11_limit_r() -
					addr_map_mc11_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst11_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst11_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel11_base_r(),
				.range_end = addr_map_rpg_pm_msschannel11_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel11_limit_r() -
					addr_map_rpg_pm_msschannel11_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(12),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst12_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc12_base_r(),
				.range_end = addr_map_mc12_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst12_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst12_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel12_base_r(),
				.range_end = addr_map_rpg_pm_msschannel12_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel12_limit_r() -
					addr_map_rpg_pm_msschannel12_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(13),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst13_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc13_base_r(),
				.range_end = addr_map_mc13_limit_r(),
				.element_stride = addr_map_mc13_limit_r() -
					addr_map_mc13_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst13_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst13_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel13_base_r(),
				.range_end = addr_map_rpg_pm_msschannel13_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel13_limit_r() -
					addr_map_rpg_pm_msschannel13_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(14),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst14_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc14_base_r(),
				.range_end = addr_map_mc14_limit_r(),
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst14_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst14_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel14_base_r(),
				.range_end = addr_map_rpg_pm_msschannel14_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel14_limit_r() -
					addr_map_rpg_pm_msschannel14_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(15),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst15_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc15_base_r(),
				.range_end = addr_map_mc15_limit_r(),
				.element_stride = addr_map_mc15_limit_r() -
					addr_map_mc15_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst15_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst15_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel15_base_r(),
				.range_end = addr_map_rpg_pm_msschannel15_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel15_limit_r() -
					addr_map_rpg_pm_msschannel15_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(16),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst16_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc16_base_r(),
				.range_end = addr_map_mc16_limit_r(),
				.element_stride = addr_map_mc16_limit_r() -
					addr_map_mc16_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst16_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst16_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel16_base_r(),
				.range_end = addr_map_rpg_pm_msschannel16_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel16_limit_r() -
					addr_map_rpg_pm_msschannel16_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(17),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst17_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc17_base_r(),
				.range_end = addr_map_mc17_limit_r(),
				.element_stride = addr_map_mc17_limit_r() -
					addr_map_mc17_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst17_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst17_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel17_base_r(),
				.range_end = addr_map_rpg_pm_msschannel17_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel17_limit_r() -
					addr_map_rpg_pm_msschannel17_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(18),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst18_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc18_base_r(),
				.range_end = addr_map_mc18_limit_r(),
				.element_stride = addr_map_mc18_limit_r() -
					addr_map_mc18_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst18_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst18_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel18_base_r(),
				.range_end = addr_map_rpg_pm_msschannel18_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel18_limit_r() -
					addr_map_rpg_pm_msschannel18_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(19),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst19_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc19_base_r(),
				.range_end = addr_map_mc19_limit_r(),
				.element_stride = addr_map_mc19_limit_r() -
					addr_map_mc19_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst19_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst19_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel19_base_r(),
				.range_end = addr_map_rpg_pm_msschannel19_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel19_limit_r() -
					addr_map_rpg_pm_msschannel19_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(20),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst20_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc20_base_r(),
				.range_end = addr_map_mc20_limit_r(),
				.element_stride = addr_map_mc20_limit_r() -
					addr_map_mc20_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst20_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst20_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel20_base_r(),
				.range_end = addr_map_rpg_pm_msschannel20_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel20_limit_r() -
					addr_map_rpg_pm_msschannel20_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(21),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst21_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc21_base_r(),
				.range_end = addr_map_mc21_limit_r(),
				.element_stride = addr_map_mc21_limit_r() -
					addr_map_mc21_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst21_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst21_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel21_base_r(),
				.range_end = addr_map_rpg_pm_msschannel21_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel21_limit_r() -
					addr_map_rpg_pm_msschannel21_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(22),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst22_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc22_base_r(),
				.range_end = addr_map_mc22_limit_r(),
				.element_stride = addr_map_mc22_limit_r() -
					addr_map_mc22_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst22_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst22_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel22_base_r(),
				.range_end = addr_map_rpg_pm_msschannel22_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel22_limit_r() -
					addr_map_rpg_pm_msschannel22_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(23),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst23_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc23_base_r(),
				.range_end = addr_map_mc23_limit_r(),
				.element_stride = addr_map_mc23_limit_r() -
					addr_map_mc23_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst23_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst23_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel23_base_r(),
				.range_end = addr_map_rpg_pm_msschannel23_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel23_limit_r() -
					addr_map_rpg_pm_msschannel23_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(24),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst24_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc24_base_r(),
				.range_end = addr_map_mc24_limit_r(),
				.element_stride = addr_map_mc24_limit_r() -
					addr_map_mc24_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst24_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst24_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel24_base_r(),
				.range_end = addr_map_rpg_pm_msschannel24_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel24_limit_r() -
					addr_map_rpg_pm_msschannel24_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(25),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst25_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc25_base_r(),
				.range_end = addr_map_mc25_limit_r(),
				.element_stride = addr_map_mc25_limit_r() -
					addr_map_mc25_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst25_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst25_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel25_base_r(),
				.range_end = addr_map_rpg_pm_msschannel25_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel25_limit_r() -
					addr_map_rpg_pm_msschannel25_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(26),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst26_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc26_base_r(),
				.range_end = addr_map_mc26_limit_r(),
				.element_stride = addr_map_mc26_limit_r() -
					addr_map_mc26_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst26_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst26_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel26_base_r(),
				.range_end = addr_map_rpg_pm_msschannel26_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel26_limit_r() -
					addr_map_rpg_pm_msschannel26_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(27),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst27_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc27_base_r(),
				.range_end = addr_map_mc27_limit_r(),
				.element_stride = addr_map_mc27_limit_r() -
					addr_map_mc27_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst27_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst27_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel27_base_r(),
				.range_end = addr_map_rpg_pm_msschannel27_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel27_limit_r() -
					addr_map_rpg_pm_msschannel27_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(28),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst28_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc28_base_r(),
				.range_end = addr_map_mc28_limit_r(),
				.element_stride = addr_map_mc28_limit_r() -
					addr_map_mc28_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst28_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst28_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel28_base_r(),
				.range_end = addr_map_rpg_pm_msschannel28_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel28_limit_r() -
					addr_map_rpg_pm_msschannel28_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(29),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst29_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc29_base_r(),
				.range_end = addr_map_mc29_limit_r(),
				.element_stride = addr_map_mc29_limit_r() -
					addr_map_mc29_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst29_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst29_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel29_base_r(),
				.range_end = addr_map_rpg_pm_msschannel29_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel29_limit_r() -
					addr_map_rpg_pm_msschannel29_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(30),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst30_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc30_base_r(),
				.range_end = addr_map_mc30_limit_r(),
				.element_stride = addr_map_mc30_limit_r() -
					addr_map_mc30_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst30_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst30_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel30_base_r(),
				.range_end = addr_map_rpg_pm_msschannel30_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel30_limit_r() -
					addr_map_rpg_pm_msschannel30_base_r() + 1ULL,
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
		.hw_inst_mask = BIT(31),
		.num_core_elements_per_inst =
			TH500_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST,
				.element_static_array =
					th500_mss_channel_inst31_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_mc31_base_r(),
				.range_end = addr_map_mc31_limit_r(),
				.element_stride = addr_map_mc31_limit_r() -
					addr_map_mc31_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					TH500_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST,
				.element_static_array =
					th500_mss_channel_inst31_broadcast_element_static_array,
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
					TH500_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST,
				.element_static_array =
					th500_mss_channel_inst31_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_msschannel31_base_r(),
				.range_end = addr_map_rpg_pm_msschannel31_limit_r(),
				.element_stride = addr_map_rpg_pm_msschannel31_limit_r() -
					addr_map_rpg_pm_msschannel31_base_r() + 1ULL,
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
struct hwpm_ip th500_hwpm_ip_mss_channel = {
	.num_instances = TH500_HWPM_IP_MSS_CHANNEL_NUM_INSTANCES,
	.ip_inst_static_array = th500_mss_channel_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_mc0_base_r(),
			.range_end = addr_map_mc31_limit_r(),
			.inst_stride = addr_map_mc0_limit_r() -
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
			.range_start = addr_map_rpg_pm_msschannel0_base_r(),
			.range_end = addr_map_rpg_pm_msschannel31_limit_r(),
			.inst_stride = addr_map_rpg_pm_msschannel0_limit_r() -
				addr_map_rpg_pm_msschannel0_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK,
	.override_enable = false,
	.inst_fs_mask = 0U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_INVALID,
	.reserved = false,
};
