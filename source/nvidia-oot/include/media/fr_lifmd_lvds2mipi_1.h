/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * lifmd_lvds2mipi_1 driver for Crosslink FPGA
 */

#ifndef __fr_lifmd_lvds2mipi_1__
#define __fr_lifmd_lvds2mipi_1__

/* lifmd FPGA firmware data */

#define LIFMD_LVDS2MIPI_1_FW_NAME_BASE			0x0010
#define LIFMD_LVDS2MIPI_1_FW_VERSION_BASE		0x0020

#define LIFMD_LVDS2MIPI_1_FW_NAME_REG_LENGTH		16
#define LIFMD_LVDS2MIPI_1_FW_VERSION_REG_LENGTH		4

#define LIFMD_LVDS2MIPI_1_IS_WIDTH_LOW			0x00A0
#define LIFMD_LVDS2MIPI_1_IS_WIDTH_HIGH			0x00A1
#define LIFMD_LVDS2MIPI_1_IS_HEIGHT_LOW			0x00A2
#define LIFMD_LVDS2MIPI_1_IS_HEIGHT_HIGH		0x00A3
#define LIFMD_LVDS2MIPI_1_DATA_RATE_LOW			0x00A4
#define LIFMD_LVDS2MIPI_1_DATA_RATE_HIGH		0x00A5
#define LIFMD_LVDS2MIPI_1_PIX_FORMAT			0x00A6
#define LIFMD_LVDS2MIPI_1_LVDS_NUM_CH			0x00A7

#define LIFMD_LVDS2MIPI_1_MASTER_SLAVE			0x00E0

#define LIFMD_LVDS2MIPI_1_LINE_SKIP_COUNT_RW		0x00F0
#define LIFMD_LVDS2MIPI_1_V_ACTIVE_LOW_RW		0x00F1
#define LIFMD_LVDS2MIPI_1_V_ACTIVE_HIGH_RW		0x00F2
#define LIFMD_LVDS2MIPI_1_LEFT_TRIM_UNIT_RW		0x00F3
#define LIFMD_LVDS2MIPI_1_LEFT_TRIM_LANE_RW		0x00F4
#define LIFMD_LVDS2MIPI_1_H_ACTIVE_UNIT_LOW_RW		0x00F5
#define LIFMD_LVDS2MIPI_1_H_ACTIVE_UNIT_HIGH_RW		0x00F6
#define LIFMD_LVDS2MIPI_1_WORD_COUNT_LOW_RW		0x00F7
#define LIFMD_LVDS2MIPI_1_WORD_COUNT_HIGH_RW		0x00F8
#define LIFMD_LVDS2MIPI_1_SW_RST_N_RW			0x00F9

#define LIFMD_LVDS2MIPI_1_TG_ENABLE			0x0100
#define LIFMD_LVDS2MIPI_1_TG_MODE			0x0101
#define LIFMD_LVDS2MIPI_1_SYNC_LOGIC			0x0102
#define LIFMD_LVDS2MIPI_1_OUT_LOGIC			0x0103

#define LIFMD_LVDS2MIPI_1_LINE_ACTIVE_WIDTH		0x0110
#define LIFMD_LVDS2MIPI_1_LINE_INACTIVE_WIDTH		0x0112

#define LIFMD_LVDS2MIPI_1_FRAME_DELAY			0x0120

#define LIFMD_LVDS2MIPI_1_FRAME_ACTIVE_WIDTH		0x0124
#define LIFMD_LVDS2MIPI_1_FRAME_INACTIVE_WIDTH		0x0128

#define MIN_TRIGGER_HIGH_WIDTH_TGES			11


extern const struct lifmd_lvds2mipi_1_readout_mode lifmd_lvds2mipi_1_def_readout_mode;

/* Enum describing available operation modes */
typedef enum {
	TG_DISABLED,
	TG_MASTER_MODE,
	TG_SLAVE_MODE,
	TG_TRIGGER_MODE,
} tg_mode;

typedef struct reg_8 lifmd_lvds2mipi_1_reg;

struct lifmd_lvds2mipi_1_readout_mode {
	u8 line_skip;
	u8 v_active_low;
	u8 v_active_high;
	u8 left_trim_unit;
	u8 left_trim_lane;
	u8 h_active_unit_low;
	u8 h_active_unit_high;
	u8 word_count_low;
	u8 word_count_high;
};

struct lifmd_lvds2mipi_1_time_generator_params {
	tg_mode tg_mode;
	u8 sync_logic;
	u8 out_logic;
	u32 xhs_min_active_width;
	u32 xhs_clk_offset;
	u32 frame_delay;
	u32 frame_active_width;
	u32 frame_inactive_width;
	u32 t_tgpd;
	u32 frame_length;
	u32 expanded_exposure;
	u32 *line_time;
	operation_mode is_operation_mode;
	shutter_mode is_shutter_mode;
};

struct lifmd_lvds2mipi_1 {
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	struct list_head entry;
	int sensor_numch;
	struct lifmd_lvds2mipi_1_time_generator_params tg_params;
	struct lifmd_lvds2mipi_1_readout_mode lifmd_lvds2mipi_1_readout_mode;
	unsigned int fw_reset_gpio;
	bool *is_pw_state;
};

static const struct regmap_config lifmd_lvds2mipi_1_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
};

int lifmd_lvds2mipi_1_verify_fw_compatibility(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
		struct v4l2_ctrl *ctrl, struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_set_readout_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
					struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_set_is_operation_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1);
int lifmd_lvds2mipi_1_tg_set_operation_mode(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
						struct v4l2_ctrl *ctrl);
int lifmd_lvds2mipi_1_tg_set_trigger_exposure(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 val, struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_tg_expand_trigger_exposure(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				u32 val, struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_tg_delay_frame(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1, u32 val,
					struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_tg_set_line_width(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
					struct camera_common_data *s_data);
int lifmd_lvds2mipi_1_tg_set_frame_width(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
								u32 val);
int lifmd_lvds2mipi_1_tg_start(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1);
int lifmd_lvds2mipi_1_tg_stop(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1);
int lifmd_lvds2mipi_1_start(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1);
int lifmd_lvds2mipi_1_stop(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1);
int lifmd_lvds2mipi_1_fw_reset(struct lifmd_lvds2mipi_1 *lifmd_lvds2mipi_1,
				struct camera_common_data *s_data, int val);

#endif
