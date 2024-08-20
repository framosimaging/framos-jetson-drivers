// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx530.c - Framos fr_imx530.c driver
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

#include "fr_imx530_mode_tbls.h"
#include "media/fr_sensor_common.h"
#include "media/fr_lifmd_lvds2mipi_1.h"

#define IMX530_K_FACTOR					1000LL
#define IMX530_M_FACTOR					1000000LL
#define IMX530_G_FACTOR					1000000000LL
#define IMX530_T_FACTOR					1000000000000LL

#define IMX530_MAX_GAIN_DEC				480
#define IMX530_MAX_GAIN_DB				48

#define IMX530_MAX_BLACK_LEVEL_10BPP			1023
#define IMX530_MAX_BLACK_LEVEL_12BPP			4095
#define IMX530_DEFAULT_BLACK_LEVEL_10BPP		60
#define IMX530_DEFAULT_BLACK_LEVEL_12BPP		240

#define IMX530_DEFAULT_LINE_TIME			14380 /* [ns] */

#define IMX530_INTEGRATION_OFFSET			0
#define IMX530_MIN_INTEGRATION_LINES			1

#define IMX530_INCK					74250000LL

#define IMX530_LVDS_NUM_CHANNELS			8
#define IMX530_READOUT_LINE_SKIP			68
#define IMX530_READOUT_LINE_SKIP_2			36

#define IMX530_CAMERA_CID_BASE				(V4L2_CTRL_CLASS_CAMERA | 0x6000)
#define IMX530_CAMERA_CID_VERIFY_FW_COMPATIBILITY	(IMX530_CAMERA_CID_BASE + 0)
#define IMX530_CAMERA_CID_TG_SET_OPERATION_MODE		(IMX530_CAMERA_CID_BASE + 1)
#define IMX530_CAMERA_CID_TG_EXPAND_TRIGGER_EXPOSURE	(IMX530_CAMERA_CID_BASE + 2)
#define IMX530_CAMERA_CID_TG_DELAY_FRAME		(IMX530_CAMERA_CID_BASE + 3)

/* Declaration */
static struct mutex serdes_lock__;

static const struct of_device_id imx530_of_match[] = {
	{
		.compatible = "framos,imx530",
	},
	{},
};
MODULE_DEVICE_TABLE(of, imx530_of_match);

static int imx530_set_custom_ctrls(struct v4l2_ctrl *ctrl);
static const struct v4l2_ctrl_ops imx530_custom_ctrl_ops = {
	.s_ctrl = imx530_set_custom_ctrls,
};

static const char *const imx530_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Sequence Pattern 1",
	[2] = "Sequence Pattern 2",
	[3] = "Gradation Pattern",
};

static const char *const verify_fw_menu[] = {
	[0] = "Check",
	[1] = "FW OK",
	[2] = "FW ERROR",
};

static const char *const timing_generator_mode_menu[] = {
	[0] = "Disabled",
	[1] = "Master Mode",
	[2] = "Slave Mode",
	[3] = "Trigger Mode",
};

static struct v4l2_ctrl_config imx530_custom_ctrl_list[] = {
	{
	.ops = &imx530_custom_ctrl_ops,
	.id = IMX530_CAMERA_CID_VERIFY_FW_COMPATIBILITY,
	.name = "Check Firmware Compatibility",
	.type = V4L2_CTRL_TYPE_MENU,
	.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
	.min = 0,
	.max = ARRAY_SIZE(verify_fw_menu) - 1,
	.def = 0,
	.qmenu = verify_fw_menu,
	},
	{
	.ops = &imx530_custom_ctrl_ops,
	.id = IMX530_CAMERA_CID_TG_SET_OPERATION_MODE,
	.name = "Timing Generator Mode",
	.type = V4L2_CTRL_TYPE_MENU,
	.min = 0,
	.max = ARRAY_SIZE(timing_generator_mode_menu) - 1,
	.def = 0,
	.qmenu = timing_generator_mode_menu,
	},
	{
	.ops = &imx530_custom_ctrl_ops,
	.id = IMX530_CAMERA_CID_TG_EXPAND_TRIGGER_EXPOSURE,
	.name = "Expanded exposure",
	.type = V4L2_CTRL_TYPE_INTEGER64,
	.flags = V4L2_CTRL_FLAG_SLIDER,
	.min = 0,
	.max = UINT_MAX,
	.def = 0,
	.step = 1,
	},
	{
	.ops = &imx530_custom_ctrl_ops,
	.id = IMX530_CAMERA_CID_TG_DELAY_FRAME,
	.name = "Delay frame",
	.type = V4L2_CTRL_TYPE_INTEGER64,
	.flags = V4L2_CTRL_FLAG_SLIDER,
	.min = 0,
	.max = UINT_MAX,
	.def = 0,
	.step = 1,
	},
};

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_GAIN,
	TEGRA_CAMERA_CID_EXPOSURE,
	TEGRA_CAMERA_CID_FRAME_RATE,
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_OPERATION_MODE,
	TEGRA_CAMERA_CID_BLACK_LEVEL,
	TEGRA_CAMERA_CID_SHUTTER_MODE,
	TEGRA_CAMERA_CID_TEST_PATTERN,
};

struct imx530 {
	struct i2c_client *i2c_client;
	struct v4l2_subdev *subdev;
	u64 frame_length;
	u64 min_frame_length;
	u64 current_pixel_format;
	u32 line_time;
	struct mutex pw_mutex;
	struct camera_common_data *s_data;
	struct tegracam_device *tc_dev;
	struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1;
};

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
	.use_single_read = true,
	.use_single_write = true,
};

static inline int imx530_read_reg(struct camera_common_data *s_data,
					u16 addr,
					u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx530_write_reg(struct camera_common_data *s_data, u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "%s: i2c write failed, 0x%x = %x\n", __func__,
			addr, val);

	return err;
}

