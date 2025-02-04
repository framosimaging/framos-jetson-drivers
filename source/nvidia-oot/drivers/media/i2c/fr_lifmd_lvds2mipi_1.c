// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_lifmd_lvds2mipi_1.c - framos lifmd_lvds2mipi_1.c driver
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

#include <media/tegra_v4l2_camera.h>
#include <media/tegracam_core.h>

#include "media/fr_sensor_common.h"
#include "media/fr_lifmd_lvds2mipi_1.h"
#include "i2c/fr_i2c_generic.h"


static const struct of_device_id lifmd_lvds2mipi_1_of_match[] = {
	{ .compatible = "framos,lifmd_lvds2mipi_1",},
	{ },
};
MODULE_DEVICE_TABLE(of, lifmd_lvds2mipi_1_of_match);

LIST_HEAD(lifmd_lvds2mipi_1_dev_list);

int lifmd_lvds2mipi_1_supported_fw_version[4] = {1, 2, 1, 0};
int lifmd_lvds2mipi_1_supported_latest_fw_version[4] = {1, 3, 1, 10};

static int get_pix_bit_from_mbus(struct camera_common_data *s_data)
{
	/* Bayer MBUS_CODEs */
	if (s_data->colorfmt->code == MEDIA_BUS_FMT_SBGGR8_1X8 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGBRG8_1X8 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGRBG8_1X8 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB8_1X8)
		return 8;
	else if (s_data->colorfmt->code == MEDIA_BUS_FMT_SBGGR10_1X10 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGBRG10_1X10 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGRBG10_1X10 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB10_1X10)
		return 10;
	else if (s_data->colorfmt->code == MEDIA_BUS_FMT_SBGGR12_1X12 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGBRG12_1X12 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SGRBG12_1X12 ||
			s_data->colorfmt->code == MEDIA_BUS_FMT_SRGGB12_1X12)
		return 12;

	dev_err(s_data->dev, "%s: unknown media bus format", __func__);
	return -1;
}

/* Read register */
static int lifmd_lvds2mipi_1_read_reg(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
							u16 addr, u8 *val)
{
	int err = 0;

	err = regmap_read(lifmd_lvds2mipi_1->regmap, addr, (int *)val);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: i2c read failed, 0x%x = %x\n",
							__func__, addr, *val);

	return err;
}

/* Write register */
static int lifmd_lvds2mipi_1_write_reg(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
							u16 addr, u8 val)
{
	int err = 0;

	err = regmap_write(lifmd_lvds2mipi_1->regmap, addr, val);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: i2c write failed, 0x%x = %x\n",
		__func__, addr, val);

	return err;
}

/* Write multiple sequential registers */
static int lifmd_lvds2mipi_1_write_buffered_reg(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u16 addr_low, u8 number_of_registers, u32 val)
{
	int err = 0;
	int i;

	for (i = 0; i < number_of_registers; i++) {
		err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, addr_low + i,
							(u8)(val >> (i * 8)));
		if (err) {
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: buffered register write error\n", __func__);
			return err;
		}
	}

	return err;
}

/* Read multiple sequential registers */
static int lifmd_lvds2mipi_1_read_buffered_reg(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u16 addr_low, u8 number_of_registers, u32 *val)
{
	int err = 0;
	int i;
	u8 reg;

	*val = 0;

	for (i = 0; i < number_of_registers; i++) {
		err = lifmd_lvds2mipi_1_read_reg(lifmd_lvds2mipi_1, addr_low + i, &reg);
		*val += reg << (i * 8);
		if (err) {
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: buffered register read error\n", __func__);
			return err;
		}
	}

	return err;
}

static int read_lifmd_lvds2mipi_1_fw_name(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1, char data[])
{
	int err = 0;
	int i, j;
	u8 val;

	for (i = 0, j = 0; i < LIFMD_LVDS2MIPI_1_FW_NAME_REG_LENGTH ; i++) {
		err = lifmd_lvds2mipi_1_read_reg(lifmd_lvds2mipi_1,
			(LIFMD_LVDS2MIPI_1_FW_NAME_BASE + LIFMD_LVDS2MIPI_1_FW_NAME_REG_LENGTH - i), &val);
		if (err) {
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "error reading lifmd_lvds2mipi_1 configruation");
			return err;
		}

		if (!val)
			continue;

		data[j] = val;
		j++;
	}

	return err;
}

