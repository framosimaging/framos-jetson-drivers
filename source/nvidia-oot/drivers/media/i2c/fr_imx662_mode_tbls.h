/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx662_mode_tbls.h - imx662 sensor mode tables
 */

#ifndef __fr_IMX662_TABLES__
#define __fr_IMX662_TABLES__


#define STANDBY			0x3000
#define REGHOLD			0x3001
#define XMSTA			0x3002
#define SECOND_SLAVE_ADD	0x300C

#define INCK_SEL		0x3014
#define DATARATE_SEL		0x3015
#define WINMODE			0x3018

#define WDMODE			0x301A
#define ADDMODE			0x301B
#define THIN_V_EN		0x301C
#define VCMODE			0x301E

#define HREVERSE		0x3020
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
#define FDG_SEL2		0x3032
#define PIX_HST_LOW		0x303C
#define PIX_HST_HIGH		0x303D
#define PIX_HWIDTH_LOW		0x303E
#define PIX_HWIDTH_HIGH		0x303F

#define LANEMODE		0x3040

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
#define SHR2_LOW		0x3058
#define SHR2_MID		0x3059
#define SHR2_HIGH		0x305A

#define RHS1_LOW		0x3060
#define RHS1_MID		0x3061
#define RHS1_HIGH		0x3062
#define RHS2_LOW		0x3064
#define RHS2_MID		0x3065
#define RHS2_HIGH		0x3066

#define GAIN_LOW		0x3070
#define GAIN_HIGH		0x3071
#define GAIN_1_LOW		0x3072
#define GAIN_1_HIGH		0x3073
#define GAIN_2_LOW		0x3074
#define GAIN_2_HIGH		0x3075

#define EXP_GAIN		0x3081

#define XVSOUTSEL_XHSOUTSEL	0x30A4
#define XVS_DRV_XHS_DRV		0x30A6

#define XVSLNG			0x30CC
#define XHSLNG			0x30CD
#define EXTMODE			0x30CE
#define BLKLEVEL_LOW		0x30DC
#define BLKLEVEL_HIGH		0x30DD

#define TPG_EN_DUOUT		0x30E0
#define TPG_PATSEL_DUOUT	0x30E2
#define TPG_COLORWIDTH		0x30E4
#define TESTCLKEN		0x4900

#define GAIN_PGC_FIDMD		0x3400

/* Resolutions of implemented frame modes */
#define IMX662_DEFAULT_WIDTH		1920
#define IMX662_DEFAULT_HEIGHT		1080

#define IMX662_1280x720_WIDTH		1280
#define IMX662_1280x720_HEIGHT		720

#define IMX662_640x480_WIDTH		640
#define IMX662_640x480_HEIGHT		480

#define IMX662_MODE_BINNING_H2V2_WIDTH	968
#define IMX662_MODE_BINNING_H2V2_HEIGHT	550

#define IMX662_DOL_HDR_WIDTH		1952
#define IMX662_DOL_HDR_HEIGHT		2396

#define IMX662_DOL_BINNING_HDR_WIDTH	984
#define IMX662_DOL_BINNING_HDR_HEIGHT	1198

#define IMX662_DEFAULT_RHS1		157

/* Special values for the write table function */
#define IMX662_TABLE_WAIT_MS		0
#define IMX662_TABLE_END		1
#define IMX662_WAIT_MS			10

#define IMX662_MIN_FRAME_LENGTH_DELTA	70

typedef struct reg_8 imx662_reg;

#define IMX662_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX662_TO_MID_BYTE(x) (x >> 8)

/* Tables for the write table function */

static const imx662_reg imx662_start[] = {
	{STANDBY,		0x00},

	{IMX662_TABLE_WAIT_MS,	24},
	{IMX662_TABLE_END,	0x00}
};

static const imx662_reg imx662_stop[] = {
	{STANDBY,		0x01},
	{IMX662_TABLE_WAIT_MS,	24},
	{XMSTA,			0x01},

	{IMX662_TABLE_WAIT_MS,	10},
	{IMX662_TABLE_END,	0x00}
};

