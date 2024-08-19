/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2023, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
 */

#ifndef TEGRA_PROD_LEGACY_DUMMY_H__
#define TEGRA_PROD_LEGACY_DUMMY_H__

/**
 * Implement the tegra prod APIs as static inline with success to avoid
 * the failure in client side due to prod.
 */

struct tegra_prod;

/**
 * tegra_prod_set_list(): Set all prods configurations
 * @base: List of IO mapped registers.
 * @tegra_prod: tegra_prod handle which is allocated by devm_tegra_prod_get()
 *		or tegra_prod_get_from_node();
 *
 * Configure all the prod configuration listed on prod-setting nodes.
 *
 * Returns 0 on success otherwise negative error number for failed case.
 */
static inline int tegra_prod_set_list(void __iomem **base, struct tegra_prod *tegra_prod)
{
	return 0;
}

/**
 * tegra_prod_set_boot_init(): Set all prods configurations which has boot init
 *			       flag on the prod setting nodes.
 * @base: List of IO mapped registers.
 * @tegra_prod: tegra_prod handle which is allocated by devm_tegra_prod_get()
 *		or tegra_prod_get_from_node();
 *
 * Configure all the prod configuration listed on prod-setting nodes.
 *
 * Returns 0 on success otherwise negative error number for failed case.
 */
static inline int tegra_prod_set_boot_init(void __iomem **base,
			     struct tegra_prod *tegra_prod)
{
	return 0;
}

/**
 * tegra_prod_set_by_name(): Set prod configuration with specific prod name.
 *			     This is used for conditional prod configurations.
 * @base: List of IO mapped registers.
 * @name: Name of conditional prod which need to be configure.
 * @tegra_prod: tegra_prod handle which is allocated by devm_tegra_prod_get()
 *		or tegra_prod_get_from_node();
 *
 * Configure prod configuration with specific prod name for conditional
 * prod configurations.
 *
 * Returns 0 on success otherwise negative error number for failed case.
 */
static inline int tegra_prod_set_by_name(void __iomem **base, const char *name,
			   struct tegra_prod *tegra_prod)
{
	return 0;
}

/**
 * tegra_prod_set_by_name_partially - Set the prod setting from list partially
 *                                    under given prod name. The matching is done
 *                                    qith index, offset and mask.
 * @base:               base address of the register.
 * @name:               the name of tegra prod need to set.
 * @tegra_prod: the list of tegra prods.
 * @index:              Index of base address.
 * @offset:             Offset of the register.
 * @mask:               Mask field on given register.
 *
 * Find the tegra prod in the list according to the name. Then set
 * that tegra prod which has matching of index, offset and mask.
 *
 * Returns 0 on success.
 */
static inline int tegra_prod_set_by_name_partially(void __iomem **base, const char *name,
				     struct tegra_prod *tegra_prod, u32 index,
				     u32 offset, u32 mask)
{
	return 0;
}

/**
 * tegra_prod_by_name_supported - Tell whether tegra prod will be supported by
 *				  given name or not.
 * @tegra_prod: the list of tegra prods.
 * @name:       the name of tegra prod need to set.
 *
 * Find the tegra prod in the list according to the name. If it exist then
 * return true else false.
 */
static inline bool tegra_prod_by_name_supported(struct tegra_prod *tegra_prod,
                                  const char *name)
{
	return true;
}

/**
 * devm_tegra_prod_get(): Get the prod handle from the device.
 * @dev: Device handle on which prod setting nodes are available.
 *
 * Parse the prod-setting node of the dev->of_node and keep all prod
 * setting data in prod handle.
 * This handle is used for setting prod configurations.
 *
 * Returns valid prod_list handle on success or pointer to the error
 * when it failed.
 */
static inline struct tegra_prod *devm_tegra_prod_get(struct device *dev)
{
	return (struct tegra_prod *)dev;
}

/**
 * devm_tegra_prod_get_from_node(): Get the prod handle from the node.
 * @dev: Device handle.
 * @np: Node pointer on which prod setting nodes are available.
 *
 * Parse the prod-setting node of the node pointer "np" and keep all prod
 * setting data in prod handle.
 * This handle is used for setting prod configurations.
 *
 * Returns valid prod_list handle on success or pointer to the error
 * when it failed.
 * The allocated resource is released by driver core framework when device
 * is unbinded and so no need to call any release APIs for the tegra_prod
 * handle.
 */
static inline struct tegra_prod *devm_tegra_prod_get_from_node(struct device *dev,
						 struct device_node *np)
{
	return (struct tegra_prod *)dev;
}

#endif /* TEGRA_PROD_LEGACY_DUMMY_H__ */
