/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx676_mode_tbls.h - imx676 sensor mode tables
 */

#ifndef __fr_IMX676_TABLES__
#define __fr_IMX676_TABLES__


#define STANDBY			0x3000
#define REGHOLD			0x3001
#define XMSTA			0x3002

#define INCK_SEL		0x3014
#define DATARATE_SEL		0x3015
#define WINMODE			0x3018
#define WDMODE			0x301A
#define ADDMODE			0x301B

#define THIN_V_EN		0x301C
#define VCMODE			0x301E

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
#define FDG_SEL2		0x3032

#define PIX_HST_LOW		0x303C
#define PIX_HST_HIGH		0x303D
#define PIX_HWIDTH_LOW		0x303E
#define PIX_HWIDTH_HIGH		0x303F

#define LANEMODE		0x3040
#define XSIZE_OVRLAP_LOW	0x3042

#define PIX_VST_LOW		0x3044
#define PIX_VST_HIGH		0x3045
#define PIX_VWIDTH_LOW		0x3046
#define PIX_VWIDTH_HIGH		0x3047

#define GAIN_HG0		0x304C
#define SHR0_LOW		0x3050
#define SHR0_MID		0x3051
#define SHR0_HIGH		0x3052
#define SHR1_LOW		0x3054
#define SHR1_MID		0x3055
#define SHR1_HIGH		0x3056
#define RHS1_LOW		0x3060
#define RHS1_MID		0x3061
#define RHS1_HIGH		0x3062

#define SHR2			0x3058
#define RHS1			0x3060
#define RHS2			0x3064

#define GAIN_0			0x3070
#define GAIN_1			0x3072
#define GAIN_2			0x3074

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

#define IMX676_DEFAULT_RHS1	250


/* Resolutions of implemented frame modes */
#define IMX676_DEFAULT_WIDTH		3552
#define IMX676_DEFAULT_HEIGHT		3556

#define IMX676_CROP_3552x2160_WIDTH	3552
#define IMX676_CROP_3552x2160_HEIGHT	2160

#define IMX676_MODE_BINNING_H2V2_WIDTH	1776
#define IMX676_MODE_BINNING_H2V2_HEIGHT	1778

#define IMX676_CROP_1768x1080_WIDTH	1768
#define IMX676_CROP_1768x1080_HEIGHT	1080

#define IMX676_DOL_HDR_WIDTH		3568
#define IMX676_DOL_HDR_HEIGHT		7400

#define IMX676_DOL_BINNING_HDR_WIDTH	1792
#define IMX676_DOL_BINNING_HDR_HEIGHT	3700


/* Special values for the write table function */
#define IMX676_TABLE_WAIT_MS	0
#define IMX676_TABLE_END	1
#define IMX676_WAIT_MS		10

#define IMX676_MIN_FRAME_LENGTH_DELTA	72

#define IMX676_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX676_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx676_reg;


/* Tables for the write table function */
static const imx676_reg imx676_start[] = {

	{STANDBY, 0x00},
	{IMX676_TABLE_WAIT_MS, 30},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x00}
};

static const imx676_reg imx676_stop[] = {

	{XMSTA, 0x01},
	{IMX676_TABLE_WAIT_MS, 30},
	{STANDBY, 0x01},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x00}
};

static const imx676_reg imx676_10bit_mode[] = {

	{ADBIT,			0x00},
	{MDBIT,			0x00},

	{0x355A,		0x1C},

	{0x3C0A,		0x03},
	{0x3C0B,		0x03},
	{0x3C0C,		0x03},
	{0x3C0D,		0x03},
	{0x3C0E,		0x03},
	{0x3C0F,		0x03},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x00}
};

static const imx676_reg imx676_12bit_mode[] = {

	{ADBIT,			0x01},
	{MDBIT,			0x01},

	{0x355A,		0x10},

	{0x3C0A,		0x1F},
	{0x3C0B,		0x1F},
	{0x3C0C,		0x1F},
	{0x3C0D,		0x1F},
	{0x3C0E,		0x1F},
	{0x3C0F,		0x1F},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x00}
};