static const imx662_reg imx662_init_settings[] = {
	{INCK_SEL,		0x01},

	{DATARATE_SEL,		0x00},
	{ADBIT,			0x00},
	{MDBIT,			0x01},
	{HMAX_LOW,		0xDE},
	{HMAX_HIGH,		0x03},
	{LANEMODE,		0x03},
	{SHR0_LOW,		0x04},
	{XVS_DRV_XHS_DRV,	0x00},

	{0x3444,		0xAC},
	{0x3460,		0x21},
	{0x3492,		0x08},
	{0x3A50,		0x62},
	{0x3A51,		0x01},
	{0x3A52,		0x19},
	{0x3B00,		0x39},
	{0x3B23,		0x2D},
	{0x3B45,		0x04},
	{0x3C0A,		0x1F},
	{0x3C0B,		0x1E},
	{0x3C38,		0x21},

	{0x3C44,		0x00},
	{0x3CB6,		0xD8},
	{0x3CC4,		0xDA},
	{0x3E24,		0x79},
	{0x3E2C,		0x15},
	{0x3EDC,		0x2D},
	{0x4498,		0x05},
	{0x449C,		0x19},
	{0x449D,		0x00},
	{0x449E,		0x32},
	{0x449F,		0x01},
	{0x44A0,		0x92},
	{0x44A2,		0x91},
	{0x44A4,		0x8C},
	{0x44A6,		0x87},
	{0x44A8,		0x82},
	{0x44AA,		0x78},
	{0x44AC,		0x6E},
	{0x44AE,		0x69},
	{0x44B0,		0x92},
	{0x44B2,		0x91},
	{0x44B4,		0x8C},
	{0x44B6,		0x87},
	{0x44B8,		0x82},
	{0x44BA,		0x78},
	{0x44BC,		0x6E},
	{0x44BE,		0x69},
	{0x44C0,		0x7F},
	{0x44C1,		0x01},
	{0x44C2,		0x7F},
	{0x44C3,		0x01},
	{0x44C4,		0x7A},
	{0x44C5,		0x01},
	{0x44C6,		0x7A},
	{0x44C7,		0x01},
	{0x44C8,		0x70},
	{0x44C9,		0x01},
	{0x44CA,		0x6B},
	{0x44CB,		0x01},
	{0x44CC,		0x6B},
	{0x44CD,		0x01},
	{0x44CE,		0x5C},
	{0x44CF,		0x01},
	{0x44D0,		0x7F},
	{0x44D1,		0x01},
	{0x44D2,		0x7F},
	{0x44D3,		0x01},
	{0x44D4,		0x7A},
	{0x44D5,		0x01},
	{0x44D6,		0x7A},
	{0x44D7,		0x01},
	{0x44D8,		0x70},
	{0x44D9,		0x01},
	{0x44DA,		0x6B},
	{0x44DB,		0x01},
	{0x44DC,		0x6B},
	{0x44DD,		0x01},
	{0x44DE,		0x5C},
	{0x44DF,		0x01},
	{0x4534,		0x1C},
	{0x4535,		0x03},
	{0x4538,		0x1C},
	{0x4539,		0x1C},
	{0x453A,		0x1C},
	{0x453B,		0x1C},
	{0x453C,		0x1C},
	{0x453D,		0x1C},
	{0x453E,		0x1C},
	{0x453F,		0x1C},
	{0x4540,		0x1C},
	{0x4541,		0x03},
	{0x4542,		0x03},
	{0x4543,		0x03},
	{0x4544,		0x03},
	{0x4545,		0x03},
	{0x4546,		0x03},
	{0x4547,		0x03},
	{0x4548,		0x03},
	{0x4549,		0x03},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

static const imx662_reg imx662_12bit_mode[] = {
	{ADBIT,			0x01},
	{MDBIT,			0x01},
	{0x3A50,		0xFF},
	{0x3A51,		0x03},
	{0x3A52,		0x00},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x00},
};

