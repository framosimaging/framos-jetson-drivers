/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx900_mode_tbls.h - imx900 sensor mode tables
 *
 */

#ifndef __fr_IMX900_TABLES__
#define __fr_IMX900_TABLES__


#define STANDBY			0x3000
#define XMSTA			0x3010

#define INCKSEL_ST0		0x3014
#define INCKSEL_ST1		0x3015
#define INCKSEL_ST2		0x3016
#define INCKSEL_ST3		0x3017
#define INCKSEL_ST4		0x3018
#define INCKSEL_ST5		0x3019
#define INCKSEL_ST6		0x301C
#define INCKSEL_ST7		0x301D
#define HVMODE			0x303C
#define I2CSPICK		0x303A
#define FDG_SEL			0x30B4
#define VOPB_VBLK_HWID_LOW	0x30D0
#define VOPB_VBLK_HWID_HIGH	0x30D1
#define FINFO_HWIDTH_LOW	0x30D2
#define FINFO_HWIDTH_HIGH	0x30D3

#define VMAX_LOW		0x30D4
#define VMAX_MID		0x30D5
#define VMAX_HIGH		0x30D6
#define HMAX_LOW		0x30D8
#define HMAX_HIGH		0x30D9

#define GMRWT			0x30E2
#define GMTWT			0x30E3
#define GAINDLY			0x30E5
#define GSDLY			0x30E6
#define REGHOLD			0x30F8

#define ROI_MODE		0x3100
#define FID0_ROI		0x3104
#define FID0_ROIPH1_LOW		0x3120
#define FID0_ROIPH1_HIGH	0x3121
#define FID0_ROIPV1_LOW		0x3122
#define FID0_ROIPV1_HIGH	0x3123
#define FID0_ROIWH1_LOW		0x3124
#define FID0_ROIWH1_HIGH	0x3125
#define FID0_ROIWV1_LOW		0x3126
#define FID0_ROIWV1_HIGH	0x3127

#define ADBIT_MONOSEL		0x3200
#define HREVERSE_VREVERSE	0x3204

#define LLBLANK_LOW		0x323C
#define LLBLANK_HIGH		0x323D
#define VINT_EN			0x323E

#define SHS_LOW			0x3240
#define SHS_MID			0x3241
#define SHS_HIGH		0x3242

#define TRIGMODE		0x3400
#define ODBIT			0x3430
#define GPO0EXPSSEL_GPO1EXPSSEL	0x3436
#define GPO2EXPSSEL		0x3437

#define SYNCSEL			0x343C

#define GAIN_RTS		0x3502
#define GAIN_LOW		0x3514
#define GAIN_HIGH		0x3515
#define BLKLEVEL_LOW		0x35B4
#define BLKLEVEL_HIGH		0x35B5

#define IMX900_36A8		0x36A8
#define IMX900_36A9		0x36A9
#define GMRWT2			0x36E2
#define GMRWT3			0x36E3

#define LANESEL			0x3904
#define EAV_SEL_MIPI		0x3942
#define RD_REGHOLD		0x3B3E

#define BASECK_FREQ_LOW		0x3C98
#define BASECK_FREQ_HIGH	0x3C99
#define SCAL_INIT_EN		0x3CA3

#define THS_PREPARE_LOW		0x3CA8
#define THS_PREPARE_HIGH	0x3CA9
#define TCLK_POST_LOW		0x3CAA
#define TCLK_POST_HIGH		0x3CAB
#define THS_TRAIL_LOW		0x3CAC
#define THS_TRAIL_HIGH		0x3CAD
#define THS_ZERO_LOW		0x3CAE
#define THS_ZERO_HIGH		0x3CAF
#define TCLK_PREPARE_LOW	0x3CB0
#define TCLK_PREPARE_HIGH	0x3CB1
#define TCLK_TRAIL_LOW		0x3CB2
#define TCLK_TRAIL_HIGH		0x3CB3
#define TLPX_LOW		0x3CB4
#define TLPX_HIGH		0x3CB5
#define TCLK_ZERO_LOW		0x3CB6
#define TCLK_ZERO_HIGH		0x3CB7
#define TCLK_PRE_LOW		0x3CB8
#define TCLK_PRE_HIGH		0x3CB9
#define THS_EXIT_LOW		0x3CBA
#define THS_EXIT_HIGH		0x3CBB

#define INCKSEL_N0		0x4100
#define INCKSEL_N1		0x4101

#define INCKSEL_D0		0x4110
#define INCKSEL_D1		0x4111
#define INCKSEL_D2		0x4112
#define INCKSEL_D3		0x4116

#define INCKSEL_STB0		0x505C
#define INCKSEL_STB1		0x505D
#define INCKSEL_STB2		0x505E
#define INCKSEL_STB3		0x505F
#define INCKSEL_STB4		0x54D0
#define INCKSEL_STB5		0x54D1
#define INCKSEL_STB6		0x54D2
#define INCKSEL_STB7		0x54D3
#define INCKSEL_STB8		0x54D4
#define INCKSEL_STB9		0x54D5
#define INCKSEL_STB10		0x54D6
#define INCKSEL_STB11		0x5934
#define INCKSEL_STB12		0x5935
#define INCKSEL_STB13		0x5936
#define INCKSEL_STB14		0x5937
#define INCKSEL_STB15		0x59AC
#define INCKSEL_STB16		0x59AE
#define INCKSEL_STB17		0x59AF
#define INCKSEL_STB18		0x5BB8
#define INCKSEL_STB19		0x5BBA
#define INCKSEL_STB20		0x5BBC
#define INCKSEL_STB21		0x5BBD
#define INCKSEL_STB22		0x5BBE
#define INCKSEL_STB23		0x5BBF
#define INCKSEL_STB24		0x5BC0
#define INCKSEL_STB25		0x5BC1
#define INCKSEL_STB26		0x5BC2
#define INCKSEL_STB27		0x5BC3
#define INCKSEL_STB28		0x5BCC

#define CHROMACITY		 0x3817

/* Resolutions of implemented frame modes */
#define IMX900_DEFAULT_WIDTH			2064
#define IMX900_DEFAULT_HEIGHT			1552

#define IMX900_ROI_MODE_WIDTH			1920
#define IMX900_ROI_MODE_HEIGHT			1080

#define IMX900_SUBSAMPLING2_MODE_WIDTH		1032
#define IMX900_SUBSAMPLING2_MODE_HEIGHT		776

#define IMX900_SUBSAMPLING10_MODE_WIDTH		2064
#define IMX900_SUBSAMPLING10_MODE_HEIGHT	154

#define IMX900_BINNING_CROP_MODE_WIDTH		1024
#define IMX900_BINNING_CROP_MODE_HEIGHT		720


/* Special values for the write table function */
#define IMX900_TABLE_WAIT_MS	0
#define IMX900_TABLE_END	1
#define IMX900_WAIT_MS		10

#define IMX900_MIN_FRAME_DELTA	137

#define IMX900_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX900_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx900_reg;


/* Tables for the write table function */
static const imx900_reg imx900_stop[] = {

	{STANDBY, 0x01},
	{IMX900_TABLE_WAIT_MS, 20},
	{XMSTA, 0x01},

	{IMX900_TABLE_WAIT_MS, 30},
	{IMX900_TABLE_END, 0x00}
};


static const imx900_reg imx900_8bit_mode[] = {

	{ODBIT,		0x02},

	{0x5572,	0x5F},
	{0x5613,	0xAF},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x00}
};

static const imx900_reg imx900_10bit_mode[] = {

	{ODBIT,		0x00},

	{0x5572,	0x5F},
	{0x5613,	0xAF},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x00}
};

static const imx900_reg imx900_12bit_mode[] = {

	{ODBIT,		0x01},

	{0x5572,	0x1F},
	{0x5613,	0x8F},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x00}
};


