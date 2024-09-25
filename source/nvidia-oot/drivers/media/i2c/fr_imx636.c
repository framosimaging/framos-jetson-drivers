// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx636.c - Framos fr_imx636.c driver
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
#include <linux/version.h>
#include <media/fr_max96793.h>
#include <media/fr_max96792.h>

#include <media/tegra_v4l2_camera.h>
#include <media/tegracam_core.h>

#include "fr_imx636_mode_tbls.h"
#include "media/fr_sensor_common.h"
#include "media/fr_lifcl_var2fixed_1.h"

#define IMX636_CAMERA_CID_BASE				(V4L2_CTRL_CLASS_CAMERA | 0x6000)
#define IMX636_CAMERA_CID_CL_HBLANK			(IMX636_CAMERA_CID_BASE + 0)
#define IMX636_CAMERA_CID_CL_VBLANK			(IMX636_CAMERA_CID_BASE + 1)
#define IMX636_CAMERA_CID_BIAS_FO			(IMX636_CAMERA_CID_BASE + 2)
#define IMX636_CAMERA_CID_BIAS_HPF			(IMX636_CAMERA_CID_BASE + 3)
#define IMX636_CAMERA_CID_BIAS_DIFF_ON			(IMX636_CAMERA_CID_BASE + 4)
#define IMX636_CAMERA_CID_BIAS_DIFF			(IMX636_CAMERA_CID_BASE + 5)
#define IMX636_CAMERA_CID_BIAS_DIFF_OFF			(IMX636_CAMERA_CID_BASE + 6)
#define IMX636_CAMERA_CID_BIAS_REFR			(IMX636_CAMERA_CID_BASE + 7)
#define IMX636_CAMERA_CID_ANALOG_ROI			(IMX636_CAMERA_CID_BASE + 8)
#define IMX636_CAMERA_CID_DIGITAL_CROP			(IMX636_CAMERA_CID_BASE + 9)
#define IMX636_CAMERA_CID_VERIFY_FW_COMPATIBILITY	(IMX636_CAMERA_CID_BASE + 10)

#define IMX636_EEPROM_ADDRESS				0x56
#define IMX636_EEPROM_SIZE				256
#define IMX636_EEPROM_STR_SIZE				(IMX636_EEPROM_SIZE * 2)


LIST_HEAD(imx636_sensor_list);

static const struct of_device_id imx636_of_match[] = {
	{.compatible = "framos,imx636",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx636_of_match);

static const char * const verify_fw_menu[] = {
	[0] = "Check",
	[1] = "FW OK",
	[2] = "FW ERROR",
};

static int imx636_set_custom_ctrls(struct v4l2_ctrl *ctrl);
static const struct v4l2_ctrl_ops imx636_custom_ctrl_ops = {
	.s_ctrl = imx636_set_custom_ctrls,
};

static struct v4l2_ctrl_config imx636_custom_ctrl_list[] = {
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_CL_HBLANK,
		.name = "Horizontal Blank",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.min = 50,
		.max = 0x7FFFFFFF,
		.def = 80,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_CL_VBLANK,
		.name = "Vertical Blank",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.min = 72,
		.max = 0x7FFFFFFF,
		.def = 72,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_FO,
		.name = "bias_fo",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 48,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_HPF,
		.name = "bias_hpf",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 0,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_DIFF_ON,
		.name = "bias_diff_on",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 105,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_DIFF,
		.name = "bias_diff",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 85,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_DIFF_OFF,
		.name = "bias_diff_off",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 57,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_BIAS_REFR,
		.name = "bias_refr",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 255,
		.def = 52,
		.step = 1,
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_ANALOG_ROI,
		.name = "Analog Roi",
		.type = V4L2_CTRL_TYPE_U32,
		.min = 0,
		.max = UINT_MAX,
		.step = 1,
		.def = 0,
		.dims = { TD_ROI_X_SIZE + TD_ROI_Y_SIZE },
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_DIGITAL_CROP,
		.name = "Digital Crop",
		.type = V4L2_CTRL_TYPE_U32,
		.min = 0,
		.max = UINT_MAX,
		.step = 1,
		.def = 0,
		.dims = { 2 },
	},
	{
		.ops = &imx636_custom_ctrl_ops,
		.id = IMX636_CAMERA_CID_VERIFY_FW_COMPATIBILITY,
		.name = "Check Firmware Compatibility",
		.type = V4L2_CTRL_TYPE_MENU,
		.min = 0,
		.max = ARRAY_SIZE(verify_fw_menu) - 1,
		.def = 1,
		.qmenu = verify_fw_menu,
	},
};

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_EEPROM_DATA,
};

