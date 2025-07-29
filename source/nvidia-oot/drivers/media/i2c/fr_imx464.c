// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx464.c - Framos fr_imx464.c driver
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

#include "fr_imx464_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX464_K_FACTOR				1000LL
#define IMX464_M_FACTOR				1000000LL
#define IMX464_G_FACTOR				1000000000LL
#define IMX464_T_FACTOR				1000000000000LL

#define IMX464_MAX_GAIN_DEC			240
#define IMX464_MAX_GAIN_DB			72

#define IMX464_MAX_BLACK_LEVEL_10BPP		1023
#define IMX464_MAX_BLACK_LEVEL_12BPP		4095
#define IMX464_DEFAULT_BLACK_LEVEL_10BPP	50
#define IMX464_DEFAULT_BLACK_LEVEL_12BPP	200

#define IMX464_MIN_SHR0_LENGTH			3
#define IMX464_MIN_INTEGRATION_LINES		1

#define IMX464_MAX_CSI_LANES			4
#define IMX464_TWO_LANE_MODE			2

#define IMX464_INCK				74250000LL

#define IMX464_DOL2_MIN_RHS1			(IMX464_DOL2_MIN_SHR1 + 4)
#define IMX464_OB_SIZE				19
#define IMX464_EMBD_SIZE			1
#define IMX464_DOL2_BRL_FULL_FRAME_MODE		1558
#define IMX464_DOL2_MIN_SEF_INTEGRATION_LINES	2
#define IMX464_DOL2_MIN_SHR0_LENGTH		(IMX464_DOL2_MIN_RHS1 + 9)
#define IMX464_DOL2_MIN_INTEGRATION_LINES	2

LIST_HEAD(imx464_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx464_of_match[] = {
	{
	.compatible = "framos,imx464",
	},
	{},
};
MODULE_DEVICE_TABLE(of, imx464_of_match);

const char *const imx464_data_rate_menu[] = {
	[IMX464_1188_DATA_RATE] = "1188 Mbps/lane",
	[IMX464_891_DATA_RATE] = "891 Mbps/lane",
};

static const char *const imx464_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "000h Pattern",
	[2] = "FFFh Pattern",
	[3] = "555h Pattern",
	[4] = "AAAh Pattern",
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
	TEGRA_CAMERA_CID_EXPOSURE,
	TEGRA_CAMERA_CID_EXPOSURE_SHORT,
	TEGRA_CAMERA_CID_FRAME_RATE,
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_OPERATION_MODE,
	TEGRA_CAMERA_CID_SYNC_MODE,
	TEGRA_CAMERA_CID_BROADCAST,
	TEGRA_CAMERA_CID_BLACK_LEVEL,
	TEGRA_CAMERA_CID_TEST_PATTERN,
	TEGRA_CAMERA_CID_DATA_RATE,
	TEGRA_CAMERA_CID_HDR_EN,
};

struct imx464 {
	struct i2c_client *i2c_client;
	struct v4l2_subdev *subdev;
	u64 frame_length;
	u64 min_frame_length;
	u64 rhs1_n;
	u64 rhs1_max;
	u64 shr0;
	u64 brl;
	u64 current_pixel_format;
	u32 line_time;
	i2c_broadcast_ctrl broadcast_ctrl;
	struct mutex pw_mutex;
	struct list_head entry;
	struct camera_common_data *s_data;
	struct tegracam_device *tc_dev;
};

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
	.use_single_read = true,
	.use_single_write = true,
};

static inline int imx464_read_reg(struct camera_common_data *s_data, u16 addr,
				u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx464_write_reg(struct camera_common_data *s_data, u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "%s: i2c write failed, 0x%x = %x\n", __func__,
			addr, val);

	return err;
}

static int imx464_write_reg_broadcast(struct camera_common_data *s_data,
					u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->broadcast_regmap, addr, val);
	if (err) {
		dev_err(dev, "%s: i2c write failed, %x = %x\n", __func__, addr,
			val);
	}

	return err;
}

static int imx464_read_buffered_reg(struct camera_common_data *s_data,
					u16 addr_low, u8 number_of_registers,
					u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;

	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx464_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
				__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx464_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx464_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
				__func__);
			return err;
		}
	}

	return err;
}

