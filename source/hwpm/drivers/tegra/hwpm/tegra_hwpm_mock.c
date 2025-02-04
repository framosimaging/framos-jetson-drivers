/*
 * Copyright (c) 2023, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/export.h>
#include <linux/module.h>
#include <uapi/linux/tegra-soc-hwpm-uapi.h>
#include <os/linux/driver.h>

void tegra_soc_hwpm_ip_register(struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops)
{
	/* Dummy function */
}
EXPORT_SYMBOL(tegra_soc_hwpm_ip_register);

void tegra_soc_hwpm_ip_unregister(struct tegra_soc_hwpm_ip_ops *hwpm_ip_ops)
{
	/* Dummy function */
}
EXPORT_SYMBOL(tegra_soc_hwpm_ip_unregister);


/* Dummy implementation for module */
static int __init tegra_hwpm_init(void)
{
        return 0;
}

static void __exit tegra_hwpm_exit(void)
{
}

#if defined(CONFIG_TEGRA_HWPM_OOT)
module_init(tegra_hwpm_init);
#else
postcore_initcall(tegra_hwpm_init);
#endif
module_exit(tegra_hwpm_exit);

MODULE_ALIAS(TEGRA_SOC_HWPM_MODULE_NAME);
MODULE_DESCRIPTION("Tegra SOC HWPM MOCK Driver");
MODULE_LICENSE("GPL v2");

