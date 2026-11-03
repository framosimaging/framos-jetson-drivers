// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2026 Framos. All rights reserved.
 *
 * fr_imx06a.c - Framos fr_imx06a.c driver
 */

/* #define DEBUG 1 */

#include <nvidia/conftest.h>
#include <linux/bitops.h>
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
#include "fr_imx06a_mode_tbls.h"
#include "media/fr_sensor_common.h"

#define IMX06A_K_FACTOR 1000LL
#define IMX06A_M_FACTOR 1000000LL
#define IMX06A_G_FACTOR 1000000000LL
#define IMX06A_T_FACTOR 1000000000000LL

#define IMX06A_ANALOG_GAIN_OFFSET					16384
#define IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL			15360
#define IMX06A_ANALOG_GAIN_MAX_BINNING				16128
#define IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL_TIMES		16000
#define IMX06A_ANALOG_GAIN_MAX_BINNING_TIMES		64000
#define IMX06A_DIGITAL_GAIN_K_FACTOR				1000
#define IMX06A_DIGITAL_GAIN_MIN 					(1 * IMX06A_DIGITAL_GAIN_K_FACTOR)
#define IMX06A_DIGITAL_GAIN_MAX 					(16 * IMX06A_DIGITAL_GAIN_K_FACTOR - 4)
#define IMX06A_DIGITAL_GAIN_STEP					39

#define IMX06A_MAX_BLACK_LEVEL_10BPP				1023
#define IMX06A_MAX_BLACK_LEVEL_12BPP				4095
#define IMX06A_DEFAULT_BLACK_LEVEL_10BPP			64
#define IMX06A_DEFAULT_BLACK_LEVEL_12BPP			256

#define IMX06A_COARSE_INTEG_TIME_MAX_MARGIN			64
#define IMX06A_COARSE_INTEG_TIME_MIN_FULL_PIXEL		6
#define IMX06A_COARSE_INTEG_TIME_MIN_4_2_BINNING	4
#define IMX06A_MAX_FRM_LENGTH_LINES					65535
#define IMX06A_MAX_COARSE_INTEG_TIME				65535

#define IMX06A_4_CSI_LANES 							4

#define IMX06A_INCK 								74250000LL

#define IMX06A_IVTPX_CH 							32
#define IMX06A_IVTPXCK_FREQ							109400000LL
#define IMX06A_IVTPXCK_FREQ_MHZ						109

#define IMX06A_QSC_DATA_ADDR						0x5800
#define IMX06A_QSC_DATA_SIZE_BYTES					2304

#define IMX06A_FINE_INTEG_TIME_EX_FULL_PIXEL_10				0x01DBE0
#define IMX06A_FINE_INTEG_TIME_EX_4_2_ADJ_PIXEL_BINNING_10	0x01DA40
#define IMX06A_FINE_INTEG_TIME_EX_FULL_PIXEL_12				0x01B800


#define IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_PIX_10BIT	220
#define IMX06A_MIN_FRAME_LENGTH_DELTA_4_2_BINN_10BIT	132
#define IMX06A_MIN_FRAME_LENGTH_DELTA_CROP_30FPS_10BIT	220
#define IMX06A_MIN_FRAME_LENGTH_DELTA_4K_10BIT			132
#define IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_HD_10BIT		1128
#define IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_PIX_12BIT	226
#define IMX06A_MIN_FRAME_LENGTH_DELTA_4_2_BINN_12BIT	132
#define IMX06A_MIN_FRAME_LENGTH_DELTA_CROP_30FPS_12BIT	226
#define IMX06A_MIN_FRAME_LENGTH_DELTA_4K_12BIT			132
#define IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_HD_12BIT		1128

#define IMX06A_CAMERA_CID_BASE						(V4L2_CTRL_CLASS_CAMERA | 0x6000)
#define IMX06A_CAMERA_CID_QBC_REMOSAIC				(IMX06A_CAMERA_CID_BASE + 0)
#define IMX06A_CAMERA_CID_QSC_ENABLE				(IMX06A_CAMERA_CID_BASE + 1)

LIST_HEAD(imx06a_sensor_list);

static struct mutex serdes_lock__;

static const struct of_device_id imx06a_of_match[] = {
	{ .compatible = "framos,imx06a",},
	{ },
};
MODULE_DEVICE_TABLE(of, imx06a_of_match);

static int imx06a_ctrls_init(struct tegracam_device *tc_dev);
static int imx06a_set_custom_ctrls(struct v4l2_ctrl *ctrl);
static const struct v4l2_ctrl_ops imx06a_custom_ctrl_ops = {
    .s_ctrl = imx06a_set_custom_ctrls,
};
static int imx06a_update_qsc_control(struct tegracam_device *tc_dev, u32 val);

static const char * const imx06a_test_pattern_menu[] = {
	[0] = "No pattern",
	[1] = "Solid Color",
	[2] = "100% Color Bars",
	[3] = "Fade to gray Color Bars",
	[4] = "PN9"
};

