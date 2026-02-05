// SPDX-License-Identifier: MIT
/* SPDX-FileCopyrightText: Copyright (c) 2022-2024, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
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

#include <linux/stat.h>
#include <linux/moduleparam.h>
#include <tegra_hwpm.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_soc.h>
#include <tegra_hwpm_common.h>
#include <tegra_hwpm_static_analysis.h>

#include <hal/th500/th500_internal.h>
#include <hal/th500/soc/th500_soc_internal.h>
#include <hal/th500/soc/hw/th500_addr_map_soc_hwpm.h>

/*
 * Optional module parameters
 */
#ifdef CONFIG_TH500_HWPM_ALLOW_FORCE_ENABLE
/* This is a WAR on TH500 */
int validate_current_config = 1;
module_param(validate_current_config, int, 0660);

/*
 * IP software masks to be used for force-enablement.
 * 0x0 means "do not force-enable". These are meant to be
 * a stop-gap measure until either we have ability to
 * decide these based on fuses or they have IP drivers.
 */
long nvlctrl_mask;
module_param(nvlctrl_mask, long, 0660);

long nvlrx_mask;
module_param(nvlrx_mask, long, 0660);

long nvltx_mask;
module_param(nvltx_mask, long, 0660);

long c2c_mask;
module_param(c2c_mask, long, 0660);

long cl2_mask;
module_param(cl2_mask, long, 0660);

long mcf_c2c_mask;
module_param(mcf_c2c_mask, long, 0660);

long mcf_clink_mask;
module_param(mcf_clink_mask, long, 0660);

long mcf_core_mask;
module_param(mcf_core_mask, long, 0660);

long mcf_soc_mask;
module_param(mcf_soc_mask, long, 0660);

long mss_channel_mask;
module_param(mss_channel_mask, long, 0660);

long mss_hub_mask;
module_param(mss_hub_mask, long, 0660);

long pcie_mask;
module_param(pcie_mask, long, 0660);

long smmu_mask;
module_param(smmu_mask, long, 0660);

/* Socket number */
int socket_number;
/*
 * The socket number must be 0, 1, 2, or 3.
 */
static int set_socket_number(const char *val, const struct kernel_param *kp)
{
	int socket_num = 0, ret;

	ret = kstrtoint(val, 10, &socket_num);
	if (ret != 0 || socket_num < 0 || socket_num > 3)
		return -EINVAL;

	return param_set_int(val, kp);
}

static const struct kernel_param_ops param_ops = {
	.set	= set_socket_number,
	.get	= param_get_int,
};
module_param_cb(socket, &param_ops, &socket_number, 0664);
#endif /* CONFIG_TH500_HWPM_ALLOW_FORCE_ENABLE */

/*
 * This function is invoked by register_ip API.
 * Convert the external resource enum to internal IP index.
 * Extract given ip_ops and update corresponding IP structure.
 */
int th500_hwpm_extract_ip_ops(struct tegra_soc_hwpm *hwpm,
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
			resource_enum, base_address);
		goto fail;
	}

	switch (ip_idx) {
#if defined(CONFIG_TH500_HWPM_IP_CL2)
	case TH500_HWPM_IP_CL2:
#endif
#if defined(CONFIG_TH500_HWPM_IP_SMMU)
	case TH500_HWPM_IP_SMMU:
#endif
#if defined(CONFIG_TH500_HWPM_IP_C_NVLINK)
	case TH500_HWPM_IP_NVLCTRL:
	case TH500_HWPM_IP_NVLRX:
	case TH500_HWPM_IP_NVLTX:
#endif
#if defined(CONFIG_TH500_HWPM_IP_PCIE)
	case TH500_HWPM_IP_PCIE_XTLQ:
	case TH500_HWPM_IP_PCIE_XTLRC:
	case TH500_HWPM_IP_PCIE_XALRC:
#endif
#if defined(CONFIG_TH500_HWPM_IP_C2C)
	case TH500_HWPM_IP_C2C:
#endif
#if defined(CONFIG_TH500_HWPM_IP_CL2) ||		\
	defined(CONFIG_TH500_HWPM_IP_SMMU) ||		\
	defined(CONFIG_TH500_HWPM_IP_C_NVLINK) ||	\
	defined(CONFIG_TH500_HWPM_IP_PCIE) ||		\
	defined(CONFIG_TH500_HWPM_IP_C2C)
		ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, ip_ops,
			base_address, ip_idx, available);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"IP %d base 0x%llx:Failed to %s fs/ops",
				ip_idx, base_address,
				available == true ? "set" : "reset");
			goto fail;
		}
		break;
#endif
#if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
	case TH500_HWPM_IP_MSS_CHANNEL:
#endif
#if defined(CONFIG_TH500_HWPM_IP_MSS_HUB)
	case TH500_HWPM_IP_MSS_HUB:
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CORE)
	case TH500_HWPM_IP_MCF_CORE:
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CLINK)
	case TH500_HWPM_IP_MCF_CLINK:
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_C2C)
	case TH500_HWPM_IP_MCF_C2C:
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_SOC)
	case TH500_HWPM_IP_MCF_SOC:
#endif
		/*
		 * MSS channel, MCF CORE, MCF CLINK, MCF C2C, MCF SOC,
		 * and MSS HUB share MC channels
		 */

		/* Check base address in TH500_HWPM_IP_MSS_CHANNEL */
#if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
		ip_idx = TH500_HWPM_IP_MSS_CHANNEL;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CORE)
		/* Check base address in TH500_HWPM_IP_MCF_CORE */
		ip_idx = TH500_HWPM_IP_MCF_CORE;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CLINK)
		/* Check base address in TH500_HWPM_IP_MCF_CLINK */
		ip_idx = TH500_HWPM_IP_MCF_CLINK;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_C2C)
		/* Check base address in TH500_HWPM_IP_MCF_C2C */
		ip_idx = TH500_HWPM_IP_MCF_C2C;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_SOC)
		/* Check base address in TH500_HWPM_IP_MCF_SOC */
		ip_idx = TH500_HWPM_IP_MCF_SOC;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
#if defined(CONFIG_TH500_HWPM_IP_MSS_HUB)
		/* Check base address in TH500_HWPM_IP_MSS_HUB */
		ip_idx = TH500_HWPM_IP_MSS_HUB;
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
					ip_idx, base_address,
					available == true ? "set" : "reset");
				goto fail;
			}
			ret = 0;
		}
#endif
		break;
	case TH500_HWPM_IP_PMA:
	case TH500_HWPM_IP_RTR:
	default:
		tegra_hwpm_err(hwpm, "Invalid IP %d for ip_ops", ip_idx);
		break;
	}

fail:
	return ret;
}

static int th500_hwpm_validate_emc_config(struct tegra_soc_hwpm *hwpm)
{
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
#if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
	struct hwpm_ip *chip_ip = NULL;
	struct hwpm_ip_inst *ip_inst = NULL;
	u32 element_mask_max = 0U;
	u32 s_inst_idx = 0U;
#endif
	u32 emc_disable_fuse_val = 0U;
	u32 emc_disable_fuse_val_mask = 0xFU;
	u32 emc_disable_fuse_bit_idx = 0U;
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
		if (!(emc_disable_fuse_val & (0x1U << emc_disable_fuse_bit_idx))) {
			emc_element_floorsweep_mask |=
				(0xFU << (emc_disable_fuse_bit_idx * 4U));
		}
		emc_disable_fuse_bit_idx++;
		emc_disable_fuse_val_mask = (emc_disable_fuse_val_mask >> 1U);
	} while (emc_disable_fuse_val_mask != 0U);

	/* Set fuse value in MSS IP instances */
	for (idx = 0U; idx < active_chip->get_ip_max_idx(); idx++) {
		switch (idx) {
#if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
		case TH500_HWPM_IP_MSS_CHANNEL:
#endif
# if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
			chip_ip = active_chip->chip_ips[idx];
			for (s_inst_idx = 0U;
				s_inst_idx < chip_ip->num_instances;
				s_inst_idx++) {
				ip_inst = &chip_ip->ip_inst_static_array[
					s_inst_idx];

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
			break;
		}
	}
	return 0;
}

int th500_hwpm_validate_current_config(struct tegra_soc_hwpm *hwpm)
{
	u32 production_mode = 0U;
	u32 security_mode = 0U;
	u32 fa_mode = 0U;
	u32 hwpm_global_disable = 0U;
	u32 opt_hwpm_disable = 0U;
	u32 idx = 0U;
	int err;
	struct tegra_soc_hwpm_chip *active_chip = hwpm->active_chip;
	struct hwpm_ip *chip_ip = NULL;

	tegra_hwpm_fn(hwpm, " ");

#ifdef CONFIG_TH500_HWPM_ALLOW_FORCE_ENABLE
	extern int validate_current_config;


	if (validate_current_config != 0)
		return 0;
#endif

	if (!tegra_hwpm_is_platform_silicon())
		return 0;

	err = th500_hwpm_validate_emc_config(hwpm);
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
	err = tegra_hwpm_fuse_readl(
		hwpm, TEGRA_FUSE_SECURITY_MODE, &security_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "security mode fuse read failed");
		return err;
	}

