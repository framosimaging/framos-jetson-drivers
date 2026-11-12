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

#ifndef TEGRA_HWPM_KMEM_H
#define TEGRA_HWPM_KMEM_H

#ifdef __KERNEL__
#include <os/linux/kmem.h>
#else
#include <os/qnx/kmem.h>
#endif

#define tegra_hwpm_kzalloc(hwpm, size)			\
	tegra_hwpm_kzalloc_impl(hwpm, size)
#define tegra_hwpm_kcalloc(hwpm, num, size)		\
	tegra_hwpm_kcalloc_impl(hwpm, num, size)

#define tegra_hwpm_kfree(hwpm, addr)			\
	tegra_hwpm_kfree_impl(hwpm, addr)


#endif /* TEGRA_HWPM_KMEM_H */
