// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2026 Framos. All rights reserved.
 *
 * fr_vb1943.c - Framos fr_vb1943.c driver
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
#include <linux/ktime.h>
#include <media/fr_max96793.h>
#include <media/fr_max96792.h>

#include <media/tegra_v4l2_camera.h>
#include <media/tegracam_core.h>

#include "fr_vb1943_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define VB1943_H_FACTOR 100LL
#define VB1943_K_FACTOR 1000LL
#define VB1943_M_FACTOR 1000000LL
#define VB1943_G_FACTOR 1000000000LL
#define VB1943_T_FACTOR 1000000000000LL

#define VB1943_MIN_AGAIN_DEC 100 // 1 Times x factor 100
#define VB1943_MAX_AGAIN_DEC 400 // 4 Times x factor 100

#define VB1943_MIN_DGAIN_DEC 0 // 0 Times x factor 100
#define VB1943_MAX_DGAIN_DEC 3199 // 31.99 Times x factor 100
#define VB1943_DEF_DGAIN_DEC 100

#define VB1943_MAX_BLACK_LEVEL_10BPP 1023
#define VB1943_MAX_BLACK_LEVEL_12BPP 4095
#define VB1943_DEFAULT_BLACK_LEVEL_10BPP 8
#define VB1943_DEFAULT_BLACK_LEVEL_12BPP 32

#define VB1943_MIN_INTEGRATION_LINES_GS_MIN 4
#define VB1943_MIN_INTEGRATION_LINES_GS_MAX 28

#define VB1943_4_CSI_LANES 4
#define VB1943_TWO_LANE_MODE 2

#define VB1943_PIXEL_CLK 93750000LL

#define VB1943_DEFAULT_LINE_LENGTH	3372
#define VB1943_TWO_LANE_10BIT_LINE_LENGTH	6744
#define VB1943_TWO_LANE_12BIT_LINE_LENGTH	7500
#define VB1943_DEFAULT_FRAME_LENGTH	1112	// ((4*PIXEL_CLOCK)/(LINE_LENGTH*FPS))
#define VB1943_MAX_FRAME_LENGTH		0xFFFE	// 16-bit CTX0_FRAME_LENGTH, even

#define VB1943_GPIO_MODE_FSYNC_IN	0x05
#define VB1943_GPIO1_EN			BIT(1)	// GPIO1 enable bit in CTX0_GPIO_CTRL
#define VB1943_VT_CTRL_LEADER		0x00
#define VB1943_VT_CTRL_FOLLOWER		0x01

#define VB1943_FSM_SW_STBY		0x03	// SYSTEM_FSM_STATE values
#define VB1943_FSM_STREAMING		0x04

#define VB1943_CAMERA_CID_BASE				(V4L2_CTRL_CLASS_CAMERA | 0x6000)
#define VB1943_CAMERA_CID_MEASURE_TEMPERATURE		(VB1943_CAMERA_CID_BASE + 0)
#define VB1943_CAMERA_CID_SET_GS_RS_MODE		(VB1943_CAMERA_CID_BASE + 1)
#define VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE	(VB1943_CAMERA_CID_BASE + 2)


LIST_HEAD(vb1943_sensor_list);

static int vb1943_set_custom_ctrls(struct v4l2_ctrl *ctrl);
static const struct v4l2_ctrl_ops vb1943_custom_ctrl_ops = {
	.s_ctrl = vb1943_set_custom_ctrls,
};

static int vb1943_switch_shutter(struct tegracam_device *tc_dev, u32 gs_rs);
static int vb1943_enter_sw_standby(struct tegracam_device *tc_dev);
static int vb1943_exit_sw_standby(struct tegracam_device *tc_dev);

static struct mutex serdes_lock__;

static const struct of_device_id vb1943_of_match[] = {
	{ .compatible = "framos,vb1943",},
	{ },
};
MODULE_DEVICE_TABLE(of, vb1943_of_match);

static const char * const vb1943_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Diagonal Grayscale(DGREY)",
	[2] = "Pseudorandom(PN28)",
};

enum {

	VB1943_GS_MODE,
	VB1943_RS_MODE,
};

static const char *const gs_rs_mode_menu[] = {
	[VB1943_GS_MODE] = "Global Shutter",
	[VB1943_RS_MODE] = "Rolling Shutter",
};

enum {

	VB1943_RGBNIR_MODE,
	VB1943_RGB_MODE,
	VB1943_NIR_MODE,
};

static const char *const rgbnir_rgb_ir_mode_menu[] = {
	[VB1943_RGBNIR_MODE] = "RGB-NIR",
	[VB1943_RGB_MODE] = "RGB",
	[VB1943_NIR_MODE] = "NIR",
};

struct vb1943_again {
	u32	gain_x100;
	u8	reg_value;
};

static const struct vb1943_again vb1943_again_lut[] = {
	{ 100, 0x00 },
	{ 107, 0x01 },
	{ 114, 0x02 },
	{ 123, 0x03 },
	{ 133, 0x04 },
	{ 145, 0x05 },
	{ 160, 0x06 },
	{ 178, 0x07 },
	{ 200, 0x08 },
	{ 229, 0x09 },
	{ 267, 0x0A },
	{ 320, 0x0B },
	{ 400, 0x0C },
};

static struct v4l2_ctrl_config vb1943_custom_ctrl_list[] = {
	{
	.ops = &vb1943_custom_ctrl_ops,
	.id = VB1943_CAMERA_CID_MEASURE_TEMPERATURE,
	.name = "Measure sensor temperature",
	.type = V4L2_CTRL_TYPE_INTEGER64,
	.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
	.min = -32768,
	.max = 32767,
	.def = 0,
	.step = 1,
	},
	{
	.ops = &vb1943_custom_ctrl_ops,
	.id = VB1943_CAMERA_CID_SET_GS_RS_MODE,
	.name = "Global/Rolling Shutter Mode",
	.type = V4L2_CTRL_TYPE_MENU,
	.min = 0,
	.max = ARRAY_SIZE(gs_rs_mode_menu) - 1,
	.def = 0,
	.qmenu = gs_rs_mode_menu,
	},
	{
	.ops = &vb1943_custom_ctrl_ops,
	.id = VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE,
	.name = "RGB-NIR/RGB/NIR Mode",
	.type = V4L2_CTRL_TYPE_MENU,
	.min = 0,
	.max = ARRAY_SIZE(rgbnir_rgb_ir_mode_menu) - 1,
	.def = 0,
	.qmenu = rgbnir_rgb_ir_mode_menu,
	},
};


