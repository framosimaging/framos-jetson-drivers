// SPDX-License-Identifier: MIT
/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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

#include <tegra_hwpm_static_analysis.h>
#include <tegra_hwpm_common.h>
#include <tegra_hwpm_soc.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>

#include <hal/t264/t264_internal.h>
#include <hal/t264/hw/t264_addr_map_soc_hwpm.h>

/*
 * This function is invoked by register_ip API.
 * Convert the external resource enum to internal IP index.
 * Extract given ip_ops and update corresponding IP structure.
 */
int t264_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available)
{
	int ret = 0;
	u32 ip_idx = 0U;

	tegra_hwpm_fn(hwpm, " ");

	tegra_hwpm_dbg(hwpm, hwpm_dbg_ip_register,
		"Extract IP ops for resource enum %d info", resource_enum);

	/* Convert tegra_soc_hwpm_resource to internal enum */
	if (!(t264_hwpm_is_resource_active(hwpm, resource_enum, &ip_idx))) {
		tegra_hwpm_dbg(hwpm, hwpm_dbg_ip_register,
			"SOC hwpm resource %d (base 0x%llx) is unconfigured",
			resource_enum, (unsigned long long)base_address);
		goto fail;
	}

	switch (ip_idx) {
#if defined(CONFIG_T264_HWPM_IP_VIC)
	case T264_HWPM_IP_VIC:
#endif
#if defined(CONFIG_T264_HWPM_IP_PVA)
	case T264_HWPM_IP_PVA:
#endif
#if defined(CONFIG_T264_HWPM_IP_OCU)
	case T264_HWPM_IP_OCU:
#endif
#if defined(CONFIG_T264_HWPM_IP_SMMU)
	case T264_HWPM_IP_SMMU:
#endif
#if defined(CONFIG_T264_HWPM_IP_UCF_MSW)
	case T264_HWPM_IP_UCF_MSW:
#endif
#if defined(CONFIG_T264_HWPM_IP_UCF_PSW)
	case T264_HWPM_IP_UCF_PSW:
#endif
#if defined(CONFIG_T264_HWPM_IP_UCF_CSW)
	case T264_HWPM_IP_UCF_CSW:
#endif
#if defined(CONFIG_T264_HWPM_IP_CPU)
	case T264_HWPM_IP_CPU:
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

#if defined(CONFIG_T264_HWPM_IP_VI)
	case T264_HWPM_IP_VI:
#endif
#if defined(CONFIG_T264_HWPM_IP_ISP)
	case T264_HWPM_IP_ISP:
#endif
	if (tegra_hwpm_is_hypervisor_mode()) {
		/*
		 * VI and ISP are enabled only on AV+L configuration
		 * as the camera driver is not supported on L4T.
		 */
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
				base_address, ip_idx, available);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"Failed to %s fs/ops for IP %d (base 0x%llx)",
				available == true ? "set" : "reset",
				ip_idx, (unsigned long long)base_address);
			goto fail;
		}
	} else {
		tegra_hwpm_err(hwpm, "Invalid IP %d for ip_ops", ip_idx);
	}
		break;

#if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
	case T264_HWPM_IP_MSS_CHANNEL:
#endif
#if defined(CONFIG_T264_HWPM_IP_MSS_HUBS)
	case T264_HWPM_IP_MSS_HUBS:
#endif
		/* MSS channel and MSS hubs share MC channels */

		/* Check base address in T264_HWPM_IP_MSS_CHANNEL */
#if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
		ip_idx = T264_HWPM_IP_MSS_CHANNEL;
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			/*
			 * Return value of ENODEV will indicate that the base
			 * address doesn't belong to this IP.
			 */
			if (ret != -ENODEV) {
				tegra_hwpm_err(hwpm,
					"IP %d base 0x%llx:Failed to %s fs/ops",
					ip_idx, (unsigned long long)base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			/*
			 * ret = -ENODEV indicates given address doesn't belong
			 * to IP. This means ip_ops will not be set for this IP.
			 * This shouldn't be a reason to fail this function.
			 * Hence, reset ret to 0.
			 */
			ret = 0;
		}
#endif
		/* Check base address in T264_HWPM_IP_MSS_HUBS */
#if defined(CONFIG_T264_HWPM_IP_MSS_HUBS)
		ip_idx = T264_HWPM_IP_MSS_HUBS;
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			/*
			 * Return value of ENODEV will indicate that the base
			 * address doesn't belong to this IP.
			 */
			if (ret != -ENODEV) {
				tegra_hwpm_err(hwpm,
					"IP %d base 0x%llx:Failed to %s fs/ops",
					ip_idx, (unsigned long long)base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			/*
			 * ret = -ENODEV indicates given address doesn't belong
			 * to IP. This means ip_ops will not be set for this IP.
			 * This shouldn't be a reason to fail this function.
			 * Hence, reset ret to 0.
			 */
			ret = 0;
		}
#endif
		break;

	case T264_HWPM_IP_PMA:
	case T264_HWPM_IP_RTR:
	default:
		tegra_hwpm_err(hwpm, "Invalid IP %d for ip_ops", ip_idx);
		break;
	}

fail:
	return ret;
}