static const imx900_reg imx900_2376_mbps[] = {

	{SCAL_INIT_EN,		0x01},
	{THS_PREPARE_LOW,	0x9F},
	{THS_PREPARE_HIGH,	0x00},
	{TCLK_POST_LOW,		0xEF},
	{TCLK_POST_HIGH,	0x00},
	{THS_TRAIL_LOW,		0x97},
	{THS_TRAIL_HIGH,	0x00},
	{THS_ZERO_LOW,		0x0F},
	{THS_ZERO_HIGH,		0x01},
	{TCLK_PREPARE_LOW,	0x8F},
	{TCLK_PREPARE_HIGH,	0x00},
	{TCLK_TRAIL_LOW,	0x97},
	{TCLK_TRAIL_HIGH,	0x00},
	{TLPX_LOW,		0x7F},
	{TLPX_HIGH,		0x00},
	{TCLK_ZERO_LOW,		0x8F},
	{TCLK_ZERO_HIGH,	0x02},
	{THS_EXIT_LOW,		0xFF},
	{THS_EXIT_HIGH,		0x00},

	{INCKSEL_D1,		0x08},
	{INCKSEL_D2,		0x0C},
	{INCKSEL_D3,		0xD8},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg imx900_1485_mbps[] = {

	{SCAL_INIT_EN,		0x00},
	{THS_PREPARE_LOW,	0x5F},
	{THS_PREPARE_HIGH,	0x00},
	{TCLK_POST_LOW,		0xAF},
	{TCLK_POST_HIGH,	0x00},
	{THS_TRAIL_LOW,		0x5F},
	{THS_TRAIL_HIGH,	0x00},
	{THS_ZERO_LOW,		0xAF},
	{THS_ZERO_HIGH,		0x00},
	{TCLK_PREPARE_LOW,	0x5F},
	{TCLK_PREPARE_HIGH,	0x00},
	{TCLK_TRAIL_LOW,	0x5F},
	{TCLK_TRAIL_HIGH,	0x00},
	{TLPX_LOW,		0x4F},
	{TLPX_HIGH,		0x00},
	{TCLK_ZERO_LOW,		0x9F},
	{TCLK_ZERO_HIGH,	0x01},
	{THS_EXIT_LOW,		0x9F},
	{THS_EXIT_HIGH,		0x00},

	{INCKSEL_D1,		0x8A},
	{INCKSEL_D2,		0x0C},
	{INCKSEL_D3,		0xD8},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg imx900_1188_mbps[] = {

	{SCAL_INIT_EN,		0x00},
	{THS_PREPARE_LOW,	0x4F},
	{THS_PREPARE_HIGH,	0x00},
	{TCLK_POST_LOW,		0x9F},
	{TCLK_POST_HIGH,	0x00},
	{THS_TRAIL_LOW,		0x4F},
	{THS_TRAIL_HIGH,	0x00},
	{THS_ZERO_LOW,		0x9F},
	{THS_ZERO_HIGH,		0x00},
	{TCLK_PREPARE_LOW,	0x4F},
	{TCLK_PREPARE_HIGH,	0x00},
	{TCLK_TRAIL_LOW,	0x4F},
	{TCLK_TRAIL_HIGH,	0x00},
	{TLPX_LOW,		0x3F},
	{TLPX_HIGH,		0x00},
	{TCLK_ZERO_LOW,		0x4F},
	{TCLK_ZERO_HIGH,	0x01},
	{THS_EXIT_LOW,		0x7F},
	{THS_EXIT_HIGH,		0x00},

	{INCKSEL_D1,		0x88},
	{INCKSEL_D2,		0x0C},
	{INCKSEL_D3,		0xD8},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg imx900_891_mbps[] = {

	{SCAL_INIT_EN,		0x00},
	{THS_PREPARE_LOW,	0x3F},
	{THS_PREPARE_HIGH,	0x00},
	{TCLK_POST_LOW,		0x7F},
	{TCLK_POST_HIGH,	0x00},
	{THS_TRAIL_LOW,		0x3F},
	{THS_TRAIL_HIGH,	0x00},
	{THS_ZERO_LOW,		0x7F},
	{THS_ZERO_HIGH,		0x00},
	{TCLK_PREPARE_LOW,	0x3F},
	{TCLK_PREPARE_HIGH,	0x00},
	{TCLK_TRAIL_LOW,	0x3F},
	{TCLK_TRAIL_HIGH,	0x00},
	{TLPX_LOW,		0x3F},
	{TLPX_HIGH,		0x00},
	{TCLK_ZERO_LOW,		0x0F},
	{TCLK_ZERO_HIGH,	0x01},
	{THS_EXIT_LOW,		0x6F},
	{THS_EXIT_HIGH,		0x00},

	{INCKSEL_D1,		0x0C},
	{INCKSEL_D2,		0x0D},
	{INCKSEL_D3,		0xC8},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg imx900_594_mbps[] = {

	{SCAL_INIT_EN,		0x00},
	{THS_PREPARE_LOW,	0x2F},
	{THS_PREPARE_HIGH,	0x00},
	{TCLK_POST_LOW,		0x6F},
	{TCLK_POST_HIGH,	0x00},
	{THS_TRAIL_LOW,		0x27},
	{THS_TRAIL_HIGH,	0x00},
	{THS_ZERO_LOW,		0x3F},
	{THS_ZERO_HIGH,		0x00},
	{TCLK_PREPARE_LOW,	0x27},
	{TCLK_PREPARE_HIGH,	0x00},
	{TCLK_TRAIL_LOW,	0x27},
	{TCLK_TRAIL_HIGH,	0x00},
	{TLPX_LOW,		0x1F},
	{TLPX_HIGH,		0x00},
	{TCLK_ZERO_LOW,		0x9F},
	{TCLK_ZERO_HIGH,	0x00},
	{THS_EXIT_LOW,		0x3F},
	{THS_EXIT_HIGH,		0x00},

	{INCKSEL_D1,		0x08},
	{INCKSEL_D2,		0x0D},
	{INCKSEL_D3,		0xD8},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg imx900_init_settings[] = {

	/* 37.125MHz */
	{INCKSEL_ST0,		0x1E},
	{INCKSEL_ST1,		0x92},
	{INCKSEL_ST2,		0xE0},
	{INCKSEL_ST3,		0x01},
	{INCKSEL_ST4,		0xB6},
	{INCKSEL_ST5,		0x00},
	{INCKSEL_ST6,		0xB6},
	{INCKSEL_ST7,		0x00},
	{I2CSPICK,		0x15},
	{BASECK_FREQ_LOW,	0x80},
	{BASECK_FREQ_HIGH,	0x09},
	{INCKSEL_N0,		0x02},
	{INCKSEL_N1,		0x07},
	{INCKSEL_D0,		0x02},

	{INCKSEL_STB0,		0x96},
	{INCKSEL_STB1,		0x02},
	{INCKSEL_STB2,		0x96},
	{INCKSEL_STB3,		0x02},
	{INCKSEL_STB4,		0x40},
	{INCKSEL_STB5,		0x01},
	{INCKSEL_STB6,		0x81},
	{INCKSEL_STB7,		0x01},
	{INCKSEL_STB8,		0x15},
	{INCKSEL_STB9,		0x01},
	{INCKSEL_STB10,		0x00},
	{INCKSEL_STB11,		0x96},
	{INCKSEL_STB12,		0x02},
	{INCKSEL_STB13,		0x96},
	{INCKSEL_STB14,		0x02},
	{INCKSEL_STB15,		0x00},
	{INCKSEL_STB16,		0x56},
	{INCKSEL_STB17,		0x01},
	{INCKSEL_STB18,		0x5C},
	{INCKSEL_STB19,		0x3A},
	{INCKSEL_STB20,		0xC5},
	{INCKSEL_STB21,		0x00},
	{INCKSEL_STB22,		0x0B},
	{INCKSEL_STB23,		0x02},
	{INCKSEL_STB24,		0x74},
	{INCKSEL_STB25,		0x02},
	{INCKSEL_STB26,		0x90},
	{INCKSEL_STB27,		0x01},
	{INCKSEL_STB28,		0x00},


	/* Global Timing */
	{TCLK_PRE_LOW,		0x0F},
	{TCLK_PRE_HIGH,		0x00},

	/* init */
	{EAV_SEL_MIPI,		0x03},

	{GAIN_RTS,		0x09},

	{0x32B6,		0x3A},
	{0x3312,		0x39},

	{0x34D4,		0x78},
	{0x34D5,		0x27},
	{0x34D8,		0xA9},
	{0x34D9,		0x5A},
	{0x34F9,		0x12},

	{0x3528,		0x00},
	{0x352A,		0x00},
	{0x352C,		0x00},
	{0x352E,		0x00},
	{0x3542,		0x03},
	{0x3549,		0x2A},
	{0x354A,		0x20},
	{0x354B,		0x0C},
	{0x359C,		0x19},
	{0x359E,		0x3F},
	{0x35EA,		0xF0},
	{0x35F4,		0x03},
	{0x35F8,		0x01},

	{0x3600,		0x00},
	{0x3614,		0x00},
	{0x362A,		0xEC},
	{0x362B,		0x1F},
	{0x362E,		0xF8},
	{0x362F,		0x1F},
	{0x3630,		0x5C},
	{0x3648,		0xC6},
	{0x364A,		0xEC},
	{0x364B,		0x1F},
	{0x364C,		0xDE},
	{0x364E,		0xF8},
	{0x364F,		0x1F},
	{0x3652,		0xEC},
	{0x3653,		0x1F},
	{0x3656,		0xF8},
	{0x3657,		0x1F},
	{0x3658,		0x5C},
	{0x3670,		0xC6},
	{0x3672,		0xEC},
	{0x3673,		0x1F},
	{0x3674,		0xDE},
	{0x3676,		0xF8},
	{0x3677,		0x1F},
	{0x367A,		0xEC},
	{0x367B,		0x1F},
	{0x367E,		0xF8},
	{0x367F,		0x1F},
	{0x3698,		0xC6},
	{0x369A,		0xEC},
	{0x369B,		0x1F},
	{0x369C,		0xDE},
	{0x369E,		0xF8},
	{0x369F,		0x1F},
	{0x36B0,		0x28},
	{0x36B1,		0x00},
	{0x36B2,		0xF8},
	{0x36B3,		0x1F},
	{0x36BC,		0x28},
	{0x36BD,		0x00},
	{0x36BE,		0xF8},
	{0x36BF,		0x1F},
	{0x36D4,		0xEF},
	{0x36D5,		0x01},
	{0x36D6,		0x94},
	{0x36D7,		0x03},
	{0x36D8,		0xEF},
	{0x36D9,		0x01},
	{0x36DA,		0x94},
	{0x36DB,		0x03},
	{0x36DC,		0x9B},
	{0x36DD,		0x09},
	{0x36DE,		0x57},
	{0x36DF,		0x11},
	{0x36E0,		0xEB},
	{0x36E1,		0x17},

	{0x37AC,		0x0E},
	{0x37AE,		0x14},

	{0x38E8,		0x82},

	{0x5032,		0xFF},
	{0x5038,		0x00},
	{0x5039,		0x00},
	{0x503A,		0xF6},
	{0x5078,		0x09},
	{0x507B,		0x11},
	{0x507C,		0xFF},

	{0x531C,		0x48},
	{0x531E,		0x52},
	{0x5320,		0x48},
	{0x5322,		0x52},
	{0x5324,		0x48},
	{0x5326,		0x52},
	{0x5328,		0x48},
	{0x532A,		0x52},
	{0x532C,		0x48},
	{0x532E,		0x52},
	{0x5330,		0x48},
	{0x5332,		0x52},
	{0x5334,		0x48},
	{0x5336,		0x52},
	{0x5338,		0x48},
	{0x533A,		0x52},

	{0x54D6,		0x00},

	{0x5545,		0xA7},
	{0x5546,		0x14},
	{0x5547,		0x14},
	{0x5548,		0x14},
	{0x5550,		0x0A},
	{0x5551,		0x0A},
	{0x5552,		0x0A},
	{0x5553,		0x6A},
	{0x5589,		0x0E},

	{0x5704,		0x0E},
	{0x5705,		0x14},

	{0x5832,		0x54},
	{0x5836,		0x54},
	{0x583A,		0x54},
	{0x583E,		0x54},
	{0x5842,		0x54},
	{0x5846,		0x54},
	{0x584A,		0x54},
	{0x584E,		0x54},
	{0x5852,		0x54},
	{0x5856,		0x54},
	{0x585A,		0x54},
	{0x585E,		0x54},
	{0x5862,		0x54},
	{0x5866,		0x54},
	{0x586A,		0x54},
	{0x586E,		0x54},
	{0x5872,		0x54},
	{0x5876,		0x54},
	{0x587A,		0x54},
	{0x587E,		0x54},
	{0x5882,		0x54},
	{0x5886,		0x54},
	{0x588A,		0x54},
	{0x588E,		0x54},

	{0x5902,		0xB0},
	{0x5903,		0x04},
	{0x590A,		0xB0},
	{0x590B,		0x04},
	{0x590C,		0xB0},
	{0x590D,		0x09},
	{0x590E,		0xC4},
	{0x590F,		0x09},
	{0x5939,		0x08},
	{0x59AC,		0x00},
	{0x59C1,		0x00},
	{0x59D4,		0x00},

	{0x5B4D,		0x24},
	{0x5B81,		0x36},
	{0x5BB5,		0x09},
	{0x5BC9,		0x11},
	{0x5BCC,		0x00},
	{0x5BD8,		0x00},
	{0x5BD9,		0x00},
	{0x5BDC,		0x1D},
	{0x5BDD,		0x00},
	{0x5BE0,		0x1E},
	{0x5BE1,		0x00},
	{0x5BE4,		0x3B},
	{0x5BE5,		0x00},
	{0x5BE8,		0x3C},
	{0x5BE9,		0x00},
	{0x5BEC,		0x59},
	{0x5BED,		0x00},
	{0x5BF0,		0x5A},
	{0x5BF1,		0x00},
	{0x5BF4,		0x77},
	{0x5BF5,		0x00},

	{0x5C00,		0x00},

	{0x5E04,		0x13},
	{0x5E05,		0x05},
	{0x5E06,		0x02},
	{0x5E07,		0x00},
	{0x5E14,		0x14},
	{0x5E15,		0x05},
	{0x5E16,		0x01},
	{0x5E17,		0x00},
	{0x5E34,		0x08},
	{0x5E35,		0x05},
	{0x5E36,		0x02},
	{0x5E37,		0x00},
	{0x5E44,		0x09},
	{0x5E45,		0x05},
	{0x5E46,		0x01},
	{0x5E47,		0x00},
	{0x5E98,		0x7C},
	{0x5E99,		0x09},
	{0x5EB8,		0x7E},
	{0x5EB9,		0x09},
	{0x5EC8,		0x18},
	{0x5EC9,		0x09},
	{0x5ECA,		0xE8},
	{0x5ECB,		0x03},
	{0x5ED8,		0x1A},
	{0x5ED9,		0x09},
	{0x5EDA,		0xE6},
	{0x5EDB,		0x03},

	{0x5F08,		0x18},
	{0x5F09,		0x09},
	{0x5F0A,		0xE8},
	{0x5F0B,		0x03},
	{0x5F18,		0x1A},
	{0x5F19,		0x09},
	{0x5F1A,		0xE6},
	{0x5F1B,		0x03},
	{0x5F38,		0x18},
	{0x5F39,		0x09},
	{0x5F3A,		0xE8},
	{0x5F3B,		0x03},
	{0x5F48,		0x1A},
	{0x5F49,		0x09},
	{0x5F4A,		0xE6},
	{0x5F4B,		0x03},
	{0x5F68,		0x18},
	{0x5F69,		0x09},
	{0x5F6A,		0xE8},
	{0x5F6B,		0x03},
	{0x5F78,		0x1A},
	{0x5F79,		0x09},
	{0x5F7A,		0xE6},
	{0x5F7B,		0x03},

	{0x60B4,		0x1E},
	{0x60C0,		0x1F},

	{0x6178,		0x7C},
	{0x6179,		0x09},
	{0x6198,		0x7E},
	{0x6199,		0x09},

	{0x6278,		0x18},
	{0x6279,		0x09},
	{0x627A,		0xE8},
	{0x627B,		0x03},
	{0x6288,		0x1A},
	{0x6289,		0x09},
	{0x628A,		0xE6},
	{0x628B,		0x03},
	{0x62A8,		0x18},
	{0x62A9,		0x09},
	{0x62AA,		0xE8},
	{0x62AB,		0x03},
	{0x62B8,		0x1A},
	{0x62B9,		0x09},
	{0x62BA,		0xE6},
	{0x62BB,		0x03},
	{0x62D8,		0x18},
	{0x62D9,		0x09},
	{0x62DA,		0xE8},
	{0x62DB,		0x03},
	{0x62E8,		0x1A},
	{0x62E9,		0x09},
	{0x62EA,		0xE6},
	{0x62EB,		0x03},

	{0x6318,		0x18},
	{0x6319,		0x09},
	{0x631A,		0xE8},
	{0x631B,		0x03},
	{0x6328,		0x1A},
	{0x6329,		0x09},
	{0x632A,		0xE6},
	{0x632B,		0x03},
	{0x6398,		0x1E},
	{0x63A4,		0x1F},

	{0x6501,		0x01},
	{0x6505,		0x00},
	{0x6508,		0x00},
	{0x650C,		0x01},
	{0x6510,		0x00},
	{0x6514,		0x01},
	{0x6519,		0x01},
	{0x651D,		0x00},
	{0x6528,		0x00},
	{0x652C,		0x01},
	{0x6531,		0x01},
	{0x6535,		0x00},
	{0x6538,		0x00},
	{0x653C,		0x01},
	{0x6541,		0x01},
	{0x6545,		0x00},
	{0x6549,		0x01},
	{0x654D,		0x00},
	{0x6558,		0x00},
	{0x655C,		0x01},
	{0x6560,		0x00},
	{0x6564,		0x01},
	{0x6571,		0x01},
	{0x6575,		0x00},
	{0x6579,		0x01},
	{0x657D,		0x00},
	{0x6588,		0x00},
	{0x658C,		0x01},
	{0x6590,		0x00},
	{0x6594,		0x01},
	{0x6598,		0x00},
	{0x659C,		0x01},
	{0x65A0,		0x00},
	{0x65A4,		0x01},
	{0x65B0,		0x00},
	{0x65B4,		0x01},
	{0x65B9,		0x00},
	{0x65BD,		0x00},
	{0x65C1,		0x00},
	{0X65C9,		0x00},
	{0x65CC,		0x00},
	{0x65D0,		0x00},
	{0x65D4,		0x00},
	{0x65DC,		0x00},


	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* All-pixel mode */
static const imx900_reg mode_2064x1552[] = {

	{HVMODE,	0x00},

	{VOPB_VBLK_HWID_LOW,	IMX900_TO_LOW_BYTE(2064)},
	{VOPB_VBLK_HWID_HIGH,	IMX900_TO_MID_BYTE(2064)},
	{FINFO_HWIDTH_LOW,	IMX900_TO_LOW_BYTE(2064)},
	{FINFO_HWIDTH_HIGH,	IMX900_TO_MID_BYTE(2064)},

	{FID0_ROI,	0x00},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};


/* ROI mode */
static const imx900_reg mode_1920x1080[] = {

	{HVMODE,	0x00},

	{VOPB_VBLK_HWID_LOW,	IMX900_TO_LOW_BYTE(1920)},
	{VOPB_VBLK_HWID_HIGH,	IMX900_TO_MID_BYTE(1920)},
	{FINFO_HWIDTH_LOW,	IMX900_TO_LOW_BYTE(1920)},
	{FINFO_HWIDTH_HIGH,	IMX900_TO_MID_BYTE(1920)},

	{FID0_ROI,	0x03},

	{FID0_ROIPH1_LOW,	IMX900_TO_LOW_BYTE(72)},
	{FID0_ROIPH1_HIGH,	IMX900_TO_MID_BYTE(72)},
	{FID0_ROIPV1_LOW,	IMX900_TO_LOW_BYTE(240)},
	{FID0_ROIPV1_HIGH,	IMX900_TO_MID_BYTE(240)},

	{FID0_ROIWH1_LOW,	IMX900_TO_LOW_BYTE(IMX900_ROI_MODE_WIDTH)},
	{FID0_ROIWH1_HIGH,	IMX900_TO_MID_BYTE(IMX900_ROI_MODE_WIDTH)},

	{FID0_ROIWV1_LOW,	IMX900_TO_LOW_BYTE(IMX900_ROI_MODE_HEIGHT)},
	{FID0_ROIWV1_HIGH,	IMX900_TO_MID_BYTE(IMX900_ROI_MODE_HEIGHT)},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};


/* Subsampling 1/2 mode */
static const imx900_reg mode_1032x776[] = {

	{HVMODE,	0x08},

	{VOPB_VBLK_HWID_LOW,	IMX900_TO_LOW_BYTE(1032)},
	{VOPB_VBLK_HWID_HIGH,	IMX900_TO_MID_BYTE(1032)},
	{FINFO_HWIDTH_LOW,	IMX900_TO_LOW_BYTE(1032)},
	{FINFO_HWIDTH_HIGH,	IMX900_TO_MID_BYTE(1032)},

	{FID0_ROI,	0x00},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};


/* Subsampling 1/10 mode */
static const imx900_reg mode_2064x154[] = {

	{HVMODE,	0x18},

	{VOPB_VBLK_HWID_LOW,	IMX900_TO_LOW_BYTE(2064)},
	{VOPB_VBLK_HWID_HIGH,	IMX900_TO_MID_BYTE(2064)},
	{FINFO_HWIDTH_LOW,	IMX900_TO_LOW_BYTE(2064)},
	{FINFO_HWIDTH_HIGH,	IMX900_TO_MID_BYTE(2064)},

	{FID0_ROI,	0x00},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* Binning-crop mode */
static const imx900_reg mode_1024x720[] = {

	{HVMODE,	0x10},

	{VOPB_VBLK_HWID_LOW,	IMX900_TO_LOW_BYTE(1024)},
	{VOPB_VBLK_HWID_HIGH,	IMX900_TO_MID_BYTE(1024)},
	{FINFO_HWIDTH_LOW,	IMX900_TO_LOW_BYTE(1024)},
	{FINFO_HWIDTH_HIGH,	IMX900_TO_MID_BYTE(1024)},

	{FID0_ROI,	0x03},

	{FID0_ROIPH1_LOW,	IMX900_TO_LOW_BYTE(8)},
	{FID0_ROIPH1_HIGH,	IMX900_TO_MID_BYTE(8)},
	{FID0_ROIPV1_LOW,	IMX900_TO_LOW_BYTE(32)},
	{FID0_ROIPV1_HIGH,	IMX900_TO_MID_BYTE(32)},

	{FID0_ROIWH1_LOW,	IMX900_TO_LOW_BYTE(IMX900_BINNING_CROP_MODE_WIDTH)},
	{FID0_ROIWH1_HIGH,	IMX900_TO_MID_BYTE(IMX900_BINNING_CROP_MODE_WIDTH)},

	{FID0_ROIWV1_LOW,	IMX900_TO_LOW_BYTE(IMX900_BINNING_CROP_MODE_HEIGHT)},
	{FID0_ROIWV1_HIGH,	IMX900_TO_MID_BYTE(IMX900_BINNING_CROP_MODE_HEIGHT)},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* all pixel and roi mode common for color and mono */
static const imx900_reg mode_allPixel_roi[] = {

	{LLBLANK_LOW,	0x0F},
	{LLBLANK_HIGH,	0x00},
	{VINT_EN,	0x1F},

	{0x3521,	0x1A},
	{0x3546,	0x06},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg mode_subsampling2_color[] = {

	{LLBLANK_LOW,	0x0B},
	{LLBLANK_HIGH,	0x00},
	{VINT_EN,	0x17},

	{0x3521,	0x0E},
	{0x3546,	0x03},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1/2 subsampling for mono and binning for mono settings are common */
static const imx900_reg mode_subsampling2_binning_mono[] = {

	{LLBLANK_LOW,	0x0D},
	{LLBLANK_HIGH,	0x00},
	{VINT_EN,	0x1B},

	{0x3521,	0x0E},
	{0x3546,	0x03},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1/10 subsampling settings common for color and mono */
static const imx900_reg mode_subsampling10[] = {

	{LLBLANK_LOW,	0x0B},
	{LLBLANK_HIGH,	0x00},
	{VINT_EN,	0x17},

	{0x3521,	0x1A},
	{0x3546,	0x06},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/*
 * allpixel mode, roi mode, 1/10 subsampling mode are common for color and monochrome
 * 2376MBPS, 8bit
 */
static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x8_1lane[] = {

	{GMRWT,		0x13},
	{GMTWT,		0x2A},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x13},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0D},
	{GMRWT3,	0x19},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x8_2lane[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x8_4lane[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 10bit */
static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x10_1lane[] = {

	{GMRWT,		0x0F},
	{GMTWT,		0x22},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0F},
	{IMX900_36A9,	0x1A},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x15},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x10_2lane[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x10_4lane[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 12bit */
static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x12_1lane[] = {

	{GMRWT,		0x0D},
	{GMTWT,		0x1D},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0D},
	{IMX900_36A9,	0x16},
	{GMRWT2,	0x09},
	{GMRWT3,	0x12},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x12_2lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_2376MBPS_1x12_4lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 8bit */
static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x8_1lane[] = {

	{GMRWT,		0x0C},
	{GMTWT,		0x1B},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0C},
	{IMX900_36A9,	0x15},
	{GMRWT2,	0x09},
	{GMRWT3,	0x10},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x8_2lane[] = {

	{GMRWT,		0x16},
	{GMTWT,		0x33},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x16},
	{IMX900_36A9,	0x27},
	{GMRWT2,	0x11},
	{GMRWT3,	0x1F},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x8_4lane[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 10bit */
static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x10_1lane[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x11},
	{GMRWT2,	0x07},
	{GMRWT3,	0x0D},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x10_2lane[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x2A},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x1A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x10_4lane[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 12bit */
static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x12_1lane[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x13},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0E},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x12_2lane[] = {

	{GMRWT,		0x10},
	{GMTWT,		0x23},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x10},
	{IMX900_36A9,	0x1B},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x15},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1485MBPS_1x12_4lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 8bit */
static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x8_1lane[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x11},
	{GMRWT2,	0x07},
	{GMRWT3,	0x0D},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x8_2lane[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x29},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x1F},
	{GMRWT2,	0x0D},
	{GMRWT3,	0x19},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x8_4lane[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 10bit */
static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x10_1lane[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x12},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0E},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x10_2lane[] = {

	{GMRWT,		0x0F},
	{GMTWT,		0x22},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0F},
	{IMX900_36A9,	0x1A},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x10_4lane[] = {

	{GMRWT,		0x1B},
	{GMTWT,		0x3E},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1B},
	{IMX900_36A9,	0x2F},
	{GMRWT2,	0x14},
	{GMRWT3,	0x26},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS 12bit*/
static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x12_1lane[] = {

	{GMRWT,		0x07},
	{GMTWT,		0x0F},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x07},
	{IMX900_36A9,	0x0C},
	{GMRWT2,	0x05},
	{GMRWT3,	0x09},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x12_2lane[] = {

	{GMRWT,		0x0D},
	{GMTWT,		0x1C},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0D},
	{IMX900_36A9,	0x16},
	{GMRWT2,	0x09},
	{GMRWT3,	0x11},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_1188MBPS_1x12_4lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 8bit */
static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x8_1lane[] = {

	{GMRWT,		0x07},
	{GMTWT,		0x11},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x07},
	{IMX900_36A9,	0x0D},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x8_2lane[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x1F},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x18},
	{GMRWT2,	0x0A},
	{GMRWT3,	0x13},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x8_4lane[] = {

	{GMRWT,		0x19},
	{GMTWT,		0x39},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x19},
	{IMX900_36A9,	0x2B},
	{GMRWT2,	0x12},
	{GMRWT3,	0x23},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 10bit */
static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x10_1lane[] = {

	{GMRWT,		0x06},
	{GMTWT,		0x0D},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x06},
	{IMX900_36A9,	0x0A},
	{GMRWT2,	0x04},
	{GMRWT3,	0x08},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x10_2lane[] = {

	{GMRWT,		0x0B},
	{GMTWT,		0x1A},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0B},
	{IMX900_36A9,	0x14},
	{GMRWT2,	0x09},
	{GMRWT3,	0x10},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x10_4lane[] = {

	{GMRWT,		0x15},
	{GMTWT,		0x2F},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x15},
	{IMX900_36A9,	0x24},
	{GMRWT2,	0x0F},
	{GMRWT3,	0x1C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 12bit */
static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x12_1lane[] = {

	{GMRWT,		0x05},
	{GMTWT,		0x0B},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x05},
	{IMX900_36A9,	0x09},
	{GMRWT2,	0x04},
	{GMRWT3,	0x07},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x12_2lane[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x11},
	{GMRWT2,	0x07},
	{GMRWT3,	0x0D},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_891MBPS_1x12_4lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 8bit */
static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x8_1lane[] = {

	{GMRWT,		0x05},
	{GMTWT,		0x0B},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x05},
	{IMX900_36A9,	0x09},
	{GMRWT2,	0x04},
	{GMRWT3,	0x07},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x8_2lane[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x15},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x10},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x8_4lane[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1E},
	{GMRWT2,	0x0D},
	{GMRWT3,	0x18},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 10bit */
static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x10_1lane[] = {

	{GMRWT,		0x04},
	{GMTWT,		0x09},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x04},
	{IMX900_36A9,	0x07},
	{GMRWT2,	0x03},
	{GMRWT3,	0x06},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x10_2lane[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x11},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0D},
	{GMRWT2,	0x05},
	{GMRWT3,	0x0A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x10_4lane[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x20},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x19},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 12bit */
static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x12_1lane[] = {

	{GMRWT,		0x04},
	{GMTWT,		0x08},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x04},
	{IMX900_36A9,	0x06},
	{GMRWT2,	0x02},
	{GMRWT3,	0x04},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x12_2lane[] = {

	{GMRWT,		0x07},
	{GMTWT,		0x0F},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x07},
	{IMX900_36A9,	0x0B},
	{GMRWT2,	0x04},
	{GMRWT3,	0x08},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg allpixel_roi_subsampling10_594MBPS_1x12_4lane[] = {

	{GMRWT,		0x0C},
	{GMTWT,		0x1B},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0C},
	{IMX900_36A9,	0x15},
	{GMRWT2,	0x09},
	{GMRWT3,	0x11},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/*
 * 1/2 subsampling for color sensor
 * 2376MBPS, 8bit
 */
static const imx900_reg subsampling2_COLOR_2376MBPS_1x8_1LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x8_2LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 10bit */
static const imx900_reg subsampling2_COLOR_2376MBPS_1x10_1LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x10_2LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x10_4LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 12bit */
static const imx900_reg subsampling2_COLOR_2376MBPS_1x12_1LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x12_2LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_2376MBPS_1x12_4LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 8bit */
static const imx900_reg subsampling2_COLOR_1485MBPS_1x8_1LANE[] = {

	{GMRWT,		0x16},
	{GMTWT,		0x33},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x16},
	{IMX900_36A9,	0x27},
	{GMRWT2,	0x11},
	{GMRWT3,	0x1F},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x8_2LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 10bit */
static const imx900_reg subsampling2_COLOR_1485MBPS_1x10_1LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x2A},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x1A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x10_2LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x10_4LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 12bit */
static const imx900_reg subsampling2_COLOR_1485MBPS_1x12_1LANE[] = {

	{GMRWT,		0x10},
	{GMTWT,		0x23},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x10},
	{IMX900_36A9,	0x1B},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x15},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x12_2LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1485MBPS_1x12_4LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 8bit */
static const imx900_reg subsampling2_COLOR_1188MBPS_1x8_1LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x29},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x1F},
	{GMRWT2,	0x0D},
	{GMRWT3,	0x19},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x8_2LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 10bit */
static const imx900_reg subsampling2_COLOR_1188MBPS_1x10_1LANE[] = {

	{GMRWT,		0x0F},
	{GMTWT,		0x22},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0F},
	{IMX900_36A9,	0x1A},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x10_2LANE[] = {

	{GMRWT,		0x1B},
	{GMTWT,		0x3E},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1B},
	{IMX900_36A9,	0x2F},
	{GMRWT2,	0x14},
	{GMRWT3,	0x26},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x10_4LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 12bit */
static const imx900_reg subsampling2_COLOR_1188MBPS_1x12_1LANE[] = {

	{GMRWT,		0x0D},
	{GMTWT,		0x1C},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0D},
	{IMX900_36A9,	0x16},
	{GMRWT2,	0x09},
	{GMRWT3,	0x11},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x12_2LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_1188MBPS_1x12_4LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 8bit */
static const imx900_reg subsampling2_COLOR_891MBPS_1x8_1LANE[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x1F},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x18},
	{GMRWT2,	0x0A},
	{GMRWT3,	0x13},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_891MBPS_1x8_2LANE[] = {

	{GMRWT,		0x19},
	{GMTWT,		0x39},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x19},
	{IMX900_36A9,	0x2B},
	{GMRWT2,	0x12},
	{GMRWT3,	0x23},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};


static const imx900_reg subsampling2_COLOR_891MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x45},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x35},
	{GMRWT2,	0x17},
	{GMRWT3,	0x2B},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 10bit */
static const imx900_reg subsampling2_COLOR_891MBPS_1x10_1LANE[] = {

	{GMRWT,		0x0B},
	{GMTWT,		0x1A},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0B},
	{IMX900_36A9,	0x14},
	{GMRWT2,	0x09},
	{GMRWT3,	0x10},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_891MBPS_1x10_2LANE[] = {

	{GMRWT,		0x15},
	{GMTWT,		0x2F},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x15},
	{IMX900_36A9,	0x24},
	{GMRWT2,	0x0F},
	{GMRWT3,	0x1C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_891MBPS_1x10_4LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x40},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x31},
	{GMRWT2,	0x15},
	{GMRWT3,	0x27},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 12bit */
static const imx900_reg subsampling2_COLOR_891MBPS_1x12_1LANE[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x11},
	{GMRWT2,	0x07},
	{GMRWT3,	0x0D},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_891MBPS_1x12_2LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_891MBPS_1x12_4LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 8bit */
static const imx900_reg subsampling2_COLOR_594MBPS_1x8_1LANE[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x15},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x10},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x8_2LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1E},
	{GMRWT2,	0x0D},
	{GMRWT3,	0x18},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x44},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x34},
	{GMRWT2,	0x16},
	{GMRWT3,	0x29},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 10bit */
