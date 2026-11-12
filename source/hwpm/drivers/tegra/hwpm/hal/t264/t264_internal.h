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

#ifndef T264_HWPM_INTERNAL_H
#define T264_HWPM_INTERNAL_H

#include <hal/t264/ip/vic/t264_vic.h>
#include <hal/t264/ip/pva/t264_pva.h>
#include <hal/t264/ip/mss_channel/t264_mss_channel.h>
#include <hal/t264/ip/mss_hubs/t264_mss_hubs.h>
#include <hal/t264/ip/ocu/t264_ocu.h>
#include <hal/t264/ip/smmu/t264_smmu.h>
#include <hal/t264/ip/ucf_msw/t264_ucf_msw.h>
#include <hal/t264/ip/ucf_psw/t264_ucf_psw.h>
#include <hal/t264/ip/ucf_csw/t264_ucf_csw.h>
#include <hal/t264/ip/cpu/t264_cpu.h>
#include <hal/t264/ip/vi/t264_vi.h>
#include <hal/t264/ip/isp/t264_isp.h>
#include <hal/t264/ip/pma/t264_pma.h>
#include <hal/t264/ip/rtr/t264_rtr.h>

#undef DEFINE_SOC_HWPM_ACTIVE_IP
#define DEFINE_SOC_HWPM_ACTIVE_IP(name)	name

#define T264_HWPM_ACTIVE_IP_MAX T264_HWPM_IP_MAX

#define T264_ACTIVE_IPS							\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_PMA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_RTR)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_VI)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_ISP)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_VIC)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_PVA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_MSS_CHANNEL)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_MSS_HUBS)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_OCU)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_SMMU)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_UCF_MSW)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_UCF_PSW)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_UCF_CSW)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_CPU)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T264_HWPM_ACTIVE_IP_MAX)

enum t264_hwpm_active_ips {
	T264_ACTIVE_IPS
};
#undef DEFINE_SOC_HWPM_ACTIVE_IP

enum tegra_soc_hwpm_ip;
enum tegra_soc_hwpm_resource;
struct tegra_soc_hwpm;
struct hwpm_ip_aperture;

bool t264_hwpm_validate_secondary_hals(struct tegra_soc_hwpm *hwpm);
bool t264_hwpm_is_ip_active(struct tegra_soc_hwpm *hwpm,
	u32 ip_enum, u32 *config_ip_index);
bool t264_hwpm_is_resource_active(struct tegra_soc_hwpm *hwpm,
	u32 res_enum, u32 *config_ip_index);

u32 t264_get_rtr_int_idx(void);
u32 t264_get_ip_max_idx(void);
int t264_hwpm_get_rtr_pma_perfmux_ptr(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture **rtr_perfmux_ptr,
	struct hwpm_ip_aperture **pma_perfmux_ptr);
int t264_hwpm_check_status(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm);

int t264_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available);
int t264_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm);

int t264_hwpm_disable_triggers(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_init_prod_values(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_credit_program(struct tegra_soc_hwpm *hwpm,
	u32 *num_credits, u8 cblock_idx, u8 pma_channel_idx,
	uint16_t credit_cmd);
int t264_hwpm_setup_trigger(struct tegra_soc_hwpm *hwpm,
	u8 enable_cross_trigger, u8 session_type);
int t264_hwpm_perfmon_enable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);
int t264_hwpm_perfmon_disable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);
int t264_hwpm_disable_cg(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_enable_cg(struct tegra_soc_hwpm *hwpm);

int t264_hwpm_disable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_enable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_invalidate_mem_config(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_stream_mem_bytes(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_disable_pma_streaming(struct tegra_soc_hwpm *hwpm);
int t264_hwpm_update_mem_bytes_get_ptr(struct tegra_soc_hwpm *hwpm,
	u64 mem_bump);
int t264_hwpm_get_mem_bytes_put_ptr(struct tegra_soc_hwpm *hwpm,
	u64 *mem_head_ptr);
int t264_hwpm_membuf_overflow_status(struct tegra_soc_hwpm *hwpm,
	u32 *overflow_status);

#endif /* T264_HWPM_INTERNAL_H */
