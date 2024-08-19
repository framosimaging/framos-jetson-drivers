/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 */
#ifndef __UAPI_NVHVIVC_MEMPOOL_IOCTL_H__
#define __UAPI_NVHVIVC_MEMPOOL_IOCTL_H__

#include <linux/ioctl.h>

/* ivc mempool IOCTL magic number */
#define TEGRA_MPLUSERSPACE_IOCTL_MAGIC 0xA6


/* IOCTL definitions */

/* query ivc mempool configuration data */
#define TEGRA_MPLUSERSPACE_IOCTL_GET_INFO \
	_IOR(TEGRA_MPLUSERSPACE_IOCTL_MAGIC, 1, struct ivc_mempool)

#define TEGRA_MPLUSERSPACE_IOCTL_NUMBER_MAX 1

#endif /* __UAPI_NVHVIVC_MEMPOOL_IOCTL_H__ */
