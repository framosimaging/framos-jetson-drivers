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

#include <tegra_hwpm.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_soc.h>
#include <tegra_hwpm_common.h>
#include <tegra_hwpm_static_analysis.h>
#include <hal/t234/t234_internal.h>
#include <hal/t234/hw/t234_addr_map_soc_hwpm.h>

/*
 * This function is invoked by register_ip API.
 * Convert the external resource enum to internal IP index.
 * Extract given ip_ops and update corresponding IP structure.
 */
int t234_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available)
{
	int ret = 0;
	u32 ip_idx = 0U;

	tegra_hwpm_fn(hwpm, " ");

	tegra_hwpm_dbg(hwpm, hwpm_dbg_ip_register,
		"Extract IP ops for resource enum %d info", resource_enum);

	/* Convert tegra_soc_hwpm_resource to internal enum */
	if (!(hwpm->active_chip->is_resource_active(hwpm,
		resource_enum, &ip_idx))) {
		tegra_hwpm_dbg(hwpm, hwpm_dbg_ip_register,
			"SOC hwpm resource %d (base 0x%llx) is unconfigured",
			resource_enum, (unsigned long long)base_address);
		goto fail;
	}

	switch (ip_idx) {
#if defined(CONFIG_T234_HWPM_IP_VI)
	case T234_HWPM_IP_VI:
#endif
#if defined(CONFIG_T234_HWPM_IP_ISP)
	case T234_HWPM_IP_ISP:
#endif
#if defined(CONFIG_T234_HWPM_IP_VIC)
	case T234_HWPM_IP_VIC:
#endif
#if defined(CONFIG_T234_HWPM_IP_OFA)
	case T234_HWPM_IP_OFA:
#endif
#if defined(CONFIG_T234_HWPM_IP_PVA)
	case T234_HWPM_IP_PVA:
#endif
#if defined(CONFIG_T234_HWPM_IP_NVDLA)
	case T234_HWPM_IP_NVDLA:
#endif
#if defined(CONFIG_T234_HWPM_IP_MGBE)
	case T234_HWPM_IP_MGBE:
#endif
#if defined(CONFIG_T234_HWPM_IP_SCF)
	case T234_HWPM_IP_SCF:
#endif
#if defined(CONFIG_T234_HWPM_IP_NVDEC)
	case T234_HWPM_IP_NVDEC:
#endif
#if defined(CONFIG_T234_HWPM_IP_NVENC)
	case T234_HWPM_IP_NVENC:
#endif
#if defined(CONFIG_T234_HWPM_IP_PCIE)
	case T234_HWPM_IP_PCIE:
#endif
#if defined(CONFIG_T234_HWPM_IP_DISPLAY)
	case T234_HWPM_IP_DISPLAY:
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_GPU_HUB)
	case T234_HWPM_IP_MSS_GPU_HUB:
#endif
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"Failed to %s fs/ops for IP %d (base 0x%llx)",
				available == true ? "set" : "reset",
				ip_idx, (unsigned long long)base_address);
			goto fail;
		}
		break;
#if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL)
	case T234_HWPM_IP_MSS_CHANNEL:
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_ISO_NISO_HUBS)
	case T234_HWPM_IP_MSS_ISO_NISO_HUBS:
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_MCF)
	case T234_HWPM_IP_MSS_MCF:
#endif
		/* MSS channel, ISO NISO hubs and MCF share MC channels */

		/* Check base address in T234_HWPM_IP_MSS_CHANNEL */