static struct v4l2_ctrl_config imx06a_custom_ctrl_list[] = {
	{
		.ops = &imx06a_custom_ctrl_ops,
		.id = IMX06A_CAMERA_CID_QBC_REMOSAIC,
		.name = "QBC Re-mosaic",
		.type = V4L2_CTRL_TYPE_BOOLEAN,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 1,
		.def = 1,
		.step = 1,
	},
	{
		.ops = &imx06a_custom_ctrl_ops,
		.id = IMX06A_CAMERA_CID_QSC_ENABLE,
		.name = "QSC Enable",
		.type = V4L2_CTRL_TYPE_BOOLEAN,
		.flags = V4L2_CTRL_FLAG_EXECUTE_ON_WRITE,
		.min = 0,
		.max = 1,
		.def = 1,
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
	TEGRA_CAMERA_CID_TEST_PATTERN,
};

struct imx06a {
	struct i2c_client			*i2c_client;
	struct i2c_client			*eeprom;
	struct v4l2_subdev			*subdev;
	u64							frame_length;
	u64							min_frame_length;
	u64							current_pixel_format;
	s32							integration_offset;
	u32							line_time;
	u8							*qsc_data;
	u8  						qsc_control_byte;
	i2c_broadcast_ctrl			broadcast_ctrl;
	struct mutex				pw_mutex;
	struct list_head			entry;
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

static inline int imx06a_read_reg(struct camera_common_data *s_data,
							u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	*val = reg_val & 0xFF;

	return err;
}

static int imx06a_write_reg(struct camera_common_data *s_data,
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

static int imx06a_read_buffered_reg_big_endian(struct camera_common_data *s_data,
			u16 start_addr, u8 number_of_registers, s64 *val)
{
	struct device *dev = s_data->dev;
	int err, i;
	u8 reg;
	*val = 0;

	if (!s_data->group_hold_active) {
		err = imx06a_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: error setting register hold\n",
								__func__);
			return err;
		}
	}

	for (i = number_of_registers - 1; i >= 0; i--) {
		err = imx06a_read_reg(s_data, start_addr + i, &reg);
		*val += reg << ((number_of_registers - i - 1) * 8);
		if (err) {
			dev_err(dev, "%s: error reading buffered registers\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx06a_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: error unsetting register hold\n",
								__func__);
			return err;
		}
	}

	return err;
}

static int imx06a_write_buffered_reg_big_endian(struct camera_common_data *s_data,
			u16 start_addr, u8 number_of_registers, u64 val)
{
	int err, i;
	struct device *dev = s_data->dev;

	if (!s_data->group_hold_active) {
		err = imx06a_write_reg(s_data, REGHOLD, 0x01);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
			return err;
		}
	}

	for (i = 0; i < number_of_registers; i++) {
		err = imx06a_write_reg(s_data, start_addr - i,
						(u8)(val >> (i * 8)));
		if (err) {
			dev_err(dev, "%s: BUFFERED register write error\n",
								__func__);
			return err;
		}
	}

	if (!s_data->group_hold_active) {
		err = imx06a_write_reg(s_data, REGHOLD, 0x00);
		if (err) {
			dev_err(dev, "%s: GRP_PARAM_HOLD erroror\n", __func__);
			return err;
		}
	}

	return err;
}

static int imx06a_write_table(struct imx06a *priv, const imx06a_reg table[])
{
	struct camera_common_data *s_data = priv->s_data;

	return regmap_util_write_table_8(s_data->regmap,
					 table,
					 NULL, 0,
					 IMX06A_TABLE_WAIT_MS,
					 IMX06A_TABLE_END);
}

static int imx06a_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	s_data->group_hold_active = val;

	err = imx06a_write_reg(s_data, REGHOLD, val);
	if (err) {
		dev_err(dev, "%s: GRP_PARAM_HOLD error\n", __func__);
		return err;
	}

	return err;
}

static int imx06a_update_ctrl(struct tegracam_device *tc_dev, int ctrl_id,
				u64 current_val, u64 default_val, u64 min_val,
								u64 max_val)
{
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
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
			ctrl->qmenu = imx06a_test_pattern_menu;
			ctrl->maximum = max_val;
			break;
		}
	}
	return 0;
}

static int imx06a_set_qbc_remosaic(struct tegracam_device *tc_dev, u32 val)
{
    struct camera_common_data *s_data = tc_dev->s_data;
    struct device *dev = tc_dev->dev;
    struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
    int err;

    if (priv->s_data->power->state == SWITCH_OFF)
        return 0;

    err = imx06a_write_reg(s_data, IMX06A_QBC_RMSC_EN, val);
    if (err)
        goto fail;


    dev_dbg(dev, "%s++ QBC Re-mosaic enable: %u\n", __func__, val);

    return 0;
fail:
    dev_err(dev, "%s: error setting QBC Re-mosaic\n", __func__);
    return err;
}

static int imx06a_set_black_level(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	s64 black_level_reg;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
		black_level_reg = val << 2;
	else
		black_level_reg = val;

	err = imx06a_write_buffered_reg_big_endian(s_data, MANUAL_DATA_PEDESTAL_VALUE_LOW,
					2, black_level_reg);
	if (err)
		goto fail;

	dev_dbg(dev, "%s: black level: %lld\n", __func__, val);

	return 0;

fail:
	dev_dbg(dev, "%s: BLACK LEVEL control error\n", __func__);
	return err;
}

static int imx06a_get_digital_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct device *dev = tc_dev->dev;
	u32 integer_part, decimal_part, digital_gain;


	if (val > IMX06A_DIGITAL_GAIN_MAX)
		val = IMX06A_DIGITAL_GAIN_MAX;

	integer_part = val / IMX06A_DIGITAL_GAIN_K_FACTOR;
	decimal_part = ((val % IMX06A_DIGITAL_GAIN_K_FACTOR) * 10)
			/ IMX06A_DIGITAL_GAIN_STEP;

	decimal_part = (decimal_part > 0xFF) ? 0xFF : decimal_part;

	digital_gain = (integer_part << 8) + decimal_part;

	dev_dbg(dev, "%s: Digital gain val: %lld, Digital gain reg: %xh\n",
						__func__, val, digital_gain);

	return digital_gain;
}

