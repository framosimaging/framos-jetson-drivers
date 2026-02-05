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

#include "t264_cpu.h"

#include <tegra_hwpm.h>
#include <hal/t264/t264_regops_allowlist.h>
#include <hal/t264/t264_perfmon_device_index.h>
#include <hal/t264/hw/t264_addr_map_soc_hwpm.h>

static struct hwpm_ip_aperture t264_cpu_inst0_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore0",
		.device_index = T264_CPU_CORE0_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core0_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core0_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core0_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core0_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst1_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore1",
		.device_index = T264_CPU_CORE1_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core1_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core1_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core1_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core1_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst2_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore2",
		.device_index = T264_CPU_CORE2_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core2_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core2_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core2_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core2_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst3_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore3",
		.device_index = T264_CPU_CORE3_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core3_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core3_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core3_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core3_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst4_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore4",
		.device_index = T264_CPU_CORE4_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core4_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core4_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core4_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core4_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst5_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore5",
		.device_index = T264_CPU_CORE5_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core5_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core5_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core5_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core5_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst6_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore6",
		.device_index = T264_CPU_CORE6_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core6_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core6_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core6_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core6_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst7_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore7",
		.device_index = T264_CPU_CORE7_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core7_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core7_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core7_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core7_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst8_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore8",
		.device_index = T264_CPU_CORE8_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core8_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core8_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core8_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core8_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst9_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore9",
		.device_index = T264_CPU_CORE9_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core9_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core9_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core9_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core9_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst10_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore10",
		.device_index = T264_CPU_CORE10_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core10_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core10_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core10_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core10_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst11_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore11",
		.device_index = T264_CPU_CORE11_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core11_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core11_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core11_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core11_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst12_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore12",
		.device_index = T264_CPU_CORE12_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core12_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core12_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core12_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core12_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst13_perfmon_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST] = {
	{
		.element_type = HWPM_ELEMENT_PERFMON,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = "perfmon_cpucore13",
		.device_index = T264_CPU_CORE13_PERFMON_DEVICE_NODE_INDEX,
		.start_abs_pa = addr_map_rpg_pm_cpu_core13_base_r(),
		.end_abs_pa = addr_map_rpg_pm_cpu_core13_limit_r(),
		.start_pa = addr_map_rpg_pm_cpu_core13_base_r(),
		.end_pa = addr_map_rpg_pm_cpu_core13_limit_r(),
		.base_pa = addr_map_rpg_pm_cpu_core_base_r(),
		.alist = t264_perfmon_alist,
		.alist_size = ARRAY_SIZE(t264_perfmon_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst0_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore0_base_r(),
		.end_abs_pa = addr_map_cpucore0_base_r() + addr_map_cpucore0_base_size_v(),
		.start_pa = addr_map_cpucore0_base_r(),
		.end_pa = addr_map_cpucore0_base_r() + addr_map_cpucore0_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst1_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore1_base_r(),
		.end_abs_pa = addr_map_cpucore1_base_r() + addr_map_cpucore1_base_size_v(),
		.start_pa = addr_map_cpucore1_base_r(),
		.end_pa = addr_map_cpucore1_base_r() + addr_map_cpucore1_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst2_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore2_base_r(),
		.end_abs_pa = addr_map_cpucore2_base_r() + addr_map_cpucore2_base_size_v(),
		.start_pa = addr_map_cpucore2_base_r(),
		.end_pa = addr_map_cpucore2_base_r() + addr_map_cpucore2_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst3_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore3_base_r(),
		.end_abs_pa = addr_map_cpucore3_base_r() + addr_map_cpucore3_base_size_v(),
		.start_pa = addr_map_cpucore3_base_r(),
		.end_pa = addr_map_cpucore3_base_r() + addr_map_cpucore3_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst4_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore4_base_r(),
		.end_abs_pa = addr_map_cpucore4_base_r() + addr_map_cpucore4_base_size_v(),
		.start_pa = addr_map_cpucore4_base_r(),
		.end_pa = addr_map_cpucore4_base_r() + addr_map_cpucore4_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst5_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore5_base_r(),
		.end_abs_pa = addr_map_cpucore5_base_r() + addr_map_cpucore5_base_size_v(),
		.start_pa = addr_map_cpucore5_base_r(),
		.end_pa = addr_map_cpucore5_base_r() + addr_map_cpucore5_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst6_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore6_base_r(),
		.end_abs_pa = addr_map_cpucore6_base_r() + addr_map_cpucore6_base_size_v(),
		.start_pa = addr_map_cpucore6_base_r(),
		.end_pa = addr_map_cpucore6_base_r() + addr_map_cpucore6_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst7_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore7_base_r(),
		.end_abs_pa = addr_map_cpucore7_base_r() + addr_map_cpucore7_base_size_v(),
		.start_pa = addr_map_cpucore7_base_r(),
		.end_pa = addr_map_cpucore7_base_r() + addr_map_cpucore7_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst8_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore8_base_r(),
		.end_abs_pa = addr_map_cpucore8_base_r() + addr_map_cpucore8_base_size_v(),
		.start_pa = addr_map_cpucore8_base_r(),
		.end_pa = addr_map_cpucore8_base_r() + addr_map_cpucore8_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst9_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore9_base_r(),
		.end_abs_pa = addr_map_cpucore9_base_r() + addr_map_cpucore9_base_size_v(),
		.start_pa = addr_map_cpucore9_base_r(),
		.end_pa = addr_map_cpucore9_base_r() + addr_map_cpucore9_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst10_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore10_base_r(),
		.end_abs_pa = addr_map_cpucore10_base_r() + addr_map_cpucore10_base_size_v(),
		.start_pa = addr_map_cpucore10_base_r(),
		.end_pa = addr_map_cpucore10_base_r() + addr_map_cpucore10_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst11_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore11_base_r(),
		.end_abs_pa = addr_map_cpucore11_base_r() + addr_map_cpucore11_base_size_v(),
		.start_pa = addr_map_cpucore11_base_r(),
		.end_pa = addr_map_cpucore11_base_r() + addr_map_cpucore11_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst12_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore12_base_r(),
		.end_abs_pa = addr_map_cpucore12_base_r() + addr_map_cpucore12_base_size_v(),
		.start_pa = addr_map_cpucore12_base_r(),
		.end_pa = addr_map_cpucore12_base_r() + addr_map_cpucore12_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

static struct hwpm_ip_aperture t264_cpu_inst13_perfmux_element_static_array[
	T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST] = {
	{
		.element_type = IP_ELEMENT_PERFMUX,
		.aperture_index = 0U,
		.element_index_mask = BIT(0),
		.element_index = 0U,
		.dt_mmio = NULL,
		.name = {'\0'},
		.start_abs_pa = addr_map_cpucore13_base_r(),
		.end_abs_pa = addr_map_cpucore13_base_r() + addr_map_cpucore13_base_size_v(),
		.start_pa = addr_map_cpucore13_base_r(),
		.end_pa = addr_map_cpucore13_base_r() + addr_map_cpucore13_base_size_v(),
		.base_pa = 0ULL,
		.alist = t264_cpucore_alist,
		.alist_size = ARRAY_SIZE(t264_cpucore_alist),
		.fake_registers = NULL,
	},
};

/* IP instance array */
static struct hwpm_ip_inst t264_cpu_inst_static_array[
	T264_HWPM_IP_CPU_NUM_INSTANCES] = {
	{
		.hw_inst_mask = BIT(0),
		.num_core_elements_per_inst =
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst0_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore0_base_r(),
				.range_end = addr_map_cpucore0_base_r() + addr_map_cpucore0_base_size_v(),
				.element_stride = addr_map_cpucore0_base_r() + addr_map_cpucore0_base_size_v() -
					addr_map_cpucore0_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst0_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core0_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core0_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core0_limit_r() -
					addr_map_rpg_pm_cpu_core0_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst1_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore1_base_r(),
				.range_end = addr_map_cpucore1_base_r() + addr_map_cpucore1_base_size_v(),
				.element_stride = addr_map_cpucore1_base_r() + addr_map_cpucore1_base_size_v() -
					addr_map_cpucore1_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst1_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core1_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core1_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core1_limit_r() -
					addr_map_rpg_pm_cpu_core1_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst2_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore2_base_r(),
				.range_end = addr_map_cpucore2_base_r() + addr_map_cpucore2_base_size_v(),
				.element_stride = addr_map_cpucore2_base_r() + addr_map_cpucore2_base_size_v() -
					addr_map_cpucore2_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst2_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core2_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core2_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core2_limit_r() -
					addr_map_rpg_pm_cpu_core2_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst3_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore3_base_r(),
				.range_end = addr_map_cpucore3_base_r() + addr_map_cpucore3_base_size_v(),
				.element_stride = addr_map_cpucore3_base_r() + addr_map_cpucore3_base_size_v() -
					addr_map_cpucore3_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst3_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core3_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core3_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core3_limit_r() -
					addr_map_rpg_pm_cpu_core3_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst4_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore4_base_r(),
				.range_end = addr_map_cpucore4_base_r() + addr_map_cpucore4_base_size_v(),
				.element_stride = addr_map_cpucore4_base_r() + addr_map_cpucore4_base_size_v() -
					addr_map_cpucore4_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst4_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core4_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core4_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core4_limit_r() -
					addr_map_rpg_pm_cpu_core4_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst5_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore5_base_r(),
				.range_end = addr_map_cpucore5_base_r() + addr_map_cpucore5_base_size_v(),
				.element_stride = addr_map_cpucore5_base_r() + addr_map_cpucore5_base_size_v() -
					addr_map_cpucore5_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst5_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core5_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core5_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core5_limit_r() -
					addr_map_rpg_pm_cpu_core5_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst6_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore6_base_r(),
				.range_end = addr_map_cpucore6_base_r() + addr_map_cpucore6_base_size_v(),
				.element_stride = addr_map_cpucore6_base_r() + addr_map_cpucore6_base_size_v() -
					addr_map_cpucore6_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst6_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core6_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core6_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core6_limit_r() -
					addr_map_rpg_pm_cpu_core6_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst7_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore7_base_r(),
				.range_end = addr_map_cpucore7_base_r() + addr_map_cpucore7_base_size_v(),
				.element_stride = addr_map_cpucore7_base_r() + addr_map_cpucore7_base_size_v() -
					addr_map_cpucore7_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst7_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core7_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core7_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core7_limit_r() -
					addr_map_rpg_pm_cpu_core7_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst8_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore8_base_r(),
				.range_end = addr_map_cpucore8_base_r() + addr_map_cpucore8_base_size_v(),
				.element_stride = addr_map_cpucore8_base_r() + addr_map_cpucore8_base_size_v() -
					addr_map_cpucore8_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst8_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core8_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core8_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core8_limit_r() -
					addr_map_rpg_pm_cpu_core8_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst9_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore9_base_r(),
				.range_end = addr_map_cpucore9_base_r() + addr_map_cpucore9_base_size_v(),
				.element_stride = addr_map_cpucore9_base_r() + addr_map_cpucore9_base_size_v() -
					addr_map_cpucore9_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst9_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core9_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core9_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core9_limit_r() -
					addr_map_rpg_pm_cpu_core9_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst10_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore10_base_r(),
				.range_end = addr_map_cpucore10_base_r() + addr_map_cpucore10_base_size_v(),
				.element_stride = addr_map_cpucore10_base_r() + addr_map_cpucore10_base_size_v() -
					addr_map_cpucore10_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst10_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core10_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core10_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core10_limit_r() -
					addr_map_rpg_pm_cpu_core10_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst11_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore11_base_r(),
				.range_end = addr_map_cpucore11_base_r() + addr_map_cpucore11_base_size_v(),
				.element_stride = addr_map_cpucore11_base_r() + addr_map_cpucore11_base_size_v() -
					addr_map_cpucore11_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst11_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core11_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core11_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core11_limit_r() -
					addr_map_rpg_pm_cpu_core11_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst12_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore12_base_r(),
				.range_end = addr_map_cpucore12_base_r() + addr_map_cpucore12_base_size_v(),
				.element_stride = addr_map_cpucore12_base_r() + addr_map_cpucore12_base_size_v() -
					addr_map_cpucore12_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst12_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core12_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core12_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core12_limit_r() -
					addr_map_rpg_pm_cpu_core12_base_r() + 1ULL,
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
			T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST,
		.element_info = {
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST,
				.element_static_array =
					t264_cpu_inst13_perfmux_element_static_array,
				/* NOTE: range should be in ascending order */
				.range_start = addr_map_cpucore13_base_r(),
				.range_end = addr_map_cpucore13_base_r() + addr_map_cpucore13_base_size_v(),
				.element_stride = addr_map_cpucore13_base_r() + addr_map_cpucore13_base_size_v() -
					addr_map_cpucore13_base_r() + 1ULL,
				.element_slots = 0U,
				.element_arr = NULL,
			},
			/*
			 * Instance info corresponding to
			 * TEGRA_HWPM_APERTURE_TYPE_BROADCAST
			 */
			{
				.num_element_per_inst =
					T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST,
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
					T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST,
				.element_static_array =
					t264_cpu_inst13_perfmon_element_static_array,
				.range_start = addr_map_rpg_pm_cpu_core13_base_r(),
				.range_end = addr_map_rpg_pm_cpu_core13_limit_r(),
				.element_stride = addr_map_rpg_pm_cpu_core13_limit_r() -
					addr_map_rpg_pm_cpu_core13_base_r() + 1ULL,
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
struct hwpm_ip t264_hwpm_ip_cpu = {
	.num_instances = T264_HWPM_IP_CPU_NUM_INSTANCES,
	.ip_inst_static_array = t264_cpu_inst_static_array,

	.inst_aperture_info = {
		/*
		 * Instance info corresponding to
		 * TEGRA_HWPM_APERTURE_TYPE_PERFMUX
		 */
		{
			/* NOTE: range should be in ascending order */
			.range_start = addr_map_cpucore0_base_r(),
			.range_end = addr_map_cpucore13_base_r() + addr_map_cpucore13_base_size_v(),
			.inst_stride = addr_map_cpucore0_base_r() + addr_map_cpucore0_base_size_v() -
				addr_map_cpucore0_base_r() + 1ULL,
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
			.range_start = addr_map_rpg_pm_cpu_core0_base_r(),
			.range_end = addr_map_rpg_pm_cpu_core13_limit_r(),
			.inst_stride = addr_map_rpg_pm_cpu_core0_limit_r() -
				addr_map_rpg_pm_cpu_core0_base_r() + 1ULL,
			.inst_slots = 0U,
			.inst_arr = NULL,
		},
	},

	.dependent_fuse_mask = TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK |
		TEGRA_HWPM_FUSE_OPT_HWPM_DISABLE_MASK,
	.override_enable = false,
	.inst_fs_mask = 0U,
	.resource_status = TEGRA_HWPM_RESOURCE_STATUS_INVALID,
	.reserved = false,
};
