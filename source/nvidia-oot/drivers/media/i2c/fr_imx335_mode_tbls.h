/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx335_mode_tbls.h - imx335 sensor mode tables
 */

#ifndef __fr_IMX335_TABLES__
#define __fr_IMX335_TABLES__


#define STANDBY			0x3000
#define REGHOLD			0x3001
#define XMSTA			0x3002

#define BCWAIT_TIME		0x300C
#define CPWAIT_TIME		0x300D
#define SECOND_SLAVE_ADD	0x3010
#define WINMODE			0x3018

#define HTRIMMING_START_LOW	0x302C
#define HTRIMMING_START_HIGH	0x302D
#define HNUM_LOW		0x302E
#define HNUM_HIGH		0x302F

#define VMAX_LOW		0x3030
#define VMAX_MID		0x3031
#define VMAX_HIGH		0x3032
#define HMAX_LOW		0x3034
#define HMAX_HIGH		0x3035

#define OPB_SIZE_V		0x304C

#define HREVERSE		0x304E
#define VREVERSE		0x304F

#define ADBIT			0x3050
#define Y_OUT_SIZE_LOW		0x3056
#define Y_OUT_SIZE_HIGH		0x3057

#define SHR0_LOW		0x3058
#define SHR0_MID		0x3059
#define SHR0_HIGH		0x305A

#define AREA2_WIDTH_1_LOW	0x3072
#define AREA2_WIDTH_1_HIGH	0x3073

#define AREA3_ST_ADR_1_LOW	0x3074
#define AREA3_ST_ADR_1_HIGH	0x3075
#define AREA3_WIDTH_1_LOW	0x3076
#define AREA3_WIDTH_1_HIGH	0x3077

#define BLACK_OFSET_ADR_LOW	0x30C6
#define BLACK_OFSET_ADR_HIGH	0x30C7

#define UNRD_LINE_MAX_LOW	0x30CE
#define UNRD_LINE_MAX_HIGH	0x30CF

#define UNREAD_ED_ADR_LOW	0x30D8
#define UNREAD_ED_ADR_HIGH	0x30D9

#define GAIN_LOW		0x30E8
#define GAIN_HIGH		0x30E9

#define INCKSEL1_LOW		0x314C
#define INCKSEL1_HIGH		0x314D
#define INCKSEL2		0x315A
#define INCKSEL3		0x3168
#define INCKSEL4		0x316A

#define VADD_HADD		0x3199

#define MDBIT			0x319D
#define SYS_MODE		0x319E

#define XHSOUTSEL_XVSOUTSEL	0x31A0
#define XVS_XHS_DRV		0x31A1

#define XVSLNG			0x31D4
#define XHSLNG			0x31D5
#define EXTMODE			0x31D9

#define TCYCLE			0x3300
#define BLKLEVEL_LOW		0x3302
#define BLKLEVEL_HIGH		0x3303

#define ADBIT1_LOW		0x341C
#define ADBIT1_HIGH		0x341D

#define LANE_MODE		0x3A01

#define TCLKPOST_LOW		0x3A18
#define TCLKPOST_HIGH		0x3A19
#define TCLKPREPARE_LOW		0x3A1A
#define TCLKPREPARE_HIGH	0x3A1B
#define TCLKTRAIL_LOW		0x3A1C
#define TCLKTRAIL_HIGH		0x3A1D
#define TCLKZERO_LOW		0x3A1E
#define TCLKZERO_HIGH		0x3A1F

#define THSPREPARE_LOW		0x3A20
#define THSPREPARE_HIGH		0x3A21
#define THSZERO_LOW		0x3A22
#define THSZERO_HIGH		0x3A23
#define THSTRAIL_LOW		0x3A24
#define THSTRAIL_HIGH		0x3A25
#define THSEXIT_LOW		0x3A26
#define THSEXIT_HIGH		0x3A27
#define TLPX_LOW		0x3A28
#define TLPX_HIGH		0x3A29

#define DIG_CLP_MODE		0x3280
#define TPG_EN_DUOUT		0x329C
#define TPG_PATSEL_DUOUT	0x329E
#define TPG_COLORWIDTH		0x32A0

#define WRJ_OPEN		0x336C