static int imx464_write_buffered_reg(struct camera_common_data *s_data,
					 u16 addr_low, u8 number_of_registers,
					 u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx464_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx464_write_reg(s_data, addr_low + i,
					(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx464_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx464_broadcast_buffered_reg(struct camera_common_data *s_data,
					 u16 addr_low, u8 number_of_registers,
					 u32 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx464_write_reg_broadcast(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx464_write_reg_broadcast(s_data, addr_low + i,
						 (u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx464_write_reg_broadcast(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx464_write_table(struct imx464 *priv, const imx464_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap, table, NULL, 0,
					 IMX464_TABLE_WAIT_MS,
					 IMX464_TABLE_END);
}

static int imx464_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx464_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx464_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
				u64 max_val)
{
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
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
			ctrl->qmenu = imx464_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		case TEGRA_CAMERA_CID_DATA_RATE:
			ctrl->qmenu = imx464_data_rate_menu;
			ctrl->maximum = max_val;
			break;
		}
	}
	return 0;
}

static bool imx464_in_dol_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_control control;
	bool hdr_en = false;
	int err;

	control.id = TEGRA_CAMERA_CID_HDR_EN;
	err = camera_common_g_ctrl(s_data, &control);
	if (err < 0) {
		dev_err(dev, "could not find device ctrl.\n");
		return err;
	}
	hdr_en = switch_ctrl_qmenu[control.value];

	return hdr_en;
}

static int imx464_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	s64 black_level_reg;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
		black_level_reg = val;
	else
		black_level_reg = val >> 2;

	err =
		imx464_write_buffered_reg(s_data, BLKLEVEL_LOW, 2, black_level_reg);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx464_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err;
	u32 gain;

	gain = val * IMX464_MAX_GAIN_DEC /
		(IMX464_MAX_GAIN_DB * mode->control_properties.gain_factor);

	if (priv->broadcast_ctrl == BROADCAST)
		err = imx464_broadcast_buffered_reg(s_data, GAIN_LOW, 2, gain);
	else
		err = imx464_write_buffered_reg(s_data, GAIN_LOW, 2, gain);

	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s:gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx464_set_exposure_shr_dol_short(struct tegracam_device *tc_dev,
						 s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u32 integration_time_line;
	u64 rhs1;

	dev_dbg(dev, "%s: short integration time: %lld [us]\n", __func__, val);

	if (val > s_data->short_exposure_max_range)
		val = s_data->short_exposure_max_range;
	else if (val < s_data->short_exposure_min_range)
		val = s_data->short_exposure_min_range;

	integration_time_line =
		DIV_ROUND_CLOSEST(val * IMX464_K_FACTOR, priv->line_time);

	rhs1 = integration_time_line + IMX464_DOL2_MIN_SHR1;

	if ((s32)rhs1 <
		(s32)((priv->rhs1_n + 2 * priv->brl - 2 * priv->frame_length) +
		2)) {
		pr_info(
			"\t\t check: RHS1 must be larger %llu\n",
			((priv->rhs1_n + 2 * priv->brl - 2 * priv->frame_length) +
			 2));
		rhs1 =
			((priv->rhs1_n + 2 * priv->brl - 2 * priv->frame_length) +
			 2);
	}

	if (rhs1 > priv->rhs1_max) {
		rhs1 = priv->rhs1_max;
	} else if (rhs1 < IMX464_DOL2_MIN_RHS1) {
		pr_info("\t\t check: RHS1 %llu < RHS_MIN %u\n", rhs1,
			IMX464_DOL2_MIN_RHS1);
		rhs1 = IMX464_DOL2_MIN_RHS1;
	}

	if (rhs1 % 4 != 1)
		rhs1 = 4 * (rhs1 / 4) + 1;
	priv->rhs1_n = rhs1;

	if (priv->broadcast_ctrl == BROADCAST)
		err = imx464_broadcast_buffered_reg(s_data, RHS1_LOW, 3, rhs1);
	else
		err = imx464_write_buffered_reg(s_data, RHS1_LOW, 3, rhs1);
	if (err) {
		dev_err(dev, "%s: failed to RHS1\n", __func__);
		return err;
	}

	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_EXPOSURE_SHORT);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	dev_dbg(dev,
		"%s: set short integration time: %lld [us], short integration time:%u [line], rhs1: %llu [line], shr1: %u [line]\n",
		__func__, val, integration_time_line, rhs1, IMX464_DOL2_MIN_SHR1);

	return err;
}

static int imx464_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u64 short_exposure_max_range = 0, short_exposure_min_range = 0;
	u32 integration_time_line;
	u8 min_shr0 = imx464_in_dol_mode(tc_dev) ? IMX464_DOL2_MIN_SHR0_LENGTH
						 : IMX464_MIN_SHR0_LENGTH;
	u64 max_shr0 =
		imx464_in_dol_mode(tc_dev)
		? 2 * priv->frame_length - IMX464_DOL2_MIN_INTEGRATION_LINES
		: priv->frame_length - IMX464_MIN_INTEGRATION_LINES;

	dev_dbg(dev, "\n\n%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line =
		DIV_ROUND_CLOSEST(val * IMX464_K_FACTOR, priv->line_time);

	if (imx464_in_dol_mode(tc_dev))
		priv->shr0 = 2 * priv->frame_length - integration_time_line;
	else
		priv->shr0 = priv->frame_length - integration_time_line;

	if (priv->shr0 < min_shr0)
		priv->shr0 = min_shr0;
	else if (priv->shr0 > max_shr0)
		priv->shr0 = max_shr0;

	if (imx464_in_dol_mode(tc_dev)) {
		priv->rhs1_max =
			2 * priv->frame_length - 2 * priv->brl - 16;
		pr_info("\t MAX_RHS1 %llu\n", priv->rhs1_max);
		if (priv->rhs1_max >= 2 * priv->brl) {
			priv->rhs1_max = 2 * priv->brl - 1;
			pr_info("\t MAX_RHS1 2*brl %llu\n", priv->rhs1_max);
		}
		if (priv->rhs1_max > priv->shr0 - 9) {
			priv->rhs1_max = priv->shr0 - 9;
			pr_info("\t MAX_RHS1 shr0 -9 %llu\n", priv->rhs1_max);
		}

		short_exposure_min_range =
			IMX464_DOL2_MIN_SEF_INTEGRATION_LINES * priv->line_time /
			IMX464_K_FACTOR;

		short_exposure_max_range =
			(priv->rhs1_max - IMX464_DOL2_MIN_SHR1) * priv->line_time /
			IMX464_K_FACTOR;

		fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE_SHORT,
					 short_exposure_min_range,
					 short_exposure_max_range);
	}

	if (priv->broadcast_ctrl == BROADCAST)
		err = imx464_broadcast_buffered_reg(s_data, SHR0_LOW, 3,
							priv->shr0);
	else
		err =
			imx464_write_buffered_reg(s_data, SHR0_LOW, 3, priv->shr0);
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
		"%s: set integration time: %lld [us], integration time:%d [line], shr0: %llu [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, priv->shr0, priv->frame_length);

	return err;
}

static int imx464_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err;
	u64 frame_length;
	u64 y_out_size = 0;
	u64 exposure_max_range, exposure_min_range;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	u8 min_shr0 = imx464_in_dol_mode(tc_dev) ? IMX464_DOL2_MIN_SHR0_LENGTH
						 : IMX464_MIN_SHR0_LENGTH;

	if (imx464_in_dol_mode(tc_dev))
		val = val * 2;

	frame_length = (((u64)mode->control_properties.framerate_factor *
			 IMX464_G_FACTOR) /
			(val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	priv->frame_length = frame_length;

	exposure_min_range = min_shr0 * priv->line_time / IMX464_K_FACTOR;

	if (imx464_in_dol_mode(tc_dev))
		exposure_max_range = (2 * priv->frame_length -
					IMX464_DOL2_MIN_INTEGRATION_LINES) *
					 priv->line_time / IMX464_K_FACTOR;
	else
		exposure_max_range =
			(priv->frame_length - IMX464_MIN_INTEGRATION_LINES) *
			priv->line_time / IMX464_K_FACTOR;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				 exposure_min_range, exposure_max_range);

	y_out_size = ((priv->rhs1_n - 3) / 2) + priv->brl;

	if (priv->broadcast_ctrl == BROADCAST) {
		err = imx464_broadcast_buffered_reg(s_data, VMAX_LOW, 3,
							priv->frame_length);
		if (imx464_in_dol_mode(tc_dev))
			err |= imx464_broadcast_buffered_reg(
				s_data, Y_OUT_SIZE_LOW, 2, y_out_size);
	} else {
		err = imx464_write_buffered_reg(s_data, VMAX_LOW, 3,
						priv->frame_length);
		if (imx464_in_dol_mode(tc_dev))
			err |= imx464_write_buffered_reg(s_data, Y_OUT_SIZE_LOW,
							 2, y_out_size);
	}
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(
		dev,
		"%s: val: %lld, frame_length set: %llu, y_out_size: %llu [line]\n",
		__func__, val, priv->frame_length, y_out_size);

	return 0;
}

static int imx464_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err =
			imx464_write_table(priv, mode_table[IMX464_EN_PATTERN_GEN]);
		if (err)
			goto fail;
		err = imx464_write_reg(s_data, TPG_PATSEL_DUOUT, (u8)(val - 1));
		if (err)
			goto fail;
	} else {
		err = imx464_write_table(priv,
					 mode_table[IMX464_DIS_PATTERN_GEN]);
		if (err)
			goto fail;
	}

	dev_dbg(dev, "%s++ Test mode pattern: %u\n", __func__, val - 1);

	return 0;
fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx464_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	u64 max_framerate;
	u64 frame_height = s_data->fmt_height;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	if (imx464_in_dol_mode(tc_dev))
		frame_height = frame_height / 2;

	priv->min_frame_length = frame_height + IMX464_MIN_FRAME_LENGTH_DELTA;
	if (imx464_in_dol_mode(tc_dev))
		max_framerate =
			(IMX464_G_FACTOR * IMX464_M_FACTOR) /
			((priv->min_frame_length * 2) * priv->line_time);
	else
		max_framerate = (IMX464_G_FACTOR * IMX464_M_FACTOR) /
				(priv->min_frame_length * priv->line_time);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				 ctrlprops->min_framerate, max_framerate);

	return 0;
}

static int imx464_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	return 0;
}
static bool imx464_find_broadcast_sensor(struct imx464 *broadcast_private)
{
	struct imx464 *current_private;

	list_for_each_entry(current_private, &imx464_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->broadcast_ctrl == BROADCAST &&
			current_private->s_data->power->state == SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			memcpy(broadcast_private, current_private,
				sizeof(*broadcast_private));
			return true;
		}
		mutex_unlock(&current_private->pw_mutex);
	}
	return false;
}

static void imx464_enable_second_slave_address(struct imx464 *ack_private)
{
	struct imx464 *current_private;
	int err;

	list_for_each_entry(current_private, &imx464_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx464_write_reg(current_private->s_data,
					SECOND_SLAVE_ADD, 5);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				 "%s: Fail to write Second I2C register\n",
				 __func__);

		mutex_unlock(&current_private->pw_mutex);

		dev_dbg(&current_private->i2c_client->dev,
			"%s: Sensors 2nd slave address configured\n", __func__);
	}

	err = imx464_write_reg(ack_private->s_data, SECOND_SLAVE_ADD, 7);
	if (err)
		dev_warn(&ack_private->i2c_client->dev,
			 "%s: Fail to write Second I2C register\n", __func__);

	dev_dbg(&ack_private->i2c_client->dev,
		": Sensors 2nd slave address configured with acknowlege\n");
}