#if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL)
		ip_idx = T234_HWPM_IP_MSS_CHANNEL;
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			/*
			 * Return value of ENODEV will indicate that the base
			 * address doesn't belong to this IP.
			 * This case is valid, as not all base addresses are
			 * shared between MSS IPs.
			 * In this case, reset return value to 0.
			 */
			if (ret != -ENODEV) {
				tegra_hwpm_err(hwpm,
					"IP %d base 0x%llx:Failed to %s fs/ops",
					ip_idx, (unsigned long long)base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_ISO_NISO_HUBS)
		/* Check base address in T234_HWPM_IP_MSS_ISO_NISO_HUBS */
		ip_idx = T234_HWPM_IP_MSS_ISO_NISO_HUBS;
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			/*
			 * Return value of ENODEV will indicate that the base
			 * address doesn't belong to this IP.
			 * This case is valid, as not all base addresses are
			 * shared between MSS IPs.
			 * In this case, reset return value to 0.
			 */
			if (ret != -ENODEV) {
				tegra_hwpm_err(hwpm,
					"IP %d base 0x%llx:Failed to %s fs/ops",
					ip_idx, (unsigned long long)base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_MCF)
		/* Check base address in T234_HWPM_IP_MSS_MCF */
		ip_idx = T234_HWPM_IP_MSS_MCF;
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			/*
			 * Return value of ENODEV will indicate that the base
			 * address doesn't belong to this IP.
			 * This case is valid, as not all base addresses are
			 * shared between MSS IPs.
			 * In this case, reset return value to 0.
			 */
			if (ret != -ENODEV) {
				tegra_hwpm_err(hwpm,
					"IP %d base 0x%llx:Failed to %s fs/ops",
					ip_idx, (unsigned long long)base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
		break;
	case T234_HWPM_IP_PMA:
	case T234_HWPM_IP_RTR:
	default:
		tegra_hwpm_err(hwpm, "Invalid IP %d for ip_ops", ip_idx);
		break;
	}

fail:
	return ret;
}

static int t234_hwpm_validate_emc_config(struct tegra_soc_hwpm *hwpm)
{
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
# if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL) || \
	defined(CONFIG_T234_HWPM_IP_MSS_ISO_NISO_HUBS) || \
	defined(CONFIG_T234_HWPM_IP_MSS_MCF)
	struct hwpm_ip *chip_ip = NULL;
	struct hwpm_ip_inst *ip_inst = NULL;
	u32 inst_idx = 0U;
	u32 element_mask_max = 0U;
#endif
	u32 emc_disable_fuse_val = 0U;
	u32 emc_disable_fuse_val_mask = 0xFU;
	u32 emc_element_floorsweep_mask = 0U;
	u32 idx = 0U;
	int err;

	tegra_hwpm_fn(hwpm, " ");

#define TEGRA_FUSE_EMC_DISABLE			0x8c0U
	err = tegra_hwpm_fuse_readl(hwpm,
		TEGRA_FUSE_EMC_DISABLE, &emc_disable_fuse_val);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "emc_disable fuse read failed");
		return err;
	}

	/*
	 * In floorsweep fuse value,
	 * each bit corresponds to 4 elements.
	 * Bit value 0 indicates those elements are
	 * available and bit value 1 indicates
	 * corresponding elements are floorswept.
	 *
	 * Convert floorsweep fuse value to available EMC elements.
	 */
	do {
		if (emc_disable_fuse_val & 0x1U) {
			emc_element_floorsweep_mask =
				(emc_element_floorsweep_mask << 4U) | 0xFU;
		}
		emc_disable_fuse_val = (emc_disable_fuse_val >> 1U);
		emc_disable_fuse_val_mask = (emc_disable_fuse_val_mask >> 1U);
	} while (emc_disable_fuse_val_mask != 0U);

	/* Set fuse value in MSS IP instances */
	for (idx = 0U; idx < active_chip->get_ip_max_idx(hwpm); idx++) {
		switch (idx) {
#if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL)
		case T234_HWPM_IP_MSS_CHANNEL:
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_ISO_NISO_HUBS)
		case T234_HWPM_IP_MSS_ISO_NISO_HUBS:
#endif
#if defined(CONFIG_T234_HWPM_IP_MSS_MCF)
		case T234_HWPM_IP_MSS_MCF:
#endif
# if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL) || \
	defined(CONFIG_T234_HWPM_IP_MSS_ISO_NISO_HUBS) || \
	defined(CONFIG_T234_HWPM_IP_MSS_MCF)
			chip_ip = active_chip->chip_ips[idx];
			for (inst_idx = 0U; inst_idx < chip_ip->num_instances;
				inst_idx++) {
				ip_inst = &chip_ip->ip_inst_static_array[
					inst_idx];

				/*
				 * Hence use max element mask to get correct
				 * fs info to use in HWPM driver.
				 */
				element_mask_max = tegra_hwpm_safe_sub_u32(
					tegra_hwpm_safe_cast_u64_to_u32(BIT(
					ip_inst->num_core_elements_per_inst)),
					1U);
				ip_inst->fuse_fs_mask =
					(emc_element_floorsweep_mask &
					element_mask_max);
				tegra_hwpm_dbg(hwpm, hwpm_info,
					"ip %d, fuse_mask 0x%x",
					idx, ip_inst->fuse_fs_mask);
			}
			break;
#endif
		default:
			continue;
		}
	}
	return 0;
}