static int read_lifmd_lvds2mipi_1_fw_version(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1, char data[])
{
	int err = 0;
	int i;
	u32 val;

	err = lifmd_lvds2mipi_1_read_buffered_reg(lifmd_lvds2mipi_1,
				LIFMD_LVDS2MIPI_1_FW_VERSION_BASE, 4, &val);
	if (err) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"error reading lifmd_lvds2mipi_1 configruation");
		return err;
	}

	for (i = LIFMD_LVDS2MIPI_1_FW_VERSION_REG_LENGTH - 1; i >= 0 ; i--)
		data[LIFMD_LVDS2MIPI_1_FW_VERSION_REG_LENGTH - 1 - i] = (val >> i * 8) & 0xFF;

	return 0;
}

static int check_lifmd_lvds2mipi_1_image_data(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 data[], struct camera_common_data *s_data)
{
	int err = 0;
	u8 pix_format, lvds_line;
	int mbus_pix_format = get_pix_bit_from_mbus(s_data);

	err = lifmd_lvds2mipi_1_read_reg(lifmd_lvds2mipi_1,
				LIFMD_LVDS2MIPI_1_PIX_FORMAT, &pix_format);
	if (err)
		goto fail;
	data[0] = pix_format;

	err = lifmd_lvds2mipi_1_read_reg(lifmd_lvds2mipi_1,
				LIFMD_LVDS2MIPI_1_LVDS_NUM_CH, &lvds_line);
	if (err)
		goto fail;
	data[1] = lvds_line;

	if (mbus_pix_format != pix_format || lifmd_lvds2mipi_1->sensor_numch != lvds_line)
		return -1;

	return 0;
fail:
	dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "error reading lifmd_lvds2mipi_1 configuration");
	return err;
}

static int lifmd_lvds2mipi_1_write_tg_parameters(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_TG_MODE, tg_params->tg_mode);
	if (err)
		goto fail;

	if (tg_params->is_shutter_mode == SEQ_TRIGGER
		&& (tg_params->frame_active_width > tg_params->frame_length)) {
		tg_params->frame_active_width = tg_params->frame_length - MIN_TRIGGER_HIGH_WIDTH_TGES;
		tg_params->frame_inactive_width = MIN_TRIGGER_HIGH_WIDTH_TGES;
	}

	if (tg_params->tg_mode == TG_TRIGGER_MODE) {
		tg_params->frame_inactive_width = tg_params->t_tgpd;

		err = lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
					LIFMD_LVDS2MIPI_1_FRAME_ACTIVE_WIDTH,
					4, tg_params->expanded_exposure);
	} else
		err = lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
					LIFMD_LVDS2MIPI_1_FRAME_ACTIVE_WIDTH,
					4, tg_params->frame_active_width);

	err |= lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
					LIFMD_LVDS2MIPI_1_FRAME_INACTIVE_WIDTH,
					4, tg_params->frame_inactive_width);
	if (err)
		goto fail;

	err = lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
						LIFMD_LVDS2MIPI_1_FRAME_DELAY,
						4, tg_params->frame_delay);
	if (err)
		goto fail;

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1,
			LIFMD_LVDS2MIPI_1_SYNC_LOGIC, tg_params->sync_logic);

	err |= lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1,
			LIFMD_LVDS2MIPI_1_OUT_LOGIC, tg_params->out_logic);
	if (err)
		goto fail;

	return 0;
fail:
	dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"error writing lifmd_lvds2mipi_1 slave parameters");
	return err;
}

int lifmd_lvds2mipi_1_verify_fw_compatibility(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
		struct v4l2_ctrl *ctrl, struct camera_common_data *s_data)
{
	int err = 0;
	int fw_ver_err = 0;
	int fw_ver_warn = 0;
	int i = 0;
	u8 cnt1 = 0;
	u8 cnt2 = 0;
	u32 image_data[4] = {0};
	char fw_name[LIFMD_LVDS2MIPI_1_FW_NAME_REG_LENGTH + 1] = "";
	char fw_version[LIFMD_LVDS2MIPI_1_FW_VERSION_REG_LENGTH + 1] = "";
	u8 val;

