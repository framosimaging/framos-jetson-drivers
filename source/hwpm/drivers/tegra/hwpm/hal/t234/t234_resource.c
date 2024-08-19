// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>

#include <hal/t234/t234_internal.h>

#include <hal/t234/hw/t234_pmasys_soc_hwpm.h>
#include <hal/t234/hw/t234_pmmsys_soc_hwpm.h>

int t234_hwpm_perfmon_enable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon)
{
	int err = 0;
	u32 reg_val;

	tegra_hwpm_fn(hwpm, " ");

	/* Enable */
	tegra_hwpm_dbg(hwpm, hwpm_dbg_bind,
		"Enabling PERFMON(0x%llx - 0x%llx)",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa);

	err = tegra_hwpm_readl(hwpm, perfmon,
		pmmsys_sys0_enginestatus_r(0), &reg_val);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm read failed");
		return err;
	}
	reg_val = set_field(reg_val, pmmsys_sys0_enginestatus_enable_m(),
		pmmsys_sys0_enginestatus_enable_out_f());
	err = tegra_hwpm_writel(hwpm, perfmon,
		pmmsys_sys0_enginestatus_r(0), reg_val);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm write failed");
		return err;
	}

	return 0;
}

int t234_hwpm_perfmon_disable(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon)
{
	int err = 0;
	u32 reg_val;

	tegra_hwpm_fn(hwpm, " ");

	if (perfmon->element_type == HWPM_ELEMENT_PERFMUX) {
		/*
		 * Since HWPM elements use perfmon functions,
		 * skip disabling HWPM PERFMUX elements
		 */
		return 0;
	}

	/* Disable */
	tegra_hwpm_dbg(hwpm, hwpm_dbg_release_resource,
		"Disabling PERFMON(0x%llx - 0x%llx)",
		(unsigned long long)perfmon->start_abs_pa,
		(unsigned long long)perfmon->end_abs_pa);

	err = tegra_hwpm_readl(hwpm, perfmon, pmmsys_control_r(0), &reg_val);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm read failed");
		return err;
	}
	reg_val = set_field(reg_val, pmmsys_control_mode_m(),
		pmmsys_control_mode_disable_f());
	err = tegra_hwpm_writel(hwpm, perfmon, pmmsys_control_r(0), reg_val);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm write failed");
		return err;
	}

	return 0;
}
