/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx675_mode_tbls.h - imx675 sensor mode tables
 */

#ifndef __fr_IMX675_TABLES__
#define __fr_IMX675_TABLES__


#define STANDBY				0x3000
#define REGHOLD				0x3001
#define XMSTA				0x3002

#define INCK_SEL			0x3014
#define DATARATE_SEL			0x3015
#define WINMODE				0x3018
#define ADDMODE				0x301B

#define HVREVERSE			0x3020
#define VREVERSE			0x3021
#define ADBIT				0x3022
#define MDBIT				0x3023

#define VMAX_LOW			0x3028
#define VMAX_MID			0x3029
#define VMAX_HIGH			0x302A
#define HMAX_LOW			0x302C
#define HMAX_HIGH			0x302D

#define FDG_SEL0			0x3030
#define FDG_SEL1			0x3031
#define PIX_HST_LOW			0x303C
#define PIX_HST_HIGH			0x303D
#define PIX_HWIDTH_LOW			0x303E
#define PIX_HWIDTH_HIGH			0x303F
#define LANEMODE			0x3040

#define PIX_VST_LOW			0x3044
#define PIX_VST_HIGH			0x3045
#define PIX_VWIDTH_LOW			0x3046
#define PIX_VWIDTH_HIGH			0x3047

#define GAIN_HG0_LOW			0x304C
#define GAIN_HG0_HIGH			0x304D

#define SHR0_LOW			0x3050
#define SHR0_MID			0x3051
#define SHR0_HIGH			0x3052
#define SHR1_LOW			0x3054
#define SHR1_MID			0x3055
#define SHR1_HIGH			0x3056

#define GAIN_LOW			0x3070
#define GAIN_HIGH			0x3071

#define XHSOUTSEL_XVSOUTSEL		0x30A4
#define XVS_XHS_DRV			0x30A6
#define XVSLNG				0x30CC
#define XHSLNG				0x30CD

#define BLKLEVEL_LOW			0x30DC
#define BLKLEVEL_HIGH			0x30DD

#define TPG_EN_DUOUT			0x30E0
#define TPG_PATSEL_DUOUT		0x30E2
#define TPG_COLORWIDTH			0x30E4
#define TESTCLKEN			0x5300

#define EXTMODE				0x30CE
#define SECOND_SLAVE_ADD		0x300C

/* Special values for the write table function */
#define IMX675_TABLE_WAIT_MS		0
#define IMX675_TABLE_END		1
#define IMX675_WAIT_MS			10

#define IMX675_DEFAULT_WIDTH		2608
#define IMX675_DEFAULT_HEIGHT		1964
#define IMX675_CROP_1920x1080_WIDTH	1920
#define IMX675_CROP_1920x1080_HEIGHT	1080
#define IMX675_MODE_BINNING_H2V2_WIDTH	1304
#define IMX675_MODE_BINNING_H2V2_HEIGHT	982

/**
 * Minimal value of frame length is resolution height + 70
 * Minimal value for scaling modes is full pixel mode height + 70
 *
 * Determined from the default value of FRM_LENGTH_LINES register
 * and empirically confirmed
 */
#define IMX675_MIN_FRAME_LENGTH_DELTA 70

#define IMX675_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX675_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx675_reg;

/**
 * Tables for the write table function
 */

static const imx675_reg imx675_start[] = {
	{STANDBY,		0x00},
	{IMX675_TABLE_WAIT_MS,	30},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x00}
};

static const imx675_reg imx675_stop[] = {
	{XMSTA,			0x01},
	{IMX675_TABLE_WAIT_MS,	30},
	{STANDBY,		0x01},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x00}
};

static const imx675_reg imx675_10bit_mode[] = {
	{ADBIT,			0x00},
	{MDBIT,			0x00},

	{0x3C0A,		0x04},
	{0x3C0B,		0x04},
	{0x3C0C,		0x04},
	{0x3C0D,		0x04},
	{0x3C0E,		0x04},
	{0x3C0F,		0x04},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x00}
};

static const imx675_reg imx675_12bit_mode[] = {
	{ADBIT,			0x01},
	{MDBIT,			0x01},

	{0x3C0A,		0x00},
	{0x3C0B,		0x00},
	{0x3C0C,		0x00},
	{0x3C0D,		0x00},
	{0x3C0E,		0x00},
	{0x3C0F,		0x00},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x00}
};

