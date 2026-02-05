/* SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef __UTIL_REGISTER_UTIL_H__
#define __UTIL_REGISTER_UTIL_H__

#define DRF_SHIFT32(drf)    ((0?drf) % 32)
#define DRF_MASK32(drf)     (0xFFFFFFFFU>>(31-((1?drf) % 32)+((0?drf) % 32)))

#define DRF_SHIFT64(drf)    ((0?drf) % 64)
#define DRF_MASK64(drf)     (0xFFFFFFFFFFFFFFFFULL>>(63-((1?drf) % 64)+((0?drf) % 64)))

#define DRF_BITRANGE(drf)   ((1?drf) - (0?drf) + 1)

#define REG32_RD(reg32, range) (reg32 >> DRF_SHIFT32(range)) & DRF_MASK32(range)
#define REG32_WR(reg32, range, val) (reg32 & ~(DRF_MASK32(range) << DRF_SHIFT32(range))) | ((val & DRF_MASK32(range)) << DRF_SHIFT32(range))

#define REG32_RD_MS(reg32, mask, shift) (reg32 >> shift) & mask
#define REG32_WR_MS(reg32, mask, shift, val) (reg32 & ~(mask << shift)) | ((val & mask) << shift)

#define REG64_RD(reg64, range) (reg64 >> DRF_SHIFT64(range)) & DRF_MASK64(range)
#define REG64_WR(reg64, range, val) (reg64 & ~(DRF_MASK64(range) << DRF_SHIFT64(range))) | ((val & DRF_MASK64(range)) << DRF_SHIFT64(range))

#define REG64_RD_MS(reg64, mask, shift) (reg64 >> shift) & mask
#define REG64_WR_MS(reg64, mask, shift, val) (reg64 & ~(mask << shift)) | ((val & mask) << shift)

#endif // __UTIL_REGISTER_UTIL_H__