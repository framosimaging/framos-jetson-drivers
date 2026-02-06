// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx838.c - Framos fr_imx838.c driver
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

#include "fr_imx838_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX838_K_FACTOR 1000LL
#define IMX838_M_FACTOR 1000000LL
#define IMX838_G_FACTOR 1000000000LL
#define IMX838_T_FACTOR 1000000000000LL

#define IMX838_MAX_GAIN_DEC 240
#define IMX838_MAX_GAIN_DB 72
#define IMX838_HIGH_GAIN_REG_MIN 34

#define IMX838_MAX_BLACK_LEVEL_10BPP 1023
#define IMX838_MAX_BLACK_LEVEL_12BPP 4095
#define IMX838_DEFAULT_BLACK_LEVEL_10BPP 50
#define IMX838_DEFAULT_BLACK_LEVEL_12BPP 200

#define IMX838_MIN_SHR0_LENGTH 8
#define IMX838_MIN_INTEGRATION_LINES 2

#define IMX838_4_CSI_LANES 4
#define IMX838_TWO_LANE_MODE 2

#define IMX838_INCK 74250000LL

LIST_HEAD(imx838_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx838_of_match[] = {
	{ .compatible = "framos,imx838",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx838_of_match);

const char * const imx838_data_rate_menu[] = {
	[IMX838_2376_MBPS] = "2376 Mbps/lane",
	[IMX838_2079_MBPS] = "2079 Mbps/lane",
	[IMX838_1782_MBPS] = "1782 Mbps/lane",
	[IMX838_1440_MBPS] = "1440 Mbps/lane",
	[IMX838_1188_MBPS] = "1188 Mbps/lane",
	[IMX838_891_MBPS] = "891 Mbps/lane",
	[IMX838_720_MBPS] = "720 Mbps/lane",
	[IMX838_594_MBPS] = "594 Mbps/lane",
};

static const char * const imx838_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "000h Pattern",
	[2] = "3FF(FFFh) Pattern",
	[3] = "155(555h) Pattern",
	[4] = "2AA(AAAh) Pattern",
	[5] = "555/AAAh Pattern",
	[6] = "AAA/555h Pattern",
	[7] = "000/555h Pattern",
	[8] = "555/000h Pattern",
	[9] = "000/FFFh Pattern",
	[10] = "FFF/000h Pattern",
	[11] = "H Color-bar",
	[12] = "V Color-bar",
};

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_GAIN,
	TEGRA_CAMERA_CID_CONVERSION_GAIN,
	TEGRA_CAMERA_CID_EXPOSURE,
	TEGRA_CAMERA_CID_FRAME_RATE,
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_OPERATION_MODE,
	TEGRA_CAMERA_CID_SYNC_MODE,
	TEGRA_CAMERA_CID_BROADCAST,
	TEGRA_CAMERA_CID_BLACK_LEVEL,
	TEGRA_CAMERA_CID_TEST_PATTERN,
	TEGRA_CAMERA_CID_DATA_RATE,
};

struct imx838 {
	struct i2c_client		*i2c_client;
	struct v4l2_subdev		*subdev;
	u64				frame_length;
	u64				min_frame_length;
	u64				current_pixel_format;
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

static bool imx838_is_binning_mode(struct camera_common_data *s_data)
{
	switch (s_data->mode) {
	case IMX838_MODE_H2V2_BINNING:
		return true;
	default:
		return false;
	}
}

static inline int imx838_read_reg(struct camera_common_data *s_data,
							u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx838_write_reg(struct camera_common_data *s_data,
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

static int imx838_write_reg_broadcast(struct camera_common_data *s_data,
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

static int imx838_read_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;
	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx838_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
								__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx838_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx838_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
								__func__);
			return err;
		}
	}

	return err;
}