	if (*lifmd_lvds2mipi_1->is_pw_state == SWITCH_OFF) {
		val = 0;
		*ctrl->p_new.p_s64 = val;
		*ctrl->p_cur.p_s64 = val;
		dev_info(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: Error because checking fw compatibility in power OFF mode!\n", __func__);
		return 0;
	}

	err = read_lifmd_lvds2mipi_1_fw_name(lifmd_lvds2mipi_1, fw_name);
	if (err)
		goto fail;

	err = read_lifmd_lvds2mipi_1_fw_version(lifmd_lvds2mipi_1, fw_version);
	if (err)
		goto fail;

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: CS fw ver.:%d.%d.%d.%d\n",
		__func__, fw_version[0], fw_version[1], fw_version[2], fw_version[3]);

	for (i = 0; i < 4; i++) {
		if (fw_version[i] == lifmd_lvds2mipi_1_supported_fw_version[i])
			cnt1 += 1;
		if (fw_version[i] == lifmd_lvds2mipi_1_supported_latest_fw_version[i])
			cnt2 += 1;
	}

	if (cnt1 == 4)
		fw_ver_warn = 1;
	else
		fw_ver_err = 1;
	if (cnt2 == 4)
		fw_ver_err = 0;

	if (fw_ver_err) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"Image sensor driver don't support this firmware version. Most features will probably not work.\n");
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"RECONFIGURE FIRMWARE with the latest version - %d.%d.%d.%d!\n",
			lifmd_lvds2mipi_1_supported_latest_fw_version[0],
			lifmd_lvds2mipi_1_supported_latest_fw_version[1],
			lifmd_lvds2mipi_1_supported_latest_fw_version[2],
			lifmd_lvds2mipi_1_supported_latest_fw_version[3]);
	}

	else if (fw_ver_warn) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"lifmd_lvds2mipi_1 is not configured with the latest firmware version (Latest version - %d.%d.%d.%d). Some features may not work properly\n",
			lifmd_lvds2mipi_1_supported_latest_fw_version[0],
			lifmd_lvds2mipi_1_supported_latest_fw_version[1],
			lifmd_lvds2mipi_1_supported_latest_fw_version[2],
			lifmd_lvds2mipi_1_supported_latest_fw_version[3]);
	}

	err = check_lifmd_lvds2mipi_1_image_data(lifmd_lvds2mipi_1, image_data, s_data);
	if (err) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"lifmd_lvds2mipi_1 firmware and sensor configuration doesn't match!\n");
		dev_info(&lifmd_lvds2mipi_1->i2c_client->dev,
				"Current firmware configuration:\n");
		dev_info(&lifmd_lvds2mipi_1->i2c_client->dev,
				" - Firmware name: \t%s\n", fw_name);
		dev_info(&lifmd_lvds2mipi_1->i2c_client->dev,
			" - Pixel format: \t%u bits/pix\n", image_data[0]);
		dev_info(&lifmd_lvds2mipi_1->i2c_client->dev,
			" - Firmware LVDS ch: \t%u\n", image_data[1]);
		if (fw_ver_err) {
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"RECONFIGURE FIRMWARE with %d.%d.%d.%d version!\n",
			lifmd_lvds2mipi_1_supported_latest_fw_version[0],
			lifmd_lvds2mipi_1_supported_latest_fw_version[1],
			lifmd_lvds2mipi_1_supported_latest_fw_version[2],
			lifmd_lvds2mipi_1_supported_latest_fw_version[3]);
		} else
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "RECONFIGURE FIRMWARE!\n");

		val = 2;
	} else
		val = 1;

	*ctrl->p_new.p_s64 = val;
	*ctrl->p_cur.p_s64 = val;

	return 0;

fail:
	val = 0;
	*ctrl->p_new.p_s64 = val;
	*ctrl->p_cur.p_s64 = val;
	dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: failed to verify fw compatibility\n", __func__);
	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_verify_fw_compatibility);