static int imx06a_read_qsc_data_sensor(struct i2c_client *i2c_client, struct tegracam_device *tc_dev)
{
    struct i2c_msg msgs[2];
    struct device *dev = tc_dev->dev;
    u8 addr[2];
	u8* sensor_data;
    int ret;

	sensor_data = kmalloc(IMX06A_QSC_DATA_SIZE_BYTES, GFP_KERNEL);

    addr[0] = (0x5800 >> 8) & 0xff;
    addr[1] = 0x5800 & 0xff;

    msgs[0].addr  = i2c_client->addr;
    msgs[0].flags = 0;
    msgs[0].len   = 2;
    msgs[0].buf   = addr;

    msgs[1].addr  = i2c_client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = IMX06A_QSC_DATA_SIZE_BYTES;
    msgs[1].buf   = sensor_data;

    ret = i2c_transfer(i2c_client->adapter, msgs, 2);

    if (ret < 0) {
        kfree(sensor_data);
        return ret;
    }

    if (ret != 2) {
        kfree(sensor_data);
        return -EIO;
    }

	if (!sensor_data) {
		dev_err(dev, "Allocating QSC data memory for sensor registers failed\n");
		kfree(sensor_data);
		return -ENOMEM;
	}

	kfree(sensor_data);

    return 0;
}

static int imx06a_set_gain(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx06a *priv = (struct imx06a *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	const struct sensor_mode_properties *mode =
		&s_data->sensor_props.sensor_modes[s_data->mode];
	int err = 0, i;
	u32 analog_gain_glob;
	u32 digital_gain = 1 << 8;
	s64 digital_part;

	analog_gain_glob = IMX06A_ANALOG_GAIN_OFFSET - (IMX06A_ANALOG_GAIN_OFFSET
			* mode->control_properties.gain_factor) / val;


	if (analog_gain_glob % 4) {
		analog_gain_glob = (analog_gain_glob + 3u) & ~3u;
	}

	if (s_data->mode == IMX06A_MODE_4x2_BINNING_4096x3072) {
		analog_gain_glob = (analog_gain_glob > IMX06A_ANALOG_GAIN_MAX_BINNING) ?
			IMX06A_ANALOG_GAIN_MAX_BINNING : analog_gain_glob;

		if (val > IMX06A_ANALOG_GAIN_MAX_BINNING_TIMES) {
			digital_part = (val * IMX06A_DIGITAL_GAIN_K_FACTOR) / IMX06A_ANALOG_GAIN_MAX_BINNING_TIMES;
			digital_gain = imx06a_get_digital_gain(tc_dev, digital_part);
		}
	} else if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
		analog_gain_glob = (analog_gain_glob > IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL) ?
			IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL : analog_gain_glob;

		if (val > IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL_TIMES) {
			digital_part = (val * IMX06A_DIGITAL_GAIN_K_FACTOR) / IMX06A_ANALOG_GAIN_MAX_FULL_PIXEL_TIMES;
			digital_gain = imx06a_get_digital_gain(tc_dev, digital_part);
		}
	}

	if (!s_data->group_hold_active)
		err = imx06a_write_reg(s_data, REGHOLD, 0x01);

	for (i = 0; i < 2; i++)
		err |= imx06a_write_reg(s_data, ANA_GAIN_GLOBAL_LOW - i,
					(u8)(analog_gain_glob >> (i * 8)));
	for (i = 0; i < 2; i++)
		err |= imx06a_write_reg(s_data, DIG_GAIN_GLOBAL_LOW - i,
				(u8)(digital_gain >> (i * 8)));

	if (!s_data->group_hold_active)
		err |= imx06a_write_reg(s_data, REGHOLD, 0x00);

	if (err) {
		dev_dbg(dev, "%s: GAIN control error\n", __func__);
		return err;
	}

	dev_dbg(dev, "%s: gain val [%lld] reg [%d]\n", __func__, val, analog_gain_glob);
	imx06a_read_qsc_data_sensor(priv->i2c_client, tc_dev);
	return 0;
}

static int imx06a_set_exposure(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx06a *priv = (struct imx06a *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	struct v4l2_ctrl *ctrl;
	int err = 0;
	u64 integration_time_line;
	int i;
	u8 cit_lshift = 0;

	if (val > s_data->exposure_max_range)
		val = s_data->exposure_max_range;
	else if (val < s_data->exposure_min_range)
		val = s_data->exposure_min_range;


	integration_time_line = ((val - priv->integration_offset)
					* IMX06A_K_FACTOR) / priv->line_time;


	if (integration_time_line > IMX06A_MAX_COARSE_INTEG_TIME) {
		fr_calc_lshift(&integration_time_line, &cit_lshift, IMX06A_MAX_COARSE_INTEG_TIME);
		integration_time_line = integration_time_line << cit_lshift;
	}

	if (s_data->mode == IMX06A_MODE_4x2_BINNING_4096x3072) {
		if (integration_time_line < IMX06A_COARSE_INTEG_TIME_MIN_4_2_BINNING)
			integration_time_line = IMX06A_COARSE_INTEG_TIME_MIN_4_2_BINNING;
	} else if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
		if (integration_time_line < IMX06A_COARSE_INTEG_TIME_MIN_FULL_PIXEL)
			integration_time_line = IMX06A_COARSE_INTEG_TIME_MIN_FULL_PIXEL;
	}

	if (integration_time_line > priv->frame_length - IMX06A_COARSE_INTEG_TIME_MAX_MARGIN) {
		integration_time_line = priv->frame_length - IMX06A_COARSE_INTEG_TIME_MAX_MARGIN;
	}

	if (!s_data->group_hold_active)
		err = imx06a_write_reg(s_data, REGHOLD, 0x01);

	for (i = 0; i < 2; i++) {
		err |= imx06a_write_reg(s_data, (IMX06A_COARSE_INTEG_TIME_LOW - i),
			(u8)((integration_time_line >> cit_lshift) >> (i * 8)));
	}

	err |= imx06a_write_reg(s_data, IMX06A_CIT_LSHIFT, cit_lshift);

	if (!s_data->group_hold_active)
		err |= imx06a_write_reg(s_data, REGHOLD, 0x00);


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
		"%s: set integration time: %lld [us], integration time: %lld [line], frame length: %llu [line]\n",
		__func__, val, integration_time_line, priv->frame_length);

	return err;

	return 0;
}