struct imx636 {
	struct camera_common_eeprom_data	eeprom;
	u8					eeprom_buf[IMX636_EEPROM_SIZE];
	struct i2c_client			*i2c_client;
	struct v4l2_subdev			*subdev;
	u64					frame_length;
	u64					min_frame_length;
	u64					black_level;
	u32					line_time;
	struct list_head			entry;
	struct mutex				pw_mutex;
	struct camera_common_data		*s_data;
	struct tegracam_device			*tc_dev;
	struct lifcl_var2fixed_1		*lifcl_var2fixed_1;
};

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 32,
	.val_bits = 32,
	.cache_type = REGCACHE_RBTREE,
	.use_single_read = true,
	.use_single_write = true,
};

static inline int imx636_read_reg(struct camera_common_data *s_data,
							u32 addr, u32 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val;

	return err;
}

static int imx636_write_reg(struct camera_common_data *s_data,
				u32 addr, u32 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "%s: i2c write failed, 0x%x = %x\n",
			__func__, addr, val);

	return err;
}

static int imx636_write_table(struct imx636 *priv, const imx636_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_32(s_data->regmap,
						table,
						NULL, 0,
						IMX636_TABLE_WAIT_MS,
						IMX636_TABLE_END);
}

static int imx636_set_bias(struct tegracam_device *tc_dev, int bias_id)
{
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct v4l2_ctrl *ctrl;
	u32 reg_address;
	u32 bias_val;
	u32 val;
	int err = 0;

	ctrl = fr_find_v4l2_ctrl(tc_dev, bias_id);
	val = *ctrl->p_new.p_u32;

	switch (bias_id) {
	case IMX636_CAMERA_CID_BIAS_FO:
		reg_address = BIAS_FO;
		break;
	case IMX636_CAMERA_CID_BIAS_HPF:
		reg_address = BIAS_HPF;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_ON:
		reg_address = BIAS_DIFF_ON;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF:
		reg_address = BIAS_DIFF;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_OFF:
		reg_address = BIAS_DIFF_OFF;
		break;
	case IMX636_CAMERA_CID_BIAS_REFR:
		reg_address = BIAS_REFR;
		break;
	default:
		dev_dbg(dev, "%s: unknown bias ID\n", __func__);
		return 1;
	}

	err = imx636_read_reg(s_data, reg_address, &bias_val);
	if (err) {
		dev_err(dev, "%s: failed to read reg\n", __func__);
		return err;
	}

	bias_val &= 0xFFFFFF00;
	bias_val |= val;

	bias_val |= 0x10000000;

	err = imx636_write_reg(s_data, reg_address, bias_val);
	if (err) {
		dev_err(dev, "%s: failed to read reg\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: reg[0x%x] val[0x%x]\n", __func__, reg_address, bias_val);

	return err;
}

static int imx636_set_analog_roi(struct tegracam_device *tc_dev, u32 *val)
{
	struct device *dev = tc_dev->dev;
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	int i, err = 0;

	imx636_reg table[TD_ROI_X_SIZE + TD_ROI_Y_SIZE + 3];
	u32 table_index = 0;

	dev_dbg(dev, "%s: set analog roi\n", __func__);

	table[table_index].addr = ROI_CTRL;
	table[table_index].val = ROI_CTRL_MASK | ROI_TD_EN;
	++table_index;
	for (i = 0; i < TD_ROI_X_SIZE; ++i) {
		table[table_index].addr = TD_ROI_X_START + i * sizeof(u32);
		table[table_index].val = val[i];
		++table_index;
	}
	for (i = 0; i < TD_ROI_Y_SIZE; ++i) {
		table[table_index].addr = TD_ROI_Y_START + i * sizeof(u32);
		table[table_index].val = val[TD_ROI_X_SIZE + i];
		if (i == TD_ROI_Y_SIZE - 1) {
			table[table_index].val &= 0x00FFFFFF;
			table[table_index].val |= 0x00FF0000;
		}
		++table_index;
	}
	table[table_index].addr = ROI_CTRL;
	table[table_index].val = ROI_CTRL_MASK | ROI_TD_EN | ROI_TD_SHADOW_TRIGGER;
	++table_index;
	table[table_index].addr = IMX636_TABLE_END;
	table[table_index].val = 0x00;

	err = imx636_write_table(priv, table);
	if (err) {
		dev_err(dev, "%s: unable to set analog roi\n", __func__);
		return err;
	}

	return err;
}

static int imx636_set_digital_crop(struct tegracam_device *tc_dev, u32 *val)
{
	struct device *dev = tc_dev->dev;
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	int err = 0;

	u16 start_x, start_y, end_x, end_y;
	imx636_reg table[4];
	u32 table_index = 0;
	bool reset = val[0] == 0 && val[1] == 0;

	dev_dbg(dev, "%s: set digital crop\n", __func__);

	if (!reset) {
		start_x = val[0] & CROP_X_MASK;
		start_y = (val[0] >> CROP_Y_SHIFT) & CROP_Y_MASK;
		end_x = val[1] & CROP_X_MASK;
		end_y = (val[1] >> CROP_Y_SHIFT) & CROP_Y_MASK;
		if (start_x > IMX636_WIDTH - 1
			|| start_y > IMX636_HEIGHT - 1
			|| end_x > IMX636_WIDTH - 1
			|| end_y > IMX636_HEIGHT - 1
			|| start_x % CROP_STEP_X != 0
			|| (end_x + 1) % CROP_STEP_X != 0
			|| start_x >= end_x
			|| start_y >= end_y) {
			dev_err(dev, "%s: invalid digital crop (%u,%u),(%u,%u)\n",
					 __func__, start_x, start_y, end_x, end_y);
			return -EINVAL;
		}
	}

	table[table_index].addr = CROP_START;
	table[table_index++].val = val[0];
	table[table_index].addr = CROP_END;
	table[table_index++].val = val[1];
	table[table_index].addr = CROP_CTRL;
	if (reset)
		table[table_index++].val = CROP_DISABLE;
	else
		table[table_index++].val = CROP_ENABLE;

	table[table_index].addr = IMX636_TABLE_END;
	table[table_index].val = 0x00;

	err = imx636_write_table(priv, table);
	if (err) {
		dev_err(dev, "%s: unable to set digital crop\n", __func__);
		return err;
	}

	return err;
}

static int imx636_fill_string_ctrl(struct tegracam_device *tc_dev,
							struct v4l2_ctrl *ctrl)
{
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	int i;

	if (ctrl->id == TEGRA_CAMERA_CID_EEPROM_DATA) {
		for (i = 0; i < IMX636_EEPROM_SIZE; i++)
			sprintf(&ctrl->p_new.p_char[i*2], "%02x",
			priv->eeprom_buf[i]);

	} else {
		return -EINVAL;
	}

	ctrl->p_cur.p_char = ctrl->p_new.p_char;
	return 0;
}

static int imx636_eeprom_device_release(struct imx636 *priv)
{
	if (priv->eeprom.i2c_client != NULL) {
		i2c_unregister_device(priv->eeprom.i2c_client);
		priv->eeprom.i2c_client = NULL;
	}

	return 0;
}

static int imx636_eeprom_device_init(struct imx636 *priv)
{
	char *dev_name = "eeprom_imx636";
	static struct regmap_config eeprom_regmap_config = {
		.reg_bits = 8,
		.val_bits = 8,
	};
	int err;

	priv->eeprom.adap = i2c_get_adapter(
			priv->i2c_client->adapter->nr);
	memset(&priv->eeprom.brd, 0, sizeof(priv->eeprom.brd));
	strncpy(priv->eeprom.brd.type, dev_name,
			sizeof(priv->eeprom.brd.type));
	priv->eeprom.brd.addr = IMX636_EEPROM_ADDRESS;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
	priv->eeprom.i2c_client = i2c_new_device(
			priv->eeprom.adap, &priv->eeprom.brd);
#else
	priv->eeprom.i2c_client = i2c_new_client_device(
			priv->eeprom.adap, &priv->eeprom.brd);
#endif
	priv->eeprom.regmap = devm_regmap_init_i2c(
		priv->eeprom.i2c_client, &eeprom_regmap_config);
	if (IS_ERR(priv->eeprom.regmap)) {
		err = PTR_ERR(priv->eeprom.regmap);
		imx636_eeprom_device_release(priv);
		return err;
	}

	return 0;
}

static int imx636_read_eeprom(struct imx636 *priv)
{
	int err;

	err = regmap_bulk_read(priv->eeprom.regmap, 0,
		&priv->eeprom_buf,
		IMX636_EEPROM_SIZE);
	if (err)
		return err;

	return 0;
}

static int imx636_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	return 0;
}

