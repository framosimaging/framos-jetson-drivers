// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx568.c - Framos fr_imx568.c driver
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

#include "fr_imx568_mode_tbls.h"
#include "media/fr_sensor_common.h"


#define IMX568_K_FACTOR					1000LL
#define IMX568_M_FACTOR					1000000LL
#define IMX568_G_FACTOR					1000000000LL
#define IMX568_T_FACTOR					1000000000000LL

#define IMX568_MAX_GAIN_DEC				480
#define IMX568_MAX_GAIN_DB				48

#define IMX568_MAX_BLACK_LEVEL_8BPP			255
#define IMX568_MAX_BLACK_LEVEL_10BPP			1023
#define IMX568_MAX_BLACK_LEVEL_12BPP			4095
#define IMX568_DEFAULT_BLACK_LEVEL_8BPP			15
#define IMX568_DEFAULT_BLACK_LEVEL_10BPP		60
#define IMX568_DEFAULT_BLACK_LEVEL_12BPP		240

#define IMX568_DEFAULT_LINE_TIME			6760 /* [ns] */

#define IMX568_MIN_SHS_LENGTH				36
#define IMX568_INTEGRATION_OFFSET_NORMAL_MODE		2
#define IMX568_MIN_INTEGRATION_LINES			1

#define IMX568_INCK					74250000LL

#define IMX568_MAX_CSI_LANES				4
#define IMX568_TWO_LANE_MODE				2


LIST_HEAD(imx568_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx568_of_match[] = {
	{
	.compatible = "framos,imx568",
	},
	{},
};
MODULE_DEVICE_TABLE(of, imx568_of_match);

const char *const imx568_data_rate_menu[] = {
	[IMX568_1188_MBPS] = "1188 Mbps/lane",
	[IMX568_891_MBPS] = "891 Mbps/lane",
	[IMX568_594_MBPS] = "594 Mbps/lane",
};

static const char *const imx568_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Sequence Pattern 1",
	[2] = "Sequence Pattern 2",
	[3] = "Gradation Pattern",
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

struct imx568 {
	struct i2c_client *i2c_client;
	struct v4l2_subdev *subdev;
	u64 frame_length;
	u64 min_frame_length;
	u64 current_pixel_format;
	u32 line_time;
	struct list_head entry;
	struct mutex pw_mutex;
	struct camera_common_data *s_data;
	struct tegracam_device *tc_dev;
	u8 chromacity;
};

static bool imx568_is_volatile_reg(struct device *dev, unsigned int reg)
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
	.volatile_reg = imx568_is_volatile_reg,
};

static inline int imx568_read_reg(struct camera_common_data *s_data, u16 addr,
				u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx568_write_reg(struct camera_common_data *s_data, u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "%s: i2c write failed, 0x%x = %x\n", __func__,
			addr, val);

	return err;
}

static int imx568_read_buffered_reg(struct camera_common_data *s_data,
					u16 addr_low, u8 number_of_registers,
					u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;

	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx568_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
				__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx568_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx568_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
				__func__);
			return err;
		}
	}

	return err;
}

static int imx568_write_buffered_reg(struct camera_common_data *s_data,
					 u16 addr_low, u8 number_of_registers,
					 u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx568_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx568_write_reg(s_data, addr_low + i,
						(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
				__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx568_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx568_write_table(struct imx568 *priv, const imx568_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap, table, NULL, 0,
					 IMX568_TABLE_WAIT_MS,
					 IMX568_TABLE_END);
}

static int imx568_chromacity_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx568 *priv = (struct imx568 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 chromacity;

	err = imx568_write_reg(s_data, STANDBY, 0x00);
	if (err) {
		dev_err(dev, "%s: error canceling standby mode\n", __func__);
		return err;
	}
	usleep_range(7000, 7100);

	err = imx568_read_reg(s_data, CHROMACITY, &chromacity);
	if (err) {
		dev_err(dev,
			"%s: error reading chromacity information register\n",
			__func__);
		return err;
	}

	err = imx568_write_reg(s_data, STANDBY, 0x01);
	if (err) {
		dev_err(dev, "%s: error setting standby mode\n", __func__);
		return err;
	}
	usleep_range(7000, 7100);

	chromacity = chromacity >> 7;
	priv->chromacity = chromacity;

	dev_dbg(dev, "%s: sensor is color(0)/monochrome(1): %d\n", __func__,
		chromacity);

	return err;
}

static int imx568_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx568_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx568_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
				u64 max_val)
{
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
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
			ctrl->qmenu = imx568_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		case TEGRA_CAMERA_CID_DATA_RATE:
			ctrl->qmenu = imx568_data_rate_menu;
			ctrl->maximum = max_val;
			break;
		}
	}
	return 0;
}

