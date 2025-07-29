/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx662_mode_tbls.h - imx662 sensor mode tables
 */

#ifndef __fr_IMX585_TABLES__
#define __fr_IMX585_TABLES__


#define STANDBY				0x3000
#define REGHOLD				0x3001
#define XMSTA				0x3002

#define INCK_SEL			0x3014
#define DATARATE_SEL			0x3015
#define WINMODE				0x3018
#define WDMODE				0x301A
#define ADDMODE				0x301B

#define HVREVERSE			0x3020
#define VREVERSE			0x3021
#define ADBIT				0x3022
#define COMBI_EN			0x3024
#define CCMP_EN				0x36EF
#define MDBIT				0x3023

#define VMAX_LOW			0x3028
#define VMAX_MID			0x3029
#define VMAX_HIGH			0x302A
#define HMAX_LOW			0x302C
#define HMAX_HIGH			0x302D

#define FDG_SEL0			0x3030
#define PIX_HST_LOW			0x303C
#define PIX_HST_HIGH			0x303D
#define PIX_HWIDTH_LOW			0x303E
#define PIX_HWIDTH_HIGH			0x303F
#define LANEMODE			0x3040

#define PIX_XSIZE_OVRLAP_LOW		0x3042
#define PIX_XSIZE_OVRLAP_HIGH		0x3043
#define PIX_VST_LOW			0x3044
#define PIX_VST_HIGH			0x3045
#define PIX_VWIDTH_LOW			0x3046
#define PIX_VWIDTH_HIGH			0x3047

#define SHR0_LOW			0x3050
#define SHR0_MID			0x3051
#define SHR0_HIGH			0x3052

#define GAIN_LOW			0x306C
#define GAIN_HIGH			0x306D
#define EXP_GAIN			0x3081

#define XHSOUTSEL_XVSOUTSEL		0x30A4
#define XVS_XHS_DRV			0x30A6
#define XVSLNG				0x30CC
#define XHSLNG				0x30CD

#define DIG_CLP_VSTART			0x30D5
#define BLKLEVEL_LOW			0x30DC
#define BLKLEVEL_HIGH			0x30DD

#define TPG_EN_DUOUT			0x30E0
#define TPG_PATSEL_DUOUT		0x30E2
#define TPG_COLORWIDTH			0x30E4
#define TESTCLKEN			0x5300

#define EXTMODE				0x30CE
#define SECOND_SLAVE_ADD		0x300C

#define DUR_LOW				0x3930
#define DUR_HIGH			0x3931

#define WAIT_ST0_LOW			0x3A4C
#define WAIT_ST0_HIGH			0x3A4D
#define WAIT_ST1_LOW			0x3A50
#define WAIT_ST1_HIGH			0x3A51
#define ADTHEN				0x3E10

#define EXP_BK				0x36E2

#define CCMP2_EXP_LOW			0x36E4
#define CCMP2_EXP_MID			0x36E5
#define CCMP2_EXP_HIGH			0x36E6
#define CCMP1_EXP_LOW			0x36E8
#define CCMP1_EXP_MID			0x36E9
#define CCMP1_EXP_HIGH			0x36EA

#define EXP_TH_H_LOW			0x36D0
#define EXP_TH_H_HIGH			0x36D1
#define EXP_TH_L_LOW			0x36D4
#define EXP_TH_L_HIGH			0x36D5

#define ACMP2_EXP			0x36EC
#define ACMP1_EXP			0x36EE

#define WAIT_10_SHF			0x493C
#define WAIT_12_SHF			0x4940

/* Special values for the write table function */
#define IMX585_TABLE_WAIT_MS		0
#define IMX585_TABLE_END		1
#define IMX585_WAIT_MS			10

#define IMX585_DEFAULT_WIDTH		3856
#define IMX585_DEFAULT_HEIGHT		2180
#define IMX585_CROP_1920x1080_WIDTH	1920
#define IMX585_CROP_1920x1080_HEIGHT	1080
#define IMX585_MODE_BINNING_H2V2_WIDTH	1928
#define IMX585_MODE_BINNING_H2V2_HEIGHT	1090

/**
 * Minimal value of frame length is resolution height + 30
 * Minimal value for scaling modes is full pixel mode height + 30
 *
 * Determined from the default value of FRM_LENGTH_LINES register
 * and empirically confirmed
 */