static const imx675_reg imx675_init_settings[] = {
	{INCK_SEL,		0x01},
	{DATARATE_SEL,		0x04},

	{ADBIT,			0x01},
	{MDBIT,			0x01},
	{VMAX_LOW,		0x08},
	{VMAX_HIGH,		0x08},
	{HMAX_LOW,		0x5A},
	{HMAX_HIGH,		0x02},

	{LANEMODE,		0x03},
	{SHR0_MID,		0x04},

	{XVS_XHS_DRV,		0x00},

	{0x30CE,		0x02},
	{0x3148,		0x00},

	{0x3460,		0x22},
	{0x3492,		0x08},
	{0x3B1D,		0x17},
	{0x3B44,		0x3F},
	{0x3B60,		0x03},
	{0x3C03,		0x04},
	{0x3C04,		0x04},
	{0x3C0A,		0x00},
	{0x3C0B,		0x00},
	{0x3C0C,		0x00},
	{0x3C0D,		0x00},
	{0x3C0E,		0x00},
	{0x3C0F,		0x00},
	{0x3C30,		0x73},
	{0x3C3C,		0x20},
	{0x3C7C,		0xB9},
	{0x3C7D,		0x01},
	{0x3C7E,		0xB7},
	{0x3C7F,		0x01},
	{0x3CB0,		0x00},
	{0x3CB2,		0xFF},
	{0x3CB3,		0x03},
	{0x3CB4,		0xFF},
	{0x3CB5,		0x03},
	{0x3CBA,		0xFF},
	{0x3CBB,		0x03},
	{0x3CC0,		0xFF},
	{0x3CC1,		0x03},
	{0x3CC2,		0x00},
	{0x3CC6,		0xFF},
	{0x3CC7,		0x03},
	{0x3CC8,		0xFF},
	{0x3CC9,		0x03},
	{0x3E00,		0x1E},
	{0x3E02,		0x04},
	{0x3E03,		0x00},
	{0x3E20,		0x04},
	{0x3E21,		0x00},
	{0x3E22,		0x1E},
	{0x3E24,		0xBA},
	{0x3E72,		0x85},
	{0x3E76,		0x0C},
	{0x3E77,		0x01},
	{0x3E7A,		0x85},
	{0x3E7E,		0x1F},
	{0x3E82,		0xA6},
	{0x3E86,		0x2D},
	{0x3EE2,		0x33},
	{0x3EE3,		0x03},
	{0x4490,		0x07},
	{0x4494,		0x19},
	{0x4495,		0x00},
	{0x4496,		0xBB},
	{0x4497,		0x00},
	{0x4498,		0x55},
	{0x449A,		0x50},
	{0x449C,		0x50},
	{0x449E,		0x50},
	{0x44A0,		0x3C},
	{0x44A2,		0x19},
	{0x44A4,		0x19},
	{0x44A6,		0x19},
	{0x44A8,		0x4B},
	{0x44AA,		0x4B},
	{0x44AC,		0x4B},
	{0x44AE,		0x4B},
	{0x44B0,		0x3C},
	{0x44B2,		0x19},
	{0x44B4,		0x19},
	{0x44B6,		0x19},
	{0x44B8,		0x4B},
	{0x44BA,		0x4B},
	{0x44BC,		0x4B},
	{0x44BE,		0x4B},
	{0x44C0,		0x3C},
	{0x44C2,		0x19},
	{0x44C4,		0x19},
	{0x44C6,		0x19},
	{0x44C8,		0xF0},
	{0x44CA,		0xEB},
	{0x44CC,		0xEB},
	{0x44CE,		0xE6},
	{0x44D0,		0xE6},
	{0x44D2,		0xBB},
	{0x44D4,		0xBB},
	{0x44D6,		0xBB},
	{0x44D8,		0xE6},
	{0x44DA,		0xE6},
	{0x44DC,		0xE6},
	{0x44DE,		0xE6},
	{0x44E0,		0xE6},
	{0x44E2,		0xBB},
	{0x44E4,		0xBB},
	{0x44E6,		0xBB},
	{0x44E8,		0xE6},
	{0x44EA,		0xE6},
	{0x44EC,		0xE6},
	{0x44EE,		0xE6},
	{0x44F0,		0xE6},
	{0x44F2,		0xBB},
	{0x44F4,		0xBB},
	{0x44F6,		0xBB},
	{0x4538,		0x15},
	{0x4539,		0x15},
	{0x453A,		0x15},
	{0x4544,		0x15},
	{0x4545,		0x15},
	{0x4546,		0x15},
	{0x4550,		0x11},
	{0x4551,		0x11},
	{0x4552,		0x11},
	{0x4553,		0x11},
	{0x4554,		0x11},
	{0x4555,		0x11},
	{0x4556,		0x11},
	{0x4557,		0x11},
	{0x4558,		0x11},
	{0x455C,		0x11},
	{0x455D,		0x11},
	{0x455E,		0x11},
	{0x455F,		0x11},
	{0x4560,		0x11},
	{0x4561,		0x11},
	{0x4562,		0x11},
	{0x4563,		0x11},
	{0x4564,		0x11},
	{0x4569,		0x01},
	{0x456A,		0x01},
	{0x456B,		0x06},
	{0x456C,		0x06},
	{0x456D,		0x06},
	{0x456E,		0x06},
	{0x456F,		0x06},
	{0x4570,		0x06},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

static const imx675_reg mode_2608x1964[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x00},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

static const imx675_reg mode_crop_1920x1080[] = {
	{WINMODE,		0x04},
	{ADDMODE,		0x00},

	{PIX_HST_HIGH,		IMX675_TO_MID_BYTE(344)},
	{PIX_HST_LOW,		IMX675_TO_LOW_BYTE(344)},
	{PIX_HWIDTH_HIGH,	IMX675_TO_MID_BYTE(IMX675_CROP_1920x1080_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX675_TO_LOW_BYTE(IMX675_CROP_1920x1080_WIDTH)},

	{PIX_VST_HIGH,		IMX675_TO_MID_BYTE(444)},
	{PIX_VST_LOW,		IMX675_TO_LOW_BYTE(444)},
	{PIX_VWIDTH_HIGH,	IMX675_TO_MID_BYTE(IMX675_CROP_1920x1080_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX675_TO_LOW_BYTE(IMX675_CROP_1920x1080_HEIGHT)},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

static const imx675_reg mode_h2v2_binning[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x01},

	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{0x3C0A,		0x04},
	{0x3C0B,		0x04},
	{0x3C0C,		0x04},
	{0x3C0D,		0x04},
	{0x3C0E,		0x04},
	{0x3C0F,		0x04},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

static const imx675_reg mode_enable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x0A},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

static const imx675_reg mode_disable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x32},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x02},

	{IMX675_TABLE_WAIT_MS,	IMX675_WAIT_MS},
	{IMX675_TABLE_END,	0x0000}
};