static int imx568_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx568_write_buffered_reg(s_data, BLKLEVEL_LOW, 2, val);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx568_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err;
	u32 gain;

	gain = val * IMX568_MAX_GAIN_DEC /
		(IMX568_MAX_GAIN_DB * mode->control_properties.gain_factor);

	err = imx568_write_buffered_reg(s_data, GAIN_LOW, 2, gain);
	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s:gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx568_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx568 *priv = (struct imx568 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err;
	u32 integration_time_line;
	u32 integration_offset = IMX568_INTEGRATION_OFFSET_NORMAL_MODE;
	u32 reg_shs;
	u8 min_reg_shs;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line =
		((val - integration_offset) * IMX568_K_FACTOR) / priv->line_time;

	reg_shs = priv->frame_length - integration_time_line;

	imx568_read_reg(s_data, GMTWT, &min_reg_shs);

	if (reg_shs < min_reg_shs)
		reg_shs = min_reg_shs;
	else if (reg_shs > (priv->frame_length - IMX568_MIN_INTEGRATION_LINES))
		reg_shs = priv->frame_length - IMX568_MIN_INTEGRATION_LINES;

	err = imx568_write_buffered_reg(s_data, SHS_LOW, 3, reg_shs);
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

static int imx568_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx568 *priv = (struct imx568 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err;
	u8 min_reg_shs;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length = (((u64)mode->control_properties.framerate_factor * IMX568_G_FACTOR) /
						(val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	priv->frame_length = frame_length;

	imx568_read_reg(s_data, GMTWT, &min_reg_shs);

	exposure_min_range = IMX568_MIN_INTEGRATION_LINES * priv->line_time /
								IMX568_K_FACTOR;
	exposure_min_range += IMX568_INTEGRATION_OFFSET_NORMAL_MODE;
	exposure_max_range = (priv->frame_length - min_reg_shs) *
					priv->line_time / IMX568_K_FACTOR;
	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				 exposure_min_range, exposure_max_range);

	err =
		imx568_write_buffered_reg(s_data, VMAX_LOW, 3, priv->frame_length);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: val: %lld, frame_length set: %llu\n", __func__, val,
		priv->frame_length);

	return 0;
}

static int imx568_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err = imx568_write_reg(s_data, 0x3550, 0x07);
		if (err)
			goto fail;

		err = imx568_write_reg(s_data, 0x3551, (u8)(val));
		if (err)
			goto fail;
	} else {
		err = imx568_write_reg(s_data, 0x3550, 0x06);
		if (err)
			goto fail;
	}

	return 0;

fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx568_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	struct device *dev = tc_dev->dev;
	u64 max_framerate;
	u8 gmrwt, gmtwt, gsdly;
	int err;

	err = imx568_read_reg(s_data, GMRWT, &gmrwt);
	err |= imx568_read_reg(s_data, GMTWT, &gmtwt);
	err |= imx568_read_reg(s_data, GSDLY, &gsdly);

	if (err) {
		dev_err(dev,
			"%s: error reading gmrtw, gmtwt, gsdly registers\n",
			__func__);
		return err;
	}

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	switch (s_data->mode) {
	case IMX568_MODE_2472x2064:
		priv->min_frame_length =
			IMX568_DEFAULT_HEIGHT + gmrwt + gmtwt + gsdly + 86;
		break;
	case IMX568_MODE_ROI_1920x1080:
	case IMX568_MODE_ROI_1280x720:
		priv->min_frame_length =
			IMX568_ROI_MODE_HEIGHT + gmrwt + gmtwt + gsdly + 86;
		break;
	case IMX568_MODE_BINNING_1236x1032:
		priv->min_frame_length =
			IMX568_BINNING_MODE_HEIGHT + gmrwt + gmtwt + gsdly + 56;
		break;
	}

	max_framerate = (IMX568_G_FACTOR * IMX568_M_FACTOR) /
			(priv->min_frame_length * priv->line_time);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				 ctrlprops->min_framerate, max_framerate);

	return 0;
}

static int imx568_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	return 0;
}

static int imx568_set_shutter_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl =
		fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE);

	if (ctrl == NULL)
		return 0;

	if (fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE,
				CURRENT) == MASTER_MODE && val == SEQ_TRIGGER) {
		dev_warn(dev,
		"%s: Sequential trigger isn't supported in master mode\n",
		__func__);
		goto default_state;
	}

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
					CURRENT) == SLAVE_MODE &&
		val == FAST_TRIGGER) {
		dev_warn(dev,
			 "%s: Fast trigger isn't supported in slave mode\n",
			 __func__);
		goto default_state;
	}

	return 0;

default_state:
	*ctrl->p_new.p_s64 = NORMAL_EXPO;
	*ctrl->p_cur.p_s64 = NORMAL_EXPO;
	return 0;
}

static int imx568_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx568 *priv = (struct imx568 *)s_data->priv;

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
		goto imx568_mclk_fail;
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

	/* Additional sleep required in the case of hardware power-on sequence*/
	usleep_range(30000, 31000);

	mutex_unlock(&priv->pw_mutex);

	return 0;

imx568_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx568_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx568 *priv = (struct imx568 *)s_data->priv;
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

static int imx568_power_get(struct tegracam_device *tc_dev)
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

		if (pdata->use_cam_gpio) {
			err = cam_gpio_register(dev, pw->reset_gpio);
			if (err) {
				dev_err(dev,
					"%s ERR can't register cam gpio %u!\n",
					__func__, pw->reset_gpio);
				goto done;
			}
		}
	}

done:
	pw->state = SWITCH_OFF;
	return err;
}

static int imx568_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = tc_dev->dev;

	if (unlikely(!pw))
		return -EFAULT;

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pdata && pdata->use_cam_gpio) {
			cam_gpio_deregister(dev, pw->reset_gpio);
		} else {
			if (pw->reset_gpio)
				gpio_free(pw->reset_gpio);
		}
	}

	return 0;
}

static int imx568_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx568_read_buffered_reg(s_data, VMAX_LOW, 2, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}
	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *
imx568_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx568_of_match, dev);
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

