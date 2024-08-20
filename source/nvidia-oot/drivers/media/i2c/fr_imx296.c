// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx296.c - Framos fr_imx296.c driver
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

#include "fr_imx296_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX296_K_FACTOR 1000LL
#define IMX296_M_FACTOR 1000000LL
#define IMX296_G_FACTOR 1000000000LL
#define IMX296_T_FACTOR 1000000000000LL

#define IMX296_MAX_GAIN_DEC 480
#define IMX296_MAX_GAIN_DB 48

#define IMX296_MAX_BLACK_LEVEL 511
#define IMX296_DEFAULT_BLACK_LEVEL 60

#define IMX296_MIN_SHR0_LENGTH 4
#define IMX296_MIN_INTEGRATION_LINES 1
#define IMX296_INTEGRATION_OFFSET 14

#define IMX296_MAX_CSI_LANES 4
#define IMX296_TWO_LANE_MODE 2

#define IMX296_INCK 74250000LL


LIST_HEAD(imx296_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx296_of_match[] = {
	{ .compatible = "framos,imx296",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx296_of_match);

static const char * const imx296_test_pattern_menu[] = {
	[0] = "No Pattern",
	[1] = "Sequence 1",
	[2] = "Sequence 2",
	[3] = "Gradiation",
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

struct imx296 {
	struct i2c_client		*i2c_client;
	struct v4l2_subdev		*subdev;
	u64				frame_length;
	u64				min_frame_length;
	u32				line_time;
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

static inline int imx296_read_reg(struct camera_common_data *s_data,
							u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx296_write_reg(struct camera_common_data *s_data,
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

static int imx296_read_buffered_reg(struct camera_common_data *s_data,
				u16 addr_low, u8 number_of_registers, u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;
	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx296_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
								__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx296_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx296_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
								__func__);
			return err;
		}
	}

	return err;
}

static int imx296_write_buffered_reg(struct camera_common_data *s_data,
				u16 addr_low, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx296_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx296_write_reg(s_data, addr_low + i,
						(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx296_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx296_write_table(struct imx296 *priv, const imx296_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					 table,
					 NULL, 0,
					 IMX296_TABLE_WAIT_MS,
					 IMX296_TABLE_END);
}

static int imx296_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx296_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx296_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
								u64 max_val)
{
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
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
			ctrl->qmenu = imx296_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		}
	}
	return 0;
}

static int imx296_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx296_write_buffered_reg(s_data, BLKLEVEL_LOW, 2, val);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;
}

static int imx296_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err;
	u32 gain;

	gain = val * IMX296_MAX_GAIN_DEC /
				(IMX296_MAX_GAIN_DB *
					mode->control_properties.gain_factor);

	err = imx296_write_buffered_reg(s_data, GAIN_LOW, 2, gain);
	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: gain val [%lld] reg [%d]\n", __func__, val, gain);

	return 0;
}

static int imx296_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx296 *priv = (struct imx296 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err;
	u32 integration_time_line;
	u32 integration_offset = IMX296_INTEGRATION_OFFSET;
	u32 reg_shs;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;


	integration_time_line = ((val - integration_offset)
					* IMX296_K_FACTOR) / priv->line_time;

	reg_shs = priv->frame_length - integration_time_line;

	if (reg_shs < IMX296_MIN_SHR0_LENGTH)
		reg_shs = IMX296_MIN_SHR0_LENGTH;
	else if (reg_shs > (priv->frame_length - IMX296_MIN_INTEGRATION_LINES))
		reg_shs = priv->frame_length - IMX296_MIN_INTEGRATION_LINES;

	err = imx296_write_buffered_reg(s_data, SHS1_LOW, 3, reg_shs);
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
		"%s: set integration time: %lld [us], coarse1:%d [line], shs: %d [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, reg_shs, priv->frame_length);

	return err;
}

static int imx296_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx296 *priv = (struct imx296 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err;
	u64 frame_length;
	u64 exposure_max_range, exposure_min_range;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length = (((u64)mode->control_properties.framerate_factor *
				IMX296_G_FACTOR) / (val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	priv->frame_length = frame_length;

	exposure_min_range = IMX296_MIN_INTEGRATION_LINES
					* priv->line_time / IMX296_K_FACTOR;
	exposure_min_range += IMX296_INTEGRATION_OFFSET;

	exposure_max_range = (priv->frame_length - IMX296_MIN_SHR0_LENGTH)
					* priv->line_time / IMX296_K_FACTOR;
	exposure_max_range += IMX296_INTEGRATION_OFFSET;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				exposure_min_range, exposure_max_range);

	err = imx296_write_buffered_reg(s_data, VMAX_LOW, 3, priv->frame_length);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: val: %lld, frame_length set: %llu\n", __func__,
						val, priv->frame_length);

	return 0;
}

