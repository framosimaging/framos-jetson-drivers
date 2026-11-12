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

#ifndef TEGRA_HWPM_OS_LINUX_SOC_UTILS_H
#define TEGRA_HWPM_OS_LINUX_SOC_UTILS_H

struct tegra_hwpm_os_linux;
struct tegra_soc_hwpm;

int tegra_hwpm_init_chip_info(struct tegra_hwpm_os_linux *hwpm_linux);
u32 tegra_hwpm_get_chip_id_impl(void);
u32 tegra_hwpm_get_major_rev_impl(void);
u32 tegra_hwpm_chip_get_revision_impl(void);
u32 tegra_hwpm_get_platform_impl(void);
bool tegra_hwpm_is_platform_simulation_impl(void);
bool tegra_hwpm_is_platform_vsp_impl(void);
bool tegra_hwpm_is_platform_silicon_impl(void);
bool tegra_hwpm_is_hypervisor_mode_impl(void);
int tegra_hwpm_fuse_readl_impl(struct tegra_soc_hwpm *hwpm,
	u64 reg_offset, u32 *val);
int tegra_hwpm_fuse_readl_prod_mode_impl(struct tegra_soc_hwpm *hwpm, u32 *val);

#endif /* TEGRA_HWPM_OS_LINUX_SOC_UTILS_H */
