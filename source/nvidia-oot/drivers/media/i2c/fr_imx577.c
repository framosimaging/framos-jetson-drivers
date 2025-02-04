// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx577.c - Framos fr_imx577.c driver
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

#include "fr_imx577_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX577_K_FACTOR 1000LL
#define IMX577_M_FACTOR 1000000LL
#define IMX577_G_FACTOR 1000000000LL
#define IMX577_T_FACTOR 1000000000000LL

#define IMX577_MAX_ANALOG_GAIN_TIMES 22261
#define IMX577_ANALOG_GAIN_OFFSET 1024
#define IMX577_MAX_ANALOG_GAIN 978
#define IMX577_DIGITAL_GAIN_COEF 1000
#define IMX577_DIGITAL_GAIN_STEP 39
#define IMX577_DIGITAL_GAIN_MIN (1*IMX577_DIGITAL_GAIN_COEF)
#define IMX577_DIGITAL_GAIN_MAX (16*IMX577_DIGITAL_GAIN_COEF - 1)

#define IMX577_MAX_BLACK_LEVEL_8BPP		63
#define IMX577_MAX_BLACK_LEVEL_10BPP		255
#define IMX577_MAX_BLACK_LEVEL_12BPP		1023
#define IMX577_DEFAULT_BLACK_LEVEL_8BPP		16
#define IMX577_DEFAULT_BLACK_LEVEL_10BPP	64
#define IMX577_DEFAULT_BLACK_LEVEL_12BPP	256

#define IMX577_IVT_PXCK_DIV_VAL 5
#define IMX577_IMAGE_PIPE_LINES 4

#define IMX577_MIN_INTEGRATION_LINES 1
#define IMX577_MIN_INTEGRATION_LINES_OFFSET 22
#define IMX577_FINE_STORAGE_TIME 0x790LL
#define IMX577_MAX_FRM_LENGTH_LINES 65535
#define IMX577_MAX_COARSE_INTEG_TIME_LINES 65535

#define IMX577_MAX_CSI_LANES 4
#define IMX577_TWO_LANE_MODE 2

#define IMX577_INCK 27000000LL

LIST_HEAD(imx577_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx577_of_match[] = {
	{ .compatible = "framos,imx577",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx577_of_match);

static const char * const imx577_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Solid color",
	[2] = "100% color bars",
	[3] = "Fade to grey color bars",
	[4] = "PN9",
};

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_GAIN,
	TEGRA_CAMERA_CID_EXPOSURE,
	TEGRA_CAMERA_CID_FRAME_RATE,
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_OPERATION_MODE,
	TEGRA_CAMERA_CID_BROADCAST,
	TEGRA_CAMERA_CID_BLACK_LEVEL,
	TEGRA_CAMERA_CID_TEST_PATTERN,
};

struct imx577 {
	struct i2c_client		*i2c_client;
	struct v4l2_subdev		*subdev;
	u64				frame_length;
	u64				min_frame_length;
	u64				current_pixel_format;
	u32				integration_offset;
	u32				line_time;
	i2c_broadcast_ctrl		broadcast_ctrl;
	struct mutex			pw_mutex;
	struct list_head		entry;
	struct camera_common_data	*s_data;
	struct tegracam_device		*tc_dev;
};

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
	.use_single_read = true,
	.use_single_write = true,
};

static inline int imx577_read_reg(struct camera_common_data *s_data,
				u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx577_write_reg(struct camera_common_data *s_data,
				u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "%s: i2c write failed, 0x%x = %x\n",
			__func__, addr, val);

	return err;
}

static int imx577_write_reg_broadcast(struct camera_common_data *s_data,
					u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->broadcast_regmap, addr, val);
	if (err) {
		dev_err(dev, "%s: i2c write failed, %x = %x\n",
		__func__, addr, val);
	}

	return err;
}

static int imx577_read_buffered_reg(struct camera_common_data *s_data,
					u16 addr_low, u8 number_of_registers,
					u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;

	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
			__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx577_read_reg(s_data, addr_low - i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
			__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
			__func__);
			return err;
		}
	}

	return err;
}