static int imx636_set_custom_ctrls(struct v4l2_ctrl *ctrl)
{
	struct tegracam_ctrl_handler *handler =
					container_of(ctrl->handler,
					struct tegracam_ctrl_handler, ctrl_handler);
	struct tegracam_device *tc_dev = handler->tc_dev;
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	int err = 0;

	if (priv->s_data->power->state == SWITCH_OFF)
		return 0;

	switch (ctrl->id) {
	case IMX636_CAMERA_CID_CL_HBLANK:
		priv->lifcl_var2fixed_1->params.hblank = *ctrl->p_new.p_u32;
		break;
	case IMX636_CAMERA_CID_CL_VBLANK:
		priv->lifcl_var2fixed_1->params.vblank = *ctrl->p_new.p_u32;
		break;
	case IMX636_CAMERA_CID_BIAS_FO:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_FO);
		break;
	case IMX636_CAMERA_CID_BIAS_HPF:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_HPF);
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_ON:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_ON);
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF);
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_OFF:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_OFF);
		break;
	case IMX636_CAMERA_CID_BIAS_REFR:
		err = imx636_set_bias(tc_dev, IMX636_CAMERA_CID_BIAS_REFR);
		break;
	case IMX636_CAMERA_CID_ANALOG_ROI:
		err = imx636_set_analog_roi(tc_dev, ctrl->p_new.p_u32);
		break;
	case IMX636_CAMERA_CID_DIGITAL_CROP:
		err = imx636_set_digital_crop(tc_dev, ctrl->p_new.p_u32);
		break;
	case IMX636_CAMERA_CID_VERIFY_FW_COMPATIBILITY:
		err = lifcl_var2fixed_1_verify_fw_compatibility(priv->lifcl_var2fixed_1,
								 &(ctrl->val));
		break;
	default:
		pr_err("%s: unknown ctrl id.\n", __func__);
		return -EINVAL;
	}

	return err;
}

