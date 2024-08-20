/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx662_mode_tbls.h - imx662 sensor mode tables
 */

#ifndef __fr_IMX577_TABLES__
#define __fr_IMX577_TABLES__


#define MASTER_SLAVE_SEL			0x3041
#define MODE_SEL				0x0100
#define DT_PEDESTAL_HIGH			0x0008
#define DT_PEDESTAL_LOW				0x0009
#define MANUAL_DATA_PEDESTAL_VALUE_HIGH		0x3032
#define MANUAL_DATA_PEDESTAL_VALUE_LOW		0x3033
#define MANUAL_DATA_PEDESTAL_EN			0x3030
#define IMG_ORIENTATION_V_H			0x0101
#define SW_RESET				0x0103
#define CSI_DT_FMT_H				0x0112
#define CSI_DT_FMT_L				0x0113
#define CSI_LANE_MODE				0x0114
#define DPHY_CTRL				0x0808
#define XVS_IO_CTRL				0x3040
#define EXTOUT_EN				0x4B81
#define EXTOUT_XVS_POL				0x4B83
#define EXCK_FREQ_HIGH				0x0136
#define EXCK_FREQ_LOW				0x0137

#define FRM_LENGTH_LINES_HIGH			0x0340
#define FRM_LENGTH_LINES_LOW			0x0341
#define LINE_LENGTH_PCK_HIGH			0x0342
#define LINE_LENGTH_PCK_LOW			0x0343
#define X_OUT_SIZE_HIGH				0x034C
#define X_OUT_SIZE_LOW				0x034D
#define Y_OUT_SIZE_HIGH				0x034E
#define Y_OUT_SIZE_LOW				0x034F
#define FRM_LENGTH_CTL				0x0350
#define LSC_CALC_MODE				0x3804
#define ADBIT_MODE				0x3F0D
#define MC_MODE					0x3F0B
#define PRSH_LENGTH_LINES_HIGH			0x3F39
#define PRSH_LENGTH_LINES_MID			0x3F3A
#define PRSH_LENGTH_LINES_LOW			0x3F3B
#define COARSE_INTEG_TIME_HIGH			0x0202
#define COARSE_INTEG_TIME_LOW			0x0203
#define CIT_LSHIFT				0x3100
#define IVT_PREPLLCK_DIV			0x0305
#define IVT_PLL_MPY_HIGH			0x0306
#define IVT_PLL_MPY_LOW				0x0307
#define PLL_MULT_DRIV				0x0310
#define IVT_SYCK_DIV				0x0303
#define IVT_PXCK_DIV				0x0301
#define IOP_SYCK_DIV				0x030B
#define IOP_PXCK_DIV				0x0309
#define REQ_LINK_BIT_RATE_MBPS_HH		0x0820
#define REQ_LINK_BIT_RATE_MBPS_HL		0x0821
#define REQ_LINK_BIT_RATE_MBPS_LH		0x0822
#define REQ_LINK_BIT_RATE_MBPS_LL		0x0823

#define X_ADD_STA_HIGH				0x0344
#define X_ADD_STA_LOW				0x0345
#define Y_ADD_STA_HIGH				0x0346
#define Y_ADD_STA_LOW				0x0347
#define X_ADD_END_HIGH				0x0348
#define X_ADD_END_LOW				0x0349
#define Y_ADD_END_HIGH				0x034A
#define Y_ADD_END_LOW				0x034B

#define BINNING_MODE				0x0900
#define BINNING_TYPE_H_V			0x0901
#define BINNING_WEIGHTING			0x0902

#define FLL_LSHIFT				0x3210
#define ANA_GAIN_GLOBAL_HIGH			0x0204
#define ANA_GAIN_GLOBAL_LOW			0x0205

#define DPGA_USE_GLOBAL_GAIN			0x3FF9
#define DIG_GAIN_GR_HIGH			0x020E
#define DIG_GAIN_GR_LOW				0x020F
#define DIG_GAIN_R_HIGH				0x0210
#define DIG_GAIN_R_LOW				0x0211
#define DIG_GAIN_B_HIGH				0x0212
#define DIG_GAIN_B_LOW				0x0213
#define DIG_GAIN_GB_HIGH			0x0214
#define DIG_GAIN_GB_LOW				0x0215

