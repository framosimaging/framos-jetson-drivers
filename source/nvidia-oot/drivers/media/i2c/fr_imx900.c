// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx900.c - Framos fr_imx900.c driver
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

#include "fr_imx900_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX900_K_FACTOR 1000LL
#define IMX900_M_FACTOR 1000000LL
#define IMX900_G_FACTOR 1000000000LL
#define IMX900_T_FACTOR 1000000000000LL

#define IMX900_MAX_GAIN_DEC 480
#define IMX900_MAX_GAIN_DB 48

#define IMX900_MAX_BLACK_LEVEL_8BPP 255
#define IMX900_MAX_BLACK_LEVEL_10BPP 1023
#define IMX900_MAX_BLACK_LEVEL_12BPP 4095
#define IMX900_DEFAULT_BLACK_LEVEL_8BPP 15
#define IMX900_DEFAULT_BLACK_LEVEL_10BPP 60
#define IMX900_DEFAULT_BLACK_LEVEL_12BPP 240

#define IMX900_DEFAULT_LINE_TIME 8220

#define IMX900_MIN_SHS_LENGTH 51
#define IMX900_INTEGRATION_OFFSET 2
#define IMX900_MIN_INTEGRATION_LINES 1

#define IMX900_INCK 74250000LL

#define IMX900_MAX_CSI_LANES 4
#define IMX900_TWO_LANE_MODE 2
#define IMX900_ONE_LANE_MODE 1


LIST_HEAD(imx900_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx900_of_match[] = {
	{ .compatible = "framos,imx900",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx900_of_match);

const char * const imx900_data_rate_menu[] = {
	[IMX900_2376_MBPS] = "2376 Mbps/lane",
	[IMX900_1485_MBPS] = "1485 Mbps/lane",
	[IMX900_1188_MBPS] = "1188 Mbps/lane",
	[IMX900_891_MBPS] = "891 Mbps/lane",
	[IMX900_594_MBPS] = "594 Mbps/lane",
};

static const char * const imx900_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Sequence Pattern 1",
	[2] = "Sequence Pattern 2",
	[3] = "Gradation Pattern",
	[4] = "Color Bar Horizontally",
	[5] = "Color Bar Vertically",
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
	TEGRA_CAMERA_CID_DATA_RATE,
};

struct imx900 {
	struct i2c_client		*i2c_client;
	struct v4l2_subdev		*subdev;
	u64				frame_length;
	u64				min_frame_length;
	u64				current_pixel_format;
	u32				line_time;
	struct list_head		entry;
	struct mutex			pw_mutex;
	struct camera_common_data	*s_data;
	struct tegracam_device		*tc_dev;
	u8				chromacity;
};

static bool imx900_is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case CHROMACITY:
		return true;
	default:
		return false;
	}
}

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
	.use_single_read = true,
	.use_single_write = true,
	.volatile_reg = imx900_is_volatile_reg,
};

static inline int imx900_read_reg(struct camera_common_data *s_data,
				u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx900_write_reg(struct camera_common_data *s_data,
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

static int imx900_read_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;

	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx900_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
								__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx900_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
								 __func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx900_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
								__func__);
		return err;
		}
	}

	return err;
}

static int imx900_write_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx900_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx900_write_reg(s_data, addr_low + i,
							(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx900_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
		return err;
		}
	}

	return err;
}

static int imx900_write_table(struct imx900 *priv, const imx900_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					table,
					NULL, 0,
					IMX900_TABLE_WAIT_MS,
					IMX900_TABLE_END);
}

static int imx900_chromacity_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 chromacity;

	err = imx900_write_reg(s_data, STANDBY, 0x00);
	if (err) {
		dev_err(dev, "%s: error canceling standby mode\n", __func__);
		return err;
	}

	usleep_range(15000, 20000);

	err = imx900_read_reg(s_data, CHROMACITY, &chromacity);
	if (err) {
		dev_err(dev, "%s: error reading chromacity information register\n",
									__func__);
		return err;
	}

	err = imx900_write_reg(s_data, STANDBY, 0x01);
	if (err) {
		dev_err(dev, "%s: error setting standby mode\n", __func__);
		return err;
	}

	usleep_range(15000, 20000);

	chromacity = chromacity >> 7;
	priv->chromacity = chromacity;

	dev_dbg(dev, "%s: sensor is color(0)/monochrome(1): %d\n",
							__func__, chromacity);

	return err;
}

