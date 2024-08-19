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

#include <linux/slab.h>
#include <uapi/linux/tegra-soc-hwpm-uapi.h>

#include <tegra_hwpm_kmem.h>
#include <tegra_hwpm_log.h>
#include <tegra_hwpm_io.h>
#include <tegra_hwpm.h>
#include <os/linux/ip_utils.h>
#include <os/linux/driver.h>

struct platform_device *tegra_soc_hwpm_pdev;
struct hwpm_ip_register_list *ip_register_list_head;

#define REGISTER_IP	true
#define UNREGISTER_IP	false

static u32 tegra_hwpm_translate_soc_hwpm_ip(struct tegra_soc_hwpm *hwpm,
	enum tegra_soc_hwpm_ip ip_enum)
{
	u32 ip_enum_idx = TEGRA_HWPM_IP_INACTIVE;

	switch (ip_enum) {
	case TEGRA_SOC_HWPM_IP_VI:
		ip_enum_idx = TEGRA_HWPM_IP_VI;
		break;
	case TEGRA_SOC_HWPM_IP_ISP:
		ip_enum_idx = TEGRA_HWPM_IP_ISP;
		break;
	case TEGRA_SOC_HWPM_IP_VIC:
		ip_enum_idx = TEGRA_HWPM_IP_VIC;
		break;
	case TEGRA_SOC_HWPM_IP_OFA:
		ip_enum_idx = TEGRA_HWPM_IP_OFA;
		break;
	case TEGRA_SOC_HWPM_IP_PVA:
		ip_enum_idx = TEGRA_HWPM_IP_PVA;
		break;
	case TEGRA_SOC_HWPM_IP_NVDLA:
		ip_enum_idx = TEGRA_HWPM_IP_NVDLA;
		break;
	case TEGRA_SOC_HWPM_IP_MGBE:
		ip_enum_idx = TEGRA_HWPM_IP_MGBE;
		break;
	case TEGRA_SOC_HWPM_IP_SCF:
		ip_enum_idx = TEGRA_HWPM_IP_SCF;
		break;
	case TEGRA_SOC_HWPM_IP_NVDEC:
		ip_enum_idx = TEGRA_HWPM_IP_NVDEC;
		break;
	case TEGRA_SOC_HWPM_IP_NVENC:
		ip_enum_idx = TEGRA_HWPM_IP_NVENC;
		break;
	case TEGRA_SOC_HWPM_IP_PCIE:
		ip_enum_idx = TEGRA_HWPM_IP_PCIE;
		break;
	case TEGRA_SOC_HWPM_IP_DISPLAY:
		ip_enum_idx = TEGRA_HWPM_IP_DISPLAY;
		break;
	case TEGRA_SOC_HWPM_IP_MSS_CHANNEL:
		ip_enum_idx = TEGRA_HWPM_IP_MSS_CHANNEL;
		break;
	case TEGRA_SOC_HWPM_IP_MSS_GPU_HUB:
		ip_enum_idx = TEGRA_HWPM_IP_MSS_GPU_HUB;
		break;
	case TEGRA_SOC_HWPM_IP_MSS_ISO_NISO_HUBS:
		ip_enum_idx = TEGRA_HWPM_IP_MSS_ISO_NISO_HUBS;
		break;
	case TEGRA_SOC_HWPM_IP_MSS_MCF:
		ip_enum_idx = TEGRA_HWPM_IP_MSS_MCF;
		break;
	case TEGRA_SOC_HWPM_IP_APE:
		ip_enum_idx = TEGRA_HWPM_IP_APE;
		break;
	case TEGRA_SOC_HWPM_IP_C2C:
		ip_enum_idx = TEGRA_HWPM_IP_C2C;
		break;
	default:
		tegra_hwpm_err(hwpm,
			"Queried enum tegra_soc_hwpm_ip %d is invalid",
			ip_enum);
		break;
	}

	return ip_enum_idx;
}

int tegra_hwpm_obtain_floorsweep_info(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_ip_floorsweep_info *fs_info)
{
	int ret = 0;
	u32 i = 0U;

	tegra_hwpm_fn(hwpm, " ");

	for (i = 0U; i < fs_info->num_queries; i++) {
		ret = hwpm->active_chip->get_fs_info(hwpm,
			tegra_hwpm_translate_soc_hwpm_ip(
				hwpm, fs_info->ip_fsinfo[i].ip),
			&fs_info->ip_fsinfo[i].ip_inst_mask,
			&fs_info->ip_fsinfo[i].status);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"Failed to get fs_info query %d", i);
		}

		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_floorsweep_info,
			"Query %d: tegra_soc_hwpm_ip %d: ip_status: %d"
			" inst_mask 0x%llx",
			i, fs_info->ip_fsinfo[i].ip,
			fs_info->ip_fsinfo[i].status,
			fs_info->ip_fsinfo[i].ip_inst_mask);
	}
	return ret;
}