static const imx662_reg imx662_10bit_mode[] = {
	{ADBIT,			0x00},
	{MDBIT,			0x00},
	{0x3A50,		0x62},
	{0x3A51,		0x01},
	{0x3A52,		0x19},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x00},
};

static const imx662_reg mode_1920x1080[] = {
	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{PIX_HST_HIGH,		IMX662_TO_MID_BYTE(8)},
	{PIX_HST_LOW,		IMX662_TO_LOW_BYTE(8)},
	{PIX_HWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_DEFAULT_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_DEFAULT_WIDTH)},

	{PIX_VST_HIGH,		IMX662_TO_MID_BYTE(12)},
	{PIX_VST_LOW,		IMX662_TO_LOW_BYTE(12)},
	{PIX_VWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_DEFAULT_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_DEFAULT_HEIGHT)},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x00},
};

static const imx662_reg mode_1280x720[] = {
	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{PIX_HST_HIGH,		IMX662_TO_MID_BYTE(328)},
	{PIX_HST_LOW,		IMX662_TO_LOW_BYTE(328)},
	{PIX_HWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_1280x720_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_1280x720_WIDTH)},

	{PIX_VST_HIGH,		IMX662_TO_MID_BYTE(192)},
	{PIX_VST_LOW,		IMX662_TO_LOW_BYTE(192)},
	{PIX_VWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_1280x720_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_1280x720_HEIGHT)},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x00},
};

static const imx662_reg mode_640x480[] = {
	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{PIX_HST_HIGH,		IMX662_TO_MID_BYTE(640)},
	{PIX_HST_LOW,		IMX662_TO_LOW_BYTE(640)},
	{PIX_HWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_640x480_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_640x480_WIDTH)},

	{PIX_VST_HIGH,		IMX662_TO_MID_BYTE(312)},
	{PIX_VST_LOW,		IMX662_TO_LOW_BYTE(312)},
	{PIX_VWIDTH_HIGH,	IMX662_TO_MID_BYTE(IMX662_640x480_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX662_TO_LOW_BYTE(IMX662_640x480_HEIGHT)},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x00},
};

static const imx662_reg mode_h2v2_binning[] = {

	{WINMODE,		0x00},
	{ADDMODE,		0x01},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{0x3A50,		0x62},
	{0x3A51,		0x01},
	{0x3A52,		0x19},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

static const imx662_reg mode_dol_hdr[] = {
	{WINMODE,		0x00},

	{WDMODE,		0x01},
	{THIN_V_EN,		0x00},
	{VCMODE,		0x00},
	{VMAX_LOW,		0xC4},
	{VMAX_MID,		0x09},
	{SHR0_LOW,		0xE8},
	{SHR0_MID,		0x03},
	{SHR1_LOW,		0x05},
	{RHS1_LOW,		IMX662_TO_LOW_BYTE(IMX662_DEFAULT_RHS1)},
	{RHS1_MID,		IMX662_TO_MID_BYTE(IMX662_DEFAULT_RHS1)},
	{GAIN_PGC_FIDMD,	0x00},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

static const imx662_reg mode_dol_binning_hdr[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x01},
	{WDMODE,		0x01},

	{THIN_V_EN,		0x00},
	{VCMODE,		0x00},
	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{SHR0_LOW,		0xE8},
	{SHR0_MID,		0x03},
	{SHR1_LOW,		0x05},
	{RHS1_LOW,		IMX662_TO_LOW_BYTE(IMX662_DEFAULT_RHS1)},
	{RHS1_MID,		IMX662_TO_MID_BYTE(IMX662_DEFAULT_RHS1)},
	{GAIN_PGC_FIDMD,	0x00},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

/* Enum describing available data rate modes */
typedef enum {
	IMX662_1440_MBPS,
	IMX662_1188_MBPS,
	IMX662_891_MBPS,
	IMX662_720_MBPS,
	IMX662_594_MBPS,
} data_rate_mode;

/* Enum of available frame modes */
enum {
	IMX662_MODE_1920x1080,
	IMX662_MODE_1280x720,
	IMX662_MODE_640x480,
	IMX662_MODE_H2V2_BINNING,
	IMX662_MODE_DOL_HDR,
	IMX662_MODE_DOL_BINNING,

	IMX662_EN_PATTERN_GEN,
	IMX662_DIS_PATTERN_GEN,

	IMX662_10BIT_MODE,
	IMX662_12BIT_MODE,

	IMX662_INIT_SETTINGS,
	IMX662_MODE_START_STREAM,
	IMX662_MODE_STOP_STREAM,
};

static const imx662_reg mode_enable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x0A},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

static const imx662_reg mode_disable_pattern_generator[] = {
	{BLKLEVEL_LOW,		0x32},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x02},

	{IMX662_TABLE_WAIT_MS,	IMX662_WAIT_MS},
	{IMX662_TABLE_END,	0x0000}
};