int t234_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm)
{
	u32 production_mode = 0U;
	u32 security_mode = 0U;
	u32 fa_mode = 0U;
	u32 hwpm_global_disable = 0U;
	u32 idx = 0U;
	int err;
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
	struct hwpm_ip *chip_ip = NULL;

	tegra_hwpm_fn(hwpm, " ");

	if (!tegra_hwpm_is_platform_silicon()) {
		return 0;
	}

	err = t234_hwpm_validate_emc_config(hwpm);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "failed to validate emc config");
		return err;
	}

	/* Read production mode fuse */
	err = tegra_hwpm_fuse_readl_prod_mode(hwpm, &production_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "prod mode fuse read failed");
		return err;
	}

#define TEGRA_FUSE_SECURITY_MODE		0xA0U
	err = tegra_hwpm_fuse_readl(hwpm,
		TEGRA_FUSE_SECURITY_MODE, &security_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "security mode fuse read failed");
		return err;
	}

#define TEGRA_FUSE_FA_MODE			0x48U
	err = tegra_hwpm_fuse_readl(hwpm, TEGRA_FUSE_FA_MODE, &fa_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "fa mode fuse read failed");
		return err;
	}

#define TEGRA_HWPM_GLOBAL_DISABLE_OFFSET	0x3CU
#define TEGRA_HWPM_GLOBAL_DISABLE_DISABLED	0x0U
	err = tegra_hwpm_read_sticky_bits(hwpm, addr_map_pmc_misc_base_r(),
		TEGRA_HWPM_GLOBAL_DISABLE_OFFSET, &hwpm_global_disable);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm global disable read failed");
		return err;
	}

	tegra_hwpm_dbg(hwpm, hwpm_info, "PROD_MODE fuse = 0x%x "
		"SECURITY_MODE fuse = 0x%x FA mode fuse = 0x%x"
		"HWPM_GLOBAL_DISABLE = 0x%x",
		production_mode, security_mode, fa_mode, hwpm_global_disable);

	/* Do not enable override if FA mode fuse is set */
	if (fa_mode != 0U) {
		tegra_hwpm_dbg(hwpm, hwpm_info,
			"fa mode fuse enabled, no override required");
		return 0;
	}

	/* Override enable depends on security mode and global hwpm disable */
	if ((security_mode == 0U) &&
		(hwpm_global_disable == TEGRA_HWPM_GLOBAL_DISABLE_DISABLED)) {
		tegra_hwpm_dbg(hwpm, hwpm_info,
			"security fuses are disabled, no override required");
		return 0;
	}

	for (idx = 0U; idx < active_chip->get_ip_max_idx(hwpm); idx++) {
		chip_ip = active_chip->chip_ips[idx];

		if ((hwpm_global_disable !=
			TEGRA_HWPM_GLOBAL_DISABLE_DISABLED) &&
			((chip_ip->dependent_fuse_mask &
			TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK) != 0U)) {
			/* HWPM disable is true */
			/* IP depends on HWPM global disable */
			chip_ip->override_enable = true;
		} else {
			/* HWPM disable is false */
			if ((security_mode != 0U) &&
				((chip_ip->dependent_fuse_mask &
				TEGRA_HWPM_FUSE_SECURITY_MODE_MASK) != 0U)) {
				/* Security mode fuse is set */
				/* IP depends on security mode fuse */
				chip_ip->override_enable = true;
			} else {
				/*
				 * This is a valid case since not all IPs
				 * depend on security fuse.
				 */
				tegra_hwpm_dbg(hwpm, hwpm_info,
					"IP %d not overridden", idx);
			}
		}
	}

	return 0;
}

