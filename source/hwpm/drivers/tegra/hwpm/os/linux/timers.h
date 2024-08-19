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

#ifndef TEGRA_HWPM_OS_LINUX_TIMERS_H
#define TEGRA_HWPM_OS_LINUX_TIMERS_H

struct tegra_hwpm_timeout {
	u32 max_attempts;
	u32 attempted;
};

struct tegra_soc_hwpm;

int tegra_hwpm_timeout_init_impl(struct tegra_soc_hwpm *hwpm,
	struct tegra_hwpm_timeout *timeout, u32 retries);
int tegra_hwpm_timeout_expired_impl(struct tegra_soc_hwpm *hwpm,
	struct tegra_hwpm_timeout *timeout);

void tegra_hwpm_msleep_impl(unsigned int msecs);

#endif /* TEGRA_HWPM_OS_LINUX_TIMERS_H */
