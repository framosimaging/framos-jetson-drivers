/* SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef __NV_SOC_HWPM_OS__
#define __NV_SOC_HWPM_OS__

#include "nv_soc_hwpm.h"
#include "common/types.h"

#ifdef __cplusplus
extern "C" {
#endif

int nv_soc_hwpm_os_open_hwpm_device(const char* dev_path);

int nv_soc_hwpm_os_close_hwpm_device(int fd);

int nv_soc_hwpm_os_enumerate_device(
	uint32_t max_count, uint32_t* actual_count,
	nv_soc_hwpm_device_int* devices_int);

int nv_soc_hwpm_os_session_reserve_resources(
        nv_soc_hwpm_session_int* session_int, nv_soc_hwpm_resource res_id);

int nv_soc_hwpm_os_session_alloc_pma(
        nv_soc_hwpm_session_int* session_int,
        const nv_soc_hwpm_pma_buffer_params *record_buffer_params);

void nv_soc_hwpm_os_session_free_pma(
        nv_soc_hwpm_session_int* session_int);

int nv_soc_hwpm_os_session_set_get_pma_state(
	nv_soc_hwpm_session_int* session_int,
	nv_soc_hwpm_pma_channel_state_params* param);

int nv_soc_hwpm_os_session_get_hs_credits(
	nv_soc_hwpm_session_int* session_int,
	tegra_soc_hwpm_get_type type,
	uint32_t* num_hs_credits);

int nv_soc_hwpm_os_session_config_hs_credits(
	nv_soc_hwpm_session_int* session_int,
	uint32_t param_count,
	const nv_soc_hwpm_config_hs_credit_params* params);

int nv_soc_hwpm_os_session_start(nv_soc_hwpm_session_int* session_int);

int nv_soc_hwpm_os_session_regops(
	nv_soc_hwpm_session_int* session_int,
	const size_t param_count,
	nv_soc_hwpm_reg_ops_params* params,
	nv_soc_hwpm_reg_ops_validation_mode mode,
	int* all_reg_ops_passed);

int nv_soc_hwpm_os_session_setup_trigger(
	nv_soc_hwpm_session_int* session_int,
	int enable_cross_trigger,
	nv_soc_hwpm_trigger_session_type session_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__NV_SOC_HWPM_OS__*/