static void imx464_disable_second_slave_address(void)
{
	struct imx464 *current_private;
	int err;

	list_for_each_entry(current_private, &imx464_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);
		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = imx464_write_reg(current_private->s_data,
					SECOND_SLAVE_ADD, 0);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				 "%s: Fail to write Second I2C register\n",
				 __func__);

		mutex_unlock(&current_private->pw_mutex);

		dev_dbg(&current_private->i2c_client->dev,
			"%s: Sensors 2nd slave address disabled\n", __func__);
	}
}

static void imx464_configure_second_slave_address(void)
{
	struct imx464 broadcast_private = {};

	if (imx464_find_broadcast_sensor(&broadcast_private))
		imx464_enable_second_slave_address(&broadcast_private);
	else
		imx464_disable_second_slave_address();
}

static int imx464_set_broadcast_ctrl(struct tegracam_device *tc_dev,
					 struct v4l2_ctrl *ctrl)
{
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
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
	imx464_configure_second_slave_address();

	return 0;
}

static int imx464_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx464 *priv = (struct imx464 *)s_data->priv;

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
		goto imx464_mclk_fail;
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

	imx464_configure_second_slave_address();

	return 0;

imx464_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx464_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx464 *priv = (struct imx464 *)s_data->priv;
	int err = 0;

	dev_dbg(dev, "%s: power off\n", __func__);

	err = imx464_write_reg(s_data, XVS_DRV_XHS_DRV, 0xF);
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
		max96792_power_off(priv->s_data->dser_dev,
				&priv->s_data->g_ctx);
	}

