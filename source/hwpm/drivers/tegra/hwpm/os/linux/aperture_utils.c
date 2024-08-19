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

#include <linux/slab.h>
#include <linux/of_address.h>

#include <tegra_hwpm_static_analysis.h>
#include <tegra_hwpm_aperture.h>
#include <tegra_hwpm_kmem.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>
#include <os/linux/driver.h>

int tegra_hwpm_perfmon_reserve_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_inst *ip_inst, struct hwpm_ip_aperture *perfmon)
{
	struct resource *res = NULL;
	struct tegra_hwpm_os_linux *hwpm_linux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	hwpm_linux = tegra_hwpm_os_linux_from_hwpm(hwpm);
	if (!hwpm_linux) {
		tegra_hwpm_err(NULL, "Invalid hwpm_linux struct");
		return -ENODEV;
	}

	/* Reserve */
	if ((perfmon->element_type == HWPM_ELEMENT_PERFMON) ||
		(perfmon->element_type == HWPM_ELEMENT_PERFMUX)) {
		/* Make sure that resource exists in device node */
		res = platform_get_resource(hwpm_linux->pdev,
			IORESOURCE_MEM, perfmon->device_index);
	} else {
		tegra_hwpm_err(hwpm,
			"Unknown perfmon type, execution shouldn't reach here");
		return -EINVAL;
	}
	if ((!res) || (res->start == 0) || (res->end == 0)) {
		tegra_hwpm_err(hwpm, "Failed to get perfmon %s", perfmon->name);
		return -ENOMEM;
	}

	/* Confirm that correct resource is retrived */
	if (res->start != perfmon->start_pa) {
		tegra_hwpm_err(hwpm, "Failed to get correct"
			"perfmon address for %s,"
			"Expected - 0x%llx, Returned - 0x%llx",
			perfmon->name, perfmon->start_pa, res->start);
		return -ENOMEM;
	}

	if (hwpm->fake_registers_enabled) {
		/* Allocate resource memory as MMIO */
		u64 address_range = tegra_hwpm_safe_add_u64(
			tegra_hwpm_safe_sub_u64(res->end, res->start), 1ULL);
		u64 num_regs = address_range / sizeof(u32);

		perfmon->fake_registers =
			tegra_hwpm_kcalloc(hwpm, num_regs, sizeof(u32));
		if (perfmon->fake_registers == NULL) {
			tegra_hwpm_err(hwpm, "Perfmon (0x%llx - 0x%llx) "
				"Couldn't allocate memory for fake regs",
				perfmon->start_abs_pa, perfmon->end_abs_pa);
			return -ENOMEM;
		}
	} else {
		/* Map resource memory in kernel space */
		perfmon->dt_mmio = devm_ioremap(
			hwpm_linux->dev, res->start, resource_size(res));
		if (IS_ERR(perfmon->dt_mmio)) {
			tegra_hwpm_err(hwpm,
				"Couldn't map perfmon %s", perfmon->name);
			return PTR_ERR(perfmon->dt_mmio);
		}
	}

	return 0;
}

int tegra_hwpm_perfmux_reserve_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_inst *ip_inst, struct hwpm_ip_aperture *perfmux)
{
	int ret = 0;
	u32 reg_val = 0U;

	tegra_hwpm_fn(hwpm, " ");

	/* Allocate fake registers */
	if (hwpm->fake_registers_enabled) {
		u64 address_range = tegra_hwpm_safe_add_u64(
			tegra_hwpm_safe_sub_u64(perfmux->end_abs_pa,
				perfmux->start_abs_pa), 1ULL);
		u64 num_regs = address_range / sizeof(u32);

		perfmux->fake_registers =
			tegra_hwpm_kcalloc(hwpm, num_regs, sizeof(u32));
		if (perfmux->fake_registers == NULL) {
			tegra_hwpm_err(hwpm, "Aperture(0x%llx - 0x%llx):"
				" Couldn't allocate memory for fake registers",
				perfmux->start_abs_pa, perfmux->end_abs_pa);
			return -ENOMEM;
		}
	}

	/* Validate perfmux availability by reading 1st alist offset */
	ret = tegra_hwpm_regops_readl(hwpm, ip_inst, perfmux,
		tegra_hwpm_safe_add_u64(perfmux->start_abs_pa,
			perfmux->alist[0U].reg_offset), &reg_val);
	if (ret != 0) {
		/*
		 * If an IP element is unavailable, perfmux register
		 * read will return with failure.
		 * Mark corresponding element as unavailable.
		 * NOTE: This is possible if IP elements are floorswept.
		 * Hence, failure should not be propagated.
		 */
		tegra_hwpm_dbg(hwpm, hwpm_dbg_reserve_resource,
			"perfmux start_abs_pa 0x%llx unavailable",
			perfmux->start_abs_pa);

		ip_inst->element_fs_mask &= ~(perfmux->element_index_mask);
	}

	return 0;
}

int tegra_hwpm_perfmon_release_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon)
{
	struct tegra_hwpm_os_linux *hwpm_linux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	hwpm_linux = tegra_hwpm_os_linux_from_hwpm(hwpm);
	if (!hwpm_linux) {
		tegra_hwpm_err(NULL, "Invalid hwpm_linux struct");
		return -ENODEV;
	}

	if (perfmon->dt_mmio == NULL) {
		tegra_hwpm_err(hwpm, "Perfmon was not mapped");
		return -EINVAL;
	}
	devm_iounmap(hwpm_linux->dev, perfmon->dt_mmio);
	perfmon->dt_mmio = NULL;

	if (perfmon->fake_registers) {
		tegra_hwpm_kfree(hwpm, perfmon->fake_registers);
		perfmon->fake_registers = NULL;
	}
	return 0;
}

int tegra_hwpm_perfmux_release_impl(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmux)
{
	tegra_hwpm_fn(hwpm, " ");

	/*
	 * Release
	 * This is only required for fake registers
	 */
	if (perfmux->fake_registers) {
		tegra_hwpm_kfree(hwpm, perfmux->fake_registers);
		perfmux->fake_registers = NULL;
	}

	return 0;
}

