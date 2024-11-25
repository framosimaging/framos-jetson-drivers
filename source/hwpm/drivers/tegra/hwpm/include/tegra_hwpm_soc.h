/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef TEGRA_HWPM_SOC_H
#define TEGRA_HWPM_SOC_H

#if defined(CONFIG_TEGRA_HWPM_OOT)
#define CHIP_ID_UNKNOWN			0x0U
#define CHIP_ID_REV_UNKNOWN		0x0U

#define PLAT_SI				0x0
#define PLAT_PRE_SI_QT			0x1
#define PLAT_PRE_SI_VDK			0x8
#define PLAT_PRE_SI_VSP			0x9
#define PLAT_INVALID			0xF

#define TEGRA_FUSE_PRODUCTION_MODE 0x0

struct hwpm_soc_chip_info {
	u32 chip_id;
	u32 chip_id_rev;
	u32 platform;
};
#endif

#ifdef __KERNEL__
#include <os/linux/soc_utils.h>
#else
#include <os/qnx/soc_utils.h>
#define CHIP_ID_UNKNOWN                 0x0U
#define CHIP_ID_REV_UNKNOWN             0x0U

#define PLAT_SI                         0x0
#define PLAT_PRE_SI_QT                  0x1
#define PLAT_PRE_SI_VDK                 0x8
#define PLAT_PRE_SI_VSP                 0x9
#define PLAT_INVALID                    0xF

#define TEGRA_FUSE_PRODUCTION_MODE 0x0

struct hwpm_soc_chip_info {
        u32 chip_id;
        u32 chip_id_rev;
        u32 platform;
};

#endif

#define tegra_hwpm_get_chip_id()		\
	tegra_hwpm_get_chip_id_impl()

#define tegra_hwpm_get_major_rev()		\
	tegra_hwpm_get_major_rev_impl()

#define tegra_hwpm_chip_get_revision()		\
	tegra_hwpm_chip_get_revision_impl()

#define tegra_hwpm_get_platform()		\
	tegra_hwpm_get_platform_impl()

#define tegra_hwpm_is_platform_simulation()	\
	tegra_hwpm_is_platform_simulation_impl()

#define tegra_hwpm_is_platform_vsp()		\
	tegra_hwpm_is_platform_vsp_impl()

#define tegra_hwpm_is_platform_silicon()	\
	tegra_hwpm_is_platform_silicon_impl()

#define tegra_hwpm_is_hypervisor_mode()		\
	tegra_hwpm_is_hypervisor_mode_impl()

#define tegra_hwpm_fuse_readl(hwpm, reg_offset, val)	\
	tegra_hwpm_fuse_readl_impl(hwpm, reg_offset, val)

#define tegra_hwpm_fuse_readl_prod_mode(hwpm, val)	\
	tegra_hwpm_fuse_readl_prod_mode_impl(hwpm, val)

#endif /* TEGRA_HWPM_SOC_H */