int t234_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;

	tegra_hwpm_fn(hwpm, " ");

	if (tegra_hwpm_is_hypervisor_mode()) {
		/* MSS CHANNEL */
		/*
		 * MSS channel driver cannot implement HWPM <-> IP
		 * interface in AV + L config.
		 * Since MSS channel is part of both POR and non-POR IPs,
		 * this force enable is not limited by minimal config or
		 * force enable flags.
		 */
#if defined(CONFIG_T234_HWPM_IP_MSS_CHANNEL)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_mc0_base_r(),
			T234_HWPM_IP_MSS_CHANNEL, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_MSS_CHANNEL force enable failed");
			return ret;
		}
#endif
	}

#if defined(CONFIG_T234_HWPM_ALLOW_FORCE_ENABLE)

	/* MSS GPU HUB */
/*
#if defined(CONFIG_T234_HWPM_IP_MSS_GPU_HUB)
	ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
		addr_map_mss_nvlink_1_base_r(),
		T234_HWPM_IP_MSS_GPU_HUB, true);
	if (ret != 0) {
		tegra_hwpm_err(hwpm,
			"T234_HWPM_IP_MSS_GPU_HUB force enable failed");
		return ret;
	}
#endif
*/
	if (tegra_hwpm_is_platform_silicon()) {
		/* Static IP instances corresponding to silicon */
		/* VI */
/*
#if defined(CONFIG_T234_HWPM_IP_VI)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_vi_thi_base_r(),
			T234_HWPM_IP_VI, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_VI force enable failed");
			return ret;
		}
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_vi2_thi_base_r(),
			T234_HWPM_IP_VI, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_VI force enable failed");
			return ret;
		}
#endif
*/
		/* ISP */
/*
#if defined(CONFIG_T234_HWPM_IP_ISP)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_isp_thi_base_r(),
			T234_HWPM_IP_ISP, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_ISP force enable failed");
			return ret;
		}
#endif
*/

		/* MGBE */
/*
#if defined(CONFIG_T234_HWPM_IP_MGBE)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_mgbe0_mac_rm_base_r(),
			T234_HWPM_IP_MGBE, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_MGBE force enable failed");
			return ret;
		}
#endif
*/
		/* NVDEC */
/*
#if defined(CONFIG_T234_HWPM_IP_NVDEC)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_nvdec_base_r(),
			T234_HWPM_IP_NVDEC, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_NVDEC force enable failed");
			return ret;
		}
#endif
*/

		/* PCIE */
/*
#if defined(CONFIG_T234_HWPM_IP_PCIE)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_pcie_c1_ctl_base_r(),
			T234_HWPM_IP_PCIE, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_PCIE force enable failed");
			return ret;
		}
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_pcie_c4_ctl_base_r(),
			T234_HWPM_IP_PCIE, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_PCIE force enable failed");
			return ret;
		}
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_pcie_c5_ctl_base_r(),
			T234_HWPM_IP_PCIE, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_PCIE force enable failed");
			return ret;
		}
#endif
*/

		/* DISPLAY */
/*
#if defined(CONFIG_T234_HWPM_IP_DISPLAY)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
			addr_map_disp_base_r(),
			T234_HWPM_IP_DISPLAY, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T234_HWPM_IP_DISPLAY force enable failed");
			return ret;
		}
#endif
*/
	}
#endif /* CONFIG_T234_HWPM_ALLOW_FORCE_ENABLE */

	return ret;
}
