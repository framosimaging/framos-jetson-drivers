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

#ifndef TEGRA_HWPM_OS_LINUX_IO_UTILS_H
#define TEGRA_HWPM_OS_LINUX_IO_UTILS_H

struct tegra_soc_hwpm;
struct hwpm_ip_inst;
struct hwpm_ip_aperture;

int tegra_hwpm_read_sticky_bits_impl(struct tegra_soc_hwpm *hwpm,
	u64 reg_base, u64 reg_offset, u32 *val);
int tegra_hwpm_fake_readl_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *aperture, u64 offset, u32 *val);
int tegra_hwpm_fake_writel_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *aperture, u64 offset, u32 val);
int tegra_hwpm_readl_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *aperture, u64 addr, u32 *val);
int tegra_hwpm_writel_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *aperture, u64 addr, u32 val);
int tegra_hwpm_regops_readl_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_inst *ip_inst, struct hwpm_ip_aperture *aperture,
	u64 addr, u32 *val);
int tegra_hwpm_regops_writel_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_inst *ip_inst, struct hwpm_ip_aperture *aperture,
	u64 addr, u32 val);

#endif /* TEGRA_HWPM_OS_LINUX_IO_UTILS_H */