static int imx900_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx900_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx900_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
								u64 max_val)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
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
				ctrl->qmenu = imx900_test_pattern_menu;
				ctrl->maximum = max_val;
				break;
			case TEGRA_CAMERA_CID_DATA_RATE:
				ctrl->qmenu = imx900_data_rate_menu;
				ctrl->maximum = max_val;
				break;
			}
		}
	return 0;
}

static int imx900_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx900_write_buffered_reg(s_data, BLKLEVEL_LOW, 2, val);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx900_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err;
	u32 gain;

	gain = val * IMX900_MAX_GAIN_DEC /
				(IMX900_MAX_GAIN_DB *
					mode->control_properties.gain_factor);

	err = imx900_write_buffered_reg(s_data, GAIN_LOW, 2, gain);
	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx900_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err;
	u32 integration_time_line;
	u32 integration_offset = IMX900_INTEGRATION_OFFSET;
	u32 reg_shs;
	u8 min_reg_shs;
	u8 reg_gmrwt2, reg_gmtwt;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);


	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line = ((val - integration_offset)
					* IMX900_K_FACTOR) / priv->line_time;

	reg_shs = priv->frame_length - integration_time_line;

	imx900_read_reg(s_data, GMTWT, &reg_gmtwt);
	imx900_read_reg(s_data, GMRWT2, &reg_gmrwt2);

	min_reg_shs = reg_gmtwt + reg_gmrwt2;

	if (reg_shs < min_reg_shs)
		reg_shs = min_reg_shs;
	else if (reg_shs > (priv->frame_length - IMX900_MIN_INTEGRATION_LINES))
		reg_shs = priv->frame_length - IMX900_MIN_INTEGRATION_LINES;

	err = imx900_write_buffered_reg(s_data, SHS_LOW, 3, reg_shs);
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

	dev_dbg(dev, "%s: set integration time: %lld [us], coarse1:%d [line], shs: %d [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, reg_shs, priv->frame_length);

	return err;
}

static int imx900_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err;
	u8 min_reg_shs;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;
	u8 reg_gmrwt2, reg_gmtwt;

	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length = (((u64)mode->control_properties.framerate_factor *
				IMX900_G_FACTOR) / (val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	priv->frame_length = frame_length;

	imx900_read_reg(s_data, GMTWT, &reg_gmtwt);
	imx900_read_reg(s_data, GMRWT2, &reg_gmrwt2);

	min_reg_shs = reg_gmtwt + reg_gmrwt2;

	exposure_min_range = IMX900_MIN_INTEGRATION_LINES *
					priv->line_time / IMX900_K_FACTOR;
	exposure_min_range += IMX900_INTEGRATION_OFFSET;
	exposure_max_range = (priv->frame_length - min_reg_shs) *
					priv->line_time / IMX900_K_FACTOR;
	exposure_max_range += IMX900_INTEGRATION_OFFSET;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
					exposure_min_range, exposure_max_range);

	err = imx900_write_buffered_reg(s_data, VMAX_LOW, 3, priv->frame_length);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: val: %lld, frame_length set: %llu\n",
					__func__, val, priv->frame_length);

	return 0;
}

static int imx900_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err = imx900_write_reg(s_data, 0x3550, 0x07);
		if (err)
			goto fail;
		if (val == 4) {
			err = imx900_write_reg(s_data, 0x3551, 0x0A);
			if (err)
				goto fail;
		} else if (val == 5) {
			err = imx900_write_reg(s_data, 0x3551, 0x0B);
			if (err)
				goto fail;
		} else {
			err = imx900_write_reg(s_data, 0x3551, (u8)(val));
			if (err)
				goto fail;
		}
	} else {
		err = imx900_write_reg(s_data, 0x3550, 0x06);
		if (err)
			goto fail;
	}

	return 0;

fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx900_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	struct device *dev = tc_dev->dev;
	u64 max_framerate;
	u8 gmrwt, gmrwt2, gmtwt, gsdly;
	int err;

	err = imx900_read_reg(s_data, GMRWT, &gmrwt);
	err |= imx900_read_reg(s_data, GMRWT2, &gmrwt2);
	err |= imx900_read_reg(s_data, GMTWT, &gmtwt);
	err |= imx900_read_reg(s_data, GSDLY, &gsdly);

	if (err) {
		dev_err(dev,
		"%s: error reading gmrtw, gmrtw2, gmtwt, gsdly registers\n",
								__func__);
		return err;
	}

	ctrlprops = &s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	switch (s_data->mode) {
	case IMX900_MODE_2064x1552:
		priv->min_frame_length = IMX900_DEFAULT_HEIGHT + gmrwt +
						gmrwt2*2 + gmtwt + gsdly + 56;
		break;
	case IMX900_MODE_ROI_1920x1080:
		priv->min_frame_length = IMX900_ROI_MODE_HEIGHT + gmrwt +
						gmrwt2*2 + gmtwt + gsdly + 56;
		break;
	case IMX900_MODE_SUBSAMPLING2_1032x776:
		if (priv->chromacity == IMX900_COLOR) {
			priv->min_frame_length = IMX900_SUBSAMPLING2_MODE_HEIGHT
					+ gmrwt + gmrwt2*2 + gmtwt + gsdly + 34;
		} else {
			priv->min_frame_length = IMX900_SUBSAMPLING2_MODE_HEIGHT
					+ gmrwt + gmrwt2*2 + gmtwt + gsdly + 38;
	}
		break;
	case IMX900_MODE_SUBSAMPLING10_2064x154:
		priv->min_frame_length = IMX900_SUBSAMPLING10_MODE_HEIGHT +
					gmrwt + gmrwt2*2 + gmtwt + gsdly + 34;
		break;
	case IMX900_MODE_BINNING_CROP_1024x720:
		priv->min_frame_length = IMX900_BINNING_CROP_MODE_HEIGHT +
					gmrwt + gmrwt2*2 + gmtwt + gsdly + 38;
		break;
	}

	max_framerate = (IMX900_G_FACTOR * IMX900_M_FACTOR) /
				(priv->min_frame_length * priv->line_time);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				ctrlprops->min_framerate, max_framerate);

	return 0;

}

static int imx900_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_pdata *pdata = s_data->pdata;

	if (pdata->xmaster_gpio)
		fr_gpio_set(s_data, pdata->xmaster_gpio, val);

	return 0;
}

static int imx900_set_shutter_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl = fr_find_v4l2_ctrl(tc_dev,
						TEGRA_CAMERA_CID_SHUTTER_MODE);

	if (ctrl == NULL)
		return 0;

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
				CURRENT) == MASTER_MODE && val == SEQ_TRIGGER) {
		dev_warn(dev,
		"%s: Sequential trigger isn't supported in master mode\n",
								__func__);
		goto default_state;
	}

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
				CURRENT) == SLAVE_MODE && val == FAST_TRIGGER){
		dev_warn(dev,
		"%s: Fast trigger isn't supported in slave mode\n", __func__);
		goto default_state;
	}

	return 0;

default_state:
	*ctrl->p_new.p_s64 = NORMAL_EXPO;
	*ctrl->p_cur.p_s64 = NORMAL_EXPO;
	return 0;
}

static int imx900_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx900 *priv = (struct imx900 *)s_data->priv;

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
		goto imx900_mclk_fail;
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
	usleep_range(30000, 31000);

	mutex_unlock(&priv->pw_mutex);

	return 0;

imx900_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx900_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx900 *priv = (struct imx900 *)s_data->priv;
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

static int imx900_power_get(struct tegracam_device *tc_dev)
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

static int imx900_power_put(struct tegracam_device *tc_dev)
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

static int imx900_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx900_read_buffered_reg(s_data, VMAX_LOW, 3, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}

	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *imx900_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx900_of_match, dev);
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

