// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/of_address.h>
#include <linux/dma-buf.h>
#include <linux/module.h>
#include <uapi/linux/tegra-soc-hwpm-uapi.h>

#include <tegra_hwpm_kmem.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm.h>
#include <tegra_hwpm_kmem.h>
#include <tegra_hwpm_common.h>
#include <tegra_hwpm_timers.h>
#include <tegra_hwpm_mem_mgmt.h>
#include <tegra_hwpm_static_analysis.h>

#include <os/linux/driver.h>

MODULE_IMPORT_NS(DMA_BUF);

static int tegra_hwpm_dma_map_stream_buffer(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_alloc_pma_stream *alloc_pma_stream)
{
	struct tegra_hwpm_os_linux *hwpm_linux = NULL;

	tegra_hwpm_fn(hwpm, " ");

	hwpm_linux = tegra_hwpm_os_linux_from_hwpm(hwpm);
	if (!hwpm_linux) {
		tegra_hwpm_err(NULL, "Invalid hwpm_linux struct");
		return -ENODEV;
	}

	hwpm->mem_mgmt->stream_buf_size = alloc_pma_stream->stream_buf_size;
	hwpm->mem_mgmt->stream_dma_buf =
		dma_buf_get(tegra_hwpm_safe_cast_u64_to_s32(
		alloc_pma_stream->stream_buf_fd));
	if (IS_ERR(hwpm->mem_mgmt->stream_dma_buf)) {
		tegra_hwpm_err(hwpm, "Unable to get stream dma_buf");
		return PTR_ERR(hwpm->mem_mgmt->stream_dma_buf);
	}

	hwpm->mem_mgmt->stream_attach =
		dma_buf_attach(hwpm->mem_mgmt->stream_dma_buf, hwpm_linux->dev);
	if (IS_ERR(hwpm->mem_mgmt->stream_attach)) {
		tegra_hwpm_err(hwpm, "Unable to attach stream dma_buf");
		return PTR_ERR(hwpm->mem_mgmt->stream_attach);
	}

	hwpm->mem_mgmt->stream_sgt = dma_buf_map_attachment(
		hwpm->mem_mgmt->stream_attach, DMA_FROM_DEVICE);
	if (IS_ERR(hwpm->mem_mgmt->stream_sgt)) {
		tegra_hwpm_err(hwpm, "Unable to map stream attachment");
		return PTR_ERR(hwpm->mem_mgmt->stream_sgt);
	}

	hwpm->mem_mgmt->stream_buf_va =
		sg_dma_address(hwpm->mem_mgmt->stream_sgt->sgl);
	alloc_pma_stream->stream_buf_pma_va = hwpm->mem_mgmt->stream_buf_va;
	if (alloc_pma_stream->stream_buf_pma_va == 0) {
		tegra_hwpm_err(hwpm, "Invalid stream buffer SMMU IOVA");
		return -ENXIO;
	}

	tegra_hwpm_dbg(hwpm, hwpm_dbg_alloc_pma_stream,
		"stream_buf_pma_va = 0x%llx",
		alloc_pma_stream->stream_buf_pma_va);

	return 0;
}

