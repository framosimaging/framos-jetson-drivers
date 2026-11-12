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

#include "common/log.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char* kLvlString[LOG_COUNT] = {
	"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

void nv_soc_hwpm_log(int lvl, const char *file, int line, const char *fmt, ...)
{
	// TODO: actual implementation with different log levels.
	printf("[%s] %s:%d - ", kLvlString[lvl], file, line);
	va_list args;
    	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

#ifdef __cplusplus
} /* extern "C" */
#endif