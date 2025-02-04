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

#ifndef TEGRA_HWPM_OS_LINUX_IP_UTILS_H
#define TEGRA_HWPM_OS_LINUX_IP_UTILS_H

enum tegra_soc_hwpm_resource;
struct tegra_soc_hwpm;
struct tegra_soc_hwpm_ip_floorsweep_info;
struct tegra_soc_hwpm_resource_info;

int tegra_hwpm_complete_ip_register_impl(struct tegra_soc_hwpm *hwpm);
u32 tegra_hwpm_translate_soc_hwpm_resource(struct tegra_soc_hwpm *hwpm,
	enum tegra_soc_hwpm_resource res_enum);
int tegra_hwpm_obtain_floorsweep_info(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_ip_floorsweep_info *fs_info);
int tegra_hwpm_obtain_resource_info(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_resource_info *rsrc_info);
void tegra_hwpm_release_ip_register_node(struct tegra_soc_hwpm *hwpm);

#endif /* TEGRA_HWPM_OS_LINUX_IP_UTILS_H */
