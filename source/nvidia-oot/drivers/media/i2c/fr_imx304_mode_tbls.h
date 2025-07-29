/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx304_mode_tbls.h - imx304 sensor mode tables
 */

#ifndef __fr_IMX304_TABLES__
#define __fr_IMX304_TABLES__


#define STANDBY			0x3000
#define STBLVDS			0x3005
#define REGHOLD			0x3008
#define XMSTA			0x300A
#define TRIGEN			0x300B

#define HMODE_WINMODE		0x300D
#define HREVERSE_VREVERSE	0x300E

#define VMAX_LOW		0x3010
#define VMAX_MID		0x3011
#define VMAX_HIGH		0x3012

#define HMAX_LOW		0x3014
#define HMAX_HIGH		0x3015
#define OPORTSEL		0x301C
#define TOUTSEL			0x3026
#define TRIG_TOUT_SEL		0x3029
#define SYNCSEL			0x3036

#define PULSE1_OPTIONS		0x306D
#define PULSE1_UP_LOW		0x3070
#define PULSE1_UP_MID		0x3071
#define PULSE1_UP_HIGH		0x3072
#define PULSE1_DN_LOW		0x3074
#define PULSE1_DN_MID		0x3075
#define PULSE1_DN_HIGH		0x3076

#define PULSE2_OPTIONS		0x3079
#define PULSE2_UP_LOW		0x307C
#define PULSE2_UP_MID		0x307D
#define PULSE2_UP_HIGH		0x307E
#define PULSE2_DN_LOW		0x3080
#define PULSE2_DN_MID		0x3081
#define PULSE2_DN_HIGH		0x3082

#define INCKSEL0		0x3089
#define INCKSEL1		0x308A
#define INCKSEL2		0x308B
#define INCKSEL3		0x308C

#define SHS1_LOW		0x308D
#define SHS1_MID		0x308E
#define SHS1_HIGH		0x308F

#define VINT_EN			0x30AA
#define LOWLAGTRG		0x30AE

#define GAIN_LOW		0x3204
#define GAIN_HIGH		0x3205

#define GAINDLY			0x3212
#define BLKLEVEL_LOW		0x3254
#define BLKLEVEL_HIGH		0x3255

#define FID0_ROI		0x3300
#define FID0_ROIPH1_LOW		0x3310
#define FID0_ROIPH1_HIGH	0x3311
#define FID0_ROIPV1_LOW		0x3312
#define FID0_ROIPV1_HIGH	0x3313
#define FID0_ROIWH1_LOW		0x3314
#define FID0_ROIWH1_HIGH	0x3315
#define FID0_ROIWV1_LOW		0x3316
#define FID0_ROIWV1_HIGH	0x3317

/* Resolutions of implemented frame modes */
#define IMX304_DEFAULT_WIDTH		4112
#define IMX304_DEFAULT_HEIGHT		3008

#define IMX304_MODE_4096x2160_WIDTH	4096
#define IMX304_MODE_4096x2160_HEIGHT	2160

#define IMX304_MODE_3840x2160_WIDTH	3840
#define IMX304_MODE_3840x2160_HEIGHT	2160

/* Special values for the write table function */
#define IMX304_TABLE_WAIT_MS		0
#define IMX304_TABLE_END		1
#define IMX304_WAIT_MS			10

#define IMX304_MIN_FRAME_LENGTH_DELTA	34

#define IMX304_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX304_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx304_reg;
typedef struct reg_8 crosslink_reg;

/* Tables for the write table function */
static const imx304_reg imx304_stop[] = {

	{STANDBY, 0x01},
	{IMX304_TABLE_WAIT_MS, IMX304_WAIT_MS},
	{XMSTA, 0x01},

	{IMX304_TABLE_WAIT_MS, 30},
	{IMX304_TABLE_END, 0x00}
};

static const imx304_reg imx304_init_settings[] = {

	{SYNCSEL,		0xC0},
	{0x3001,		0xD0},
	{0x3002,		0xAA},
	{0x300C,		0x01},

	{0x3016,		0x01},
	{0x3018,		0x01},
	{0x3079,		0x08},

	{0x3090,		0x0A},
	{0x3094,		0x0A},
	{0x3098,		0x0A},
	{0x309E,		0x08},
	{0x30A0,		0x06},
	{0x30AF,		0x0C},

	{0x3165,		0x40},
	{0x3166,		0x00},

	{0x3226,		0x03},

	{0x3518,		0x78},
	{0x3519,		0x0C},

	{0x3D70,		0x1E},
	{0x3D71,		0x00},
	{0x3D72,		0x67},
	{0x3D73,		0x01},
	{0x3D74,		0x1E},
	{0x3D75,		0x00},
	{0x3D76,		0x67},
	{0x3D77,		0x01},

	{0x4002,		0x20},
	{0x4003,		0x55},
	{0x4017,		0x03},
	{0x401E,		0x03},
	{0x403D,		0x24},
	{0x4040,		0x09},
	{0x4041,		0x6A},
	{0x404A,		0xC0},
	{0x4056,		0x18},
	{0x4094,		0x06},

	{GAINDLY,		0x09},

	{IMX304_TABLE_WAIT_MS, IMX304_WAIT_MS},
	{IMX304_TABLE_END, 0x0000}
};

