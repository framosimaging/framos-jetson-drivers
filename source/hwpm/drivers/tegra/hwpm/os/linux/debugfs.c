// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <linux/debugfs.h>

#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>
#include <os/linux/debugfs.h>
#include <os/linux/driver.h>

void tegra_hwpm_debugfs_init(struct tegra_hwpm_os_linux *hwpm_linux)
{
	struct tegra_soc_hwpm *hwpm = &hwpm_linux->hwpm;

	if (!hwpm_linux) {
		tegra_hwpm_err(hwpm, "Invalid hwpm_linux struct");
		return;
	}

	hwpm_linux->debugfs_root =
		debugfs_create_dir(TEGRA_SOC_HWPM_MODULE_NAME, NULL);
	if (!hwpm_linux->debugfs_root) {
		tegra_hwpm_err(hwpm, "Failed to create debugfs root directory");
		goto fail;
	}

	/* Debug logs */
	debugfs_create_u32("log_mask", S_IRUGO|S_IWUSR,
		hwpm_linux->debugfs_root, &hwpm->dbg_mask);

	return;

fail:
	debugfs_remove_recursive(hwpm_linux->debugfs_root);
	hwpm_linux->debugfs_root = NULL;
}

void tegra_hwpm_debugfs_deinit(struct tegra_hwpm_os_linux *hwpm_linux)
{
	struct tegra_soc_hwpm *hwpm = &hwpm_linux->hwpm;

	if (!hwpm_linux) {
		tegra_hwpm_err(hwpm, "Invalid hwpm struct");
		return;
	}

	debugfs_remove_recursive(hwpm_linux->debugfs_root);
	hwpm_linux->debugfs_root = NULL;
}