static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_GAIN,
	TEGRA_CAMERA_CID_DIGITAL_GAIN,
//	TEGRA_CAMERA_CID_CONVERSION_GAIN,
	TEGRA_CAMERA_CID_EXPOSURE,
	TEGRA_CAMERA_CID_FRAME_RATE,
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
	TEGRA_CAMERA_CID_OPERATION_MODE,
//	TEGRA_CAMERA_CID_SYNC_MODE,
//	TEGRA_CAMERA_CID_BROADCAST,
	TEGRA_CAMERA_CID_BLACK_LEVEL,
	TEGRA_CAMERA_CID_TEST_PATTERN,
//	TEGRA_CAMERA_CID_EXPOSURE_SHORT,
//	TEGRA_CAMERA_CID_HDR_EN,
};

struct vb1943 {
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

static bool vb1943_is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case TEMPERATURE_1:
	case TEMPERATURE_2:
	case SYSTEM_FSM_STATE:
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
	.volatile_reg = vb1943_is_volatile_reg,
};

/*static bool vb1943_is_binning_mode(struct camera_common_data *s_data)
{
	switch (s_data->mode) {
	case VB1943_MODE_H2V2_BINNING:
	case VB1943_MODE_DOL_BINNING:
		return true;
	default:
		return false;
	}
}*/

static inline int vb1943_read_reg(struct camera_common_data *s_data,
							u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int vb1943_write_reg(struct camera_common_data *s_data,
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

static int vb1943_write_reg_broadcast(struct camera_common_data *s_data,
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

static int vb1943_read_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;
	*val = 0;

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
								__func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = vb1943_read_reg(s_data, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
								__func__);
			return err;
		}
	}

	return err;
}

static int vb1943_write_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = vb1943_write_reg(s_data, addr_low + i,
						(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int vb1943_broadcast_buffered_reg(struct camera_common_data *s_data,
			u16 addr_low, u8 number_of_registers, u32 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg_broadcast(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = vb1943_write_reg_broadcast(s_data, addr_low + i,
			(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = vb1943_write_reg_broadcast(s_data, CTX0_GROUP_PARAMETER_HOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int vb1943_write_table(struct vb1943 *priv, const vb1943_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					 table,
					 NULL, 0,
					 VB1943_TABLE_WAIT_MS,
					 VB1943_TABLE_END);
}

static int vb1943_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = vb1943_write_reg(s_data, CTX0_GROUP_PARAMETER_HOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int vb1943_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
								u64 max_val)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
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
			ctrl->qmenu = vb1943_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		case VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE:
			*ctrl->p_new.p_s64 = current_val;
			*ctrl->p_cur.p_s64 = current_val;
			break;
		case VB1943_CAMERA_CID_SET_GS_RS_MODE:
			*ctrl->p_new.p_s64 = current_val;
			*ctrl->p_cur.p_s64 = current_val;
			break;
		case TEGRA_CAMERA_CID_DIGITAL_GAIN:
			*ctrl->p_new.p_s64 = default_val;
			*ctrl->p_cur.p_s64 = default_val;
			ctrl->default_value = default_val;
			ctrl->minimum = min_val;
			ctrl->maximum = max_val;
			break;
		}
	}

	return 0;
}

static int vb1943_read_temperature(struct tegracam_device *tc_dev, s16 *temperature)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	int err = 0;
	u64 reg_temp;

	if (priv->s_data->power->state == SWITCH_OFF)
		return 0;

	err = vb1943_read_buffered_reg(s_data, TEMPERATURE_1, 2, &reg_temp);
	if (err) {
		dev_err(dev, "%s: error reading Temperature_1 value register\n", __func__);
		return err;
	}

	*temperature = (s16)reg_temp * VB1943_H_FACTOR;

	dev_info(dev, "TEMPERATURE_1 value: %d\n", (s16)reg_temp);

	return err;
}

/*static int vb1943_in_dol_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_control control;
	int hdr_en = 0;
	int err;

	control.id = TEGRA_CAMERA_CID_HDR_EN;
	err = camera_common_g_ctrl(s_data, &control);
	if (err < 0) {
		dev_err(dev, "could not find hdr enable device ctrl.\n");
		return err;
	}
	hdr_en = switch_ctrl_qmenu[control.value];

	return hdr_en;
}*/

/* Rolling shutter needs twice the global shutter minimum frame length */
static void vb1943_set_min_frame_length(struct vb1943 *priv, u32 gs_rs)
{
	if (gs_rs == VB1943_GS_MODE)
		priv->min_frame_length = VB1943_DEFAULT_FRAME_LENGTH;
	else
		priv->min_frame_length = VB1943_DEFAULT_FRAME_LENGTH * 2;
}

static int vb1943_set_gs_rs_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	u64 prev_min_frame_length = priv->min_frame_length;
	int err;

	vb1943_set_min_frame_length(priv, val);

	/* Not streaming, applied by set_mode on the next stream start */
	if (!tc_dev->is_streaming)
		return 0;

	err = vb1943_switch_shutter(tc_dev, val);
	if (err)
		priv->min_frame_length = prev_min_frame_length;

	return err;
}

static int vb1943_set_rgbnir_rgb_ir_mode(struct tegracam_device *tc_dev, u32 val)
{

	return 0;
}

static int vb1943_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	s64 black_level_reg;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SGBRG10_1X10)
		black_level_reg = val << 2;
	else
		black_level_reg = val;

	err = vb1943_write_buffered_reg(s_data, CTX0_DARKCAL_PEDESTAL, 2, black_level_reg);
	if (err) {
		dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: black level control value: %lld, black level register value: %lld\n", __func__, val, black_level_reg);

	return 0;
}