static int imx900_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 adbit_monosel;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB8_1X8:
	case MEDIA_BUS_FMT_SGBRG8_1X8:
		adbit_monosel = (priv->chromacity == IMX900_COLOR) ? 0x21 : 0x25;
		err = imx900_write_reg(s_data, ADBIT_MONOSEL, adbit_monosel);
		if (err) {
			dev_err(dev, "%s: error setting chromacity pixel format\n",
									__func__);
		return err;
		}
		err = imx900_write_table(priv, mode_table[IMX900_8BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB10_1X10:
	case MEDIA_BUS_FMT_SGBRG10_1X10:
		adbit_monosel = (priv->chromacity == IMX900_COLOR) ? 0x01 : 0x05;
		err = imx900_write_reg(s_data, ADBIT_MONOSEL, adbit_monosel);
		if (err) {
			dev_err(dev, "%s: error setting chromacity pixel format\n",
									__func__);
		return err;
		}
		err = imx900_write_table(priv, mode_table[IMX900_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
	case MEDIA_BUS_FMT_SGBRG12_1X12:
		adbit_monosel = (priv->chromacity == IMX900_COLOR) ? 0x11 : 0x15;
		err = imx900_write_reg(s_data, ADBIT_MONOSEL, adbit_monosel);
		if (err) {
			dev_err(dev, "%s: error setting chromacity pixel format\n",
									__func__);
		return err;
	}
	err = imx900_write_table(priv, mode_table[IMX900_12BIT_MODE]);
		break;
	default:
	dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx900_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == IMX900_ONE_LANE_MODE) {
		err = imx900_write_reg(s_data, LANESEL, 4);
		if (err) {
			dev_err(dev, "%s: error setting one lane mode\n",
								__func__);
			return err;
		}
	} else if (s_data->numlanes == IMX900_TWO_LANE_MODE) {
		err = imx900_write_reg(s_data, LANESEL, 3);
		if (err) {
			dev_err(dev, "%s: error setting two lane mode\n",
								__func__);
		return err;
		}
	} else if (s_data->numlanes == IMX900_MAX_CSI_LANES) {
		err = imx900_write_reg(s_data, LANESEL, 2);
		if (err) {
			dev_err(dev, "%s: error setting four lane mode\n",
								__func__);
		return err;
		}
	}

	dev_dbg(dev, "%s: sensor is in %d CSI lane mode\n",
				__func__, s_data->numlanes);

	return 0;
}

static int imx900_write_mode_dep_chromacity(struct tegracam_device *tc_dev,
					int table1, int table2, int table3)
{
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err = 0;

	if (priv->chromacity == IMX900_COLOR) {
		if (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776)
			err = imx900_write_table(priv, mode_table[table1]);
		else
			err = imx900_write_table(priv, mode_table[table2]);
	} else {
		if ((s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ||
			(s_data->mode == IMX900_MODE_BINNING_CROP_1024x720))
			err = imx900_write_table(priv, mode_table[table3]);
		else
			err = imx900_write_table(priv, mode_table[table2]);
	}

	return err;
}

static int imx900_set_dep_registers(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, CURRENT)) {
	case IMX900_2376_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
				return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
					}
				break;
			default:
					dev_err(dev, "%s: unknown lane mode\n", __func__);
					return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
				return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
				return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_1485_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_1188_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
		return 0;
		}
		break;
	case IMX900_891_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
				return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_594_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_1LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_1LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_1LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_TWO_LANE_MODE:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_2LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_2LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_2LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			case IMX900_MAX_CSI_LANES:
				err = imx900_write_mode_dep_chromacity(tc_dev,
					IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_4LANE,
					IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_4LANE,
					IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_4LANE);
				if (err) {
					dev_err(dev, "%s: error setting dep register table\n",
										__func__);
					return err;
				}
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	default:
		/* Adjusment isn't needed */
		return 0;
	}

	return err;
}