static const imx900_reg subsampling2_COLOR_594MBPS_1x10_1LANE[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x11},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0D},
	{GMRWT2,	0x05},
	{GMRWT3,	0x0A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x10_2LANE[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x20},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x19},
	{GMRWT2,	0x0B},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x10_4LANE[] = {

	{GMRWT,		0x19},
	{GMTWT,		0x39},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x19},
	{IMX900_36A9,	0x2C},
	{GMRWT2,	0x13},
	{GMRWT3,	0x23},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 12bit */
static const imx900_reg subsampling2_COLOR_594MBPS_1x12_1LANE[] = {

	{GMRWT,		0x07},
	{GMTWT,		0x0F},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x07},
	{IMX900_36A9,	0x0B},
	{GMRWT2,	0x04},
	{GMRWT3,	0x08},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x12_2LANE[] = {

	{GMRWT,		0x0C},
	{GMTWT,		0x1C},
	{GAINDLY,	0x01},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x0C},
	{IMX900_36A9,	0x15},
	{GMRWT2,	0x09},
	{GMRWT3,	0x11},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_COLOR_594MBPS_1x12_4LANE[] = {

	{GMRWT,		0x11},
	{GMTWT,		0x27},
	{GAINDLY,	0x02},
	{GSDLY,		0x01},
	{IMX900_36A8,	0x11},
	{IMX900_36A9,	0x1D},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x17},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/*
 * 1/2 subsampling mono and binning mono are common
 * 2376MBPS, 8bit
 */