static int imx296_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (val) {
		err = imx296_write_reg(s_data, PATERN_GEN, (val << 3) + 7);
		if (err)
			goto fail;
	} else {
		err = imx296_write_reg(s_data, PATERN_GEN, 6);
		if (err)
			goto fail;
	}

	dev_dbg(dev, "%s++ Test mode pattern: %u\n", __func__, val-1);

	return 0;
fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx296_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	u64 max_framerate, min_framerate;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	priv->min_frame_length = s_data->fmt_height
					+ IMX296_MIN_FRAME_LENGTH_DELTA;

	max_framerate = (IMX296_G_FACTOR * IMX296_M_FACTOR) /
				(priv->min_frame_length * priv->line_time);

	min_framerate = ctrlprops->min_framerate;

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
						min_framerate, max_framerate);

	return 0;
}

static int imx296_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	return 0;
}

static int imx296_set_shutter_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl = fr_find_v4l2_ctrl(tc_dev,
						TEGRA_CAMERA_CID_SHUTTER_MODE);

	if (ctrl == NULL)
		return 0;

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
				CURRENT) == MASTER_MODE && val == SEQ_TRIGGER){
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

static int imx296_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx296 *priv = (struct imx296 *)s_data->priv;

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
		goto imx296_mclk_fail;
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

	mutex_unlock(&priv->pw_mutex);

	return 0;

imx296_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx296_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx296 *priv = (struct imx296 *)s_data->priv;
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

static int imx296_power_get(struct tegracam_device *tc_dev)
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
				dev_err(dev, "%s ERR can't register cam gpio %u!\n",
						 __func__, pw->reset_gpio);
			}
		}
	}

	pw->state = SWITCH_OFF;

	return err;
}

static int imx296_power_put(struct tegracam_device *tc_dev)
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

static int imx296_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 vmax;

	err = imx296_read_buffered_reg(s_data, VMAX_LOW, 2, &vmax);
	if (err) {
		dev_err(dev, "%s: failed to read VMAX\n", __func__);
		return err;
	}
	priv->frame_length = vmax;

	return err;
}

static struct camera_common_pdata *imx296_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx296_of_match, dev);
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

static int imx296_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx296 *priv = (struct imx296 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 hmax;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx296_read_buffered_reg(s_data, HMAX_LOW, 2, &hmax);
	if (err) {
		dev_err(dev, "%s: unable to read hmax\n", __func__);
		return err;
	}

	priv->line_time = (hmax*IMX296_G_FACTOR) / (IMX296_INCK);

	dev_dbg(dev, "%s: hmax: %llu [inck], INCK: %u [Hz], line_time: %u [ns]\n",
		__func__, hmax, s_data->def_clk_freq, priv->line_time);

	return 0;
}

static int imx296_configure_shutter(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u8 trigen = 0;
	u8 trigger_sel = 0;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_SHUTTER_MODE,
								CURRENT)) {
	case NORMAL_EXPO:
		trigen = 0;
		trigger_sel = 0;
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
		trigen = 1;
		trigger_sel = 0;
		dev_dbg(dev, "%s: Sensor is in Sequential Trigger Mode\n", __func__);
		break;

	case FAST_TRIGGER:
		if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
							CURRENT) == SLAVE_MODE) {
			dev_warn(dev,
				"%s: Fast Trigger Mode not supported in Slave mode, switchig to default\n",
				__func__);
			break;
		}
		trigen = 1;
		trigger_sel = 1;
		dev_dbg(dev, "%s: Sensor is in Fast Trigger Mode\n", __func__);
		break;
	default:
		pr_err("%s: unknown exposure mode.\n", __func__);
		return -EINVAL;
	}

	err = imx296_write_reg(s_data, TRIGEN, trigen);
	err |= imx296_write_reg(s_data, LOWLAGTRG, trigger_sel);
	if (err) {
		dev_err(dev, "%s: error setting exposure mode\n", __func__);
		return err;
	}

	return 0;
}

static int imx296_configure_triggering_pins(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u8 xvs_xhs_drv = 0x30;

	switch (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
								CURRENT)) {
	case MASTER_MODE:
		xvs_xhs_drv = 0;
		dev_dbg(dev, "%s: Sensor is in Master mode\n", __func__);
		break;
	case SLAVE_MODE:
		xvs_xhs_drv = 0x30;
		dev_dbg(dev, "%s: Sensor is in Slave mode\n", __func__);
		break;
	default:
		pr_err("%s: unknown operating mode.\n", __func__);
		return -EINVAL;
	}

	err = imx296_write_reg(s_data, SYNCSEL, xvs_xhs_drv);
	if (err) {
		dev_err(dev, "%s: error setting XVS XHS pin\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__, xvs_xhs_drv);

	return 0;
}

static int imx296_set_mode(struct tegracam_device *tc_dev)
{
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx296_write_table(priv, mode_table[IMX296_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
		return err;
	}

	err = imx296_write_table(priv, mode_table[s_data->mode]);
	if (err)
		return err;

	err = imx296_set_operation_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to operation mode\n", __func__);
		return err;
	}

	err = imx296_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}

	err = imx296_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = imx296_configure_shutter(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to configure exposure\n", __func__);
		return err;
	}

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	err = imx296_calculate_line_time(priv->tc_dev);
	if (err)
		return err;

	err = imx296_update_framerate_range(tc_dev);
	if (err)
		return err;

	dev_dbg(dev, "%s: set mode %u\n", __func__, s_data->mode);

	return 0;
}

