/* SPDX-License-Identifier: MIT */
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

#ifndef TH500_HWPM_REGOPS_ALLOWLIST_H
#define TH500_HWPM_REGOPS_ALLOWLIST_H

#include <tegra_hwpm.h>

extern struct allowlist th500_perfmon_alist[76];
extern struct allowlist th500_pma_res_cmd_slice_rtr_alist[56];
extern struct allowlist th500_pma_res_pma_alist[1];
extern struct allowlist th500_rtr_alist[18];
extern struct allowlist th500_nvlrx_alist[5];
extern struct allowlist th500_nvltx_alist[3];
extern struct allowlist th500_nvlctrl_alist[2];
extern struct allowlist th500_smmu_alist[1];
extern struct allowlist th500_c2c_alist[5];
extern struct allowlist th500_pcie_xtlq_alist[1];
extern struct allowlist th500_pcie_xtlrc_alist[1];
extern struct allowlist th500_pcie_xalrc_alist[1];
extern struct allowlist th500_mss_channel_alist[2];
extern struct allowlist th500_mcf_core_alist[2];
extern struct allowlist th500_mcf_clink_alist[3];
extern struct allowlist th500_mcf_c2c_alist[3];
extern struct allowlist th500_mcf_soc_alist[3];
extern struct allowlist th500_mss_hub_alist[3];
extern struct allowlist th500_cl2_alist[4];

#endif /* TH500_HWPM_REGOPS_ALLOWLIST_H */