static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x8_1LANE[] = {

	{GMRWT,		0x24},
	{GMTWT,		0x50},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x24},
	{IMX900_36A9,	0x3C},
	{GMRWT2,	0x18},
	{GMRWT3,	0x30},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x8_2LANE[] = {

	{GMRWT,		0x3C},
	{GMTWT,		0x8A},
	{GAINDLY,	0x04},
	{GSDLY,		0x04},
	{IMX900_36A8,	0x3C},
	{IMX900_36A9,	0x6A},
	{GMRWT2,	0x2E},
	{GMRWT3,	0x56},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x8_4LANE[] = {

	{GMRWT,		0x3C},
	{GMTWT,		0x8A},
	{GAINDLY,	0x04},
	{GSDLY,		0x04},
	{IMX900_36A8,	0x3C},
	{IMX900_36A9,	0x6A},
	{GMRWT2,	0x2E},
	{GMRWT3,	0x56},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 10bit */
static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x10_1LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x42},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x32},
	{GMRWT2,	0x14},
	{GMRWT3,	0x26},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x10_2LANE[] = {

	{GMRWT,		0x34},
	{GMTWT,		0x76},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x34},
	{IMX900_36A9,	0x5A},
	{GMRWT2,	0x26},
	{GMRWT3,	0x48},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x10_4LANE[] = {

	{GMRWT,		0x38},
	{GMTWT,		0x80},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x38},
	{IMX900_36A9,	0x62},
	{GMRWT2,	0x2A},
	{GMRWT3,	0x4E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 2376MBPS, 12bit */
static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x12_1LANE[] = {

	{GMRWT,		0x18},
	{GMTWT,		0x38},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x18},
	{IMX900_36A9,	0x2A},
	{GMRWT2,	0x12},
	{GMRWT3,	0x22},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x12_2LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4E},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_2376MBPS_1x12_4LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4E},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 8bit */