static int tegra_hwpm_dma_map_mem_bytes_buffer(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_alloc_pma_stream *alloc_pma_stream)
{
	struct tegra_hwpm_os_linux *hwpm_linux = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
	int err = 0;
#endif

	tegra_hwpm_fn(hwpm, " ");

	hwpm_linux = tegra_hwpm_os_linux_from_hwpm(hwpm);
	if (!hwpm_linux) {
		tegra_hwpm_err(NULL, "Invalid hwpm_linux struct");
		return -ENODEV;
	}

	hwpm->mem_mgmt->mem_bytes_dma_buf =
		dma_buf_get(tegra_hwpm_safe_cast_u64_to_s32(
			alloc_pma_stream->mem_bytes_buf_fd));
	if (IS_ERR(hwpm->mem_mgmt->mem_bytes_dma_buf)) {
		tegra_hwpm_err(hwpm, "Unable to get mem bytes dma_buf");
		return PTR_ERR(hwpm->mem_mgmt->mem_bytes_dma_buf);
	}

	hwpm->mem_mgmt->mem_bytes_attach = dma_buf_attach(
		hwpm->mem_mgmt->mem_bytes_dma_buf, hwpm_linux->dev);
	if (IS_ERR(hwpm->mem_mgmt->mem_bytes_attach)) {
		tegra_hwpm_err(hwpm, "Unable to attach mem bytes dma_buf");
		return PTR_ERR(hwpm->mem_mgmt->mem_bytes_attach);
	}

	hwpm->mem_mgmt->mem_bytes_sgt = dma_buf_map_attachment(
			hwpm->mem_mgmt->mem_bytes_attach, DMA_FROM_DEVICE);
	if (IS_ERR(hwpm->mem_mgmt->mem_bytes_sgt)) {
		tegra_hwpm_err(hwpm, "Unable to map mem bytes attachment");
		return PTR_ERR(hwpm->mem_mgmt->mem_bytes_sgt);
	}

	hwpm->mem_mgmt->mem_bytes_buf_va =
		sg_dma_address(hwpm->mem_mgmt->mem_bytes_sgt->sgl);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
	/*
	 * Kernel version 5.11 and above introduces dma_buf_map structure
	 * (called iosys_map in versions later than 5.18).
	 * The kernel virtual address generated from dma_buf_vmap API
	 * is stored in mem_bytes_map, which is a dma_buf_map structure
	 */
	err = dma_buf_vmap(hwpm->mem_mgmt->mem_bytes_dma_buf,
		&hwpm->mem_mgmt->mem_bytes_map);
	if (err != 0) {
		tegra_hwpm_err(hwpm,
			"Unable to map mem_bytes buffer into kernel VA space");
		return -ENOMEM;
	}

	hwpm->mem_mgmt->mem_bytes_kernel = hwpm->mem_mgmt->mem_bytes_map.vaddr;
#else
	hwpm->mem_mgmt->mem_bytes_kernel =
		dma_buf_vmap(hwpm->mem_mgmt->mem_bytes_dma_buf);
	if (!hwpm->mem_mgmt->mem_bytes_kernel) {
		tegra_hwpm_err(hwpm,
			"Unable to map mem_bytes buffer into kernel VA space");
		return -ENOMEM;
	}
#endif

	memset(hwpm->mem_mgmt->mem_bytes_kernel, 0, 32);

	return 0;
}

static int tegra_hwpm_reset_stream_buf(struct tegra_soc_hwpm *hwpm)
{
	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->mem_mgmt->stream_sgt &&
		(!IS_ERR(hwpm->mem_mgmt->stream_sgt))) {
		dma_buf_unmap_attachment(hwpm->mem_mgmt->stream_attach,
			hwpm->mem_mgmt->stream_sgt, DMA_FROM_DEVICE);
	}
	hwpm->mem_mgmt->stream_sgt = NULL;

	if (hwpm->mem_mgmt->stream_attach &&
		(!IS_ERR(hwpm->mem_mgmt->stream_attach))) {
		dma_buf_detach(hwpm->mem_mgmt->stream_dma_buf,
			hwpm->mem_mgmt->stream_attach);
	}
	hwpm->mem_mgmt->stream_attach = NULL;
	hwpm->mem_mgmt->stream_buf_size = 0ULL;
	hwpm->mem_mgmt->stream_buf_va = 0ULL;

	if (hwpm->mem_mgmt->stream_dma_buf &&
		(!IS_ERR(hwpm->mem_mgmt->stream_dma_buf))) {
		dma_buf_put(hwpm->mem_mgmt->stream_dma_buf);
	}
	hwpm->mem_mgmt->stream_dma_buf = NULL;

	if (hwpm->mem_mgmt->mem_bytes_kernel) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
		dma_buf_vunmap(hwpm->mem_mgmt->mem_bytes_dma_buf,
			       &hwpm->mem_mgmt->mem_bytes_map);
