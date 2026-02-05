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
 */

#include <tegra_hwpm_timers.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>

#include <hal/t264/t264_internal.h>
#include <hal/t264/ip/rtr/t264_rtr.h>
#include <hal/t264/hw/t264_pmasys_soc_hwpm.h>
#include <hal/t264/hw/t264_pmmsys_soc_hwpm.h>

#define T264_HWPM_ENGINE_INDEX_GPMA0		3U
#define T264_HWPM_ENGINE_INDEX_GPMA1		4U
#define T264_HWPM_ENGINE_INDEX_PMA		8U

int t264_hwpm_get_rtr_pma_perfmux_ptr(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture **rtr_perfmux_ptr,
	struct hwpm_ip_aperture **pma_perfmux_ptr)
{
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
	struct hwpm_ip *chip_ip = active_chip->chip_ips[
		active_chip->get_rtr_int_idx()];
	struct hwpm_ip_inst *ip_inst_rtr = &chip_ip->ip_inst_static_array[
		T264_HWPM_IP_RTR_STATIC_RTR_INST];
	struct hwpm_ip_inst *ip_inst_pma = &chip_ip->ip_inst_static_array[
		T264_HWPM_IP_RTR_STATIC_PMA_INST];

	if (rtr_perfmux_ptr != NULL) {
		*rtr_perfmux_ptr = &ip_inst_rtr->element_info[
			TEGRA_HWPM_APERTURE_TYPE_PERFMUX].element_static_array[
				T264_HWPM_IP_RTR_STATIC_RTR_PERFMUX_INDEX];
	}

	if (pma_perfmux_ptr != NULL) {
		*pma_perfmux_ptr = &ip_inst_pma->element_info[
			TEGRA_HWPM_APERTURE_TYPE_PERFMUX].element_static_array[
				T264_HWPM_IP_RTR_STATIC_PMA_PERFMUX_INDEX];
	}

	return 0;
}

int t264_hwpm_check_status(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;
	u32 reg_val = 0U;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	/* Check ROUTER state */
	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_enginestatus_r(), &reg_val);
	hwpm_assert_print(hwpm,
		pmmsys_router_enginestatus_status_v(reg_val) ==
			pmmsys_router_enginestatus_status_empty_v(),
		return -EINVAL, "Router not ready value 0x%x", reg_val);

	/* Check PMA state */
	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_channel_status_r(0, 0), &reg_val);

	hwpm_assert_print(hwpm,
		(reg_val & pmasys_channel_status_engine_status_m()) ==
			pmasys_channel_status_engine_status_empty_f(),
		return -EINVAL, "PMA not ready value 0x%x", reg_val);

	return 0;
}

int t264_hwpm_disable_triggers(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;
	u32 reg_val = 0U;
	u32 retries = 10U;
	u32 sleep_msecs = 100U;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux, &pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	/* Disable PMA triggers */
	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_config_user_r(0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_command_slice_trigger_config_user_pma_pulse_m(),
		pmasys_command_slice_trigger_config_user_pma_pulse_disable_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_config_user_r(0), reg_val);

	/* Reset TRIGGER_START_MASK registers */
	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_start_mask0_r(0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_command_slice_trigger_start_mask0_engine_m(),
		pmasys_command_slice_trigger_start_mask0_engine_init_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_start_mask0_r(0), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_start_mask1_r(0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_command_slice_trigger_start_mask1_engine_m(),
		pmasys_command_slice_trigger_start_mask1_engine_init_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_start_mask1_r(0), reg_val);

	/* Reset TRIGGER_STOP_MASK registers */
	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_stop_mask0_r(0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_command_slice_trigger_stop_mask0_engine_m(),
		pmasys_command_slice_trigger_stop_mask0_engine_init_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_stop_mask0_r(0), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_stop_mask1_r(0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_command_slice_trigger_stop_mask1_engine_m(),
		pmasys_command_slice_trigger_stop_mask1_engine_init_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_command_slice_trigger_stop_mask1_r(0), reg_val);

	/* Wait for PERFMONs to idle */
	tegra_hwpm_timeout_print(hwpm, retries, sleep_msecs, rtr_perfmux,
		pmmsys_router_enginestatus_r(), &reg_val,
		(pmmsys_router_enginestatus_merged_perfmon_status_v(
				reg_val) != 0U),
		"PMMSYS_ROUTER_ENGINESTATUS_PERFMON_STATUS timed out");

	/* Wait for ROUTER to idle */
	tegra_hwpm_timeout_print(hwpm, retries, sleep_msecs, rtr_perfmux,
		pmmsys_router_enginestatus_r(), &reg_val,
		(pmmsys_router_enginestatus_status_v(reg_val) !=
			pmmsys_router_enginestatus_status_empty_v()),
		"PMMSYS_ROUTER_ENGINESTATUS_STATUS timed out");

	/* Wait for PMA to idle */
	tegra_hwpm_timeout_print(hwpm, retries, sleep_msecs, pma_perfmux,
		pmasys_channel_status_r(0, 0), &reg_val,
		((reg_val & pmasys_channel_status_engine_status_m()) !=
			pmasys_channel_status_engine_status_empty_f()),
		"PMASYS_CHANNEL_STATUS timed out");

	return err;
}

int t264_hwpm_init_prod_values(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;
	u32 reg_val = 0U;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_channel_config_user_r(0,0), &reg_val);
	reg_val = set_field(reg_val,
		pmasys_channel_config_user_coalesce_timeout_cycles_m(),
		pmasys_channel_config_user_coalesce_timeout_cycles__prod_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_channel_config_user_r(0,0), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg2_secure_slcg_m(),
			pmasys_profiling_cg2_secure_slcg__prod_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg1_secure_flcg_m(),
			pmasys_profiling_cg1_secure_flcg__prod_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_dg_cg1_secure_flcg_m(),
		pmmsys_router_profiling_dg_cg1_secure_flcg__prod_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg1_secure_flcg_m(),
		pmmsys_router_profiling_cg1_secure_flcg__prod_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_perfmon_cg2_secure_slcg_m(),
		pmmsys_router_perfmon_cg2_secure_slcg__prod_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg2_secure_slcg_m(),
		pmmsys_router_profiling_cg2_secure_slcg__prod_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), reg_val);

	return 0;
}

