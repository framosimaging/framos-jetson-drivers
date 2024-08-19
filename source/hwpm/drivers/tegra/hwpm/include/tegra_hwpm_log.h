/* SPDX-License-Identifier: MIT */
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

#ifndef TEGRA_HWPM_LOG_H
#define TEGRA_HWPM_LOG_H

#ifdef __KERNEL__
#include <linux/bits.h>
#include <os/linux/log.h>
#else
#include <os/qnx/log.h>
#endif

#define TEGRA_SOC_HWPM_MODULE_NAME	"tegra-soc-hwpm"

enum tegra_soc_hwpm_log_type {
	TEGRA_HWPM_ERROR,	/* Error prints */
	TEGRA_HWPM_DEBUG,	/* Debug prints */
};

#define TEGRA_HWPM_DEFAULT_DBG_MASK	(0)
/* Primary info prints */
#define hwpm_info			BIT(0)
/* Trace function execution */
#define hwpm_fn				BIT(1)
/* Log register accesses */
#define hwpm_register			BIT(2)
/* General verbose prints */
#define hwpm_verbose			BIT(3)
/* Driver init specific verbose prints */
#define hwpm_dbg_driver_init		BIT(4)
/* IP register specific verbose prints */
#define hwpm_dbg_ip_register		BIT(5)
/* Device info specific verbose prints */
#define hwpm_dbg_device_info		BIT(6)
/* Floorsweep info specific verbose prints */
#define hwpm_dbg_floorsweep_info	BIT(7)
/* Resource info specific verbose prints */
#define hwpm_dbg_resource_info		BIT(8)
/* Reserve resource specific verbose prints */
#define hwpm_dbg_reserve_resource	BIT(9)
/* Release resource specific verbose prints */
#define hwpm_dbg_release_resource	BIT(10)
/* Alloc PMA stream specific verbose prints */
#define hwpm_dbg_alloc_pma_stream	BIT(11)
/* Bind operation specific verbose prints */
#define hwpm_dbg_bind			BIT(12)
/* Allowlist specific verbose prints */
#define hwpm_dbg_allowlist		BIT(13)
/* Regops specific verbose prints */
#define hwpm_dbg_regops			BIT(14)
/* Get Put pointer specific verbose prints */
#define hwpm_dbg_update_get_put		BIT(15)
/* Driver release specific verbose prints */
#define hwpm_dbg_driver_release		BIT(16)
/* Kmem debug prints */
#define hwpm_dbg_kmem			BIT(17)

#ifdef __KERNEL__

#define tegra_hwpm_err(hwpm, fmt, arg...)				\
	tegra_hwpm_err_impl(hwpm, __func__, __LINE__, fmt, ##arg)
#define tegra_hwpm_dbg(hwpm, dbg_mask, fmt, arg...)			\
	tegra_hwpm_dbg_impl(hwpm, dbg_mask, __func__, __LINE__, fmt, ##arg)
#define tegra_hwpm_fn(hwpm, fmt, arg...)				\
	tegra_hwpm_dbg_impl(hwpm, hwpm_fn, __func__, __LINE__, fmt, ##arg)
#else
#define tegra_hwpm_err(hwpm, fmt, arg...)                               \
        tegra_hwpm_err_impl(hwpm, fmt, ##arg)
#define tegra_hwpm_dbg(hwpm, dbg_mask, fmt, arg...)                     \
        tegra_hwpm_dbg_impl(hwpm, dbg_mask, fmt, ##arg)
#define tegra_hwpm_fn(hwpm, fmt, arg...)                                \
        tegra_hwpm_dbg_impl(hwpm, hwpm_fn, fmt, ##arg)
#endif

#endif /* TEGRA_HWPM_LOG_H */