static const imx676_reg imx676_init_settings[] = {

	{LANEMODE,		0x03},
	{INCK_SEL,		0x01},

	{0x304E,		0x04},
	{0x3148,		0x00},
	{0x3460,		0x22},
	{0x347B,		0x02},
	{0x3A3C,		0x0F},
	{0x3A44,		0x0B},
	{0x3A76,		0xB5},
	{0x3A77,		0x00},
	{0x3A78,		0x03},
	{0x3B22,		0x04},
	{0x3B23,		0x44},
	{0x3C03,		0x04},
	{0x3C04,		0x04},
	{0x3C30,		0x73},
	{0x3C34,		0x6C},
	{0x3C3C,		0x20},
	{0x3C44,		0x06},
	{0x3CB8,		0x00},
	{0x3CBA,		0xFF},
	{0x3CBB,		0x03},
	{0x3CBC,		0xFF},
	{0x3CBD,		0x03},
	{0x3CC2,		0xFF},
	{0x3CC3,		0x03},
	{0x3CC8,		0xFF},
	{0x3CC9,		0x03},
	{0x3CCA,		0x00},
	{0x3CCE,		0xFF},
	{0x3CCF,		0x03},
	{0x3CD0,		0xFF},
	{0x3CD1,		0x03},
	{0x3E00,		0x1E},
	{0x3E02,		0x04},
	{0x3E03,		0x00},
	{0x3E20,		0x04},
	{0x3E21,		0x00},
	{0x3E22,		0x1E},
	{0x3E24,		0xB6},
	{0x4490,		0x07},
	{0x4494,		0x10},
	{0x4495,		0x00},
	{0x4496,		0xB2},
	{0x4497,		0x00},
	{0x44A0,		0x33},
	{0x44A2,		0x10},
	{0x44A4,		0x10},
	{0x44A6,		0x10},
	{0x44A8,		0x4B},
	{0x44AA,		0x4B},
	{0x44AC,		0x4B},
	{0x44AE,		0x46},
	{0x44B0,		0x33},
	{0x44B2,		0x10},
	{0x44B4,		0x10},
	{0x44B6,		0x10},
	{0x44B8,		0x42},
	{0x44BA,		0x42},
	{0x44BC,		0x42},
	{0x44BE,		0x42},
	{0x44C0,		0x33},
	{0x44C2,		0x10},
	{0x44C4,		0x10},
	{0x44C6,		0x10},
	{0x44C8,		0xE7},
	{0x44CA,		0xE2},
	{0x44CC,		0xE2},
	{0x44CE,		0xDD},
	{0x44D0,		0xDD},
	{0x44D2,		0xB2},
	{0x44D4,		0xB2},
	{0x44D6,		0xB2},
	{0x44D8,		0xE1},
	{0x44DA,		0xE1},
	{0x44DC,		0xE1},
	{0x44DE,		0xDD},
	{0x44E0,		0xDD},
	{0x44E2,		0xB2},
	{0x44E4,		0xB2},
	{0x44E6,		0xB2},
	{0x44E8,		0xDD},
	{0x44EA,		0xDD},
	{0x44EC,		0xDD},
	{0x44EE,		0xDD},
	{0x44F0,		0xDD},
	{0x44F2,		0xB2},
	{0x44F4,		0xB2},
	{0x44F6,		0xB2},
	{0x4538,		0x15},
	{0x4539,		0x15},
	{0x453A,		0x15},
	{0x4544,		0x15},
	{0x4545,		0x15},
	{0x4546,		0x15},
	{0x4550,		0x10},
	{0x4551,		0x10},
	{0x4552,		0x10},
	{0x4553,		0x10},
	{0x4554,		0x10},
	{0x4555,		0x10},
	{0x4556,		0x10},
	{0x4557,		0x10},
	{0x4558,		0x10},
	{0x455C,		0x10},
	{0x455D,		0x10},
	{0x455E,		0x10},
	{0x455F,		0x10},
	{0x4560,		0x10},
	{0x4561,		0x10},
	{0x4562,		0x10},
	{0x4563,		0x10},
	{0x4564,		0x10},
	{0x4604,		0x04},
	{0x4608,		0x22},
	{0x479C,		0x04},
	{0x47A0,		0x22},
	{0x4E3C,		0x07},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_3552x3556[] = {

	{WINMODE,		0x00},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{0x4498,		0x4C},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x49},

	{0x48A7,		0x01},
	{0x4EE8,		0x00},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_crop_3552x2160[] = {

	{WINMODE,		0x04},
	{ADDMODE,		0x00},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{0x4498,		0x4C},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x49},

	{0x48A7,		0x01},
	{0x4EE8,		0x00},

	{PIX_HST_LOW,		IMX676_TO_LOW_BYTE(0)},
	{PIX_HST_HIGH,		IMX676_TO_MID_BYTE(0)},
	{PIX_HWIDTH_LOW,	IMX676_TO_LOW_BYTE(IMX676_CROP_3552x2160_WIDTH)},
	{PIX_HWIDTH_HIGH,	IMX676_TO_MID_BYTE(IMX676_CROP_3552x2160_WIDTH)},

	{PIX_VST_HIGH,		IMX676_TO_MID_BYTE(698)},
	{PIX_VST_LOW,		IMX676_TO_LOW_BYTE(698)},
	{PIX_VWIDTH_HIGH,	IMX676_TO_MID_BYTE(IMX676_CROP_3552x2160_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX676_TO_LOW_BYTE(IMX676_CROP_3552x2160_HEIGHT)},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_h2v2_binning[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x01},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{0x355A,		0x00},

	{0x3C0A,		0x03},
	{0x3C0B,		0x03},
	{0x3C0C,		0x03},
	{0x3C0D,		0x03},
	{0x3C0E,		0x03},
	{0x3C0F,		0x03},

	{0x4498,		0x50},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x47},

	{0x48A7,		0x01},
	{0x4EE8,		0x00},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_crop_1768x1080[] = {

	{WINMODE,		0x04},
	{ADDMODE,		0x01},
	{WDMODE,		0x00},
	{VCMODE,		0x01},

	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{0x355A,		0x00},

	{0x3C0A,		0x03},
	{0x3C0B,		0x03},
	{0x3C0C,		0x03},
	{0x3C0D,		0x03},
	{0x3C0E,		0x03},
	{0x3C0F,		0x03},

	{0x4498,		0x50},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x47},

	{0x48A7,		0x01},
	{0x4EE8,		0x00},

	{PIX_HST_HIGH,		IMX676_TO_MID_BYTE(0)},
	{PIX_HST_LOW,		IMX676_TO_LOW_BYTE(0)},
	{PIX_HWIDTH_HIGH,	IMX676_TO_MID_BYTE(2 * IMX676_CROP_1768x1080_WIDTH)},
	{PIX_HWIDTH_LOW,	IMX676_TO_LOW_BYTE(2 * IMX676_CROP_1768x1080_WIDTH)},

	{PIX_VST_HIGH,		IMX676_TO_MID_BYTE(698)},
	{PIX_VST_LOW,		IMX676_TO_LOW_BYTE(698)},
	{PIX_VWIDTH_HIGH,	IMX676_TO_MID_BYTE(2 * IMX676_CROP_1768x1080_HEIGHT)},
	{PIX_VWIDTH_LOW,	IMX676_TO_LOW_BYTE(2 * IMX676_CROP_1768x1080_HEIGHT)},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_dol_hdr[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x00},
	{WDMODE,		0x01},

	{THIN_V_EN,		0x00},
	{VCMODE,		0x00},

	{SHR0_LOW,		0x00},
	{SHR0_MID,		0x10},
	{SHR1_LOW,		0x0A},
	{RHS1_LOW,		IMX676_TO_LOW_BYTE(IMX676_DEFAULT_RHS1)},
	{RHS1_MID,		IMX676_TO_MID_BYTE(IMX676_DEFAULT_RHS1)},
	{GAIN_PGC_FIDMD,	0x00},

	{0x4498,		0x4C},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x49},

	{0x48A7,		0x02},
	{0x4EE8,		0x01},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_dol_binning_hdr[] = {
	{WINMODE,		0x00},
	{ADDMODE,		0x01},
	{WDMODE,		0x01},

	{THIN_V_EN,		0x00},
	{VCMODE,		0x00},
	{ADBIT,			0x00},
	{MDBIT,			0x01},

	{SHR0_LOW,		0x38},
	{SHR0_MID,		0x01},
	{SHR1_LOW,		0x0A},
	{RHS1_LOW,		IMX676_TO_LOW_BYTE(IMX676_DEFAULT_RHS1)},
	{RHS1_MID,		IMX676_TO_MID_BYTE(IMX676_DEFAULT_RHS1)},
	{GAIN_PGC_FIDMD,	0x00},
	{0x355A,		0x09},

	{0x4498,		0x50},
	{0x449A,		0x4B},
	{0x449C,		0x4B},
	{0x449E,		0x47},

	{0x48A7,		0x02},
	{0x4EE8,		0x01},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_enable_pattern_generator[] = {

	{BLKLEVEL_LOW,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x0A},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

static const imx676_reg mode_disable_pattern_generator[] = {

	{BLKLEVEL_LOW,		0x32},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x00},
	{TESTCLKEN,		0x02},

	{IMX676_TABLE_WAIT_MS, IMX676_WAIT_MS},
	{IMX676_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {

	IMX676_MODE_3552x3556,
	IMX676_MODE_CROP_3552x2160,

	IMX676_MODE_H2V2_BINNING,
	IMX676_MODE_CROP_1768x1080,
	IMX676_MODE_DOL_HDR,
	IMX676_MODE_DOL_BINNING,

	IMX676_10BIT_MODE,
	IMX676_12BIT_MODE,

	IMX676_EN_PATTERN_GEN,
	IMX676_DIS_PATTERN_GEN,

	IMX676_INIT_SETTINGS,
	IMX676_MODE_START_STREAM,
	IMX676_MODE_STOP_STREAM,
};

typedef enum {

	IMX676_2376_MBPS,
	IMX676_2079_MBPS,
	IMX676_1782_MBPS,
	IMX676_1440_MBPS,
	IMX676_1188_MBPS,
	IMX676_891_MBPS,
	IMX676_720_MBPS,
	IMX676_594_MBPS,
} data_rate_mode;

/* Connecting frame modes to mode tables */
static const imx676_reg *mode_table[] = {

	[IMX676_MODE_3552x3556] = mode_3552x3556,
	[IMX676_MODE_CROP_3552x2160] = mode_crop_3552x2160,

	[IMX676_MODE_H2V2_BINNING] = mode_h2v2_binning,
	[IMX676_MODE_CROP_1768x1080] = mode_crop_1768x1080,
	[IMX676_MODE_DOL_HDR] = mode_dol_hdr,
	[IMX676_MODE_DOL_BINNING] = mode_dol_binning_hdr,

	[IMX676_EN_PATTERN_GEN] = mode_enable_pattern_generator,
	[IMX676_DIS_PATTERN_GEN] = mode_disable_pattern_generator,

	[IMX676_10BIT_MODE] = imx676_10bit_mode,
	[IMX676_12BIT_MODE] = imx676_12bit_mode,

	[IMX676_INIT_SETTINGS] = imx676_init_settings,

	[IMX676_MODE_START_STREAM] = imx676_start,
	[IMX676_MODE_STOP_STREAM] = imx676_stop,
};

/* Framerates of available frame modes */
static const int imx676_30fps[] = {
	30,
};

static const int imx676_60fps[] = {
	60,
};

static const int imx676_105fps[] = {
	105,
};

static const int imx676_205fps[] = {
	205,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx676_frmfmt[] = {
	{
		.size = {IMX676_DEFAULT_WIDTH, IMX676_DEFAULT_HEIGHT},
		.framerates = imx676_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX676_MODE_3552x3556
	},
	{
		.size = {IMX676_CROP_3552x2160_WIDTH,
						IMX676_CROP_3552x2160_HEIGHT},
		.framerates = imx676_105fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX676_MODE_CROP_3552x2160
	},
	{
		.size = {IMX676_MODE_BINNING_H2V2_WIDTH,
						IMX676_MODE_BINNING_H2V2_HEIGHT},
		.framerates = imx676_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX676_MODE_H2V2_BINNING
	},
	{
		.size = {IMX676_CROP_1768x1080_WIDTH,
						IMX676_CROP_1768x1080_HEIGHT},
		.framerates = imx676_205fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX676_MODE_CROP_1768x1080
	},
	{
		.size = {IMX676_DOL_HDR_WIDTH, IMX676_DOL_HDR_HEIGHT},
		.framerates = imx676_30fps,
		.num_framerates = 1,
		.hdr_en = true,
		.mode = IMX676_MODE_DOL_HDR
	},
	{
		.size = {IMX676_DOL_BINNING_HDR_WIDTH,
						IMX676_DOL_BINNING_HDR_HEIGHT},
		.framerates = imx676_30fps,
		.num_framerates = 1,
		.hdr_en = true,
		.mode = IMX676_MODE_DOL_BINNING
	}
};

#endif