static int imx577_write_buffered_reg(struct camera_common_data *s_data,
				u16 addr_low, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx577_write_reg(s_data, addr_low - i, (u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
			 __func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx577_broadcast_buffered_reg(struct camera_common_data *s_data,
				u16 addr_low, u8 number_of_registers, u32 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx577_write_reg_broadcast(s_data, GRP_PARAM_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx577_write_reg_broadcast(s_data, addr_low - i,
		(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
			__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx577_write_reg_broadcast(s_data, GRP_PARAM_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx577_write_table(struct imx577 *priv,
				const imx577_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					 table,
					 NULL, 0,
					 IMX577_TABLE_WAIT_MS,
					 IMX577_TABLE_END);
}

static int imx577_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx577_write_reg(s_data, GRP_PARAM_HOLD, val);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}

	return err;
}

static int imx577_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val,
				u64 min_val, u64 max_val)
{
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct v4l2_ctrl *ctrl;

	/* Update Black level control*/
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
			ctrl->qmenu = imx577_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		}
	}
	return 0;
}

static int imx577_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	s64 black_level_reg;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB8_1X8)
		black_level_reg = val << 4;
	else if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
		black_level_reg = val << 2;
	else
		black_level_reg = val;

	err = imx577_write_reg(s_data, MANUAL_DATA_PEDESTAL_EN, 1);
	if (err)
		goto fail;

	err = imx577_write_buffered_reg(s_data, MANUAL_DATA_PEDESTAL_VALUE_LOW,
					2, black_level_reg);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;

fail:
	dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
	return err;
}

static int imx577_get_digital_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct device *dev = tc_dev->dev;
	u32 integer_part, decimal_part, digital_gain;


	if (val > IMX577_DIGITAL_GAIN_MAX)
		val = IMX577_DIGITAL_GAIN_MAX;

	integer_part = val / IMX577_DIGITAL_GAIN_COEF;
	decimal_part = ((val % IMX577_DIGITAL_GAIN_COEF) * 10)
			/ IMX577_DIGITAL_GAIN_STEP;
	decimal_part = (decimal_part > 0xFF) ? 0xFF : decimal_part;

	digital_gain = (integer_part << 8) + decimal_part;

	dev_dbg(dev, "%s: Digital gain val: %lld, Digital gain reg: %xh\n",
						__func__, val, digital_gain);

	return digital_gain;
}

static int imx577_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx577 *priv = (struct imx577 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err = 0, i;
	u32 analog_gain;
	u32 digital_gain = 1 << 8;
	s64 digital_part;

	analog_gain = IMX577_ANALOG_GAIN_OFFSET - (IMX577_ANALOG_GAIN_OFFSET
			* mode->control_properties.gain_factor) / val;
	analog_gain = (analog_gain > IMX577_MAX_ANALOG_GAIN) ?
			IMX577_MAX_ANALOG_GAIN : analog_gain;

	if (val > IMX577_MAX_ANALOG_GAIN_TIMES) {
		digital_part = (val * IMX577_DIGITAL_GAIN_COEF +
				 IMX577_MAX_ANALOG_GAIN_TIMES/2)
				/ IMX577_MAX_ANALOG_GAIN_TIMES;
	digital_gain = imx577_get_digital_gain(tc_dev, digital_part);
	}

	if (priv->broadcast_ctrl == BROADCAST) {
		if (!s_data->group_hold_active)
			err = imx577_write_reg_broadcast(s_data,
			GRP_PARAM_HOLD, 0x01);

		for (i = 0; i < 2; i++)
			err |= imx577_write_reg_broadcast(s_data, ANA_GAIN_GLOBAL_LOW - i,
						(u8)(analog_gain >> (i * 8)));

		for (i = 0; i < 2; i++)
			err |= imx577_write_reg_broadcast(s_data, DIG_GAIN_GR_LOW - i,
						(u8)(digital_gain >> (i * 8)));

		if (!s_data->group_hold_active)
			err |= imx577_write_reg_broadcast(s_data, GRP_PARAM_HOLD, 0x00);
	} else {
		if (!s_data->group_hold_active)
			err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x01);

		for (i = 0; i < 2; i++)
			err |= imx577_write_reg(s_data, ANA_GAIN_GLOBAL_LOW - i,
						(u8)(analog_gain >> (i * 8)));

		for (i = 0; i < 2; i++)
			err |= imx577_write_reg(s_data, DIG_GAIN_GR_LOW - i,
						(u8)(digital_gain >> (i * 8)));

		if (!s_data->group_hold_active)
			err |= imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x00);
	}
	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: gain val [%lld] reg [%d]\n", __func__, val, analog_gain);

	return 0;
}