#define SCALE_MODE				0x0401
#define SCALE_M_HIGH				0x0404
#define SCALE_M_LOW				0x0405
#define DIG_CROP_IMAGE_WIDTH_HIGH		0x040C
#define DIG_CROP_IMAGE_WIDTH_LOW		0x040D
#define DIG_CROP_IMAGE_HEIGHT_HIGH		0x040E
#define DIG_CROP_IMAGE_HEIGHT_LOW		0x040F
#define GRP_PARAM_HOLD				0x0104

#define FINE_INTEG_TIME_HIGH			0x0200
#define FINE_INTEG_TIME_LOW			0x0201

#define TP_MODE					0x0601
#define SLAVE_ADD_EN_2ND			0x3010
#define SLAVE_ADD_ACKEN_2ND			0x3011
#define INTERNAL_XVS_OFFSET_LINE_HIGH		0x3DA8
#define INTERNAL_XVS_OFFSET_LINE_LOW		0x3DA9
#define INTERNAL_XVS_OFFSET_VTPXCK_HIGH		0x3DAA
#define INTERNAL_XVS_OFFSET_VTPXCK_LOW		0x3DAB

/*Resolutions of implemented frame modes */
#define IMX577_DEFAULT_WIDTH			4056
#define IMX577_DEFAULT_HEIGHT			3040

#define IMX577_CROP_3840x2160_WIDTH		3840
#define IMX577_CROP_3840x2160_HEIGHT		2160

#define IMX577_MODE_SCALE_HV3_WIDTH		1280
#define IMX577_MODE_SCALE_HV3_HEIGHT		720

#define IMX577_MODE_CROP_BINNING_H2V2_WIDTH	1920
#define IMX577_MODE_CROP_BINNING_H2V2_HEIGHT	1080

/* Special values for the write table function */
#define IMX577_TABLE_WAIT_MS			0
#define IMX577_TABLE_END			1
#define IMX577_WAIT_MS				10

#define IMX577_MIN_FRAME_LENGTH_DELTA		52

typedef struct reg_8 imx577_reg;

/*Tables for the write table function */
static const imx577_reg imx577_start[] = {
	{MODE_SEL,		0x01},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x00}
};


static const imx577_reg imx577_stop[] = {
	{MODE_SEL,		0x00},
	{SCALE_MODE,		0x00},
	{BINNING_MODE,		0x00},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x00}
};

static const imx577_reg imx577_8bit_mode[] = {
	{MODE_SEL,		0x00},

	{ADBIT_MODE,		0x00},
	{CSI_DT_FMT_L,		0x08},
	{CSI_DT_FMT_H,		0x08},
	{IOP_PXCK_DIV,		0x08},

	{LINE_LENGTH_PCK_HIGH,	0x11},
	{LINE_LENGTH_PCK_LOW,	0xA0},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x00}
};

static const imx577_reg imx577_10bit_mode[] = {
	{MODE_SEL,		0x00},

	{ADBIT_MODE,		0x00},
	{CSI_DT_FMT_L,		0x0A},
	{CSI_DT_FMT_H,		0x0A},
	{IOP_PXCK_DIV,		0x0A},

	{LINE_LENGTH_PCK_HIGH,	0x11},
	{LINE_LENGTH_PCK_LOW,	0xA0},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x00}
};

static const imx577_reg imx577_12bit_mode[] = {
	{MODE_SEL,		0x00},

	{ADBIT_MODE,		0x01},
	{CSI_DT_FMT_L,		0x0C},
	{CSI_DT_FMT_H,		0x0C},
	{IOP_PXCK_DIV,		0x0C},

	{LINE_LENGTH_PCK_HIGH,	0x18},
	{LINE_LENGTH_PCK_LOW,	0x50},

	{0x3F4B,		0x85},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x00}
};

static const imx577_reg imx577_init_settings[] = {
	{IVT_PREPLLCK_DIV,		0x04},
	{IOP_SYCK_DIV,			0x01},

	{FRM_LENGTH_CTL,		0x01},

	{PLL_MULT_DRIV,			0x00},

	{DPHY_CTRL,			0x00},

	{MANUAL_DATA_PEDESTAL_EN,	0x01},
	{DPGA_USE_GLOBAL_GAIN,		0x01},

	{CSI_LANE_MODE,			0x03},

	{MASTER_SLAVE_SEL,		0x01},

	{IMX577_TABLE_WAIT_MS,		IMX577_WAIT_MS},
	{IMX577_TABLE_END,		0x0000}
};