static int imx568_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB8_1X8:
		err = imx568_write_table(priv, mode_table[IMX568_8BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx568_write_table(priv, mode_table[IMX568_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx568_write_table(priv, mode_table[IMX568_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx568_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == IMX568_TWO_LANE_MODE) {
		err = imx568_write_reg(s_data, LANESEL, 3);
		if (err) {
			dev_err(dev, "%s: error setting two lane mode\n",
				__func__);
			return err;
		}
	} else if (s_data->numlanes == IMX568_MAX_CSI_LANES) {
		err = imx568_write_reg(s_data, LANESEL, 2);
		if (err) {
			dev_err(dev, "%s: error setting four lane mode\n",
				__func__);
			return err;
		}
	}

	dev_dbg(dev, "%s: sensor is in %d CSI lane mode\n", __func__,
		s_data->numlanes);

	return 0;
}

static int imx568_set_dep_registers(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 gmrwt, gmtwt, gsdly;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
						CURRENT)) {
	case IMX568_1188_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0C : 0x08)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x06 : 0x04);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x4C : 0x2C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x2A : 0x16);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x20 : 0x14)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x12 : 0x0A);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0C : 0x08)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x06 : 0x04);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x40 : 0x24)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x22 : 0x12);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x1C : 0x10)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x10 : 0x08);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x38 : 0x20)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x1E : 0x10);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x18 : 0x10)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0E : 0x08);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX568_891_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x08)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x06 : 0x04);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x38 : 0x20)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x20 : 0x12);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x18 : 0x10)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0E : 0x08);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x30 : 0x1C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x1A : 0x0E);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x14 : 0x0C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0C : 0x06);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x28 : 0x18)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x16 : 0x0C);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x14 : 0x0C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0A : 0x06);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX568_594_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x28 : 0x18)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x16 : 0x0C);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x14 : 0x0C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0A : 0x06);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x20 : 0x14)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x12 : 0x0A);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x10 : 0x08)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			gmrwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x04 : 0x04)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x02 : 0x02);
			gmtwt = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x1C : 0x10)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x10 : 0x08);
			gsdly = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x0C : 0x08)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
								? 0x08 : 0x04);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	default:
		return 0;
	}

	err = imx568_write_reg(s_data, GMRWT, gmrwt);
	err |= imx568_write_reg(s_data, GMTWT, gmtwt);
	err |= imx568_write_reg(s_data, GSDLY, gsdly);
	if (err) {
		dev_err(dev, "%s: error setting exposure mode\n", __func__);
		return err;
	}

	return err;
}

static int imx568_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx568 *priv = (struct imx568 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx568_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax * IMX568_G_FACTOR) / (IMX568_INCK);

	dev_dbg(dev,
		"%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

static int imx568_adjust_hmax_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 hmax;

	dev_dbg(dev, "%s:++\n", __func__);

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
						CURRENT)) {
	case IMX568_1188_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0xC2 : 0x15C)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x15C : 0x290);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0xE8 : 0x1A9)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x1A9 : 0x32A);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x10F : 0x1F7)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x1F6 : 0x3C5);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX568_891_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0xFE : 0x1CB)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x1CC : 0x366);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x131 : 0x230)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x232 : 0x432);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x164 : 0x298)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x298 : 0x500);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	case IMX568_594_MBPS:
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x176 : 0x2A9)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x2AA : 0x511);
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x1C2 : 0x341)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x343 : 0x634);
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			hmax = (s_data->mode == IMX568_MODE_BINNING_1236x1032)
				? (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x20F : 0x3DC)
				: (s_data->numlanes == IMX568_MAX_CSI_LANES
							? 0x3DD : 0x778);
			break;
		default:
			dev_err(dev, "%s: unknown pixel format\n", __func__);
			return 0;
		}
		break;
	default:
		return 0;
	}

	err = imx568_write_buffered_reg(s_data, HMAX_LOW, 2, hmax);
	if (err) {
		dev_err(dev, "%s: failed to set HMAX register\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s:HMAX: %llu\n", __func__, hmax);

	return 0;
}

