// SPDX-License-Identifier: GPL-2.0-only
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

#include <linux/of.h>
#if CONFIG_ACPI
#include <linux/acpi.h>
#endif
#include <soc/tegra/fuse.h>

#include <tegra_hwpm_log.h>
#include <tegra_hwpm_soc.h>
#include <os/linux/driver.h>

#if defined(CONFIG_TEGRA_HWPM_OOT)
#ifdef CONFIG_SOC_TEGRA_PLATFORM_HELPER
#include <soc/tegra/tegra-platform-helper.h>
#endif
#if defined(CONFIG_TEGRA_NEXT1_HWPM)
#include <os/linux/next1_soc_utils.h>
#endif
#if defined(CONFIG_TEGRA_NEXT2_HWPM)
#include <os/linux/next2_soc_utils.h>
#endif

static struct hwpm_soc_chip_info chip_info = {
	.chip_id = CHIP_ID_UNKNOWN,
	.chip_id_rev = CHIP_ID_REV_UNKNOWN,
	.platform = PLAT_INVALID,
};
static bool chip_info_initialized;

static const struct hwpm_soc_chip_info t234_soc_chip_info = {
	.chip_id = 0x23,
	.chip_id_rev = 0x4,
	.platform = PLAT_SI,
};

/* This function should be invoked only once before retrieving soc chip info */
int tegra_hwpm_init_chip_info(struct tegra_hwpm_os_linux *hwpm_linux)
{
	struct device *dev = hwpm_linux->dev;
#if defined(CONFIG_ACPI)
	const struct acpi_device_id *id;
#endif

	if (chip_info_initialized) {
		return 0;
	}

#if defined(CONFIG_ACPI)
	/* Get device node info from ACPI table */
	id = acpi_match_device(dev->driver->acpi_match_table, dev);
	if (id) {
		chip_info.chip_id = (id->driver_data >> 8) & 0xff;
		chip_info.chip_id_rev = (id->driver_data >> 4) & 0xf;
		chip_info.platform = (id->driver_data >> 20) & 0xf;

		goto complete;
	}
#endif
	/* Get device node info from device tree */
	if (of_machine_is_compatible("nvidia,tegra234")) {
		chip_info.chip_id = t234_soc_chip_info.chip_id;
		chip_info.chip_id_rev = t234_soc_chip_info.chip_id_rev;
		chip_info.platform = t234_soc_chip_info.platform;

		goto complete;
	}
#if defined(CONFIG_TEGRA_NEXT1_HWPM)
	if (tegra_hwpm_next1_get_chip_compatible(&chip_info) == 0) {
		goto complete;
	}
#endif
#if defined(CONFIG_TEGRA_NEXT2_HWPM)
	if (tegra_hwpm_next2_get_chip_compatible(&chip_info) == 0) {
		goto complete;
	}
#endif

	return -ENODEV;
complete:
	chip_info_initialized = true;
	return 0;
}

u32 tegra_hwpm_get_chip_id_impl(void)
{
	if (chip_info_initialized) {
		return chip_info.chip_id;
	}
	return CHIP_ID_UNKNOWN;
}

u32 tegra_hwpm_get_major_rev_impl(void)
{
	if (chip_info_initialized) {
		return chip_info.chip_id_rev;
	}
	return CHIP_ID_REV_UNKNOWN;
}

u32 tegra_hwpm_get_platform_impl(void)
{
	if (chip_info_initialized) {
		return chip_info.platform;
	}
	return PLAT_INVALID;
}

u32 tegra_hwpm_chip_get_revision_impl(void)
{
	return 0x0U;
}

bool tegra_hwpm_is_platform_silicon_impl(void)
{
	return tegra_hwpm_get_platform() == PLAT_SI;
}

bool tegra_hwpm_is_platform_simulation_impl(void)
{
	return tegra_hwpm_get_platform() == PLAT_PRE_SI_VDK;
}

bool tegra_hwpm_is_platform_vsp_impl(void)
{
	return tegra_hwpm_get_platform() == PLAT_PRE_SI_VSP;
}

bool tegra_hwpm_is_hypervisor_mode_impl(void)
{
#ifdef CONFIG_SOC_TEGRA_PLATFORM_HELPER
	return tegra_is_hypervisor_mode();
#else
	return false;
#endif
}

#else /* !CONFIG_TEGRA_HWPM_OOT */

u32 tegra_hwpm_get_chip_id_impl(void)
{
	return (u32)tegra_get_chip_id();
}

u32 tegra_hwpm_get_major_rev_impl(void)
{
	return (u32)tegra_get_major_rev();
}

u32 tegra_hwpm_chip_get_revision_impl(void)
{
	return (u32)tegra_chip_get_revision();
}

u32 tegra_hwpm_get_platform_impl(void)
{
	return (u32)tegra_get_platform();
}

bool tegra_hwpm_is_platform_silicon_impl(void)
{
	return tegra_platform_is_silicon();
}

bool tegra_hwpm_is_platform_simulation_impl(void)
{
	return tegra_platform_is_vdk();
}

bool tegra_hwpm_is_platform_vsp_impl(void)
{
	return tegra_platform_is_vsp();
}

bool tegra_hwpm_is_hypervisor_mode_impl(void)
{
	return is_tegra_hypervisor_mode();
}

#endif /* CONFIG_TEGRA_HWPM_OOT */

int tegra_hwpm_fuse_readl_impl(struct tegra_soc_hwpm *hwpm,
	u64 reg_offset, u32 *val)
{
	u32 fuse_val = 0U;
	int err = 0;

	err = tegra_fuse_readl(reg_offset, &fuse_val);
	if (err != 0) {
		return err;
	}

	*val = fuse_val;
	return 0;
}

int tegra_hwpm_fuse_readl_prod_mode_impl(struct tegra_soc_hwpm *hwpm, u32 *val)
{
	return tegra_hwpm_fuse_readl(hwpm, TEGRA_FUSE_PRODUCTION_MODE, val);
}