int t264_hwpm_disable_cg(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;
	u32 reg_val = 0U;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg2_secure_slcg_m(),
			pmasys_profiling_cg2_secure_slcg_disabled_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg1_secure_flcg_m(),
			pmasys_profiling_cg1_secure_flcg_disabled_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_dg_cg1_secure_flcg_m(),
		pmmsys_router_profiling_dg_cg1_secure_flcg_disabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg1_secure_flcg_m(),
		pmmsys_router_profiling_cg1_secure_flcg_disabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_perfmon_cg2_secure_slcg_m(),
		pmmsys_router_perfmon_cg2_secure_slcg_disabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg2_secure_slcg_m(),
		pmmsys_router_profiling_cg2_secure_slcg_disabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), reg_val);

	return 0;
}

int t264_hwpm_enable_cg(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;
	u32 reg_val = 0U;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
		&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get rtr pma perfmux failed");

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg2_secure_slcg_m(),
			pmasys_profiling_cg2_secure_slcg_enabled_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val, pmasys_profiling_cg1_secure_flcg_m(),
		pmasys_profiling_cg1_secure_flcg_enabled_f());
	tegra_hwpm_writel(hwpm, pma_perfmux,
		pmasys_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_dg_cg1_secure_flcg_m(),
		pmmsys_router_profiling_dg_cg1_secure_flcg_enabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_dg_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg1_secure_flcg_m(),
		pmmsys_router_profiling_cg1_secure_flcg_enabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg1_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_perfmon_cg2_secure_slcg_m(),
		pmmsys_router_perfmon_cg2_secure_slcg_enabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_perfmon_cg2_secure_r(), reg_val);

	tegra_hwpm_readl(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), &reg_val);
	reg_val = set_field(reg_val,
		pmmsys_router_profiling_cg2_secure_slcg_m(),
		pmmsys_router_profiling_cg2_secure_slcg_enabled_f());
	tegra_hwpm_writel(hwpm, rtr_perfmux,
		pmmsys_router_profiling_cg2_secure_r(), reg_val);

	return 0;
}

int t264_hwpm_credit_program(struct tegra_soc_hwpm *hwpm,
	u32 *num_credits, u8 cblock_idx, u8 pma_channel_idx,
	uint16_t credit_cmd)
{
	int err = 0;
	u32 reg_val = 0U;
	struct hwpm_ip_aperture *pma_perfmux = NULL;
	struct hwpm_ip_aperture *rtr_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, &rtr_perfmux,
			&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
			"get rtr, pma perfmux failed");

	switch (credit_cmd) {
	case TEGRA_HWPM_CMD_SET_HS_CREDITS:
		/* Write credits information */
		tegra_hwpm_readl(hwpm, rtr_perfmux,
				pmmsys_user_channel_config_secure_r(
					cblock_idx, pma_channel_idx),
				&reg_val);
		reg_val = set_field(reg_val,
				pmmsys_user_channel_config_secure_hs_credits_m(),
				*num_credits);
		tegra_hwpm_writel(hwpm, rtr_perfmux,
				pmmsys_user_channel_config_secure_r(
					cblock_idx, pma_channel_idx),
				reg_val);
	break;
	case TEGRA_HWPM_CMD_GET_HS_CREDITS:
		/* Read credits information */
		tegra_hwpm_readl(hwpm, rtr_perfmux,
				pmmsys_user_channel_config_secure_r(
					cblock_idx, pma_channel_idx),
				num_credits);
	break;
	case TEGRA_HWPM_CMD_GET_TOTAL_HS_CREDITS:
		/* read the total HS Credits */
		tegra_hwpm_readl(hwpm, pma_perfmux,
				pmasys_streaming_capabilities1_r(), &reg_val);
		*num_credits = pmasys_streaming_capabilities1_total_credits_v(
				reg_val);
	break;
	case TEGRA_HWPM_CMD_GET_CHIPLET_HS_CREDITS_POOL:
		/* Defined for future chips */
		tegra_hwpm_err(hwpm,
				"TEGRA_SOC_HWPM_CMD_GET_CHIPLET_HS_CREDIT_POOL"
				" not supported");
		err = -EINVAL;
	break;
	case TEGRA_HWPM_CMD_GET_HS_CREDITS_MAPPING:
		/* Defined for future chips */
		tegra_hwpm_err(hwpm,
				"TEGRA_SOC_HWPM_CMD_GET_HS_CREDIT_MAPPING"
				" not supported");
		err = -EINVAL;
		break;
	default:
		tegra_hwpm_err(hwpm, "Invalid Credit Programming State (%d)",
				credit_cmd);
		err = -EINVAL;
		break;
	}

	return err;
}

