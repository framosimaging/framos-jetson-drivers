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

#ifndef TEGRA_HWPM_OS_LINUX_CLK_RST_UTILS_H
#define TEGRA_HWPM_OS_LINUX_CLK_RST_UTILS_H

struct tegra_hwpm_os_linux;

int tegra_hwpm_clk_rst_prepare(struct tegra_hwpm_os_linux *hwpm_linux);
int tegra_hwpm_clk_rst_set_rate_enable(struct tegra_hwpm_os_linux *hwpm_linux);
int tegra_hwpm_clk_rst_disable(struct tegra_hwpm_os_linux *hwpm_linux);
void tegra_hwpm_clk_rst_release(struct tegra_hwpm_os_linux *hwpm_linux);

#endif /* TEGRA_HWPM_OS_LINUX_CLK_RST_UTILS_H */