static int imx838_write_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx838_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx838_write_reg(s_data, addr_low + i,
						(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx838_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx838_broadcast_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u32 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx838_write_reg_broadcast(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx838_write_reg_broadcast(s_data, addr_low + i,
			(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx838_write_reg_broadcast(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx838_write_table(struct imx838 *priv, const imx838_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					 table,
					 NULL, 0,
					 IMX838_TABLE_WAIT_MS,
					 IMX838_TABLE_END);
}

static int imx838_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx838_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx838_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
								u64 max_val)
{
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct v4l2_ctrl *ctrl;

	ctrl = fr_find_v4l2_ctrl(tc_dev, ctrl_id);
	if (ctrl) {
		switch (ctrl->id) {
		case TEGRA_CAMERA_CID_BLACK_LEVEL:
			*ctrl->p_new.p_s64 = current_val;
			*ctrl->p_cur.p_s64 = current_val;
			ctrl->default_value = default_val;
			priv->s_data->blklvl_max_range = max_val;
			break;
		case TEGRA_CAMERA_CID_TEST_PATTERN:
			ctrl->qmenu = imx838_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		case TEGRA_CAMERA_CID_DATA_RATE:
			ctrl->qmenu = imx838_data_rate_menu;
			ctrl->maximum = max_val;
			break;
		}
	}

	return 0;
}

static int imx838_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	s64 black_level_reg;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
		black_level_reg = val;
	else
		black_level_reg = val >> 2;

	err = imx838_write_buffered_reg(s_data, BLKLEVEL_LOW, 2,
							black_level_reg);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx838_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err = 0;
	u32 gain;

	gain = val * IMX838_MAX_GAIN_DEC /
				(IMX838_MAX_GAIN_DB *
					mode->control_properties.gain_factor);


	if (priv->broadcast_ctrl == BROADCAST)
		err = imx838_broadcast_buffered_reg(s_data,
							GAIN_LOW, 2, gain);
	else {
		err = imx838_write_buffered_reg(s_data,
							GAIN_LOW, 2, gain);
	}

	if (err) {
		dev_err(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx838_set_conversion_gain(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u64 curr_gain = 0;
	u64 min_high_gain = 0;

	dev_dbg(dev, "%s: Enter conv gain: %d\n", __func__, val);
	err = imx838_write_reg(s_data, FDG_SEL0, val);
	if (err) {
		dev_err(dev, "%s: changing conversion gain error\n", __func__);
		return err;
	}

	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_GAIN);
	if (!ctrl) {
		dev_err(dev, "%s: Unable to find gain control\n", __func__);
	}

	// minimal gain value is higher according to sensor datasheet
	if (val) {
		err = imx838_read_buffered_reg(s_data, GAIN_LOW, 2, &curr_gain);
		if (err) {
			dev_err(dev, "%s: reading gain value error\n", __func__);
			return err;
		}
		min_high_gain = IMX838_HIGH_GAIN_REG_MIN * IMX838_MAX_GAIN_DB
			* mode->control_properties.gain_factor / IMX838_MAX_GAIN_DEC;

		if (curr_gain < IMX838_HIGH_GAIN_REG_MIN) {
			dev_warn(dev, "%s:Gain value too large for high conversion gain\n", __func__);
			curr_gain = IMX838_HIGH_GAIN_REG_MIN * IMX838_MAX_GAIN_DB
				* mode->control_properties.gain_factor
				/ IMX838_MAX_GAIN_DEC;
			err = imx838_set_gain(tc_dev, curr_gain);
			if (err) {
				dev_err(dev, "%s:Error changing gain value\n", __func__);
				return err;
			}
			*ctrl->p_new.p_s64 = curr_gain;
			*ctrl->p_cur.p_s64 = curr_gain;
			dev_dbg(tc_dev->dev, "%s: changing gain range\n", __func__);
		}
		ctrl->default_value = min_high_gain;
		ctrl->minimum = min_high_gain;
	} else {
		ctrl->minimum = mode->control_properties.min_gain_val;
		ctrl->default_value = mode->control_properties.default_gain;
	}

	return 0;
}

static int imx838_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u32 integration_time_line;
	u64 shr0;
	u64 max_shr0;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line = (val * IMX838_K_FACTOR) / priv->line_time;
	shr0 = priv->frame_length - integration_time_line;
	max_shr0 = priv->frame_length - IMX838_MIN_INTEGRATION_LINES;

	if (shr0 < IMX838_MIN_SHR0_LENGTH)
		shr0 = IMX838_MIN_SHR0_LENGTH;
	else if (shr0 > max_shr0)
		shr0 = max_shr0;

	if (priv->broadcast_ctrl == BROADCAST)
		err = imx838_broadcast_buffered_reg(s_data, SHR0_LOW, 3, shr0);
	else
		err = imx838_write_buffered_reg(s_data, SHR0_LOW, 3, shr0);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	/* Update new ctrl value */
	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_EXPOSURE);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	dev_dbg(dev,
		"%s: set integration time: %lld [us], integration time:%d [line], shr0: %llu [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, shr0, priv->frame_length);

	return err;
}

static int imx838_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;

	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length = (((u64)mode->control_properties.framerate_factor *
				IMX838_G_FACTOR) / (val * priv->line_time));

	/* Value must be multiple of 2 */
	frame_length = (frame_length % 2) ? frame_length + 1 : frame_length;
	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;
	priv->frame_length = frame_length;

	/* Update exposure range, before writing the new frame length */
	exposure_min_range = IMX838_MIN_INTEGRATION_LINES
					    * priv->line_time / IMX838_K_FACTOR;

	exposure_max_range = (priv->frame_length - IMX838_MIN_INTEGRATION_LINES)
					    * priv->line_time / IMX838_K_FACTOR;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
			     exposure_min_range, exposure_max_range);

	if (priv->broadcast_ctrl == BROADCAST)
		err = imx838_broadcast_buffered_reg(s_data,
					VMAX_LOW, 3, priv->frame_length);
	else
		err = imx838_write_buffered_reg(s_data,
					VMAX_LOW, 3, priv->frame_length);

	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev,
		"%s: val: %lld, frame_length set: %llu\n",
		__func__, val, priv->frame_length);

	return 0;
}

static int imx838_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err = imx838_write_table(priv, mode_table[IMX838_EN_PATTERN_GEN]);
		if (err)
			goto fail;
		err = imx838_write_reg(s_data, TPG_PATSEL_DUOUT, (u8)(val - 1));
		if (err)
			goto fail;
	} else {
		err = imx838_write_table(priv, mode_table[IMX838_DIS_PATTERN_GEN]);
		if (err)
			goto fail;
	}

	dev_dbg(dev, "%s++ Test mode pattern: %u\n", __func__, val-1);

	return 0;
fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx838_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	struct device *dev = tc_dev->dev;
	u64 max_framerate;
	u64 frame_height = s_data->fmt_height;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	if (imx838_is_binning_mode(s_data))
		priv->min_frame_length = frame_height * 2
						+ IMX838_MIN_FRAME_LENGTH_DELTA;
	else
		priv->min_frame_length = frame_height
						+ IMX838_MIN_FRAME_LENGTH_DELTA;

	max_framerate = (IMX838_G_FACTOR * IMX838_M_FACTOR) /
				(priv->min_frame_length * priv->line_time);

	dev_dbg(dev, "%s: Max framerate is equal to %llu\n",
						__func__, max_framerate);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				ctrlprops->min_framerate, max_framerate);

	dev_dbg(tc_dev->dev, "%s min_framerate: %llu, max_framerate: %llu\n",
			__func__, priv->min_frame_length, max_framerate);

	return 0;
}

static int imx838_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_pdata *pdata = s_data->pdata;

	if (pdata->xmaster_gpio)
		fr_gpio_set(s_data, pdata->xmaster_gpio, val);

	return 0;
}

static bool imx838_find_broadcast_sensor(struct imx838 *broadcast_private)
{
	struct imx838 *current_private;

	list_for_each_entry(current_private, &imx838_sensor_list, entry) {
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

static void imx838_enable_second_slave_address(struct imx838 *ack_private)
{
	struct imx838 *current_private;
	int err;

	list_for_each_entry(current_private, &imx838_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);

		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx838_write_reg(current_private->s_data,
					SECOND_SLAVE_ADD, 1);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				"%s: Fail to write Second I2C register\n",
								__func__);

	mutex_unlock(&current_private->pw_mutex);

	dev_dbg(&current_private->i2c_client->dev,
		"%s: Sensors 2nd slave address configured\n", __func__);
	}

	err = imx838_write_reg(ack_private->s_data, SECOND_SLAVE_ADD, 3);
	if (err)
		dev_warn(&ack_private->i2c_client->dev,
			"%s: Fail to write Second I2C register\n", __func__);

	dev_dbg(&ack_private->i2c_client->dev,
		": Sensors 2nd slave address configured with acknowlege\n");
}