static int imx530_read_buffered_reg(struct camera_common_data *s_data,
					u16 addr_low,
					u8 number_of_registers,
					u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;

	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx530_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
				__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx530_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx530_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
				__func__);
			return err;
		}
	}

	return err;
}

static int imx530_write_buffered_reg(struct camera_common_data *s_data,
					u16 addr_low,
					u8 number_of_registers,
					u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx530_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx530_write_reg(s_data, addr_low + i,
					(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx530_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx530_write_table(struct imx530 *priv, const imx530_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap, table, NULL, 0,
					 IMX530_TABLE_WAIT_MS,
					 IMX530_TABLE_END);
}

static int imx530_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx530_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx530_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val,
				u64 min_val, u64 max_val)
{
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct v4l2_ctrl *ctrl;

	ctrl = fr_find_v4l2_ctrl(tc_dev, ctrl_id);
	if (ctrl) {
		switch (ctrl->id) {
		case TEGRA_CAMERA_CID_BLACK_LEVEL:
			*ctrl->p_new.p_s64 = current_val;
			*ctrl->p_cur.p_s64 = current_val;
			ctrl->minimum = min_val;
			ctrl->maximum = max_val;
			ctrl->default_value = default_val;
			priv->s_data->blklvl_max_range = max_val;
			break;
		case TEGRA_CAMERA_CID_TEST_PATTERN:
			ctrl->qmenu = imx530_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		case TEGRA_CAMERA_CID_SHUTTER_MODE:
			*ctrl->p_new.p_s64 = current_val;
			*ctrl->p_cur.p_s64 = current_val;
			break;
		}
	}
	return 0;
}

static int imx530_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx530_write_buffered_reg(s_data, BLKLEVEL_LOW, 2, val);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx530_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
			&s_data->sensor_props.sensor_modes[s_data->mode];
	int err;
	u32 gain;

	gain = val * IMX530_MAX_GAIN_DEC /
		(IMX530_MAX_GAIN_DB * mode->control_properties.gain_factor);

	err = imx530_write_buffered_reg(s_data, GAIN_LOW, 2, gain);
	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

dev_dbg(dev, "%s:		gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx530_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err;
	u32 integration_time_line;
	u32 integration_offset = IMX530_INTEGRATION_OFFSET;
	u32 reg_shs, min_reg_shs;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line =
		((val - integration_offset) * IMX530_K_FACTOR) / priv->line_time;

	reg_shs = priv->frame_length - integration_time_line;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304)
			min_reg_shs = 0x18;
		else
			min_reg_shs = 0x0C;
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304)
			min_reg_shs = 0x10;
		else
			min_reg_shs = 0x08;
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	if (reg_shs < min_reg_shs)
		reg_shs = min_reg_shs;
	else if (reg_shs > (priv->frame_length - IMX530_MIN_INTEGRATION_LINES))
		reg_shs = priv->frame_length - IMX530_MIN_INTEGRATION_LINES;

	err = lifmd_lvds2mipi_1_tg_set_trigger_exposure(priv->lifmd_lvds2mipi_1,
							(u32)val,
							s_data);
	err = imx530_write_buffered_reg(s_data, SHS_LOW, 3, reg_shs);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_EXPOSURE);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	dev_dbg(dev,
		"%s: set integration time: %lld [us], coarse1:%d [line], shs: %d [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, reg_shs, priv->frame_length);

	return err;
}

static int imx530_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params =
					&priv->lifmd_lvds2mipi_1->tg_params;
	struct device *dev = tc_dev->dev;
	int err;
	u32 min_reg_shs;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;
	const struct sensor_mode_properties *mode =
			&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length =
	(((u64)mode->control_properties.framerate_factor * IMX530_G_FACTOR) /
						(val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	priv->frame_length = frame_length;
	tg_params->frame_length = frame_length;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304)
			min_reg_shs = 0x18;
		else
			min_reg_shs = 0x0C;
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304)
			min_reg_shs = 0x10;
		else
			min_reg_shs = 0x08;
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	exposure_min_range =
		IMX530_MIN_INTEGRATION_LINES * priv->line_time / IMX530_K_FACTOR;
	exposure_min_range += IMX530_INTEGRATION_OFFSET;
	exposure_max_range = (priv->frame_length - min_reg_shs) *
					priv->line_time / IMX530_K_FACTOR;
	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				 exposure_min_range, exposure_max_range);

	err = lifmd_lvds2mipi_1_tg_set_frame_width(priv->lifmd_lvds2mipi_1,
						IMX530_G_FACTOR / (val / 1000));
	err =
		imx530_write_buffered_reg(s_data, VMAX_LOW, 3, priv->frame_length);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: val: %lld, frame_length set: %llu\n", __func__, val,
		priv->frame_length);

	return 0;
}

static int imx530_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err = imx530_write_reg(s_data, 0x3550, 0x07);
		if (err)
			goto fail;

		err = imx530_write_reg(s_data, 0x3551, (u8)(val));
		if (err)
			goto fail;
	} else {
		err = imx530_write_reg(s_data, 0x3550, 0x06);
		if (err)
			goto fail;
	}

	return 0;

fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx530_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	u64 max_framerate;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	switch (s_data->mode) {
	case IMX530_MODE_5328x4608:
		priv->min_frame_length =
			IMX530_DEFAULT_HEIGHT + IMX530_MIN_FRAME_DELTA;
		break;
	case IMX530_MODE_ROI_3840x2160:
		if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
			priv->min_frame_length = IMX530_ROI_MODE_HEIGHT +
						 IMX530_MIN_FRAME_DELTA_10BIT;
		} else {
			priv->min_frame_length =
				IMX530_ROI_MODE_HEIGHT + IMX530_MIN_FRAME_DELTA;
		}
		break;
	case IMX530_MODE_ROI_4512x4512:
		if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
			priv->min_frame_length = IMX530_ROI_MODE_2_HEIGHT +
						 IMX530_MIN_FRAME_DELTA_10BIT;
		} else {
			priv->min_frame_length =
				IMX530_ROI_MODE_2_HEIGHT + IMX530_MIN_FRAME_DELTA;
		}
		break;
	case IMX530_MODE_ROI_5328x3040:
		priv->min_frame_length =
			IMX530_ROI_MODE_3_HEIGHT + IMX530_MIN_FRAME_DELTA;
		break;
	case IMX530_MODE_ROI_4064x3008:
		if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
			priv->min_frame_length = IMX530_ROI_MODE_4_HEIGHT +
						 IMX530_MIN_FRAME_DELTA_10BIT;
		} else {
			priv->min_frame_length =
				IMX530_ROI_MODE_4_HEIGHT + IMX530_MIN_FRAME_DELTA;
		}
		break;
	case IMX530_MODE_BINNING_2656x2304:
		priv->min_frame_length = 2432;
		break;
	}

	max_framerate = (IMX530_G_FACTOR * IMX530_M_FACTOR) /
			(priv->min_frame_length * priv->line_time);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				 ctrlprops->min_framerate, max_framerate);

	return 0;
}

static int imx530_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;

	priv->lifmd_lvds2mipi_1->tg_params.is_operation_mode = val;

	return 0;
}

static int imx530_set_shutter_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 trigen = 0;
	u8 vint_en = 2;

	switch (val) {
	case NORMAL_EXPO:
		dev_dbg(dev, "%s: Sensor is in Normal Exposure Mode\n",
			__func__);
		break;

	case SEQ_TRIGGER:
		if ((fr_get_v4l2_ctrl_value(tc_dev,
						TEGRA_CAMERA_CID_OPERATION_MODE,
						CURRENT)) == MASTER_MODE) {
			dev_warn(dev,
				 "%s: Sequential Trigger Mode not supported in Master mode, switchig to default\n",
				 __func__);
			val = NORMAL_EXPO;
			imx530_update_ctrl(tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE, val,
						0, 0, SEQ_TRIGGER);
			break;
		}
		trigen = 1;
		vint_en = 1;
		dev_dbg(dev, "%s: Sensor is in Sequential Trigger Mode\n",
			__func__);
		break;
	default:
		pr_err("%s: unknown exposure mode.\n", __func__);
		return -EINVAL;
	}

	priv->lifmd_lvds2mipi_1->tg_params.is_shutter_mode = val;

	if (s_data->power->state == SWITCH_ON) {
		err = imx530_write_reg(s_data, TRIGMODE, trigen);
		err |= imx530_write_reg(s_data, VINT_EN, vint_en);
		if (err) {
			dev_err(dev,
				"%s: error setting shutter - exposure mode\n",
				__func__);
			return err;
		}
	}

	return 0;
}

static int imx530_set_custom_ctrls(struct v4l2_ctrl *ctrl)
{
	struct tegracam_ctrl_handler *handler = container_of(
		ctrl->handler, struct tegracam_ctrl_handler, ctrl_handler);
	struct tegracam_device *tc_dev = handler->tc_dev;
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	int err = 0;

	switch (ctrl->id) {
	case IMX530_CAMERA_CID_VERIFY_FW_COMPATIBILITY:
		err = lifmd_lvds2mipi_1_verify_fw_compatibility(
			priv->lifmd_lvds2mipi_1, ctrl, priv->s_data);
		break;
	case IMX530_CAMERA_CID_TG_SET_OPERATION_MODE:
		err = lifmd_lvds2mipi_1_tg_set_operation_mode(
			priv->lifmd_lvds2mipi_1, ctrl);
		break;
	case IMX530_CAMERA_CID_TG_EXPAND_TRIGGER_EXPOSURE:
		err = lifmd_lvds2mipi_1_tg_expand_trigger_exposure(
			priv->lifmd_lvds2mipi_1, *ctrl->p_new.p_u32, priv->s_data);
		break;
	case IMX530_CAMERA_CID_TG_DELAY_FRAME:
		err = lifmd_lvds2mipi_1_tg_delay_frame(
			priv->lifmd_lvds2mipi_1, *ctrl->p_new.p_u32, priv->s_data);
		break;
	default:
		pr_err("%s: unknown ctrl id.\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx530_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx530 *priv = (struct imx530 *)s_data->priv;

	dev_dbg(dev, "%s: power on\n", __func__);

	mutex_lock(&priv->pw_mutex);

	lifmd_lvds2mipi_1_fw_reset(priv->lifmd_lvds2mipi_1, s_data, 1);

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
		goto imx530_mclk_fail;
	}

	err = clk_prepare_enable(pw->mclk);
	if (err) {
		dev_err(dev, "%s: failed to enable mclk\n", __func__);
		return err;
	}

	usleep_range(1, 2);

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pw->reset_gpio)
			fr_gpio_set(s_data, pw->reset_gpio, 1);
	} else {
		dev_dbg(dev, "%s: max96792_power_on\n", __func__);
		max96792_power_on(priv->s_data->dser_dev, &priv->s_data->g_ctx);
	}

	pw->state = SWITCH_ON;

	/* Additional sleep required in the case of hardware power-on sequence*/
	usleep_range(30000, 31000);

	mutex_unlock(&priv->pw_mutex);

	return 0;

imx530_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx530_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx530 *priv = (struct imx530 *)s_data->priv;
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

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pw->reset_gpio)
			fr_gpio_set(s_data, pw->reset_gpio, 0);
	} else {
		dev_dbg(dev, "%s: max96792_power_off\n", __func__);
		max96792_power_off(priv->s_data->dser_dev,
						&priv->s_data->g_ctx);
	}

