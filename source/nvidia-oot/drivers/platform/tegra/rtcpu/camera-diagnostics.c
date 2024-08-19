// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <soc/tegra/ivc_ext.h>
#include <linux/tegra-ivc-bus.h>
#include <soc/tegra/ivc-priv.h>

static int tegra_camera_diagnostics_probe(struct tegra_ivc_channel *ch)
{
	(void)ch;
	return 0;
}

static void tegra_camera_diagnostics_remove(struct tegra_ivc_channel *ch)
{
	(void)ch;
}

static const struct tegra_ivc_channel_ops
tegra_camera_diagnostics_channel_ops = {
	.probe	= tegra_camera_diagnostics_probe,
	.remove	= tegra_camera_diagnostics_remove,
};

static const struct of_device_id camera_diagnostics_of_match[] = {
	{ .compatible = "nvidia,tegra186-camera-diagnostics", },
	{ },
};

MODULE_DEVICE_TABLE(of, camera_diagnostics_of_match);

static struct tegra_ivc_driver camera_diagnostics_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.bus	= &tegra_ivc_bus_type,
		.name	= "tegra-camera-diagnostics",
		.of_match_table = camera_diagnostics_of_match,
	},
	.dev_type	= &tegra_ivc_channel_type,
	.ops.channel	= &tegra_camera_diagnostics_channel_ops,
};

tegra_ivc_subsys_driver_default(camera_diagnostics_driver);
MODULE_AUTHOR("Pekka Pessi <ppessi@nvidia.com>");
MODULE_DESCRIPTION("Dummy device driver for Camera Diagnostics IVC Channel");
MODULE_LICENSE("GPL v2");
