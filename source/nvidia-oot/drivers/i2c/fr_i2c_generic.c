// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_i2c_generic.c - Framos i2c generic driver
 */

//#define DEBUG 1

#include <nvidia/conftest.h>

#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/i2c-dev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <media/camera_common.h>
#include "i2c/fr_i2c_generic.h"


enum generic_i2c_dev_type {
	broadcast_mux_ch,
};

struct broadcast_channel {
	struct i2c_client *device;
};

struct broadcast_channel *mux_broadcast;

/* Get i2c-mux broadcast channel */
struct i2c_client *get_broadcast_client(struct i2c_client *image_sensor)
{
	if (mux_broadcast == NULL) {
		dev_warn(&image_sensor->dev, "Couldn't get handle of mux i2c_client\n");
		return NULL;
	}

	return mux_broadcast->device;
}
EXPORT_SYMBOL(get_broadcast_client);

static const struct of_device_id generic_sd_of_match[] = {
	{ .compatible = "framos, mux_broadcast_ch"},
	{},
};

MODULE_DEVICE_TABLE(of, generic_sd_of_match);

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int generic_sd_probe(struct i2c_client *client)
#else
static int generic_sd_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
#endif
{
	const struct of_device_id *match;
	struct device_node *np = client->dev.of_node;
	const char *name;
	int err;

	dev_info(&client->dev, "%s++\n", __func__);

	match = of_match_device(generic_sd_of_match, &client->dev);
	if (!match) {
		dev_info(&client->dev, "Failed to find matching dt id\n");
		return -1;
	}

	err = of_property_read_string(np, "device", &name);
	if (err)
		dev_err(&client->dev, "device not in Device Tree\n");

	if (!strcmp(name, "i2c-mux")) {
		mux_broadcast = devm_kzalloc(&client->dev, sizeof(struct broadcast_channel),
								GFP_KERNEL);
		if (!mux_broadcast)
			return -ENOMEM;

		mux_broadcast->device = client;
		dev_info(&client->dev, "%s broadcast channel registered.\n", name);
	} else
		dev_info(&client->dev, "device name [%s] not compatible.\n", name);

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int generic_sd_remove(struct i2c_client *client)
#else
static void generic_sd_remove(struct i2c_client *client)
#endif
{
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id generic_sd_id[] = {
	{ "i2c-mux", broadcast_mux_ch },
	{},
};

MODULE_DEVICE_TABLE(i2c, generic_sd_id);

static struct i2c_driver generic_sd_i2c_driver = {
	.driver = {
		.name = "framos_i2c_generic_driver",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(generic_sd_of_match),
	},
	.probe = generic_sd_probe,
	.remove = generic_sd_remove,
	.id_table = generic_sd_id,
};

module_i2c_driver(generic_sd_i2c_driver);

MODULE_DESCRIPTION("framos i2c generic driver");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