u32 tegra_hwpm_translate_soc_hwpm_resource(struct tegra_soc_hwpm *hwpm,
	enum tegra_soc_hwpm_resource res_enum)
{
	u32 res_enum_idx = TEGRA_HWPM_IP_INACTIVE;

	switch (res_enum) {
	case TEGRA_SOC_HWPM_RESOURCE_VI:
		res_enum_idx = TEGRA_HWPM_RESOURCE_VI;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_ISP:
		res_enum_idx = TEGRA_HWPM_RESOURCE_ISP;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_VIC:
		res_enum_idx = TEGRA_HWPM_RESOURCE_VIC;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_OFA:
		res_enum_idx = TEGRA_HWPM_RESOURCE_OFA;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_PVA:
		res_enum_idx = TEGRA_HWPM_RESOURCE_PVA;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_NVDLA:
		res_enum_idx = TEGRA_HWPM_RESOURCE_NVDLA;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_MGBE:
		res_enum_idx = TEGRA_HWPM_RESOURCE_MGBE;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_SCF:
		res_enum_idx = TEGRA_HWPM_RESOURCE_SCF;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_NVDEC:
		res_enum_idx = TEGRA_HWPM_RESOURCE_NVDEC;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_NVENC:
		res_enum_idx = TEGRA_HWPM_RESOURCE_NVENC;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_PCIE:
		res_enum_idx = TEGRA_HWPM_RESOURCE_PCIE;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_DISPLAY:
		res_enum_idx = TEGRA_HWPM_RESOURCE_DISPLAY;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_MSS_CHANNEL:
		res_enum_idx = TEGRA_HWPM_RESOURCE_MSS_CHANNEL;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_MSS_GPU_HUB:
		res_enum_idx = TEGRA_HWPM_RESOURCE_MSS_GPU_HUB;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_MSS_ISO_NISO_HUBS:
		res_enum_idx = TEGRA_HWPM_RESOURCE_MSS_ISO_NISO_HUBS;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_MSS_MCF:
		res_enum_idx = TEGRA_HWPM_RESOURCE_MSS_MCF;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_PMA:
		res_enum_idx = TEGRA_HWPM_RESOURCE_PMA;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_CMD_SLICE_RTR:
		res_enum_idx = TEGRA_HWPM_RESOURCE_CMD_SLICE_RTR;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_APE:
		res_enum_idx = TEGRA_HWPM_RESOURCE_APE;
		break;
	case TEGRA_SOC_HWPM_RESOURCE_C2C:
		res_enum_idx = TEGRA_HWPM_RESOURCE_C2C;
		break;
	default:
		tegra_hwpm_err(hwpm,
			"Queried enum tegra_soc_hwpm_resource %d is invalid",
			res_enum);
		break;
	}

	return res_enum_idx;
}

int tegra_hwpm_obtain_resource_info(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_resource_info *rsrc_info)
{
	int ret = 0;
	u32 i = 0U;

	tegra_hwpm_fn(hwpm, " ");

	for (i = 0U; i < rsrc_info->num_queries; i++) {
		ret = hwpm->active_chip->get_resource_info(
			hwpm, tegra_hwpm_translate_soc_hwpm_resource(
				hwpm, rsrc_info->resource_info[i].resource),
			&rsrc_info->resource_info[i].status);
		if (ret < 0) {
			/* Print error for debug purpose. */
			tegra_hwpm_err(hwpm, "Failed to get rsrc_info");
		}

		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_resource_info,
			"Query %d: resource %d: status: %d",
			i, rsrc_info->resource_info[i].resource,
			rsrc_info->resource_info[i].status);
	}
	return ret;
}

static int tegra_hwpm_record_ip_ops(struct tegra_soc_hwpm *hwpm,
	struct tegra_soc_hwpm_ip_ops *soc_ip_ops)
{
	struct tegra_hwpm_ip_ops ip_ops;

	tegra_hwpm_fn(hwpm, " ");

	ip_ops.ip_dev = soc_ip_ops->ip_dev;
	ip_ops.hwpm_ip_pm = soc_ip_ops->hwpm_ip_pm;
	ip_ops.hwpm_ip_reg_op = soc_ip_ops->hwpm_ip_reg_op;

	if (soc_ip_ops->resource_enum >= TERGA_SOC_HWPM_NUM_RESOURCES) {
		tegra_hwpm_err(hwpm, "resource enum %d out of scope",
			soc_ip_ops->resource_enum);
		return -EINVAL;
	}

	return hwpm->active_chip->extract_ip_ops(hwpm,
		tegra_hwpm_translate_soc_hwpm_resource(hwpm,
			(enum tegra_soc_hwpm_resource)soc_ip_ops->resource_enum),
		soc_ip_ops->ip_base_address,
		&ip_ops, true);
}