power_off_done:
	pw->state = SWITCH_OFF;
	mutex_unlock(&priv->pw_mutex);

	return 0;
}

static int imx530_power_get(struct tegracam_device *tc_dev)
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

	mclk_name = pdata->mclk_name ? pdata->mclk_name : "extperiph1";
	pw->mclk = devm_clk_get(dev, mclk_name);
	if (IS_ERR(pw->mclk)) {
		dev_err(dev, "unable to get clock %s\n", mclk_name);
		return PTR_ERR(pw->mclk);
	}

	parentclk_name = pdata->parentclk_name;
	if (parentclk_name) {
		parent = devm_clk_get(dev, parentclk_name);
		if (IS_ERR(parent)) {
			dev_err(dev, "unable to get parent clcok %s",
				parentclk_name);
		} else
			clk_set_parent(pw->mclk, parent);
	}

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		pw->reset_gpio = pdata->reset_gpio;
		pw->pwdn_gpio = pdata->pwdn_gpio;

		if (pdata->use_cam_gpio) {
			err = cam_gpio_register(dev, pw->reset_gpio);
			if (err) {
				dev_err(dev,
					"%s ERR can't register cam gpio %u!\n",
					__func__, pw->reset_gpio);
				goto done;
			}
			err = cam_gpio_register(dev, pw->pwdn_gpio);
			if (err) {
				dev_err(dev,
					"%s ERR can't register cam gpio %u!\n",
					__func__, pw->pwdn_gpio);
				goto done;
			}
		}
	}

done:
	pw->state = SWITCH_OFF;
	return err;
}

static int imx530_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = tc_dev->dev;

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pdata && pdata->use_cam_gpio) {
			cam_gpio_deregister(dev, pw->reset_gpio);
			cam_gpio_deregister(dev, pw->pwdn_gpio);
		} else {
			if (pw->reset_gpio)
				gpio_free(pw->reset_gpio);
			if (pw->pwdn_gpio)
				gpio_free(pw->pwdn_gpio);
		}
	}

	return 0;
}

static struct camera_common_pdata *
imx530_parse_dt(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct device_node *sensor_node = dev->of_node;
	struct device_node *i2c_mux_ch_node;
	struct device_node *gmsl_node;
	struct camera_common_pdata *board_priv_pdata;
	const struct of_device_id *match;
	struct camera_common_pdata *ret = NULL;
	int err;
	int gpio;

	if (!sensor_node)
		return NULL;

	match = of_match_device(imx530_of_match, dev);
	if (!match) {
		dev_err(dev, "Failed to find matching dt id\n");
		return NULL;
	}

	board_priv_pdata =
		devm_kzalloc(dev, sizeof(*board_priv_pdata), GFP_KERNEL);
	if (!board_priv_pdata)
		return NULL;

	err = camera_common_parse_clocks(dev, board_priv_pdata);
	if (err) {
		dev_err(dev, "Failed to find clocks\n");
		goto error;
	}

	gmsl_node = of_get_child_by_name(sensor_node, "gmsl-link");
	if (!gmsl_node) {
		dev_warn(dev, "initializing mipi...\n");
		board_priv_pdata->gmsl = "mipi";
		dev_dbg(dev, "no gmsl-link property found in dt...\n");

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
	} else {
		dev_warn(dev, "initializing GMSL...\n");
		board_priv_pdata->gmsl = "gmsl";
		dev_dbg(dev, "gmsl-link property found in dt...\n");
	}

	fr_get_gpio_ctrl(board_priv_pdata);

	return board_priv_pdata;

error:
	devm_kfree(dev, board_priv_pdata);
	return ret;
}

