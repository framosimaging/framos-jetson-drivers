// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_lifcl_var2fixed_1.c - framos lifcl_var2fixed_1.c driver
 */

//#define DEBUG 1

#include <nvidia/conftest.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#include <media/tegra_v4l2_camera.h>
#include <media/tegracam_core.h>

#include "media/fr_sensor_common.h"
#include "media/fr_lifcl_var2fixed_1.h"
#include "i2c/fr_i2c_generic.h"


static const struct of_device_id lifcl_var2fixed_1_of_match[] = {
	{ .compatible = "framos,lifcl_var2fixed_1",},
	{ },
};
MODULE_DEVICE_TABLE(of, lifcl_var2fixed_1_of_match);

LIST_HEAD(lifcl_var2fixed_1_dev_list);

int lifcl_var2fixed_1_supported_fw_version[4] = {1, 0, 0, 0};
int lifcl_var2fixed_1_supported_latest_fw_version[4] = {1, 0, 0, 0};

/* Read register */
static int lifcl_var2fixed_1_read_reg(struct lifcl_var2fixed_1 *lifcl_var2fixed_1,
							u16 addr, u8 *val)
{
	int err = 0;

	err = regmap_read(lifcl_var2fixed_1->regmap, addr, (int *)val);
	if (err)
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
			"%s: i2c read failed, 0x%x = %x\n", __func__, addr, *val);

	return err;
}

/* Write register */
static int lifcl_var2fixed_1_write_reg(struct lifcl_var2fixed_1 *lifcl_var2fixed_1,
								u16 addr, u8 val)
{
	int err = 0;

	err = regmap_write(lifcl_var2fixed_1->regmap, addr, val);
	if (err)
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
			"%s: i2c write failed, 0x%x = %x\n", __func__, addr, val);

	return err;
}

/* Write multiple sequential registers */
static int lifcl_var2fixed_1_write_buffered_reg(struct lifcl_var2fixed_1 *lifcl_var2fixed_1,
				u16 addr_low, u8 number_of_registers, u32 val)
{
	int err = 0;
	int i;

	for (i = 0; i < number_of_registers; i++) {
		err = lifcl_var2fixed_1_write_reg(lifcl_var2fixed_1, addr_low + i, (u8)(val >> (i * 8)));
		if (err) {
			dev_err(&lifcl_var2fixed_1->i2c_client->dev, "%s: buffered register write error\n", __func__);
			return err;
		}
	}

	return err;
}

/* Read multiple sequential registers */
static int lifcl_var2fixed_1_read_buffered_reg(struct lifcl_var2fixed_1 *lifcl_var2fixed_1,
				u16 addr_low, u8 number_of_registers, u32 *val)
{
	int err = 0;
	int i;
	u8 reg;

	*val = 0;

	for (i = 0; i < number_of_registers; i++) {
		err = lifcl_var2fixed_1_read_reg(lifcl_var2fixed_1, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(&lifcl_var2fixed_1->i2c_client->dev, "%s: buffered register read error\n", __func__);
			return err;
		}
	}

	return err;
}

static int read_lifcl_var2fixed_1_fw_name(struct lifcl_var2fixed_1 *lifcl_var2fixed_1, char data[])
{
	int err = 0;
	int i, j;
	u8 val;

	for (i = 0, j = 0; i < LIFCL_VAR2FIXED_1_FW_NAME_REG_LENGTH; i++) {
		err = lifcl_var2fixed_1_read_reg(lifcl_var2fixed_1,
			(LIFCL_VAR2FIXED_1_FW_NAME_BASE + LIFCL_VAR2FIXED_1_FW_NAME_REG_LENGTH - i), &val);
		if (err) {
			dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"error reading lifcl_var2fixed_1 configruation");
			return err;
		}

		if (!val)
			continue;

		data[j] = val;
		j++;
	}

	return err;
}

static int read_lifcl_var2fixed_1_fw_version(struct lifcl_var2fixed_1 *lifcl_var2fixed_1, char data[])
{
	int err = 0;
	int i;
	u32 val;

	err = lifcl_var2fixed_1_read_buffered_reg(lifcl_var2fixed_1,
				LIFCL_VAR2FIXED_1_FW_VERSION_BASE, 4, &val);
	if (err) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev, "error reading lifcl_var2fixed_1 configruation");
		return err;
	}

	for (i = LIFCL_VAR2FIXED_1_FW_VERSION_REG_LENGTH - 1; i >= 0 ; i--)
		data[LIFCL_VAR2FIXED_1_FW_VERSION_REG_LENGTH - 1 - i] = (val >> i * 8) & 0xFF;

	return 0;
}