static struct tegracam_ctrl_ops imx636_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.string_ctrl_size = {IMX636_EEPROM_STR_SIZE},
	.set_group_hold = imx636_set_group_hold,
	.fill_string_ctrl = imx636_fill_string_ctrl,
};

static int imx636_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx636 *priv = (struct imx636 *)s_data->priv;

	dev_dbg(dev, "%s: power on\n", __func__);

	mutex_lock(&priv->pw_mutex);

	lifcl_var2fixed_1_fw_reset(priv->lifcl_var2fixed_1, s_data, 1);

	if (pw->pwdn_gpio)
		fr_gpio_set(s_data, pw->pwdn_gpio, 1);

	if (pdata && pdata->power_on) {
		err = pdata->power_on(pw);
		if (err)
			dev_err(dev, "%s failed.\n", __func__);
		else
			pw->state = SWITCH_ON;
		mutex_unlock(&priv->pw_mutex);
		return err;
	}

	if (!pw->mclk) {
		dev_err(dev, "%s: mclk not available\n", __func__);
			goto imx636_mclk_fail;
	}

	usleep_range(1, 2);

	if (pw->reset_gpio)
		fr_gpio_set(s_data, pw->reset_gpio, 1);

	err = clk_prepare_enable(pw->mclk);
	if (err) {
		dev_err(dev, "%s: failed to enable mclk\n", __func__);
		return err;
	}

	pw->state = SWITCH_ON;

	/* Additional sleep required in the case of hardware power-on sequence */
	usleep_range(300000, 301000);

	mutex_unlock(&priv->pw_mutex);

	return 0;