static int imx530_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx530_write_table(priv, mode_table[IMX530_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx530_write_table(priv, mode_table[IMX530_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx530_set_dep_registers(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 gmrwt = 0x04;
	u8 gmtwt = 0x04;
	u8 gaindly = 0x04;
	u8 gsdly = 0x08;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304) {
			gmrwt = 0x08;
			gmtwt = 0x10;
			gaindly = 0x08;
			gsdly = 0x10;
		} else {
			gmrwt = 0x04;
			gmtwt = 0x08;
			gaindly = 0x04;
			gsdly = 0x08;
		}
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		if (s_data->mode == IMX530_MODE_BINNING_2656x2304) {
			gmrwt = 0x08;
			gmtwt = 0x08;
			gaindly = 0x08;
			gsdly = 0x10;
		} else {
			gmrwt = 0x04;
			gmtwt = 0x04;
			gaindly = 0x04;
			gsdly = 0x08;
		}
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	err = imx530_write_reg(s_data, GMRWT, gmrwt);
	err |= imx530_write_reg(s_data, GMTWT, gmtwt);
	err |= imx530_write_reg(s_data, GAINDLY, gaindly);
	err |= imx530_write_reg(s_data, GSDLY, gsdly);
	if (err) {
		dev_err(dev, "%s: error setting exposure mode\n", __func__);
		return err;
	}

	return err;
}

static int imx530_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx530_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax * IMX530_G_FACTOR) / (IMX530_INCK);

	dev_dbg(dev,
		"%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

/*
 * According to the V4L2 documentation, driver should not return error when
 * invalid settings are detected.
 * It should apply the settings closest to the ones that the user has requested.
 */
static int imx530_check_unsupported_mode(struct camera_common_data *s_data,
					 struct v4l2_mbus_framefmt *mf)
{
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct lifmd_lvds2mipi_1_readout_mode *lifmd_lvds2mipi_1_readout_mode =
		&priv->lifmd_lvds2mipi_1->lifmd_lvds2mipi_1_readout_mode;
	struct device *dev = s_data->dev;
	bool unsupported_mode = false;

	dev_dbg(dev, "%s++\n", __func__);

	if (mf->code == MEDIA_BUS_FMT_SRGGB10_1X10 &&
		(s_data->mode == IMX530_MODE_5328x4608 ||
		 s_data->mode == IMX530_MODE_ROI_5328x3040 ||
		 s_data->mode == IMX530_MODE_BINNING_2656x2304)) {
		unsupported_mode = true;
		dev_warn(dev,
			 "%s: selected mode is not supported with RAW10, switching to default\n",
			 __func__);
	}

	if (unsupported_mode) {
		mf->width = s_data->frmfmt[s_data->def_mode].size.width;
		mf->height = s_data->frmfmt[s_data->def_mode].size.height;
	}

	if (s_data->mode == IMX530_MODE_BINNING_2656x2304)
		lifmd_lvds2mipi_1_readout_mode->line_skip =
			IMX530_READOUT_LINE_SKIP_2;
	else
		lifmd_lvds2mipi_1_readout_mode->line_skip =
			IMX530_READOUT_LINE_SKIP;

	return 0;
}

static int imx530_set_trigger_tgpd(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;

	switch (s_data->mode) {
	case IMX530_MODE_5328x4608:
	case IMX530_MODE_ROI_3840x2160:
	case IMX530_MODE_ROI_4512x4512:
	case IMX530_MODE_ROI_5328x3040:
	case IMX530_MODE_ROI_4064x3008:
	case IMX530_MODE_BINNING_2656x2304:
		priv->lifmd_lvds2mipi_1->tg_params.t_tgpd =
				s_data->fmt_height + IMX530_MIN_FRAME_DELTA;
	}

	return 0;
}

static int imx530_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx530 *priv = (struct imx530 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	/* Update Black level V4L control*/
	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			err = imx530_update_ctrl(
				tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				(*ctrl->p_cur.p_s64 >> 2),
				IMX530_DEFAULT_BLACK_LEVEL_10BPP, 0,
				IMX530_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			err = imx530_update_ctrl(
				tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				(*ctrl->p_cur.p_s64 << 2),
				IMX530_DEFAULT_BLACK_LEVEL_12BPP, 0,
				IMX530_MAX_BLACK_LEVEL_12BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB12_1X12;
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return -EINVAL;
		}
		if (err)
			return err;
	}

	return 0;
}

static int imx530_ctrls_init(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = priv->s_data;
	struct v4l2_ctrl_config *ctrl_cfg;
	struct v4l2_ctrl *ctrl;
	struct tegracam_ctrl_handler *handler = s_data->tegracam_ctrl_hdl;
	int numctrls;
	int err, i;

	numctrls = ARRAY_SIZE(imx530_custom_ctrl_list);

	for (i = 0; i < numctrls; i++) {
		ctrl_cfg = &imx530_custom_ctrl_list[i];

		ctrl = v4l2_ctrl_new_custom(&handler->ctrl_handler, ctrl_cfg,
						NULL);
		if (ctrl == NULL) {
			dev_err(dev, "%s: Failed to create control %s\n",
						__func__, ctrl_cfg->name);
			continue;
		}

		if (ctrl_cfg->type == V4L2_CTRL_TYPE_STRING &&
			ctrl_cfg->flags & V4L2_CTRL_FLAG_READ_ONLY) {
			ctrl->p_new.p_char =
			devm_kzalloc(tc_dev->dev, ctrl_cfg->max + 1, GFP_KERNEL);
			if (!ctrl->p_new.p_char) {
				dev_err(dev, "%s: failed to allocate memory\n",
								__func__);
				return -ENOMEM;
			}
		}
		handler->ctrls[handler->numctrls + i] = ctrl;
		dev_dbg(dev,
			"%s: Added custom control %s to handler index: %d\n",
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

static int imx530_set_mode(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err;

	err = lifmd_lvds2mipi_1_set_readout_mode(priv->lifmd_lvds2mipi_1, s_data);
	if (err)
		goto fail;

	err = imx530_write_table(priv, mode_table[IMX530_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
			__func__);
		return err;
	}

	err = imx530_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
			__func__);
		return err;
	}

	err = imx530_set_dep_registers(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
			__func__);
		return err;
	}

	err = imx530_write_table(priv, mode_table[s_data->mode]);
	if (err) {
		dev_err(dev, "%s: unable to set sensor mode settings\n",
			__func__);
		return err;
	}

	err = imx530_set_operation_mode(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}

	err = imx530_set_shutter_mode(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set shutter mode\n", __func__);
		return err;
	}

	ctrl = fr_find_v4l2_ctrl(tc_dev,
				 IMX530_CAMERA_CID_VERIFY_FW_COMPATIBILITY);
	err = lifmd_lvds2mipi_1_verify_fw_compatibility(priv->lifmd_lvds2mipi_1,
							ctrl, priv->s_data);
	if (err) {
		dev_err(dev, "%s: unable to verify fw compatibility\n",
			__func__);
		return err;
	}

	err = lifmd_lvds2mipi_1_set_is_operation_mode(priv->lifmd_lvds2mipi_1);
	if (err)
		goto fail;

	if (fr_get_v4l2_ctrl_value(tc_dev, IMX530_CAMERA_CID_TG_SET_OPERATION_MODE, CURRENT) != TG_DISABLED) {
		err = lifmd_lvds2mipi_1_tg_set_operation_mode(
			priv->lifmd_lvds2mipi_1,
			fr_find_v4l2_ctrl(tc_dev,
				IMX530_CAMERA_CID_TG_SET_OPERATION_MODE));
		if (err) {
			dev_err(dev, "%s: unable to set TG operation mode\n",
				__func__);
			return err;
		}

		err = lifmd_lvds2mipi_1_tg_expand_trigger_exposure(
			priv->lifmd_lvds2mipi_1,
			fr_get_v4l2_ctrl_value(tc_dev,
				IMX530_CAMERA_CID_TG_EXPAND_TRIGGER_EXPOSURE,
								CURRENT),
			s_data);
		if (err) {
			dev_err(dev,
				"%s: unable to set TG expand triger exposure\n",
				__func__);
			return err;
		}

		err = lifmd_lvds2mipi_1_tg_delay_frame(
			priv->lifmd_lvds2mipi_1,
			fr_get_v4l2_ctrl_value(tc_dev,
				IMX530_CAMERA_CID_TG_DELAY_FRAME, CURRENT),
			s_data);
		if (err) {
			dev_err(dev, "%s: unable to set TG delay frame\n",
				__func__);
			return err;
		}
	}

	err = imx530_set_trigger_tgpd(tc_dev);
	if (err)
		goto fail;

	err = imx530_set_test_pattern(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	s_data->override_enable = true;

	err = imx530_calculate_line_time(tc_dev);
	if (err)
		goto fail;

	err = lifmd_lvds2mipi_1_tg_set_line_width(priv->lifmd_lvds2mipi_1,
							s_data);
	if (err)
		goto fail;

	err = imx530_update_framerate_range(tc_dev);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;

fail:
	dev_err(dev, "%s: unable to set mode\n", __func__);
	return err;
}

static int imx530_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
		err = max96793_setup_streaming(priv->s_data->ser_dev, s_data);
		if (err)
			goto exit;
		err = max96792_setup_streaming(priv->s_data->dser_dev,
						dev,
						s_data);
		if (err)
			goto exit;
		err = max96792_start_streaming(priv->s_data->dser_dev, dev);
		if (err)
			goto exit;
	}

	err = imx530_write_reg(s_data, STANDBY, 0x00);

	usleep_range(7000, 7100);

	err |= lifmd_lvds2mipi_1_start(priv->lifmd_lvds2mipi_1);

	if ((fr_get_v4l2_ctrl_value(priv->tc_dev,
					TEGRA_CAMERA_CID_OPERATION_MODE,
					CURRENT)) == MASTER_MODE) {
		err |= imx530_write_reg(s_data, XMSTA, 0x00);
	} else {
		err |= lifmd_lvds2mipi_1_tg_start(priv->lifmd_lvds2mipi_1);
	}

	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx530_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx530 *priv = (struct imx530 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx530_write_table(priv, mode_table[IMX530_MODE_STOP_STREAM]);

	err |= lifmd_lvds2mipi_1_tg_stop(priv->lifmd_lvds2mipi_1);

	err |= lifmd_lvds2mipi_1_stop(priv->lifmd_lvds2mipi_1);

	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX530_K_FACTOR,
		priv->frame_length * priv->line_time / IMX530_K_FACTOR + 1000);

	return 0;
}

static int imx530_gmsl_serdes_setup(struct imx530 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev ||
							!priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	mutex_lock(&serdes_lock__);

	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	if (!strcmp(priv->s_data->pdata->gmsl, "gmsl")) {
		err = max96792_gmsl_setup(priv->s_data->dser_dev);
		if (err) {
			dev_err(dev, "deserializer gmsl setup failed\n");
			goto error;
		}

		err = max96793_gmsl_setup(priv->s_data->ser_dev);
		if (err) {
			dev_err(dev, "serializer gmsl setup failed\n");
			goto error;
		}
	}

	dev_dbg(dev, "%s: max96792_setup_link\n", __func__);
	err = max96792_setup_link(priv->s_data->dser_dev, &priv->i2c_client->dev);
	if (err) {
		dev_err(dev, "gmsl deserializer link config failed\n");
		goto error;
	}

	dev_dbg(dev, "%s: max96793_setup_control\n", __func__);
	err = max96793_setup_control(priv->s_data->ser_dev);

	if (err)
		dev_err(dev, "gmsl serializer setup failed\n");

	err = max96793_gpio10_xtrig1_setup(priv->s_data->ser_dev, "mipi");
	if (err) {
		dev_err(dev,
			"gmsl serializer gpio10/xtrig1 pin config failed\n");
		goto error;
	}

	dev_dbg(dev, "%s: max96792_setup_control\n", __func__);
	des_err = max96792_setup_control(priv->s_data->dser_dev,
					 &priv->i2c_client->dev);
	if (des_err) {
		dev_err(dev, "gmsl deserializer setup failed\n");
		err = des_err;
	}

error:
	mutex_unlock(&serdes_lock__);
	return err;
}

static void imx530_gmsl_serdes_reset(struct imx530 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static struct camera_common_sensor_ops imx530_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx530_frmfmt),
	.frmfmt_table = imx530_frmfmt,
	.power_on = imx530_power_on,
	.power_off = imx530_power_off,
	.write_reg = imx530_write_reg,
	.read_reg = imx530_read_reg,
	.parse_dt = imx530_parse_dt,
	.power_get = imx530_power_get,
	.power_put = imx530_power_put,
	.set_mode = imx530_set_mode,
	.start_streaming = imx530_start_streaming,
	.stop_streaming = imx530_stop_streaming,
	.check_unsupported_mode = imx530_check_unsupported_mode,
	.after_set_pixel_format = imx530_after_set_pixel_format,
	.init_private_controls = imx530_ctrls_init,
};

static int imx530_board_setup(struct imx530 *priv)
{
	struct camera_common_data *s_data = priv->s_data;
	struct device *dev = s_data->dev;
	struct device_node *node = dev->of_node;
	struct device_node *ser_node;
	struct i2c_client *ser_i2c = NULL;
	struct device_node *dser_node;
	struct i2c_client *dser_i2c = NULL;
	struct device_node *gmsl;
	struct device_node *lifmd_lvds2mipi_1_node;
	struct i2c_client *lifmd_lvds2mipi_1_i2c = NULL;
	int value = 0xFFFF;
	const char *str_value;
	const char *str_value1[2];
	int i;
	int err = 0;

	dev_dbg(dev, "%s++\n", __func__);
	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
		err = of_property_read_u32(node, "reg",
						&priv->s_data->g_ctx.sdev_reg);
		if (err < 0) {
			dev_err(dev, "reg not found\n");
			return err;
		}

		priv->s_data->g_ctx.sdev_def = priv->s_data->g_ctx.sdev_reg;

		ser_node = of_parse_phandle(node, "nvidia,gmsl-ser-device", 0);
		if (ser_node == NULL) {
			dev_err(dev, "missing %s handle\n",
						"nvidia,gmsl-ser-device");
			return err;
		}

		err = of_property_read_u32(ser_node, "reg",
						&priv->s_data->g_ctx.ser_reg);
		if (err < 0) {
			dev_err(dev, "serializer reg not found\n");
			return err;
		}

		ser_i2c = of_find_i2c_device_by_node(ser_node);
		of_node_put(ser_node);

		if (ser_i2c == NULL) {
			dev_err(dev, "missing serializer dev handle\n");
			return err;
		}
		if (ser_i2c->dev.driver == NULL) {
			dev_err(dev, "missing serializer driver\n");
			return err;
		}

		priv->s_data->ser_dev = &ser_i2c->dev;

		dser_node =
			of_parse_phandle(node, "nvidia,gmsl-dser-device", 0);
		if (dser_node == NULL) {
			dev_err(dev, "missing %s handle\n",
				"nvidia,gmsl-dser-device");
			return err;
		}

		dser_i2c = of_find_i2c_device_by_node(dser_node);
		of_node_put(dser_node);

		if (dser_i2c == NULL) {
			dev_err(dev, "missing deserializer dev handle\n");
			return err;
		}
		if (dser_i2c->dev.driver == NULL) {
			dev_err(dev, "missing deserializer driver\n");
			return err;
		}

		priv->s_data->dser_dev = &dser_i2c->dev;

		gmsl = of_get_child_by_name(node, "gmsl-link");
		if (gmsl == NULL) {
			dev_err(dev, "missing gmsl-link device node\n");
			err = -EINVAL;
			return err;
		}

		err = of_property_read_string(gmsl, "dst-csi-port", &str_value);
		if (err < 0) {
			dev_err(dev, "No dst-csi-port found\n");
			return err;
		}
		priv->s_data->g_ctx.dst_csi_port = (!strcmp(str_value, "a"))
							? GMSL_CSI_PORT_A
							: GMSL_CSI_PORT_B;

		err = of_property_read_string(gmsl, "src-csi-port", &str_value);
		if (err < 0) {
			dev_err(dev, "No src-csi-port found\n");
			return err;
		}
		priv->s_data->g_ctx.src_csi_port = (!strcmp(str_value, "a"))
							? GMSL_CSI_PORT_A
							: GMSL_CSI_PORT_B;

		err = of_property_read_string(gmsl, "csi-mode", &str_value);
		if (err < 0) {
			dev_err(dev, "No csi-mode found\n");
			return err;
		}

		if (!strcmp(str_value, "1x4")) {
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_1X4_MODE;
		} else if (!strcmp(str_value, "2x4")) {
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_2X4_MODE;
		} else if (!strcmp(str_value, "2x2")) {
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_2X2_MODE;
		} else {
			dev_err(dev, "invalid csi mode\n");
			return err;
		}

		err = of_property_read_string(gmsl, "serdes-csi-link",
							&str_value);
		if (err < 0) {
			dev_err(dev, "No serdes-csi-link found\n");
			return err;
		}
		priv->s_data->g_ctx.serdes_csi_link =
			(!strcmp(str_value, "a")) ? GMSL_SERDES_CSI_LINK_A
						: GMSL_SERDES_CSI_LINK_B;

		err = of_property_read_u32(gmsl, "st-vc", &value);
		if (err < 0) {
			dev_err(dev, "No st-vc info\n");
			return err;
		}
		priv->s_data->g_ctx.st_vc = value;

		err = of_property_read_u32(gmsl, "vc-id", &value);
		if (err < 0) {
			dev_err(dev, "No vc-id info\n");
			return err;
		}
		priv->s_data->g_ctx.dst_vc = value;

		priv->s_data->g_ctx.num_csi_lanes = s_data->numlanes;

		priv->s_data->g_ctx.num_streams =
			of_property_count_strings(gmsl, "streams");
		if (priv->s_data->g_ctx.num_streams <= 0) {
			dev_err(dev, "No streams found\n");
			err = -EINVAL;
			return err;
		}

		for (i = 0; i < priv->s_data->g_ctx.num_streams; i++) {
			of_property_read_string_index(gmsl, "streams", i,
								&str_value1[i]);
			if (!str_value1[i]) {
				dev_err(dev, "invalid stream info\n");
				return err;
			}
			if (!strcmp(str_value1[i], "raw12")) {
				priv->s_data->g_ctx.streams[i].st_data_type =
					GMSL_CSI_DT_RAW_12;
			} else if (!strcmp(str_value1[i], "embed")) {
				priv->s_data->g_ctx.streams[i].st_data_type =
					GMSL_CSI_DT_EMBED;
			} else if (!strcmp(str_value1[i], "ued-u1")) {
				priv->s_data->g_ctx.streams[i].st_data_type =
					GMSL_CSI_DT_UED_U1;
			} else {
				dev_err(dev, "invalid stream data type\n");
				return err;
			}
		}

		priv->s_data->g_ctx.s_dev = dev;

		mutex_init(&serdes_lock__);
		err = max96793_sdev_pair(priv->s_data->ser_dev,
					 &priv->s_data->g_ctx);
		if (err) {
			dev_err(dev, "gmsl ser pairing failed\n");
			return err;
		}

		err = max96792_sdev_register(priv->s_data->dser_dev,
						 &priv->s_data->g_ctx);
		if (err) {
			dev_err(dev, "gmsl deserializer register failed\n");
			return err;
		}

		err = imx530_gmsl_serdes_setup(priv);
		if (err) {
			dev_err(dev, "%s gmsl serdes setup failed\n", __func__);
			return err;
		}
	}

	lifmd_lvds2mipi_1_node =
		of_parse_phandle(node, "lifmd_lvds2mipi_1-device", 0);
	if (lifmd_lvds2mipi_1_node == NULL) {
		dev_err(dev, "missing %s handle\n", "lifmd_lvds2mipi_1-device");
		return err;
	}

	lifmd_lvds2mipi_1_i2c =
		of_find_i2c_device_by_node(lifmd_lvds2mipi_1_node);
	if (lifmd_lvds2mipi_1_i2c == NULL) {
		dev_err(dev, "missing lifmd_lvds2mipi_1 i2c dev handle\n");
		return err;
	}

	priv->lifmd_lvds2mipi_1->i2c_client = lifmd_lvds2mipi_1_i2c;
	priv->lifmd_lvds2mipi_1->i2c_client->dev = lifmd_lvds2mipi_1_i2c->dev;

	priv->lifmd_lvds2mipi_1->regmap =
		devm_regmap_init_i2c(priv->lifmd_lvds2mipi_1->i2c_client,
					&lifmd_lvds2mipi_1_regmap_config);
	if (IS_ERR_OR_NULL(priv->lifmd_lvds2mipi_1->regmap)) {
		dev_err(dev, "regmap init failed: %ld\n",
				PTR_ERR(priv->lifmd_lvds2mipi_1->regmap));
		return -ENODEV;
	}

	err = camera_common_mclk_enable(s_data);
	if (err) {
		dev_err(dev, "Error %d turning on mclk\n", err);
		goto error2;
	}

	err = imx530_power_on(s_data);
	if (err) {
		dev_err(dev, "Error %d during power on sensor\n", err);
		goto error2;
	}

error2:
	imx530_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx530_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "imx530 : open\n");

	return 0;
}