static const imx577_reg imx577_global_settings[] = {
	{0x38A8,		0x1F},
	{0x38A9,		0xFF},
	{0x38AA,		0x1F},
	{0x38AB,		0xFF},
	{0x55D4,		0x00},
	{0x55D5,		0x00},
	{0x55D6,		0x07},
	{0x55D7,		0xFF},
	{0x55E8,		0x07},
	{0x55E9,		0xFF},
	{0x55EA,		0x00},
	{0x55EB,		0x00},
	{0x575C,		0x07},
	{0x575D,		0xFF},
	{0x575E,		0x00},
	{0x575F,		0x00},
	{0x5764,		0x00},
	{0x5765,		0x00},
	{0x5766,		0x07},
	{0x5767,		0xFF},
	{0x5974,		0x04},
	{0x5975,		0x01},
	{0x5F10,		0x09},
	{0x5F11,		0x92},
	{0x5F12,		0x32},
	{0x5F13,		0x72},
	{0x5F14,		0x16},
	{0x5F15,		0xBA},
	{0x5F17,		0x13},
	{0x5F18,		0x24},
	{0x5F19,		0x60},
	{0x5F1A,		0xE3},
	{0x5F1B,		0xAD},
	{0x5F1C,		0x74},
	{0x5F2D,		0x25},
	{0x5F5C,		0xD0},
	{0x6A22,		0x00},
	{0x6A23,		0x1D},
	{0x7BA8,		0x00},
	{0x7BA9,		0x00},
	{0x886B,		0x00},
	{0x9002,		0x0A},
	{0x9004,		0x1A},
	{0x9214,		0x93},
	{0x9215,		0x69},
	{0x9216,		0x93},
	{0x9217,		0x6B},
	{0x9218,		0x93},
	{0x9219,		0x6D},
	{0x921A,		0x57},
	{0x921B,		0x58},
	{0x921C,		0x57},
	{0x921D,		0x59},
	{0x921E,		0x57},
	{0x921F,		0x5A},
	{0x9220,		0x57},
	{0x9221,		0x5B},
	{0x9222,		0x93},
	{0x9223,		0x02},
	{0x9224,		0x93},
	{0x9225,		0x03},
	{0x9226,		0x93},
	{0x9227,		0x04},
	{0x9228,		0x93},
	{0x9229,		0x05},
	{0x922A,		0x98},
	{0x922B,		0x21},
	{0x922C,		0xB2},
	{0x922D,		0xDB},
	{0x922E,		0xB2},
	{0x922F,		0xDC},
	{0x9230,		0xB2},
	{0x9231,		0xDD},
	{0x9232,		0xB2},
	{0x9233,		0xE1},
	{0x9234,		0xB2},
	{0x9235,		0xE2},
	{0x9236,		0xB2},
	{0x9237,		0xE3},
	{0x9238,		0xB7},
	{0x9239,		0xB9},
	{0x923A,		0xB7},
	{0x923B,		0xBB},
	{0x923C,		0xB7},
	{0x923D,		0xBC},
	{0x923E,		0xB7},
	{0x923F,		0xC5},
	{0x9240,		0xB7},
	{0x9241,		0xC7},
	{0x9242,		0xB7},
	{0x9243,		0xC9},
	{0x9244,		0x98},
	{0x9245,		0x56},
	{0x9246,		0x98},
	{0x9247,		0x55},
	{0x9380,		0x00},
	{0x9381,		0x62},
	{0x9382,		0x00},
	{0x9383,		0x56},
	{0x9384,		0x00},
	{0x9385,		0x52},
	{0x9388,		0x00},
	{0x9389,		0x55},
	{0x938A,		0x00},
	{0x938B,		0x55},
	{0x938C,		0x00},
	{0x938D,		0x41},
	{0x5078,		0x01},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x0000}
};