static int imx577_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx577 *priv = (struct imx577 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u64 integration_time_line;
	int i;
	u8 cit_lshift = 0;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line = ((val - priv->integration_offset)
					* IMX577_K_FACTOR) / priv->line_time;

	if (integration_time_line > IMX577_MAX_COARSE_INTEG_TIME_LINES) {
		fr_calc_lshift(&integration_time_line, &cit_lshift,
				IMX577_MAX_COARSE_INTEG_TIME_LINES);
		integration_time_line = integration_time_line << cit_lshift;
	}

	if (integration_time_line < IMX577_MIN_INTEGRATION_LINES)
		integration_time_line = IMX577_MIN_INTEGRATION_LINES;
	else if (integration_time_line >
		(priv->frame_length - IMX577_MIN_INTEGRATION_LINES_OFFSET))
		integration_time_line = priv->frame_length - IMX577_MIN_INTEGRATION_LINES_OFFSET;

	if (priv->broadcast_ctrl == BROADCAST) {
		err = imx577_broadcast_buffered_reg(s_data, COARSE_INTEG_TIME_LOW, 2,
					integration_time_line >> cit_lshift);
	} else {
		if (!s_data->group_hold_active)
			err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x01);

		for (i = 0; i < 2; i++) {
			err |= imx577_write_reg(s_data, (COARSE_INTEG_TIME_LOW - i),
			 (u8)((integration_time_line >> cit_lshift) >> (i * 8)));
		}

		err |= imx577_write_reg(s_data, CIT_LSHIFT, cit_lshift);

		if (!s_data->group_hold_active)
			err |= imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x00);
		}

		if (err) {
			dev_err(dev, "%s: failed to set exposure\n", __func__);
		return err;
		}

	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_EXPOSURE);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	dev_dbg(dev,
	"%s: set integration time: %lld [us], coarse1:%llu [line], frame length: %llu [line]\n",
	__func__, val, integration_time_line, priv->frame_length);

	return err;
}

static int imx577_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx577 *priv = (struct imx577 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int i;
	u8 fll_lshift = 0;

	frame_length = (((u64)mode->control_properties.framerate_factor *
				IMX577_G_FACTOR) / (val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	if (frame_length > IMX577_MAX_FRM_LENGTH_LINES) {
		fr_calc_lshift(&frame_length, &fll_lshift,
				IMX577_MAX_FRM_LENGTH_LINES);
		frame_length = frame_length << fll_lshift;
	}

	priv->frame_length = frame_length;

	exposure_min_range = IMX577_MIN_INTEGRATION_LINES
				* priv->line_time / IMX577_K_FACTOR;
	exposure_min_range += priv->integration_offset;

	exposure_max_range = (priv->frame_length - IMX577_MIN_INTEGRATION_LINES_OFFSET)
				* priv->line_time / IMX577_K_FACTOR;
	exposure_max_range += priv->integration_offset;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				exposure_min_range, exposure_max_range);

	if (!s_data->group_hold_active)
		err = imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x01);

	for (i = 0; i < 2; i++)
		err |= imx577_write_reg(s_data, FRM_LENGTH_LINES_LOW - i,
			(u8)((priv->frame_length >> fll_lshift) >> (i * 8)));

	err |= imx577_write_reg(s_data, FLL_LSHIFT, fll_lshift);

	if (!s_data->group_hold_active)
		err |= imx577_write_reg(s_data, GRP_PARAM_HOLD, 0x00);

	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
	return err;
	}

	dev_dbg(dev,
	"%s: val: %lld, frame_length set: %llu\n",
		__func__, val, priv->frame_length);

	return 0;
}

static int imx577_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx577_write_reg(s_data, TP_MODE, val);
	if (err)
		goto fail;

	dev_dbg(dev, "%s++ Test mode pattern: %u\n", __func__, val);

	return 0;
fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx577_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	u64 max_framerate;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	switch (s_data->mode) {
	case IMX577_MODE_SCALE_HV3:
		priv->min_frame_length = IMX577_DEFAULT_HEIGHT
					+ IMX577_MIN_FRAME_LENGTH_DELTA;
		break;
	default:
		priv->min_frame_length = s_data->fmt_height
					+ IMX577_MIN_FRAME_LENGTH_DELTA;
	}

	max_framerate = (IMX577_G_FACTOR * IMX577_M_FACTOR) /
					(priv->min_frame_length * priv->line_time);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				ctrlprops->min_framerate, max_framerate);

	return 0;
}