static int vb1943_set_again(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u8 reg_value = 0;
	int i;
	struct v4l2_ctrl *ctrl;
	int err = 0;

	if (val < VB1943_MIN_AGAIN_DEC)
		val = VB1943_MIN_AGAIN_DEC;

	if (val > VB1943_MAX_AGAIN_DEC)
		val = VB1943_MAX_AGAIN_DEC;

	for (i = ARRAY_SIZE(vb1943_again_lut) - 1; i >= 0; i--) {
		if (val >= vb1943_again_lut[i].gain_x100) {
			reg_value = vb1943_again_lut[i].reg_value;
			val = vb1943_again_lut[i].gain_x100;
			break;
		}
	}

	err = vb1943_write_reg(s_data, CTX0_ANALOG_GAIN, reg_value);
	if (err) {
		dev_err(dev, "%s: error setting Analog gain\n", __func__);
		return err;
	}

	/* Update new ctrl value */
	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_GAIN);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	return 0;
}

static int vb1943_set_dgain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 reg_value = 0;
	int err = 0;

	if (val < VB1943_MIN_DGAIN_DEC)
		val = VB1943_MIN_DGAIN_DEC;

	if (val > VB1943_MAX_DGAIN_DEC)
		val = VB1943_MAX_DGAIN_DEC;

	reg_value = DIV_ROUND_CLOSEST((val * 256), 100);

	err = vb1943_write_buffered_reg(s_data, CTX0_DIGITAL_GAIN_R, 2, reg_value);
	err |= vb1943_write_buffered_reg(s_data, CTX0_DIGITAL_GAIN_G, 2, reg_value);
	err |= vb1943_write_buffered_reg(s_data, CTX0_DIGITAL_GAIN_B, 2, reg_value);
	err |= vb1943_write_buffered_reg(s_data, CTX0_DIGITAL_GAIN_IR, 2, reg_value);
	if (err) {
		dev_err(dev, "%s: error setting Digital gain\n", __func__);
		return err;
	}

	return 0;
}

/*
static int vb1943_set_conversion_gain(struct tegracam_device *tc_dev, bool val)
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
	err = vb1943_write_reg(s_data, FDG_SEL0, val);
	err |= vb1943_write_reg(s_data, FDG_SEL1, val); // change conversion gain for dol2 also
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
		err = vb1943_read_buffered_reg(s_data, GAIN_LOW, 2, &curr_gain);
		if (err) {
			dev_err(dev, "%s: reading gain value error\n", __func__);
			return err;
		}
		min_high_gain = VB1943_HIGH_GAIN_REG_MIN * VB1943_MAX_GAIN_DB
			* mode->control_properties.gain_factor / VB1943_MAX_GAIN_DEC;

		if (curr_gain < VB1943_HIGH_GAIN_REG_MIN) {
			dev_warn(dev, "%s:Gain value too large for high conversion gain\n", __func__);
			curr_gain = VB1943_HIGH_GAIN_REG_MIN * VB1943_MAX_GAIN_DB
				* mode->control_properties.gain_factor
				/ VB1943_MAX_GAIN_DEC;
			err = vb1943_set_gain(tc_dev, curr_gain);
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
*/

static int vb1943_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u32 integration_time_line;

	dev_dbg(dev, "%s: integration time: %lld [us]\n", __func__, val);

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;

	integration_time_line = (val * VB1943_K_FACTOR) / priv->line_time;

	if (priv->broadcast_ctrl == BROADCAST)
		err = vb1943_broadcast_buffered_reg(s_data, CTX0_INTEGRATION_TIME_PRIMARY,
														2, integration_time_line);
	else
		err = vb1943_write_buffered_reg(s_data, CTX0_INTEGRATION_TIME_PRIMARY,
														2, integration_time_line);
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
		"%s: set integration time: %lld [us], integration time:%d [line]\n",
		__func__, val, integration_time_line);

	return err;
}

/*
static int vb1943_set_exposure_short(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u32 integration_time_line;
	u64 shr1;

	dev_dbg(dev, "%s: short integration time: %lld [us]\n", __func__, val);

	if (val > s_data->short_exposure_max_range)
		val = s_data->short_exposure_max_range;
	else if (val < s_data->short_exposure_min_range)
		val = s_data->short_exposure_min_range;

	integration_time_line = DIV_ROUND_CLOSEST(val
					* VB1943_K_FACTOR, priv->line_time);


	shr1 = VB1943_DEFAULT_RHS1 - integration_time_line;

	shr1 = shr1 - (shr1 % 2) + 1;

	if (shr1 < VB1943_MIN_SHR1_LENGTH)
		shr1 = VB1943_MIN_SHR1_LENGTH;
	else if (shr1 > VB1943_DEFAULT_RHS1 - VB1943_DOL2_MIN_INTEGRATION_LINES)
		shr1 = VB1943_DEFAULT_RHS1 - VB1943_DOL2_MIN_INTEGRATION_LINES;

	if (priv->broadcast_ctrl == BROADCAST)
		err = vb1943_broadcast_buffered_reg(s_data, SHR1_LOW, 3, shr1);
	else
		err = vb1943_write_buffered_reg(s_data, SHR1_LOW, 3, shr1);
	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	// Update new ctrl value
	ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_EXPOSURE_SHORT);
	if (ctrl) {
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
	}

	dev_dbg(dev,
	"%s: set short integration time: %lld [us], short integration time:%u [line], shr1: %llu [line]\n",
			__func__, val, integration_time_line, shr1);

	dev_dbg(dev,
	"%s: min short exp: %lld [us], max short exp:%lld [us]\n",
				__func__, s_data->short_exposure_min_range,
					s_data->short_exposure_max_range);

	return err;
}
*/

static int vb1943_update_exposure_ranges(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	u64 exposure_max_range, exposure_min_range;

	exposure_min_range = DIV_ROUND_UP((priv->line_time * VB1943_MIN_INTEGRATION_LINES_GS_MIN), VB1943_K_FACTOR);

	exposure_max_range = (priv->frame_length - VB1943_MIN_INTEGRATION_LINES_GS_MAX) *
							priv->line_time / VB1943_K_FACTOR;

	dev_dbg(dev, "exp min: %llu, exp max: %llu\n", exposure_min_range, exposure_max_range);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_EXPOSURE,
				exposure_min_range, exposure_max_range);

	return 0;
}

