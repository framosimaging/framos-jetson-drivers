/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * lifcl_var2fixed_1 driver for Lifcl FPGA
 */

#ifndef __fr_lifcl_var2fixed_1__
#define __fr_lifcl_var2fixed_1__

/* lifcl FPGA firmware data */

#define LIFCL_VAR2FIXED_1_FW_NAME_BASE			0x0010
#define LIFCL_VAR2FIXED_1_FW_VERSION_BASE		0x0020

#define LIFCL_VAR2FIXED_1_FW_NAME_REG_LENGTH		16
#define LIFCL_VAR2FIXED_1_FW_VERSION_REG_LENGTH		4

#define LIFCL_VAR2FIXED_1_SW_RST_N_RW			0x00F9

#define LIFCL_VAR2FIXED_1_WIDTH_REG			0x0130
#define LIFCL_VAR2FIXED_1_HEIGHT_REG			0x0132
#define LIFCL_VAR2FIXED_1_VBLANK_REG			0x0134
#define LIFCL_VAR2FIXED_1_HBLANK_REG			0x0138


struct lifcl_var2fixed_1_params {
	u16 width;
	u16 height;
	u32 vblank;
	u32 hblank;
};

struct lifcl_var2fixed_1 {
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	struct list_head entry;
	int sensor_numch;
	struct lifcl_var2fixed_1_params params;
	unsigned int fw_reset_gpio;
};

static const struct regmap_config lifcl_var2fixed_1_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
};

int lifcl_var2fixed_1_verify_fw_compatibility(struct lifcl_var2fixed_1 *lifcl_var2fixed_1, s32 *val);
int lifcl_var2fixed_1_start(struct lifcl_var2fixed_1 *lifcl_var2fixed_1);
int lifcl_var2fixed_1_stop(struct lifcl_var2fixed_1 *lifcl_var2fixed_1);
int lifcl_var2fixed_1_fw_reset(struct lifcl_var2fixed_1 *lifcl_var2fixed_1,
				struct camera_common_data *s_data, int val);

#endif