int lifmd_lvds2mipi_1_set_readout_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
						struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_readout_mode *lifmd_lvds2mipi_1_readout_mode =
				&lifmd_lvds2mipi_1->lifmd_lvds2mipi_1_readout_mode;
	int err = 0;
	int i;
	int mbus_pix_format = get_pix_bit_from_mbus(s_data);
	u32 v_active, h_active, word_count;

	v_active = s_data->fmt_height;
	h_active = s_data->fmt_width / lifmd_lvds2mipi_1->sensor_numch;
	word_count = (s_data->fmt_width * mbus_pix_format) / 8;

	lifmd_lvds2mipi_1_readout_mode->v_active_low = v_active & 0xFF;
	lifmd_lvds2mipi_1_readout_mode->v_active_high = (v_active >> 8) & 0xFF;
	lifmd_lvds2mipi_1_readout_mode->h_active_unit_low = h_active & 0xFF;
	lifmd_lvds2mipi_1_readout_mode->h_active_unit_high = (h_active >> 8) & 0xFF;
	lifmd_lvds2mipi_1_readout_mode->word_count_low = word_count & 0xFF;
	lifmd_lvds2mipi_1_readout_mode->word_count_high = (word_count >> 8) & 0xFF;

	for (i = 0; i < sizeof(struct lifmd_lvds2mipi_1_readout_mode)/sizeof(u8); i++) {
		err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1,
					(LIFMD_LVDS2MIPI_1_LINE_SKIP_COUNT_RW + i),
					*((u8 *)lifmd_lvds2mipi_1_readout_mode + sizeof(u8) * i));
		if (err)
			goto fail;
	}
	return 0;
fail:
	dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
		"%s: failed to set lifmd_lvds2mipi_1 readout mode\n", __func__);
	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_set_readout_mode);

/**
 * Set Image Sensor operation and trigger mode
 *	0 - Master mode, 1 - Slave Normal trigger mode, 2 - Slave Seq. trigger mode
 */
int lifmd_lvds2mipi_1_set_is_operation_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;
	u8 val;

	if (tg_params->is_operation_mode == SLAVE_MODE &&
				tg_params->is_shutter_mode == NORMAL_EXPO)
		val = 1;
	else if (tg_params->is_operation_mode == SLAVE_MODE &&
				tg_params->is_shutter_mode == SEQ_TRIGGER)
		val = 2;
	else
		val = 0;

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_MASTER_SLAVE, val);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: error setting operation mode\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_set_is_operation_mode);

/**
 * Set timing generator mode
 *	0 - Disabled, 1 - Master mode, 2 - Slave mode, 3 - Trigger mode,
 */
int lifmd_lvds2mipi_1_tg_set_operation_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1, struct v4l2_ctrl *ctrl)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	u8 val = *ctrl->p_new.p_u8;
	int err = 0;

	if (val == TG_DISABLED) {
		if (*lifmd_lvds2mipi_1->is_pw_state == SWITCH_ON) {
			err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1,
						LIFMD_LVDS2MIPI_1_TG_ENABLE, 0);
			if (err)
				dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: Trigger generator stop failure\n", __func__);
		}
	}

	if (tg_params->is_operation_mode == MASTER_MODE && val != TG_DISABLED) {
		val = TG_DISABLED;
		__v4l2_ctrl_s_ctrl(ctrl, val);
		dev_warn(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: sensor must be in slave mode to select TG mode\n", __func__);
	}

	if (tg_params->is_shutter_mode == NORMAL_EXPO && val == TG_TRIGGER_MODE) {
		val = TG_MASTER_MODE;
		__v4l2_ctrl_s_ctrl(ctrl, val);
		dev_warn(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: Selected TG mode is disabled in combination with selected shutter mode, switching to default\n",
								__func__);
	}

	tg_params->tg_mode = val;

	return 0;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_set_operation_mode);