power_off_done:
	pw->state = SWITCH_OFF;
	mutex_unlock(&priv->pw_mutex);

	return 0;
}

static int imx464_power_get(struct tegracam_device *tc_dev)
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

	pw->reset_gpio = pdata->reset_gpio;

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		pw->reset_gpio = pdata->reset_gpio;

		if (pdata->use_cam_gpio) {
			err = cam_gpio_register(dev, pw->reset_gpio);
			if (err)
				dev_err(dev,
					"%s ERR can't register cam gpio %u!\n",
					__func__, pw->reset_gpio);
		}
	}

	pw->state = SWITCH_OFF;
	return err;
}

static int imx464_power_put(struct tegracam_device *tc_dev)
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

static int imx464_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx464_read_buffered_reg(s_data, VMAX_LOW, 2, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}
	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *
imx464_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx464_of_match, dev);
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
	if (gmsl_node == NULL) {
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

static int imx464_check_unsupported_mode(struct camera_common_data *s_data,
					 struct v4l2_mbus_framefmt *mf)
{
	struct imx464 *priv = (struct imx464 *)s_data->priv;
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = priv->tc_dev;

	priv->brl = s_data->frmfmt[s_data->mode].size.height;

	dev_dbg(dev, "%s++\n", __func__);

	if (imx464_in_dol_mode(tc_dev)) {
		switch (s_data->mode) {
			break;
		default:
			priv->brl = s_data->frmfmt[s_data->mode].size.height;
		}
	}

	return 0;
}

static int imx464_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx464_write_table(priv, mode_table[IMX464_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx464_write_table(priv, mode_table[IMX464_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx464_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == IMX464_TWO_LANE_MODE) {
		err = imx464_write_reg(s_data, LANEMODE, 1);
		if (err) {
			dev_err(dev, "%s: error setting two lane mode\n",
				__func__);
			return err;
		}
	}

	dev_dbg(dev, "%s: sensor is in %d CSI lane mode\n", __func__,
		s_data->numlanes);

	return 0;
}

static int imx464_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx464_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax * IMX464_G_FACTOR) / (IMX464_INCK);

	dev_dbg(dev,
		"%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

static int imx464_adjust_hmax_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 hmax = 0;

	dev_dbg(dev, "%s:++\n", __func__);

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
					CURRENT)) {
	case IMX464_1188_DATA_RATE:
		if (s_data->numlanes == IMX464_MAX_CSI_LANES) {
			if (s_data->colorfmt->code ==
				MEDIA_BUS_FMT_SRGGB10_1X10) {
				hmax = 0x01F4;
			} else {
				hmax = 0x02EE;
			}
		} else {
			hmax = 0x05DC;
		}
		break;
	case IMX464_891_DATA_RATE:
		if (s_data->numlanes == IMX464_MAX_CSI_LANES &&
			s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
			hmax = 0x02EE;
		} else {
			hmax = 0x05DC;
		}
		break;

	default:
		return 0;
	}

	err = imx464_write_buffered_reg(s_data, HMAX_LOW, 2, hmax);
	if (err) {
		dev_err(dev, "%s: failed to set HMAX register\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s:HMAX: %llu\n", __func__, hmax);

	return 0;
}

static int imx464_set_data_rate(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	if (s_data->power->state == SWITCH_ON) {
		err = imx464_write_table(
			priv, data_rate_table[fr_get_v4l2_ctrl_value(
				tc_dev, TEGRA_CAMERA_CID_DATA_RATE, CURRENT)]);
		if (err)
			goto fail;
	}

	dev_dbg(dev, "%s: Data rate: %llu\n", __func__,
		fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
					CURRENT));

	return 0;

fail:
	dev_err(dev, "%s: unable to set data rate\n", __func__);
	return err;
}

