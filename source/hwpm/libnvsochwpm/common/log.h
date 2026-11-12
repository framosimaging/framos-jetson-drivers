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

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL, LOG_COUNT };

#define log_trace(...) nv_soc_hwpm_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) nv_soc_hwpm_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  nv_soc_hwpm_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  nv_soc_hwpm_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) nv_soc_hwpm_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) nv_soc_hwpm_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void nv_soc_hwpm_log(int lvl, const char *file, int line, const char *fmt, ...);

#endif /*__LOG_H__*/