/* Enum of available frame modes */
enum {
	IMX675_MODE_2608x1964,
	IMX675_MODE_CROP_1920x1080,
	IMX675_MODE_H2V2_BINNING,

	IMX675_10BIT_MODE,
	IMX675_12BIT_MODE,

	IMX675_EN_PATTERN_GEN,
	IMX675_DIS_PATTERN_GEN,

	IMX675_INIT_SETTINGS,
	IMX675_MODE_START_STREAM,
	IMX675_MODE_STOP_STREAM,
};

typedef enum {
	IMX675_2376_MBPS,
	IMX675_2079_MBPS,
	IMX675_1782_MBPS,
	IMX675_1440_MBPS,
	IMX675_1188_MBPS,
	IMX675_891_MBPS,
	IMX675_720_MBPS,
	IMX675_594_MBPS,
} data_rate_mode;

/* Connecting frame modes to mode tables */
static const imx675_reg *mode_table[] = {
	[IMX675_MODE_2608x1964]		= mode_2608x1964,
	[IMX675_MODE_CROP_1920x1080]	= mode_crop_1920x1080,
	[IMX675_MODE_H2V2_BINNING]	= mode_h2v2_binning,

	[IMX675_EN_PATTERN_GEN]		= mode_enable_pattern_generator,
	[IMX675_DIS_PATTERN_GEN]	= mode_disable_pattern_generator,

	[IMX675_10BIT_MODE]		= imx675_10bit_mode,
	[IMX675_12BIT_MODE]		= imx675_12bit_mode,

	[IMX675_INIT_SETTINGS]		= imx675_init_settings,

	[IMX675_MODE_START_STREAM]	= imx675_start,
	[IMX675_MODE_STOP_STREAM]	= imx675_stop,
};

/* Framerates of available frame modes */
static const int imx675_60fps[] = {
	60,
};

static const int imx675_80fps[] = {
	80,
};

static const int imx675_107fps[] = {
	107,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx675_frmfmt[] = {
	{
		.size = {IMX675_DEFAULT_WIDTH, IMX675_DEFAULT_HEIGHT},
		.framerates = imx675_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX675_MODE_2608x1964
	},
	{
		.size = {IMX675_CROP_1920x1080_WIDTH, IMX675_CROP_1920x1080_HEIGHT},
		.framerates = imx675_107fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX675_MODE_CROP_1920x1080
	},
	{
		.size = {IMX675_MODE_BINNING_H2V2_WIDTH, IMX675_MODE_BINNING_H2V2_HEIGHT},
		.framerates = imx675_80fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX675_MODE_H2V2_BINNING
	},
};

#endif