static void imx838_disable_second_slave_address(void)
{
	struct imx838 *current_private;
	int err;

	list_for_each_entry(current_private, &imx838_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);

		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx838_write_reg(current_private->s_data,
					SECOND_SLAVE_ADD, 0);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				"%s: Fail to write Second I2C register\n",
								__func__);

		mutex_unlock(&current_private->pw_mutex);

		dev_dbg(&current_private->i2c_client->dev,
				"%s: Sensors 2nd slave address disabled\n",
								__func__);
	}
}

static void imx838_configure_second_slave_address(void)
{
	struct imx838 broadcast_private = {};

	if (imx838_find_broadcast_sensor(&broadcast_private))
		imx838_enable_second_slave_address(&broadcast_private);
	else
		imx838_disable_second_slave_address();
}

static int imx838_set_broadcast_ctrl(struct tegracam_device *tc_dev,
						struct v4l2_ctrl *ctrl)
{
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
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
	imx838_configure_second_slave_address();

	return 0;
}

static int imx838_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx838 *priv = (struct imx838 *)s_data->priv;

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
		goto imx838_mclk_fail;
	}

	usleep_range(1, 2);

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pw->reset_gpio)
			fr_gpio_set(s_data, pw->reset_gpio, 1);
	} else {
		dev_dbg(dev, "%s: max96792_power_on\n", __func__);
		max96792_power_on(priv->s_data->dser_dev, &priv->s_data->g_ctx);
	}

	err = clk_prepare_enable(pw->mclk);
	if (err) {
		dev_err(dev, "%s: failed to enable mclk\n", __func__);
		return err;
	}

	pw->state = SWITCH_ON;

	/* Additional sleep required in the case of hardware power-on sequence */
	usleep_range(40000, 41000);

	mutex_unlock(&priv->pw_mutex);

	imx838_configure_second_slave_address();

	return 0;

imx838_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx838_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx838 *priv = (struct imx838 *)s_data->priv;
	int err = 0;

	dev_dbg(dev, "%s: power off\n", __func__);

	err = imx838_write_reg(s_data, XVS_XHS_DRV, 0xF);
	if (err)
		dev_err(dev, "%s: error setting XVS XHS to Hi-Z\n", __func__);


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

static int imx838_power_get(struct tegracam_device *tc_dev)
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

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		pw->reset_gpio = pdata->reset_gpio;

		if (pdata->use_cam_gpio) {
			err = cam_gpio_register(dev, pw->reset_gpio);
			if (err)
				dev_err(dev, "%s ERR can't register cam gpio %u!\n",
						__func__, pw->reset_gpio);
		}
	}


	pw->state = SWITCH_OFF;

	return err;
}

static int imx838_power_put(struct tegracam_device *tc_dev)
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

static int imx838_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx838_read_buffered_reg(s_data, VMAX_LOW, 2, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}

	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *imx838_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx838_of_match, dev);
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

		gpio = of_get_named_gpio(i2c_mux_ch_node, "xmaster-gpio", 0);
		if (gpio < 0)
			dev_warn(dev, "xmaster-gpio not found\n");
		else
			board_priv_pdata->xmaster_gpio = (unsigned int)gpio;

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