imx636_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx636_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx636 *priv = (struct imx636 *)s_data->priv;
	int err = 0;

	dev_dbg(dev, "%s: power off\n", __func__);

	mutex_lock(&priv->pw_mutex);

	if (pdata && pdata->power_off) {
		err = pdata->power_off(pw);
		if (!err)
			goto power_off_done;
		else
			dev_err(dev, "%s failed.\n", __func__);
		mutex_unlock(&priv->pw_mutex);
		return err;
	}

	clk_disable_unprepare(pw->mclk);

	if (pw->reset_gpio)
		fr_gpio_set(s_data, pw->reset_gpio, 0);

power_off_done:
	pw->state = SWITCH_OFF;
	mutex_unlock(&priv->pw_mutex);

	return 0;
}

static int imx636_power_get(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	const char *mclk_name;
	const char *parentclk_name;
	struct clk *parent;
	int err = 0;

	if (!pdata) {
		dev_err(dev, "pdata missing\n");
		return -EFAULT;
	}

	mclk_name = pdata->mclk_name ?
			pdata->mclk_name : "extperiph1";
	pw->mclk = devm_clk_get(dev, mclk_name);
	if (IS_ERR(pw->mclk)) {
		dev_err(dev, "unable to get clock %s\n", mclk_name);
		return PTR_ERR(pw->mclk);
	}

	parentclk_name = pdata->parentclk_name;
	if (parentclk_name) {
		parent = devm_clk_get(dev, parentclk_name);
		if (IS_ERR(parent))
			dev_err(dev, "unable to get parent clcok %s",
				parentclk_name);
		else
			clk_set_parent(pw->mclk, parent);
	}

	pw->reset_gpio = pdata->reset_gpio;
	pw->pwdn_gpio = pdata->pwdn_gpio;

	if (pdata->use_cam_gpio) {
		err = cam_gpio_register(dev, pw->reset_gpio);
		if (err) {
			dev_err(dev, "%s ERR can't register cam gpio %u!\n",
				 __func__, pw->reset_gpio);
			goto done;
		}

		err = cam_gpio_register(dev, pw->pwdn_gpio);
		if (err) {
			dev_err(dev, "%s ERR can't register cam gpio %u!\n",
				 __func__, pw->pwdn_gpio);
			goto done;
		}
	}

done:
	pw->state = SWITCH_OFF;
	return err;
}

static int imx636_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = tc_dev->dev;

	if (unlikely(!pw))
		return -EFAULT;

	if (pdata && pdata->use_cam_gpio) {
		cam_gpio_deregister(dev, pw->reset_gpio);
		cam_gpio_deregister(dev, pw->pwdn_gpio);
	} else {
		if (pw->reset_gpio)
			gpio_free(pw->reset_gpio);
		if (pw->pwdn_gpio)
			gpio_free(pw->pwdn_gpio);
	}

	return 0;
}

static struct camera_common_pdata *imx636_parse_dt(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct device_node *sensor_node = dev->of_node;
	struct device_node *i2c_mux_ch_node;
	struct camera_common_pdata *board_priv_pdata;
	const struct of_device_id *match;
	struct camera_common_pdata *ret = NULL;
	int err;
	int gpio;

	if (!sensor_node)
		return NULL;

	match = of_match_device(imx636_of_match, dev);
	if (!match) {
		dev_err(dev, "Failed to find matching dt id\n");
		return NULL;
	}

	board_priv_pdata = devm_kzalloc(dev,
					sizeof(*board_priv_pdata), GFP_KERNEL);
	if (!board_priv_pdata)
		return NULL;

	err = camera_common_parse_clocks(dev,
					 board_priv_pdata);
	if (err) {
		dev_err(dev, "Failed to find clocks\n");
		goto error;
	}

	i2c_mux_ch_node = of_get_parent(sensor_node);
	if (i2c_mux_ch_node == NULL) {
		dev_err(dev, "i2c mux channel node not found in dt\n");
		goto error;
	}

	gpio = of_get_named_gpio(i2c_mux_ch_node, "reset-gpios", 0);
	if (gpio < 0) {
		if (gpio == -EPROBE_DEFER)
			ret = ERR_PTR(-EPROBE_DEFER);
		dev_err(dev, "reset-gpios not found %d\n", err);
		goto error;
	}
	board_priv_pdata->reset_gpio = (unsigned int)gpio;

	gpio = of_get_named_gpio(i2c_mux_ch_node, "pwdn-gpios", 0);
	if (gpio < 0) {
		if (gpio == -EPROBE_DEFER)
			ret = ERR_PTR(-EPROBE_DEFER);
		dev_err(dev, "pwdn gpio not found %d\n", err);
		goto error;
	}
	board_priv_pdata->pwdn_gpio = (unsigned int)gpio;

	fr_get_gpio_ctrl(board_priv_pdata);

	return board_priv_pdata;

error:
	devm_kfree(dev, board_priv_pdata);
	return ret;
}