/* Convert us to 1H */
static u32 tg_convert_us_to_1h(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 usec, struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	u32 tg_hmax_clk;
	u64 usec_clk;
	u32 usec_1h;

	tg_hmax_clk = (*tg_params->line_time * (s_data->def_clk_freq / 1000)) / 1000000 + tg_params->xhs_clk_offset;
	usec_clk = ((u64)usec * (s_data->def_clk_freq / 1000)) / 1000;
	/* Round result */
	usec_1h = (usec_clk + (tg_hmax_clk/2)) / tg_hmax_clk;

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: Value [%u]us, TG line time [%u]ns, Value [%u]1H\n",
		__func__, usec, (tg_hmax_clk * 1000000) / (s_data->def_clk_freq / 1000), usec_1h);

	return usec_1h;
}

/* Delay frame */
int lifmd_lvds2mipi_1_tg_delay_frame(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
					u32 val, struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;
	u32 tg_frame_delay_1h = tg_convert_us_to_1h(lifmd_lvds2mipi_1, val, s_data);

	tg_params->frame_delay = tg_frame_delay_1h;

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: error setting delay frame\n", __func__);

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: delay [%u]us, delay [%u] 1H\n",
					__func__, val, tg_frame_delay_1h);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_delay_frame);

/* TG 1H length */
int lifmd_lvds2mipi_1_tg_set_line_width(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
					struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;
	u32 tg_hmax_clk;
	u32 tg_xhs_low_width, tg_xhs_high_width;

	if (tg_params->is_operation_mode == MASTER_MODE)
		return 0;

	tg_hmax_clk = (*tg_params->line_time * (s_data->def_clk_freq / 1000)) / 1000000 + tg_params->xhs_clk_offset;

	tg_xhs_low_width = tg_params->xhs_min_active_width;
	tg_xhs_high_width = tg_hmax_clk - tg_xhs_low_width;

	err = lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
		LIFMD_LVDS2MIPI_1_LINE_ACTIVE_WIDTH, 2, tg_xhs_low_width);
	err |= lifmd_lvds2mipi_1_write_buffered_reg(lifmd_lvds2mipi_1,
		LIFMD_LVDS2MIPI_1_LINE_INACTIVE_WIDTH, 2, tg_xhs_high_width);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: error setting TG XHS WIDTH\n", __func__);

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: tg xhs [%u], xhs low [%u], xhs high [%u]\n",
				__func__, tg_hmax_clk, tg_xhs_low_width,
							tg_xhs_high_width);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_set_line_width);

/**
 * Set frame signal
 * val must be in useconds
 */
int lifmd_lvds2mipi_1_tg_set_frame_width(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1, u32 val)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;
	u32 tg_frame_high_widht;
	u32 min_trigger_high_width = 1;

	if (tg_params->is_operation_mode == MASTER_MODE)
		return 0;

	/* Write low width only in normal exposure */
	if (tg_params->is_shutter_mode == NORMAL_EXPO) {
		tg_params->frame_active_width = 2;
		dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: frame low [%u]1H\n",
				__func__, tg_params->frame_active_width);
	} else
		min_trigger_high_width = MIN_TRIGGER_HIGH_WIDTH_TGES;

	tg_frame_high_widht = tg_params->frame_length - tg_params->frame_active_width;

	if (tg_frame_high_widht <= min_trigger_high_width)
		tg_frame_high_widht = min_trigger_high_width;

	tg_params->frame_inactive_width = tg_frame_high_widht;

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: error setting TG XVS WIDTH\n", __func__);

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: tg frame [%u]lines, frame high [%u]1H\n",
			__func__, tg_params->frame_length, tg_frame_high_widht);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_set_frame_width);

/* Set trigger exposure; only for IS TRIGGER mode */
int lifmd_lvds2mipi_1_tg_set_trigger_exposure(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 val, struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;

	/* If Timing generator is in TRIGGER MODE expand exposure */
	if (tg_params->is_operation_mode == MASTER_MODE ||
				tg_params->is_shutter_mode == NORMAL_EXPO ||
					tg_params->tg_mode == TG_TRIGGER_MODE) {
		tg_params->frame_active_width = 0;
		return 0;
	}

	tg_params->frame_active_width = tg_convert_us_to_1h(lifmd_lvds2mipi_1, val, s_data);

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: error setting trigger exposure\n", __func__);

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev, "%s: frame low [%u]usec, frame low [%u]1H\n",
				__func__, val, tg_params->frame_active_width);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_set_trigger_exposure);