static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x8_1LANE[] = {

	{GMRWT,		0x16},
	{GMTWT,		0x34},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x16},
	{IMX900_36A9,	0x28},
	{GMRWT2,	0x12},
	{GMRWT3,	0x20},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x8_2LANE[] = {

	{GMRWT,		0x28},
	{GMTWT,		0x5C},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x28},
	{IMX900_36A9,	0x46},
	{GMRWT2,	0x1E},
	{GMRWT3,	0x38},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x8_4LANE[] = {

	{GMRWT,		0x3C},
	{GMTWT,		0x8A},
	{GAINDLY,	0x04},
	{GSDLY,		0x04},
	{IMX900_36A8,	0x3C},
	{IMX900_36A9,	0x6A},
	{GMRWT2,	0x2E},
	{GMRWT3,	0x56},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 10bit */
static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x10_1LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x2A},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x1A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x10_2LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4E},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x10_4LANE[] = {

	{GMRWT,		0x38},
	{GMTWT,		0x80},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x38},
	{IMX900_36A9,	0x62},
	{GMRWT2,	0x2A},
	{GMRWT3,	0x4E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1485MBPS, 12bit */
static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x12_1LANE[] = {

	{GMRWT,		0x10},
	{GMTWT,		0x24},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x10},
	{IMX900_36A9,	0x1C},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x16},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x12_2LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x42},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x32},
	{GMRWT2,	0x14},
	{GMRWT3,	0x28},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1485MBPS_1x12_4LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4E},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 8bit */
