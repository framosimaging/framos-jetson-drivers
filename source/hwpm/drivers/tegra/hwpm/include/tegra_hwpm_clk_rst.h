/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#ifndef TEGRA_HWPM_CLK_RST_H
#define TEGRA_HWPM_CLK_RST_H

#ifdef __KERNEL__
#include <os/linux/clk_rst_utils.h>
#else
#include <errno.h>

struct tegra_hwpm_os_qnx;

static inline int tegra_hwpm_clk_rst_prepare(struct tegra_hwpm_os_qnx *hwpm_qnx)
{
	return -EINVAL;
}

static inline int tegra_hwpm_clk_rst_set_rate_enable(struct tegra_hwpm_os_qnx *hwpm_qnx)
{
	return -EINVAL;
}

static inline int tegra_hwpm_clk_rst_disable(struct tegra_hwpm_os_qnx *hwpm_qnx)
{
	return -EINVAL;
}

static inline void tegra_hwpm_clk_rst_release(struct tegra_hwpm_os_qnx *hwpm_qnx)
{
}
#endif

#endif /* TEGRA_HWPM_CLK_RST_H */
