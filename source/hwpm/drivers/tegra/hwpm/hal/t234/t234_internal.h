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

#ifndef T234_HWPM_INTERNAL_H
#define T234_HWPM_INTERNAL_H

#include <hal/t234/ip/vi/t234_vi.h>
#include <hal/t234/ip/isp/t234_isp.h>
#include <hal/t234/ip/vic/t234_vic.h>
#include <hal/t234/ip/ofa/t234_ofa.h>
#include <hal/t234/ip/pva/t234_pva.h>
#include <hal/t234/ip/nvdla/t234_nvdla.h>
#include <hal/t234/ip/mgbe/t234_mgbe.h>
#include <hal/t234/ip/scf/t234_scf.h>
#include <hal/t234/ip/nvdec/t234_nvdec.h>
#include <hal/t234/ip/nvenc/t234_nvenc.h>
#include <hal/t234/ip/pcie/t234_pcie.h>
#include <hal/t234/ip/display/t234_display.h>
#include <hal/t234/ip/mss_channel/t234_mss_channel.h>
#include <hal/t234/ip/mss_gpu_hub/t234_mss_gpu_hub.h>
#include <hal/t234/ip/mss_iso_niso_hubs/t234_mss_iso_niso_hubs.h>
#include <hal/t234/ip/mss_mcf/t234_mss_mcf.h>
#include <hal/t234/ip/pma/t234_pma.h>
#include <hal/t234/ip/rtr/t234_rtr.h>

#define T234_HWPM_ACTIVE_IP_MAX		T234_HWPM_IP_MAX

#define T234_ACTIVE_IPS							\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_PMA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_RTR)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_VI)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_ISP)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_VIC)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_OFA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_PVA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_NVDLA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MGBE)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_SCF)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_NVDEC)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_NVENC)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_PCIE)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_DISPLAY)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MSS_CHANNEL)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MSS_GPU_HUB)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MSS_ISO_NISO_HUBS) \
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MSS_MCF)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(T234_HWPM_ACTIVE_IP_MAX)

#undef DEFINE_SOC_HWPM_ACTIVE_IP
#define DEFINE_SOC_HWPM_ACTIVE_IP(name)	name
enum t234_hwpm_active_ips {
	T234_ACTIVE_IPS
};
#undef DEFINE_SOC_HWPM_ACTIVE_IP

enum tegra_soc_hwpm_ip;
enum tegra_soc_hwpm_resource;
struct tegra_soc_hwpm;
struct hwpm_ip_aperture;

bool t234_hwpm_validate_secondary_hals(struct tegra_soc_hwpm *hwpm);
bool t234_hwpm_is_ip_active(struct tegra_soc_hwpm *hwpm,
	u32 ip_enum, u32 *config_ip_index);
bool t234_hwpm_is_resource_active(struct tegra_soc_hwpm *hwpm,
	u32 res_enum, u32 *config_ip_index);

u32 t234_get_rtr_int_idx(struct tegra_soc_hwpm *hwpm);
u32 t234_get_ip_max_idx(struct tegra_soc_hwpm *hwpm);

int t234_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available);
int t234_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm);

int t234_hwpm_init_prod_values(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_disable_cg(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_enable_cg(struct tegra_soc_hwpm *hwpm);

int t234_hwpm_check_status(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_disable_triggers(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_perfmon_enable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);
int t234_hwpm_perfmon_disable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);

int t234_hwpm_disable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_enable_mem_mgmt(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_invalidate_mem_config(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_stream_mem_bytes(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_disable_pma_streaming(struct tegra_soc_hwpm *hwpm);
int t234_hwpm_update_mem_bytes_get_ptr(struct tegra_soc_hwpm *hwpm,
	u64 mem_bump);
u64 t234_hwpm_get_mem_bytes_put_ptr(struct tegra_soc_hwpm *hwpm);
bool t234_hwpm_membuf_overflow_status(struct tegra_soc_hwpm *hwpm);

#endif /* T234_HWPM_INTERNAL_H */