static int imx900_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx900_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax*IMX900_G_FACTOR) / (IMX900_INCK);


	dev_dbg(dev, "%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

static int imx900_adjust_hmax_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err;
	u64 hmax;

	dev_dbg(dev, "%s:++\n", __func__);

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, CURRENT)) {
	case IMX900_2376_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0X152 : 0x22A;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x128 : 0x22A;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x152;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0A9 : 0x152;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x152;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0A9 : 0x152;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x16C : 0x2AB;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x168 : 0x2AB;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x16C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0C7 : 0x16C;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x16C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0B6 : 0x16C;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x262 : 0x32C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x1A9 : 0x32C;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x262;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x131 : 0x262;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x262;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x131 : 0x262;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_1485_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x1CC : 0x369;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x152 : 0x1CC;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0FE : 0x1CC;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x152;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0A9 : 0x152;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x234 : 0x438;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x16C : 0x234;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x131 : 0x234;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x16C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0B6 : 0x16C;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x29B : 0x506;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x262 : 0x29B;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x165 : 0x29B;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x262;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x131 : 0x262;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_1188_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x23B : 0x43F;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x152 : 0x23B;
				else
					hmax = ((s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ||
						(s_data->mode == IMX900_MODE_BINNING_CROP_1024x720)) ? 0x139 : 0x23B;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x152;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0B8 : 0x152;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x2BC : 0x541;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x179 : 0x2BC;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x16C : 0x17A;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0D8 : 0x17A;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
		break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x33D : 0x643;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x262 : 0x33D;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x1BA : 0x33D;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x262;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x131 : 0x262;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_891_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x2F4 : 0x5A4;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x19C : 0x2F4;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x152 : 0x19C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x0F0 : 0x19C;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x3A0 : 0x6FC;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x1F2 : 0x3A0;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x16C : 0x1F3;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x11B : 0x1F3;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x44C : 0x854;
				break;
			case IMX900_TWO_LANE_MODE:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x262 : 0x44C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x248 : 0x44C;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = 0x262;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x147 : 0x262;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX900_594_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x45E : 0x866;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x258 : 0x45C;
				break;
			case IMX900_MAX_CSI_LANES:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x158 : 0x25A;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x560 : 0xA6A;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x2DA : 0x55E;
				break;
			case IMX900_MAX_CSI_LANES:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x198 : 0x2DA;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			switch (s_data->numlanes) {
			case IMX900_ONE_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x662 : 0xC6E;
				break;
			case IMX900_TWO_LANE_MODE:
				hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
					s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x35A : 0x660;
				break;
			case IMX900_MAX_CSI_LANES:
				if (priv->chromacity == IMX900_COLOR)
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776) ? 0x262 : 0x35C;
				else
					hmax = (s_data->mode == IMX900_MODE_SUBSAMPLING2_1032x776 ||
						s_data->mode == IMX900_MODE_BINNING_CROP_1024x720) ? 0x1D8 : 0x35C;
				break;
			default:
				dev_err(dev, "%s: unknown lane mode\n", __func__);
				return 0;
			}
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	default:
		/* Adjusment isn't needed */
		return 0;
	}

	err = imx900_write_buffered_reg(s_data, HMAX_LOW, 2, hmax);
	if (err) {
		dev_err(dev, "%s: failed to set HMAX register\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: HMAX: %llu\n", __func__, hmax);

	return 0;
}

static int imx900_set_data_rate(struct tegracam_device *tc_dev, u32 val)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	if (s_data->power->state == SWITCH_ON) {
		err = imx900_write_table(priv,
			data_rate_table[fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_DATA_RATE, CURRENT)]);
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

static int imx900_configure_shutter(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 trigen = 0;
	u8 vint_en = 0;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE,
								CURRENT)) {
	case NORMAL_EXPO:
		trigen = 0;
		vint_en = 2;
		dev_dbg(dev, "%s: Sensor is in Normal Exposure Mode\n", __func__);
		break;
	case SEQ_TRIGGER:
		if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
							CURRENT) == MASTER_MODE) {
			dev_warn(dev,
				"%s: Sequential Trigger Mode not supported in Master mode, switchig to default\n",
				__func__);
			break;
		}
		trigen = 9;
		vint_en = 1;
		dev_dbg(dev, "%s: Sensor is in Sequential Trigger Mode\n",
								__func__);
		break;
	case FAST_TRIGGER:
		if (fr_get_v4l2_ctrl_value(tc_dev,
			TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT) == SLAVE_MODE) {
			dev_warn(dev,
				"%s: Fast Trigger Mode not supported in Slave mode, switchig to default\n",
				__func__);
			break;
		}
		trigen = 10;
		dev_dbg(dev, "%s: Sensor is in Fast Trigger Mode\n", __func__);
		break;
	default:
		pr_err("%s: unknown exposure mode.\n", __func__);
		return -EINVAL;
	}

	switch (s_data->mode) {
	case IMX900_MODE_2064x1552:
		vint_en |= 0x1C;
		break;
	case IMX900_MODE_ROI_1920x1080:
		vint_en |= 0x1C;
		break;
	case IMX900_MODE_SUBSAMPLING2_1032x776:
		vint_en |= (priv->chromacity == IMX900_COLOR) ? 0x14 : 0x18;
		break;
	case IMX900_MODE_SUBSAMPLING10_2064x154:
		vint_en |= 0x14;
		break;
	case IMX900_MODE_BINNING_CROP_1024x720:
		vint_en |= 0x18;
		break;
	}

	err = imx900_write_reg(s_data, TRIGMODE, trigen);
	err |= imx900_write_reg(s_data, VINT_EN, vint_en);
	if (err) {
		dev_err(dev, "%s: error setting exposure mode\n", __func__);
		return err;
	}

	return 0;
}