static int imx838_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx838_write_table(priv, mode_table[IMX838_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx838_write_table(priv, mode_table[IMX838_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx838_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == IMX838_TWO_LANE_MODE) {
		err = imx838_write_reg(s_data, LANEMODE, 1);
		if (err) {
			dev_err(dev, "%s: error setting two lane mode\n",
								__func__);
			return err;
		}
	}

	dev_dbg(dev, "%s: sensor is in %d CSI lane mode\n",
						__func__, s_data->numlanes);

	return 0;
}

static int imx838_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx838 *priv = (struct imx838 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx838_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax*IMX838_G_FACTOR) / (IMX838_INCK);

	dev_dbg(dev, "%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

static int imx838_adjust_hmax_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 hmax = 1100;
	u8 csi_lane_coef = IMX838_4_CSI_LANES / s_data->numlanes;

	dev_dbg(dev, "%s:++\n", __func__);

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
								CURRENT)) {
	case IMX838_2376_MBPS:
		hmax = 458;
		break;
	case IMX838_2079_MBPS:
		hmax = (s_data->numlanes == IMX838_4_CSI_LANES) ? 458 : 1100;
		break;
	case IMX838_1782_MBPS:
		if (s_data->numlanes == IMX838_4_CSI_LANES)
			hmax = 550;
		else
			hmax = (s_data->colorfmt->code ==
				MEDIA_BUS_FMT_SRGGB10_1X10) ? 1100 : 1320;
		break;
	case IMX838_1440_MBPS:
		if (imx838_is_binning_mode(s_data))
			hmax = 550 * csi_lane_coef;
		else {
			if (s_data->numlanes == IMX838_TWO_LANE_MODE)
				hmax = 1320;
			else
				hmax = (s_data->colorfmt->code ==
					MEDIA_BUS_FMT_SRGGB10_1X10) ? 550 : 660;
		}
		break;
	case IMX838_1188_MBPS:
		hmax = 1100;
		break;
	case IMX838_891_MBPS:
		hmax = (s_data->colorfmt->code ==
					MEDIA_BUS_FMT_SRGGB10_1X10) ? 1100 : 1320;
		break;
	case IMX838_720_MBPS:
		if (imx838_is_binning_mode(s_data))
			hmax = 1100;
		else
			hmax = (s_data->colorfmt->code ==
					MEDIA_BUS_FMT_SRGGB10_1X10) ? 1100 : 1320;
		break;
	default:
		/* Adjusment isn't needed - this should never happen */
		dev_err(dev, "%s: failed to adjust HMAX register\n", __func__);
		return 0;
	}

	err = imx838_write_buffered_reg(s_data, HMAX_LOW, 2, hmax);
	if (err) {
		dev_err(dev, "%s: failed to set HMAX register\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: HMAX: %llu\n", __func__, hmax);

	return 0;
}

static int imx838_verify_data_rate(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl = fr_find_v4l2_ctrl(tc_dev,
						TEGRA_CAMERA_CID_DATA_RATE);
	u8 data_rate;
	s64 current_data_rate;

	dev_dbg(dev, "%s++\n", __func__);

	if (ctrl == NULL)
		return 0;

	current_data_rate = fr_get_v4l2_ctrl_value(tc_dev,
					TEGRA_CAMERA_CID_DATA_RATE, CURRENT);

	if (s_data->numlanes == IMX838_TWO_LANE_MODE) {
		if (imx838_is_binning_mode(s_data)) {
			if (current_data_rate != IMX838_1440_MBPS) {
				dev_warn(dev,
					"%s: Selected data rate is not supported with 2 CSI lane mode and binning mode, switching to default!\n",
					__func__);
				data_rate = IMX838_1440_MBPS;
				goto modify_ctrl;
			} else {
				return 0;
			}
		} else {
			switch (current_data_rate) {
			case IMX838_2376_MBPS:
			case IMX838_1188_MBPS:
			case IMX838_891_MBPS:
			case IMX838_720_MBPS:
			case IMX838_594_MBPS:
				dev_warn(dev,
					"%s: Selected data rate is not supported with 2 CSI lane mode, switching to default!\n",
					__func__);
				if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB10_1X10)
					data_rate = IMX838_1782_MBPS;
				else
					data_rate = IMX838_2079_MBPS;
				goto modify_ctrl;
			case IMX838_2079_MBPS:
				if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB10_1X10) {
					data_rate = IMX838_1782_MBPS;
					goto modify_ctrl;
				}
				break;
			case IMX838_1440_MBPS:
				if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB12_1X12) {
					data_rate = IMX838_2079_MBPS;
					goto modify_ctrl;
				}
			break;
			}
		}
	} else {
		if (imx838_is_binning_mode(s_data)) {
			switch (current_data_rate) {
			case IMX838_2079_MBPS:
			case IMX838_1782_MBPS:
			case IMX838_1188_MBPS:
			case IMX838_891_MBPS:
				data_rate = IMX838_2376_MBPS;
				goto modify_ctrl;
			default:
				return 0;
			}
		} else {
			switch (current_data_rate) {
			case IMX838_2376_MBPS:
				dev_warn(dev,
					"%s: Selected data rate is not supported with 4 lane mode, switching to default!\n",
					__func__);
				if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB10_1X10)
					data_rate = IMX838_2079_MBPS;
				else
					data_rate = IMX838_1782_MBPS;
				goto modify_ctrl;
			case IMX838_2079_MBPS:
			case IMX838_720_MBPS:
				if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB12_1X12) {
					data_rate = IMX838_1782_MBPS;
					goto modify_ctrl;
				}
				break;
			case IMX838_1782_MBPS:
			if (s_data->colorfmt->code ==
						MEDIA_BUS_FMT_SRGGB10_1X10) {
				data_rate = IMX838_2079_MBPS;
				goto modify_ctrl;
			}
			break;
			}
		}
	}

	return 0;