static int imx296_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;
	u8 xmsta;

	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
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

	err = imx296_write_reg(s_data, STANDBY, 0x00);
	if (err)
		goto exit;

	usleep_range(29000, 29100);

	if (fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_OPERATION_MODE,
							CURRENT) == MASTER_MODE)
		xmsta = 0x00;
	else
		xmsta = 0x01;

	err = imx296_write_reg(s_data, XMSTA, xmsta);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);
	return err;
}

static int imx296_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx296 *priv = (struct imx296 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!strcmp(s_data->pdata->gmsl, "gmsl"))
		max96792_stop_streaming(priv->s_data->dser_dev, dev);

	err = imx296_write_table(priv, mode_table[IMX296_MODE_STOP_STREAM]);
	if (err)
		return err;

	usleep_range(priv->frame_length * priv->line_time / IMX296_K_FACTOR,
		priv->frame_length * priv->line_time / IMX296_K_FACTOR + 1000);

	return 0;
}

static struct camera_common_sensor_ops imx296_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx296_frmfmt),
	.frmfmt_table = imx296_frmfmt,
	.power_on = imx296_power_on,
	.power_off = imx296_power_off,
	.write_reg = imx296_write_reg,
	.read_reg = imx296_read_reg,
	.parse_dt = imx296_parse_dt,
	.power_get = imx296_power_get,
	.power_put = imx296_power_put,
	.set_mode = imx296_set_mode,
	.start_streaming = imx296_start_streaming,
	.stop_streaming = imx296_stop_streaming,
};

static int imx296_gmsl_serdes_setup(struct imx296 *priv)
{
	int err = 0;
	int des_err = 0;
	struct device *dev;

	if (!priv || !priv->s_data->ser_dev || !priv->s_data->dser_dev ||
							!priv->i2c_client)
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

static void imx296_gmsl_serdes_reset(struct imx296 *priv)
{
	mutex_lock(&serdes_lock__);

	/* reset serdes addressing and control pipeline */
	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx296_board_setup(struct imx296 *priv)
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

		err = imx296_gmsl_serdes_setup(priv);
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

	err = imx296_power_on(s_data);
	if (err) {
		dev_err(dev,
			"Error %d during power on sensor\n", err);
		return err;
	}

	err = imx296_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
								__func__);
		goto error;
	}

	err = imx296_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error;
	}

	priv->min_frame_length = IMX296_DEFAULT_HEIGHT
						+ IMX296_MIN_FRAME_LENGTH_DELTA;

error:
	imx296_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int imx296_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s:++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx296_subdev_internal_ops = {
	.open = imx296_open,
};

static struct tegracam_ctrl_ops imx296_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx296_set_gain,
	.set_exposure = imx296_set_exposure,
	.set_frame_rate = imx296_set_frame_rate,
	.set_group_hold = imx296_set_group_hold,
	.set_test_pattern = imx296_set_test_pattern,
	.set_operation_mode = imx296_set_operation_mode,
	.set_black_level = imx296_set_black_level,
	.set_shutter_mode = imx296_set_shutter_mode,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int imx296_probe(struct i2c_client *client)
#else
static int imx296_probe(struct i2c_client *client, const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx296 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx296), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx296", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx296_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx296_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx296_ctrl_ops;

	err = tegracam_device_register(tc_dev);
	if (err) {
		dev_err(dev, "tegra camera driver registration failed\n");
		return err;
	}
	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);


	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	INIT_LIST_HEAD(&priv->entry);

	err = imx296_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx296_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					IMX296_DEFAULT_BLACK_LEVEL,
					IMX296_DEFAULT_BLACK_LEVEL, 0,
					IMX296_MAX_BLACK_LEVEL);
	if (err)
		return err;

	err = imx296_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
					(ARRAY_SIZE(imx296_test_pattern_menu)-1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx296_sensor_list);

	dev_info(dev, "Detected imx296 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int imx296_remove(struct i2c_client *client)
#else
static void imx296_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx296 *priv;

	if (s_data == NULL) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct imx296 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx296_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id imx296_id[] = {
	{ "imx296", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx296_id);

static struct i2c_driver imx296_i2c_driver = {
	.driver = {
		.name = "imx296",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx296_of_match),
	},
	.probe = imx296_probe,
	.remove = imx296_remove,
	.id_table = imx296_id,
};

module_i2c_driver(imx296_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX296");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