#define IMX335_REG_3078		0x3078
#define IMX335_REG_3079		0x3079
#define IMX335_REG_307A		0x307A
#define IMX335_REG_307B		0x307B
#define IMX335_REG_307C		0x307C
#define IMX335_REG_307D		0x307D
#define IMX335_REG_307E		0x307E
#define IMX335_REG_307F		0x307F
#define IMX335_REG_3080		0x3080
#define IMX335_REG_3081		0x3081
#define IMX335_REG_3082		0x3082
#define IMX335_REG_3083		0x3083
#define IMX335_REG_3084		0x3084
#define IMX335_REG_3085		0x3085
#define IMX335_REG_3086		0x3086
#define IMX335_REG_3087		0x3087
#define IMX335_REG_30A4		0x30A4
#define IMX335_REG_30A8		0x30A8
#define IMX335_REG_30A9		0x30A9
#define IMX335_REG_30AC		0x30AC
#define IMX335_REG_30AD		0x30AD
#define IMX335_REG_30B0		0x30B0
#define IMX335_REG_30B1		0x30B1
#define IMX335_REG_30B4		0x30B4
#define IMX335_REG_30B5		0x30B5
#define IMX335_REG_30B6		0x30B6
#define IMX335_REG_30B7		0x30B7
#define IMX335_REG_3112		0x3112
#define IMX335_REG_3113		0x3113
#define IMX335_REG_3116		0x3116
#define IMX335_REG_3117		0x3117

#define IMX335_REG_3288		0x3288
#define IMX335_REG_328A		0x328A
#define IMX335_REG_3414		0x3414
#define IMX335_REG_3416		0x3416
#define IMX335_REG_35AC		0x35AC

#define IMX335_REG_3648		0x3648
#define IMX335_REG_364A		0x364A
#define IMX335_REG_364C		0x364C
#define IMX335_REG_3678		0x3678
#define IMX335_REG_367C		0x367C
#define IMX335_REG_367E		0x367E

#define IMX335_REG_3706		0x3706
#define IMX335_REG_3708		0x3708
#define IMX335_REG_3714		0x3714
#define IMX335_REG_3715		0x3715
#define IMX335_REG_3716		0x3716
#define IMX335_REG_3717		0x3717
#define IMX335_REG_371C		0x371C
#define IMX335_REG_371D		0x371D
#define IMX335_REG_372C		0x372C
#define IMX335_REG_372D		0x372D
#define IMX335_REG_372E		0x372E
#define IMX335_REG_372F		0x372F
#define IMX335_REG_3730		0x3730
#define IMX335_REG_3731		0x3731
#define IMX335_REG_3732		0x3732
#define IMX335_REG_3733		0x3733
#define IMX335_REG_3734		0x3734
#define IMX335_REG_3735		0x3735
#define IMX335_REG_3740		0x3740
#define IMX335_REG_375D		0x375D
#define IMX335_REG_375E		0x375E
#define IMX335_REG_375F		0x375F
#define IMX335_REG_3760		0x3760
#define IMX335_REG_3768		0x3768
#define IMX335_REG_3769		0x3769
#define IMX335_REG_376A		0x376A
#define IMX335_REG_376B		0x376B
#define IMX335_REG_376C		0x376C
#define IMX335_REG_376D		0x376D
#define IMX335_REG_376E		0x376E
#define IMX335_REG_3776		0x3776
#define IMX335_REG_3777		0x3777
#define IMX335_REG_3778		0x3778
#define IMX335_REG_3779		0x3779
#define IMX335_REG_377A		0x377A
#define IMX335_REG_377B		0x377B
#define IMX335_REG_377C		0x377C
#define IMX335_REG_377D		0x377D
#define IMX335_REG_377E		0x377E
#define IMX335_REG_377F		0x377F
#define IMX335_REG_3780		0x3780
#define IMX335_REG_3781		0x3781
#define IMX335_REG_3782		0x3782
#define IMX335_REG_3783		0x3783
#define IMX335_REG_3784		0x3784
#define IMX335_REG_3788		0x3788
#define IMX335_REG_378A		0x378A
#define IMX335_REG_378B		0x378B
#define IMX335_REG_378C		0x378C
#define IMX335_REG_378D		0x378D
#define IMX335_REG_378E		0x378E
#define IMX335_REG_378F		0x378F
#define IMX335_REG_3790		0x3790
#define IMX335_REG_3792		0x3792
#define IMX335_REG_3794		0x3794
#define IMX335_REG_3796		0x3796
#define IMX335_REG_37B0		0x37B0

