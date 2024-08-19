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

#ifndef TEGRA_HWPM_APERTURE_H
#define TEGRA_HWPM_APERTURE_H

#ifdef __KERNEL__
#include <os/linux/aperture_utils.h>
#else
#include <os/qnx/aperture_utils.h>
#endif

#define tegra_hwpm_perfmon_reserve(hwpm, ip_inst, perfmon)	\
	tegra_hwpm_perfmon_reserve_impl(hwpm, ip_inst, perfmon)
#define tegra_hwpm_perfmux_reserve(hwpm, ip_inst, perfmux)	\
	tegra_hwpm_perfmux_reserve_impl(hwpm, ip_inst, perfmux)
#define tegra_hwpm_perfmon_release(hwpm, perfmon)		\
	tegra_hwpm_perfmon_release_impl(hwpm, perfmon)
#define tegra_hwpm_perfmux_release(hwpm, perfmux)		\
	tegra_hwpm_perfmux_release_impl(hwpm, perfmux)

#endif /* TEGRA_HWPM_APERTURE_H */
