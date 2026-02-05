/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IP_NAMES_H
#define IP_NAMES_H

#include "nv_soc_hwpm.h"

static const char *kIpNames[NV_SOC_HWPM_NUM_IPS] = {
	"VI",
	"ISP",
	"VIC",
	"OFA",
	"PVA",
	"NVDLA",
	"MGBE",
	"SCF",
	"NVDEC",
	"NVENC",
	"PCIE",
	"DISPLAY",
	"MSS_CHANNEL",
	"MSS_GPU_HUB",
	"MSS_ISO_NISO_HUBS",
	"MSS_MCF",
	"APE",
	"C2C",
	"SMMU",
	"CL2",
	"NVLCTRL",
	"NVLRX",
	"NVLTX",
	"MSS_HUB",
	"MCF_SOC",
	"MCF_C2C",
	"MCF_CLINK",
	"MCF_CORE",
	"MCF_OCU",
	"PCIE_XTLQ",
	"PCIE_XTLRC",
	"PCIE_XALRC",
	"UCF_MSW",
	"UCF_PSW",
	"UCF_CSW",
	"UCF_HUB",
	"UCF_SCB",
	"CPU",
	"CPU_EXT_0",
	"CPU_EXT_1",
	"CPU_EXT_2",
	"NVTHERM",
	"CSN",
	"CSN_EXT_0",
	"CSNH",
};

static const char *kResourceNames[NV_SOC_HWPM_NUM_RESOURCES] = {
	"VI",
	"ISP",
	"VIC",
	"OFA",
	"PVA",
	"NVDLA",
	"MGBE",
	"SCF",
	"NVDEC",
	"NVENC",
	"PCIE",
	"DISPLAY",
	"MSS_CHANNEL",
	"MSS_GPU_HUB",
	"MSS_ISO_NISO_HUBS",
	"MSS_MCF",
	"PMA",
	"CMD_SLICE_RTR",
	"APE",
	"C2C",
	"SMMU",
	"CL2",
	"NVLCTRL",
	"NVLRX",
	"NVLTX",
	"MSS_HUB",
	"MCF_SOC",
	"MCF_C2C",
	"MCF_CLINK",
	"MCF_CORE",
	"MCF_OCU",
	"PCIE_XTLQ",
	"PCIE_XTLRC",
	"PCIE_XALRC",
	"UCF_MSW",
	"UCF_PSW",
	"UCF_CSW",
	"UCF_HUB",
	"UCF_SCB",
	"CPU",
	"CPU_EXT_0",
	"CPU_EXT_1",
	"CPU_EXT_2",
	"NVTHERM",
	"CSN",
	"CSN_EXT_0",
	"CSNH",
};

#endif // IP_NAMES_H
