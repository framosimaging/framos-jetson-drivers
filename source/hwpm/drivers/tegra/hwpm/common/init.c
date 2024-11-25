// SPDX-License-Identifier: MIT
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

#include <tegra_hwpm_mem_mgmt.h>
#include <tegra_hwpm_common.h>
#include <tegra_hwpm_kmem.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm_ip.h>
#include <tegra_hwpm.h>

#include <hal/t234/t234_init.h>

#ifdef CONFIG_TEGRA_NEXT1_HWPM
#include <tegra_hwpm_next1_init.h>
#endif
#ifdef CONFIG_TEGRA_NEXT2_HWPM
#include <tegra_hwpm_next2_init.h>
#endif

static int tegra_hwpm_init_chip_ip_structures(struct tegra_soc_hwpm *hwpm,
	u32 chip_id, u32 chip_id_rev)
{
	int err = -EINVAL;

	tegra_hwpm_fn(hwpm, " ");

	switch (chip_id) {
	case 0x23:
		switch (chip_id_rev) {
		case 0x4:
			err = t234_hwpm_init_chip_info(hwpm);
			break;
		default:
#ifdef CONFIG_TEGRA_NEXT1_HWPM
			err = tegra_hwpm_next1_init_chip_ip_structures(hwpm,
				chip_id, chip_id_rev);
#else
			tegra_hwpm_err(hwpm, "Chip 0x%x rev 0x%x not supported",
				chip_id, chip_id_rev);
#endif
			break;
		}
		break;
	default:
#ifdef CONFIG_TEGRA_NEXT2_HWPM
		err = tegra_hwpm_next2_init_chip_ip_structures(
			hwpm, chip_id, chip_id_rev);
#else
		tegra_hwpm_err(hwpm, "Chip 0x%x not supported", chip_id);
#endif
		break;
	}

	if (err != 0) {
		tegra_hwpm_err(hwpm, "init_chip_info failed");
		return err;
	}

	err = tegra_hwpm_func_all_ip(hwpm, NULL, TEGRA_HWPM_INIT_IP_STRUCTURES);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "failed init IP structures");
		return err;
	}

	return err;
}

int tegra_hwpm_init_sw_components(struct tegra_soc_hwpm *hwpm,
	u32 chip_id, u32 chip_id_rev)
{
	int err = 0;

	tegra_hwpm_fn(hwpm, " ");

	hwpm->dbg_mask = TEGRA_HWPM_DEFAULT_DBG_MASK;

	err = tegra_hwpm_init_chip_ip_structures(hwpm, chip_id, chip_id_rev);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "IP structure init failed");
		return err;
	}

	err = tegra_hwpm_finalize_chip_info(hwpm);
	if (err < 0) {
		tegra_hwpm_err(hwpm, "Unable to initialize chip fs_info");
		return err;
	}

	return 0;
}

int tegra_hwpm_setup_sw(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;
	tegra_hwpm_fn(hwpm, " ");

	ret = hwpm->active_chip->validate_current_config(hwpm);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to validate current conifg");
		return ret;
	}

	ret = tegra_hwpm_func_all_ip(hwpm, NULL,
		TEGRA_HWPM_UPDATE_IP_INST_MASK);
	if (ret != 0) {
		tegra_hwpm_err(hwpm, "Failed to update IP fs_info");
		return ret;
	}

	/* Initialize SW state */
	hwpm->bind_completed = false;

	return 0;
}

int tegra_hwpm_setup_hw(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;

	tegra_hwpm_fn(hwpm, " ");

	/*
	 * Map RTR aperture
	 * RTR is hwpm aperture which includes hwpm config registers.
	 * Map/reserve these apertures to get MMIO address required for hwpm
	 * configuration (following steps).
	 */
	ret = hwpm->active_chip->reserve_rtr(hwpm);
	if (ret < 0) {
		tegra_hwpm_err(hwpm, "Unable to reserve RTR aperture");
		goto fail;
	}

	/* Program PROD values */
	ret = hwpm->active_chip->init_prod_values(hwpm);
	if (ret < 0) {
		tegra_hwpm_err(hwpm, "Unable to set PROD values");
		goto fail;
	}

	/* Disable SLCG */
	ret = hwpm->active_chip->disable_cg(hwpm);
	if (ret < 0) {
		tegra_hwpm_err(hwpm, "Unable to disable SLCG");
		goto fail;
	}

	return 0;
fail:
	return ret;
}

int tegra_hwpm_check_status(struct tegra_soc_hwpm *hwpm)
{
	tegra_hwpm_fn(hwpm, " ");

	return hwpm->active_chip->check_status(hwpm);
}

int tegra_hwpm_disable_triggers(struct tegra_soc_hwpm *hwpm)
{
	tegra_hwpm_fn(hwpm, " ");

	return hwpm->active_chip->disable_triggers(hwpm);
}

int tegra_hwpm_release_hw(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;

	tegra_hwpm_fn(hwpm, " ");

	/* Enable CG */
	ret = hwpm->active_chip->enable_cg(hwpm);
	if (ret < 0) {
		tegra_hwpm_err(hwpm, "Unable to enable SLCG");
		goto fail;
	}

	/*
	 * Unmap RTR apertures
	 * Since, RTR hwpm apertures consist of hwpm config registers,
	 * these aperture mappings are required to reset hwpm config.
	 * Hence, explicitly unmap/release these apertures as a last step.
	 */
	ret = hwpm->active_chip->release_rtr(hwpm);
	if (ret < 0) {
		tegra_hwpm_err(hwpm, "Unable to release RTR aperture");
		goto fail;
	}

	return 0;
fail:
	return ret;
}

