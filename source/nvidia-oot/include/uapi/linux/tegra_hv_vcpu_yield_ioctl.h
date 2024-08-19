/* SPDX-License-Identifier: (GPL-2.0 WITH Linux-syscall-note)
 *
 * Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 */

#ifndef _UAPI_TEGRA_HV_VCPU_YIELD_IOCTL_H_
#define _UAPI_TEGRA_HV_VCPU_YIELD_IOCTL_H_

#include <linux/ioctl.h>

#define VCPU_YIELD_IOC_MAGIC    'Y'
#define VCPU_YIELD_START_CMDID  1

/* Control data for VCPU Yield Start ioctl */
struct vcpu_yield_start_ctl {
	/* max time in micro seconds for which vcpu will be yielded */
	uint32_t timeout_us;
};

#define VCPU_YIELD_START_IOCTL _IOW(VCPU_YIELD_IOC_MAGIC, \
		VCPU_YIELD_START_CMDID, struct vcpu_yield_start_ctl)

#endif /* #ifndef _UAPI_TEGRA_HV_VCPU_YIELD_IOCTL_H_ */