/* Connecting frame modes to mode tables */
static const imx662_reg *mode_table[] = {
	[IMX662_MODE_1920x1080]		= mode_1920x1080,
	[IMX662_MODE_1280x720]		= mode_1280x720,
	[IMX662_MODE_640x480]		= mode_640x480,
	[IMX662_MODE_H2V2_BINNING]	= mode_h2v2_binning,
	[IMX662_MODE_DOL_HDR]		= mode_dol_hdr,
	[IMX662_MODE_DOL_BINNING]	= mode_dol_binning_hdr,

	[IMX662_EN_PATTERN_GEN]		= mode_enable_pattern_generator,
	[IMX662_DIS_PATTERN_GEN]	= mode_disable_pattern_generator,

	[IMX662_10BIT_MODE]		= imx662_10bit_mode,
	[IMX662_12BIT_MODE]		= imx662_12bit_mode,

	[IMX662_INIT_SETTINGS]		= imx662_init_settings,

	[IMX662_MODE_START_STREAM]	= imx662_start,
	[IMX662_MODE_STOP_STREAM]	= imx662_stop,
};

/* Frame rates of available frame modes */

static const int imx662_30fps[] = {
	30,
};
static const int imx662_90fps[] = {
	90,
};
static const int imx662_60fps[] = {
	60,
};
static const int imx662_176fps[] = {
	176,
};

/* Connecting resolutions, frame rates and mode tables */
static const struct camera_common_frmfmt imx662_frmfmt[] = {
	{
		.size = {IMX662_DEFAULT_WIDTH, IMX662_DEFAULT_HEIGHT},
		.framerates = imx662_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX662_MODE_1920x1080
	},
	{
		.size = {IMX662_1280x720_WIDTH, IMX662_1280x720_HEIGHT},
		.framerates = imx662_176fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX662_MODE_1280x720
	},
	{
		.size = {IMX662_640x480_WIDTH, IMX662_640x480_HEIGHT},
		.framerates = imx662_176fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX662_MODE_640x480
	},
	{
		.size = {IMX662_MODE_BINNING_H2V2_WIDTH, IMX662_MODE_BINNING_H2V2_HEIGHT},
		.framerates = imx662_90fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX662_MODE_H2V2_BINNING
	},
	{
		.size = {IMX662_DOL_HDR_WIDTH, IMX662_DOL_HDR_HEIGHT},
		.framerates = imx662_30fps,
		.num_framerates = 1,
		.hdr_en = true,
		.mode = IMX662_MODE_DOL_HDR
	},
	{
		.size = {IMX662_DOL_BINNING_HDR_WIDTH, IMX662_DOL_BINNING_HDR_HEIGHT},
		.framerates = imx662_30fps,
		.num_framerates = 1,
		.hdr_en = true,
		.mode = IMX662_MODE_DOL_BINNING
	}
};

#endif