static int imx06a_set_frame_rate(struct tegracam_device *tc_dev, s64 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx06a *priv = (struct imx06a *)tc_dev->priv;
	struct device *dev = tc_dev->dev;
	int err = 0;
	u64 frm_length_lines = 0;
	u64 line_length_pck = 0;
	u64 frame_length = 0;
	s64 exposure_max_range = 0, exposure_min_range = 0;
	u8 fll_lshift = 0;
	int i = 0;
	const struct sensor_mode_properties *mode = &s_data->sensor_props.sensor_modes[s_data->mode];

	err = imx06a_read_buffered_reg_big_endian(s_data, LINE_LENGTH_PCK_HIGH, 2, &line_length_pck);


	frm_length_lines = (IMX06A_IVTPXCK_FREQ * IMX06A_IVTPX_CH ) /
					   (val / IMX06A_M_FACTOR * line_length_pck);


	frame_length = ((u64)mode->control_properties.framerate_factor * IMX06A_M_FACTOR * IMX06A_K_FACTOR) /
					(val * priv->line_time);


	if (frame_length < priv->min_frame_length)
		frame_length = priv->min_frame_length;

	if (frame_length > IMX06A_MAX_FRM_LENGTH_LINES) {
		fr_calc_lshift(&frame_length, &fll_lshift,
				IMX06A_MAX_FRM_LENGTH_LINES);
		frame_length = frame_length << fll_lshift;
	}

	priv->frame_length = frame_length;

	if (s_data->mode == IMX06A_MODE_4x2_BINNING_4096x3072) {
		exposure_min_range = IMX06A_COARSE_INTEG_TIME_MIN_4_2_BINNING
							* priv->line_time / IMX06A_K_FACTOR;
	} else if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
		exposure_min_range = IMX06A_COARSE_INTEG_TIME_MIN_FULL_PIXEL
							* priv->line_time / IMX06A_K_FACTOR;
	}

	exposure_min_range += priv->integration_offset;


	exposure_max_range = (priv->frame_length - IMX06A_COARSE_INTEG_TIME_MAX_MARGIN)
						* priv->line_time / IMX06A_K_FACTOR;
	exposure_max_range += priv->integration_offset;


	fr_update_ctrl_range(tc_dev,
						TEGRA_CAMERA_CID_EXPOSURE,
						(u64)exposure_min_range,
						(u64)exposure_max_range);


	if (!s_data->group_hold_active)
		err = imx06a_write_reg(s_data, REGHOLD, 0x01);


	for (i = 0; i < 2; i++)
		err |= imx06a_write_reg(s_data, FRM_LENGTH_LINES_LOW - i,
			(u8)((priv->frame_length >> fll_lshift) >> (i * 8)));


	err |= imx06a_write_reg(s_data, IMX06A_FLL_LSHIFT, fll_lshift);


	if (!s_data->group_hold_active)
		err |= imx06a_write_reg(s_data, REGHOLD, 0x00);

	if (err) {
		dev_err(dev, "%s: failed to set frame length\n", __func__);
	return err;
	}

	return 0;
}

static int imx06a_set_test_pattern(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	err = imx06a_write_reg(s_data, IMX06A_TP_MODE_LOW, val);
	if (err)
		goto fail;

	dev_dbg(dev, "%s++ test mode pattern: %u\n", __func__, val);

	return 0;
fail:
	dev_err(dev, "%s: error setting test pattern\n", __func__);
	return err;
}

static int imx06a_update_framerate_range(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct sensor_control_properties *ctrlprops = NULL;
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	u64 max_framerate = 0;

	ctrlprops =
		&s_data->sensor_props.sensor_modes[s_data->mode].control_properties;

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
		if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144) {
			priv->min_frame_length = IMX06A_DEFAULT_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_PIX_10BIT;
		} else if (s_data->mode == IMX06A_MODE_4x2_BINNING_4096x3072) {
			priv->min_frame_length = IMX06A_4X2_BINNING_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_4_2_BINN_10BIT;
		} else if (s_data->mode == IMX06A_MODE_8192x3072) {
			priv->min_frame_length = IMX06A_CROP_30FPS_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_CROP_30FPS_10BIT ;
		} else if (s_data->mode == IMX06A_MODE_3840x2160) {
			priv->min_frame_length = IMX06A_4K_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_4K_10BIT;
		} else {
			priv->min_frame_length = IMX06A_FULL_HD_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_HD_10BIT;
		}
	} else {
		if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144) {
			priv->min_frame_length = IMX06A_DEFAULT_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_PIX_12BIT;
		} else if (s_data->mode == IMX06A_MODE_4x2_BINNING_4096x3072) {
			priv->min_frame_length = IMX06A_4X2_BINNING_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_4_2_BINN_12BIT;
		} else if  (s_data->mode == IMX06A_MODE_8192x3072) {
			priv->min_frame_length = IMX06A_CROP_30FPS_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_CROP_30FPS_12BIT;
		} else if (s_data->mode == IMX06A_MODE_3840x2160) {
			priv->min_frame_length = IMX06A_4K_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_4K_12BIT;
		} else {
			priv->min_frame_length = IMX06A_FULL_HD_HEIGHT + IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_HD_12BIT;
		}
	}

	max_framerate = (IMX06A_G_FACTOR * IMX06A_M_FACTOR) /
					(priv->min_frame_length * priv->line_time);

	dev_dbg(dev, "%s: Max framerate is equal to %llu\n",
						__func__, max_framerate);

	fr_update_ctrl_range(tc_dev, TEGRA_CAMERA_CID_FRAME_RATE,
				ctrlprops->min_framerate, max_framerate);

	return 0;
}

