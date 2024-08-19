/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023, NVIDIA CORPORATION.  All rights reserved.
 */

#include <linux/version.h>
#include <linux/sys_soc.h>
#include <soc/tegra/fuse.h>

#define FUSE_SKU_INFO	0x10

static inline u32 tegra_get_sku_id(void)
{
	if (!tegra_sku_info.sku_id)
		tegra_fuse_readl(FUSE_SKU_INFO, &tegra_sku_info.sku_id);

	return tegra_sku_info.sku_id;
}

static inline bool tegra_platform_is_silicon(void)
{
	const struct soc_device_attribute tegra_soc_attrs[] = {
		{ .revision = "Silicon*" },
		{/* sentinel */}
	};

	if (soc_device_match(tegra_soc_attrs))
		return true;

	return false;
}

static inline bool tegra_platform_is_sim(void)
{
	return !tegra_platform_is_silicon();
}

static inline bool tegra_platform_is_fpga(void)
{
	const struct soc_device_attribute tegra_soc_attrs[] = {
		{ .revision = "*FPGA" },
		{/* sentinel */}
	};

	if (soc_device_match(tegra_soc_attrs))
		return true;

	return false;
}

static inline bool tegra_platform_is_vdk(void)
{
	const struct soc_device_attribute tegra_soc_attrs[] = {
		{ .revision = "VDK" },
		{/* sentinel */}
	};

	if (soc_device_match(tegra_soc_attrs))
		return true;

	return false;
}
