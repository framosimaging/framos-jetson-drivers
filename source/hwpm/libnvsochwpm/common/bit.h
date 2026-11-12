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

#ifndef __BIT_H__
#define __BIT_H__

#include <stdint.h>

#define FIELD_GET(v, m, s) ((v & m) >> s)
#define FIELD_SET(v, m, s) ((v & m) << s)

#define VAL_64(lo, hi) (uint64_t)lo | ((uint64_t)hi << 32U)
#define VAL_LO64(v) FIELD_GET(v, 0xFFFFFFFFULL, 0U)
#define VAL_HI64(v) FIELD_GET(v, 0xFFFFFFFF00000000ULL, 32U)

/* Align value up to the next multiple of alignment */
#define ALIGN_UP(value, alignment) (((value) + ((alignment) - 1)) & ~((alignment) - 1))


#endif /*__BIT_H__*/