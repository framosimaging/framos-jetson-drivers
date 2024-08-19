/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2023, NVIDIA CORPORATION.  All rights reserved.
 */

#ifndef __LINUX_TEGRA_PROD_UPSTREAM_DUMMY_H__
#define __LINUX_TEGRA_PROD_UPSTREAM_DUMMY_H__

#include <linux/types.h>

/**
 * Prod configuration of device provided from client.
 * This structure have the  details about each of prod settings.
 */
struct tegra_prod_dev_reg_field {
	const char *field_name;
	uint32_t reg_index;
	uint32_t reg_offset;
	uint32_t field_start;
	uint32_t field_len;
};

/**
 * Prod setting from controller where it passes the total number of registers
 * having prod, and their register field names.
 */
struct tegra_prod_dev_info {
	uint32_t num_total_dev_reg;
	uint32_t num_dev_reg_field;
	const struct tegra_prod_dev_reg_field *dev_reg_field;
};

/**
 * Prod register information as register index, offset from base,
 * register mask for the given prod and prod value for given
 * register field.
 */
struct tegra_prod_reg_info {
	uint32_t reg_index;
	uint32_t reg_offset;
	uint32_t reg_mask;
	uint32_t reg_value;
};

/**
 * Contains prod setting of all registers.
 */
struct tegra_prod_cfg_info {
	const char *prod_name;
	uint32_t num_reg_info;
	struct tegra_prod_reg_info *reg_info;
};

/**
 * Contains prod setting list of all prod values.
 */
struct tegra_prod_cfg_list {
	uint32_t num_list;
	struct tegra_prod_cfg_info *prod_cfg;
};

static inline struct tegra_prod_cfg_info *tegra_prod_get_by_name_from_list(struct device *dev,
				const struct tegra_prod_cfg_list *prod_list,
				const char *prod_name)
{
	return ERR_PTR(-ENODEV);
}

static inline struct tegra_prod_cfg_list *devm_tegra_prod_get_list_from_node(struct device *dev,
				const struct device_node *np,
				const struct tegra_prod_dev_info *prod_dev)
{
	return ERR_PTR(-ENODEV);
}

static inline struct tegra_prod_cfg_list *devm_tegra_prod_get_list(struct device *dev,
				const struct tegra_prod_dev_info *prod_dev)
{
	return ERR_PTR(-ENODEV);
}
#endif /* __LINUX_TEGRA_PROD_UPSTREAM_DUMMY_H__ */
