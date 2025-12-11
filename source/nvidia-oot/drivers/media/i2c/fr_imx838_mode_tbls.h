/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx838_mode_tbls.h - imx838 sensor mode tables
 */

#ifndef __fr_IMX838_TABLES__
#define __fr_IMX838_TABLES__


#define STANDBY			0x3000
#define REGHOLD			0x3001
#define XMSTA			0x3002

#define INCK_SEL		0x3014
#define DATARATE_SEL		0x3015
#define WINMODE			0x3018
#define WDMODE			0x301A
#define ADDMODE			0x301B

#define HVREVERSE		0x3020
#define VREVERSE		0x3021
#define ADBIT			0x3022
#define MDBIT			0x3023

#define VMAX_LOW		0x3028
#define VMAX_MID		0x3029
#define VMAX_HIGH		0x302A
#define HMAX_LOW		0x302C
#define HMAX_HIGH		0x302D

#define FDG_SEL0		0x3030
#define FDG_SEL1		0x3031
#define PIX_HST_LOW		0x303C
#define PIX_HST_HIGH		0x303D
#define PIX_HWIDTH_LOW		0x303E
#define PIX_HWIDTH_HIGH		0x303F
#define LANEMODE		0x3040

#define PIX_XSIZE_OVRLAP_LOW	0x3042
#define PIX_XSIZE_OVRLAP_HIGH	0x3043
#define PIX_VST_LOW		0x3044
#define PIX_VST_HIGH		0x3045
#define PIX_VWIDTH_LOW		0x3046
#define PIX_VWIDTH_HIGH		0x3047

#define SHR0_LOW		0x3050
#define SHR0_MID		0x3051
#define SHR0_HIGH		0x3052
#define SHR1_LOW		0x3054
#define SHR1_MID		0x3055
#define SHR1_HIGH		0x3056
#define RHS1_LOW		0x3060
#define RHS1_MID		0x3061
#define RHS1_HIGH		0x3062

#define GAIN_LOW		0x3070
#define GAIN_HIGH		0x3071
#define GAIN_1			0x3072

#define XHSOUTSEL_XVSOUTSEL	0x30A4
#define XVS_XHS_DRV		0x30A6
#define XVSLNG			0x30CC
#define XHSLNG			0x30CD

#define BLKLEVEL_LOW		0x30DC
#define BLKLEVEL_HIGH		0x30DD

#define TPG_EN_DUOUT		0x30E0
#define TPG_PATSEL_DUOUT	0x30E2
#define TPG_COLORWIDTH		0x30E4
#define TESTCLKEN		0x5300

#define EXTMODE			0x30CE
#define SECOND_SLAVE_ADD	0x300C

#define GAIN_PGC_FIDMD		0x3400

/* Resolutions of implemented frame modes */
#define IMX838_DEFAULT_WIDTH		3856
#define IMX838_DEFAULT_HEIGHT		2180
#define IMX838_CROP_2608x1964_WIDTH	2608
#define IMX838_CROP_2608x1964_HEIGHT	1964
#define IMX838_CROP_1920x1080_WIDTH	1920
#define IMX838_CROP_1920x1080_HEIGHT	1080
#define IMX838_MODE_BINNING_H2V2_WIDTH	1928
#define IMX838_MODE_BINNING_H2V2_HEIGHT	1090

/* Special values for the write table function */
#define IMX838_TABLE_WAIT_MS	0
#define IMX838_TABLE_END	1
#define IMX838_WAIT_MS		10

#define IMX838_MIN_FRAME_LENGTH_DELTA	70

#define IMX838_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX838_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx838_reg;


/* Tables for the write table function */
static const imx838_reg imx838_start[] = {

	{STANDBY, 0x00},
	{IMX838_TABLE_WAIT_MS, 30},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x00}
};

static const imx838_reg imx838_stop[] = {

	{XMSTA, 0x01},
	{IMX838_TABLE_WAIT_MS, 30},
	{STANDBY, 0x01},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x00}
};

static const imx838_reg imx838_10bit_mode[] = {

	{ADBIT,	0x00},
	{MDBIT, 0x00},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x00}
};