static const imx577_reg imx577_imageQuality_settings[] = {
	{0x9827,		0x20},
	{0x9830,		0x0A},
	{0x9833,		0x0A},
	{0x9834,		0x32},
	{0x9837,		0x22},
	{0x983C,		0x04},
	{0x983F,		0x0A},
	{0x994F,		0x00},
	{0x9A48,		0x06},
	{0x9A49,		0x06},
	{0x9A4A,		0x06},
	{0x9A4B,		0x06},
	{0x9A4E,		0x03},
	{0x9A4F,		0x03},
	{0x9A54,		0x03},
	{0x9A66,		0x03},
	{0x9A67,		0x03},
	{0xA2C9,		0x02},
	{0xA2CB,		0x02},
	{0xA2CD,		0x02},
	{0xB249,		0x3F},
	{0xB24F,		0x3F},
	{0xB290,		0x3F},
	{0xB293,		0x3F},
	{0xB296,		0x3F},
	{0xB299,		0x3F},
	{0xB2A2,		0x3F},
	{0xB2A8,		0x3F},
	{0xB2A9,		0x0D},
	{0xB2AA,		0x0D},
	{0xB2AB,		0x3F},
	{0xB2BA,		0x2F},
	{0xB2BB,		0x2F},
	{0xB2BC,		0x2F},
	{0xB2BD,		0x10},
	{0xB2C0,		0x3F},
	{0xB2C3,		0x3F},
	{0xB2D2,		0x3F},
	{0xB2DE,		0x20},
	{0xB2DF,		0x20},
	{0xB2E0,		0x20},
	{0xB2EA,		0x3F},
	{0xB2ED,		0x3F},
	{0xB2EE,		0x3F},
	{0xB2EF,		0x3F},
	{0xB2F0,		0x2F},
	{0xB2F1,		0x2F},
	{0xB2F2,		0x2F},
	{0xB2F9,		0x0E},
	{0xB2FA,		0x0E},
	{0xB2FB,		0x0E},
	{0xB759,		0x01},
	{0xB765,		0x3F},
	{0xB76B,		0x3F},
	{0xB7B3,		0x03},
	{0xB7B5,		0x03},
	{0xB7B7,		0x03},
	{0xB7BF,		0x03},
	{0xB7C1,		0x03},
	{0xB7C3,		0x03},
	{0xB7EF,		0x02},
	{0xB7F5,		0x1F},
	{0xB7F7,		0x1F},
	{0xB7F9,		0x1F},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x0000}
};

static const imx577_reg imx577_other_settings[] = {
	{0x3C0A,		0x5A},
	{0x3C0B,		0x55},
	{0x3C0C,		0x28},
	{0x3C0D,		0x07},
	{0x3C0E,		0xFF},
	{0x3C0F,		0x00},
	{0x3C10,		0x00},
	{0x3C11,		0x02},
	{0x3C12,		0x00},
	{0x3C13,		0x03},
	{0x3C14,		0x00},
	{0x3C15,		0x00},
	{0x3C16,		0x0C},
	{0x3C17,		0x0C},
	{0x3C18,		0x0C},
	{0x3C19,		0x0A},
	{0x3C1A,		0x0A},
	{0x3C1B,		0x0A},
	{0x3C1C,		0x00},
	{0x3C1D,		0x00},
	{0x3C1E,		0x00},
	{0x3C1F,		0x00},
	{0x3C20,		0x00},
	{0x3C21,		0x00},
	{0x3C22,		0x3F},
	{0x3C23,		0x0A},
	{0x3E35,		0x01},
	{0x3F4A,		0x03},
	{0x3F4B,		0xBF},
	{0x3F26,		0x00},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x0000}
};

static const imx577_reg imx577_other_settings_binning[] = {
	{0x3C0A,		0x73},
	{0x3C0B,		0x64},
	{0x3C0C,		0x5F},
	{0x3C0D,		0x00},
	{0x3C0E,		0x00},
	{0x3C0F,		0x00},
	{0x3C10,		0xA4},
	{0x3C11,		0x02},
	{0x3C12,		0x00},
	{0x3C13,		0x03},
	{0x3C14,		0x80},
	{0x3C15,		0x04},
	{0x3C16,		0x15},
	{0x3C17,		0x15},
	{0x3C18,		0x15},
	{0x3C19,		0x15},
	{0x3C1A,		0x15},
	{0x3C1B,		0x15},
	{0x3C1C,		0x06},
	{0x3C1D,		0x06},
	{0x3C1E,		0x06},
	{0x3C1F,		0x06},
	{0x3C20,		0x06},
	{0x3C21,		0x06},
	{0x3C22,		0x3F},
	{0x3C23,		0x0A},
	{0x3E35,		0x01},
	{0x3F4A,		0x01},
	{0x3F4B,		0x7F},
	{0x3F26,		0x00},

	{IMX577_TABLE_WAIT_MS,	IMX577_WAIT_MS},
	{IMX577_TABLE_END,	0x0000}
};