modify_ctrl:
	dev_warn(dev,
		"%s: Selected data rate is not supported in this mode, switching to data rate %d mode !\n",
		__func__, data_rate);
	*ctrl->p_new.p_s64 = data_rate;
	*ctrl->p_cur.p_s64 = data_rate;
	return 0;
}

static int imx838_set_data_rate(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx838_verify_data_rate(tc_dev);
	if (err)
		goto fail;

	if (s_data->power->state == SWITCH_ON) {
		err = imx838_write_reg(s_data, DATARATE_SEL,
				fr_get_v4l2_ctrl_value(tc_dev,
					TEGRA_CAMERA_CID_DATA_RATE, CURRENT));
		if (err)
			goto fail;
	} else {
		dev_dbg(dev, "%s: Power is switch off, data rate not set.\n",
								__func__);
	}

	dev_dbg(dev, "%s: Data rate: %llu\n", __func__,
		fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
								CURRENT));

	return 0;

fail:
	dev_err(dev, "%s: unable to set data rate\n", __func__);
	return err;
}

static int imx838_set_sync_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 extmode;

	if (val == EXTERNAL_SYNC)
		extmode = 1;
	else
		extmode = 0;

	if (s_data->power->state == SWITCH_ON) {
		err = imx838_write_reg(s_data, EXTMODE, extmode);
		if (err)
			dev_err(dev, "%s: error setting sync mode\n", __func__);
	}

	return err;
}