static int imx06a_set_operation_mode(struct tegracam_device *tc_dev, u32 val)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_pdata *pdata = s_data->pdata;

	if (pdata->xmaster_gpio)
		fr_gpio_set(s_data, pdata->xmaster_gpio, val);

	return 0;
}

static int imx06a_set_custom_ctrls(struct v4l2_ctrl *ctrl)
{
    struct tegracam_ctrl_handler *handler =
                                container_of(ctrl->handler,
                                struct tegracam_ctrl_handler, ctrl_handler);
    struct tegracam_device *tc_dev = handler->tc_dev;
    int err = 0;

	switch (ctrl->id) {
		case IMX06A_CAMERA_CID_QBC_REMOSAIC:
			err = imx06a_set_qbc_remosaic(tc_dev, ctrl->val);
			break;
		case IMX06A_CAMERA_CID_QSC_ENABLE:
			err = imx06a_update_qsc_control(tc_dev, ctrl->val);
			break;
		default:
			pr_err("%s:	unknown ctrl id.\n", __func__);
		return -EINVAL;
	}
	return err;
}

static int imx06a_read_qsc_data(struct i2c_client *eeprom, u16 offset,  u8 *buf, size_t len)
{
    struct i2c_msg msgs[2];
    u8 addr[2];
    int ret;

    addr[0] = (offset >> 8) & 0xff;
    addr[1] = offset & 0xff;

    msgs[0].addr  = eeprom->addr;
    msgs[0].flags = 0;
    msgs[0].len   = 2;
    msgs[0].buf   = addr;

    msgs[1].addr  = eeprom->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = len;
    msgs[1].buf   = buf;

    ret = i2c_transfer(eeprom->adapter, msgs, 2);
    if (ret < 0)
        return ret;
    if (ret != 2)
        return -EIO;

    return 0;
}

static int imx06a_apply_qsc_data( struct i2c_client *sensor, u16 start_reg,
								const u8 *data, size_t len)
{

    const size_t chunk = 16;
    size_t done = 0;
    int ret;

    while (done < len) {
        size_t n = min_t(size_t, chunk, len - done);
        u16 reg = start_reg + done;
        u8 *tx;

        tx = kmalloc(2 + n, GFP_KERNEL);
        if (!tx)
            return -ENOMEM;

        tx[0] = (reg >> 8) & 0xff;
        tx[1] = reg & 0xff;
        memcpy(&tx[2], data + done, n);

        {
            struct i2c_msg msg = {
                .addr  = sensor->addr,
                .flags = 0,
                .len   = 2 + n,
                .buf   = tx,
            };

            ret = i2c_transfer(sensor->adapter, &msg, 1);
            if (ret == 1)
                ret = 0;
            else if (ret >= 0)
                ret = -EIO;
        }

        kfree(tx);

        if (ret)
            return ret;

        done += n;
		usleep_range(3000, 3000);
    }

    return 0;

}

static bool imx06a_is_qsc_data_zero(const u8 *qsc_data) {
	size_t i = 0;
	for (i = 0; i < IMX06A_QSC_DATA_SIZE_BYTES; ++i) {
		if ((unsigned char) qsc_data[i] != 0x00) {
			return false;
		}
	}
	return true;
}

static int imx06a_update_qsc_control(struct tegracam_device *tc_dev, u32 val)
{
    struct camera_common_data *s_data = tc_dev->s_data;
    struct device *dev = tc_dev->dev;
    struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
    int err = 0;
	u8 qsc_updated_val = 0;

	if (priv->s_data->power->state == SWITCH_OFF)
        return 0;

	if ((s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) &&
		val && priv->qsc_control_byte == 0x01 ) {
		qsc_updated_val = 0x01;
	} else {
		qsc_updated_val = 0x00;
	}

    err = imx06a_write_reg(s_data, IMX06A_QSC_CALIBRATION_EN, qsc_updated_val);
    if (err)
        goto fail;

    dev_dbg(dev, "%s++ QSC enable: %u\n", __func__, qsc_updated_val);
    return 0;

fail:
    dev_err(dev, "%s: error updating QSC\n", __func__);
    return err;
}

static int imx06a_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx06a *priv = (struct imx06a *)s_data->priv;

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
		goto imx06a_mclk_fail;
	}

	usleep_range(1, 2);

	if (strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (pw->reset_gpio) {
			fr_gpio_set(s_data, pw->reset_gpio, 1);
		}
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

	return 0;

imx06a_mclk_fail:
	mutex_unlock(&priv->pw_mutex);
	dev_err(dev, "%s failed.\n", __func__);

	return -ENODEV;
}