int lifcl_var2fixed_1_verify_fw_compatibility(struct lifcl_var2fixed_1 *lifcl_var2fixed_1, s32 *val)
{
	int err = 0;
	int fw_ver_err = 0;
	int i = 0;
	u8 cnt1 = 0;
	char fw_name[LIFCL_VAR2FIXED_1_FW_NAME_REG_LENGTH + 1] = "";
	char fw_version[LIFCL_VAR2FIXED_1_FW_VERSION_REG_LENGTH + 1] = "";

	err = read_lifcl_var2fixed_1_fw_name(lifcl_var2fixed_1, fw_name);
	if (err)
		goto fail;

	err = read_lifcl_var2fixed_1_fw_version(lifcl_var2fixed_1, fw_version);
	if (err)
		goto fail;

	dev_dbg(&lifcl_var2fixed_1->i2c_client->dev,
						"%s: CS fw ver.:%d.%d.%d.%d\n",
					__func__, fw_version[0], fw_version[1],
						fw_version[2], fw_version[3]);

	for (i = 0; i < 4; i++) {
		if (fw_version[i] == lifcl_var2fixed_1_supported_latest_fw_version[i])
			cnt1 += 1;
	}

	if (cnt1 == 4)
		fw_ver_err = 0;

	if (fw_ver_err) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
			"Image sensor driver don't support this firmware version. Most features will probably not work.\n");
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"RECONFIGURE FIRMWARE with the latest version - %d.%d.%d.%d!\n",
				lifcl_var2fixed_1_supported_latest_fw_version[0],
				lifcl_var2fixed_1_supported_latest_fw_version[1],
				lifcl_var2fixed_1_supported_latest_fw_version[2],
				lifcl_var2fixed_1_supported_latest_fw_version[3]);
	}

	return 0;

fail:
	*val = 2;
	dev_err(&lifcl_var2fixed_1->i2c_client->dev, "%s: failed to verify fw compatibility\n", __func__);
	return err;
}
EXPORT_SYMBOL(lifcl_var2fixed_1_verify_fw_compatibility);

static int write_lifcl_var2fixed_1_parameters(struct lifcl_var2fixed_1 *lifcl_var2fixed_1)
{
	struct lifcl_var2fixed_1_params *params = &lifcl_var2fixed_1->params;
	int err = 0;

	err = lifcl_var2fixed_1_write_buffered_reg(lifcl_var2fixed_1,
				LIFCL_VAR2FIXED_1_WIDTH_REG, 2, params->width);
	if (err)
		goto fail;

	err = lifcl_var2fixed_1_write_buffered_reg(lifcl_var2fixed_1,
				LIFCL_VAR2FIXED_1_HEIGHT_REG, 2, params->height);
	if (err)
		goto fail;

	err = lifcl_var2fixed_1_write_buffered_reg(lifcl_var2fixed_1,
				LIFCL_VAR2FIXED_1_VBLANK_REG, 4, params->vblank);
	if (err)
		goto fail;

	err = lifcl_var2fixed_1_write_buffered_reg(lifcl_var2fixed_1,
				LIFCL_VAR2FIXED_1_HBLANK_REG, 4, params->hblank);
	if (err)
		goto fail;

	dev_dbg(&lifcl_var2fixed_1->i2c_client->dev,
				"%s: Width: [%u]\n", __func__, params->width);
	dev_dbg(&lifcl_var2fixed_1->i2c_client->dev,
				"%s: Height: [%u]\n", __func__, params->height);
	dev_dbg(&lifcl_var2fixed_1->i2c_client->dev,
				"%s: Vblank: [%u]\n", __func__, params->vblank);
	dev_dbg(&lifcl_var2fixed_1->i2c_client->dev,
				"%s: Hblank: [%u]\n", __func__, params->hblank);

	return 0;
fail:
	dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"error writing lifcl_var2fixed_1 parameters");
	return err;
}