static int t264_hwpm_validate_emc_config(struct tegra_soc_hwpm *hwpm)
{
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
# if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
	struct hwpm_ip *chip_ip = NULL;
	struct hwpm_ip_inst *ip_inst = NULL;
	u32 inst_idx = 0U;
	u32 element_mask_max = 0U;
#endif
	u32 mss_disable_fuse_val = 0U;
	u32 mss_disable_fuse_val_mask = 0xFU;
	u32 mss_disable_fuse_bit_idx = 0U;
	u32 emc_element_floorsweep_mask = 0U;
	u32 idx = 0U;
	int err;

	tegra_hwpm_fn(hwpm, " ");

	if (!tegra_hwpm_is_platform_silicon()) {
		tegra_hwpm_err(hwpm,
			"Fuse readl is not implemented yet. Skip for now ");
		return 0;
	}

#define TEGRA_FUSE_OPT_MSS_DISABLE			0x8c0U
	err = tegra_hwpm_fuse_readl(hwpm,
		TEGRA_FUSE_OPT_MSS_DISABLE, &mss_disable_fuse_val);
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
		if (!(mss_disable_fuse_val & (0x1U << mss_disable_fuse_bit_idx))) {
			emc_element_floorsweep_mask |=
				(0xFU << (mss_disable_fuse_bit_idx * 4U));
		}
		mss_disable_fuse_bit_idx++;
		mss_disable_fuse_val_mask = (mss_disable_fuse_val_mask >> 1U);
	} while (mss_disable_fuse_val_mask != 0U);

	/* Set fuse value in MSS IP instances */
	for (idx = 0U; idx < active_chip->get_ip_max_idx(); idx++) {
		switch (idx) {
#if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
		case T264_HWPM_IP_MSS_CHANNEL:
#endif
# if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
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

int t264_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm)
{
	u32 opt_hwpm_disable = 0U;
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

	err = t264_hwpm_validate_emc_config(hwpm);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "failed to validate emc config");
		return err;
	}

#define TEGRA_FUSE_OPT_HWPM_DISABLE		0xc18
	/* Read fuse_opt_hwpm_disable_0 fuse */
	err = tegra_hwpm_fuse_readl(hwpm,
		TEGRA_FUSE_OPT_HWPM_DISABLE, &opt_hwpm_disable);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "opt_hwpm_disable fuse read failed");
		return err;
	}

#define TEGRA_FUSE_FA_MODE			0x48U
	err = tegra_hwpm_fuse_readl(hwpm, TEGRA_FUSE_FA_MODE, &fa_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "fa mode fuse read failed");
		return err;
	}

	/*
	 * Configure global control register to disable PCFIFO interlock
	 * By writing to MSS_HUB_HUBC_CONFIG_0 register
	 */
#define TEGRA_HUB_HUBC_CONFIG0_OFFSET			0x6244U
#define TEGRA_HUB_HUBC_PCFIFO_INTERLOCK_DISABLED	0x1U
	err = tegra_hwpm_write_sticky_bits(hwpm, addr_map_mcb_base_r(),
		TEGRA_HUB_HUBC_CONFIG0_OFFSET,
		TEGRA_HUB_HUBC_PCFIFO_INTERLOCK_DISABLED);
	hwpm_assert_print(hwpm, err == 0, return err,
		"PCFIFO Interlock disable failed");

#define TEGRA_HWPM_GLOBAL_DISABLE_OFFSET		0x300CU
#define TEGRA_HWPM_GLOBAL_DISABLE_DISABLED		0x0U
	err = tegra_hwpm_read_sticky_bits(hwpm, addr_map_pmc_misc_base_r(),
		TEGRA_HWPM_GLOBAL_DISABLE_OFFSET, &hwpm_global_disable);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm global disable read failed");
		return err;
	}

	/*
	 * Do not enable override if FA mode fuse is set. FA_MODE fuse enables
	 * all PERFMONs regardless of level of fuse, sticky bit or secure register
	 * settings.
	 */
	if (fa_mode != 0U) {
		tegra_hwpm_dbg(hwpm, hwpm_info,
			"fa mode fuse enabled, no override required, enable HWPM");
		return 0;
	}

	/* Override enable depends on opt_hwpm_disable and global hwpm disable */
	if ((opt_hwpm_disable == 0U) &&
		(hwpm_global_disable == TEGRA_HWPM_GLOBAL_DISABLE_DISABLED)) {
		tegra_hwpm_dbg(hwpm, hwpm_info,
			"OPT_HWPM_DISABLE fuses are disabled, no override required");
		return 0;
	}

	for (idx = 0U; idx < active_chip->get_ip_max_idx(); idx++) {
		chip_ip = active_chip->chip_ips[idx];

		if ((hwpm_global_disable !=
			TEGRA_HWPM_GLOBAL_DISABLE_DISABLED) ||
				(opt_hwpm_disable != 0U)) {
			/*
			 * Both HWPM_GLOBAL_DISABLE and OPT_HWPM_DISABLE disables all
			 * Perfmons in SOC HWPM. Hence, check for either of them to be set.
			 */
			if ((chip_ip->dependent_fuse_mask &
				TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK) != 0U) {
				/*
				 * check to prevent RTR from being overriden
				 */
				chip_ip->override_enable = true;
			} else {
				tegra_hwpm_dbg(hwpm, hwpm_info,
					"IP %d not overridden", idx);
			}
		}
	}

	return 0;
}

