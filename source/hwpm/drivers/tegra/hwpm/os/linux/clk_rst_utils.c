// SPDX-License-Identifier: GPL-2.0-only
// SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

#include <linux/bits.h>
#include <linux/reset.h>
#include <linux/clk.h>

#include <tegra_hwpm.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_soc.h>
#include <tegra_hwpm_clk_rst.h>
#include <os/linux/driver.h>

int tegra_hwpm_clk_rst_prepare(struct tegra_hwpm_os_linux *hwpm_linux)
{
	int ret = 0;
	struct tegra_soc_hwpm *hwpm = &hwpm_linux->hwpm;

	if (tegra_hwpm_is_platform_silicon()) {
		hwpm_linux->la_clk = devm_clk_get(hwpm_linux->dev, "la");
		if (IS_ERR(hwpm_linux->la_clk)) {
			tegra_hwpm_err(hwpm, "Missing la clock");
			ret = PTR_ERR(hwpm_linux->la_clk);
			goto fail;
		}

		hwpm_linux->la_parent_clk =
			devm_clk_get(hwpm_linux->dev, "parent");
		if (IS_ERR(hwpm_linux->la_parent_clk)) {
			tegra_hwpm_err(hwpm, "Missing la parent clk");
			ret = PTR_ERR(hwpm_linux->la_parent_clk);
			goto fail;
		}

		hwpm_linux->la_rst =
			devm_reset_control_get(hwpm_linux->dev, "la");
		if (IS_ERR(hwpm_linux->la_rst)) {
			tegra_hwpm_err(hwpm, "Missing la reset");
			ret = PTR_ERR(hwpm_linux->la_rst);
			goto fail;
		}

		hwpm_linux->hwpm_rst =
			devm_reset_control_get(hwpm_linux->dev, "hwpm");
		if (IS_ERR(hwpm_linux->hwpm_rst)) {
			tegra_hwpm_err(hwpm, "Missing hwpm reset");
			ret = PTR_ERR(hwpm_linux->hwpm_rst);
			goto fail;
		}
	}

fail:
	return ret;
}

int tegra_hwpm_clk_rst_set_rate_enable(struct tegra_hwpm_os_linux *hwpm_linux)
{
	int ret = 0;
	struct tegra_soc_hwpm *hwpm = &hwpm_linux->hwpm;

	if (tegra_hwpm_is_platform_silicon()) {
		ret = reset_control_assert(hwpm_linux->hwpm_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "hwpm reset assert failed");
			goto fail;
		}
		ret = reset_control_assert(hwpm_linux->la_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "la reset assert failed");
			goto fail;
		}
		/* Set required parent for la_clk */
		if (hwpm_linux->la_clk && hwpm_linux->la_parent_clk) {
			ret = clk_set_parent(
				hwpm_linux->la_clk, hwpm_linux->la_parent_clk);
			if (ret < 0) {
				tegra_hwpm_err(hwpm,
					"la clk set parent failed");
				goto fail;
			}
		}
		/* set la_clk rate */
		ret = clk_set_rate(hwpm_linux->la_clk,
			hwpm->active_chip->la_clk_rate);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "la clock set rate failed");
			goto fail;
		}
		ret = clk_prepare_enable(hwpm_linux->la_clk);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "la clock enable failed");
			goto fail;
		}
		ret = reset_control_deassert(hwpm_linux->la_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "la reset deassert failed");
			goto fail;
		}
		ret = reset_control_deassert(hwpm_linux->hwpm_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "hwpm reset deassert failed");
			goto fail;
		}
	}

fail:
	return ret;
}

int tegra_hwpm_clk_rst_disable(struct tegra_hwpm_os_linux *hwpm_linux)
{
	int ret = 0;
	struct tegra_soc_hwpm *hwpm = &hwpm_linux->hwpm;

	if (tegra_hwpm_is_platform_silicon()) {
		ret = reset_control_assert(hwpm_linux->hwpm_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "hwpm reset assert failed");
			goto fail;
		}
		ret = reset_control_assert(hwpm_linux->la_rst);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "la reset assert failed");
			goto fail;
		}
		clk_disable_unprepare(hwpm_linux->la_clk);
	}
fail:
	return ret;
}

void tegra_hwpm_clk_rst_release(struct tegra_hwpm_os_linux *hwpm_linux)
{
	if (tegra_hwpm_is_platform_silicon()) {
		if (hwpm_linux->la_clk) {
			devm_clk_put(hwpm_linux->dev, hwpm_linux->la_clk);
		}
		if (hwpm_linux->la_parent_clk) {
			devm_clk_put(hwpm_linux->dev,
				hwpm_linux->la_parent_clk);
		}
		if (hwpm_linux->la_rst) {
			reset_control_assert(hwpm_linux->la_rst);
		}
		if (hwpm_linux->hwpm_rst) {
			reset_control_assert(hwpm_linux->hwpm_rst);
		}
	}
}