static const struct v4l2_subdev_internal_ops imx530_subdev_internal_ops = {
	.open = imx530_open,
};

static struct tegracam_ctrl_ops imx530_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx530_set_gain,
	.set_exposure = imx530_set_exposure,
	.set_frame_rate = imx530_set_frame_rate,
	.set_group_hold = imx530_set_group_hold,
	.set_test_pattern = imx530_set_test_pattern,
	.set_operation_mode = imx530_set_operation_mode,
	.set_shutter_mode = imx530_set_shutter_mode,
	.set_black_level = imx530_set_black_level,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx530_probe(struct i2c_client *client)
#else
static int imx530_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx530 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev, sizeof(struct imx530), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev, sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx530", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx530_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx530_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx530_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	priv->frame_length = IMX530_DEFAULT_HEIGHT + IMX530_MIN_FRAME_DELTA;
	priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB12_1X12;
	priv->line_time = IMX530_DEFAULT_LINE_TIME;
	priv->min_frame_length = IMX530_DEFAULT_HEIGHT + IMX530_MIN_FRAME_DELTA;
	priv->s_data->def_mode = IMX530_MODE_ROI_3840x2160;

	priv->lifmd_lvds2mipi_1 =
		devm_kzalloc(dev, sizeof(struct lifmd_lvds2mipi_1), GFP_KERNEL);
	if (!priv->lifmd_lvds2mipi_1)
		return -ENOMEM;

	priv->lifmd_lvds2mipi_1->tg_params.tg_mode = TG_DISABLED;
	priv->lifmd_lvds2mipi_1->tg_params.sync_logic = 0;
	priv->lifmd_lvds2mipi_1->tg_params.out_logic = 0;
	priv->lifmd_lvds2mipi_1->tg_params.xhs_min_active_width = 8;
	priv->lifmd_lvds2mipi_1->tg_params.xhs_clk_offset = 1;
	priv->lifmd_lvds2mipi_1->tg_params.frame_active_width = 2;
	priv->lifmd_lvds2mipi_1->tg_params.line_time = &priv->line_time;

	priv->lifmd_lvds2mipi_1->sensor_numch = IMX530_LVDS_NUM_CHANNELS;
	priv->lifmd_lvds2mipi_1->lifmd_lvds2mipi_1_readout_mode =
					lifmd_lvds2mipi_1_def_readout_mode;
	priv->lifmd_lvds2mipi_1->lifmd_lvds2mipi_1_readout_mode.line_skip =
						IMX530_READOUT_LINE_SKIP;

	priv->lifmd_lvds2mipi_1->is_pw_state = &priv->s_data->power->state;

	/* Get default device tree properties of first sensor mode */
	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	err = imx530_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx530_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				 IMX530_DEFAULT_BLACK_LEVEL_12BPP,
				 IMX530_DEFAULT_BLACK_LEVEL_12BPP, 0,
				 IMX530_MAX_BLACK_LEVEL_12BPP);
	if (err)
		return err;

	err = imx530_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				 (ARRAY_SIZE(imx530_test_pattern_menu) - 1));
	if (err)
		return err;

	dev_info(dev, "Detected imx530 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx530_remove(struct i2c_client *client)
#else
static void imx530_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx530 *priv;

	if (s_data == NULL) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx530 *)s_data->priv;

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		imx530_gmsl_serdes_reset(priv);


	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		mutex_destroy(&serdes_lock__);


#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx530_id[] = {{"imx530", 0}, {}};

MODULE_DEVICE_TABLE(i2c, imx530_id);

static struct i2c_driver imx530_i2c_driver = {
	.driver = {
		.name = "imx530",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx530_of_match),
	},
	.probe = imx530_probe,
	.remove = imx530_remove,
	.id_table = imx530_id,
};

module_i2c_driver(imx530_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX530");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