static int imx464_set_sync_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 extmode;

	if (val == INTERNAL_SYNC)
		extmode = 0;
	else
		extmode = 1;

	if (s_data->power->state == SWITCH_ON) {
		err = imx464_write_reg(s_data, EXTMODE, extmode);
		if (err)
			dev_err(dev, "%s: error setting sync mode\n", __func__);
	}
	return err;
}

static int imx464_configure_triggering_pins(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 xvs_xhs_drv = 0xF;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
					CURRENT)) {
	case MASTER_MODE:
		if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SYNC_MODE,
					CURRENT) == INTERNAL_SYNC) {
			xvs_xhs_drv = 0;
			dev_dbg(dev,
				"%s: Sensor is in Internal sync Master mode\n",
				__func__);
		} else if ((fr_get_v4l2_ctrl_value(tc_dev,
						TEGRA_CAMERA_CID_SYNC_MODE,
						CURRENT)) == EXTERNAL_SYNC) {
			xvs_xhs_drv = 0x3;
			dev_dbg(
				dev,
				"%s: Sensor is in - External sync Master mode\n",
				__func__);
		} else {
			xvs_xhs_drv = 0xF;
			dev_dbg(dev, "%s: Sensor is in No sync Master mode\n",
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

	err = imx464_write_reg(s_data, XVS_DRV_XHS_DRV, xvs_xhs_drv);
	if (err) {
		dev_err(dev, "%s: error setting Slave mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__,
		xvs_xhs_drv);

	return 0;
}

static int imx464_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx464 *priv = (struct imx464 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			err = imx464_update_ctrl(
				tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				(*ctrl->p_cur.p_s64 >> 2),
				IMX464_DEFAULT_BLACK_LEVEL_10BPP, 0,
				IMX464_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			err = imx464_update_ctrl(
				tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				(*ctrl->p_cur.p_s64 << 2),
				IMX464_DEFAULT_BLACK_LEVEL_12BPP, 0,
				IMX464_MAX_BLACK_LEVEL_12BPP);
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

static int imx464_set_mode(struct tegracam_device *tc_dev)
{
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx464_write_table(priv, mode_table[IMX464_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
			__func__);
		return err;
	}

	err = imx464_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = imx464_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
			__func__);
		return err;
	}

	err = imx464_write_table(priv, mode_table[s_data->mode]);
	if (err)
		return err;

	err = imx464_set_operation_mode(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to operation mode\n", __func__);
		return err;
	}

	err = imx464_set_sync_mode(
		tc_dev, fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SYNC_MODE,
					CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set sync mode\n", __func__);
		return err;
	}

	err = imx464_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx464_set_data_rate(
		tc_dev, fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
					CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set data rate\n", __func__);
		return err;
	}

	err = imx464_set_test_pattern(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx464_adjust_hmax_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust hmax\n", __func__);
		return err;
	}

	s_data->override_enable = true;

	err = imx464_calculate_line_time(tc_dev);
	if (err)
		goto fail;

	err = imx464_update_framerate_range(tc_dev);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;

fail:
	dev_err(dev, "%s: unable to set mode\n", __func__);
	return err;
}

static int imx464_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 xmsta;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {

		err = max96793_setup_streaming(priv->s_data->ser_dev, s_data);
		if (err)
			goto exit;
		err = max96792_setup_streaming(priv->s_data->dser_dev, dev,
						s_data);
		if (err)
			goto exit;
		err = max96792_start_streaming(priv->s_data->dser_dev, dev);
		if (err)
			goto exit;
	}

	err = imx464_write_table(priv, mode_table[IMX464_MODE_START_STREAM]);
	if (err)
		goto exit;

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
				CURRENT) == MASTER_MODE)
		xmsta = 0x00;
	else
		xmsta = 0x01;

	err = imx464_write_reg(s_data, XMSTA, xmsta);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx464_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx464 *priv = (struct imx464 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx464_write_table(priv, mode_table[IMX464_MODE_STOP_STREAM]);
	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX464_K_FACTOR,
			 priv->frame_length * priv->line_time / IMX464_K_FACTOR +
			 1000);

	return 0;
}