/* Expand trigger exposure; only for TG TRIGGER mode */
int lifmd_lvds2mipi_1_tg_expand_trigger_exposure(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 val, struct camera_common_data *s_data)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;

	/* If Timing generator is in TRIGGER MODE expand exposure */
	if (tg_params->is_shutter_mode != SEQ_TRIGGER ||
					tg_params->tg_mode == TG_SLAVE_MODE) {
		dev_warn(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: not applicable in this mode\n", __func__);
		return 0;
	}

	tg_params->expanded_exposure = tg_convert_us_to_1h(lifmd_lvds2mipi_1, val, s_data);

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: error setting expand trigger exposure\n", __func__);

	dev_dbg(&lifmd_lvds2mipi_1->i2c_client->dev,
		"%s: frame low expand val [%u]usec, frame low expand value [%u]1H\n",
				__func__, val, tg_params->expanded_exposure);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_expand_trigger_exposure);

/* Out device from software reset */
int lifmd_lvds2mipi_1_start(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	int err = 0;

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_SW_RST_N_RW, 1);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: Trigger generator software reset failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_start);

/* Set device to software reset */
int lifmd_lvds2mipi_1_stop(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	int err = 0;

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_SW_RST_N_RW, 0);
	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: Trigger generator software reset failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_stop);

/* Start timing generator */
int lifmd_lvds2mipi_1_tg_start(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	int err = 0;

	err = lifmd_lvds2mipi_1_write_tg_parameters(lifmd_lvds2mipi_1);

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: Trigger generator write parameters failure\n", __func__);

	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_TG_ENABLE, 1);

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: Trigger generator start failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_start);

