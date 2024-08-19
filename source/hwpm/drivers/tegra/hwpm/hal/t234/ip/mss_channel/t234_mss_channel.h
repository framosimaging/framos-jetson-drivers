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

#ifndef T234_HWPM_IP_MSS_CHANNEL_H
#define T234_HWPM_IP_MSS_CHANNEL_H

#if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL)
#define T234_HWPM_ACTIVE_IP_MSS_CHANNEL		T234_HWPM_IP_MSS_CHANNEL,

/* This data should ideally be available in HW headers */
#define T234_HWPM_IP_MSS_CHANNEL_NUM_INSTANCES			1U
#define T234_HWPM_IP_MSS_CHANNEL_NUM_CORE_ELEMENT_PER_INST	16U
#define T234_HWPM_IP_MSS_CHANNEL_NUM_PERFMON_PER_INST		16U
#define T234_HWPM_IP_MSS_CHANNEL_NUM_PERFMUX_PER_INST		16U
#define T234_HWPM_IP_MSS_CHANNEL_NUM_BROADCAST_PER_INST		1U

extern struct hwpm_ip t234_hwpm_ip_mss_channel;

#else
#define T234_HWPM_ACTIVE_IP_MSS_CHANNEL
#endif

#endif /* T234_HWPM_IP_MSS_CHANNEL_H */
