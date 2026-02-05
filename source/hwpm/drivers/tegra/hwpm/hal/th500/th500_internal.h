/* SPDX-License-Identifier: MIT */
/* SPDX-FileCopyrightText: Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
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

#ifndef TH500_HWPM_INTERNAL_H
#define TH500_HWPM_INTERNAL_H

#include <hal/th500/soc/ip/mss_channel/th500_mss_channel.h>
#include <hal/th500/soc/ip/rtr/th500_rtr.h>
#include <hal/th500/soc/ip/pma/th500_pma.h>
#include <hal/th500/soc/ip/c2c/th500_c2c.h>
#include <hal/th500/soc/ip/smmu/th500_smmu.h>
#include <hal/th500/soc/ip/cl2/th500_cl2.h>
#include <hal/th500/soc/ip/c_nvlink/th500_nvlrx.h>
#include <hal/th500/soc/ip/c_nvlink/th500_nvltx.h>
#include <hal/th500/soc/ip/c_nvlink/th500_nvlctrl.h>
#include <hal/th500/soc/ip/mss_hub/th500_mss_hub.h>
#include <hal/th500/soc/ip/mcf_soc/th500_mcf_soc.h>
#include <hal/th500/soc/ip/mcf_c2c/th500_mcf_c2c.h>
#include <hal/th500/soc/ip/mcf_clink/th500_mcf_clink.h>
#include <hal/th500/soc/ip/mcf_core/th500_mcf_core.h>
#include <hal/th500/soc/ip/pcie/th500_pcie_xtlq.h>
#include <hal/th500/soc/ip/pcie/th500_pcie_xtlrc.h>
#include <hal/th500/soc/ip/pcie/th500_pcie_xalrc.h>

#define TH500_HWPM_ACTIVE_IP_MAX		TH500_HWPM_IP_MAX

#define TH500_ACTIVE_IPS						\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_RTR)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_PMA)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MSS_CHANNEL)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MCF_C2C)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_C2C)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_SMMU)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_CL2)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_NVLCTRL)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_NVLRX)   	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_NVLTX)   	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MSS_HUB)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MCF_SOC)		\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MCF_CLINK)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MCF_CORE)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_PCIE_XTLQ)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_PCIE_XTLRC)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_PCIE_XALRC)	\
	DEFINE_SOC_HWPM_ACTIVE_IP(TH500_HWPM_ACTIVE_IP_MAX)

#undef DEFINE_SOC_HWPM_ACTIVE_IP
#define DEFINE_SOC_HWPM_ACTIVE_IP(name)	name
enum th500_hwpm_active_ips {
	TH500_ACTIVE_IPS
};
#undef DEFINE_SOC_HWPM_ACTIVE_IP

struct tegra_soc_hwpm;

bool th500_hwpm_validate_secondary_hals(struct tegra_soc_hwpm *hwpm);
bool th500_hwpm_is_ip_active(struct tegra_soc_hwpm *hwpm,
	u32 ip_index, u32 *config_ip_index);
bool th500_hwpm_is_resource_active(struct tegra_soc_hwpm *hwpm,
	u32 res_index, u32 *config_ip_index);

u32 th500_get_rtr_int_idx(void);
u32 th500_get_ip_max_idx(void);

#endif /* TH500_HWPM_INTERNAL_H */