static const imx577_reg mode_4056x3040[] = {
	{FRM_LENGTH_LINES_HIGH,		(IMX577_DEFAULT_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) >> 8},
	{FRM_LENGTH_LINES_LOW,		(IMX577_DEFAULT_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) & 0xFF},
	{SCALE_MODE,			0x00},

	{X_ADD_STA_HIGH,		0 >> 8},
	{X_ADD_STA_LOW,			0 & 0xFF},
	{Y_ADD_STA_HIGH,		0 >> 8},
	{Y_ADD_STA_LOW,			0 & 0xFF},
	{X_ADD_END_HIGH,		(IMX577_DEFAULT_WIDTH - 1) >> 8},
	{X_ADD_END_LOW,			(IMX577_DEFAULT_WIDTH - 1) & 0xFF},
	{Y_ADD_END_HIGH,		(IMX577_DEFAULT_HEIGHT - 1) >> 8},
	{Y_ADD_END_LOW,			(IMX577_DEFAULT_HEIGHT - 1) & 0xFF},

	{DIG_CROP_IMAGE_WIDTH_HIGH,	IMX577_DEFAULT_WIDTH >> 8},
	{DIG_CROP_IMAGE_WIDTH_LOW,	IMX577_DEFAULT_WIDTH & 0xFF},
	{DIG_CROP_IMAGE_HEIGHT_HIGH,	IMX577_DEFAULT_HEIGHT >> 8},
	{DIG_CROP_IMAGE_HEIGHT_LOW,	IMX577_DEFAULT_HEIGHT & 0xFF},

	{X_OUT_SIZE_HIGH,		IMX577_DEFAULT_WIDTH >> 8},
	{X_OUT_SIZE_LOW,		IMX577_DEFAULT_WIDTH & 0xFF},

	{Y_OUT_SIZE_HIGH,		IMX577_DEFAULT_HEIGHT >> 8},
	{Y_OUT_SIZE_LOW,		IMX577_DEFAULT_HEIGHT & 0xFF},

	{IMX577_TABLE_WAIT_MS,		IMX577_WAIT_MS},
	{IMX577_TABLE_END,		0x0000}
};

static const imx577_reg mode_3840x2160[] = {
	{FRM_LENGTH_LINES_HIGH,		(IMX577_CROP_3840x2160_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) >> 8},
	{FRM_LENGTH_LINES_LOW,		(IMX577_CROP_3840x2160_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) & 0xFF},
	{SCALE_MODE,			0x00},

	{X_ADD_STA_HIGH,		108 >> 8},
	{X_ADD_STA_LOW,			108 & 0xFF},
	{Y_ADD_STA_HIGH,		440 >> 8},
	{Y_ADD_STA_LOW,			440 & 0xFF},
	{X_ADD_END_HIGH,		(3948 - 1) >> 8},
	{X_ADD_END_LOW,			(3948 - 1) & 0xFF},
	{Y_ADD_END_HIGH,		(2600 - 1) >> 8},
	{Y_ADD_END_LOW,			(2600 - 1) & 0xFF},

	{BINNING_MODE,			0x00},

	{DIG_CROP_IMAGE_WIDTH_HIGH,	IMX577_CROP_3840x2160_WIDTH >> 8},
	{DIG_CROP_IMAGE_WIDTH_LOW,	IMX577_CROP_3840x2160_WIDTH & 0xFF},
	{DIG_CROP_IMAGE_HEIGHT_HIGH,	IMX577_CROP_3840x2160_HEIGHT >> 8},
	{DIG_CROP_IMAGE_HEIGHT_LOW,	IMX577_CROP_3840x2160_HEIGHT & 0xFF},

	{X_OUT_SIZE_HIGH,		IMX577_CROP_3840x2160_WIDTH >> 8},
	{X_OUT_SIZE_LOW,		IMX577_CROP_3840x2160_WIDTH & 0xFF},

	{Y_OUT_SIZE_HIGH,		IMX577_CROP_3840x2160_HEIGHT >> 8},
	{Y_OUT_SIZE_LOW,		IMX577_CROP_3840x2160_HEIGHT & 0xFF},

	{IMX577_TABLE_WAIT_MS,		IMX577_WAIT_MS},
	{IMX577_TABLE_END,		0x0000}
};