void tegra_hwpm_release_sw_setup(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;

	tegra_hwpm_fn(hwpm, " ");

	err = tegra_hwpm_func_all_ip(hwpm, NULL,
		TEGRA_HWPM_RELEASE_IP_STRUCTURES);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "failed release IP structures");
		return;
	}

	tegra_hwpm_kfree(hwpm, hwpm->active_chip->chip_ips);

	return;
}

/* Validate HALs that are expected to be populated for each chip */
bool tegra_hwpm_validate_primary_hals(struct tegra_soc_hwpm *hwpm)
{
	tegra_hwpm_fn(hwpm, " ");

	if (hwpm->active_chip->is_ip_active == NULL) {
		tegra_hwpm_err(hwpm, "is_ip_active HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->is_resource_active == NULL) {
		tegra_hwpm_err(hwpm, "is_resource_active HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_rtr_int_idx == NULL) {
		tegra_hwpm_err(hwpm, "get_rtr_int_idx HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_ip_max_idx == NULL) {
		tegra_hwpm_err(hwpm, "get_ip_max_idx HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->extract_ip_ops == NULL) {
		tegra_hwpm_err(hwpm, "extract_ip_ops uninitialized");
		return false;
	}

	if (hwpm->active_chip->force_enable_ips == NULL) {
		tegra_hwpm_err(hwpm, "force_enable_ips uninitialized");
		return false;
	}

	if (hwpm->active_chip->validate_current_config == NULL) {
		tegra_hwpm_err(hwpm, "validate_current_config uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_fs_info == NULL) {
		tegra_hwpm_err(hwpm, "get_fs_info uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_resource_info == NULL) {
		tegra_hwpm_err(hwpm, "get_resource_info uninitialized");
		return false;
	}

	if (hwpm->active_chip->init_prod_values == NULL) {
		tegra_hwpm_err(hwpm, "init_prod_values uninitialized");
		return false;
	}

	if (hwpm->active_chip->disable_cg == NULL) {
		tegra_hwpm_err(hwpm, "disable_cg uninitialized");
		return false;
	}

	if (hwpm->active_chip->enable_cg == NULL) {
		tegra_hwpm_err(hwpm, "enable_cg uninitialized");
		return false;
	}

	if (hwpm->active_chip->reserve_rtr == NULL) {
		tegra_hwpm_err(hwpm, "reserve_rtr uninitialized");
		return false;
	}

	if (hwpm->active_chip->release_rtr == NULL) {
		tegra_hwpm_err(hwpm, "release_rtr uninitialized");
		return false;
	}

	if (hwpm->active_chip->perfmon_enable == NULL) {
		tegra_hwpm_err(hwpm, "perfmon_enable HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->perfmon_disable == NULL) {
		tegra_hwpm_err(hwpm, "perfmon_disable HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->perfmux_disable == NULL) {
		tegra_hwpm_err(hwpm, "perfmux_disable HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->disable_triggers == NULL) {
		tegra_hwpm_err(hwpm, "disable_triggers uninitialized");
		return false;
	}

	if (hwpm->active_chip->check_status == NULL) {
		tegra_hwpm_err(hwpm, "check_status uninitialized");
		return false;
	}

	if (hwpm->active_chip->disable_mem_mgmt == NULL) {
		tegra_hwpm_err(hwpm, "disable_mem_mgmt HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->enable_mem_mgmt == NULL) {
		tegra_hwpm_err(hwpm, "enable_mem_mgmt HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->invalidate_mem_config == NULL) {
		tegra_hwpm_err(hwpm, "invalidate_mem_config HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->stream_mem_bytes == NULL) {
		tegra_hwpm_err(hwpm, "stream_mem_bytes uninitialized");
		return false;
	}

	if (hwpm->active_chip->disable_pma_streaming == NULL) {
		tegra_hwpm_err(hwpm, "disable_pma_streaming uninitialized");
		return false;
	}

	if (hwpm->active_chip->update_mem_bytes_get_ptr == NULL) {
		tegra_hwpm_err(hwpm, "update_mem_bytes_get_ptr uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_mem_bytes_put_ptr == NULL) {
		tegra_hwpm_err(hwpm, "get_mem_bytes_put_ptr uninitialized");
		return false;
	}

	if (hwpm->active_chip->membuf_overflow_status == NULL) {
		tegra_hwpm_err(hwpm, "membuf_overflow_status uninitialized");
		return false;
	}

	if (hwpm->active_chip->get_alist_buf_size == NULL) {
		tegra_hwpm_err(hwpm, "alist_buf_size uninitialized");
		return false;
	}

	if (hwpm->active_chip->zero_alist_regs == NULL) {
		tegra_hwpm_err(hwpm, "zero_alist_regs HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->copy_alist == NULL) {
		tegra_hwpm_err(hwpm, "copy_alist HAL uninitialized");
		return false;
	}

	if (hwpm->active_chip->check_alist == NULL) {
		tegra_hwpm_err(hwpm, "check_alist uninitialized");
		return false;
	}

	if (hwpm->active_chip->validate_secondary_hals == NULL) {
		tegra_hwpm_err(hwpm,
			"validate_secondary_hals HAL uninitialized");
		return false;
	}

	return hwpm->active_chip->validate_secondary_hals(hwpm);
}