static const imx304_reg mode_4112x3008[] = {

	{STBLVDS,		0x10},
	{HMODE_WINMODE,		0x00},

	{VMAX_HIGH,		0x00},
	{VMAX_MID,		0x0B},
	{VMAX_LOW,		0xE2},

	{HMAX_HIGH,		0x04},
	{HMAX_LOW,		0x11},

	{OPORTSEL,		0x10},
	{FID0_ROI,		0x00},

	{INCKSEL0,		0x10},
	{INCKSEL1,		0x02},
	{INCKSEL2,		0x10},
	{INCKSEL3,		0x02},

	{IMX304_TABLE_WAIT_MS, IMX304_WAIT_MS},
	{IMX304_TABLE_END, 0x0000}
};

static const imx304_reg mode_ROI_4096x2160[] = {

	{STBLVDS,		0x10},
	{HMODE_WINMODE,		0x00},

	{VMAX_HIGH,		0x00},
	{VMAX_MID,		0x08},
	{VMAX_LOW,		0x92},

	{HMAX_HIGH,		0x04},
	{HMAX_LOW,		0x11},

	{OPORTSEL,		0x10},

	{INCKSEL0,		0x10},
	{INCKSEL1,		0x02},
	{INCKSEL2,		0x10},
	{INCKSEL3,		0x02},

	{FID0_ROI,		0x03},

	{FID0_ROIPH1_LOW,	IMX304_TO_LOW_BYTE(8)},
	{FID0_ROIPH1_HIGH,	IMX304_TO_MID_BYTE(8)},

	{FID0_ROIPV1_LOW,	IMX304_TO_LOW_BYTE(424)},
	{FID0_ROIPV1_HIGH,	IMX304_TO_MID_BYTE(424)},

	{FID0_ROIWH1_LOW,	IMX304_TO_LOW_BYTE(IMX304_MODE_4096x2160_WIDTH)},
	{FID0_ROIWH1_HIGH,	IMX304_TO_MID_BYTE(IMX304_MODE_4096x2160_WIDTH)},

	{FID0_ROIWV1_LOW,	IMX304_TO_LOW_BYTE(IMX304_MODE_4096x2160_HEIGHT)},
	{FID0_ROIWV1_HIGH,	IMX304_TO_MID_BYTE(IMX304_MODE_4096x2160_HEIGHT)},

	{IMX304_TABLE_WAIT_MS, IMX304_WAIT_MS},
	{IMX304_TABLE_END, 0x0000}
};

static const imx304_reg mode_ROI_3840x2160[] = {

	{STBLVDS,		0x10},
	{HMODE_WINMODE,		0x00},

	{VMAX_HIGH,		0x00},
	{VMAX_MID,		0x08},
	{VMAX_LOW,		0x92},

	{HMAX_HIGH,		0x04},
	{HMAX_LOW,		0x11},

	{OPORTSEL,		0x10},

	{INCKSEL0,		0x10},
	{INCKSEL1,		0x02},
	{INCKSEL2,		0x10},
	{INCKSEL3,		0x02},

	{FID0_ROI,		0x03},

	{FID0_ROIPH1_LOW,	IMX304_TO_LOW_BYTE(136)},
	{FID0_ROIPH1_HIGH,	IMX304_TO_MID_BYTE(136)},

	{FID0_ROIPV1_LOW,	IMX304_TO_LOW_BYTE(424)},
	{FID0_ROIPV1_HIGH,	IMX304_TO_MID_BYTE(424)},

	{FID0_ROIWH1_LOW,	IMX304_TO_LOW_BYTE(IMX304_MODE_3840x2160_WIDTH)},
	{FID0_ROIWH1_HIGH,	IMX304_TO_MID_BYTE(IMX304_MODE_3840x2160_WIDTH)},

	{FID0_ROIWV1_LOW,	IMX304_TO_LOW_BYTE(IMX304_MODE_3840x2160_HEIGHT)},
	{FID0_ROIWV1_HIGH,	IMX304_TO_MID_BYTE(IMX304_MODE_3840x2160_HEIGHT)},

	{IMX304_TABLE_WAIT_MS, IMX304_WAIT_MS},
	{IMX304_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {

	IMX304_MODE_4112x3008,
	IMX304_MODE_ROI_4096x2160,
	IMX304_MODE_ROI_3840x2160,

	IMX304_INIT_SETTINGS,
	IMX304_MODE_STOP_STREAM,
};

/* Connecting frame modes to mode tables */
static const imx304_reg *mode_table[] = {

	[IMX304_MODE_4112x3008] = mode_4112x3008,
	[IMX304_MODE_ROI_4096x2160] = mode_ROI_4096x2160,
	[IMX304_MODE_ROI_3840x2160] = mode_ROI_3840x2160,

	[IMX304_INIT_SETTINGS] = imx304_init_settings,

	[IMX304_MODE_STOP_STREAM] = imx304_stop,
};

/* Framerates of available frame modes */
static const int imx304_23fps[] = {
	23,
};

static const int imx304_30fps[] = {
	30,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx304_frmfmt[] = {
	{
		.size = {IMX304_DEFAULT_WIDTH, IMX304_DEFAULT_HEIGHT},
		.framerates = imx304_23fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX304_MODE_4112x3008
	},
	{
		.size = {IMX304_MODE_4096x2160_WIDTH, IMX304_MODE_4096x2160_HEIGHT},
		.framerates = imx304_30fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX304_MODE_ROI_4096x2160
	},
	{
		.size = {IMX304_MODE_3840x2160_WIDTH, IMX304_MODE_3840x2160_HEIGHT},
		.framerates = imx304_30fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX304_MODE_ROI_3840x2160
	},
};

#endif