static int imx838_configure_triggering_pins(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 xvs_xhs_drv = 0xF;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
								CURRENT)) {
	case MASTER_MODE:
		if ((fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SYNC_MODE,
						CURRENT)) == INTERNAL_SYNC) {
			xvs_xhs_drv = 0x0;
			dev_dbg(dev,
				"%s: Sensor is in - Internal sync Master mode\n",
								__func__);
		} else if ((fr_get_v4l2_ctrl_value(tc_dev,
			TEGRA_CAMERA_CID_SYNC_MODE, CURRENT)) == EXTERNAL_SYNC) {
			xvs_xhs_drv = 0x3;
			dev_dbg(dev,
				"%s: Sensor is in - External sync Master mode\n",
								__func__);
		} else {
			xvs_xhs_drv = 0xF;
			dev_dbg(dev,
				"%s: Sensor is in - No sync Master mode\n",
								__func__);
		}
		break;
	case SLAVE_MODE:
		xvs_xhs_drv = 0xF;
		dev_dbg(dev, "%s: Sensor is in Slave mode\n", __func__);
		break;

	default:
		pr_err("%s: unknown synchronizing function.\n", __func__);
		return -EINVAL;
	}

	err = imx838_write_reg(s_data, XVS_XHS_DRV, xvs_xhs_drv);
	if (err) {
		dev_err(dev, "%s: error setting Slave mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__, xvs_xhs_drv);

	return 0;
}

/*
 * According to the V4L2 documentation, driver should not return error when
 * invalid settings are detected.
 * It should apply the settings closest to the ones that the user has requested.
 */
static int imx838_check_unsupported_mode(struct camera_common_data *s_data,
					struct v4l2_mbus_framefmt *mf)
{
	struct device *dev = s_data->dev;
	bool unsupported_mode = false;

	dev_dbg(dev, "%s++\n", __func__);

	if (mf->code == MEDIA_BUS_FMT_SRGGB10_1X10
				&& imx838_is_binning_mode(s_data)) {
		unsupported_mode = true;
		dev_warn(dev,
			"%s: selected mode is not supported with RAW10, switching to default\n",
			__func__);
	}

	if (unsupported_mode) {
		mf->width = s_data->frmfmt[s_data->def_mode].size.width;
		mf->height = s_data->frmfmt[s_data->def_mode].size.height;
	}

	return 0;
}

static int imx838_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx838 *priv = (struct imx838 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	/* Update Black level V4L control*/
	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			err = imx838_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						(*ctrl->p_cur.p_s64 >> 2),
						IMX838_DEFAULT_BLACK_LEVEL_10BPP,
						0, IMX838_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			err = imx838_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						(*ctrl->p_cur.p_s64 << 2),
						IMX838_DEFAULT_BLACK_LEVEL_12BPP,
						0, IMX838_MAX_BLACK_LEVEL_12BPP);
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

static int imx838_set_mode(struct tegracam_device *tc_dev)
{
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s: setting mode %u\n", __func__, s_data->mode);

	err = imx838_write_table(priv, mode_table[IMX838_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
		return err;
	}

	err = imx838_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = imx838_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
								__func__);
		return err;
	}

	err = imx838_write_table(priv, mode_table[s_data->mode]);

	if (err) {
		dev_err(dev, "%s: unable to write table for mode %u\n", __func__,
								s_data->mode);
		return err;
	}

	err = imx838_set_operation_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}

	err = imx838_set_sync_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_SYNC_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set sync mode\n", __func__);
		return err;
	}

	err = imx838_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx838_set_data_rate(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_DATA_RATE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set data rate\n", __func__);
		return err;
	}

	err = imx838_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx838_adjust_hmax_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust hmax\n", __func__);
		return err;
	}

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	err = imx838_calculate_line_time(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		return err;
	}

	err = imx838_update_framerate_range(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to update frame range\n", __func__);
		return err;
	}
	dev_dbg(dev, "%s: set mode %u succesfully\n", __func__, s_data->mode);

	return 0;
}