int t264_hwpm_setup_trigger(struct tegra_soc_hwpm *hwpm,
	u8 enable_cross_trigger, u8 session_type)
{
	int err = 0;
	u32 trigger_mask_secure0 = 0U;
	u32 record_select_secure = 0U;
	struct hwpm_ip_aperture *pma_perfmux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	err = hwpm->active_chip->get_rtr_pma_perfmux_ptr(hwpm, NULL,
			&pma_perfmux);
	hwpm_assert_print(hwpm, err == 0, return err,
		"get pma perfmux failed");

	/*
	 * Case 1: profiler, cross-trigger enabled, GPU->SoC
	 * - Action: enable incoming start-stop trigger from GPU PMA
	 * - GPU PMA Action: enable outgoing trigger from GPU PMA,
	 * trigger type doesn't matter on GPU side
	 *
	 * Case 2: sampler, cross-trigger enabled, GPU->SoC
	 * - Action: enable incoming periodic trigger from GPU PMA
	 * - GPU PMA Action: enable outgoing trigger from GPU PMA,
	 * trigger type doesn't matter on GPU side
	 *
	 * Case 3: profiler, cross-trigger enabled, SoC->GPU
	 * - Action: enable outgoing trigger from SoC PMA,
	 * trigger type doesn't matter on SoC side
	 * - GPU PMA Action: configure incoming start-stop trigger from SoC PMA
	 *
	 * Case 4: sampler, cross-trigger enabled, SoC->GPU
	 * - Action: enable outgoing trigger from SoC PMA,
	 * trigger type doesn't matter on SoC side
	 * - GPU PMA Action: configure incoming periodic trigger from SoC PMA
	 *
	 * Case 5: profiler, cross-trigger disabled
	 * - Action: enable own trigger from SoC PMA,
	 * trigger type doesn't matter
	 * - GPU PMA Action: enable own trigger from GPU PMA,
	 * trigger type doesn't matter)
	 *
	 * Case 6: sampler, cross-trigger disabled
	 * - Action: enable own trigger from SoC PMA,
	 * trigger type doesn't matter
	 * - GPU PMA Action: enable own trigger from GPU PMA,
	 * trigger type doesn't matter
	 */
	if (!enable_cross_trigger) {
		/*
		 * Handle Case-3 to Case-6
		 */
		trigger_mask_secure0 = BIT(T264_HWPM_ENGINE_INDEX_PMA);
		record_select_secure = T264_HWPM_ENGINE_INDEX_PMA;
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_trigger_mask_secure0_r(0),
			trigger_mask_secure0);
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_record_select_secure_r(0),
			record_select_secure);

		return err;
	}

	switch (session_type) {
	case TEGRA_HWPM_CMD_PERIODIC_SESSION:
		/*
		 * Handle Case-1
		 */
		trigger_mask_secure0 = BIT(T264_HWPM_ENGINE_INDEX_GPMA1);
		record_select_secure = T264_HWPM_ENGINE_INDEX_GPMA1;
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_trigger_mask_secure0_r(0),
			trigger_mask_secure0);
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_record_select_secure_r(0),
			record_select_secure);
		break;
	case TEGRA_HWPM_CMD_START_STOP_SESSION:
		/*
		 * Handle Case-2
		 */
		trigger_mask_secure0 = BIT(T264_HWPM_ENGINE_INDEX_GPMA0);
		record_select_secure = T264_HWPM_ENGINE_INDEX_GPMA0;
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_trigger_mask_secure0_r(0),
			trigger_mask_secure0);
		tegra_hwpm_writel(hwpm, pma_perfmux,
			pmasys_command_slice_record_select_secure_r(0),
			record_select_secure);
		break;
	case TEGRA_HWPM_CMD_INVALID_SESSION:
	default:
		tegra_hwpm_err(hwpm, "Invalid Session type");
		err = -EINVAL;
		break;
	}

	return err;
}