/* Out device from software reset */
int lifcl_var2fixed_1_start(struct lifcl_var2fixed_1 *lifcl_var2fixed_1)
{
	int err = 0;

	err = write_lifcl_var2fixed_1_parameters(lifcl_var2fixed_1);

	err = lifcl_var2fixed_1_write_reg(lifcl_var2fixed_1,
					LIFCL_VAR2FIXED_1_SW_RST_N_RW, 1);
	if (err)
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
			"%s: Starting lifcl_var2fixed_1 failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifcl_var2fixed_1_start);

/* Set device to software reset */
int lifcl_var2fixed_1_stop(struct lifcl_var2fixed_1 *lifcl_var2fixed_1)
{
	int err = 0;

	err = lifcl_var2fixed_1_write_reg(lifcl_var2fixed_1, LIFCL_VAR2FIXED_1_SW_RST_N_RW, 0);
	if (err)
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
			"%s: Stopping lifcl_var2fixed_1 failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifcl_var2fixed_1_stop);

int lifcl_var2fixed_1_fw_reset(struct lifcl_var2fixed_1 *lifcl_var2fixed_1, struct camera_common_data *s_data, int val)
{
	struct lifcl_var2fixed_1 *priv = dev_get_drvdata(&lifcl_var2fixed_1->i2c_client->dev);
	int err = 0;

	if (priv->fw_reset_gpio)
		fr_gpio_set(s_data, priv->fw_reset_gpio, val);
	else
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"%s: failed to use gpio cresetb\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifcl_var2fixed_1_fw_reset);

static int lifcl_var2fixed_1_parse_dt(struct lifcl_var2fixed_1 *lifcl_var2fixed_1)
{
	struct device_node *node = lifcl_var2fixed_1->i2c_client->dev.of_node;
	struct device_node *i2c_mux_ch_node = NULL;
	int err = 0;
	const char *name;
	const struct of_device_id *match;
	bool use_cam_gpio;

	if (!node)
		return -EINVAL;

	match = of_match_device(lifcl_var2fixed_1_of_match, &lifcl_var2fixed_1->i2c_client->dev);
	if (!match) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
					"Failed to find matching dt id\n");
		return -EFAULT;
	}

	err = of_property_read_string(node, "device", &name);
	if (err) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
					"device not in Device Tree\n");
		return err;
	}

	if (strcmp(name, "lifcl_var2fixed_1")) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"lifcl_var2fixed_1 not in Device Tree\n");
		return err;
	}

	i2c_mux_ch_node = of_get_parent(node);
	if (i2c_mux_ch_node == NULL) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"i2c mux channel node not found in Device Tree\n");
		return err;
	}
	lifcl_var2fixed_1->fw_reset_gpio = of_get_named_gpio(i2c_mux_ch_node, "cresetb", 0);
	if (lifcl_var2fixed_1->fw_reset_gpio < 0) {
		dev_err(&lifcl_var2fixed_1->i2c_client->dev,
					"creset_b gpio not found %d\n", err);
		return err;
	}

	node = of_find_node_by_name(NULL, "framos_platform_adapter");
	if (node) {
		use_cam_gpio = of_property_read_bool(node, "cam,use-cam-gpio");
		of_node_put(node);
	}

	if (use_cam_gpio) {
		err = cam_gpio_register(&lifcl_var2fixed_1->i2c_client->dev,
						lifcl_var2fixed_1->fw_reset_gpio);
		if (err) {
			dev_err(&lifcl_var2fixed_1->i2c_client->dev,
				"%s ERR can't register cam gpio fw_reset_gpio %u!\n",
					__func__, lifcl_var2fixed_1->fw_reset_gpio);
			return err;
		}
	}

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int lifcl_var2fixed_1_probe(struct i2c_client *client)
#else
static int lifcl_var2fixed_1_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct lifcl_var2fixed_1 *lifcl_var2fixed_1;

	int err = 0;

	dev_info(dev, "Probing lifcl_var2fixed_1 device\n");

	lifcl_var2fixed_1 = devm_kzalloc(&client->dev, sizeof(struct lifcl_var2fixed_1), GFP_KERNEL);
	if (!lifcl_var2fixed_1)
		return -ENOMEM;

	lifcl_var2fixed_1->i2c_client = client;
	lifcl_var2fixed_1->regmap = devm_regmap_init_i2c(lifcl_var2fixed_1->i2c_client,
		&lifcl_var2fixed_1_regmap_config);
	if (IS_ERR(lifcl_var2fixed_1->regmap)) {
		dev_err(dev, "regmap init failed: %ld\n", PTR_ERR(lifcl_var2fixed_1->regmap));
		return -ENODEV;
	}

	err = lifcl_var2fixed_1_parse_dt(lifcl_var2fixed_1);
	if (err) {
		dev_err(&client->dev, "unable to parse dt\n");
		return -EFAULT;
	}

	dev_set_drvdata(dev, lifcl_var2fixed_1);

	INIT_LIST_HEAD(&lifcl_var2fixed_1->entry);
	list_add_tail(&lifcl_var2fixed_1->entry, &lifcl_var2fixed_1_dev_list);

	dev_info(dev, "Detected lifcl_var2fixed_1 device\n");

	return err;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int lifcl_var2fixed_1_remove(struct i2c_client *client)
#else
static void lifcl_var2fixed_1_remove(struct i2c_client *client)
#endif
{
	struct lifcl_var2fixed_1 *priv;

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		dev_dbg(&client->dev, "Removed lifcl_var2fixed_1 module device\n");
		devm_kfree(&client->dev, priv);
		client = NULL;
	}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id lifcl_var2fixed_1_id[] = {
	{ "lifcl_var2fixed_1", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, lifcl_var2fixed_1_id);

static struct i2c_driver lifcl_var2fixed_1_i2c_driver = {
	.driver = {
	.name = "lifcl_var2fixed_1",
	.owner = THIS_MODULE,
	.of_match_table = of_match_ptr(lifcl_var2fixed_1_of_match),
	},
	.probe = lifcl_var2fixed_1_probe,
	.remove = lifcl_var2fixed_1_remove,
	.id_table = lifcl_var2fixed_1_id,
};

module_i2c_driver(lifcl_var2fixed_1_i2c_driver);

MODULE_DESCRIPTION("Framos Image Sensor lifcl_var2fixed_1 logic");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
