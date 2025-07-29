/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx464_mode_tbls.h - imx464 sensor mode tables
 */

#ifndef __fr_IMX464_TABLES__
#define __fr_IMX464_TABLES__


#define STANDBY						0x3000
#define REGHOLD						0x3001
#define XMSTA						0x3002
#define RESTART						0x3004
#define BCWAIT_TIME					0x300C
#define CPWAIT_TIME					0x300D
#define SECOND_SLAVE_ADD				0x3010
#define WINMODE						0x3018
#define FDG_SEL						0x3019
#define HTRIMMING_START_LOW				0x302C
#define HTRIMMING_START_HIGH				0x302D
#define HNUM_LOW					0x302E
#define HNUM_HIGH					0x302F
#define VMAX_LOW					0x3030
#define VMAX_MID					0x3031
#define VMAX_HIGH					0x3032
#define HMAX_LOW					0x3034
#define HMAX_HIGH					0x3035
#define WDMODE						0x3048
#define WDSEL						0x3049
#define WD_SET1						0x304A
#define WD_SET2						0x304B
#define OPB_SIZE_V					0x304C
#define RAMPSHSWSEL					0x304D
#define HREVERSE					0x304E
#define VREVERSE					0x304F
#define ADBIT						0x3050
#define Y_OUT_SIZE_LOW					0x3056
#define Y_OUT_SIZE_HIGH					0x3057
#define SHR0_LOW					0x3058
#define SHR0_MID					0x3059
#define SHR0_HIGH					0x305A
#define SHR1_LOW					0x305C
#define SHR1_MID					0x305D
#define SHR1_HIGH					0x305E
#define SHR2_LOW					0x3060
#define SHR2_MID					0x3061
#define SHR2_HIGH					0x3062
#define RHS1_LOW					0x3068
#define RHS1_MID					0x3069
#define RHS1_HIGH					0x306A
#define RHS2_LOW					0x306C
#define RHS2_MID					0x306D
#define RHS2_HIGH					0x306E
#define AREA2_WIDTH_1_LOW				0x3072
#define AREA2_WIDTH_1_HIGH				0x3073
#define AREA3_ST_ADR_1_LOW				0x3074
#define AREA3_ST_ADR_1_HIGH				0x3075
#define AREA3_WIDTH_1_LOW				0x3076
#define AREA3_WIDTH_1_HIGH				0x3077
#define BLACK_OFSET_ADR_LOW				0x30C6
#define BLACK_OFSET_ADR_HIGH				0x30C7
#define UNRD_LINE_MAX_LOW				0x30CE
#define UNRD_LINE_MAX_HIGH				0x30CF
#define UNREAD_ED_ADR_LOW				0x30D8
#define UNREAD_ED_ADR_HIGH				0x30D9
#define GAIN_LOW					0x30E8
#define GAIN_HIGH					0x30E9
#define INCKSEL1_LOW					0x314C
#define INCKSEL1_HIGH					0x314D
#define INCKSEL2_PLL_IF_GC				0x315A
#define INCKSEL3					0x3168
#define INCKSEL4					0x316A
#define MDBIT						0x319D
#define SYS_MODE					0x319E
#define VCEN						0x319F
#define XVSOUTSEL_XHSOUTSEL				0x31A0
#define XVS_DRV_XHS_DRV					0x31A1
#define EXTMODE						0x31D9
#define XVSLNG						0x31D4
#define XHSLNG						0x31D5
#define XVSMSKCNT_INT					0x31D7

#define BLKLEVEL_LOW					0x3302
#define BLKLEVEL_HIGH					0x3303

#define LANEMODE					0x3A01
#define TCLKPOST_LOW					0x3A18
#define TCLKPOST_HIGH					0x3A19
#define TCLKPREPARE_LOW					0x3A1A
#define TCLKPREPARE_HIGH				0x3A1B
#define TCLKTRAIL_LOW					0x3A1C
#define TCLKTRAIL_HIGH					0x3A1D
#define TCLKZERO_LOW					0x3A1E
#define TCLKZERO_HIGH					0x3A1F
#define THSPREPARE_LOW					0x3A20
#define THSPREPARE_HIGH					0x3A21
#define THSZERO_LOW					0x3A22
#define THSZERO_HIGH					0x3A23
#define THSTRAIL_LOW					0x3A24
#define THSTRAIL_HIGH					0x3A25
#define THSEXIT_LOW					0x3A26
#define THSEXIT_HIGH					0x3A27
#define TLPX_LOW					0x3A28
#define TLPX_HIGH					0x3A29

