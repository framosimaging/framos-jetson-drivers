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

#include <tegra_hwpm_static_analysis.h>
#include <tegra_hwpm_timers.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>

#include <hal/t264/t264_internal.h>
#include <hal/t264/hw/t264_pmasys_soc_hwpm.h>
#include <hal/t264/hw/t264_pmmsys_soc_hwpm.h>

#define TEGRA_HWPM_CBLOCK_CHANNEL_TO_CMD_SLICE(cblock, channel)		\
	(((cblock) * pmmsys_num_channels_per_cblock_v()) + (channel))
#define TEGRA_HWPM_MAX_SUPPORTED_DGS	256U
#define TEGRA_HWPM_NUM_DG_STATUS_PER_REG				\
	(TEGRA_HWPM_MAX_SUPPORTED_DGS /					\
		pmmsys_router_user_dgmap_status_secure__size_1_v())

int t264_hwpm_perfmon_enable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon)
{
	u32 reg_val;
	u32 cblock = 0U;
	u32 channel = 0U;
	u32 dg_idx = 0U;
	u32 config_dgmap = 0U;
	u32 dgmap_status_reg_idx = 0U, dgmap_status_reg_dgidx = 0U;
	u32 retries = 10U;
	u32 sleep_msecs = 10U;
	int err = 0;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		NULL);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	/* Enable */
	tegra_hwpm_dbg(hwpm, hwpm_dbg_bind,
		"Enabling PERFMON(0x%llx - 0x%llx)",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa);

	/*
	 * HWPM readl function expects register address relative to
	 * perfmon group base address.
	 * Hence use enginestatus offset + perfmon base_pa as the register
	 */

	tegra_hwpm_readl(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_enginestatus_o(),
			perfmon->base_pa), &reg_val);
	reg_val = set_field(reg_val, pmmsys_enginestatus_enable_m(),
		pmmsys_enginestatus_enable_out_f());
	tegra_hwpm_writel(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_enginestatus_o(),
			perfmon->base_pa), reg_val);

	/*
	 * HWPM readl function expects register address relative to
	 * perfmon group base address.
	 * Hence use secure_config offset + perfmon base_pa as the register
	 * The register also contains dg_idx programmed by HW that will be used
	 * to poll dg mapping in router.
	 */
	tegra_hwpm_readl(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_secure_config_o(),
			perfmon->base_pa), &config_dgmap);
	dg_idx = pmmsys_secure_config_dg_idx_v(config_dgmap);

	/* Configure DG map for this perfmon */
	config_dgmap = set_field(config_dgmap,
		pmmsys_secure_config_cmd_slice_id_m() |
		pmmsys_secure_config_channel_id_m() |
		pmmsys_secure_config_cblock_id_m() |
		pmmsys_secure_config_mapped_m() |
		pmmsys_secure_config_use_prog_dg_idx_m() |
		pmmsys_secure_config_command_pkt_decoder_m(),
		pmmsys_secure_config_cmd_slice_id_f(
			TEGRA_HWPM_CBLOCK_CHANNEL_TO_CMD_SLICE(
				cblock, channel)) |
		pmmsys_secure_config_channel_id_f(channel) |
		pmmsys_secure_config_cblock_id_f(cblock) |
		pmmsys_secure_config_mapped_true_f() |
		pmmsys_secure_config_use_prog_dg_idx_false_f() |
		pmmsys_secure_config_command_pkt_decoder_enable_f());
	tegra_hwpm_writel(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_secure_config_o(),
			perfmon->base_pa), config_dgmap);

	/* Make sure that the DG map status is propagated to the router */
	dgmap_status_reg_idx = dg_idx / TEGRA_HWPM_NUM_DG_STATUS_PER_REG;
	dgmap_status_reg_dgidx = dg_idx % TEGRA_HWPM_NUM_DG_STATUS_PER_REG;

	tegra_hwpm_timeout_print(hwpm, retries, sleep_msecs, rtr_perfmux,
		pmmsys_router_user_dgmap_status_secure_r(dgmap_status_reg_idx),
		&reg_val,
		(((reg_val >> dgmap_status_reg_dgidx) &
			pmmsys_router_user_dgmap_status_secure_dg_s()) !=
			pmmsys_router_user_dgmap_status_secure_dg_mapped_v()),
		"Perfmon(0x%llx - 0x%llx) dgmap %d status update timed out",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa, dg_idx);

	return 0;
}

int t264_hwpm_perfmon_disable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon)
{
	u32 reg_val;
	u32 dg_idx = 0U;
	u32 config_dgmap = 0U;
	u32 dgmap_status_reg_idx = 0U, dgmap_status_reg_dgidx = 0U;
	u32 retries = 10U;
	u32 sleep_msecs = 10U;
	int err = 0;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	if (perfmon->element_type == HWPM_ELEMENT_PERFMUX) {
		/*
		 * Since HWPM elements use perfmon functions,
		 * skip disabling HWPM PERFMUX elements
		 */
		return 0;
	}

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		NULL);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	/* Disable */
	tegra_hwpm_dbg(hwpm, hwpm_dbg_release_resource,
		"Disabling PERFMON(0x%llx - 0x%llx)",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa);

	/*
	 * HWPM readl function expects register address relative to
	 * perfmon group base address.
	 * Hence use sys0_control offset + perfmon base_pa as the register
	 */

	tegra_hwpm_readl(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_control_o(),
			perfmon->base_pa), &reg_val);
	reg_val = set_field(reg_val, pmmsys_control_mode_m(),
		pmmsys_control_mode_disable_f());
	tegra_hwpm_writel(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_control_o(),
			perfmon->base_pa), reg_val);

	/*
	 * HWPM readl function expects register address relative to
	 * perfmon group base address.
	 * Hence use secure_config offset + perfmon base_pa as the register
	 * The register also contains dg_idx programmed by HW that will be used
	 * to poll dg mapping in router.
	 */
	tegra_hwpm_readl(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_secure_config_o(),
			perfmon->base_pa), &config_dgmap);
	dg_idx = pmmsys_secure_config_dg_idx_v(config_dgmap);
	/* Reset DG map for this perfmon */
	config_dgmap = set_field(config_dgmap,
		pmmsys_secure_config_mapped_m(),
		pmmsys_secure_config_mapped_false_f());
	tegra_hwpm_writel(hwpm, perfmon,
		tegra_hwpm_safe_add_u64(pmmsys_secure_config_o(),
			perfmon->base_pa), config_dgmap);

	/* Make sure that the DG map status is propagated to the router */
	dgmap_status_reg_idx = dg_idx / TEGRA_HWPM_NUM_DG_STATUS_PER_REG;
	dgmap_status_reg_dgidx = dg_idx % TEGRA_HWPM_NUM_DG_STATUS_PER_REG;

	tegra_hwpm_timeout_print(hwpm, retries, sleep_msecs, rtr_perfmux,
		pmmsys_router_user_dgmap_status_secure_r(dgmap_status_reg_idx),
		&reg_val,
		(((reg_val >> dgmap_status_reg_dgidx) &
			pmmsys_router_user_dgmap_status_secure_dg_s()) !=
			pmmsys_router_user_dgmap_status_secure_dg_not_mapped_v()),
		"Perfmon(0x%llx - 0x%llx) dgmap %d status update timed out",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa, dg_idx);

	return 0;
}