static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x8_1LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x2A},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x1A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x8_2LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4C},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x8_4LANE[] = {

	{GMRWT,		0x38},
	{GMTWT,		0x80},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x38},
	{IMX900_36A9,	0x60},
	{GMRWT2,	0x28},
	{GMRWT3,	0x4E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 10bit */
static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x10_1LANE[] = {

	{GMRWT,		0x10},
	{GMTWT,		0x22},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x10},
	{IMX900_36A9,	0x1A},
	{GMRWT2,	0x0A},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x10_2LANE[] = {

	{GMRWT,		0x1C},
	{GMTWT,		0x3E},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1C},
	{IMX900_36A9,	0x30},
	{GMRWT2,	0x14},
	{GMRWT3,	0x26},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x10_4LANE[] = {

	{GMRWT,		0x30},
	{GMTWT,		0x6C},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x30},
	{IMX900_36A9,	0x52},
	{GMRWT2,	0x22},
	{GMRWT3,	0x42},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 1188MBPS, 12bit */
static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x12_1LANE[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x1C},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x16},
	{GMRWT2,	0x08},
	{GMRWT3,	0x10},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x12_2LANE[] = {

	{GMRWT,		0x18},
	{GMTWT,		0x36},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x18},
	{IMX900_36A9,	0x28},
	{GMRWT2,	0x10},
	{GMRWT3,	0x20},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_1188MBPS_1x12_4LANE[] = {

	{GMRWT,		0x22},
	{GMTWT,		0x4E},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x22},
	{IMX900_36A9,	0x3A},
	{GMRWT2,	0x18},
	{GMRWT3,	0x2E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 8bit */
static const imx900_reg subsampling2_binning_MONO_891MBPS_1x8_1LANE[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x20},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x18},
	{GMRWT2,	0x0A},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x8_2LANE[] = {

	{GMRWT,		0x1A},
	{GMTWT,		0x3A},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1A},
	{IMX900_36A9,	0x2C},
	{GMRWT2,	0x12},
	{GMRWT3,	0x22},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x8_4LANE[] = {

	{GMRWT,		0x2C},
	{GMTWT,		0x62},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x2C},
	{IMX900_36A9,	0x4A},
	{GMRWT2,	0x1E},
	{GMRWT3,	0x3A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 10bit */
static const imx900_reg subsampling2_binning_MONO_891MBPS_1x10_1LANE[] = {

	{GMRWT,		0x0C},
	{GMTWT,		0x1A},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0C},
	{IMX900_36A9,	0x14},
	{GMRWT2,	0x08},
	{GMRWT3,	0x10},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x10_2LANE[] = {

	{GMRWT,		0x16},
	{GMTWT,		0x30},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x16},
	{IMX900_36A9,	0x24},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x1C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x10_4LANE[] = {

	{GMRWT,		0x24},
	{GMTWT,		0x54},
	{GAINDLY,	0x04},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x24},
	{IMX900_36A9,	0x40},
	{GMRWT2,	0x1C},
	{GMRWT3,	0x34},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 891MBPS, 12bit */
static const imx900_reg subsampling2_binning_MONO_891MBPS_1x12_1LANE[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x12},
	{GMRWT2,	0x08},
	{GMRWT3,	0x0E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x12_2LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x28},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x20},
	{GMRWT2,	0x0E},
	{GMRWT3,	0x18},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_891MBPS_1x12_4LANE[] = {

	{GMRWT,		0x20},
	{GMTWT,		0x48},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x20},
	{IMX900_36A9,	0x36},
	{GMRWT2,	0x16},
	{GMRWT3,	0x2C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 8bit */
static const imx900_reg subsampling2_binning_MONO_594MBPS_1x8_1LANE[] = {

	{GMRWT,		0x0A},
	{GMTWT,		0x16},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0A},
	{IMX900_36A9,	0x10},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0C},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x8_2LANE[] = {

	{GMRWT,		0x12},
	{GMTWT,		0x28},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x12},
	{IMX900_36A9,	0x1E},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x18},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x8_4LANE[] = {

	{GMRWT,		0x1E},
	{GMTWT,		0x44},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1E},
	{IMX900_36A9,	0x34},
	{GMRWT2,	0x16},
	{GMRWT3,	0x2A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 10bit */
static const imx900_reg subsampling2_binning_MONO_594MBPS_1x10_1LANE[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x12},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0E},
	{GMRWT2,	0x06},
	{GMRWT3,	0x0A},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x10_2LANE[] = {

	{GMRWT,		0x0E},
	{GMTWT,		0x20},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0E},
	{IMX900_36A9,	0x1A},
	{GMRWT2,	0x0C},
	{GMRWT3,	0x14},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x10_4LANE[] = {

	{GMRWT,		0x1A},
	{GMTWT,		0x3A},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x1A},
	{IMX900_36A9,	0x2C},
	{GMRWT2,	0x12},
	{GMRWT3,	0x22},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* 594MBPS, 12bit */