static u64 vb1943_calc_frame_length(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	u64 frame_length;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];

	frame_length = (((u64)mode->control_properties.framerate_factor *
				VB1943_G_FACTOR) / (val * priv->line_time));

	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	if (frame_length > VB1943_MAX_FRAME_LENGTH)
		frame_length = VB1943_MAX_FRAME_LENGTH;

	frame_length = (frame_length % 2) ? frame_length + 1 : frame_length;

	return frame_length;
}

/* CTX0_FRAME_LENGTH is a static register, sensor must not be streaming */
static int vb1943_write_frame_length(struct tegracam_device *tc_dev,
						u64 frame_length)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;

	priv->frame_length = frame_length;

	err = vb1943_update_exposure_ranges(tc_dev);
	if (err < 0) {
		dev_err(dev, "%s: failed to update exposure ranges\n", __func__);
		return err;
	}

	if (priv->broadcast_ctrl == BROADCAST)
		err = vb1943_broadcast_buffered_reg(s_data,
					CTX0_FRAME_LENGTH, 2, priv->frame_length);
	else
		err = vb1943_write_buffered_reg(s_data,
					CTX0_FRAME_LENGTH, 2, priv->frame_length);

	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: frame_length set: %llu\n", __func__, priv->frame_length);

	return 0;
}

static int vb1943_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	u64 frame_length;
	ktime_t start;
	int err, ret;

	frame_length = vb1943_calc_frame_length(tc_dev, val);

	dev_dbg(dev, "%s: val: %lld, frame_length: %llu\n",
					__func__, val, frame_length);

	/* Not streaming, written directly (also stream start overrides) */
	if (!tc_dev->is_streaming)
		return vb1943_write_frame_length(tc_dev, frame_length);

	/* Avoid SW standby round trip if frame length is not changed */
	if (frame_length == priv->frame_length)
		return 0;

	start = ktime_get();

	err = vb1943_enter_sw_standby(tc_dev);
	if (!err)
		err = vb1943_write_frame_length(tc_dev, frame_length);

	ret = vb1943_exit_sw_standby(tc_dev);

	dev_dbg(dev, "%s: frame_length changed on the fly in %lld us, err: %d\n",
		__func__, ktime_us_delta(ktime_get(), start), err ? err : ret);

	return err ? err : ret;
}

static int vb1943_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	u8 reg_val;
	int err;

	if (val) {
		err = vb1943_write_table(priv, mode_table[VB1943_EN_PATTERN_GEN]);

		if(val == 1)
			reg_val = 0x22;
		else if(val == 2)
			reg_val = 0x28;

		err |= vb1943_write_reg(s_data, (PATGEN_CTRL + 1), reg_val);
		if (err)
			goto fail;
	} else {
		err = vb1943_write_table(priv, mode_table[VB1943_DIS_PATTERN_GEN]);
		if (err)
			goto fail;
	}

	dev_dbg(dev, "%s++ Test mode pattern: %x\n", __func__, reg_val);

	return 0;
fail:
	dev_err(dev, "%s: error setting Test Pattern\n", __func__);
	return err;
}

static int vb1943_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct sensor_control_properties *ctrlprops = NULL;
	struct device *dev = tc_dev->dev;
	u64 max_framerate, min_framerate;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	max_framerate = (VB1943_G_FACTOR * VB1943_M_FACTOR) /
				(priv->min_frame_length * priv->line_time);

	/* Lowest frame rate whose frame length fits in CTX0_FRAME_LENGTH */
	min_framerate = DIV_ROUND_UP(VB1943_G_FACTOR * VB1943_M_FACTOR,
				VB1943_MAX_FRAME_LENGTH * priv->line_time);
	min_framerate = max_t(u64, min_framerate, ctrlprops->min_framerate);

	dev_dbg(dev, "%s: Max framerate is equal to %llu\n",
						__func__, max_framerate);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				min_framerate, max_framerate);

	dev_dbg(tc_dev->dev, "%s min_framerate: %llu, max_framerate: %llu\n",
			__func__, min_framerate, max_framerate);

	return 0;
}

static int vb1943_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u8 gpio_ctrl;
	int err;

	/* Control is set regardless of power state, applied in set_mode */
	if (s_data->power->state != SWITCH_ON)
		return 0;

	err = vb1943_read_reg(s_data, CTX0_GPIO_CTRL, &gpio_ctrl);
	if (err) {
		dev_err(dev, "%s: error reading GPIO control\n", __func__);
		return err;
	}

	if (val == SLAVE_MODE) {
		gpio_ctrl |= VB1943_GPIO1_EN;
		err = vb1943_write_reg(s_data, GPIO_1_CTRL,
						VB1943_GPIO_MODE_FSYNC_IN);
		err |= vb1943_write_reg(s_data, NB_FRAMES_TO_SEND, 0x00);
	} else {
		gpio_ctrl &= ~VB1943_GPIO1_EN;
	}

	err |= vb1943_write_reg(s_data, CTX0_GPIO_CTRL, gpio_ctrl);
	err |= vb1943_write_reg(s_data, VT_CTRL, (val == SLAVE_MODE) ?
			VB1943_VT_CTRL_FOLLOWER : VB1943_VT_CTRL_LEADER);
	if (err) {
		dev_err(dev, "%s: error setting operation mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: Sensor is in %s mode\n", __func__,
				(val == SLAVE_MODE) ? "Follower" : "Leader");

	return 0;
}

/*
static bool vb1943_find_broadcast_sensor(struct vb1943 *broadcast_private)
{
	struct vb1943 *current_private;

	list_for_each_entry(current_private, &vb1943_sensor_list, entry) {
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
*/

/*
static void vb1943_enable_second_slave_address(struct vb1943 *ack_private)
{
	struct vb1943 *current_private;
	int err;

	list_for_each_entry(current_private, &vb1943_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);

		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = vb1943_write_reg(current_private->s_data,
					SECOND_SLAVE_ADD, 1);
		if (err)
			dev_warn(&current_private->i2c_client->dev,
				"%s: Fail to write Second I2C register\n",
								__func__);

	mutex_unlock(&current_private->pw_mutex);

	dev_dbg(&current_private->i2c_client->dev,
		"%s: Sensors 2nd slave address configured\n", __func__);
	}

	err = vb1943_write_reg(ack_private->s_data, SECOND_SLAVE_ADD, 3);
	if (err)
		dev_warn(&ack_private->i2c_client->dev,
			"%s: Fail to write Second I2C register\n", __func__);

	dev_dbg(&ack_private->i2c_client->dev,
		": Sensors 2nd slave address configured with acknowlege\n");
}
*/

