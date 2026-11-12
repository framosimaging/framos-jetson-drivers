/* SPDX-License-Identifier: MIT */
/* SPDX-FileCopyrightText: Copyright (c) 2023, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
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

#ifndef TH500_HWPM_SOC_INTERNAL_H
#define TH500_HWPM_SOC_INTERNAL_H

enum tegra_soc_hwpm_ip;
enum tegra_soc_hwpm_resource;
struct tegra_soc_hwpm;
struct hwpm_ip_aperture;

int th500_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available);
int th500_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm);

int th500_hwpm_soc_get_rtr_pma_perfmux_ptr(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture **rtr_perfmux_ptr,
	struct hwpm_ip_aperture **pma_perfmux_ptr);

int th500_hwpm_soc_init_prod_values(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_disable_cg(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_enable_cg(struct tegra_soc_hwpm *hwpm);

int th500_hwpm_soc_check_status(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_disable_triggers(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_perfmon_enable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);
int th500_hwpm_soc_perfmon_disable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);

int th500_hwpm_soc_disable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_enable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_invalidate_mem_config(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_stream_mem_bytes(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_disable_pma_streaming(struct tegra_soc_hwpm *hwpm);
int th500_hwpm_soc_update_mem_bytes_get_ptr(struct tegra_soc_hwpm *hwpm,
	u64 mem_bump);
int th500_hwpm_soc_get_mem_bytes_put_ptr(struct tegra_soc_hwpm *hwpm,
	u64 *mem_head_ptr);
int th500_hwpm_soc_membuf_overflow_status(struct tegra_soc_hwpm *hwpm,
	u32 *overflow_status);

#endif /* TH500_HWPM_SOC_INTERNAL_H */