static int imx06a_power_off(struct camera_common_data *s_data)
{
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;
	struct imx06a *priv = (struct imx06a *)s_data->priv;
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

static int imx06a_power_get(struct tegracam_device *tc_dev)
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
			dev_err(dev, "unable to get parent clock %s",
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

static int imx06a_power_put(struct tegracam_device *tc_dev)
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

static int imx06a_communication_verify(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err = 0;
	u64 frm_length_lines = 0;

	err = imx06a_read_buffered_reg_big_endian(s_data, FRM_LENGTH_LINES_LOW, 2, &frm_length_lines);
	if (err) {
		dev_err(dev, "%s: failed to read FRM_LENGTH_LINES\n", __func__);
		return err;
	}

	priv->frame_length = frm_length_lines;

	return err;
}

static struct camera_common_pdata *imx06a_parse_dt(struct tegracam_device *tc_dev)
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

	match = of_match_device(imx06a_of_match, dev);
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

static int imx06a_calculate_line_time(struct tegracam_device *tc_dev)
{
	struct imx06a *priv = (struct imx06a *)tc_dev->priv;
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	u64 line_length_pck = 0;
	s32 signed_fine_integ_time_ex = 0;
	int err;

	dev_dbg(dev, "%s:++\n", __func__);

	err = imx06a_read_buffered_reg_big_endian(s_data, LINE_LENGTH_PCK_HIGH, 2, &line_length_pck);
	if (err) {
		dev_err(dev, "%s: reading fine integration time value error\n", __func__);
		return err;
	}

	priv->line_time = line_length_pck * IMX06A_K_FACTOR /
					 (IMX06A_IVTPX_CH * IMX06A_IVTPXCK_FREQ_MHZ);

	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10) {
		if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
			signed_fine_integ_time_ex = sign_extend32(IMX06A_FINE_INTEG_TIME_EX_FULL_PIXEL_10 & 0xFFFF, 15);
		} else  {
			signed_fine_integ_time_ex = sign_extend32(IMX06A_FINE_INTEG_TIME_EX_4_2_ADJ_PIXEL_BINNING_10 & 0xFFFF, 15);
		}
	} else {
		if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
			signed_fine_integ_time_ex = sign_extend32(IMX06A_FINE_INTEG_TIME_EX_FULL_PIXEL_12 & 0xFFFF, 15);
		} else {
			signed_fine_integ_time_ex = sign_extend32(IMX06A_FINE_INTEG_TIME_EX_4_2_ADJ_PIXEL_BINNING_10 & 0xFFFF, 15);
		}
	}

	priv->integration_offset = ((s64)signed_fine_integ_time_ex * (s64)priv->line_time) /
							   ((s64)(line_length_pck * (s64)IMX06A_K_FACTOR));

	return 0;
}

static int imx06a_after_set_pixel_format(struct camera_common_data *s_data)
{
	struct device *dev = s_data->dev;
	struct tegracam_device *tc_dev = to_tegracam_device(s_data);
	struct imx06a *priv = (struct imx06a *)tc_dev->priv;
	struct v4l2_ctrl *ctrl;
	int err;

	dev_dbg(dev, "%s++\n", __func__);

	if (priv->current_pixel_format != s_data->colorfmt->code) {
		ctrl = fr_find_v4l2_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL);
		switch (s_data->colorfmt->code) {
		case MEDIA_BUS_FMT_SRGGB10_1X10:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB12_1X12)
				err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 >> 2),
						IMX06A_DEFAULT_BLACK_LEVEL_10BPP,
						0, IMX06A_MAX_BLACK_LEVEL_10BPP);
			else
				err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 2),
						IMX06A_DEFAULT_BLACK_LEVEL_10BPP,
						0, IMX06A_MAX_BLACK_LEVEL_10BPP);
			priv->current_pixel_format = MEDIA_BUS_FMT_SRGGB10_1X10;
			break;
		case MEDIA_BUS_FMT_SRGGB12_1X12:
			if (priv->current_pixel_format == MEDIA_BUS_FMT_SRGGB10_1X10)
				err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 2),
						IMX06A_DEFAULT_BLACK_LEVEL_12BPP,
						0, IMX06A_MAX_BLACK_LEVEL_12BPP);
			else
				err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
						 (*ctrl->p_cur.p_s64 << 4),
						IMX06A_DEFAULT_BLACK_LEVEL_12BPP,
						0, IMX06A_MAX_BLACK_LEVEL_12BPP);
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

static int imx06a_set_pixel_format(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	int err = 0;

	switch (s_data->colorfmt->code) {
	case MEDIA_BUS_FMT_SRGGB10_1X10:
		err = imx06a_write_table(priv, mode_table[IMX06A_10BIT_MODE]);
		break;
	case MEDIA_BUS_FMT_SRGGB12_1X12:
		err = imx06a_write_table(priv, mode_table[IMX06A_12BIT_MODE]);
		break;
	default:
		dev_err(dev, "%s: unknown pixel format\n", __func__);
		return -EINVAL;
	}

	return err;
}

