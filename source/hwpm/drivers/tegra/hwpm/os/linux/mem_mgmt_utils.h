/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TEGRA_HWPM_OS_LINUX_MEM_MGMT_UTILS_H
#define TEGRA_HWPM_OS_LINUX_MEM_MGMT_UTILS_H

#include <nvidia/conftest.h>

#include <linux/types.h>
#include <linux/version.h>
#if defined(NV_LINUX_IOSYS_MAP_H_PRESENT)
#include <linux/iosys-map.h>
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
#include <linux/dma-buf-map.h>
#endif
#endif

/* This macro is copy of TEGRA_SOC_HWPM_MEM_BYTES_INVALID */
#define TEGRA_HWPM_MEM_BYTES_INVALID	0xffffffff

struct tegra_soc_hwpm;
struct tegra_soc_hwpm_alloc_pma_stream;
struct tegra_soc_hwpm_update_get_put;
struct sg_table;
struct dma_buf;
struct dma_buf_attachment;
struct tegra_soc_hwpm_update_get_put;

struct tegra_hwpm_mem_mgmt {
	struct sg_table *stream_sgt;
	struct sg_table *mem_bytes_sgt;
	struct dma_buf *stream_dma_buf;
	struct dma_buf_attachment *stream_attach;
	u64 stream_buf_size;
	u64 stream_buf_va;
	u64 mem_bytes_buf_va;
	struct dma_buf *mem_bytes_dma_buf;
	struct dma_buf_attachment *mem_bytes_attach;
	void *mem_bytes_kernel;
#if defined(NV_LINUX_IOSYS_MAP_H_PRESENT)
	struct iosys_map mem_bytes_map;
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
	struct dma_buf_map mem_bytes_map;
#endif
#endif
};

struct tegra_hwpm_allowlist_map {
	u64 full_alist_size;
	u64 num_pages;
	struct page **pages;
	void *full_alist_map;
};

int tegra_hwpm_map_stream_buffer(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_alloc_pma_stream *alloc_pma_stream);
int tegra_hwpm_clear_mem_pipeline(struct tegra_soc_hwpm *hwpm);
int tegra_hwpm_update_mem_bytes(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_update_get_put *update_get_put);
int tegra_hwpm_map_update_allowlist(struct tegra_soc_hwpm *hwpm,
	void *ioctl_struct);
void tegra_hwpm_release_alist_map(struct tegra_soc_hwpm *hwpm);
void tegra_hwpm_release_mem_mgmt(struct tegra_soc_hwpm *hwpm);

#endif /* TEGRA_HWPM_OS_LINUX_MEM_MGMT_UTILS_H */
