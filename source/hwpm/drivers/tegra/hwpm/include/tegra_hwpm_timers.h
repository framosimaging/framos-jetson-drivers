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

#ifndef TEGRA_HWPM_TIMERS_H
#define TEGRA_HWPM_TIMERS_H

#include <tegra_hwpm_types.h>

#ifdef __KERNEL__
#include <os/linux/timers.h>
#else
#include <os/qnx/timers.h>
#endif

#define tegra_hwpm_timeout_init(hwpm, timeout, retries)	\
	tegra_hwpm_timeout_init_impl(hwpm, timeout, retries)
#define tegra_hwpm_timeout_expired(hwpm, timeout)	\
	tegra_hwpm_timeout_expired_impl(hwpm, timeout)
#define tegra_hwpm_msleep(msecs)	\
	tegra_hwpm_msleep_impl(msecs)

#endif /* TEGRA_HWPM_TIMERS_H */