static int imx06a_set_mode(struct tegracam_device *tc_dev)
{
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;
	u8 *qsc_data;

	dev_dbg(dev, "%s: setting mode %u\n", __func__, s_data->mode);

	err = imx06a_write_table(priv, mode_table[IMX06A_INIT_SETTINGS]);
	if (err) {
		dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
		return err;
	}

	err = imx06a_set_pixel_format(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to write format to image sensor\n",
								__func__);
		return err;
	}

	err = imx06a_write_table(priv, mode_table[s_data->mode]);
	if (err) {
		dev_err(dev, "%s: unable to write table for mode %u\n", __func__,
								s_data->mode);
		return err;
	}

	if ((s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 ||
		 s_data->mode == IMX06A_MODE_8192x3072) &&
		 s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB12_1X12) {
		err = imx06a_write_table(priv, mode_table[IMX06A_12BIT_PARTIAL_PD_MODE]);
		if (err) {
			dev_err(dev, "%s: unable to initialize sensor settings\n",
								__func__);
			return err;
		}
	}

	err = imx06a_set_test_pattern(tc_dev, fr_get_v4l2_ctrl_value(tc_dev,
				TEGRA_CAMERA_CID_TEST_PATTERN, CURRENT));
	if (err) {
		dev_err(dev, "%s: unable to set Test pattern\n", __func__);
		return err;
	}

    err = imx06a_set_qbc_remosaic(tc_dev, fr_get_v4l2_ctrl_value(tc_dev, IMX06A_CAMERA_CID_QBC_REMOSAIC, CURRENT));
    if (err) {
        dev_err(dev, "%s: unable to set remosaic\n", __func__);
        return err;
    }

	s_data->override_enable = true;

	err = imx06a_calculate_line_time(priv->tc_dev);
	if (err)
		return err;

	err = imx06a_update_framerate_range(tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to update frame range\n", __func__);
		return err;
	}

	if (!priv->eeprom) {
		dev_err(dev, "eeprom device not found\n");
		return -ENODEV;
	}

	if (s_data->mode == IMX06A_MODE_FULL_PIX_8192x6144 || s_data->mode == IMX06A_MODE_8192x3072) {
		err = imx06a_read_qsc_data(priv->eeprom, 0x0000, &priv->qsc_control_byte, 1);

		if (priv->qsc_control_byte == 0x01) {
			qsc_data = kmalloc(IMX06A_QSC_DATA_SIZE_BYTES, GFP_KERNEL);
			if (!qsc_data) {
				dev_err(dev, "allocating QSC data memory failed\n");
				return -ENOMEM;
			}

			err = imx06a_read_qsc_data(priv->eeprom, 0x0001, qsc_data, IMX06A_QSC_DATA_SIZE_BYTES);
			if (err) {
				dev_err(dev, "QSC data reading failed\n");
				goto out;
			}

			if (imx06a_is_qsc_data_zero(qsc_data)) {
				priv->qsc_control_byte = 0x00;
				dev_dbg(dev, "%s++ QSC data in EEPROM is 0x00 and will not be applied\n", __func__);
				goto out;
			}

			err = imx06a_apply_qsc_data(priv->i2c_client, IMX06A_QSC_DATA_ADDR, qsc_data, IMX06A_QSC_DATA_SIZE_BYTES);
			if (err) {
				dev_err(dev, "QSC data applying failed\n");
				goto out;
			}
	out:
			kfree(qsc_data);
		}
	}
	imx06a_update_qsc_control(tc_dev, fr_get_v4l2_ctrl_value(tc_dev, IMX06A_CAMERA_CID_QSC_ENABLE, CURRENT));


	dev_dbg(dev, "%s: set mode %u succesfully\n", __func__, s_data->mode);
	return 0;
}

static int imx06a_start_streaming(struct tegracam_device *tc_dev)
{
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
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

	err = imx06a_write_table(priv, mode_table[IMX06A_MODE_START_STREAM]);
	if (err)
		goto exit;

	return 0;

exit:
	dev_err(dev, "%s: error setting stream\n", __func__);
	return err;
}

static int imx06a_stop_streaming(struct tegracam_device *tc_dev)
{
	struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
	struct camera_common_data *s_data = tc_dev->s_data;
	struct device *dev = tc_dev->dev;
	int err;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl"))) {
		/* disable serdes streaming */
		max96792_stop_streaming(priv->s_data->dser_dev, dev);
	}

	err = imx06a_write_table(priv, mode_table[IMX06A_MODE_STOP_STREAM]);
	if (err)
		return err;
	usleep_range(priv->frame_length * priv->line_time / IMX06A_K_FACTOR,
		priv->frame_length * priv->line_time / IMX06A_K_FACTOR + 1000);

	return 0;
}

static struct camera_common_sensor_ops imx06a_common_ops = {
	.numfrmfmts = ARRAY_SIZE(imx06a_frmfmt),
	.frmfmt_table = imx06a_frmfmt,
	.power_on = imx06a_power_on,
	.power_off = imx06a_power_off,
	.write_reg = imx06a_write_reg,
	.read_reg = imx06a_read_reg,
	.parse_dt = imx06a_parse_dt,
	.power_get = imx06a_power_get,
	.power_put = imx06a_power_put,
	.set_mode = imx06a_set_mode,
	.start_streaming = imx06a_start_streaming,
	.stop_streaming = imx06a_stop_streaming,
	.after_set_pixel_format = imx06a_after_set_pixel_format,
	.init_private_controls = imx06a_ctrls_init,
};

static int imx06a_gmsl_serdes_setup(struct imx06a *priv)
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

static void imx06a_gmsl_serdes_reset(struct imx06a *priv)
{
	mutex_lock(&serdes_lock__);

	max96793_reset_control(priv->s_data->ser_dev);
	max96792_reset_control(priv->s_data->dser_dev, &priv->i2c_client->dev);

	max96792_power_off(priv->s_data->dser_dev, &priv->s_data->g_ctx);

	mutex_unlock(&serdes_lock__);
}

static int imx06a_board_setup(struct imx06a *priv)
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

		err = imx06a_gmsl_serdes_setup(priv);
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

	err = imx06a_power_on(s_data);
	if (err) {
		dev_err(dev,
		"Error %d during power on sensor\n", err);
		return err;
	}

	err = imx06a_communication_verify(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to communicate with sensor\n",
								__func__);
		goto error2;
	}

	err = imx06a_calculate_line_time(priv->tc_dev);
	if (err) {
		dev_err(dev, "%s: unable to calculate line time\n", __func__);
		goto error2;
	}

	priv->min_frame_length = IMX06A_DEFAULT_HEIGHT +
						IMX06A_MIN_FRAME_LENGTH_DELTA_FULL_PIX_10BIT;