static int imx577_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 xmaster;

	if (tc_dev->is_streaming)
		return 0;

	if (val == SLAVE_MODE)
		xmaster = 0;
	else
		xmaster = 1;

	if (s_data->power->state == SWITCH_ON) {
		err = imx577_write_reg(s_data, MASTER_SLAVE_SEL, xmaster);
	if (err)
		dev_err(dev, "%s: error setting operation mode\n", __func__);
	}

	return err;
}

static bool imx577_find_broadcast_sensor(struct imx577 *broadcast_private)
{
	struct imx577 *current_private;

	list_for_each_entry(current_private, &imx577_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->broadcast_ctrl == BROADCAST
			&& current_private->s_data->power->state == SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			memcpy(broadcast_private, current_private,
					sizeof(*broadcast_private));
			return true;
		}
		mutex_unlock(&current_private->pw_mutex);
	}
	return false;
}

static void imx577_enable_second_slave_address(struct imx577 *ack_private)
{
	struct imx577 *current_private;
	int err;

	list_for_each_entry(current_private, &imx577_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx577_write_reg(current_private->s_data,
					SLAVE_ADD_EN_2ND, 1);
		err |= imx577_write_reg(current_private->s_data,
					SLAVE_ADD_ACKEN_2ND, 0);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				"%s: Fail to write Second I2C register\n", __func__);

		mutex_unlock(&current_private->pw_mutex);

		dev_dbg(&current_private->i2c_client->dev,
				"%s: Sensors 2nd slave address configured\n", __func__);
	}

	err = imx577_write_reg(ack_private->s_data,
				SLAVE_ADD_ACKEN_2ND, 1);
	if (err)
		dev_warn(&ack_private->i2c_client->dev,
			"%s: Fail to write Second I2C register\n", __func__);

	dev_dbg(&ack_private->i2c_client->dev,
			": Sensors 2nd slave address configured with acknowlege\n");
}

static void imx577_disable_second_slave_address(void)
{
	struct imx577 *current_private;
	int err;

	list_for_each_entry(current_private, &imx577_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx577_write_reg(current_private->s_data, SLAVE_ADD_EN_2ND, 0);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				"%s: Fail to write Second I2C register\n", __func__);

		mutex_unlock(&current_private->pw_mutex);

		dev_dbg(&current_private->i2c_client->dev,
				"%s: Sensors 2nd slave address disabled\n", __func__);
	}
}

static void imx577_configure_second_slave_address(void)
{
	struct imx577 broadcast_private = {};

	if (imx577_find_broadcast_sensor(&broadcast_private))
		imx577_enable_second_slave_address(&broadcast_private);
	else
		imx577_disable_second_slave_address();
}

static int imx577_set_broadcast_ctrl(struct tegracam_device *tc_dev,
					struct v4l2_ctrl *ctrl)
{
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	if (ctrl->flags & V4L2_CTRL_FLAG_INACTIVE) {
		ctrl->val = UNICAST;
		dev_info(dev, "%s: Broadcast control is inactive\n", __func__);
		return 0;
	}

	err = common_get_broadcast_client(tc_dev, ctrl, &sensor_regmap_config);
	if (err)
		return err;

	priv->broadcast_ctrl = *ctrl->p_new.p_u8;
	imx577_configure_second_slave_address();

	return 0;
}

static int imx577_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx577 *priv = (struct imx577 *)s_data->priv;

	dev_dbg(dev, "%s: power on\n", __func__);

	mutex_lock(&priv->pw_mutex);

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
		goto imx577_mclk_fail;
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

	/* Additional sleep required in the case of hardware power-on sequence */
	usleep_range(30000, 31000);

	imx577_write_reg(s_data, EXCK_FREQ_HIGH, 0x1B);
	imx577_write_reg(s_data, EXCK_FREQ_LOW, 0x00);

	mutex_unlock(&priv->pw_mutex);

	imx577_configure_second_slave_address();

	return 0;

imx577_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx577_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx577 *priv = (struct imx577 *)s_data->priv;
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
		max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);
	}

power_off_done:
	pw->state = SWITCH_OFF;
	mutex_unlock(&priv->pw_mutex);

	return 0;
}

static int imx577_power_get(struct tegracam_device *tc_dev)
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
		if (IS_ERR(parent)) {
			dev_err(dev, "unable to get parent clcok %s",
				parentclk_name);
		} else
			clk_set_parent(pw->mclk, parent);
	}




	if (err) {
		dev_info(dev, "%s: unable to get regulator(s)\n", __func__);
		goto done;
	}

	pw->reset_gpio = pdata->reset_gpio;

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		pw->reset_gpio = pdata->reset_gpio;

		if (pdata->use_cam_gpio) {
			err = cam_gpio_register(dev, pw->reset_gpio);
			if (err) {
				dev_err(dev, "%s ERR can't register cam gpio %u!\n",
				 __func__, pw->reset_gpio);
			}
		}
	}