static int imx900_configure_triggering_pins(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 sync_sel = 0xF0;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
								CURRENT)) {
	case MASTER_MODE:
		sync_sel = 0xC0;
		dev_dbg(dev, "%s: Sensor is in Master mode\n", __func__);
		break;
	case SLAVE_MODE:
		sync_sel = 0xF0;
		dev_dbg(dev, "%s: Sensor is in Slave mode\n", __func__);
		break;
	default:
		pr_err("%s: unknown operation mode.\n", __func__);
		return -EINVAL;
	}

	err = imx900_write_reg(s_data, SYNCSEL, sync_sel);
	if (err) {
		dev_err(dev, "%s: error setting Slave mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__, sync_sel);

	return 0;
}

/*
 * According to the V4L2 documentation, driver should not return error when
 * invalid settings are detected.
 * It should apply the settings closest to the ones that the user has requested.
 */
static int imx900_check_unsupported_mode(struct camera_common_data *s_data,
					struct v4l2_mbus_framefmt *mf)
{
	struct device *dev = s_data->dev;
	struct imx900 *priv = (struct imx900 *)s_data->priv;

	dev_dbg(dev, "%s++\n", __func__);

	switch (s_data->mode) {
	case IMX900_MODE_2064x1552:
	case IMX900_MODE_ROI_1920x1080:
		switch (mf->code) {
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			mf->code = MEDIA_BUS_FMT_SRGGB8_1X8;
			dev_warn(dev,
				"%s: selected mode is not supported with GBRG8 pattern, switching to RGGB8\n",
				__func__);
			break;
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			mf->code = MEDIA_BUS_FMT_SRGGB10_1X10;
			dev_warn(dev,
				"%s: selected mode is not supported with GBRG10 pattern, switching to RGGB10\n",
				__func__);
			break;
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			mf->code = MEDIA_BUS_FMT_SRGGB12_1X12;
			dev_warn(dev,
				"%s: selected mode is not supported with GBRG12 pattern, switching to RGGB12\n",
				__func__);
			break;
		default:
			break;
		}
		break;
	case IMX900_MODE_SUBSAMPLING2_1032x776:
	case IMX900_MODE_SUBSAMPLING10_2064x154:
		switch (mf->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			mf->code = MEDIA_BUS_FMT_SGBRG8_1X8;
			dev_warn(dev,
				"%s: selected mode is not supported with RGGB8 pattern, switching to GBRG8\n",
				__func__);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			mf->code = MEDIA_BUS_FMT_SGBRG10_1X10;
			dev_warn(dev,
				"%s: selected mode is not supported with RGGB10 pattern, switching to GBRG10\n",
				__func__);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			mf->code = MEDIA_BUS_FMT_SGBRG12_1X12;
			dev_warn(dev,
				"%s: selected mode is not supported with RGGB12 pattern, switching to GBRG12\n",
				__func__);
			break;
		default:
			break;
		}
		break;
	case IMX900_MODE_BINNING_CROP_1024x720:
		if (priv->chromacity == IMX900_COLOR) {
			dev_warn(dev,
				"%s: selected mode is not supported for color sensor, switching to default\n",
				__func__);
			mf->width = s_data->frmfmt[s_data->def_mode].size.width;
			mf->height = s_data->frmfmt[s_data->def_mode].size.height;
		}
		break;
	default:
		break;
	}

	return 0;
}

static int imx900_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx900 *priv = (struct imx900 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	/* Update Black level V4L control*/
	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
		case MEDIA_BUS_FMT_SGBRG8_1X8:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG12_1X12)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 >> 4),
							IMX900_DEFAULT_BLACK_LEVEL_8BPP,
							0, IMX900_MAX_BLACK_LEVEL_8BPP);
			else if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB10_1X10 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG10_1X10)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 >> 2),
							IMX900_DEFAULT_BLACK_LEVEL_8BPP,
							0, IMX900_MAX_BLACK_LEVEL_8BPP);
			if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB8_1X8)
				priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB8_1X8;
			else
				priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG8_1X8;
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG12_1X12)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 >> 2),
							IMX900_DEFAULT_BLACK_LEVEL_10BPP,
							0, IMX900_MAX_BLACK_LEVEL_10BPP);
			else if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB8_1X8 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG8_1X8)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 << 2),
							IMX900_DEFAULT_BLACK_LEVEL_10BPP,
							0, IMX900_MAX_BLACK_LEVEL_10BPP);
			if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
				priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			else
				priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB10_1X10 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG10_1X10)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 << 2),
							IMX900_DEFAULT_BLACK_LEVEL_12BPP,
							0, IMX900_MAX_BLACK_LEVEL_12BPP);
			else if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB8_1X8 ||
				priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG8_1X8)
				err = imx900_update_ctrl(tc_dev,
							TEGRA_CAMERA_CID_BLACK_LEVEL,
							(*ctrl->p_cur.p_s64 << 4),
							IMX900_DEFAULT_BLACK_LEVEL_12BPP,
							0, IMX900_MAX_BLACK_LEVEL_12BPP);
			if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB12_1X12)
				priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB12_1X12;
			else
				priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG12_1X12;
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