static struct camera_common_sensor_ops imx464_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx464_frmfmt),
	.frmfmt_table = imx464_frmfmt,
	.power_on = imx464_power_on,
	.power_off = imx464_power_off,
	.write_reg = imx464_write_reg,
	.read_reg = imx464_read_reg,
	.parse_dt = imx464_parse_dt,
	.power_get = imx464_power_get,
	.power_put = imx464_power_put,
	.set_mode = imx464_set_mode,
	.start_streaming = imx464_start_streaming,
	.stop_streaming = imx464_stop_streaming,
	.check_unsupported_mode = imx464_check_unsupported_mode,
	.after_set_pixel_format = imx464_after_set_pixel_format,
};

static int imx464_gmsl_serdes_setup(struct imx464 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev ||
		!priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	dev_dbg(dev, "%s: Enter function\n", __func__);

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
	err =
		max96792_setup_link(priv->s_data->dser_dev, &priv->i2c_client->dev);
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

static void imx464_gmsl_serdes_reset(struct imx464 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx464_board_setup(struct imx464 *priv)
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

		err = imx464_gmsl_serdes_setup(priv);
		if (err) {
			dev_err(dev, "%s gmsl serdes setup failed\n", __func__);
			return err;
		}
	}

	err = camera_common_mclk_enable(s_data);
	if (err) {
		dev_err(dev, "Error %d turning on mclk\n", err);
		return err;
	}

	err = imx464_power_on(s_data);
	if (err) {
		dev_err(dev, "Error %d during power on sensor\n", err);
		return err;
	}

	err = imx464_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
			__func__);
		goto error2;
	}

	err = imx464_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

	priv->min_frame_length =
		IMX464_DEFAULT_HEIGHT + IMX464_MIN_FRAME_LENGTH_DELTA;

error2:
	imx464_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx464_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "imx464 : open\n");

	return 0;
}

static const struct v4l2_subdev_internal_ops imx464_subdev_internal_ops = {
	.open = imx464_open,
};

static struct tegracam_ctrl_ops imx464_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx464_set_gain,
	.set_exposure = imx464_set_exposure,
	.set_exposure_short = imx464_set_exposure_shr_dol_short,
	.set_frame_rate = imx464_set_frame_rate,
	.set_group_hold = imx464_set_group_hold,
	.set_test_pattern = imx464_set_test_pattern,
	.set_data_rate = imx464_set_data_rate,
	.set_operation_mode = imx464_set_operation_mode,
	.set_sync_mode = imx464_set_sync_mode,
	.set_broadcast_ctrl = imx464_set_broadcast_ctrl,
	.set_black_level = imx464_set_black_level,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx464_probe(struct i2c_client *client)
#else
static int imx464_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx464 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev, sizeof(struct imx464), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev, sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx464", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx464_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx464_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx464_ctrl_ops;

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
	priv->brl = IMX464_DOL2_BRL_FULL_FRAME_MODE;
	priv->rhs1_n = 0xc9;
	priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx464_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx464_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				 IMX464_DEFAULT_BLACK_LEVEL_10BPP,
				 IMX464_DEFAULT_BLACK_LEVEL_10BPP, 0,
				 IMX464_MAX_BLACK_LEVEL_10BPP);
	if (err)
		return err;

	err = imx464_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				 (ARRAY_SIZE(imx464_test_pattern_menu) - 1));
	if (err)
		return err;

	err = imx464_update_ctrl(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, 0, 0, 0,
				 (ARRAY_SIZE(imx464_data_rate_menu) - 1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx464_sensor_list);

	dev_info(dev, "Detected imx464 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx464_remove(struct i2c_client *client)
#else
static void imx464_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx464 *priv;

	if (s_data == NULL) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx464 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx464_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx464_id[] = {{"imx464", 0}, {}};

MODULE_DEVICE_TABLE(i2c, imx464_id);

static struct i2c_driver imx464_i2c_driver = {
	.driver = {
		.name = "imx464",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx464_of_match),
	},
	.probe = imx464_probe,
	.remove = imx464_remove,
	.id_table = imx464_id,
};

module_i2c_driver(imx464_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX464");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