/* Stop timing generator */
int lifmd_lvds2mipi_1_tg_stop(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	struct lifmd_lvds2mipi_1_time_generator_params *tg_params = &lifmd_lvds2mipi_1->tg_params;
	int err = 0;

	/* No need to stop timing generator in IS Master mode */
	if (tg_params->is_operation_mode == MASTER_MODE)
		return 0;
	err = lifmd_lvds2mipi_1_write_reg(lifmd_lvds2mipi_1, LIFMD_LVDS2MIPI_1_TG_ENABLE, 0);

	if (err)
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"%s: Trigger generator stop failure\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_tg_stop);

int lifmd_lvds2mipi_1_fw_reset(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				struct camera_common_data *s_data, int val)
{
	struct lifmd_lvds2mipi_1 *priv = dev_get_drvdata(&lifmd_lvds2mipi_1->i2c_client->dev);
	int err = 0;

	if (priv->fw_reset_gpio)
		fr_gpio_set(s_data, priv->fw_reset_gpio, val);
	else
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s: failed to use gpio cresetb\n", __func__);

	return err;
}
EXPORT_SYMBOL(lifmd_lvds2mipi_1_fw_reset);

const struct lifmd_lvds2mipi_1_readout_mode lifmd_lvds2mipi_1_def_readout_mode = {
	.left_trim_unit = 0,
	.left_trim_lane = 0,
};
EXPORT_SYMBOL(lifmd_lvds2mipi_1_def_readout_mode);

static int lifmd_lvds2mipi_1_parse_dt(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1)
{
	struct device_node *node = lifmd_lvds2mipi_1->i2c_client->dev.of_node;
	struct device_node *i2c_mux_ch_node = NULL;
	int err = 0;
	const char *name;
	const struct of_device_id *match;
	bool use_cam_gpio;

	if (!node)
		return -EINVAL;

	match = of_match_device(lifmd_lvds2mipi_1_of_match, &lifmd_lvds2mipi_1->i2c_client->dev);
	if (!match) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "Failed to find matching dt id\n");
		return -EFAULT;
	}

	err = of_property_read_string(node, "device", &name);
	if (err) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev, "device not in Device Tree\n");
		return err;
	}

	if (strcmp(name, "lifmd_lvds2mipi_1")) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"lifmd_lvds2mipi_1 not in Device Tree\n");
		return err;
	}

	i2c_mux_ch_node = of_get_parent(node);
	if (i2c_mux_ch_node == NULL) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
			"i2c mux channel node not found in Device Tree\n");
		return err;
	}
	lifmd_lvds2mipi_1->fw_reset_gpio = of_get_named_gpio(i2c_mux_ch_node, "cresetb", 0);
	if (lifmd_lvds2mipi_1->fw_reset_gpio < 0) {
		dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
					"creset_b gpio not found %d\n", err);
		return err;
	}

	node = of_find_node_by_name(NULL, "framos_platform_adapter");
	if (node) {
		use_cam_gpio = of_property_read_bool(node, "cam,use-cam-gpio");
		of_node_put(node);
	}

	if (use_cam_gpio) {
		err = cam_gpio_register(&lifmd_lvds2mipi_1->i2c_client->dev,
					lifmd_lvds2mipi_1->fw_reset_gpio);
		if (err) {
			dev_err(&lifmd_lvds2mipi_1->i2c_client->dev,
				"%s ERR can't register cam gpio fw_reset_gpio %u!\n",
				__func__, lifmd_lvds2mipi_1->fw_reset_gpio);
			return err;
		}
	}

	return 0;
}

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int lifmd_lvds2mipi_1_probe(struct i2c_client *client)
#else
static int lifmd_lvds2mipi_1_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
#endif
{
	struct device *dev = &client->dev;
	struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1;

	int err = 0;

	dev_info(dev, "Probing lifmd_lvds2mipi_1 device\n");

	lifmd_lvds2mipi_1 = devm_kzalloc(&client->dev, sizeof(struct lifmd_lvds2mipi_1), GFP_KERNEL);
	if (!lifmd_lvds2mipi_1)
		return -ENOMEM;

	lifmd_lvds2mipi_1->i2c_client = client;
	lifmd_lvds2mipi_1->regmap = devm_regmap_init_i2c(lifmd_lvds2mipi_1->i2c_client,
		&lifmd_lvds2mipi_1_regmap_config);
	if (IS_ERR(lifmd_lvds2mipi_1->regmap)) {
		dev_err(dev,
				"regmap init failed: %ld\n", PTR_ERR(lifmd_lvds2mipi_1->regmap));
		return -ENODEV;
	}

	err = lifmd_lvds2mipi_1_parse_dt(lifmd_lvds2mipi_1);
	if (err) {
		dev_err(&client->dev, "unable to parse dt\n");
		return -EFAULT;
	}

	dev_set_drvdata(dev, lifmd_lvds2mipi_1);

	INIT_LIST_HEAD(&lifmd_lvds2mipi_1->entry);
	list_add_tail(&lifmd_lvds2mipi_1->entry, &lifmd_lvds2mipi_1_dev_list);

	dev_info(dev, "Detected lifmd_lvds2mipi_1 device\n");

	return err;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int lifmd_lvds2mipi_1_remove(struct i2c_client *client)
#else
static void lifmd_lvds2mipi_1_remove(struct i2c_client *client)
#endif
{
	struct lifmd_lvds2mipi_1 *priv;

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		dev_dbg(&client->dev, "Removed lifmd_lvds2mipi_1 module device\n");
		devm_kfree(&client->dev, priv);
		client = NULL;
	}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id lifmd_lvds2mipi_1_id[] = {
	{ "lifmd_lvds2mipi_1", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, lifmd_lvds2mipi_1_id);

static struct i2c_driver lifmd_lvds2mipi_1_i2c_driver = {
	.driver = {
	.name = "lifmd_lvds2mipi_1",
	.owner = THIS_MODULE,
	.of_match_table = of_match_ptr(lifmd_lvds2mipi_1_of_match),
	},
	.probe = lifmd_lvds2mipi_1_probe,
	.remove = lifmd_lvds2mipi_1_remove,
	.id_table = lifmd_lvds2mipi_1_id,
};

module_i2c_driver(lifmd_lvds2mipi_1_i2c_driver);

MODULE_DESCRIPTION("Framos Image Sensor lifmd_lvds2mipi_1 logic");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
