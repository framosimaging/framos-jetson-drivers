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

#ifndef TEGRA_HWPM_OS_LINUX_DRIVER_H
#define TEGRA_HWPM_OS_LINUX_DRIVER_H

#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#include <tegra_hwpm.h>

#include <uapi/linux/tegra-soc-hwpm-uapi.h>

#define TEGRA_SOC_HWPM_MODULE_NAME	"tegra-soc-hwpm"

extern struct platform_device *tegra_soc_hwpm_pdev;
extern const struct file_operations tegra_hwpm_ops;

typedef struct tegra_hwpm_linux_atomic_t {
	atomic_t var;
} tegra_hwpm_atomic_t;

struct hwpm_ip_register_list {
	struct tegra_soc_hwpm_ip_ops ip_ops;
	struct hwpm_ip_register_list *next;
};
extern struct hwpm_ip_register_list *ip_register_list_head;

struct tegra_hwpm_os_linux {
	struct tegra_soc_hwpm hwpm;

	/* Device */
	struct platform_device *pdev;
	struct device *dev;
	struct device_node *np;
	struct class class;
	dev_t dev_t;
	struct cdev cdev;

	/* Clocks and resets */
	struct clk *la_clk;
	struct clk *la_parent_clk;
	struct reset_control *la_rst;
	struct reset_control *hwpm_rst;

	/* Device info */
	struct tegra_soc_hwpm_device_info device_info;

	/* Reference count */
	tegra_hwpm_atomic_t usage_count;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_root;
#endif
};

static inline struct tegra_hwpm_os_linux *tegra_hwpm_os_linux_from_cdev(
	struct cdev *cdev)
{
	return container_of(cdev, struct tegra_hwpm_os_linux, cdev);
}

static inline struct tegra_hwpm_os_linux *tegra_hwpm_os_linux_from_hwpm(
	struct tegra_soc_hwpm *hwpm)
{
	return container_of(hwpm, struct tegra_hwpm_os_linux, hwpm);
}

#endif /* TEGRA_HWPM_OS_LINUX_DRIVER_H */