static int imx568_set_data_rate(struct tegracam_device *tc_dev, u32 val)
{
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	if (s_data->power->state == SWITCH_ON) {
		err = imx568_write_table(priv,
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

static int imx568_configure_shutter(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 trigen = 0;
	u8 vint_en = 0;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE,
								CURRENT)) {
	case NORMAL_EXPO:
		trigen = 0;
		vint_en = 2;
		dev_dbg(dev, "%s: Sensor is in Normal Exposure Mode\n",
			__func__);
		break;

	case SEQ_TRIGGER:
		if (fr_get_v4l2_ctrl_value(tc_dev,
					TEGRA_CAMERA_CID_OPERATION_MODE,
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
					TEGRA_CAMERA_CID_OPERATION_MODE,
					CURRENT) == SLAVE_MODE) {
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

	err = imx568_write_reg(s_data, TRIGMODE_TIMMING, trigen);
	err |= imx568_write_reg(s_data, VINT_EN, vint_en | 0x30);
	if (err) {
		dev_err(dev, "%s: error setting exposure mode\n", __func__);
		return err;
	}

	return 0;
}

static int imx568_configure_triggering_pins(struct tegracam_device *tc_dev)
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

	err = imx568_write_reg(s_data, SYNCSEL, sync_sel);
	if (err) {
		dev_err(dev, "%s: error setting Slave mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__, sync_sel);

	return 0;
}

static int imx568_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx568 *priv = (struct imx568 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB8_1X8:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12)
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 >> 4),
					IMX568_DEFAULT_BLACK_LEVEL_8BPP, 0,
					IMX568_MAX_BLACK_LEVEL_8BPP);
			else
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 >> 2),
					IMX568_DEFAULT_BLACK_LEVEL_8BPP, 0,
					IMX568_MAX_BLACK_LEVEL_8BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB8_1X8;
			break;
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12)
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 >> 2),
					IMX568_DEFAULT_BLACK_LEVEL_10BPP, 0,
					IMX568_MAX_BLACK_LEVEL_10BPP);
			else
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 << 2),
					IMX568_DEFAULT_BLACK_LEVEL_10BPP, 0,
					IMX568_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB10_1X10)
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 << 2),
					IMX568_DEFAULT_BLACK_LEVEL_12BPP, 0,
					IMX568_MAX_BLACK_LEVEL_12BPP);
			else
				err = imx568_update_ctrl(
					tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					(*ctrl->p_cur.p_s64 << 4),
					IMX568_DEFAULT_BLACK_LEVEL_12BPP, 0,
					IMX568_MAX_BLACK_LEVEL_12BPP);
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

static int imx568_set_mode(struct tegracam_device *tc_dev)
{
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx568_chromacity_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to get chromacity information\n",
			__func__);
		return err;
	}
	if (priv->chromacity == IMX568_COLOR) {
		err = imx568_write_reg(s_data, 0x3797, 0x20);
		if (err) {
			dev_err(dev, "%s: error setting register 0x3797\n",
				__func__);
			return err;
		}
	}

	err = imx568_write_table(priv, mode_table[IMX568_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
			__func__);
		return err;
	}

	err = imx568_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = imx568_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
			__func__);
		return err;
	}

	err = imx568_write_table(priv, mode_table[s_data->mode]);
	if (err) {
		dev_err(dev, "%s: unable to set sensor mode settings\n",
			__func__);
		return err;
	}

	err = imx568_set_operation_mode(tc_dev,
		fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
								CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}

	err = imx568_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx568_set_data_rate(tc_dev,
		fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_DATA_RATE,
								CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set data rate\n", __func__);
		return err;
	}

	err = imx568_set_dep_registers(tc_dev);
	if (err) {
		dev_err(dev,
			"%s: unable to write dep registers to image sensor\n",
			__func__);
		return err;
	}

	err = imx568_set_test_pattern(
		tc_dev, fr_get_v4l2_ctrl_value(
			tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx568_adjust_hmax_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust hmax\n", __func__);
		return err;
	}

	err = imx568_configure_shutter(tc_dev);
	if (err)
		goto fail;

	s_data->override_enable = true;

	err = imx568_calculate_line_time(tc_dev);
	if (err)
		goto fail;

	err = imx568_update_framerate_range(tc_dev);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;

fail:
	dev_err(dev, "%s: unable to set mode\n", __func__);
	return err;
}

static int imx568_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
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

	err = imx568_write_reg(s_data, STANDBY, 0x00);

	usleep_range(7000, 7100);

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT) == MASTER_MODE)
		xmsta = 0x00;
	else
		xmsta = 0x01;

	err = imx568_write_reg(s_data, XMSTA, xmsta);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int imx568_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx568 *priv = (struct imx568 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx568_write_table(priv, mode_table[IMX568_MODE_STOP_STREAM]);
	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX568_K_FACTOR,
		priv->frame_length * priv->line_time / IMX568_K_FACTOR + 1000);

	return 0;
}