static const imx900_reg subsampling2_binning_MONO_594MBPS_1x12_1LANE[] = {

	{GMRWT,		0x08},
	{GMTWT,		0x10},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x08},
	{IMX900_36A9,	0x0C},
	{GMRWT2,	0x04},
	{GMRWT3,	0x08},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x12_2LANE[] = {

	{GMRWT,		0x0C},
	{GMTWT,		0x1C},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x0C},
	{IMX900_36A9,	0x16},
	{GMRWT2,	0x0A},
	{GMRWT3,	0x12},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

static const imx900_reg subsampling2_binning_MONO_594MBPS_1x12_4LANE[] = {

	{GMRWT,		0x16},
	{GMTWT,		0x32},
	{GAINDLY,	0x02},
	{GSDLY,		0x02},
	{IMX900_36A8,	0x16},
	{IMX900_36A9,	0x26},
	{GMRWT2,	0x10},
	{GMRWT3,	0x1E},

	{IMX900_TABLE_WAIT_MS, IMX900_WAIT_MS},
	{IMX900_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {

	IMX900_MODE_2064x1552,
	IMX900_MODE_ROI_1920x1080,
	IMX900_MODE_SUBSAMPLING2_1032x776,
	IMX900_MODE_SUBSAMPLING10_2064x154,
	IMX900_MODE_BINNING_CROP_1024x720,

	IMX900_MODE_ALLPIXEL_ROI,
	IMX900_MODE_SUBSAMPLING2_COLOR,
	IMX900_MODE_SUBSAMPLING2_BINNING_MONO,
	IMX900_MODE_SUBSAMPLING10,

	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_4LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_1LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_2LANE,
	IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_4LANE,


	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_4LANE,


	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_4LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_1LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_2LANE,
	IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_4LANE,


	IMX900_INIT_SETTINGS,
	IMX900_MODE_STOP_STREAM,

	IMX900_8BIT_MODE,
	IMX900_10BIT_MODE,
	IMX900_12BIT_MODE,

};

typedef enum {

	IMX900_2376_MBPS,
	IMX900_1485_MBPS,
	IMX900_1188_MBPS,
	IMX900_891_MBPS,
	IMX900_594_MBPS,

} data_rate_mode;

typedef enum {

	IMX900_COLOR = 0,
	IMX900_MONO = 1,

} chromacity_mode;

static const imx900_reg *data_rate_table[] = {

	[IMX900_2376_MBPS] = imx900_2376_mbps,
	[IMX900_1485_MBPS] = imx900_1485_mbps,
	[IMX900_1188_MBPS] = imx900_1188_mbps,
	[IMX900_891_MBPS] = imx900_891_mbps,
	[IMX900_594_MBPS] = imx900_594_mbps,
};

/* Connecting frame modes to mode tables */
static const imx900_reg *mode_table[] = {

	[IMX900_MODE_2064x1552] = mode_2064x1552,
	[IMX900_MODE_ROI_1920x1080] = mode_1920x1080,
	[IMX900_MODE_SUBSAMPLING2_1032x776] = mode_1032x776,
	[IMX900_MODE_SUBSAMPLING10_2064x154] = mode_2064x154,
	[IMX900_MODE_BINNING_CROP_1024x720] = mode_1024x720,


	[IMX900_MODE_ALLPIXEL_ROI] = mode_allPixel_roi,
	[IMX900_MODE_SUBSAMPLING2_COLOR] = mode_subsampling2_color,
	[IMX900_MODE_SUBSAMPLING2_BINNING_MONO] = mode_subsampling2_binning_mono,
	[IMX900_MODE_SUBSAMPLING10] = mode_subsampling10,


	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_1LANE] = allpixel_roi_subsampling10_2376MBPS_1x8_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_2LANE] = allpixel_roi_subsampling10_2376MBPS_1x8_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x8_4LANE] = allpixel_roi_subsampling10_2376MBPS_1x8_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_1LANE] = allpixel_roi_subsampling10_2376MBPS_1x10_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_2LANE] = allpixel_roi_subsampling10_2376MBPS_1x10_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x10_4LANE] = allpixel_roi_subsampling10_2376MBPS_1x10_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_1LANE] = allpixel_roi_subsampling10_2376MBPS_1x12_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_2LANE] = allpixel_roi_subsampling10_2376MBPS_1x12_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_2376MBPS_1x12_4LANE] = allpixel_roi_subsampling10_2376MBPS_1x12_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_1LANE] = allpixel_roi_subsampling10_1485MBPS_1x8_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_2LANE] = allpixel_roi_subsampling10_1485MBPS_1x8_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x8_4LANE] = allpixel_roi_subsampling10_1485MBPS_1x8_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_1LANE] = allpixel_roi_subsampling10_1485MBPS_1x10_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_2LANE] = allpixel_roi_subsampling10_1485MBPS_1x10_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x10_4LANE] = allpixel_roi_subsampling10_1485MBPS_1x10_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_1LANE] = allpixel_roi_subsampling10_1485MBPS_1x12_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_2LANE] = allpixel_roi_subsampling10_1485MBPS_1x12_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1485MBPS_1x12_4LANE] = allpixel_roi_subsampling10_1485MBPS_1x12_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_1LANE] = allpixel_roi_subsampling10_1188MBPS_1x8_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_2LANE] = allpixel_roi_subsampling10_1188MBPS_1x8_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x8_4LANE] = allpixel_roi_subsampling10_1188MBPS_1x8_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_1LANE] = allpixel_roi_subsampling10_1188MBPS_1x10_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_2LANE] = allpixel_roi_subsampling10_1188MBPS_1x10_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x10_4LANE] = allpixel_roi_subsampling10_1188MBPS_1x10_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_1LANE] = allpixel_roi_subsampling10_1188MBPS_1x12_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_2LANE] = allpixel_roi_subsampling10_1188MBPS_1x12_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_1188MBPS_1x12_4LANE] = allpixel_roi_subsampling10_1188MBPS_1x12_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_1LANE] = allpixel_roi_subsampling10_891MBPS_1x8_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_2LANE] = allpixel_roi_subsampling10_891MBPS_1x8_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x8_4LANE] = allpixel_roi_subsampling10_891MBPS_1x8_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_1LANE] = allpixel_roi_subsampling10_891MBPS_1x10_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_2LANE] = allpixel_roi_subsampling10_891MBPS_1x10_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x10_4LANE] = allpixel_roi_subsampling10_891MBPS_1x10_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_1LANE] = allpixel_roi_subsampling10_891MBPS_1x12_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_2LANE] = allpixel_roi_subsampling10_891MBPS_1x12_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_891MBPS_1x12_4LANE] = allpixel_roi_subsampling10_891MBPS_1x12_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_1LANE] = allpixel_roi_subsampling10_594MBPS_1x8_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_2LANE] = allpixel_roi_subsampling10_594MBPS_1x8_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x8_4LANE] = allpixel_roi_subsampling10_594MBPS_1x8_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_1LANE] = allpixel_roi_subsampling10_594MBPS_1x10_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_2LANE] = allpixel_roi_subsampling10_594MBPS_1x10_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x10_4LANE] = allpixel_roi_subsampling10_594MBPS_1x10_4lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_1LANE] = allpixel_roi_subsampling10_594MBPS_1x12_1lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_2LANE] = allpixel_roi_subsampling10_594MBPS_1x12_2lane,
	[IMX900_ALLPIXEL_ROI_SUBSAMPLING10_594MBPS_1x12_4LANE] = allpixel_roi_subsampling10_594MBPS_1x12_4lane,


	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_1LANE] = subsampling2_COLOR_2376MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_2LANE] = subsampling2_COLOR_2376MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x8_4LANE] = subsampling2_COLOR_2376MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_1LANE] = subsampling2_COLOR_2376MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_2LANE] = subsampling2_COLOR_2376MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x10_4LANE] = subsampling2_COLOR_2376MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_1LANE] = subsampling2_COLOR_2376MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_2LANE] = subsampling2_COLOR_2376MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_2376MBPS_1x12_4LANE] = subsampling2_COLOR_2376MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_1LANE] = subsampling2_COLOR_1485MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_2LANE] = subsampling2_COLOR_1485MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x8_4LANE] = subsampling2_COLOR_1485MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_1LANE] = subsampling2_COLOR_1485MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_2LANE] = subsampling2_COLOR_1485MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x10_4LANE] = subsampling2_COLOR_1485MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_1LANE] = subsampling2_COLOR_1485MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_2LANE] = subsampling2_COLOR_1485MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1485MBPS_1x12_4LANE] = subsampling2_COLOR_1485MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_1LANE] = subsampling2_COLOR_1188MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_2LANE] = subsampling2_COLOR_1188MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x8_4LANE] = subsampling2_COLOR_1188MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_1LANE] = subsampling2_COLOR_1188MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_2LANE] = subsampling2_COLOR_1188MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x10_4LANE] = subsampling2_COLOR_1188MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_1LANE] = subsampling2_COLOR_1188MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_2LANE] = subsampling2_COLOR_1188MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_1188MBPS_1x12_4LANE] = subsampling2_COLOR_1188MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_1LANE] = subsampling2_COLOR_891MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_2LANE] = subsampling2_COLOR_891MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x8_4LANE] = subsampling2_COLOR_891MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_1LANE] = subsampling2_COLOR_891MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_2LANE] = subsampling2_COLOR_891MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x10_4LANE] = subsampling2_COLOR_891MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_1LANE] = subsampling2_COLOR_891MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_2LANE] = subsampling2_COLOR_891MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_891MBPS_1x12_4LANE] = subsampling2_COLOR_891MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_1LANE] = subsampling2_COLOR_594MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_2LANE] = subsampling2_COLOR_594MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x8_4LANE] = subsampling2_COLOR_594MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_1LANE] = subsampling2_COLOR_594MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_2LANE] = subsampling2_COLOR_594MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x10_4LANE] = subsampling2_COLOR_594MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_1LANE] = subsampling2_COLOR_594MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_2LANE] = subsampling2_COLOR_594MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_COLOR_594MBPS_1x12_4LANE] = subsampling2_COLOR_594MBPS_1x12_4LANE,


	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_1LANE] = subsampling2_binning_MONO_2376MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_2LANE] = subsampling2_binning_MONO_2376MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x8_4LANE] = subsampling2_binning_MONO_2376MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_1LANE] = subsampling2_binning_MONO_2376MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_2LANE] = subsampling2_binning_MONO_2376MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x10_4LANE] = subsampling2_binning_MONO_2376MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_1LANE] = subsampling2_binning_MONO_2376MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_2LANE] = subsampling2_binning_MONO_2376MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_2376MBPS_1x12_4LANE] = subsampling2_binning_MONO_2376MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_1LANE] = subsampling2_binning_MONO_1485MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_2LANE] = subsampling2_binning_MONO_1485MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x8_4LANE] = subsampling2_binning_MONO_1485MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_1LANE] = subsampling2_binning_MONO_1485MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_2LANE] = subsampling2_binning_MONO_1485MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x10_4LANE] = subsampling2_binning_MONO_1485MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_1LANE] = subsampling2_binning_MONO_1485MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_2LANE] = subsampling2_binning_MONO_1485MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1485MBPS_1x12_4LANE] = subsampling2_binning_MONO_1485MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_1LANE] = subsampling2_binning_MONO_1188MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_2LANE] = subsampling2_binning_MONO_1188MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x8_4LANE] = subsampling2_binning_MONO_1188MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_1LANE] = subsampling2_binning_MONO_1188MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_2LANE] = subsampling2_binning_MONO_1188MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x10_4LANE] = subsampling2_binning_MONO_1188MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_1LANE] = subsampling2_binning_MONO_1188MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_2LANE] = subsampling2_binning_MONO_1188MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_1188MBPS_1x12_4LANE] = subsampling2_binning_MONO_1188MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_1LANE] = subsampling2_binning_MONO_891MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_2LANE] = subsampling2_binning_MONO_891MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x8_4LANE] = subsampling2_binning_MONO_891MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_1LANE] = subsampling2_binning_MONO_891MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_2LANE] = subsampling2_binning_MONO_891MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x10_4LANE] = subsampling2_binning_MONO_891MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_1LANE] = subsampling2_binning_MONO_891MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_2LANE] = subsampling2_binning_MONO_891MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_891MBPS_1x12_4LANE] = subsampling2_binning_MONO_891MBPS_1x12_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_1LANE] = subsampling2_binning_MONO_594MBPS_1x8_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_2LANE] = subsampling2_binning_MONO_594MBPS_1x8_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x8_4LANE] = subsampling2_binning_MONO_594MBPS_1x8_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_1LANE] = subsampling2_binning_MONO_594MBPS_1x10_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_2LANE] = subsampling2_binning_MONO_594MBPS_1x10_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x10_4LANE] = subsampling2_binning_MONO_594MBPS_1x10_4LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_1LANE] = subsampling2_binning_MONO_594MBPS_1x12_1LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_2LANE] = subsampling2_binning_MONO_594MBPS_1x12_2LANE,
	[IMX900_SUBSAMPLING2_BINNING_MONO_594MBPS_1x12_4LANE] = subsampling2_binning_MONO_594MBPS_1x12_4LANE,


	[IMX900_INIT_SETTINGS] = imx900_init_settings,

	[IMX900_8BIT_MODE] = imx900_8bit_mode,
	[IMX900_10BIT_MODE] = imx900_10bit_mode,
	[IMX900_12BIT_MODE] = imx900_12bit_mode,

	[IMX900_MODE_STOP_STREAM] = imx900_stop,
};


