/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2023, NVIDIA Corporation. All rights reserved.
 */

#ifndef __LINUX_NVHOST_T194_H__
#define __LINUX_NVHOST_T194_H__

int nvhost_syncpt_unit_interface_get_aperture(
				struct platform_device *host_pdev,
				phys_addr_t *base,
				size_t *size);

u32 nvhost_syncpt_unit_interface_get_byte_offset(u32 syncpt_id);

#ifdef CONFIG_TEGRA_HOST1X
u32 nvhost_syncpt_unit_interface_get_byte_offset_ext(
				struct platform_device *host_pdev,
				u32 syncpt_id);
#else
static inline u32 nvhost_syncpt_unit_interface_get_byte_offset_ext(
				struct platform_device *host_pdev,
				u32 syncpt_id)
{
	return nvhost_syncpt_unit_interface_get_byte_offset(syncpt_id);
}
#endif

#endif /* __LINUX_NVHOST_T194_H__ */
