// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2016-2023, NVIDIA CORPORATION.  All rights reserved.
 */

#define pr_fmt(fmt)	"nvmap: %s() " fmt, __func__

#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/nvmap_t19x.h>
#include <linux/kobject.h>
#include <linux/debugfs.h>
#include <linux/sysfs.h>
#include <linux/io.h>

#include "nvmap_priv.h"

int nvmap_register_cvsram_carveout(struct device *dma_dev,
		phys_addr_t base, size_t size, int (*busy)(void),
		int (*idle)(void))
{
	static struct nvmap_platform_carveout cvsram = {
		.name = "cvsram",
		.usage_mask = NVMAP_HEAP_CARVEOUT_CVSRAM,
		.disable_dynamic_dma_map = true,
		.no_cpu_access = true,
	};

	cvsram.pm_ops.busy = busy;
	cvsram.pm_ops.idle = idle;

	if (!base || !size || (base != PAGE_ALIGN(base)) ||
	    (size != PAGE_ALIGN(size)))
		return -EINVAL;
	cvsram.base = base;
	cvsram.size = size;

	cvsram.dma_dev = &cvsram.dev;
	return nvmap_create_carveout(&cvsram);
}
EXPORT_SYMBOL(nvmap_register_cvsram_carveout);