static const imx577_reg mode_scale_hv3[] = {
	{FRM_LENGTH_LINES_HIGH,		(IMX577_DEFAULT_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) >> 8},
	{FRM_LENGTH_LINES_LOW,		(IMX577_DEFAULT_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) & 0xFF},

	{SCALE_MODE,			0x02},
	{SCALE_M_HIGH,			48 >> 8},
	{SCALE_M_LOW,			48 & 0xFF},

	{X_ADD_STA_HIGH,		108 >> 8},
	{X_ADD_STA_LOW,			108 & 0xFF},
	{Y_ADD_STA_HIGH,		440 >> 8},
	{Y_ADD_STA_LOW,			440 & 0xFF},
	{X_ADD_END_HIGH,		(108 + IMX577_MODE_SCALE_HV3_WIDTH*3 - 1) >> 8},
	{X_ADD_END_LOW,			(108 + IMX577_MODE_SCALE_HV3_WIDTH*3 - 1) & 0xFF},
	{Y_ADD_END_HIGH,		(440 + IMX577_MODE_SCALE_HV3_HEIGHT*3 - 1) >> 8},
	{Y_ADD_END_LOW,			(440 + IMX577_MODE_SCALE_HV3_HEIGHT*3 - 1) & 0xFF},
	{BINNING_MODE,			0x00},
	{DIG_CROP_IMAGE_WIDTH_HIGH,	IMX577_MODE_SCALE_HV3_WIDTH*3 >> 8},
	{DIG_CROP_IMAGE_WIDTH_LOW,	IMX577_MODE_SCALE_HV3_WIDTH*3 & 0xFF},
	{DIG_CROP_IMAGE_HEIGHT_HIGH,	IMX577_MODE_SCALE_HV3_HEIGHT*3 >> 8},
	{DIG_CROP_IMAGE_HEIGHT_LOW,	IMX577_MODE_SCALE_HV3_HEIGHT*3 & 0xFF},

	{X_OUT_SIZE_HIGH,		IMX577_MODE_SCALE_HV3_WIDTH >> 8},
	{X_OUT_SIZE_LOW,		IMX577_MODE_SCALE_HV3_WIDTH & 0xFF},

	{Y_OUT_SIZE_HIGH,		IMX577_MODE_SCALE_HV3_HEIGHT >> 8},
	{Y_OUT_SIZE_LOW,		IMX577_MODE_SCALE_HV3_HEIGHT & 0xFF},

	{IMX577_TABLE_WAIT_MS,		IMX577_WAIT_MS},
	{IMX577_TABLE_END,		0x0000}
};

static const imx577_reg mode_crop_binning_h2v2[] = {
	{FRM_LENGTH_LINES_HIGH,		(IMX577_MODE_CROP_BINNING_H2V2_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) >> 8},
	{FRM_LENGTH_LINES_LOW,		(IMX577_MODE_CROP_BINNING_H2V2_HEIGHT +
					IMX577_MIN_FRAME_LENGTH_DELTA) & 0xFF},
	{SCALE_MODE,			0x00},

	{X_ADD_STA_HIGH,		108 >> 8},
	{X_ADD_STA_LOW,			108 & 0xFF},
	{Y_ADD_STA_HIGH,		440 >> 8},
	{Y_ADD_STA_LOW,			440 & 0xFF},
	{X_ADD_END_HIGH,		(108 + IMX577_MODE_CROP_BINNING_H2V2_WIDTH*2 - 1) >> 8},
	{X_ADD_END_LOW,			(108 + IMX577_MODE_CROP_BINNING_H2V2_WIDTH*2 - 1) & 0xFF},
	{Y_ADD_END_HIGH,		(440 + IMX577_MODE_CROP_BINNING_H2V2_HEIGHT*2 - 1) >> 8},
	{Y_ADD_END_LOW,			(440 + IMX577_MODE_CROP_BINNING_H2V2_HEIGHT*2 - 1) & 0xFF},

	{BINNING_MODE,			0x01},
	{BINNING_TYPE_H_V,		0x22},
	{LINE_LENGTH_PCK_HIGH,		2976 >> 8},
	{LINE_LENGTH_PCK_LOW,		2976 & 0xFF},

	{DIG_CROP_IMAGE_WIDTH_HIGH,	IMX577_MODE_CROP_BINNING_H2V2_WIDTH >> 8},
	{DIG_CROP_IMAGE_WIDTH_LOW,	IMX577_MODE_CROP_BINNING_H2V2_WIDTH & 0xFF},
	{DIG_CROP_IMAGE_HEIGHT_HIGH,	IMX577_MODE_CROP_BINNING_H2V2_HEIGHT >> 8},
	{DIG_CROP_IMAGE_HEIGHT_LOW,	IMX577_MODE_CROP_BINNING_H2V2_HEIGHT & 0xFF},

	{X_OUT_SIZE_HIGH,		IMX577_MODE_CROP_BINNING_H2V2_WIDTH >> 8},
	{X_OUT_SIZE_LOW,		IMX577_MODE_CROP_BINNING_H2V2_WIDTH & 0xFF},

	{Y_OUT_SIZE_HIGH,		IMX577_MODE_CROP_BINNING_H2V2_HEIGHT >> 8},
	{Y_OUT_SIZE_LOW,		IMX577_MODE_CROP_BINNING_H2V2_HEIGHT & 0xFF},

	{IMX577_TABLE_WAIT_MS,		IMX577_WAIT_MS},
	{IMX577_TABLE_END,		0x0000}
};

