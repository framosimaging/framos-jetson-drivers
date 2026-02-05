// SPDX-License-Identifier: MIT
/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include "t264_regops_allowlist.h"

struct allowlist t264_perfmon_alist[67] = {
	{0x00000000, true},
	{0x00000004, true},
	{0x00000008, true},
	{0x0000000c, true},
	{0x00000010, true},
	{0x00000014, true},
	{0x00000020, true},
	{0x00000024, true},
	{0x00000028, true},
	{0x0000002c, true},
	{0x00000030, true},
	{0x00000034, true},
	{0x00000040, true},
	{0x00000044, true},
	{0x00000048, true},
	{0x0000004c, true},
	{0x00000050, true},
	{0x00000054, true},
	{0x00000058, true},
	{0x0000005c, true},
	{0x00000060, true},
	{0x00000064, true},
	{0x00000068, true},
	{0x0000006c, true},
	{0x00000070, true},
	{0x00000074, true},
	{0x00000078, true},
	{0x0000007c, true},
	{0x00000080, true},
	{0x00000084, true},
	{0x00000088, true},
	{0x0000008c, true},
	{0x00000090, true},
	{0x00000098, true},
	{0x0000009c, true},
	{0x000000a0, true},
	{0x000000a4, true},
	{0x000000a8, true},
	{0x000000ac, true},
	{0x000000b0, true},
	{0x000000b4, true},
	{0x000000b8, true},
	{0x000000bc, true},
	{0x000000c0, true},
	{0x000000c4, true},
	{0x000000c8, true},
	{0x000000cc, true},
	{0x000000d0, true},
	{0x000000d4, true},
	{0x000000d8, true},
	{0x000000dc, true},
	{0x000000e0, true},
	{0x000000e4, true},
	{0x000000e8, true},
	{0x000000ec, true},
	{0x000000f8, true},
	{0x000000fc, true},
	{0x00000100, true},
	{0x00000108, true},
	{0x00000110, true},
	{0x00000114, true},
	{0x00000118, true},
	{0x0000011c, true},
	{0x00000120, true},
	{0x00000124, true},
	{0x00000128, true},
	{0x00000130, true},
};

struct allowlist t264_pma_res_cmd_slice_rtr_alist[41] = {
	{0x00000858, false},
	{0x00000a00, false},
	{0x00000a10, false},
	{0x00000a14, false},
	{0x00000a20, false},
	{0x00000a24, false},
	{0x00000a28, false},
	{0x00000a2c, false},
	{0x00000a30, false},
	{0x00000a34, false},
	{0x00000a38, false},
	{0x00000a3c, false},
	{0x00001104, false},
	{0x00001110, false},
	{0x00001114, false},
	{0x0000111c, false},
	{0x00001120, false},
	{0x00001124, false},
	{0x00001128, false},
	{0x0000112c, false},
	{0x00001130, false},
	{0x00001134, false},
	{0x00001138, false},
	{0x0000113c, false},
	{0x00001140, false},
	{0x00001144, false},
	{0x00001148, false},
	{0x0000114c, false},
	{0x00001150, false},
	{0x00001154, false},
	{0x00001158, false},
	{0x0000115c, false},
	{0x00001160, false},
	{0x00001164, false},
	{0x00001168, false},
	{0x0000116c, false},
	{0x00001170, false},
	{0x00001174, false},
	{0x00001178, false},
	{0x0000117c, false},
	{0x00000818, false},
};

struct allowlist t264_pma_res_pma_alist[1] = {
	{0x00000858, true},
};

struct allowlist t264_rtr_alist[2] = {
	{0x00000080, false},
	{0x000000a4, false},
};

struct allowlist t264_vic_alist[8] = {
	{0x00001088, true},
	{0x000010a8, true},
	{0x0000cb94, true},
	{0x0000cb80, true},
	{0x0000cb84, true},
	{0x0000cb88, true},
	{0x0000cb8c, true},
	{0x0000cb90, true},
};

struct allowlist t264_pva_pm_alist[10] = {
	{0x0000800c, true},
	{0x00008010, true},
	{0x00008014, true},
	{0x00008018, true},
	{0x0000801c, true},
	{0x00008020, true},
	{0x00008024, true},
	{0x00008028, true},
	{0x0000802c, true},
	{0x00008030, true},
};

struct allowlist t264_mss_channel_alist[2] = {
	{0x00008914, true},
	{0x00008918, true},
};

struct allowlist t264_mss_hub_alist[3] = {
	{0x00006f3c, true},
	{0x00006f34, true},
	{0x00006f38, true},
};

struct allowlist t264_ocu_alist[1] = {
	{0x00000058, true},
};

struct allowlist t264_smmu_alist[1] = {
	{0x00005000, true},
};

struct allowlist t264_ucf_msw_cbridge_alist[1] = {
	{0x0000891c, true},
};

struct allowlist t264_ucf_msn_msw0_alist[2] = {
	{0x00000000, true},
	{0x00000008, true},
};

struct allowlist t264_ucf_msn_msw1_alist[2] = {
	{0x00000010, true},
	{0x00000018, true},
};

struct allowlist t264_ucf_msw_slc_alist[1] = {
	{0x00000000, true},
};

struct allowlist t264_ucf_psn_psw_alist[2] = {
	{0x00000000, true},
	{0x00000008, true},
};

struct allowlist t264_ucf_csw_alist[2] = {
	{0x00000000, true},
	{0x00000008, true},
};

struct allowlist t264_cpucore_alist[4] = {
	{0x00000000, true},
	{0x00000008, true},
	{0x00000010, true},
	{0x00000018, true},
};

struct allowlist t264_vi_alist[5] = {
	{0x00030008, true},
	{0x0003000c, true},
	{0x00030010, true},
	{0x00030014, true},
	{0x00030018, true},
};

struct allowlist t264_isp_alist[5] = {
	{0x00030008, true},
	{0x0003000c, true},
	{0x00030010, true},
	{0x00030014, true},
	{0x00030018, true},
};
