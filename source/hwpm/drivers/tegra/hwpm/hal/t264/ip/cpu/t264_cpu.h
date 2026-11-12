/* SPDX-License-Identifier: MIT */
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

#ifndef T264_HWPM_IP_CPU_H
#define T264_HWPM_IP_CPU_H

#if defined(CONFIG_T264_HWPM_IP_CPU)
#define T264_HWPM_ACTIVE_IP_CPU	T264_HWPM_IP_CPU,

/* This data should ideally be available in HW headers */
#define T264_HWPM_IP_CPU_NUM_INSTANCES               14U
#define T264_HWPM_IP_CPU_NUM_CORE_ELEMENT_PER_INST   1U
#define T264_HWPM_IP_CPU_NUM_PERFMON_PER_INST        1U
#define T264_HWPM_IP_CPU_NUM_PERFMUX_PER_INST        1U
#define T264_HWPM_IP_CPU_NUM_BROADCAST_PER_INST      0U

extern struct hwpm_ip t264_hwpm_ip_cpu;

#define addr_map_rpg_pm_cpu_core_size()		BIT(0x00000014U)

#define addr_map_rpg_pm_cpu_core0_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r())
#define addr_map_rpg_pm_cpu_core0_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x00FFF)
#define addr_map_rpg_pm_cpu_core1_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x10000)
#define addr_map_rpg_pm_cpu_core1_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x10FFF)
#define addr_map_rpg_pm_cpu_core2_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x20000)
#define addr_map_rpg_pm_cpu_core2_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x20FFF)
#define addr_map_rpg_pm_cpu_core3_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x30000)
#define addr_map_rpg_pm_cpu_core3_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x30FFF)
#define addr_map_rpg_pm_cpu_core4_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x40000)
#define addr_map_rpg_pm_cpu_core4_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x40FFF)
#define addr_map_rpg_pm_cpu_core5_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x50000)
#define addr_map_rpg_pm_cpu_core5_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x50FFF)
#define addr_map_rpg_pm_cpu_core6_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x60000)
#define addr_map_rpg_pm_cpu_core6_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x60FFF)
#define addr_map_rpg_pm_cpu_core7_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x70000)
#define addr_map_rpg_pm_cpu_core7_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x70FFF)
#define addr_map_rpg_pm_cpu_core8_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x80000)
#define addr_map_rpg_pm_cpu_core8_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x80FFF)
#define addr_map_rpg_pm_cpu_core9_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x90000)
#define addr_map_rpg_pm_cpu_core9_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0x90FFF)
#define addr_map_rpg_pm_cpu_core10_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xa0000)
#define addr_map_rpg_pm_cpu_core10_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xa0FFF)
#define addr_map_rpg_pm_cpu_core11_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xb0000)
#define addr_map_rpg_pm_cpu_core11_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xb0FFF)
#define addr_map_rpg_pm_cpu_core12_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xc0000)
#define addr_map_rpg_pm_cpu_core12_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xc0FFF)
#define addr_map_rpg_pm_cpu_core13_base_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xd0000)
#define addr_map_rpg_pm_cpu_core13_limit_r()	\
	(addr_map_rpg_pm_cpu_core_base_r() + 0xd0FFF)

#else
#define T264_HWPM_ACTIVE_IP_CPU
#endif

#endif /* T264_HWPM_IP_CPU_H */
