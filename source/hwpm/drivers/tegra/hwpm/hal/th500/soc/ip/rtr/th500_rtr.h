/* SPDX-License-Identifier: MIT */
/*
 * SPDX-FileCopyrightText: Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
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

#ifndef TH500_HWPM_IP_RTR_H
#define TH500_HWPM_IP_RTR_H

#define TH500_HWPM_ACTIVE_IP_RTR		TH500_HWPM_IP_RTR,

/* This data should ideally be available in HW headers */
#define TH500_HWPM_IP_RTR_NUM_INSTANCES			2U
#define TH500_HWPM_IP_RTR_NUM_CORE_ELEMENT_PER_INST	1U
#define TH500_HWPM_IP_RTR_NUM_PERFMON_PER_INST		0U
#define TH500_HWPM_IP_RTR_NUM_PERFMUX_PER_INST		1U
#define TH500_HWPM_IP_RTR_NUM_BROADCAST_PER_INST	0U

#define TH500_HWPM_IP_RTR_STATIC_RTR_INST		0U
#define TH500_HWPM_IP_RTR_STATIC_RTR_PERFMUX_INDEX	0U
#define TH500_HWPM_IP_RTR_STATIC_PMA_INST		1U
#define TH500_HWPM_IP_RTR_STATIC_PMA_PERFMUX_INDEX	0U

extern struct hwpm_ip th500_hwpm_ip_rtr;

#endif /* TH500_HWPM_IP_RTR_H */
