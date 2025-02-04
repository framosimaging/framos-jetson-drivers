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

#ifndef TEGRA_HWPM_IO_H
#define TEGRA_HWPM_IO_H

#include "tegra_hwpm_types.h"

/**
 * Sets a particular field value in input data.
 *
 * Uses mask to clear specific bit positions in curr_val. field_val
 * is used to set the bits in curr_val to be returned.
 * Note: Function does not perform any validation of input parameters.
 *
 * curr_val [in] Current input data value.
 *
 * mask [in] Mask of the bits to be updated.
 *
 * field_val [in] Value to change the mask bits to.
 *
 * Returns updated value.
 */
static inline u32 set_field(u32 curr_val, u32 mask, u32 field_val)
{
	return ((curr_val & ~mask) | field_val);
}

/**
 * Retrieve value of specific bits from input data.
 * Note: Function does not perform any validation of input parameters.
 *
 * input_data [in] Data to retrieve value from.
 *
 * mask [in] Mask of the bits to get value from.
 *
 * Return value from input_data corresponding to mask bits.
 */
static inline u32 get_field(u32 input_data, u32 mask)
{
	return (input_data & mask);
}

#ifdef __KERNEL__
#include <os/linux/io_utils.h>
#else
#include <os/qnx/io_utils.h>
#endif

#define tegra_hwpm_read_sticky_bits(hwpm, reg_base, reg_offset, val) \
	tegra_hwpm_read_sticky_bits_impl(hwpm, reg_base, reg_offset, val)

#define tegra_hwpm_fake_readl(hwpm, aperture, addr, val)	\
	tegra_hwpm_fake_readl_impl(hwpm, aperture, addr, val)

#define tegra_hwpm_fake_writel(hwpm, aperture, addr, val)	\
	tegra_hwpm_fake_writel_impl(hwpm, aperture, addr, val)

#define tegra_hwpm_readl(hwpm, aperture, addr, val)	\
	tegra_hwpm_readl_impl(hwpm, aperture, addr, val)

#define tegra_hwpm_writel(hwpm, aperture, addr, val)	\
	tegra_hwpm_writel_impl(hwpm, aperture, addr, val)

#define tegra_hwpm_regops_readl(hwpm, ip_inst, aperture, addr, val)	\
	tegra_hwpm_regops_readl_impl(hwpm, ip_inst, aperture, addr, val)

#define tegra_hwpm_regops_writel(hwpm, ip_inst, aperture, addr, val)	\
	tegra_hwpm_regops_writel_impl(hwpm, ip_inst, aperture, addr, val)

#endif /* TEGRA_HWPM_IO_H */
