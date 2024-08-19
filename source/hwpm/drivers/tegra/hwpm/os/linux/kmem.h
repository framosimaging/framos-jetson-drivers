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

#ifndef TEGRA_HWPM_OS_LINUX_KMEM_H
#define TEGRA_HWPM_OS_LINUX_KMEM_H

#include <linux/types.h>

struct tegra_soc_hwpm;

void *tegra_hwpm_kzalloc_impl(struct tegra_soc_hwpm *hwpm, size_t size);
void *tegra_hwpm_kcalloc_impl(struct tegra_soc_hwpm *hwpm, u32 num, size_t size);

void tegra_hwpm_kfree_impl(struct tegra_soc_hwpm *hwpm, void *addr);

#endif /* TEGRA_HWPM_OS_LINUX_KMEM_H */