static int imx838_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 xmsta;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {
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

	err = imx838_write_table(priv, mode_table[IMX838_MODE_START_STREAM]);
	if (err)
		goto exit;

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
							CURRENT) == MASTER_MODE)
		xmsta = 0x00;
	else
		xmsta = 0x01;

	err = imx838_write_reg(s_data, XMSTA, xmsta);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx838_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx838 *priv = (struct imx838 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {
		/* disable serdes streaming */
		max96792_stop_streaming(priv->s_data->dser_dev, dev);
	}

	err = imx838_write_table(priv, mode_table[IMX838_MODE_STOP_STREAM]);
	if (err)
		return err;
	usleep_range(priv->frame_length * priv->line_time / IMX838_K_FACTOR,
		priv->frame_length * priv->line_time / IMX838_K_FACTOR + 1000);

	return 0;
}

static struct camera_common_sensor_ops imx838_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx838_frmfmt),
	.frmfmt_table = imx838_frmfmt,
	.power_on = imx838_power_on,
	.power_off = imx838_power_off,
	.write_reg = imx838_write_reg,
	.read_reg = imx838_read_reg,
	.parse_dt = imx838_parse_dt,
	.power_get = imx838_power_get,
	.power_put = imx838_power_put,
	.set_mode = imx838_set_mode,
	.start_streaming = imx838_start_streaming,
	.stop_streaming = imx838_stop_streaming,
	.check_unsupported_mode = imx838_check_unsupported_mode,
	.after_set_pixel_format = imx838_after_set_pixel_format,
};

static int imx838_gmsl_serdes_setup(struct imx838 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev || !priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	dev_dbg(dev, "%s++\n", __func__);

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

static void imx838_gmsl_serdes_reset(struct imx838 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx838_board_setup(struct imx838 *priv)
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

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {

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

		err = imx838_gmsl_serdes_setup(priv);
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

	err = imx838_power_on(s_data);
	if (err) {
		dev_err(dev,
		"Error %d during power on sensor\n", err);
		return err;
	}

	err = imx838_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
								__func__);
		goto error2;
	}

	err = imx838_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

	priv->min_frame_length = IMX838_DEFAULT_HEIGHT +
						IMX838_MIN_FRAME_LENGTH_DELTA;

error2:
	imx838_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx838_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx838_subdev_internal_ops = {
	.open = imx838_open,
};

static struct tegracam_ctrl_ops imx838_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx838_set_gain,
	.set_conversion_gain = imx838_set_conversion_gain,
	.set_exposure = imx838_set_exposure,
	.set_frame_rate = imx838_set_frame_rate,
	.set_group_hold = imx838_set_group_hold,
	.set_test_pattern = imx838_set_test_pattern,
	.set_data_rate = imx838_set_data_rate,
	.set_operation_mode = imx838_set_operation_mode,
	.set_sync_mode = imx838_set_sync_mode,
	.set_broadcast_ctrl = imx838_set_broadcast_ctrl,
	.set_black_level = imx838_set_black_level,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx838_probe(struct i2c_client *client)
#else
static int imx838_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx838 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx838), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx838", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx838_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx838_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx838_ctrl_ops;

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
	priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB12_1X12;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx838_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx838_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					IMX838_DEFAULT_BLACK_LEVEL_12BPP,
					IMX838_DEFAULT_BLACK_LEVEL_12BPP, 0,
					IMX838_MAX_BLACK_LEVEL_12BPP);
	if (err)
		return err;

	err = imx838_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				(ARRAY_SIZE(imx838_test_pattern_menu)-1));
	if (err)
		return err;

	err = imx838_update_ctrl(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, 0, 0, 0,
				(ARRAY_SIZE(imx838_data_rate_menu)-1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx838_sensor_list);

	dev_info(dev, "Detected imx838 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx838_remove(struct i2c_client *client)
#else
static void imx838_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx838 *priv;

	if (!s_data) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx838 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx838_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);


#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx838_id[] = {
	{ "imx838", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx838_id);

static struct i2c_driver imx838_i2c_driver = {
	.driver = {
		.name = "imx838",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx838_of_match),
	},
	.probe = imx838_probe,
	.remove = imx838_remove,
	.id_table = imx838_id,
};

module_i2c_driver(imx838_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX838");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