error2:
	imx06a_power_off(s_data);
	camera_common_mclk_disable(s_data);


	return err;
}

static int imx06a_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s++\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops imx06a_subdev_internal_ops = {
	.open = imx06a_open,
};

static struct tegracam_ctrl_ops imx06a_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_gain = imx06a_set_gain,
	.set_exposure = imx06a_set_exposure,
	.set_frame_rate = imx06a_set_frame_rate,
	.set_group_hold = imx06a_set_group_hold,
	.set_test_pattern = imx06a_set_test_pattern,
	.set_operation_mode = imx06a_set_operation_mode,
	.set_black_level = imx06a_set_black_level,
};

static int imx06a_ctrls_init(struct tegracam_device *tc_dev)
{
    struct imx06a *priv = (struct imx06a *)tegracam_get_privdata(tc_dev);
    struct device *dev = tc_dev->dev;
    struct camera_common_data *s_data = priv->s_data;
    struct v4l2_ctrl_config *ctrl_cfg;
    struct v4l2_ctrl *ctrl;
    struct tegracam_ctrl_handler *handler = s_data->tegracam_ctrl_hdl;
    int numctrls;
    int err, i;

    numctrls = ARRAY_SIZE(imx06a_custom_ctrl_list);

    for (i = 0; i < numctrls; i++) {
        ctrl_cfg = &imx06a_custom_ctrl_list[i];

        ctrl = v4l2_ctrl_new_custom(&handler->ctrl_handler, ctrl_cfg, NULL);
        if (ctrl == NULL) {
            dev_err(dev, "%s: Failed to create control %s\n", __func__,
                                                              ctrl_cfg->name);
            continue;
        }

        if (ctrl_cfg->type == V4L2_CTRL_TYPE_STRING &&
                ctrl_cfg->flags & V4L2_CTRL_FLAG_READ_ONLY) {
            ctrl->p_new.p_char = devm_kzalloc(tc_dev->dev,
                                    ctrl_cfg->max + 1, GFP_KERNEL);
            if (!ctrl->p_new.p_char) {
                dev_err(dev, "%s: failed to allocate memory\n", __func__);
                return -ENOMEM;
            }
        }
        handler->ctrls[handler->numctrls + i] = ctrl;
        dev_dbg(dev, "%s: Added custom control %s to handler index: %d\n",
            __func__, ctrl_cfg->name,  handler->numctrls + i);
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

static int imx06a_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct tegracam_device *tc_dev;
	struct imx06a *priv;
	struct sensor_control_properties *ctrlprops = NULL;
	int err;

	dev_info(dev, "probing v4l2 sensor\n");

	if (!IS_ENABLED(CONFIG_OF) || !client->dev.of_node)
		return -EINVAL;

	priv = devm_kzalloc(dev,
			sizeof(struct imx06a), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	tc_dev = devm_kzalloc(dev,
			sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	mutex_init(&priv->pw_mutex);
	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "imx06a", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &imx06a_common_ops;
	tc_dev->v4l2sd_internal_ops = &imx06a_subdev_internal_ops;
	tc_dev->tcctrl_ops = &imx06a_ctrl_ops;

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

	err = imx06a_board_setup(priv);
	if (err) {
		dev_err(dev, "board setup failed\n");
		return err;
	}

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "tegra camera subdev registration failed\n");
		return err;
	}

	err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_BLACK_LEVEL,
					IMX06A_DEFAULT_BLACK_LEVEL_10BPP,
					IMX06A_DEFAULT_BLACK_LEVEL_10BPP, 0,
					IMX06A_MAX_BLACK_LEVEL_10BPP);
	if (err)
		return err;

	err = imx06a_update_ctrl(tc_dev, TEGRA_CAMERA_CID_TEST_PATTERN, 0, 0, 0,
				(ARRAY_SIZE(imx06a_test_pattern_menu)-1));
	if (err)
		return err;

	list_add_tail(&priv->entry, &imx06a_sensor_list);


	priv->eeprom = i2c_new_dummy_device(client->adapter, 0x56);
	if (IS_ERR(priv->eeprom)) {
		err = PTR_ERR(priv->eeprom);
		dev_err(dev, "failed to create EEPROM client\n");
		return err;
	}

	dev_info(dev, "detected imx06a sensor\n");

	return 0;
}

static int imx06a_remove(struct i2c_client *client)
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct imx06a *priv;

	if (!s_data) {
		dev_err(&client->dev, "camera common data is NULL\n");
		return -EINVAL;
	}

	priv = (struct imx06a *)s_data->priv;

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		imx06a_gmsl_serdes_reset(priv);

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	if (!(strcmp(s_data->pdata->gmsl, "gmsl")))
		mutex_destroy(&serdes_lock__);

	if (priv->eeprom) {
		i2c_unregister_device(priv->eeprom);
	}

	return 0;
}

static const struct i2c_device_id imx06a_id[] = {
	{ "imx06a", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, imx06a_id);

static struct i2c_driver imx06a_i2c_driver = {
	.driver = {
		.name = "imx06a",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(imx06a_of_match),
	},
	.probe = imx06a_probe,
	.remove = imx06a_remove,
	.id_table = imx06a_id,
};

module_i2c_driver(imx06a_i2c_driver);

MODULE_DESCRIPTION("Media Controller driver for Sony IMX06A");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