static int imx636_update_bias_ctrl(struct tegracam_device *tc_dev, int ctrl_id)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	u32 reg_address;
	u32 bias_val;
	int err = 0;

	ctrl = fr_find_v4l2_ctrl(tc_dev, ctrl_id);

	switch (ctrl_id) {
	case IMX636_CAMERA_CID_BIAS_FO:
		reg_address = BIAS_FO;
		break;
	case IMX636_CAMERA_CID_BIAS_HPF:
		reg_address = BIAS_HPF;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_ON:
		reg_address = BIAS_DIFF_ON;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF:
		reg_address = BIAS_DIFF;
		break;
	case IMX636_CAMERA_CID_BIAS_DIFF_OFF:
		reg_address = BIAS_DIFF_OFF;
		break;
	case IMX636_CAMERA_CID_BIAS_REFR:
		reg_address = BIAS_REFR;
		break;
	default:
		dev_dbg(dev, "%s: unknown bias ID\n", __func__);
		return 1;
	}

	err = imx636_read_reg(s_data, reg_address, &bias_val);
	if (err) {
		dev_err(dev, "%s: failed to read reg\n", __func__);
		return err;
	}
	bias_val &= 0xFF;

	*ctrl->p_new.p_u32 = bias_val;
	*ctrl->p_cur.p_u32 = bias_val;
	ctrl->default_value = bias_val;

	dev_dbg(dev, "%s: reg[0x%x] val[0x%x]\n", __func__, reg_address, bias_val);

	return err;
}

static int imx636_update_bias_ctrls(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;

	err = imx636_power_on(s_data);
	if (err) {
		dev_err(dev, "Error %d during power on sensor\n", err);
		return err;
	}

	err = imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_FO);
	err |= imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_HPF);
	err |= imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_ON);
	err |= imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF);
	err |= imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_OFF);
	err |= imx636_update_bias_ctrl(tc_dev, IMX636_CAMERA_CID_BIAS_REFR);

	imx636_power_off(s_data);

	return err;
}

/**
 * According to the V4L2 documentation, driver should not return error when
 * invalid settings are detected.
 * It should apply the settings closest to the ones that the user has requested.
 */
static int imx636_check_unsupported_mode(struct camera_common_data *s_data,
						struct v4l2_mbus_framefmt *mf)
{
	return 0;
}

static int imx636_ctrl_set_override(struct tegracam_device *tc_dev, u32 id)
{
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl_handler *hdl = tc_dev->s_data->ctrl_handler;
	struct v4l2_ctrl *ctrl;
	int err = 0;

	ctrl = v4l2_ctrl_find(hdl, id);
	if (ctrl == NULL) {
		dev_err(dev, "%s: unable to find control\n", __func__);
		return -EINVAL;
	}

	err = imx636_set_custom_ctrls(ctrl);
	if (err) {
		dev_err(dev, "%s: unable to set control\n", __func__);
		return err;
	}

	return 0;
}