/* Framerates of available frame modes */
static const int imx900_125fps[] = {
	125,
};

static const int imx900_172fps[] = {
	172,
};

static const int imx900_230fps[] = {
	230,
};

static const int imx900_397fps[] = {
	397,
};

static const int imx900_656fps[] = {
	656,
};


/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx900_frmfmt[] = {
	{
		.size = {IMX900_DEFAULT_WIDTH, IMX900_DEFAULT_HEIGHT},
		.framerates = imx900_125fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX900_MODE_2064x1552
	},
	{
		.size = {IMX900_ROI_MODE_WIDTH, IMX900_ROI_MODE_HEIGHT},
		.framerates = imx900_172fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX900_MODE_ROI_1920x1080
	},
	{
		.size = {IMX900_SUBSAMPLING2_MODE_WIDTH,
				IMX900_SUBSAMPLING2_MODE_HEIGHT},
		.framerates = imx900_230fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX900_MODE_SUBSAMPLING2_1032x776
	},
	{
		.size = {IMX900_SUBSAMPLING10_MODE_WIDTH,
				IMX900_SUBSAMPLING10_MODE_HEIGHT},
		.framerates = imx900_656fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX900_MODE_SUBSAMPLING10_2064x154
	},
	{
		.size = {IMX900_BINNING_CROP_MODE_WIDTH,
				IMX900_BINNING_CROP_MODE_HEIGHT},
		.framerates = imx900_397fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX900_MODE_BINNING_CROP_1024x720
	},
};

#endif