static struct camera_common_sensor_ops imx568_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx568_frmfmt),
	.frmfmt_table = imx568_frmfmt,
	.power_on = imx568_power_on,
	.power_off = imx568_power_off,
	.write_reg = imx568_write_reg,
	.read_reg = imx568_read_reg,
	.parse_dt = imx568_parse_dt,
	.power_get = imx568_power_get,
	.power_put = imx568_power_put,
	.set_mode = imx568_set_mode,
	.start_streaming = imx568_start_streaming,
	.stop_streaming = imx568_stop_streaming,
	.after_set_pixel_format = imx568_after_set_pixel_format,
};

static int imx568_gmsl_serdes_setup(struct imx568 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev ||
		!priv->i2c_client)
		return -EINVAL;

	dev = &priv->i2c_client->dev;

	dev_dbg(dev, "%s : Entered function\n", __func__);

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

static void imx568_gmsl_serdes_reset(struct imx568 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx568_board_setup(struct imx568 *priv)
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
			dev_err(dev,
				"missing %s handle\n nvidia",
							"gmsl-dser-device");
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

		err = imx568_gmsl_serdes_setup(priv);
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

	err = imx568_power_on(s_data);
	if (err) {
		dev_err(dev, "Error %d during power on sensor\n", err);
		return err;
	}

	err = imx568_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
			__func__);
		goto error2;
	}

error2:
	imx568_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx568_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "imx568 : open:\n");

	return 0;
}

static const struct v4l2_subdev_internal_ops imx568_subdev_internal_ops = {
	.open = imx568_open,
};

static struct tegracam_ctrl_ops imx568_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx568_set_gain,
	.set_exposure = imx568_set_exposure,
	.set_frame_rate = imx568_set_frame_rate,
	.set_group_hold = imx568_set_group_hold,
	.set_test_pattern = imx568_set_test_pattern,
	.set_data_rate = imx568_set_data_rate,
	.set_operation_mode = imx568_set_operation_mode,
	.set_black_level = imx568_set_black_level,
	.set_shutter_mode = imx568_set_shutter_mode,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx568_probe(struct i2c_client *client)
#else
static int imx568_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx568 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev, sizeof(struct imx568), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev, sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx568", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx568_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx568_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx568_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	priv->frame_length = IMX568_DEFAULT_HEIGHT + IMX568_MIN_FRAME_DELTA;
	priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB12_1X12;
	priv->line_time = IMX568_DEFAULT_LINE_TIME;
	priv->min_frame_length = IMX568_DEFAULT_HEIGHT + IMX568_MIN_FRAME_DELTA;
	priv->s_data->def_mode = IMX568_MODE_2472x2064;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx568_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx568_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
				 IMX568_DEFAULT_BLACK_LEVEL_12BPP,
				 IMX568_DEFAULT_BLACK_LEVEL_12BPP, 0,
				 IMX568_MAX_BLACK_LEVEL_12BPP);
	if (err)
		return err;

	err = imx568_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				 (ARRAY_SIZE(imx568_test_pattern_menu) - 1));
	if (err)
		return err;

	err = imx568_update_ctrl(tc_dev, TEGRA_CAMERA_CID_DATA_RATE, 0, 0, 0,
				 (ARRAY_SIZE(imx568_data_rate_menu) - 1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx568_sensor_list);

	dev_info(dev, "Detected imx568 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx568_remove(struct i2c_client *client)
#else
static void imx568_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx568 *priv;

	if (s_data == NULL) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx568 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx568_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx568_id[] = {{"imx568", 0}, {}};

MODULE_DEVICE_TABLE(i2c, imx568_id);

static struct i2c_driver imx568_i2c_driver = {
	.driver = {
		.name = "imx568",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx568_of_match),
	},
	.probe = imx568_probe,
	.remove = imx568_remove,
	.id_table = imx568_id,
};

module_i2c_driver(imx568_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX568");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