/* Resolutions of implemented frame modes */
#define IMX335_DEFAULT_WIDTH	2616
#define IMX335_DEFAULT_HEIGHT	1964

/* Special values for the write table function */
#define IMX335_TABLE_WAIT_MS	0
#define IMX335_TABLE_END	1
#define IMX335_WAIT_MS		10

#define IMX335_MIN_FRAME_LENGTH_DELTA 96

#define IMX335_TO_LOW_BYTE(x) (x & 0xFF)
#define IMX335_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 imx335_reg;


/* Tables for the write table function */
static const imx335_reg imx335_start[] = {

	{STANDBY, 0x00},
	{IMX335_TABLE_WAIT_MS, 30},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x00}
};

static const imx335_reg imx335_stop[] = {

	{XMSTA, 0x01},
	{IMX335_TABLE_WAIT_MS, 30},
	{STANDBY, 0x01},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x00}
};

static const imx335_reg imx335_10bit_mode[] = {

	{ADBIT,			0x00},
	{ADBIT1_HIGH,		0x01},
	{ADBIT1_LOW,		0xFF},

	{MDBIT,			0x00},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x00}
};

static const imx335_reg imx335_12bit_mode[] = {

	{ADBIT,			0x01},
	{ADBIT1_HIGH,		0x00},
	{ADBIT1_LOW,		0x47},

	{MDBIT,			0x01},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x00}
};