static int imx636_set_mode(struct tegracam_device *tc_dev)
{
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct sensor_image_properties *imgprops = NULL;
	int err;

	err = imx636_write_table(priv, mode_table[IMX636_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n", __func__);
		return err;
	}

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_CL_HBLANK);
	if (err)
		dev_err(dev, "%s: unable to override hblank\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_CL_VBLANK);
	if (err)
		dev_err(dev, "%s: unable to override vblank\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_FO);
	if (err)
		dev_err(dev, "%s: unable to override bias fo\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_HPF);
	if (err)
		dev_err(dev, "%s: unable to override bias hpf\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_ON);
	if (err)
		dev_err(dev, "%s: unable to override bias diff on\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF);
	if (err)
		dev_err(dev, "%s: unable to override bias diff\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_DIFF_OFF);
	if (err)
		dev_err(dev, "%s: unable to override bias diff off\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_BIAS_REFR);
	if (err)
		dev_err(dev, "%s: unable to override bias refr\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_ANALOG_ROI);
	if (err)
		dev_err(dev, "%s: unable to override analog roi\n", __func__);

	err |= imx636_ctrl_set_override(tc_dev, IMX636_CAMERA_CID_DIGITAL_CROP);
	if (err)
		dev_err(dev, "%s: unable to override digital crop\n", __func__);

	imgprops = &priv->s_data->sensor_props.sensor_modes[s_data->mode].image_properties;

	priv->lifcl_var2fixed_1->params.width = imgprops->width;
	priv->lifcl_var2fixed_1->params.height = imgprops->height;

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;
}

static int imx636_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	err = lifcl_var2fixed_1_start(priv->lifcl_var2fixed_1);

	usleep_range(1000, 2000);

	err |= imx636_write_table(priv, mode_table[IMX636_MODE_START_STREAM]);
	if (err)
		return err;

	dev_dbg(dev, "%s: start stream\n", __func__);

	return 0;
}

static int imx636_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	err = imx636_write_table(priv, mode_table[IMX636_MODE_STOP_STREAM]);
	err |= lifcl_var2fixed_1_stop(priv->lifcl_var2fixed_1);
	if (err)
		return err;

	dev_dbg(dev, "%s: stop stream\n", __func__);

	return 0;
}

static int imx636_ctrls_init(struct tegracam_device *tc_dev)
{
	struct imx636 *priv = (struct imx636 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = priv->s_data;
	struct v4l2_ctrl_config *ctrl_cfg;
	struct v4l2_ctrl *ctrl;
	struct tegracam_ctrl_handler *handler = s_data->tegracam_ctrl_hdl;
	int numctrls;
	int err, i;

	numctrls = ARRAY_SIZE(imx636_custom_ctrl_list);

	for (i = 0; i < numctrls; i++) {
		ctrl_cfg = &imx636_custom_ctrl_list[i];

		ctrl = v4l2_ctrl_new_custom(&handler->ctrl_handler, ctrl_cfg, NULL);
		if (ctrl == NULL) {
			dev_err(dev, "%s: Failed to create control %s\n",
						__func__, ctrl_cfg->name);
			continue;
		}

		if (ctrl_cfg->type == V4L2_CTRL_TYPE_STRING &&
				ctrl_cfg->flags & V4L2_CTRL_FLAG_READ_ONLY) {
			ctrl->p_new.p_char = devm_kzalloc(tc_dev->dev,
						ctrl_cfg->max + 1, GFP_KERNEL);
			if (!ctrl->p_new.p_char)
				return -ENOMEM;
		}
		handler->ctrls[handler->numctrls + i] = ctrl;
		dev_dbg(dev, "%s: Added custom control %s to handler index: %d\n",
			__func__, ctrl_cfg->name, handler->numctrls + i);
	}

	handler->numctrls = handler->numctrls + numctrls;

	err = handler->ctrl_handler.error;
	if (err) {
		dev_err(dev, "Error %d adding controls\n", err);
		goto error;
	}

	return 0;

error:
	v4l2_ctrl_handler_free(&handler->ctrl_handler);
	return err;
}

static struct camera_common_sensor_ops imx636_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx636_frmfmt),
	.frmfmt_table = imx636_frmfmt,
	.power_on = imx636_power_on,
	.power_off = imx636_power_off,
	.write_reg_32 = imx636_write_reg,
	.read_reg_32 = imx636_read_reg,
	.parse_dt = imx636_parse_dt,
	.power_get = imx636_power_get,
	.power_put = imx636_power_put,
	.set_mode = imx636_set_mode,
	.start_streaming = imx636_start_streaming,
	.stop_streaming = imx636_stop_streaming,
	.check_unsupported_mode = imx636_check_unsupported_mode,
	.init_private_controls = imx636_ctrls_init,
};

static int imx636_board_setup(struct imx636 *priv)
{
	struct camera_common_data *s_data = priv->s_data;
	struct device *dev = s_data->dev;
	struct device_node *node = dev->of_node;
	struct device_node *lifcl_var2fixed_1_node;
	struct i2c_client *lifcl_var2fixed_1_i2c = NULL;
	int err = 0;

	dev_dbg(dev, "%s++\n", __func__);

	err = imx636_eeprom_device_init(priv);
	if (err) {
		dev_err(dev,
			"Failed to allocate eeprom reg map: %d\n", err);
		return err;
	}

	lifcl_var2fixed_1_node = of_parse_phandle(node, "lifcl_var2fixed_1-device", 0);
	if (lifcl_var2fixed_1_node == NULL) {
		dev_err(dev,
			"missing %s handle\n",
				"lifcl_var2fixed_1-device");
		return err;
	}

	lifcl_var2fixed_1_i2c = of_find_i2c_device_by_node(lifcl_var2fixed_1_node);
	if (lifcl_var2fixed_1_i2c == NULL) {
		dev_err(dev, "missing lifcl_var2fixed_1 i2c dev handle\n");
		return err;
	}

	priv->lifcl_var2fixed_1->i2c_client = lifcl_var2fixed_1_i2c;
	priv->lifcl_var2fixed_1->i2c_client->dev = lifcl_var2fixed_1_i2c->dev;

	priv->lifcl_var2fixed_1->regmap = devm_regmap_init_i2c(priv->lifcl_var2fixed_1->i2c_client,
						&lifcl_var2fixed_1_regmap_config);
	if (IS_ERR_OR_NULL(priv->lifcl_var2fixed_1->regmap)) {
		dev_err(dev,
		"regmap init failed: %ld\n",
		PTR_ERR(priv->lifcl_var2fixed_1->regmap));
		return -ENODEV;
	}

	err = camera_common_mclk_enable(s_data);
	if (err) {
		dev_err(dev,
			"Error %d turning on mclk\n", err);
		return err;
	}

	err = imx636_power_on(s_data);
	if (err) {
		dev_err(dev,
			"Error %d during power on sensor\n", err);
		return err;
	}

	err = imx636_read_eeprom(priv);
	if (err) {
		dev_err(dev,
			"Error %d reading eeprom\n", err);
	}

	imx636_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx636_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s:++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx636_subdev_internal_ops = {
	.open = imx636_open,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx636_probe(struct i2c_client *client)
#else
static int imx636_probe(struct i2c_client *client, const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx636 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	struct sensor_image_properties *imgprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx636), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx636", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx636_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx636_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx636_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	priv->s_data->def_mode = IMX636_INIT_SETTINGS;
	priv->lifcl_var2fixed_1 = devm_kzalloc(dev,
			sizeof(struct lifcl_var2fixed_1), GFP_KERNEL);
	if (!priv->lifcl_var2fixed_1)
		return -ENOMEM;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;
	imgprops =
		&priv->s_data->sensor_props.sensor_modes[0].image_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	priv->lifcl_var2fixed_1->params.width = imgprops->width;
	priv->lifcl_var2fixed_1->params.height = imgprops->height;
	priv->lifcl_var2fixed_1->params.hblank = 100;
	priv->lifcl_var2fixed_1->params.vblank = 72;

	INIT_LIST_HEAD(&priv->entry);

	err = imx636_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx636_update_bias_ctrls(tc_dev);
	if (err) {
		dev_err(dev, "tegra bias ctrl update failed\n");
		return err;
	}

	list_add_tail(&priv->entry, &imx636_sensor_list);

	dev_info(dev, "Detected imx636 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx636_remove(struct i2c_client *client)
#else
static void imx636_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx636 *priv;

	if (s_data == NULL) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx636 *)s_data->priv;

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);
	imx636_eeprom_device_release(priv);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx636_id[] = {
	{ "imx636", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx636_id);

static struct i2c_driver imx636_i2c_driver = {
	.driver = {
		.name = "imx636",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx636_of_match),
	},
	.probe = imx636_probe,
	.remove = imx636_remove,
	.id_table = imx636_id,
};

module_i2c_driver(imx636_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX636");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
