/* SPDX-License-Identifier: MIT */
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

#ifndef T264_HWPM_REGOPS_ALLOWLIST_H
#define T264_HWPM_REGOPS_ALLOWLIST_H

#include <tegra_hwpm.h>

extern struct allowlist t264_perfmon_alist[67];
extern struct allowlist t264_pma_res_cmd_slice_rtr_alist[41];
extern struct allowlist t264_pma_res_pma_alist[1];
extern struct allowlist t264_rtr_alist[2];
extern struct allowlist t264_vic_alist[8];
extern struct allowlist t264_pva_pm_alist[10];
extern struct allowlist t264_mss_channel_alist[2];
extern struct allowlist t264_mss_hub_alist[3];
extern struct allowlist t264_ocu_alist[1];
extern struct allowlist t264_smmu_alist[1];
extern struct allowlist t264_ucf_msw_cbridge_alist[1];
extern struct allowlist t264_ucf_msn_msw0_alist[2];
extern struct allowlist t264_ucf_msn_msw1_alist[2];
extern struct allowlist t264_ucf_msw_slc_alist[1];
extern struct allowlist t264_ucf_psn_psw_alist[2];
extern struct allowlist t264_ucf_csw_alist[2];
extern struct allowlist t264_cpucore_alist[4];
extern struct allowlist t264_vi_alist[5];
extern struct allowlist t264_isp_alist[5];

#endif /* T264_HWPM_REGOPS_ALLOWLIST_H */