/* Enum of available frame modes */
enum {
	IMX577_MODE_4056x3040,
	IMX577_MODE_CROP_3840x2160,
	IMX577_MODE_SCALE_HV3,
	IMX577_MODE_CROP_BINNING_H2V2,

	IMX577_8BIT_MODE,
	IMX577_10BIT_MODE,
	IMX577_12BIT_MODE,

	IMX577_INIT_SETTINGS,
	IMX577_GLOBAL_SETTINGS,
	IMX577_IMAGEQUALITY_SETTINGS,
	IMX577_OTHER_SETTINGS,
	IMX577_OTHER_SETTINGS_BINNING,

	IMX577_MODE_START_STREAM,
	IMX577_MODE_STOP_STREAM,
};

/* Connecting frame modes to mode tables */
static const imx577_reg *mode_table[] = {
	[IMX577_MODE_4056x3040]		= mode_4056x3040,
	[IMX577_MODE_CROP_3840x2160]	= mode_3840x2160,

	[IMX577_MODE_SCALE_HV3]		= mode_scale_hv3,

	[IMX577_MODE_CROP_BINNING_H2V2]	= mode_crop_binning_h2v2,

	[IMX577_8BIT_MODE]		= imx577_8bit_mode,
	[IMX577_10BIT_MODE]		= imx577_10bit_mode,
	[IMX577_12BIT_MODE]		= imx577_12bit_mode,

	[IMX577_INIT_SETTINGS]		= imx577_init_settings,
	[IMX577_GLOBAL_SETTINGS]	= imx577_global_settings,
	[IMX577_IMAGEQUALITY_SETTINGS]	= imx577_imageQuality_settings,
	[IMX577_OTHER_SETTINGS]		= imx577_other_settings,
	[IMX577_OTHER_SETTINGS_BINNING]	= imx577_other_settings_binning,

	[IMX577_MODE_START_STREAM]	= imx577_start,
	[IMX577_MODE_STOP_STREAM]	= imx577_stop,
};

/* Framerates of available frame modes */
static const int imx577_60fps[] = {
	60,
};
static const int imx577_84fps[] = {
	84,
};
static const int imx577_249fps[] = {
	249,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx577_frmfmt[] = {
	{
		.size = {IMX577_DEFAULT_WIDTH, IMX577_DEFAULT_HEIGHT},
		.framerates = imx577_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX577_MODE_4056x3040
	},
	{
		.size = {IMX577_CROP_3840x2160_WIDTH, IMX577_CROP_3840x2160_HEIGHT},
		.framerates = imx577_84fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX577_MODE_CROP_3840x2160
	},
	{
		.size = {IMX577_MODE_SCALE_HV3_WIDTH, IMX577_MODE_SCALE_HV3_HEIGHT},
		.framerates = imx577_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX577_MODE_SCALE_HV3
	},
	{
		.size = {IMX577_MODE_CROP_BINNING_H2V2_WIDTH, IMX577_MODE_CROP_BINNING_H2V2_HEIGHT},
		.framerates = imx577_249fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX577_MODE_CROP_BINNING_H2V2
	},
};

#endif