#define IMX585_MIN_FRAME_LENGTH_DELTA 70

#define IMX585_TO_LOW_BYTE(x)	(x & 0xFF)
#define IMX585_TO_MID_BYTE(x)	(x >> 8)

typedef struct reg_8 imx585_reg;

/**
 * Tables for the write table function
 */

static const imx585_reg imx585_start[] = {
	{STANDBY,		0x00},
	{IMX585_TABLE_WAIT_MS,	30},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_stop[] = {
	{XMSTA,			0x01},
	{IMX585_TABLE_WAIT_MS,	30},
	{STANDBY,		0x01},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_10bit_mode[] = {
	{WDMODE,		0x00},
	{ADBIT,			0x00},
	{MDBIT,			0x00},
	{0x3930,		0x66},
	{0x3931,		0x00},
	{0x4231,		0x18},
	{DUR_LOW,		0x66},
	{DUR_HIGH,		0x00},
	{COMBI_EN,		0x00},

	{WAIT_ST0_LOW,		0x39},
	{WAIT_ST0_HIGH,		0x01},
	{WAIT_ST1_LOW,		0x48},
	{WAIT_ST1_HIGH,		0x01},

	{ADTHEN,		0x10},
	{WAIT_10_SHF,		0x23},
	{0x3069,		0x00},
	{0x3074,		0x64},

	{IMX585_TABLE_WAIT_MS, IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_12bit_mode[] = {
	{WDMODE,		0x00},
	{ADBIT,			0x02},
	{MDBIT,			0x01},
	{0x3930,		0x0C},
	{0x3931,		0x01},
	{0x4231,		0x08},
	{DUR_LOW,		0x0C},
	{DUR_HIGH,		0x01},
	{COMBI_EN,		0x00},

	{WAIT_ST0_LOW,		0x39},
	{WAIT_ST0_HIGH,		0x01},
	{WAIT_ST1_LOW,		0x48},
	{WAIT_ST1_HIGH,		0x01},

	{ADTHEN,		0x10},
	{WAIT_12_SHF,		0x23},
	{0x3069,		0x00},
	{0x3074,		0x64},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_clearHDR_16bit_mode[] = {
	{MDBIT,			0x03},
	{COMBI_EN,		0x00},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_clearHDR_10bit_mode[] = {
	{WDMODE,		0x10},
	{DUR_LOW,		0x5C},
	{DUR_HIGH,		0x00},
	{COMBI_EN,		0x02},

	{WAIT_ST0_LOW,		0x61},
	{WAIT_ST0_HIGH,		0x02},
	{WAIT_ST1_LOW,		0x70},
	{WAIT_ST1_HIGH,		0x02},

	{ADTHEN,		0x17},
	{WAIT_10_SHF,		0x41},
	{0x3069,		0x02},
	{0x3074,		0x63},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}

};

static const imx585_reg imx585_clearHDR_12bit_mode[] = {
	{WDMODE, 0x10},
	{DUR_LOW,		0xE6},
	{DUR_HIGH,		0x00},
	{COMBI_EN,		0x02},

	{WAIT_ST0_LOW,		0x61},
	{WAIT_ST0_HIGH,		0x02},
	{WAIT_ST1_LOW,		0x70},
	{WAIT_ST1_HIGH,		0x02},

	{ADTHEN,		0x17},
	{WAIT_12_SHF,		0x41},
	{0x3069,		0x02},
	{0x3074,		0x63},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x00}
};

static const imx585_reg imx585_init_settings[] = {
	{LANEMODE,		0x03},
	{INCK_SEL,		0x01},
	{0x303A,		0x01},

	{0x3460,		0x21},
	{0x3478,		0xA1},
	{0x347C,		0x01},
	{0x3480,		0x01},
	{WAIT_ST0_LOW,		0x39},
	{WAIT_ST0_HIGH,		0x01},
	{0x3A4E,		0x14},
	{WAIT_ST1_LOW,		0x48},
	{WAIT_ST1_HIGH,		0x01},
	{0x3A52,		0x14},
	{0x3A56,		0x00},
	{0x3A5A,		0x00},
	{0x3A5E,		0x00},
	{0x3A62,		0x00},
	{0x3A6A,		0x20},
	{0x3A6C,		0x42},
	{0x3A6E,		0xA0},
	{0x3B2C,		0x0C},
	{0x3B30,		0x1C},
	{0x3B34,		0x0C},
	{0x3B38,		0x1C},
	{0x3BA0,		0x0C},
	{0x3BA4,		0x1C},
	{0x3BA8,		0x0C},
	{0x3BAC,		0x1C},
	{0x3D3C,		0x11},
	{0x3D46,		0x0B},
	{0x3DE0,		0x3F},
	{0x3DE1,		0x08},
	{ADTHEN,		0x10},
	{0x3E14,		0x87},
	{0x3E16,		0x91},
	{0x3E18,		0x91},
	{0x3E1A,		0x87},
	{0x3E1C,		0x78},
	{0x3E1E,		0x50},
	{0x3E20,		0x50},
	{0x3E22,		0x50},
	{0x3E24,		0x87},
	{0x3E26,		0x91},
	{0x3E28,		0x91},
	{0x3E2A,		0x87},
	{0x3E2C,		0x78},
	{0x3E2E,		0x50},
	{0x3E30,		0x50},
	{0x3E32,		0x50},
	{0x3E34,		0x87},
	{0x3E36,		0x91},
	{0x3E38,		0x91},
	{0x3E3A,		0x87},
	{0x3E3C,		0x78},
	{0x3E3E,		0x50},
	{0x3E40,		0x50},
	{0x3E42,		0x50},
	{0x4054,		0x64},
	{0x4148,		0xFE},
	{0x4149,		0x05},
	{0x414A,		0xFF},
	{0x414B,		0x05},
	{0x420A,		0x03},
	{0x423D,		0x9C},
	{0x4242,		0xB4},
	{0x4246,		0xB4},
	{0x424E,		0xB4},
	{0x425C,		0xB4},
	{0x425E,		0xB6},
	{0x426C,		0xB4},
	{0x426E,		0xB6},
	{0x428C,		0xB4},
	{0x428E,		0xB6},
	{0x4708,		0x00},
	{0x4709,		0x00},
	{0x470A,		0xFF},
	{0x470B,		0x03},
	{0x470C,		0x00},
	{0x470D,		0x00},
	{0x470E,		0xFF},
	{0x470F,		0x03},
	{0x47EB,		0x1C},
	{0x47F0,		0xA6},
	{0x47F2,		0xA6},
	{0x47F4,		0xA0},
	{0x47F6,		0x96},
	{0x4808,		0xA6},
	{0x480A,		0xA6},
	{0x480C,		0xA0},
	{0x480E,		0x96},
	{0x492C,		0xB2},
	{0x4930,		0x03},
	{0x4932,		0x03},
	{0x4936,		0x5B},
	{0x4938,		0x82},
	{0x493C,		0x23},
	{0x493E,		0x23},
	{0x4940,		0x23},
	{0x4BA8,		0x1C},
	{0x4BA9,		0x03},
	{0x4BAC,		0x1C},
	{0x4BAD,		0x1C},
	{0x4BAE,		0x1C},
	{0x4BAF,		0x1C},
	{0x4BB0,		0x1C},
	{0x4BB1,		0x1C},
	{0x4BB2,		0x1C},
	{0x4BB3,		0x1C},
	{0x4BB4,		0x1C},
	{0x4BB8,		0x03},
	{0x4BB9,		0x03},
	{0x4BBA,		0x03},
	{0x4BBB,		0x03},
	{0x4BBC,		0x03},
	{0x4BBD,		0x03},
	{0x4BBE,		0x03},
	{0x4BBF,		0x03},
	{0x4BC0,		0x03},
	{0x4C14,		0x87},
	{0x4C16,		0x91},
	{0x4C18,		0x91},
	{0x4C1A,		0x87},
	{0x4C1C,		0x78},
	{0x4C1E,		0x50},
	{0x4C20,		0x50},
	{0x4C22,		0x50},
	{0x4C24,		0x87},
	{0x4C26,		0x91},
	{0x4C28,		0x91},
	{0x4C2A,		0x87},
	{0x4C2C,		0x78},
	{0x4C2E,		0x50},
	{0x4C30,		0x50},
	{0x4C32,		0x50},
	{0x4C34,		0x87},
	{0x4C36,		0x91},
	{0x4C38,		0x91},
	{0x4C3A,		0x87},
	{0x4C3C,		0x78},
	{0x4C3E,		0x50},
	{0x4C40,		0x50},
	{0x4C42,		0x50},
	{0x4D12,		0x1F},
	{0x4D13,		0x1E},
	{0x4D26,		0x33},
	{0x4E0E,		0x59},
	{0x4E14,		0x55},
	{0x4E16,		0x59},
	{0x4E1E,		0x3B},
	{0x4E20,		0x47},
	{0x4E22,		0x54},
	{0x4E26,		0x81},
	{0x4E2C,		0x7D},
	{0x4E2E,		0x81},
	{0x4E36,		0x63},
	{0x4E38,		0x6F},
	{0x4E3A,		0x7C},
	{0x4F3A,		0x3C},
	{0x4F3C,		0x46},
	{0x4F3E,		0x59},
	{0x4F42,		0x64},
	{0x4F44,		0x6E},
	{0x4F46,		0x81},
	{0x4F4A,		0x82},
	{0x4F5A,		0x81},
	{0x4F62,		0xAA},
	{0x4F72,		0xA9},
	{0x4F78,		0x36},
	{0x4F7A,		0x41},
	{0x4F7C,		0x61},
	{0x4F7D,		0x01},
	{0x4F7E,		0x7C},
	{0x4F7F,		0x01},
	{0x4F80,		0x77},
	{0x4F82,		0x7B},
	{0x4F88,		0x37},
	{0x4F8A,		0x40},
	{0x4F8C,		0x62},
	{0x4F8D,		0x01},
	{0x4F8E,		0x76},
	{0x4F8F,		0x01},
	{0x4F90,		0x5E},
	{0x4F91,		0x02},
	{0x4F92,		0x69},
	{0x4F93,		0x02},
	{0x4F94,		0x89},
	{0x4F95,		0x02},
	{0x4F96,		0xA4},
	{0x4F97,		0x02},
	{0x4F98,		0x9F},
	{0x4F99,		0x02},
	{0x4F9A,		0xA3},
	{0x4F9B,		0x02},
	{0x4FA0,		0x5F},
	{0x4FA1,		0x02},
	{0x4FA2,		0x68},
	{0x4FA3,		0x02},
	{0x4FA4,		0x8A},
	{0x4FA5,		0x02},
	{0x4FA6,		0x9E},
	{0x4FA7,		0x02},
	{0x519E,		0x79},
	{0x51A6,		0xA1},
	{0x51F0,		0xAC},
	{0x51F2,		0xAA},
	{0x51F4,		0xA5},
	{0x51F6,		0xA0},
	{0x5200,		0x9B},
	{0x5202,		0x91},
	{0x5204,		0x87},
	{0x5206,		0x82},
	{0x5208,		0xAC},
	{0x520A,		0xAA},
	{0x520C,		0xA5},
	{0x520E,		0xA0},
	{0x5210,		0x9B},
	{0x5212,		0x91},
	{0x5214,		0x87},
	{0x5216,		0x82},
	{0x5218,		0xAC},
	{0x521A,		0xAA},
	{0x521C,		0xA5},
	{0x521E,		0xA0},
	{0x5220,		0x9B},
	{0x5222,		0x91},
	{0x5224,		0x87},
	{0x5226,		0x82},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

static const imx585_reg mode_3864x2180[] = {
	{WINMODE,		0x10},
	{ADDMODE,		0x00},
	{DIG_CLP_VSTART,	0x04},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

static const imx585_reg mode_crop_1920x1080[] = {
	{WINMODE,		0x14},
	{ADDMODE,		0x00},
	{DIG_CLP_VSTART,	0x04},

	{PIX_HST_HIGH,		IMX585_TO_MID_BYTE(972)},
	{PIX_HST_LOW,		IMX585_TO_LOW_BYTE(972)},
	{PIX_HWIDTH_HIGH,	IMX585_TO_MID_BYTE(IMX585_CROP_1920x1080_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX585_TO_LOW_BYTE(IMX585_CROP_1920x1080_WIDTH)},

	{PIX_VST_HIGH,		IMX585_TO_MID_BYTE(548)},
	{PIX_VST_LOW,		IMX585_TO_LOW_BYTE(548)},
	{PIX_VWIDTH_HIGH,	IMX585_TO_MID_BYTE(IMX585_CROP_1920x1080_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX585_TO_LOW_BYTE(IMX585_CROP_1920x1080_HEIGHT)},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

static const imx585_reg mode_h2v2_binning[] = {
	{WINMODE,		0x10},
	{ADDMODE,		0x01},
	{DIG_CLP_VSTART,	0x02},

	{ADBIT,			0x00},
	{MDBIT,			0x01},
	{0x3930,		0x66},
	{0x3931,		0x00},
	{0x4231,		0x18},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

static const imx585_reg mode_enable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x0A},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

static const imx585_reg mode_disable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x32},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x02},

	{IMX585_TABLE_WAIT_MS,	IMX585_WAIT_MS},
	{IMX585_TABLE_END,	0x0000}
};

/* Enum of available frame modes */
enum {
	IMX585_MODE_3864x2180,
	IMX585_MODE_CROP_1920x1080,
	IMX585_MODE_H2V2_BINNING,

	IMX585_10BIT_MODE,
	IMX585_12BIT_MODE,
	IMX585_CLEAR_HDR_10BIT_MODE,
	IMX585_CLEAR_HDR_12BIT_MODE,
	IMX585_CLEAR_HDR_16BIT_MODE,

	IMX585_EN_PATTERN_GEN,
	IMX585_DIS_PATTERN_GEN,

	IMX585_INIT_SETTINGS,
	IMX585_MODE_START_STREAM,
	IMX585_MODE_STOP_STREAM,
};

typedef enum {
	IMX585_2376_MBPS,
	IMX585_2079_MBPS,
	IMX585_1782_MBPS,
	IMX585_1440_MBPS,
	IMX585_1188_MBPS,
	IMX585_891_MBPS,
	IMX585_720_MBPS,
	IMX585_594_MBPS,
} data_rate_mode;

/* Connecting frame modes to mode tables */
static const imx585_reg *mode_table[]	= {
	[IMX585_MODE_3864x2180]		= mode_3864x2180,
	[IMX585_MODE_CROP_1920x1080]	= mode_crop_1920x1080,
	[IMX585_MODE_H2V2_BINNING]	= mode_h2v2_binning,

	[IMX585_EN_PATTERN_GEN]		= mode_enable_pattern_generator,
	[IMX585_DIS_PATTERN_GEN]	= mode_disable_pattern_generator,

	[IMX585_10BIT_MODE]		= imx585_10bit_mode,
	[IMX585_12BIT_MODE]		= imx585_12bit_mode,
	[IMX585_CLEAR_HDR_10BIT_MODE]	= imx585_clearHDR_10bit_mode,
	[IMX585_CLEAR_HDR_12BIT_MODE]	= imx585_clearHDR_12bit_mode,
	[IMX585_CLEAR_HDR_16BIT_MODE]	= imx585_clearHDR_16bit_mode,

	[IMX585_INIT_SETTINGS]		= imx585_init_settings,

	[IMX585_MODE_START_STREAM]	= imx585_start,
	[IMX585_MODE_STOP_STREAM]	= imx585_stop,
};

/* Framerates of available frame modes */
static const int imx585_72fps[] = {
	72,
};
static const int imx585_90fps[] = {
	90,
};
static const int imx585_142fps[] = {
	142,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx585_frmfmt[] = {
	{
		.size = {IMX585_DEFAULT_WIDTH, IMX585_DEFAULT_HEIGHT},
		.framerates = imx585_72fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX585_MODE_3864x2180
	},
	{
		.size = {IMX585_CROP_1920x1080_WIDTH, IMX585_CROP_1920x1080_HEIGHT},
		.framerates = imx585_142fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX585_MODE_CROP_1920x1080
	},
	{
		.size = {IMX585_MODE_BINNING_H2V2_WIDTH, IMX585_MODE_BINNING_H2V2_HEIGHT},
		.framerates = imx585_90fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX585_MODE_H2V2_BINNING
	},
};

#endif