#else
		dma_buf_vunmap(hwpm->mem_mgmt->mem_bytes_dma_buf,
			       hwpm->mem_mgmt->mem_bytes_kernel);
#endif
		hwpm->mem_mgmt->mem_bytes_kernel = NULL;
	}

	if (hwpm->mem_mgmt->mem_bytes_sgt &&
		(!IS_ERR(hwpm->mem_mgmt->mem_bytes_sgt))) {
		dma_buf_unmap_attachment(hwpm->mem_mgmt->mem_bytes_attach,
			hwpm->mem_mgmt->mem_bytes_sgt, DMA_FROM_DEVICE);
	}
	hwpm->mem_mgmt->mem_bytes_sgt = NULL;
	hwpm->mem_mgmt->mem_bytes_buf_va = 0ULL;

	if (hwpm->mem_mgmt->mem_bytes_attach &&
		(!IS_ERR(hwpm->mem_mgmt->mem_bytes_attach))) {
		dma_buf_detach(hwpm->mem_mgmt->mem_bytes_dma_buf,
			hwpm->mem_mgmt->mem_bytes_attach);
	}
	hwpm->mem_mgmt->mem_bytes_attach = NULL;

	if (hwpm->mem_mgmt->mem_bytes_dma_buf &&
		(!IS_ERR(hwpm->mem_mgmt->mem_bytes_dma_buf))) {
		dma_buf_put(hwpm->mem_mgmt->mem_bytes_dma_buf);
	}
	hwpm->mem_mgmt->mem_bytes_dma_buf = NULL;

	return 0;
}

int tegra_hwpm_map_stream_buffer(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_alloc_pma_stream *alloc_pma_stream)
{
	int ret = 0, err = 0;

	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->mem_mgmt == NULL) {
		/* Allocate tegra_hwpm_mem_mgmt */
		hwpm->mem_mgmt = tegra_hwpm_kzalloc(hwpm,
			sizeof(struct tegra_hwpm_mem_mgmt));
		if (!hwpm->mem_mgmt) {
			tegra_hwpm_err(NULL,
				"Couldn't allocate memory for mem_mgmt struct");
			return -ENOMEM;
		}
	}

	/* Memory map stream buffer */
	ret = tegra_hwpm_dma_map_stream_buffer(hwpm, alloc_pma_stream);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to map stream buffer");
		goto fail;
	}

	/* Memory map mem bytes buffer */
	ret = tegra_hwpm_dma_map_mem_bytes_buffer(hwpm, alloc_pma_stream);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to map mem bytes buffer");
		goto fail;
	}

	/* Configure memory management */
	ret = hwpm->active_chip->enable_mem_mgmt(hwpm);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to configure stream memory");
		goto fail;
	}

	return 0;

fail:
	/* Invalidate memory config */
	err = hwpm->active_chip->invalidate_mem_config(hwpm);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "Failed to invalidate memory config");
	}

	/* Disable memory management */
	err = hwpm->active_chip->disable_mem_mgmt(hwpm);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "Failed to disable memory management");
	}

	alloc_pma_stream->stream_buf_pma_va = 0;

	/* Reset stream buffer */
	err = tegra_hwpm_reset_stream_buf(hwpm);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "Failed to reset stream buffer");
	}

	tegra_hwpm_release_mem_mgmt(hwpm);

	return ret;
}