int tegra_hwpm_complete_ip_register_impl(struct tegra_soc_hwpm *hwpm)
{
	int ret = 0;
	struct hwpm_ip_register_list *node = ip_register_list_head;

	tegra_hwpm_fn(hwpm, " ");

	while (node != NULL) {
		ret = tegra_hwpm_record_ip_ops(hwpm, &node->ip_ops);
		if (ret != 0) {
			tegra_hwpm_err(hwpm,
				"Resource enum %d extract IP ops failed",
				node->ip_ops.resource_enum);
			return ret;
		}
		node = node->next;
	}
	return ret;
}

static int tegra_hwpm_alloc_ip_register_list_node(
	struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops,
	struct hwpm_ip_register_list **node_ptr)
{
	struct hwpm_ip_register_list *new_node = NULL;

	new_node = tegra_hwpm_kzalloc(NULL,
		sizeof(struct hwpm_ip_register_list));
	if (new_node == NULL) {
		tegra_hwpm_err(NULL,
			"struct hwpm_ip_register_list node allocation failed");
		return -ENOMEM;
	}
	new_node->next = NULL;

	/* Copy given ip register details to node */
	memcpy(&new_node->ip_ops, hwpm_ip_ops,
		sizeof(struct tegra_soc_hwpm_ip_ops));
	(*node_ptr) = new_node;

	return 0;
}

static int tegra_hwpm_note_ip_register(
	struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops)
{
	int err = 0;
	struct hwpm_ip_register_list *node;

	if (ip_register_list_head == NULL) {
		err = tegra_hwpm_alloc_ip_register_list_node(hwpm_ip_ops,
			&ip_register_list_head);
		if (err != 0) {
			tegra_hwpm_err(NULL,
				"failed to note ip registration");
			return err;
		}
	} else {
		node = ip_register_list_head;
		while (node->next != NULL) {
			node = node->next;
		}

		err = tegra_hwpm_alloc_ip_register_list_node(hwpm_ip_ops,
			&node->next);
		if (err != 0) {
			tegra_hwpm_err(NULL,
				"failed to note ip registration");
			return err;
		}
	}

	return err;
}

void tegra_soc_hwpm_ip_register(struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops)
{
	struct tegra_soc_hwpm *hwpm = NULL;
	int ret = 0;

	if (hwpm_ip_ops == NULL) {
		tegra_hwpm_err(NULL, "IP details missing");
		return;
	}

	if (tegra_soc_hwpm_pdev == NULL) {
		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_ip_register,
			"Noting IP 0x%llx register request",
			hwpm_ip_ops->ip_base_address);
		ret = tegra_hwpm_note_ip_register(hwpm_ip_ops);
		if (ret != 0) {
			tegra_hwpm_err(NULL,
				"Couldn't save IP register details");
			return;
		}
	} else {
		if (hwpm_ip_ops->ip_dev == NULL) {
			tegra_hwpm_err(hwpm, "IP dev to register is NULL");
			return;
		}
		hwpm = platform_get_drvdata(tegra_soc_hwpm_pdev);

		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_ip_register,
		"Register IP 0x%llx", hwpm_ip_ops->ip_base_address);

		ret = tegra_hwpm_record_ip_ops(hwpm, hwpm_ip_ops);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "Failed to set IP ops for IP %d",
				hwpm_ip_ops->resource_enum);
		}
	}
}
EXPORT_SYMBOL(tegra_soc_hwpm_ip_register);

void tegra_soc_hwpm_ip_unregister(struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops)
{
	struct tegra_soc_hwpm *hwpm = NULL;
	int ret = 0;

	if (hwpm_ip_ops == NULL) {
		tegra_hwpm_err(NULL, "IP details missing");
		return;
	}

	if (tegra_soc_hwpm_pdev == NULL) {
		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_ip_register,
			"HWPM device not available");
	} else {
		if (hwpm_ip_ops->ip_dev == NULL) {
			tegra_hwpm_err(hwpm, "IP dev to unregister is NULL");
			return;
		}
		hwpm = platform_get_drvdata(tegra_soc_hwpm_pdev);

		tegra_hwpm_dbg(hwpm, hwpm_info | hwpm_dbg_ip_register,
		"Unregister IP 0x%llx", hwpm_ip_ops->ip_base_address);

		ret = tegra_hwpm_record_ip_ops(hwpm, hwpm_ip_ops);
		if (ret < 0) {
			tegra_hwpm_err(hwpm, "Failed to reset IP ops for IP %d",
				hwpm_ip_ops->resource_enum);
		}
	}
}
EXPORT_SYMBOL(tegra_soc_hwpm_ip_unregister);

void tegra_hwpm_release_ip_register_node(struct tegra_soc_hwpm *hwpm)
{
	struct hwpm_ip_register_list *node = ip_register_list_head;
	struct hwpm_ip_register_list *tmp_node = NULL;

	tegra_hwpm_fn(hwpm, " ");

	while (node != NULL) {
		tmp_node = node;
		node = tmp_node->next;
		tegra_hwpm_kfree(hwpm, tmp_node);
	}
}