/*
static void vb1943_disable_second_slave_address(void)
{
	struct vb1943 *current_private;
	int err;

	list_for_each_entry(current_private, &vb1943_sensor_list, entry) {
		mutex_lock(&current_private->pw_mutex);

		if (current_private->s_data->power->state != SWITCH_ON) {
			mutex_unlock(&current_private->pw_mutex);
			continue;
		}

		err = vb1943_write_reg(current_private->s_data,
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
*/

/*
static void vb1943_configure_second_slave_address(void)
{
	struct vb1943 broadcast_private = {};

	if (vb1943_find_broadcast_sensor(&broadcast_private))
		vb1943_enable_second_slave_address(&broadcast_private);
	else
		vb1943_disable_second_slave_address();
}
*/

/*
static int vb1943_set_broadcast_ctrl(struct tegracam_device *tc_dev,
						struct v4l2_ctrl *ctrl)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
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
	vb1943_configure_second_slave_address();

	return 0;
}
*/

static int vb1943_set_custom_ctrls(struct v4l2_ctrl *ctrl)
{
	struct tegracam_ctrl_handler *handler = container_of(
		ctrl->handler, struct tegracam_ctrl_handler, ctrl_handler);
	struct tegracam_device *tc_dev = handler->tc_dev;
	int err = 0;
	s16 temperature;

	switch (ctrl->id) {
	case VB1943_CAMERA_CID_MEASURE_TEMPERATURE:
		err = vb1943_read_temperature(tc_dev, &temperature);
		*ctrl->p_new.p_s64 = temperature;
		*ctrl->p_cur.p_s64 = temperature;
		break;
	case VB1943_CAMERA_CID_SET_GS_RS_MODE:
		err = vb1943_set_gs_rs_mode(tc_dev, *ctrl->p_new.p_u32);
		break;
	case VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE:
		err = vb1943_set_rgbnir_rgb_ir_mode(tc_dev, *ctrl->p_new.p_u32);
		break;
	default:
		pr_err("%s: unknown ctrl id.\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int vb1943_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct vb1943 *priv = (struct vb1943 *)s_data->priv;

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
		goto vb1943_mclk_fail;
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

	usleep_range(100000, 110000);

	err = vb1943_write_reg(s_data, SYSTEM_UP, 0x01);
	if (err)
		dev_err(dev, "%s: error setting SYSTEM_UP register, value START_SENSOR\n", __func__);

	usleep_range(10000, 11000);

	err = vb1943_write_reg(s_data, BOOT, 0x10);
	if (err)
		dev_err(dev, "%s: error setting BOOT register, value END_BOOT\n", __func__);

	pw->state = SWITCH_ON;

	/* Additional sleep required in the case of hardware power-on sequence */
	usleep_range(40000, 41000);

	mutex_unlock(&priv->pw_mutex);

//	vb1943_configure_second_slave_address();

	return 0;

vb1943_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int vb1943_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct vb1943 *priv = (struct vb1943 *)s_data->priv;
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

static int vb1943_power_get(struct tegracam_device *tc_dev)
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

static int vb1943_power_put(struct tegracam_device *tc_dev)
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

static int vb1943_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;
	u64 frame_length = VB1943_DEFAULT_FRAME_LENGTH;

	err = vb1943_write_buffered_reg(s_data, CTX0_FRAME_LENGTH, 2, frame_length);
	if (err) {
		dev_err(dev, "%s: failed to write CTX0_FRAME_LENGTH\n", __func__);
		return err;
	}

	err = vb1943_read_buffered_reg(s_data, CTX0_FRAME_LENGTH, 2, &frame_length);
	if (err) {
		dev_err(dev, "%s: failed to read CTX0_FRAME_LENGTH\n", __func__);
		return err;
	}

	priv->frame_length = frame_length;

	dev_dbg(dev, "%s: frame_length: %llu \n", __func__, frame_length);

	return err;
}

static struct camera_common_pdata *vb1943_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(vb1943_of_match, dev);
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

static int vb1943_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SGBRG10_1X10:
		err = vb1943_write_table(priv, mode_table[VB1943_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SGBRG12_1X12:
		err = vb1943_write_table(priv, mode_table[VB1943_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int vb1943_set_csi_lane_mode(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (s_data->numlanes == VB1943_TWO_LANE_MODE) {
		err = vb1943_write_reg(s_data, LANE_NB_SEL, 0x01);
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

static int vb1943_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 line_length;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = vb1943_read_buffered_reg(s_data, LINE_LENGTH, 2, &line_length);
	if (err) {
		dev_err(dev, "%s: unable to read LINE_LENGTH register\n", __func__);
		return err;
	}

	priv->line_time = (line_length * VB1943_G_FACTOR) / (4 * VB1943_PIXEL_CLK);

	dev_dbg(dev, "%s: line_length: %llu [1/4 of pixel clock period], PIXEL_CLK: %llu [Hz], line_time: %u [ns]\n",
		__func__, line_length, VB1943_PIXEL_CLK, priv->line_time);

	return 0;
}

static int vb1943_adjust_line_length_register(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u64 line_length;

	dev_dbg(dev, "%s:++\n", __func__);

	line_length = VB1943_DEFAULT_LINE_LENGTH;

	if (s_data->numlanes == VB1943_TWO_LANE_MODE) {
		if (s_data->colorfmt->code == MEDIA_BUS_FMT_SGBRG12_1X12)
			line_length = VB1943_TWO_LANE_12BIT_LINE_LENGTH;
		else
			line_length = VB1943_TWO_LANE_10BIT_LINE_LENGTH;
	}

	err = vb1943_write_buffered_reg(s_data, LINE_LENGTH, 2, line_length);
	if (err) {
		dev_err(dev, "%s: unable to write LINE_LENGTH register\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: LINE_LENGTH: %llu\n", __func__, line_length);

	return 0;
}

/*
static int vb1943_set_sync_mode(struct tegracam_device *tc_dev, u32 val)
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
		err = vb1943_write_reg(s_data, EXTMODE, extmode);
		if (err)
			dev_err(dev, "%s: error setting sync mode\n", __func__);
	}

	return err;
}
*/

/*
static int vb1943_configure_triggering_pins(struct tegracam_device *tc_dev)
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

	err = vb1943_write_reg(s_data, XVS_XHS_DRV, xvs_xhs_drv);
	if (err) {
		dev_err(dev, "%s: error setting Slave mode\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: XVS_XHS driver register: %x\n", __func__, xvs_xhs_drv);

	return 0;
}
*/

static int vb1943_check_unsupported_mode(struct camera_common_data *s_data,
					struct v4l2_mbus_framefmt *mf)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	s64 current_gs_rs_mode, current_rgbnir_rgb_nir_mode;
	int err=0;

	dev_dbg(dev, "%s++\n", __func__);

	current_gs_rs_mode = fr_get_v4l2_ctrl_value(tc_dev, VB1943_CAMERA_CID_SET_GS_RS_MODE, CURRENT);
	current_rgbnir_rgb_nir_mode = fr_get_v4l2_ctrl_value(tc_dev, VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE, CURRENT);

	if(priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG10_1X10 && current_gs_rs_mode == VB1943_RS_MODE &&
								current_rgbnir_rgb_nir_mode == VB1943_NIR_MODE) {
		dev_warn(dev,
		"%s: selected mode: 10BPP, Rolling Shutter & NIR is not supported! Switching to mode: 10BPP, Rolling Shutter & RGB+NIR\n",
		 __func__);
		err = vb1943_update_ctrl(tc_dev, VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE, VB1943_RGBNIR_MODE,
							VB1943_RGBNIR_MODE, VB1943_RGBNIR_MODE, VB1943_RGBNIR_MODE);
	}

	else if(priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG12_1X12) {
		if(current_gs_rs_mode == VB1943_GS_MODE) {
			dev_warn(dev,
			"%s: selected mode: 12BPP & Global Shutter is not supported! Switching to mode: 12BPP & Rolling Shutter\n",
			__func__);
			err = vb1943_update_ctrl(tc_dev, VB1943_CAMERA_CID_SET_GS_RS_MODE, VB1943_RS_MODE,
									VB1943_RS_MODE, VB1943_RS_MODE, VB1943_RS_MODE);
			/* Control is forced, vb1943_set_gs_rs_mode() is not called */
			vb1943_set_min_frame_length(priv, VB1943_RS_MODE);
			current_gs_rs_mode = VB1943_RS_MODE;
		}
		if(current_gs_rs_mode == VB1943_RS_MODE && current_rgbnir_rgb_nir_mode == VB1943_NIR_MODE) {
			dev_warn(dev,
			"%s: selected mode: 12BPP, Rolling Shutter & NIR is not supported! Switching to mode: 12BPP, Rolling Shutter & RGB+NIR\n",
			__func__);
			err = vb1943_update_ctrl(tc_dev, VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE, VB1943_RGBNIR_MODE,
							VB1943_RGBNIR_MODE, VB1943_RGBNIR_MODE, VB1943_RGBNIR_MODE);
		}
	}

	if (err)
		return err;

	return 0;
}

static int vb1943_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct vb1943 *priv = (struct vb1943 *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	// Update Black level V4L control
	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SGBRG10_1X10:
			err = vb1943_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						(*ctrl->p_cur.p_s64 >> 2),
						VB1943_DEFAULT_BLACK_LEVEL_10BPP,
						0, VB1943_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG10_1X10;
			break;
		case MEDIA_BUS_FMT_SGBRG12_1X12:
			err = vb1943_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						(*ctrl->p_cur.p_s64 << 2),
						VB1943_DEFAULT_BLACK_LEVEL_12BPP,
						0, VB1943_MAX_BLACK_LEVEL_12BPP);
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

static int vb1943_ctrls_init(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = priv->s_data;
	struct v4l2_ctrl_config *ctrl_cfg;
	struct v4l2_ctrl *ctrl;
	struct tegracam_ctrl_handler *handler = s_data->tegracam_ctrl_hdl;
	int numctrls;
	int err, i;

	numctrls = ARRAY_SIZE(vb1943_custom_ctrl_list);

	for (i = 0; i < numctrls; i++) {
		ctrl_cfg = &vb1943_custom_ctrl_list[i];

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

/*
 * Shutter mode is passed in, while its control is being set (on the fly switch)
 * the control still holds the previous value
 */
static int vb1943_set_sensor_configuration(struct tegracam_device *tc_dev,
						s64 current_gs_rs_mode)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	s64 current_rgbnir_rgb_nir_mode;
	u8 conf_reg;
	int err;

	current_rgbnir_rgb_nir_mode = fr_get_v4l2_ctrl_value(tc_dev, VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE, CURRENT);
	
	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SGBRG10_1X10:
		if(current_gs_rs_mode == VB1943_GS_MODE) {	// Global Shutter
			if(current_rgbnir_rgb_nir_mode == VB1943_RGBNIR_MODE)
				conf_reg = VB1943_GS_RGB_NIR_RAW10;
			else if(current_rgbnir_rgb_nir_mode == VB1943_RGB_MODE)
				conf_reg = VB1943_GS_RGB_RAW10;
			else
				conf_reg = VB1943_GS_NIR_RAW10;
		}
		else {						// Rolling Shutter
			if(current_rgbnir_rgb_nir_mode == VB1943_RGBNIR_MODE)
				conf_reg = VB1943_RS_RGB_NIR_RAW10;
			else if(current_rgbnir_rgb_nir_mode == VB1943_RGB_MODE)
				conf_reg = VB1943_RS_RGB_RAW10;
		}
		break;
	case MEDIA_BUS_FMT_SGBRG12_1X12:
		if(current_gs_rs_mode == VB1943_RS_MODE) {	// Rolling Shutter
			if(current_rgbnir_rgb_nir_mode == VB1943_RGBNIR_MODE)
				conf_reg = VB1943_RS_RGB_NIR_RAW12;
			else if(current_rgbnir_rgb_nir_mode == VB1943_RGB_MODE)
				conf_reg = VB1943_RS_RGB_RAW12;
		}
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}
	
	err = vb1943_write_reg(s_data, CTX0_SENSOR_CONFIGURATION, conf_reg);
	if (err) {
		dev_err(dev, "%s: unable to write to CTX0_SENSOR_CONFIGURATION register\n", __func__);
		return err;
	}

	return 0;
}
static int vb1943_set_mode(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	struct v4l2_mbus_framefmt mf = {};
	int err;

	dev_dbg(dev, "%s: setting mode %u\n", __func__, s_data->mode);

	err = camera_common_g_fmt(&s_data->subdev, &mf);
	err = vb1943_check_unsupported_mode(s_data, &mf);

	err = vb1943_write_table(priv, mode_table[VB1943_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
		return err;
	}

	err = vb1943_set_csi_lane_mode(tc_dev);
	if (err) {
		dev_err(dev, "%s: error setting CSI lane mode\n", __func__);
		return err;
	}

	err = vb1943_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
								__func__);
		return err;
	}

	err = vb1943_set_sensor_configuration(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				VB1943_CAMERA_CID_SET_GS_RS_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set sensor configuration\n",
								__func__);
		return err;
	}

	err = vb1943_write_table(priv, mode_table[s_data->mode]);

	if (err) {
		dev_err(dev, "%s: unable to write table for mode %u\n", __func__,
								s_data->mode);
		return err;
	}

	err = vb1943_set_operation_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_OPERATION_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set operation mode\n", __func__);
		return err;
	}
/*
	err = vb1943_set_sync_mode(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_SYNC_MODE, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set sync mode\n", __func__);
		return err;
	}

	err = vb1943_configure_triggering_pins(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable configure XVS/XHS pins\n", __func__);
		return err;
	}
*/

	err = vb1943_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

	err = vb1943_adjust_line_length_register(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust line_length\n", __func__);
		return err;
	}

	/* Override V4L GAIN, EXPOSURE and FRAME RATE controls */
	s_data->override_enable = true;

	err = vb1943_calculate_line_time(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		return err;
	}

	err = vb1943_update_framerate_range(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to update frame range\n", __func__);
		return err;
	}
	dev_dbg(dev, "%s: set mode %u succesfully\n", __func__, s_data->mode);

	return 0;
}

static int vb1943_start_streaming(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	struct camera_common_data *s_data = tc_dev->s_data;
	int err;

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

	err = vb1943_write_table(priv, mode_table[VB1943_MODE_START_STREAM]);
	if (err)
		goto exit;


	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);

	return err;
}

static int vb1943_stop_streaming(struct tegracam_device *tc_dev)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {
		/* disable serdes streaming */
		max96792_stop_streaming(priv->s_data->dser_dev, dev);
	}

	err = vb1943_write_table(priv, mode_table[VB1943_MODE_STOP_STREAM]);
	if (err)
		return err;
	usleep_range(priv->frame_length * priv->line_time / VB1943_K_FACTOR,
		priv->frame_length * priv->line_time / VB1943_K_FACTOR + 1000);

	return 0;
}

/* Poll SYSTEM_FSM_STATE until it is (or is no longer) equal to state */
static int vb1943_wait_fsm_state(struct tegracam_device *tc_dev, u8 state,
						bool equal, u8 *fsm_state)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	u64 frame_time_us = priv->frame_length * priv->line_time / VB1943_K_FACTOR;
	ktime_t timeout = ktime_add_us(ktime_get(), 2 * frame_time_us + 50000);
	int err;

	for (;;) {
		err = vb1943_read_reg(s_data, SYSTEM_FSM_STATE, fsm_state);
		if (err)
			return err;

		if ((*fsm_state == state) == equal)
			return 0;

		if (ktime_after(ktime_get(), timeout))
			return -ETIMEDOUT;

		usleep_range(500, 1000);
	}
}

/* Stop streaming, static registers are writable only in SW standby */
static int vb1943_enter_sw_standby(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u8 fsm_state;
	int err;

	err = vb1943_write_reg(s_data, STREAMING, 0x01);
	if (err) {
		dev_err(dev, "%s: unable to enter SW standby\n", __func__);
		return err;
	}

	err = vb1943_wait_fsm_state(tc_dev, VB1943_FSM_SW_STBY, true, &fsm_state);
	if (err)
		dev_err(dev, "%s: SW standby not reached (%d), FSM: 0x%02x\n",
						__func__, err, fsm_state);

	return err;
}

/* Start streaming from SW standby */
static int vb1943_exit_sw_standby(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u8 fsm_state;
	int err;

	err = vb1943_write_reg(s_data, SW_STBY, 0x01);
	if (err) {
		dev_err(dev, "%s: unable to start streaming\n", __func__);
		return err;
	}

	err = vb1943_wait_fsm_state(tc_dev, VB1943_FSM_STREAMING, true, &fsm_state);
	if (err)
		dev_warn(dev, "%s: streaming not confirmed (%d), FSM: 0x%02x\n",
						__func__, err, fsm_state);

	return 0;
}

/* SW standby, rewrite only shutter dependent registers, back to streaming */
static int vb1943_switch_shutter(struct tegracam_device *tc_dev, u32 gs_rs)
{
	struct vb1943 *priv = (struct vb1943 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	s64 rgbnir_mode;
	ktime_t start;
	int err, ret;

	rgbnir_mode = fr_get_v4l2_ctrl_value(tc_dev,
				VB1943_CAMERA_CID_SET_RGBNIR_RGB_IR_MODE, CURRENT);

	if (gs_rs == VB1943_GS_MODE &&
		priv->current_pixel_format == MEDIA_BUS_FMT_SGBRG12_1X12) {
		dev_warn(dev, "%s: Global Shutter is not supported in 12BPP\n", __func__);
		return -EINVAL;
	}

	if (gs_rs == VB1943_RS_MODE && rgbnir_mode == VB1943_NIR_MODE) {
		dev_warn(dev, "%s: Rolling Shutter is not supported in NIR mode\n", __func__);
		return -EINVAL;
	}

	start = ktime_get();

	err = vb1943_enter_sw_standby(tc_dev);
	if (err)
		goto restart;

	err = vb1943_set_sensor_configuration(tc_dev, gs_rs);
	if (err) {
		dev_err(dev, "%s: unable to set sensor configuration\n", __func__);
		goto restart;
	}

	/* Frame length minimum differs between GS and RS, already in SW standby */
	err = vb1943_write_frame_length(tc_dev, vb1943_calc_frame_length(tc_dev,
			fr_get_v4l2_ctrl_value(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
								CURRENT)));
	if (err) {
		dev_err(dev, "%s: unable to set frame rate\n", __func__);
		goto restart;
	}

	err = vb1943_update_framerate_range(tc_dev);
	if (err)
		dev_err(dev, "%s: unable to update frame rate range\n", __func__);

restart:
	ret = vb1943_exit_sw_standby(tc_dev);
	if (ret)
		return ret;

	dev_info(dev, "%s: switched to %s in %lld us, err: %d\n", __func__,
		gs_rs_mode_menu[gs_rs], ktime_us_delta(ktime_get(), start), err);

	return err;
}

static struct camera_common_sensor_ops vb1943_common_ops = {
	.numfrmfmts = ARRAY_SIZE(vb1943_frmfmt),
	.frmfmt_table = vb1943_frmfmt,
	.power_on = vb1943_power_on,
	.power_off = vb1943_power_off,
	.write_reg = vb1943_write_reg,
	.read_reg = vb1943_read_reg,
	.parse_dt = vb1943_parse_dt,
	.power_get = vb1943_power_get,
	.power_put = vb1943_power_put,
	.set_mode = vb1943_set_mode,
	.start_streaming = vb1943_start_streaming,
	.stop_streaming = vb1943_stop_streaming,
	.check_unsupported_mode = vb1943_check_unsupported_mode,
	.after_set_pixel_format = vb1943_after_set_pixel_format,
	.init_private_controls = vb1943_ctrls_init,
};

static int vb1943_gmsl_serdes_setup(struct vb1943 *priv)
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

static void vb1943_gmsl_serdes_reset(struct vb1943 *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int vb1943_board_setup(struct vb1943 *priv)
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

		err = vb1943_gmsl_serdes_setup(priv);
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

	err = vb1943_power_on(s_data);
	if (err) {
		dev_err(dev,
		"Error %d during power on sensor\n", err);
		return err;
	}

	err = vb1943_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
								__func__);
		goto error2;
	}

	err = vb1943_adjust_line_length_register(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to adjust line_length\n", __func__);
		goto error2;
	}

	err = vb1943_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

	priv->min_frame_length = priv->frame_length;

error2:
	vb1943_power_off(s_data);
	camera_common_mclk_disable(s_data);

	return err;
}