#define TEGRA_FUSE_OPT_HWPM_DISABLE		0x2cU
	err = tegra_hwpm_fuse_readl(
		hwpm, TEGRA_FUSE_SECURITY_MODE, &opt_hwpm_disable);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "opt hwpm disable fuse read failed");
		return err;
	}

#define TEGRA_FUSE_FA_MODE			0x48U
	err = tegra_hwpm_fuse_readl(hwpm, TEGRA_FUSE_FA_MODE, &fa_mode);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "fa mode fuse read failed");
		return err;
	}

#define TEGRA_HWPM_GLOBAL_DISABLE_OFFSET	0x3CU
#define TEGRA_HWPM_GLOBAL_DISABLE_DISABLED	0x1U
	err = tegra_hwpm_read_sticky_bits(hwpm, addr_map_pmc_misc_base_r(),
		TEGRA_HWPM_GLOBAL_DISABLE_OFFSET, &hwpm_global_disable);
	if (err != 0) {
		tegra_hwpm_err(hwpm, "hwpm global disable read failed");
		return err;
	}

	tegra_hwpm_dbg(hwpm, hwpm_info,
		"PROD_MODE fuse = 0x%x "
		"SECURITY_MODE fuse = 0x%x "
		"HWPM disable fuse = 0x%x"
		"FA mode fuse = 0x%x"
		"HWPM_GLOBAL_DISABLE sticky bit = 0x%x",
		production_mode, security_mode, opt_hwpm_disable,
		fa_mode, hwpm_global_disable);

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

	for (idx = 0U; idx < active_chip->get_ip_max_idx(); idx++) {
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

/*
 * This routine force enables IPs in TH500 chip. Many of the IPs in TH500 do
 * not have any IP drivers. Therefore, there is no natural way for them to be
 * enabled for performance monitoring. The approach taken here is to provide
 * module parameters for this driver, such that when the driver is loaded, the
 * user can provide bitmasks for the IPs she wants to enable. For example, to
 * enable PCI-E controllers 0 and 1, she could provide the module parameter
 * pcie_mask=0x3. The valid range of mask values depends on each IP.
 *
 * Linux has two ways to provide module parameters: (1) On the command line at
 * 'insmod' time, or (2) via /etc/modprobe.d/nvhwpm.conf file.
 *
 * The following routine simply examples each mask and force enables the IP by
 * calling tegra_hwpm_set_fs_info_ip_ops(). The complicated looking data
 * structures in this routine are just convenience structures that permit
 * looping over all IPs vs. writing custom code for each IP separately.
 */
int th500_hwpm_force_enable_ips(struct tegra_soc_hwpm *hwpm)
{
	int err = 0;

	tegra_hwpm_fn(hwpm, " ");

#if defined(CONFIG_TH500_HWPM_ALLOW_FORCE_ENABLE)
	extern struct hwpm_ip_inst th500_nvlctrl_inst_static_array[];
	extern struct hwpm_ip_inst th500_nvlrx_inst_static_array[];
	extern struct hwpm_ip_inst th500_nvltx_inst_static_array[];
	extern struct hwpm_ip_inst th500_c2c_inst_static_array[];
	extern struct hwpm_ip_inst th500_cl2_inst_static_array[];
	extern struct hwpm_ip_inst th500_mcf_c2c_inst_static_array[];
	extern struct hwpm_ip_inst th500_mcf_clink_inst_static_array[];
	extern struct hwpm_ip_inst th500_mcf_core_inst_static_array[];
	extern struct hwpm_ip_inst th500_mcf_soc_inst_static_array[];
	extern struct hwpm_ip_inst th500_mss_hub_inst_static_array[];
	extern struct hwpm_ip_inst th500_mss_channel_inst_static_array[];
	extern struct hwpm_ip_inst th500_pcie_xalrc_inst_static_array[];
	extern struct hwpm_ip_inst th500_pcie_xtlrc_inst_static_array[];
	extern struct hwpm_ip_inst th500_pcie_xtlq_inst_static_array[];
	extern struct hwpm_ip_inst th500_smmu_inst_static_array[];
	int ret = 0;
	int ip, inst;
	u64 base_addr;
	struct hwpm_ip_inst *ip_inst = NULL;
	struct hwpm_ip_element_info *elem_info = NULL;

	struct hwpm_force_enable_ip {
		char name[16];
		long int mask;
		int id;
		int instances;
		struct hwpm_ip_inst *inst_static_array;
	};
	struct hwpm_force_enable_ip force_enable_ips[] = {
		{"none", 0, 0, 0, NULL},
#if defined(CONFIG_TH500_HWPM_IP_C_NVLINK)
		{"nvlctrl", nvlctrl_mask, TH500_HWPM_IP_NVLCTRL, TH500_HWPM_IP_NVLCTRL_NUM_INSTANCES, th500_nvlctrl_inst_static_array},
		{"nvlrx", nvlrx_mask, TH500_HWPM_IP_NVLRX, TH500_HWPM_IP_NVLRX_NUM_INSTANCES, th500_nvlrx_inst_static_array},
		{"nvltx", nvltx_mask, TH500_HWPM_IP_NVLTX, TH500_HWPM_IP_NVLTX_NUM_INSTANCES, th500_nvltx_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_C2C)
		{"c2c", c2c_mask, TH500_HWPM_IP_C2C, TH500_HWPM_IP_C2C_NUM_INSTANCES, th500_c2c_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_CL2)
		{"cl2", cl2_mask, TH500_HWPM_IP_CL2, TH500_HWPM_IP_CL2_NUM_INSTANCES, th500_cl2_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_C2C)
		{"mcf_c2c", mcf_c2c_mask, TH500_HWPM_IP_MCF_C2C, TH500_HWPM_IP_MCF_C2C_NUM_INSTANCES, th500_mcf_c2c_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CLINK)
		{"mcf_clink", mcf_clink_mask, TH500_HWPM_IP_MCF_CLINK, TH500_HWPM_IP_MCF_CLINK_NUM_INSTANCES, th500_mcf_clink_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_CORE)
		{"mcf_core", mcf_core_mask, TH500_HWPM_IP_MCF_CORE, TH500_HWPM_IP_MCF_CORE_NUM_INSTANCES, th500_mcf_core_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MCF_SOC)
		{"mcf_soc", mcf_soc_mask, TH500_HWPM_IP_MCF_SOC, TH500_HWPM_IP_MCF_SOC_NUM_INSTANCES, th500_mcf_soc_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MSS_HUB)
		{"mss_hub", mss_hub_mask, TH500_HWPM_IP_MSS_HUB, TH500_HWPM_IP_MSS_HUB_NUM_INSTANCES, th500_mss_hub_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_MSS_CHANNEL)
		{"mss_channel", mss_channel_mask, TH500_HWPM_IP_MSS_CHANNEL, TH500_HWPM_IP_MSS_CHANNEL_NUM_INSTANCES, th500_mss_channel_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_PCIE)
		{"pcie_xalrc", pcie_mask, TH500_HWPM_IP_PCIE_XALRC, TH500_HWPM_IP_PCIE_XALRC_NUM_INSTANCES, th500_pcie_xalrc_inst_static_array},
		{"pcie_xtlrc", pcie_mask, TH500_HWPM_IP_PCIE_XTLRC, TH500_HWPM_IP_PCIE_XTLRC_NUM_INSTANCES, th500_pcie_xtlrc_inst_static_array},
		{"pcie_xtlq", pcie_mask, TH500_HWPM_IP_PCIE_XTLQ, TH500_HWPM_IP_PCIE_XTLQ_NUM_INSTANCES, th500_pcie_xtlq_inst_static_array},
#endif
#if defined(CONFIG_TH500_HWPM_IP_SMMU)
		{"smmu", smmu_mask, TH500_HWPM_IP_SMMU, TH500_HWPM_IP_SMMU_NUM_INSTANCES, th500_smmu_inst_static_array},
#endif
	};
	int force_enable_ips_size = sizeof(force_enable_ips)/sizeof(force_enable_ips[0]);

	for (ip = 1; ip < force_enable_ips_size; ip++) {
		struct hwpm_force_enable_ip *current_ip = &force_enable_ips[ip];

		tegra_hwpm_dbg(hwpm, hwpm_info, "Force enabling %s on socket 0", current_ip->name);

		for (inst = 0; inst < current_ip->instances; inst++) {
			if (!(current_ip->mask & (1ULL << inst))) {
				continue;
			}

			tegra_hwpm_err(hwpm, "\tenabling instance %d...", inst);

			ip_inst = &current_ip->inst_static_array[inst];
			elem_info = &ip_inst->element_info[TEGRA_HWPM_APERTURE_TYPE_PERFMUX];
			base_addr = elem_info->range_start;
			ret = tegra_hwpm_set_fs_info_ip_ops(hwpm, NULL,
				base_addr, current_ip->id, true);
			if (ret != 0) {
				tegra_hwpm_err(hwpm,
					"%s force enable failed for instance %d",
					current_ip->name, inst);
				err = ret;
			}
		}
	}
#endif /* CONFIG_TH500_HWPM_ALLOW_FORCE_ENABLE */

	return err;
}