static int imx900_set_mode_additional(struct tegracam_device *tc_dev)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->mode) {
	case IMX900_MODE_2064x1552:
		err = imx900_write_table(priv,
				mode_table[IMX900_MODE_ALLPIXEL_ROI]);
		break;
	case IMX900_MODE_ROI_1920x1080:
		err = imx900_write_table(priv,
				mode_table[IMX900_MODE_ALLPIXEL_ROI]);
		break;
	case IMX900_MODE_SUBSAMPLING2_1032x776:
		if (priv->chromacity == IMX900_COLOR)
			err = imx900_write_table(priv,
				mode_table[IMX900_MODE_SUBSAMPLING2_COLOR]);
		else
			err = imx900_write_table(priv,
				mode_table[IMX900_MODE_SUBSAMPLING2_BINNING_MONO]);
		break;
	case IMX900_MODE_SUBSAMPLING10_2064x154:
		err = imx900_write_table(priv,
				mode_table[IMX900_MODE_SUBSAMPLING10]);
		break;
	case IMX900_MODE_BINNING_CROP_1024x720:
		err = imx900_write_table(priv,
				mode_table[IMX900_MODE_SUBSAMPLING2_BINNING_MONO]);
		break;
	}

	if (err) {
		dev_err(dev, "%s: unable to set additional mode registers\n",
								__func__);
		return err;
	}

	return 0;

}

static int imx900_set_mode(struct tegracam_device *tc_dev)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx900_chromacity_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to get chromacity information\n",
								__func__);
		return err;
	}

	err = imx900_write_table(priv, mode_table[IMX900_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
		return err;
	}

	err = imx900_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = imx900_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
								__func__);
		return err;
	}

	err = imx900_write_table(priv, mode_table[s_data->mode]);
	if (err) {
		dev_err(dev, "%s: unable to set sensor mode settings\n",
								__func__);
		return err;
	}

	err = imx900_set_mode_additional(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to set additional sensor mode settings\n",
								__func__);
		return err;
	}

	err = imx900_set_operation_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}

	err = imx900_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx900_set_data_rate(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
					TEGRA_CAMERA_CID_DATA_RATE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set data rate\n", __func__);
		return err;
	}

	err = imx900_set_dep_registers(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write dep registers to image sensor\n",
								__func__);
		return err;
	}

	err = imx900_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx900_adjust_hmax_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust hmax\n", __func__);
		return err;
	}

	err = imx900_configure_shutter(tc_dev);
	if (err)
		goto fail;

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	err = imx900_calculate_line_time(tc_dev);
	if (err)
		goto fail;

	err = imx900_update_framerate_range(tc_dev);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;

fail:
	dev_err(dev, "%s: unable to set mode\n", __func__);
	return err;
}

static int imx900_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
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

	err = imx900_write_reg(s_data, STANDBY, 0x00);

	usleep_range(15000, 20000);

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
							CURRENT) == MASTER_MODE)
		xmsta = 0x00;
	else
		xmsta = 0x01;

	err = imx900_write_reg(s_data, XMSTA, xmsta);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx900_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx900 *priv = (struct imx900 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx900_write_table(priv, mode_table[IMX900_MODE_STOP_STREAM]);
	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX900_K_FACTOR,
		priv->frame_length * priv->line_time / IMX900_K_FACTOR + 1000);

	return 0;
}