static int vb1943_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops vb1943_subdev_internal_ops = {
	.open = vb1943_open,
};

static struct tegracam_ctrl_ops vb1943_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = vb1943_set_again,
	.set_digital_gain = vb1943_set_dgain,
//	.set_conversion_gain = vb1943_set_conversion_gain,
	.set_exposure = vb1943_set_exposure,
	.set_frame_rate = vb1943_set_frame_rate,
	.set_group_hold = vb1943_set_group_hold,
	.set_test_pattern = vb1943_set_test_pattern,
	.set_operation_mode = vb1943_set_operation_mode,
//	.set_sync_mode = vb1943_set_sync_mode,
//	.set_broadcast_ctrl = vb1943_set_broadcast_ctrl,
	.set_black_level = vb1943_set_black_level,
//	.set_exposure_short = vb1943_set_exposure_short,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int vb1943_probe(struct i2c_client *client)
#else
static int vb1943_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct vb1943 *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct vb1943), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "vb1943", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &vb1943_common_ops;
	tc_dev->v4l2sd_internal_ops = &vb1943_subdev_internal_ops;
	tc_dev->tcctrl_ops = &vb1943_ctrl_ops;

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
	priv->current_pixel_format = MEDIA_BUS_FMT_SGBRG10_1X10;

	ctrlprops =
		&priv->s_data->sensor_props.sensor_modes[0].control_properties;

	priv->s_data->exposure_min_range = ctrlprops->min_exp_time.val;
	priv->s_data->exposure_max_range = ctrlprops->max_exp_time.val;

	priv->s_data->dig_gain_def_value = VB1943_DEF_DGAIN_DEC;
	priv->s_data->dig_gain_min_range = VB1943_MIN_DGAIN_DEC;
	priv->s_data->dig_gain_max_range = VB1943_MAX_DGAIN_DEC;

	INIT_LIST_HEAD(&priv->entry);

	err = vb1943_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = vb1943_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					VB1943_DEFAULT_BLACK_LEVEL_10BPP,
					VB1943_DEFAULT_BLACK_LEVEL_10BPP, 0,
					VB1943_MAX_BLACK_LEVEL_10BPP);
	if (err)
		return err;

	err = vb1943_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				(ARRAY_SIZE(vb1943_test_pattern_menu)-1));
	if (err)
		return err;

	err = vb1943_update_ctrl(tc_dev, TEGRA_CAMERA_CID_DIGITAL_GAIN,
							VB1943_DEF_DGAIN_DEC,
							VB1943_DEF_DGAIN_DEC,
							VB1943_MIN_DGAIN_DEC,
							VB1943_MAX_DGAIN_DEC);
	if (err)
		return err;

	list_add_tail(&priv->entry, &vb1943_sensor_list);

	dev_info(dev, "Detected vb1943 sensor\n");

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int vb1943_remove(struct i2c_client *client)
#else
static void vb1943_remove(struct i2c_client *client)
#endif
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct vb1943 *priv;

	if (!s_data) {
		dev_err(&client->dev, "camera common data is NULL\n");
#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
		return -EINVAL;
#else
		return;
#endif
	}

	priv = (struct vb1943 *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		vb1943_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);


#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id vb1943_id[] = {
	{ "vb1943", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, vb1943_id);

static struct i2c_driver vb1943_i2c_driver = {
	.driver = {
		.name = "vb1943",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(vb1943_of_match),
	},
	.probe = vb1943_probe,
	.remove = vb1943_remove,
	.id_table = vb1943_id,
};

module_i2c_driver(vb1943_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony VB1943");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
