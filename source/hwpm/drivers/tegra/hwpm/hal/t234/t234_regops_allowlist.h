/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef T234_HWPM_REGOPS_ALLOWLIST_H
#define T234_HWPM_REGOPS_ALLOWLIST_H

#include <tegra_hwpm.h>

extern struct allowlist t234_perfmon_alist[67];
extern struct allowlist t234_pma_res_cmd_slice_rtr_alist[86];
extern struct allowlist t234_pma_res_pma_alist[1];
extern struct allowlist t234_rtr_alist[8];
extern struct allowlist t234_vi_thi_alist[7];
extern struct allowlist t234_isp_thi_alist[7];
extern struct allowlist t234_vic_alist[9];
extern struct allowlist t234_ofa_alist[8];
extern struct allowlist t234_pva0_pm_alist[9];
extern struct allowlist t234_nvdla_alist[37];
extern struct allowlist t234_mgbe_alist[2];
extern struct allowlist t234_nvdec_alist[8];
extern struct allowlist t234_nvenc_alist[9];
extern struct allowlist t234_pcie_ctl_alist[2];
extern struct allowlist t234_disp_alist[3];
extern struct allowlist t234_mss_channel_alist[2];
extern struct allowlist t234_mss_nvlink_alist[33];
extern struct allowlist t234_mc0to7_res_mss_iso_niso_hub_alist[2];
extern struct allowlist t234_mc8_res_mss_iso_niso_hub_alist[1];
extern struct allowlist t234_mcb_res_mss_iso_niso_hub_alist[3];
extern struct allowlist t234_mcb_mss_mcf_alist[4];
extern struct allowlist t234_mc0to1_mss_mcf_alist[3];
extern struct allowlist t234_mc2to7_mss_mcf_alist[1];

#endif /* T234_HWPM_REGOPS_ALLOWLIST_H */