done:
	pw->state = SWITCH_OFF;
	return err;
}

static int imx577_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = tc_dev->dev;

	if (unlikely(!pw))
		return -EFAULT;



	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pdata && pdata->use_cam_gpio)
			cam_gpio_deregister(dev, pw->reset_gpio);
		else {
			if (pw->reset_gpio)
				gpio_free(pw->reset_gpio);
		}
	}

	return 0;
}

static int imx577_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx577_read_buffered_reg(s_data, FRM_LENGTH_LINES_LOW, 2, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}

	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *imx577_parse_dt(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct device_node *sensor_node = dev->of_node;
	struct camera_common_pdata *board_priv_pdata;
	const struct of_device_id *match;
	struct camera_common_pdata *ret = NULL;
	int err;
	int gpio;
	struct device_node *i2c_mux_ch_node;
	struct device_node *gmsl_node;

	if (!sensor_node)
		return NULL;

	match = of_match_device(imx577_of_match, dev);
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

gmsl_node = of_get_child_by_name(sensor_node, "gmsl-link");
	if (gmsl_node == NULL) {
		dev_warn(dev, "initializing mipi...\n");
		board_priv_pdata->gmsl = "mipi";
		dev_dbg(dev, "no gmsl-link property found in dt...\n");

		i2c_mux_ch_node = of_get_parent(sensor_node);
		if (!i2c_mux_ch_node) {
			dev_err(dev, "i2c mux channel node not found in dt\n");
			goto error;
		}
		gpio = of_get_named_gpio(i2c_mux_ch_node, "reset-gpios", 0);
		if (gpio < 0) {
			if (gpio == -EPROBE_DEFER)
				ret = ERR_PTR(-EPROBE_DEFER);
			dev_err(dev, "reset-gpios not found\n");
			goto error;
		}
		board_priv_pdata->reset_gpio = (unsigned int)gpio;
	} else {
		dev_warn(dev, "initializing GMSL...\n");
		board_priv_pdata->gmsl = "gmsl";
		dev_dbg(dev, "gmsl-link property found in dt...\n");
	}

	gpio = of_get_named_gpio(sensor_node, "vsync-gpios", 0);
	if (gpio > 0)
		gpio_direction_input(gpio);

	fr_get_gpio_ctrl(board_priv_pdata);

	return board_priv_pdata;

error:
	devm_kfree(dev, board_priv_pdata);
	return ret;
}