static const imx838_reg imx838_12bit_mode[] = {

	{ADBIT, 0x01},
	{MDBIT, 0x01},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x00}
};

static const imx838_reg imx838_init_settings[] = {

	{LANEMODE,		0x03},
	{INCK_SEL,		0x01},

	{0x3460,		0x22},
	{0x3B4C,		0x30},
	{0x3B4E,		0x30},
	{0x3BD8,		0x62},
	{0x3BDC,		0x62},

	{0x3C04,		0x06},
	{0x3C05,		0x06},
	{0x3C0C,		0x00},
	{0x3C0D,		0x00},
	{0x3C0E,		0x04},
	{0x3C0F,		0x04},
	{0x3C10,		0x04},
	{0x3C11,		0x04},
	{0x3C12,		0x04},
	{0x3C13,		0x04},
	{0x3C3C,		0x02},
	{0x3CAA,		0x02},
	{0x3CAB,		0x01},
	{0x3CC0,		0x04},
	{0x3CC1,		0x01},

	{0x3D39,		0xEE},
	{0x3D3C,		0xAA},
	{0x3D3D,		0x79},
	{0x3D48,		0xCC},
	{0x43C0,		0x1A},
	{0x43C2,		0x1A},
	{0x43C4,		0x1A},
	{0x43C6,		0x1A},
	{0x43C8,		0x1A},
	{0x43CA,		0x1A},
	{0x43CC,		0x1A},
	{0x43CE,		0x1A},
	{0x43D0,		0xE7},
	{0x43D2,		0xE7},
	{0x43D4,		0xE7},
	{0x43D6,		0xE5},
	{0x43D8,		0xBC},
	{0x43DA,		0xBC},
	{0x43DC,		0xBC},
	{0x43DE,		0xBC},
	{0x443D,		0x33},
	{0x449F,		0x0C},

	{0x44A8,		0x1A},
	{0x44AA,		0x1A},
	{0x44AC,		0x1A},
	{0x44AE,		0x1A},
	{0x44C0,		0xE7},
	{0x44C2,		0xE7},
	{0x44C4,		0xE7},
	{0x44C6,		0xE5},
	{0x44C8,		0xE3},
	{0x44CA,		0xBC},
	{0x44CC,		0xBC},
	{0x44CE,		0xBC},
	{0x44E0,		0x19},
	{0x44E1,		0x00},
	{0x44E2,		0xBB},
	{0x44E3,		0x00},

	{0x45B4,		0x1B},
	{0x45B8,		0x1B},

	// other than clear HDR
	{0x3A24,		0x05},
	{0x3A26,		0x0A},
	{0x355A,		0x64},
	{0x44A0,		0x4C},
	{0x44A2,		0x44},
	{0x44A4,		0x44},
	{0x44A6,		0x3C},

	{0x4549,		0x01},
	{0x454A,		0x01},
	{0x454B,		0x06},
	{0x454C,		0x06},
	{0x454D,		0x06},
	{0x454E,		0x06},
	{0x454F,		0x06},
	{0x4550,		0x06},
	{0x4E3C,		0x07},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_3864x2180[] = {

	{WINMODE,		0x00},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_crop_2608x1964[] = {

	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},

	{PIX_HST_HIGH,		IMX838_TO_MID_BYTE(628)},
	{PIX_HST_LOW,		IMX838_TO_LOW_BYTE(628)},
	{PIX_HWIDTH_HIGH,	IMX838_TO_MID_BYTE(IMX838_CROP_2608x1964_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX838_TO_LOW_BYTE(IMX838_CROP_2608x1964_WIDTH)},

	{PIX_VST_HIGH,		IMX838_TO_MID_BYTE(108)},
	{PIX_VST_LOW,		IMX838_TO_LOW_BYTE(108)},
	{PIX_VWIDTH_HIGH,	IMX838_TO_MID_BYTE(IMX838_CROP_2608x1964_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX838_TO_LOW_BYTE(IMX838_CROP_2608x1964_HEIGHT)},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_crop_1920x1080[] = {

	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},

	{PIX_HST_HIGH,		IMX838_TO_MID_BYTE(972)},
	{PIX_HST_LOW,		IMX838_TO_LOW_BYTE(972)},
	{PIX_HWIDTH_HIGH,	IMX838_TO_MID_BYTE(IMX838_CROP_1920x1080_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX838_TO_LOW_BYTE(IMX838_CROP_1920x1080_WIDTH)},

	{PIX_VST_HIGH,		IMX838_TO_MID_BYTE(548)},
	{PIX_VST_LOW,		IMX838_TO_LOW_BYTE(548)},
	{PIX_VWIDTH_HIGH,	IMX838_TO_MID_BYTE(IMX838_CROP_1920x1080_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX838_TO_LOW_BYTE(IMX838_CROP_1920x1080_HEIGHT)},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_h2v2_binning[] = {

	{WINMODE,		0x00},
	{ADDMODE,		0x01},
	{WDMODE,		0x00},

	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_enable_pattern_generator[] = {

	{BLKLEVEL_LOW,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x0A},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

static const imx838_reg mode_disable_pattern_generator[] = {

	{BLKLEVEL_LOW,		0x32},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x02},

	{IMX838_TABLE_WAIT_MS, IMX838_WAIT_MS},
	{IMX838_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {

	IMX838_MODE_3864x2180,
	IMX838_MODE_CROP_2608x1964,
	IMX838_MODE_CROP_1920x1080,
	IMX838_MODE_H2V2_BINNING,

	IMX838_10BIT_MODE,
	IMX838_12BIT_MODE,

	IMX838_EN_PATTERN_GEN,
	IMX838_DIS_PATTERN_GEN,

	IMX838_INIT_SETTINGS,
	IMX838_MODE_START_STREAM,
	IMX838_MODE_STOP_STREAM,
};

typedef enum {

	IMX838_2376_MBPS,
	IMX838_2079_MBPS,
	IMX838_1782_MBPS,
	IMX838_1440_MBPS,
	IMX838_1188_MBPS,
	IMX838_891_MBPS,
	IMX838_720_MBPS,
	IMX838_594_MBPS,
} data_rate_mode;


/* Connecting frame modes to mode tables */
static const imx838_reg *mode_table[] = {

	[IMX838_MODE_3864x2180] = mode_3864x2180,
	[IMX838_MODE_CROP_2608x1964] = mode_crop_2608x1964,
	[IMX838_MODE_CROP_1920x1080] = mode_crop_1920x1080,
	[IMX838_MODE_H2V2_BINNING] = mode_h2v2_binning,

	[IMX838_EN_PATTERN_GEN] = mode_enable_pattern_generator,
	[IMX838_DIS_PATTERN_GEN] = mode_disable_pattern_generator,

	[IMX838_10BIT_MODE] = imx838_10bit_mode,
	[IMX838_12BIT_MODE] = imx838_12bit_mode,

	[IMX838_INIT_SETTINGS] = imx838_init_settings,

	[IMX838_MODE_START_STREAM] = imx838_start,
	[IMX838_MODE_STOP_STREAM] = imx838_stop,
};

/* Framerates of available frame modes */

static const int imx838_72fps[] = {
	72,
};
static const int imx838_79fps[] = {
	79,
};
static const int imx838_60fps[] = {
	60,
};
static const int imx838_140fps[] = {
	140,
};

/** Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx838_frmfmt[] = {
	{
		.size = {IMX838_DEFAULT_WIDTH, IMX838_DEFAULT_HEIGHT},
		.framerates = imx838_72fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX838_MODE_3864x2180
	},
	{
		.size = {IMX838_CROP_2608x1964_WIDTH,
						IMX838_CROP_2608x1964_HEIGHT},
		.framerates = imx838_79fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX838_MODE_CROP_2608x1964
	},
	{
		.size = {IMX838_CROP_1920x1080_WIDTH,
						IMX838_CROP_1920x1080_HEIGHT},
		.framerates = imx838_140fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX838_MODE_CROP_1920x1080
	},
	{
		.size = {IMX838_MODE_BINNING_H2V2_WIDTH,
						IMX838_MODE_BINNING_H2V2_HEIGHT},
		.framerates = imx838_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX838_MODE_H2V2_BINNING
	},
};

#endif
