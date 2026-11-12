// SPDX-License-Identifier: GPL-2.0-only
/*
 * SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef TEGRA_HWPM_OS_LINUX_REGOPS_UTILS_H
#define TEGRA_HWPM_OS_LINUX_REGOPS_UTILS_H

struct tegra_soc_hwpm;
struct tegra_soc_hwpm_exec_reg_ops;
struct tegra_soc_hwpm_exec_credit_program;
struct tegra_soc_hwpm_setup_trigger;

int tegra_hwpm_exec_regops(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_exec_reg_ops *exec_reg_ops);
int tegra_hwpm_credit_program(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_exec_credit_program *credit_prog);
int tegra_hwpm_setup_trigger(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_setup_trigger *setup_trigger);

#endif /* TEGRA_HWPM_OS_LINUX_REGOPS_UTILS_H */
