/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2024, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#ifndef TEGRA_HWPM_LINUX_HWPM_ACPI_H
#define TEGRA_HWPM_LINUX_HWPM_ACPI_H

#if defined(CONFIG_ACPI)
#include <linux/acpi.h>

/*
 * The device data contains chip info as expected below:
 * bit 23:20 - platform details
 * bit 19:16 - minor number (not used in the driver)
 * bit 15:08 - chip id
 * bit 07:04 - chip id revision
 */
#if defined(CONFIG_TEGRA_HWPM_TH500)
static const unsigned long hwpm_th500_device_data =
	(PLAT_SI << 20U) | (0x50U << 8U) | (0x0 << 4U);
#endif

static const struct acpi_device_id tegra_hwpm_acpi_match[] = {
#if defined(CONFIG_TEGRA_HWPM_TH500)
        {
                .id = "NVDA2006",
                .driver_data = hwpm_th500_device_data,
        },
#endif
        {
        },
};
MODULE_DEVICE_TABLE(acpi, tegra_hwpm_acpi_match);
#endif

#endif /* TEGRA_HWPM_LINUX_HWPM_ACPI_H */