int tegra_hwpm_clear_mem_pipeline(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;

	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->mem_mgmt == NULL) {
		/* Memory buffer was not initialized */
		tegra_hwpm_dbg(hwpm, hwpm_dbg_alloc_pma_stream,
			"mem_mgmt struct was uninitialized in this sesion");
		return 0;
	}

	/* Stream MEM_BYTES to clear pipeline */
	if (hwpm->mem_mgmt->mem_bytes_kernel) {
		u32 *mem_bytes_kernel_u32 =
			(u32 *)(hwpm->mem_mgmt->mem_bytes_kernel);
		u32 sleep_msecs = 100;
		struct tegra_hwpm_timeout timeout = {0};

		ret = tegra_hwpm_timeout_init(hwpm, &timeout, 10U);
		if (ret != 0) {
			tegra_hwpm_err(hwpm, "hwpm timeout init failed");
			return ret;
		}

		do {
			ret = hwpm->active_chip->stream_mem_bytes(hwpm);
			if (ret != 0) {
				tegra_hwpm_err(hwpm,
					"Trigger mem_bytes streaming failed");
				goto fail;
			}
			tegra_hwpm_msleep(sleep_msecs);
		} while ((*mem_bytes_kernel_u32 ==
			TEGRA_SOC_HWPM_MEM_BYTES_INVALID) &&
			(tegra_hwpm_timeout_expired(hwpm, &timeout) == 0));

		if (*mem_bytes_kernel_u32 == TEGRA_SOC_HWPM_MEM_BYTES_INVALID) {
			tegra_hwpm_err(hwpm,
				"Timeout expired for MEM_BYTES streaming");
			return -ETIMEDOUT;
		}
	}

	ret = hwpm->active_chip->disable_pma_streaming(hwpm);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to disable pma streaming");
		goto fail;
	}

	/* Disable memory management */
	ret = hwpm->active_chip->disable_mem_mgmt(hwpm);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to disable memory management");
		goto fail;
	}

	/* Reset stream buffer */
	ret = tegra_hwpm_reset_stream_buf(hwpm);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to reset stream buffer");
		goto fail;
	}
fail:
	return ret;
}

int tegra_hwpm_update_mem_bytes(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_update_get_put *update_get_put)
{
	int ret;

	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->mem_mgmt == NULL) {
		/* Memory buffer was not initialized */
		tegra_hwpm_dbg(hwpm, hwpm_dbg_alloc_pma_stream,
			"mem_mgmt struct was uninitialized in this sesion");
		return -ENXIO;
	}

	if (!hwpm->mem_mgmt->mem_bytes_kernel) {
		tegra_hwpm_err(hwpm,
			"mem_bytes buffer is not mapped in the driver");
		return -ENXIO;
	}

	/* Update SW get pointer */
	ret = hwpm->active_chip->update_mem_bytes_get_ptr(hwpm,
		update_get_put->mem_bump);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to update mem_bytes get ptr");
		return -EINVAL;
	}

	/* Stream MEM_BYTES value to MEM_BYTES buffer */
	if (update_get_put->b_stream_mem_bytes) {
		ret = hwpm->active_chip->stream_mem_bytes(hwpm);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"Failed to trigger mem_bytes streaming");
		}
	}

	/* Read HW put pointer */
	if (update_get_put->b_read_mem_head) {
		update_get_put->mem_head =
			hwpm->active_chip->get_mem_bytes_put_ptr(hwpm);
		tegra_hwpm_dbg(hwpm, hwpm_dbg_update_get_put,
			"MEM_HEAD = 0x%llx", update_get_put->mem_head);
	}

	/* Check overflow error status */
	if (update_get_put->b_check_overflow) {
		update_get_put->b_overflowed =
			(u8) hwpm->active_chip->membuf_overflow_status(hwpm);
		tegra_hwpm_dbg(hwpm, hwpm_dbg_update_get_put, "OVERFLOWED = %u",
			update_get_put->b_overflowed);
	}

	return 0;
}

