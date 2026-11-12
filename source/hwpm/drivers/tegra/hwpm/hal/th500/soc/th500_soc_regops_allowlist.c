// SPDX-License-Identifier: MIT
/*
 * SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include "th500_soc_regops_allowlist.h"

struct allowlist th500_perfmon_alist[76] = {
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
	{0x0000012c, true},
	{0x00000138, true},
	{0x00000140, true},
	{0x00000148, true},
	{0x00000150, true},
	{0x00000154, true},
	{0x0000015c, true},
	{0x00000144, true},
	{0x0000014c, true},
};

struct allowlist th500_pma_res_cmd_slice_rtr_alist[56] = {
	{0x00000800, false},
	{0x00000a00, false},
	{0x00000a10, false},
	{0x00000a20, false},
	{0x00000a24, false},
	{0x00000a38, false},
	{0x00000a3c, false},
	{0x00000a40, false},
	{0x00000a44, false},
	{0x00000a48, false},
	{0x00000a4c, false},
	{0x00000a50, false},
	{0x00000a54, false},
	{0x00000608, false},
	{0x00000a30, false},
	{0x00000a34, false},
	{0x00000a58, false},
	{0x00000a5c, false},
	{0x00000ae4, false},
	{0x00000af0, false},
	{0x00000af4, false},
	{0x00000afc, false},
	{0x00000b00, false},
	{0x00000b04, false},
	{0x00000b08, false},
	{0x00000b0c, false},
	{0x00000b10, false},
	{0x00000b14, false},
	{0x00000b18, false},
	{0x00000b1c, false},
	{0x00000b20, false},
	{0x00000b24, false},
	{0x00000b28, false},
	{0x00000b2c, false},
	{0x00000b30, false},
	{0x00000b34, false},
	{0x00000b38, false},
	{0x00000b3c, false},
	{0x00000b40, false},
	{0x00000b44, false},
	{0x00000b48, false},
	{0x00000b4c, false},
	{0x00000b50, false},
	{0x00000b54, false},
	{0x00000b58, false},
	{0x00000b5c, false},
	{0x00000ae0, false},
	{0x00000aec, false},
	{0x00000af8, false},
	{0x00000b60, false},
	{0x0000075c, false},
	{0x00000700, false},
	{0x00001ffc, false},
	{0x00000604, false},
	{0x00000600, false},
	{0x000009f0, false},
};

struct allowlist th500_pma_res_pma_alist[1] = {
	{0x00000800, true},
};

struct allowlist th500_rtr_alist[18] = {
	{0x0000001c, false},
	{0x0000005c, false},
	{0x00000044, false},
	{0x00000010, false},
	{0x00000050, false},
	{0x00000048, false},
	{0x00000008, false},
	{0x0000004c, false},
	{0x00000060, false},
	{0x00000064, false},
	{0x00000068, false},
	{0x0000006c, false},
	{0x00000070, false},
	{0x00000074, false},
	{0x00000078, false},
	{0x0000007c, false},
	{0x00000080, false},
	{0x00000084, false},
};

struct allowlist th500_nvlrx_alist[5] = {
	{0x00003340, false},
	{0x00000d00, false},
	{0x00000d04, false},
	{0x00001d00, false},
	{0x00001d04, false},
};

struct allowlist th500_nvltx_alist[3] = {
	{0x00000180, false},
	{0x00001500, false},
	{0x00001504, false},
};

struct allowlist th500_nvlctrl_alist[2] = {
	{0x00000804, false},
	{0x00000808, false},
};

struct allowlist th500_smmu_alist[1] = {
	{0x00005000, false},
};

struct allowlist th500_c2c_alist[5] = {
	/* Disable C2CS0 and C2CS1 -- Broadcast apertures for now. See Bug 4411532 */
	/* {0x00000028, false}, */
	{0x000000ec, false},
	{0x000000f0, false},
	{0x000000f4, false},
	{0x000000f8, false},
	{0x000000fc, false},
};

struct allowlist th500_pcie_xtlq_alist[1] = {
	{0x000039e0, true},
};

struct allowlist th500_pcie_xtlrc_alist[1] = {
	{0x000004e0, true},
};

struct allowlist th500_pcie_xalrc_alist[1] = {
	{0x00000470, true},
};

struct allowlist th500_mss_channel_alist[2] = {
	{0x00008914, false},
	{0x00008918, false},
};

struct allowlist th500_mcf_core_alist[2] = {
	{0x0000d604, false},
	{0x0000d600, false},
};

struct allowlist th500_mcf_clink_alist[3] = {
	{0x0000d610, false},
	{0x0000d614, false},
	{0x0000e430, false},
};

struct allowlist th500_mcf_c2c_alist[3] = {
	{0x0000d608, false},
	{0x0000d60c, false},
	{0x0000e430, false},
};

struct allowlist th500_mcf_soc_alist[3] = {
	{0x0000d620, false},
	{0x0000d618, false},
	{0x0000d61c, false},
};

struct allowlist th500_mss_hub_alist[3] = {
	{0x00006f34, false},
	{0x00006f38, false},
	{0x00006f3c, false},
};

struct allowlist th500_cl2_alist[4] = {
	{0x00000550, false},
	{0x00000578, false},
	{0x00000750, false},
	{0x00000778, false},
};