static int imx577_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		err = imx577_write_table(priv, mode_table[IMX577_8BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx577_write_table(priv, mode_table[IMX577_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx577_write_table(priv, mode_table[IMX577_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx577_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == IMX577_TWO_LANE_MODE) {
		err = imx577_write_reg(s_data, CSI_LANE_MODE, 1);
		if (err) {
			dev_err(dev, "%s: error setting two lane mode\n", __func__);
			return err;
		}
	}

	dev_dbg(dev, "%s: sensor is in %d CSI lane mode\n",
			__func__, s_data->numlanes);

	return 0;
}

static int imx577_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx577 *priv = (struct imx577 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 line_length;
	u32 ivtck;
	u32 ivtpxck;
	u8 ivt_prepllck_div;
	u64 ivt_pll_mpy;
	u8 ivt_syck_div;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx577_read_buffered_reg(s_data, LINE_LENGTH_PCK_LOW, 2, &line_length);
	if (err)
		goto fail;

	err = imx577_read_reg(s_data, IVT_SYCK_DIV, &ivt_syck_div);
	if (err)
		goto fail;

	err = imx577_read_reg(s_data, IVT_PREPLLCK_DIV, &ivt_prepllck_div);
	if (err)
		goto fail;

	err = imx577_read_buffered_reg(s_data, IVT_PLL_MPY_LOW, 2, &ivt_pll_mpy);
	if (err)
		goto fail;

	ivtck = IMX577_INCK * ivt_pll_mpy / ivt_prepllck_div;
	ivtpxck = ivtck / (ivt_syck_div * IMX577_IVT_PXCK_DIV_VAL) / IMX577_M_FACTOR;
	priv->line_time = ((line_length / IMX577_IMAGE_PIPE_LINES)
				* IMX577_K_FACTOR) / ivtpxck;

	priv->integration_offset = (IMX577_FINE_STORAGE_TIME * priv->line_time) /
				(line_length * IMX577_K_FACTOR);

	dev_dbg(dev,
"%s: Line length: %llu [pix], ivt_syck_div: %u, ivt_prepllck_div: %u, ivt_pll_mpy: %llu, Line time: %u[ns]\n",
	__func__, line_length, (u32)ivt_syck_div, (u32)ivt_prepllck_div,
				ivt_pll_mpy, priv->line_time);

	return 0;

fail:
	dev_err(dev, "%s: Fail to calculate line time\n", __func__);
	return err;
}

static int imx577_adjust_hmax_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 line_length;

	dev_dbg(dev, "%s:++\n", __func__);

	if (s_data->numlanes == 2) {
		imx577_read_buffered_reg(s_data, LINE_LENGTH_PCK_LOW, 2, &line_length);

		line_length *= 2;

		err = imx577_write_buffered_reg(s_data, LINE_LENGTH_PCK_LOW,
					2, line_length);
		if (err) {
			dev_err(dev, "%s: error setting line length\n", __func__);
			return err;
		}

		dev_dbg(dev, "%s: Line length: %llu\n", __func__, line_length);
	}

	return 0;
}

static int imx577_configure_triggering_pins(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;


	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT)) {
	case MASTER_MODE:

		err = imx577_write_reg(s_data, XVS_IO_CTRL, 1);

		err |= imx577_write_reg(s_data, EXTOUT_EN, 1);
		break;
	case SLAVE_MODE:

		err = imx577_write_reg(s_data, XVS_IO_CTRL, 0);
		dev_dbg(dev, "%s: Sensor is in Slave mode\n", __func__);
		break;
	default:
		dev_err(dev, "%s: unknown operating mode.\n", __func__);
		return -EINVAL;
	}

	if (err) {
		dev_err(dev, "%s: Fail to configure triggering pin\n", __func__);
		return err;
	}

	return 0;
}

/*
 * According to the V4L2 documentation, driver should not return error when
 * invalid settings are detected.
 * It should apply the settings closest to the ones that the user has requested.
 */
static int imx577_check_unsupported_mode(struct camera_common_data *s_data,
				 struct v4l2_mbus_framefmt *mf)
{
	struct device *dev = s_data->dev;
	bool unsupported_mode = false;

	dev_dbg(dev, "%s++\n", __func__);

	if (mf->code == MEDIA_BUS_FMT_SRGGB12_1X12) {
		switch (s_data->mode_prop_idx) {
		case IMX577_MODE_CROP_BINNING_H2V2:
			unsupported_mode = true;
			dev_warn(dev,
			"%s: selected mode is not supported with RAW12, switching to default\n",
				 __func__);
			break;
		default:
			unsupported_mode = false;
		}
}

	if (unsupported_mode) {
		mf->width	= s_data->frmfmt[s_data->def_mode].size.width;
		mf->height	= s_data->frmfmt[s_data->def_mode].size.height;
	}

	return 0;
}

static int imx577_set_ivtck(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 ivt_pll_mpy = 311;

	err = imx577_write_buffered_reg(s_data, IVT_PLL_MPY_LOW, 2, ivt_pll_mpy);
	if (err) {
		dev_err(dev, "%s: unable to set IVT_PLL_MPY\n", __func__);
		return err;
	}

	return 0;
}

static int imx577_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx577 *priv = (struct imx577 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	/* Update Black level V4L control*/
	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12)
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 >> 4),
						IMX577_DEFAULT_BLACK_LEVEL_8BPP,
						0, IMX577_MAX_BLACK_LEVEL_8BPP);
			else
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						(*ctrl->p_cur.p_s64 >> 2),
						IMX577_DEFAULT_BLACK_LEVEL_8BPP,
						0, IMX577_MAX_BLACK_LEVEL_8BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB8_1X8;
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12)
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 >> 2),
						IMX577_DEFAULT_BLACK_LEVEL_10BPP,
						0, IMX577_MAX_BLACK_LEVEL_10BPP);
			else
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 2),
						IMX577_DEFAULT_BLACK_LEVEL_10BPP,
						0, IMX577_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB10_1X10)
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 2),
						IMX577_DEFAULT_BLACK_LEVEL_12BPP,
						0, IMX577_MAX_BLACK_LEVEL_12BPP);
			else
				err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 4),
						IMX577_DEFAULT_BLACK_LEVEL_12BPP,
						0, IMX577_MAX_BLACK_LEVEL_12BPP);
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