static const imx335_reg imx335_1188_mbps[] = {

	{TCLKPOST_LOW,		0x8F},
	{TCLKPREPARE_LOW,	0x4F},
	{TCLKTRAIL_LOW,		0x47},
	{TCLKZERO_HIGH,		0x01},
	{TCLKZERO_LOW,		0x37},
	{THSPREPARE_LOW,	0x4F},
	{THSZERO_LOW,		0x87},
	{THSTRAIL_LOW,		0x4F},
	{THSEXIT_LOW,		0x7F},
	{TLPX_LOW,		0x3F},

	{BCWAIT_TIME,		0x5B},
	{CPWAIT_TIME,		0x40},
	{INCKSEL1_HIGH,		0x00},
	{INCKSEL1_LOW,		0x80},
	{INCKSEL2,		0x02},

	{INCKSEL3,		0x68},
	{INCKSEL4,		0x7E},

	{SYS_MODE,		0x01},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg imx335_891_mbps[] = {

	{TCLKPOST_LOW,		0x7F},
	{TCLKPREPARE_LOW,	0x37},
	{TCLKTRAIL_LOW,		0x37},
	{TCLKZERO_HIGH,		0x00},
	{TCLKZERO_LOW,		0xF7},
	{THSPREPARE_LOW,	0x3F},
	{THSZERO_LOW,		0x6F},
	{THSTRAIL_LOW,		0x3F},
	{THSEXIT_LOW,		0x5F},
	{TLPX_LOW,		0x2F},

	{BCWAIT_TIME,		0x5B},
	{CPWAIT_TIME,		0x40},
	{INCKSEL1_HIGH,		0x00},
	{INCKSEL1_LOW,		0xC0},
	{INCKSEL2,		0x06},

	{INCKSEL3,		0x68},
	{INCKSEL4,		0x7E},

	{SYS_MODE,		0x02},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg imx335_init_settings[] = {

	{HREVERSE,		0x00},
	{VREVERSE,		0x00},

	{WINMODE,		0x00},
	{VMAX_MID,		0x11},
	{VMAX_LOW,		0x94},
	{HMAX_HIGH,		0x02},
	{HMAX_LOW,		0x94},
	{OPB_SIZE_V,		0x14},
	{VADD_HADD,		0x00},

	{LANE_MODE,		0x03},

	{IMX335_REG_3288,	0x21},
	{IMX335_REG_328A,	0x02},
	{IMX335_REG_3414,	0x05},
	{IMX335_REG_3416,	0x18},

	{IMX335_REG_3648,	0x01},
	{IMX335_REG_364A,	0x04},
	{IMX335_REG_364C,	0x04},
	{IMX335_REG_3678,	0x01},
	{IMX335_REG_367C,	0x31},
	{IMX335_REG_367E,	0x31},

	{IMX335_REG_3706,	0x10},
	{IMX335_REG_3708,	0x03},
	{IMX335_REG_3714,	0x02},
	{IMX335_REG_3715,	0x02},
	{IMX335_REG_3716,	0x01},
	{IMX335_REG_3717,	0x03},
	{IMX335_REG_371C,	0x3D},

	{IMX335_REG_371D,	0x3F},
	{IMX335_REG_372C,	0x00},
	{IMX335_REG_372D,	0x00},
	{IMX335_REG_372E,	0x46},
	{IMX335_REG_372F,	0x00},
	{IMX335_REG_3730,	0x89},

	{IMX335_REG_3731,	0x00},
	{IMX335_REG_3732,	0x08},
	{IMX335_REG_3733,	0x01},
	{IMX335_REG_3734,	0xFE},
	{IMX335_REG_3735,	0x05},
	{IMX335_REG_3740,	0x02},
	{IMX335_REG_375D,	0x00},
	{IMX335_REG_375E,	0x00},
	{IMX335_REG_375F,	0x11},
	{IMX335_REG_3760,	0x01},
	{IMX335_REG_3768,	0x1B},
	{IMX335_REG_3769,	0x1B},
	{IMX335_REG_376A,	0x1B},
	{IMX335_REG_376B,	0x1B},
	{IMX335_REG_376C,	0x1A},
	{IMX335_REG_376D,	0x17},
	{IMX335_REG_376E,	0x0F},
	{IMX335_REG_3776,	0x00},
	{IMX335_REG_3777,	0x00},
	{IMX335_REG_3778,	0x46},
	{IMX335_REG_3779,	0x00},
	{IMX335_REG_377A,	0x89},
	{IMX335_REG_377B,	0x00},
	{IMX335_REG_377C,	0x08},
	{IMX335_REG_377D,	0x01},
	{IMX335_REG_377E,	0x23},
	{IMX335_REG_377F,	0x02},
	{IMX335_REG_3780,	0xD9},
	{IMX335_REG_3781,	0x03},
	{IMX335_REG_3782,	0xF5},
	{IMX335_REG_3783,	0x06},
	{IMX335_REG_3784,	0xA5},
	{IMX335_REG_3788,	0x0F},
	{IMX335_REG_378A,	0xD9},
	{IMX335_REG_378B,	0x03},
	{IMX335_REG_378C,	0xEB},
	{IMX335_REG_378D,	0x05},
	{IMX335_REG_378E,	0x87},
	{IMX335_REG_378F,	0x06},
	{IMX335_REG_3790,	0xF5},
	{IMX335_REG_3792,	0x43},
	{IMX335_REG_3794,	0x7A},
	{IMX335_REG_3796,	0xA1},
	{IMX335_REG_37B0,	0x36},

	{IMX335_REG_3078,	0x01},
	{IMX335_REG_3079,	0x02},
	{IMX335_REG_307A,	0xFF},
	{IMX335_REG_307B,	0x02},
	{IMX335_REG_307C,	0x00},
	{IMX335_REG_307D,	0x00},
	{IMX335_REG_307E,	0x00},
	{IMX335_REG_307F,	0x00},
	{IMX335_REG_3080,	0x01},
	{IMX335_REG_3081,	0x02},
	{IMX335_REG_3082,	0xFF},
	{IMX335_REG_3083,	0x02},
	{IMX335_REG_3084,	0x00},
	{IMX335_REG_3085,	0x00},
	{IMX335_REG_3086,	0x00},
	{IMX335_REG_3087,	0x00},
	{IMX335_REG_30A4,	0x33},
	{IMX335_REG_30A8,	0x10},
	{IMX335_REG_30A9,	0x04},
	{IMX335_REG_30AC,	0x00},
	{IMX335_REG_30AD,	0x00},
	{IMX335_REG_30B0,	0x10},
	{IMX335_REG_30B1,	0x08},
	{IMX335_REG_30B4,	0x00},
	{IMX335_REG_30B5,	0x00},
	{IMX335_REG_30B6,	0x00},
	{IMX335_REG_30B7,	0x00},
	{IMX335_REG_3112,	0x08},
	{IMX335_REG_3113,	0x00},
	{IMX335_REG_3116,	0x08},
	{IMX335_REG_3117,	0x00},

	{HTRIMMING_START_HIGH,	0x00},
	{HTRIMMING_START_LOW,	0x30},
	{HNUM_HIGH,		0x0A},
	{HNUM_LOW,		0x38},
	{AREA3_ST_ADR_1_HIGH,	0x00},
	{AREA3_ST_ADR_1_LOW,	0xB0},
	{AREA3_WIDTH_1_HIGH,	0x0F},
	{AREA3_WIDTH_1_LOW,	0x58},
	{AREA2_WIDTH_1_LOW,	0x28},

	{BLACK_OFSET_ADR_LOW,	0x00},
	{UNRD_LINE_MAX_LOW,	0x00},
	{UNREAD_ED_ADR_HIGH,	0x10},
	{UNREAD_ED_ADR_LOW,	0x4C},
	{VADD_HADD,		0x00},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg mode_2616x1964[] = {

	{WINMODE,		0x00},
	{AREA2_WIDTH_1_LOW,	0x28},
	{VADD_HADD,		0x00},
	{TCYCLE,		0x00},

	{Y_OUT_SIZE_HIGH,	IMX335_TO_MID_BYTE(1964)},
	{Y_OUT_SIZE_LOW,	IMX335_TO_LOW_BYTE(1964)},

	{AREA3_ST_ADR_1_HIGH,	IMX335_TO_MID_BYTE(40)},
	{AREA3_ST_ADR_1_LOW,	IMX335_TO_LOW_BYTE(40)},
	{AREA3_WIDTH_1_HIGH,	IMX335_TO_MID_BYTE(3928)},
	{AREA3_WIDTH_1_LOW,	IMX335_TO_LOW_BYTE(3928)},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg mode_h2v2_binning[] = {

	{WINMODE,		0x01},
	{AREA2_WIDTH_1_LOW,	0x30},
	{VADD_HADD,		0x30},
	{TCYCLE,		0x01},

	{Y_OUT_SIZE_HIGH,	IMX335_TO_MID_BYTE(984)},
	{Y_OUT_SIZE_LOW,	IMX335_TO_LOW_BYTE(984)},

	{AREA3_ST_ADR_1_HIGH,	IMX335_TO_MID_BYTE(168)},
	{AREA3_ST_ADR_1_LOW,	IMX335_TO_LOW_BYTE(168)},
	{AREA3_WIDTH_1_HIGH,	IMX335_TO_MID_BYTE(3936)},
	{AREA3_WIDTH_1_LOW,	IMX335_TO_LOW_BYTE(3936)},

	{ADBIT,			0x00},
	{ADBIT1_HIGH,		0x01},
	{ADBIT1_LOW,		0xFF},
	{MDBIT,			0x01},

	{IMX335_REG_3078,	0x04},
	{IMX335_REG_3079,	0xFD},
	{IMX335_REG_307A,	0x04},
	{IMX335_REG_307B,	0xFE},
	{IMX335_REG_307C,	0x04},
	{IMX335_REG_307D,	0xFB},
	{IMX335_REG_307E,	0x04},
	{IMX335_REG_307F,	0x02},
	{IMX335_REG_3080,	0x04},
	{IMX335_REG_3081,	0xFD},
	{IMX335_REG_3082,	0x04},
	{IMX335_REG_3083,	0xFE},
	{IMX335_REG_3084,	0x04},
	{IMX335_REG_3085,	0xFB},
	{IMX335_REG_3086,	0x04},
	{IMX335_REG_3087,	0x02},
	{IMX335_REG_30A4,	0x77},
	{IMX335_REG_30A8,	0x20},
	{IMX335_REG_30A9,	0x00},
	{IMX335_REG_30AC,	0x08},
	{IMX335_REG_30AD,	0x08},
	{IMX335_REG_30B0,	0x20},
	{IMX335_REG_30B1,	0x00},
	{IMX335_REG_30B4,	0x10},
	{IMX335_REG_30B5,	0x10},
	{IMX335_REG_30B6,	0x00},
	{IMX335_REG_30B7,	0x00},
	{IMX335_REG_3112,	0x10},
	{IMX335_REG_3113,	0x00},
	{IMX335_REG_3116,	0x10},
	{IMX335_REG_3117,	0x00},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg mode_crop_1920x1080[] = {

	{WINMODE,		4},
	{VADD_HADD,		0},
	{TCYCLE,		0},

	{HTRIMMING_START_HIGH,	IMX335_TO_MID_BYTE(396)},
	{HTRIMMING_START_LOW,	IMX335_TO_LOW_BYTE(396)},
	{HNUM_HIGH,		IMX335_TO_MID_BYTE(1920)},
	{HNUM_LOW,		IMX335_TO_LOW_BYTE(1920)},

	{AREA3_ST_ADR_1_HIGH,	IMX335_TO_MID_BYTE(1060)},
	{AREA3_ST_ADR_1_LOW,	IMX335_TO_LOW_BYTE(1060)},
	{AREA3_WIDTH_1_HIGH,	IMX335_TO_MID_BYTE(2160)},
	{AREA3_WIDTH_1_LOW,	IMX335_TO_LOW_BYTE(2160)},

	{Y_OUT_SIZE_HIGH,	IMX335_TO_MID_BYTE(1080)},
	{Y_OUT_SIZE_LOW,	IMX335_TO_LOW_BYTE(1080)},

	{BLACK_OFSET_ADR_LOW,	18},
	{UNRD_LINE_MAX_LOW,	100},
	{UNREAD_ED_ADR_HIGH,	IMX335_TO_MID_BYTE(3428)},
	{UNREAD_ED_ADR_LOW,	IMX335_TO_LOW_BYTE(3428)},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg mode_enable_pattern_generator[] = {

	{DIG_CLP_MODE,		0x00},
	{TPG_EN_DUOUT,		0x01},
	{TPG_COLORWIDTH,	0x11},
	{BLKLEVEL_LOW,		0x00},
	{WRJ_OPEN,		0x00},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

static const imx335_reg mode_disable_pattern_generator[] = {

	{DIG_CLP_MODE,		0x01},
	{TPG_EN_DUOUT,		0x00},
	{TPG_COLORWIDTH,	0x10},
	{BLKLEVEL_LOW,		0x32},
	{WRJ_OPEN,		0x01},

	{IMX335_TABLE_WAIT_MS, IMX335_WAIT_MS},
	{IMX335_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {
	IMX335_MODE_2616x1964,
	IMX335_MODE_CROP_1920x1080,

	IMX335_MODE_H2V2_BINNING,

	IMX335_10BIT_MODE,
	IMX335_12BIT_MODE,

	IMX335_EN_PATTERN_GEN,
	IMX335_DIS_PATTERN_GEN,

	IMX335_INIT_SETTINGS,
	IMX335_MODE_START_STREAM,
	IMX335_MODE_STOP_STREAM,
};

typedef enum {

	IMX335_1188_MBPS,
	IMX335_891_MBPS,
} data_rate_mode;

static const imx335_reg *data_rate_table[] = {

	[IMX335_1188_MBPS] = imx335_1188_mbps,
	[IMX335_891_MBPS] = imx335_891_mbps,
};

/* Connecting frame modes to mode tables */
static const imx335_reg *mode_table[] = {

	[IMX335_MODE_2616x1964] = mode_2616x1964,
	[IMX335_MODE_CROP_1920x1080] = mode_crop_1920x1080,

	[IMX335_MODE_H2V2_BINNING] = mode_h2v2_binning,

	[IMX335_EN_PATTERN_GEN] = mode_enable_pattern_generator,
	[IMX335_DIS_PATTERN_GEN] = mode_disable_pattern_generator,

	[IMX335_10BIT_MODE] = imx335_10bit_mode,
	[IMX335_12BIT_MODE] = imx335_12bit_mode,

	[IMX335_INIT_SETTINGS] = imx335_init_settings,

	[IMX335_MODE_START_STREAM] = imx335_start,
	[IMX335_MODE_STOP_STREAM] = imx335_stop,
};

/* Framerates of available frame modes */
static const int imx335_60fps[] = {
	60,
};
static const int imx335_120fps[] = {
	120,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx335_frmfmt[] = {
	{
		.size = {IMX335_DEFAULT_WIDTH, IMX335_DEFAULT_HEIGHT},
		.framerates = imx335_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX335_MODE_2616x1964
	},
	{
		.size = {1920, 1080},
		.framerates = imx335_120fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX335_MODE_CROP_1920x1080
	},
	{
		.size = {1320, 984},
		.framerates = imx335_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX335_MODE_H2V2_BINNING
	},
};

#endif