int tegra_hwpm_map_update_allowlist(struct tegra_soc_hwpm *hwpm,
	void *ioctl_struct)
{
	int err = 0;
	u64 pinned_pages = 0;
	u64 alist_buf_size = 0;
	u64 *full_alist_u64 = NULL;
	struct tegra_soc_hwpm_query_allowlist *query_allowlist =
			(struct tegra_soc_hwpm_query_allowlist *)ioctl_struct;
	unsigned long user_va = (unsigned long)(query_allowlist->allowlist);
	unsigned long offset = user_va & ~PAGE_MASK;

	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->alist_map->full_alist_size == 0ULL) {
		tegra_hwpm_err(hwpm,
			"Allowlist size should be computed before");
		return -EINVAL;
	}

	alist_buf_size =
		tegra_hwpm_safe_mult_u64(hwpm->alist_map->full_alist_size,
			hwpm->active_chip->get_alist_buf_size(hwpm));

	tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_allowlist,
		"alist_buf_size 0x%llx", alist_buf_size);

	/* Memory map user buffer into kernel address space */
	alist_buf_size = tegra_hwpm_safe_add_u64(offset, alist_buf_size);

	/* Round-up and Divide */
	alist_buf_size = tegra_hwpm_safe_sub_u64(
		tegra_hwpm_safe_add_u64(alist_buf_size, PAGE_SIZE), 1ULL);
	hwpm->alist_map->num_pages = alist_buf_size / PAGE_SIZE;

	hwpm->alist_map->pages = (struct page **)tegra_hwpm_kcalloc(
		hwpm, hwpm->alist_map->num_pages, sizeof(struct page *));
	if (!hwpm->alist_map->pages) {
		tegra_hwpm_err(hwpm,
			"Couldn't allocate memory for pages array");
		err = -ENOMEM;
		goto fail;
	}

#if defined(NV_GET_USER_PAGES_HAS_ARGS_FLAGS) /* Linux v6.5 */
	pinned_pages = get_user_pages(user_va & PAGE_MASK,
		hwpm->alist_map->num_pages, 0, hwpm->alist_map->pages);
#else
	pinned_pages = get_user_pages(user_va & PAGE_MASK,
		hwpm->alist_map->num_pages, 0, hwpm->alist_map->pages, NULL);
#endif
	if (pinned_pages != hwpm->alist_map->num_pages) {
		tegra_hwpm_err(hwpm, "Requested %llu pages / Got %ld pages",
			hwpm->alist_map->num_pages, pinned_pages);
		err = -ENOMEM;
		goto fail;
	}

	hwpm->alist_map->full_alist_map = vmap(hwpm->alist_map->pages,
		hwpm->alist_map->num_pages, VM_MAP, PAGE_KERNEL);
	if (!hwpm->alist_map->full_alist_map) {
		tegra_hwpm_err(hwpm,
			"Couldn't map allowlist buffer in kernel addr space");
		err = -ENOMEM;
		goto fail;
	}
	full_alist_u64 = (u64 *)(hwpm->alist_map->full_alist_map + offset);

	err = tegra_hwpm_combine_alist(hwpm, full_alist_u64);
	if (err != 0) {
		goto fail;
	}

	query_allowlist->allowlist_size = hwpm->alist_map->full_alist_size;
	return 0;

fail:
	tegra_hwpm_release_alist_map(hwpm);
	return err;
}

void tegra_hwpm_release_alist_map(struct tegra_soc_hwpm *hwpm)
{
	u64 idx = 0U;

	if (hwpm->alist_map) {
		if (hwpm->alist_map->full_alist_map) {
			vunmap(hwpm->alist_map->full_alist_map);
		}

		if (hwpm->alist_map->pages) {
			for (idx = 0ULL; idx < hwpm->alist_map->num_pages;
				idx++) {
				set_page_dirty(hwpm->alist_map->pages[idx]);
				put_page(hwpm->alist_map->pages[idx]);
			}

			tegra_hwpm_kfree(hwpm, hwpm->alist_map->pages);
		}

		tegra_hwpm_kfree(hwpm, hwpm->alist_map);
	}
}

void tegra_hwpm_release_mem_mgmt(struct tegra_soc_hwpm *hwpm)
{
	if (hwpm->mem_mgmt != NULL) {
		tegra_hwpm_kfree(hwpm, hwpm->mem_mgmt);
		hwpm->mem_mgmt = NULL;
	}
}