#define TESTCLKEN_MIPI					0x3148
#define DIG_CLP_MODE					0x3280
#define TPG_EN_DUOUT					0x329C
#define TPG_PATSEL_DUOUT				0x329E
#define TPG_COLORWIDTH					0x32A0
#define WRJ_OPEN					0x336C

/* Default resolution */
#define IMX464_DEFAULT_WIDTH				2712
#define IMX464_DEFAULT_HEIGHT				1538

#define IMX464_CROP_1920x1080_WIDTH			1920
#define IMX464_CROP_1920x1080_HEIGHT			1080

#define IMX464_DOL2_MIN_SHR1				9

/* Special values for the write table function */
#define IMX464_TABLE_WAIT_MS				0
#define IMX464_TABLE_END				1
#define IMX464_WAIT_MS					10

#define IMX464_MIN_FRAME_LENGTH_DELTA			112

#define IMX464_TO_LOW_BYTE(x)				(x & 0xFF)
#define IMX464_TO_MID_BYTE(x)				(x >> 8)

typedef struct reg_8 imx464_reg;

/* Tables for the write table function */
static const imx464_reg imx464_start[] = {
	{STANDBY, 0x00},
	{IMX464_TABLE_WAIT_MS, 20},

	{0x37B0, 0x36},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_stop[] = {
	{XMSTA, 0x01},
	{0x37B0, 0x37},
	{IMX464_TABLE_WAIT_MS, 20},
	{STANDBY, 0x01},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_10bit_mode[] = {
	{ADBIT, 0x00},
	{MDBIT, 0x00},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}};

static const imx464_reg imx464_12bit_mode[] = {
	{ADBIT, 0x01},
	{MDBIT, 0x01},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_891_data_rate[] = {
	{TCLKPOST_LOW, 0x7F},
	{TCLKPREPARE_LOW, 0x37},
	{TCLKTRAIL_LOW, 0x37},
	{TCLKZERO_HIGH, 0x00},
	{TCLKZERO_LOW, 0xF7},
	{THSPREPARE_LOW, 0x3F},
	{THSZERO_LOW, 0x6F},
	{THSTRAIL_LOW, 0x3F},
	{THSEXIT_LOW, 0x5F},
	{TLPX_LOW, 0x2F},

	{BCWAIT_TIME, 0x5B},
	{CPWAIT_TIME, 0x40},
	{INCKSEL1_HIGH, 0x00},
	{INCKSEL1_LOW, 0xC0},
	{INCKSEL2_PLL_IF_GC, 0x06},
	{INCKSEL3, 0x68},
	{INCKSEL4, 0x7E},
	{SYS_MODE, 0x02},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_1188_data_rate[] = {
	{TCLKPOST_LOW, 0x8F},
	{TCLKPREPARE_LOW, 0x4F},
	{TCLKTRAIL_LOW, 0x47},
	{TCLKZERO_HIGH, 0x01},
	{TCLKZERO_LOW, 0x37},
	{THSPREPARE_LOW, 0x4F},
	{THSZERO_LOW, 0x87},
	{THSTRAIL_LOW, 0x4F},
	{THSEXIT_LOW, 0x7F},
	{TLPX_LOW, 0x3F},

	{BCWAIT_TIME, 0x5B},
	{CPWAIT_TIME, 0x40},
	{INCKSEL1_HIGH, 0x00},
	{INCKSEL1_LOW, 0x80},
	{INCKSEL2_PLL_IF_GC, 0x02},
	{INCKSEL3, 0x68},
	{INCKSEL4, 0x7E},
	{SYS_MODE, 0x01},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_init_settings[] = {

	{WINMODE, 0x00},
	{OPB_SIZE_V, 0x14},
	{HREVERSE, 0x00},
	{VREVERSE, 0x00},
	{LANEMODE, 0x03},

	{WDMODE, 0x00},
	{WDSEL, 0x00},
	{WD_SET1, 0x03},
	{WD_SET2, 0x02},
	{RAMPSHSWSEL, 0x03},
	{XVSMSKCNT_INT, 0x00},

	{0x30BE, 0x5E},
	{0x30BF, 0x06},
	{0x3288, 0x22},
	{0x328A, 0x02},
	{0x328C, 0xA2},
	{0x328E, 0x22},
	{0x3415, 0x27},
	{0x3418, 0x27},
	{0x3428, 0xFE},
	{0x3429, 0x06},
	{0x349E, 0x6A},
	{0x349F, 0x00},
	{0x34A2, 0x9A},
	{0x34A4, 0x8A},
	{0x34A6, 0x8E},
	{0x35BC, 0x00},
	{0x35BD, 0x00},
	{0x35BE, 0xFF},
	{0x35BF, 0x01},
	{0x35CC, 0x1B},
	{0x35CD, 0x00},
	{0x35CE, 0x2A},
	{0x35CF, 0x00},
	{0x35DC, 0x07},
	{0x35DD, 0x00},
	{0x35DE, 0x1A},
	{0x35DF, 0x00},
	{0x35E4, 0x2B},
	{0x35E5, 0x00},
	{0x35E6, 0x07},
	{0x35E7, 0x01},
	{0x3648, 0x01},
	{0x3678, 0x01},
	{0x367C, 0x69},
	{0x367D, 0x00},
	{0x367E, 0x69},
	{0x367F, 0x00},
	{0x3680, 0x69},
	{0x3681, 0x00},
	{0x3682, 0x69},
	{0x3683, 0x00},
	{0x3718, 0x1C},
	{0x371D, 0x05},
	{0x375D, 0x11},
	{0x375E, 0x43},
	{0x375F, 0x76},
	{0x3760, 0x07},
	{0x3768, 0x1B},
	{0x3769, 0x1B},
	{0x376A, 0x1A},
	{0x376B, 0x19},
	{0x376C, 0x17},
	{0x376D, 0x0F},
	{0x376E, 0x0B},
	{0x376F, 0x0B},
	{0x3770, 0x0B},
	{0x3776, 0x89},
	{0x3777, 0x00},
	{0x3778, 0xCA},
	{0x3779, 0x00},
	{0x377A, 0x45},
	{0x377B, 0x01},
	{0x377C, 0x56},
	{0x377D, 0x02},
	{0x377E, 0xFE},
	{0x377F, 0x03},
	{0x3780, 0xFE},
	{0x3781, 0x05},
	{0x3782, 0xFE},
	{0x3783, 0x06},
	{0x3784, 0x7F},
	{0x3785, 0x07},
	{0x3788, 0x1F},
	{0x378A, 0xCA},
	{0x378B, 0x00},
	{0x378C, 0x45},
	{0x378D, 0x01},
	{0x378E, 0x56},
	{0x378F, 0x02},
	{0x3790, 0xFE},
	{0x3791, 0x03},
	{0x3792, 0xFE},
	{0x3793, 0x05},
	{0x3794, 0xFE},
	{0x3795, 0x06},
	{0x3796, 0x7F},
	{0x3797, 0x07},
	{0x3798, 0xBF},
	{0x3799, 0x07},

	{0x3080, 0x01},
	{0x30AD, 0x02},
	{0x30B6, 0x00},
	{0x30B7, 0x00},
	{0x30D8, 0x44},
	{0x30D9, 0x06},
	{0x3116, 0x02},
	{0x3117, 0x00},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg mode_2712x1538[] = {
	{WINMODE, 0x00},
	{Y_OUT_SIZE_LOW, 0x02},
	{Y_OUT_SIZE_HIGH, 0x06},

	{HTRIMMING_START_LOW, 0x24},
	{HTRIMMING_START_HIGH, 0x00},
	{HNUM_LOW, 0x98},
	{HNUM_HIGH, 0x0A},
	{AREA2_WIDTH_1_LOW, 0x14},
	{AREA2_WIDTH_1_HIGH, 0x00},
	{AREA3_ST_ADR_1_LOW, 0x3C},
	{AREA3_ST_ADR_1_HIGH, 0x00},
	{AREA3_WIDTH_1_LOW, 0x02},
	{AREA3_WIDTH_1_HIGH, 0x06},
	{BLACK_OFSET_ADR_LOW, 0x06},
	{BLACK_OFSET_ADR_HIGH, 0x00},
	{UNRD_LINE_MAX_LOW, 0x04},
	{UNRD_LINE_MAX_HIGH, 0x00},
	{UNREAD_ED_ADR_LOW, 0x44},
	{UNREAD_ED_ADR_HIGH, 0x06},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg mode_crop_1920x1080[] = {
	{WINMODE, 0x04},

	{HTRIMMING_START_LOW, 0xA4},
	{HTRIMMING_START_HIGH, 0x01},
	{HNUM_LOW, 0x80},
	{HNUM_HIGH, 0x07},
	{Y_OUT_SIZE_LOW, 0x38},
	{Y_OUT_SIZE_HIGH, 0x04},
	{AREA2_WIDTH_1_LOW, 0x14},
	{AREA2_WIDTH_1_HIGH, 0x00},
	{AREA3_ST_ADR_1_LOW, 0x18},
	{AREA3_ST_ADR_1_HIGH, 0x01},
	{AREA3_WIDTH_1_LOW, 0x38},
	{AREA3_WIDTH_1_HIGH, 0x04},
	{BLACK_OFSET_ADR_LOW, 0x12},
	{BLACK_OFSET_ADR_HIGH, 0x00},
	{UNRD_LINE_MAX_LOW, 0x64},
	{UNRD_LINE_MAX_HIGH, 0x00},
	{UNREAD_ED_ADR_LOW, 0x56},
	{UNREAD_ED_ADR_HIGH, 0x05},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg imx464_dol2_hdr_mode[] = {
	{VCEN, 0x00},	{WDMODE, 0x01},	{WDSEL, 0x01},
	{WD_SET1, 0x04},	{WD_SET2, 0x04},	{OPB_SIZE_V, 0x00},
	{XVSMSKCNT_INT, 0x01}, {RAMPSHSWSEL, 0x00},
};

static const imx464_reg mode_enable_pattern_generator[] = {
	{TESTCLKEN_MIPI, (0x01 << 4)},
	{DIG_CLP_MODE, 0x00},
	{TPG_EN_DUOUT, 0x01},
	{TPG_COLORWIDTH, 0x12},
	{BLKLEVEL_LOW, 0x00},
	{BLKLEVEL_HIGH, 0x00},
	{WRJ_OPEN, 0x04},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

static const imx464_reg mode_disable_pattern_generator[] = {
	{TESTCLKEN_MIPI, 0x00},
	{DIG_CLP_MODE, 0x01},
	{TPG_EN_DUOUT, 0x00},
	{TPG_COLORWIDTH, 0x10},
	{WRJ_OPEN, 0x05},

	{IMX464_TABLE_WAIT_MS, IMX464_WAIT_MS},
	{IMX464_TABLE_END, 0x00}
};

/* Enum of available frame modes */
enum {
	IMX464_MODE_2712x1538,

	IMX464_MODE_crop_1920x1080,

	IMX464_10BIT_MODE,
	IMX464_12BIT_MODE,

	IMX464_EN_PATTERN_GEN,
	IMX464_DIS_PATTERN_GEN,

	IMX464_INIT_SETTINGS,
	IMX464_MODE_START_STREAM,
	IMX464_MODE_STOP_STREAM,
};

enum {
	IMX464_1188_DATA_RATE,
	IMX464_891_DATA_RATE,
};

static const imx464_reg *data_rate_table[] = {
	[IMX464_1188_DATA_RATE] = imx464_1188_data_rate,
	[IMX464_891_DATA_RATE] = imx464_891_data_rate,
};

/* Connecting frame modes to mode tables */
static const imx464_reg *mode_table[] = {
	[IMX464_MODE_2712x1538] = mode_2712x1538,
	[IMX464_MODE_crop_1920x1080] = mode_crop_1920x1080,

	[IMX464_EN_PATTERN_GEN] = mode_enable_pattern_generator,
	[IMX464_DIS_PATTERN_GEN] = mode_disable_pattern_generator,

	[IMX464_10BIT_MODE] = imx464_10bit_mode,
	[IMX464_12BIT_MODE] = imx464_12bit_mode,

	[IMX464_INIT_SETTINGS] = imx464_init_settings,

	[IMX464_MODE_START_STREAM] = imx464_start,
	[IMX464_MODE_STOP_STREAM] = imx464_stop,
};

/* Framerates of available frame modes */
static const int imx464_45fps[] = {
	45,
};
static const int imx464_90fps[] = {
	90,
};
static const int imx464_124fps[] = {
	124,
};

/* Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt imx464_frmfmt[] = {
	{.size = {IMX464_DEFAULT_WIDTH, IMX464_DEFAULT_HEIGHT},
	 .framerates = imx464_90fps,
	 .num_framerates = 1,
	 .hdr_en = false,
	 .mode = IMX464_MODE_2712x1538},
	{.size = {IMX464_CROP_1920x1080_WIDTH, IMX464_CROP_1920x1080_HEIGHT},
	 .framerates = imx464_124fps,
	 .num_framerates = 1,
	 .hdr_en = false,
	 .mode = IMX464_MODE_crop_1920x1080}};

#endif