int t264_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;

	tegra_hwpm_fn(hwpm, " ");

	/* Force enable MSS channel IP for AV+L/Q */
	if (tegra_hwpm_is_hypervisor_mode()) {
		/*
		 * MSS CHANNEL
		 * MSS channel driver cannot implement HWPM <-> IP interface in AV + L, and
		 * AV + Q configs. Since MSS channel is part of both POR and non-POR IPs,
		 * this force enable is not limited by minimal config or force enable flags.
		 */
#if defined(CONFIG_T264_HWPM_IP_MSS_CHANNEL)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
				addr_map_mc0_base_r(),
				T264_HWPM_IP_MSS_CHANNEL, true);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"T264_HWPM_IP_MSS_CHANNEL force enable failed");
			return ret;
		}
#endif
	} else {
#if defined(CONFIG_T264_HWPM_ALLOW_FORCE_ENABLE)
		if (tegra_hwpm_is_platform_vsp()) {
		/* Static IP instances as per VSP netlist */
		}
		if (tegra_hwpm_is_platform_silicon()) {
		/* Static IP instances corresponding to silicon */
#if defined(CONFIG_T264_HWPM_IP_OCU)
			if (hwpm->ip_config[TEGRA_HWPM_IP_MCF_OCU]) {
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ocu_base_r(),
					T264_HWPM_IP_OCU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_OCU force enable failed");
					return ret;
				}
			}
#endif
#if defined(CONFIG_T264_HWPM_IP_UCF_PSW)
			if (hwpm->ip_config[TEGRA_HWPM_IP_UCF_PSW]) {
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_psn0_psw_base_r(),
					T264_HWPM_IP_UCF_PSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_PSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_psn1_psw_base_r(),
					T264_HWPM_IP_UCF_PSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_PSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_psn2_psw_base_r(),
					T264_HWPM_IP_UCF_PSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_PSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_psn3_psw_base_r(),
					T264_HWPM_IP_UCF_PSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_PSW force enable failed");
					return ret;
				}
			}
#endif /* CONFIG_T264_HWPM_IP_UCF_PSW */
#if defined(CONFIG_T264_HWPM_IP_UCF_CSW)
			if (hwpm->ip_config[TEGRA_HWPM_IP_UCF_CSW]) {
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_csw0_base_r(),
					T264_HWPM_IP_UCF_CSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_CSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_ucf_csw1_base_r(),
					T264_HWPM_IP_UCF_CSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_CSW force enable failed");
					return ret;
				}
			}
#endif /* CONFIG_T264_HWPM_IP_UCF_CSW */
#if defined(CONFIG_T264_HWPM_IP_UCF_MSW)
			if (hwpm->ip_config[TEGRA_HWPM_IP_UCF_MSW]) {
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc0_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc2_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc4_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc6_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc8_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc10_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc12_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_mc14_base_r(),
					T264_HWPM_IP_UCF_MSW, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_UCF_MSW force enable failed");
					return ret;
				}
			}
#endif /* CONFIG_T264_HWPM_IP_UCF_MSW */
#if defined(CONFIG_T264_HWPM_IP_CPU)
			if (hwpm->ip_config[TEGRA_HWPM_IP_CPU]) {
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore0_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore1_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore2_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore3_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore4_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore5_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore6_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore7_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore8_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore9_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore10_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore11_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore12_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
				ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
					addr_map_cpucore13_base_r(),
					T264_HWPM_IP_CPU, true);
				if (ret != 0) {
					tegra_hwpm_err(hwpm,
						"T264_HWPM_IP_CPU force enable failed");
					return ret;
				}
			}
#endif /* CONFIG_T264_HWPM_IP_CPU */
		}
#endif /* CONFIG_T264_HWPM_ALLOW_FORCE_ENABLE */
	}

	return ret;

}