static int imx577_set_mode(struct tegracam_device *tc_dev)
{
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx577_write_table(priv, mode_table[IMX577_GLOBAL_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to set global settings\n", __func__);
		return err;
	}

	err = imx577_write_table(priv, mode_table[IMX577_IMAGEQUALITY_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to set image quality settings\n", __func__);
		return err;
	}

	err = imx577_write_table(priv, mode_table[IMX577_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n", __func__);
		return err;
	}

	switch (s_data->mode) {
	case IMX577_MODE_CROP_BINNING_H2V2:
		err = imx577_write_table(priv, mode_table[IMX577_OTHER_SETTINGS_BINNING]);
		break;
	default:
		err = imx577_write_table(priv, mode_table[IMX577_OTHER_SETTINGS]);
	}
	if (err) {
		dev_err(dev, "%s: unable to set other settings\n", __func__);
		return err;
	}

	err = imx577_set_ivtck(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to set clock settings\n", __func__);
		return err;
	}

	err = imx577_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = imx577_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n", __func__);
		return err;
	}

	err = imx577_write_table(priv, mode_table[s_data->mode]);
	if (err)
		return err;

	err = imx577_set_operation_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}

	err = imx577_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx577_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx577_adjust_hmax_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust hmax\n", __func__);
		return err;
	}

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	err = imx577_calculate_line_time(tc_dev);
	if (err)
		return err;

	err = imx577_update_framerate_range(tc_dev);
	if (err)
		return err;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;
}

static int imx577_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
		err = max96793_setup_streaming(priv->s_data->ser_dev, s_data);
		if (err)
			goto exit;
		err = max96792_setup_streaming(priv->s_data->dser_dev, dev, s_data);
		if (err)
			goto exit;
		err = max96792_start_streaming(priv->s_data->dser_dev, dev);
		if (err)
			goto exit;
	}

	err = imx577_write_table(priv, mode_table[IMX577_MODE_START_STREAM]);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx577_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx577 *priv = (struct imx577 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx577_write_table(priv, mode_table[IMX577_MODE_STOP_STREAM]);
	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX577_K_FACTOR,
		priv->frame_length * priv->line_time / IMX577_K_FACTOR + 1000);

	return 0;
}


static struct camera_common_sensor_ops imx577_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx577_frmfmt),
	.frmfmt_table = imx577_frmfmt,
	.power_on = imx577_power_on,
	.power_off = imx577_power_off,
	.write_reg = imx577_write_reg,
	.read_reg = imx577_read_reg,
	.parse_dt = imx577_parse_dt,
	.power_get = imx577_power_get,
	.power_put = imx577_power_put,
	.set_mode = imx577_set_mode,
	.start_streaming = imx577_start_streaming,
	.stop_streaming = imx577_stop_streaming,
	.check_unsupported_mode = imx577_check_unsupported_mode,
	.after_set_pixel_format = imx577_after_set_pixel_format,
};

static int imx577_gmsl_serdes_setup(struct imx577 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev || !priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	dev_dbg(dev, "%s:++\n", __func__);

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
		dev_err(dev, "gmsl serializer gpio10/xtrig1 pin config failed\n");
		goto error;
	}

	dev_dbg(dev, "%s: max96792_setup_control\n", __func__);
	des_err = max96792_setup_control(priv->s_data->dser_dev, &priv->i2c_client->dev);
	if (des_err) {
		dev_err(dev, "gmsl deserializer setup failed\n");
		err = des_err;
	}

error:
	mutex_unlock(&serdes_lock__);
	return err;
}