static struct camera_common_sensor_ops imx900_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx900_frmfmt),
	.frmfmt_table = imx900_frmfmt,
	.power_on = imx900_power_on,
	.power_off = imx900_power_off,
	.write_reg = imx900_write_reg,
	.read_reg = imx900_read_reg,
	.parse_dt = imx900_parse_dt,
	.power_get = imx900_power_get,
	.power_put = imx900_power_put,
	.set_mode = imx900_set_mode,
	.start_streaming = imx900_start_streaming,
	.stop_streaming = imx900_stop_streaming,
	.check_unsupported_mode = imx900_check_unsupported_mode,
	.after_set_pixel_format = imx900_after_set_pixel_format,
};

static int imx900_gmsl_serdes_setup(struct imx900 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev ||
							!priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	dev_dbg(dev, "%s: IMX900_gmsl_serdes_setup\n", __func__);
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

static void imx900_gmsl_serdes_reset(struct imx900 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx900_board_setup(struct imx900 *priv)
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

		err = of_property_read_u32(ser_node,
					"reg", &priv->s_data->g_ctx.ser_reg);
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

		if (!strcmp(str_value, "1x4"))
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_1X4_MODE;
		else if (!strcmp(str_value, "2x4"))
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_2X4_MODE;
		else if (!strcmp(str_value, "2x2"))
			priv->s_data->g_ctx.csi_mode = GMSL_CSI_2X2_MODE;
		else {
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

		err = imx900_gmsl_serdes_setup(priv);
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

	err = imx900_power_on(s_data);
	if (err) {
		dev_err(dev,
		"Error %d during power on sensor\n", err);
		return err;
	}

	err = imx900_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
								__func__);
		goto error2;
	}

	err = imx900_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

error2:
	imx900_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx900_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s:++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx900_subdev_internal_ops = {
	.open = imx900_open,
};

static struct tegracam_ctrl_ops imx900_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx900_set_gain,
	.set_exposure = imx900_set_exposure,
	.set_frame_rate = imx900_set_frame_rate,
	.set_group_hold = imx900_set_group_hold,
	.set_test_pattern = imx900_set_test_pattern,
	.set_data_rate = imx900_set_data_rate,
	.set_operation_mode = imx900_set_operation_mode,
	.set_shutter_mode = imx900_set_shutter_mode,
	.set_black_level = imx900_set_black_level,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx900_probe(struct i2c_client *client)
#else
static int imx900_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx900 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx900), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx900", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx900_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx900_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx900_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	priv->frame_length = IMX900_DEFAULT_HEIGHT + IMX900_MIN_FRAME_DELTA;
	priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG12_1X12;
	priv->line_time = IMX900_DEFAULT_LINE_TIME;
	priv->min_frame_length = IMX900_DEFAULT_HEIGHT + IMX900_MIN_FRAME_DELTA;
	priv->s_data->def_mode = IMX900_MODE_2064x1552;

	ctrlprops = &priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx900_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx900_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					IMX900_DEFAULT_BLACK_LEVEL_12BPP,
					IMX900_DEFAULT_BLACK_LEVEL_12BPP,
					0, IMX900_MAX_BLACK_LEVEL_12BPP);
	if (err)
		return err;

	err = imx900_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				(ARRAY_SIZE(imx900_test_pattern_menu)-1));
	if (err)
		return err;

	err = imx900_update_ctrl(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, 0, 0, 0,
					(ARRAY_SIZE(imx900_data_rate_menu)-1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx900_sensor_list);

	dev_info(dev, "Detected imx900 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx900_remove(struct i2c_client *client)
#else
static void imx900_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx900 *priv;

	if (!s_data) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx900 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx900_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx900_id[] = {
	{ "imx900", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx900_id);

static struct i2c_driver imx900_i2c_driver = {
	.driver = {
		.name = "imx900",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx900_of_match),
	},
	.probe = imx900_probe,
	.remove = imx900_remove,
	.id_table = imx900_id,
};

module_i2c_driver(imx900_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX900");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