static void imx577_gmsl_serdes_reset(struct imx577 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx577_board_setup(struct imx577 *priv)
{
	struct camera_common_data *s_data = priv->s_data;
	struct device *dev = s_data->dev;
	struct device_node *node = dev->of_node;
	struct device_node *ser_node;
	struct i2c_client *ser_i2c = NULL;
	struct device_node *dser_node;
	struct i2c_client *dser_i2c = NULL;
	struct device_node *gmsl;
	int value = 0xFFFF;
	const char *str_value;
	const char *str_value1[2];
	int i;
	int err = 0;

	dev_dbg(dev, "%s++\n", __func__);
	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
		err = of_property_read_u32(node, "reg", &priv->s_data->g_ctx.sdev_reg);
		if (err < 0) {
			dev_err(dev, "reg not found\n");
			return err;
		}

		priv->s_data->g_ctx.sdev_def = priv->s_data->g_ctx.sdev_reg;

		ser_node = of_parse_phandle(node, "nvidia,gmsl-ser-device", 0);
		if (ser_node == NULL) {
			dev_err(dev,
				"missing %s handle\n",
					"nvidia,gmsl-ser-device");
			return err;
		}

		err = of_property_read_u32(ser_node, "reg", &priv->s_data->g_ctx.ser_reg);
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

		dser_node = of_parse_phandle(node, "nvidia,gmsl-dser-device", 0);
		if (dser_node == NULL) {
			dev_err(dev,
				"missing %s handle\n",
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
		priv->s_data->g_ctx.dst_csi_port =
			(!strcmp(str_value, "a")) ? GMSL_CSI_PORT_A : GMSL_CSI_PORT_B;

		err = of_property_read_string(gmsl, "src-csi-port", &str_value);
		if (err < 0) {
			dev_err(dev, "No src-csi-port found\n");
			return err;
		}
		priv->s_data->g_ctx.src_csi_port =
			(!strcmp(str_value, "a")) ? GMSL_CSI_PORT_A : GMSL_CSI_PORT_B;

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

		err = of_property_read_string(gmsl, "serdes-csi-link", &str_value);
		if (err < 0) {
			dev_err(dev, "No serdes-csi-link found\n");
			return err;
		}
		priv->s_data->g_ctx.serdes_csi_link =
			(!strcmp(str_value, "a")) ?
				GMSL_SERDES_CSI_LINK_A : GMSL_SERDES_CSI_LINK_B;

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

		err = max96793_sdev_pair(priv->s_data->ser_dev, &priv->s_data->g_ctx);
		if (err) {
			dev_err(dev, "gmsl ser pairing failed\n");
			return err;
		}

		err = max96792_sdev_register(priv->s_data->dser_dev, &priv->s_data->g_ctx);
		if (err) {
			dev_err(dev, "gmsl deserializer register failed\n");
			return err;
		}

		err = imx577_gmsl_serdes_setup(priv);
		if (err) {
			dev_err(dev, "%s gmsl serdes setup failed\n", __func__);
			return err;
		}
	}

	err = camera_common_mclk_enable(s_data);
	if (err) {
		dev_err(dev,
			"Error %d turning on mclk\n", err);
		return err;
	}

	err = imx577_power_on(s_data);
	if (err) {
		dev_err(dev, "Error %d during power on sensor\n", err);
		return err;
	}

	err = imx577_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n", __func__);
		goto error2;
	}

	err = imx577_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

	priv->min_frame_length = IMX577_DEFAULT_HEIGHT
					+ IMX577_MIN_FRAME_LENGTH_DELTA;
error2:
	imx577_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx577_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx577_subdev_internal_ops = {
	.open = imx577_open,
};

static struct tegracam_ctrl_ops imx577_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx577_set_gain,
	.set_exposure = imx577_set_exposure,
	.set_frame_rate = imx577_set_frame_rate,
	.set_group_hold = imx577_set_group_hold,
	.set_test_pattern = imx577_set_test_pattern,
	.set_operation_mode = imx577_set_operation_mode,
	.set_broadcast_ctrl = imx577_set_broadcast_ctrl,
	.set_black_level = imx577_set_black_level,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx577_probe(struct i2c_client *client)
#else
static int imx577_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx577 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx577), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx577", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx577_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx577_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx577_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	priv->broadcast_ctrl = UNICAST;
	priv->s_data->broadcast_regmap = NULL;
	priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx577_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				IMX577_DEFAULT_BLACK_LEVEL_10BPP,
				IMX577_DEFAULT_BLACK_LEVEL_10BPP, 0,
				IMX577_MAX_BLACK_LEVEL_10BPP);
	if (err)
		return err;

	err = imx577_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				 (ARRAY_SIZE(imx577_test_pattern_menu)-1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx577_sensor_list);

	dev_info(dev, "Detected imx577 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx577_remove(struct i2c_client *client)
#else
static void imx577_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx577 *priv;

	if (!s_data) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx577 *)s_data->priv;

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		imx577_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		mutex_destroy(&serdes_lock__);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx577_id[] = {
	{ "imx577", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx577_id);

static struct i2c_driver imx577_i2c_driver = {
	.driver = {
		.name = "imx577",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx577_of_match),
	},
	.probe = imx577_probe,
	.remove = imx577_remove,
	.id_table = imx577_id,
};

module_i2c_driver(imx577_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX577");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
