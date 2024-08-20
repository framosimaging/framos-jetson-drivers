/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_imx296_mode_tbls.h - imx296 sensor mode tables
 */

#ifndef __fr_IMX296_TABLES__
#define __fr_IMX296_TABLES__


#define IMX_3A00		0x3A00
#define IMX_3005		0x3005
#define IMX_350B		0x350B
#define IMX_300D		0x300D
#define IMX_400E		0x400E
#define VMAX_LOW		0x3010
#define VMAX_MID		0x3011
#define VMAX_HIGH		0x3012
#define HMAX_LOW		0x3014
#define HMAX_HIGH		0x3015

#define IMX_4014		0x4014
#define IMX_3516		0x3516
#define IMX_321A		0x321A
#define IMX_3226		0x3226
#define IMX_3832		0x3832
#define IMX_3833		0x3833
#define IMX_3541		0x3541
#define IMX_4041		0x4041
#define IMX_3D48		0x3D48
#define IMX_3D49		0x3D49
#define IMX_3D4A		0x3D4A
#define IMX_3D4B		0x3D4B
#define IMX_3256		0x3256
#define IMX_3758		0x3758
#define IMX_3759		0x3759
#define IMX_375A		0x375A
#define IMX_375B		0x375B
#define IMX_3165		0x3165
#define IMX_3169		0x3169
#define IMX_316A		0x316A
#define IMX_4174		0x4174
#define PULSE2			0x3079
#define IMX_3090		0x3090
#define IMX_3094		0x3094
#define IMX_3098		0x3098
#define IMX_309E		0x309E
#define IMX_30A0		0x30A0
#define IMX_30A1		0x30A1
#define IMX_38A2		0x38A2
#define IMX_40A2		0x40A2
#define IMX_38A3		0x38A3
#define IMX_30A4		0x30A4
#define IMX_30A8		0x30A8

#define IMX_30AC		0x30AC
#define IMX_30AF		0x30AF
#define IMX_40C1		0x40C1
#define IMX_40C7		0x40C7
#define IMX_31C8		0x31C8
#define IMX_40C8		0x40C8
#define IMX_31D0		0x31D0
#define IMX_30DF		0x30DF

#define PATERN_GEN		0x3238

#define STANDBY			0x3000
#define REGHOLD			0x3008
#define XMSTA			0x300A
#define SYNCSEL			0x3036
#define TRIGEN			0x300B
#define LOWLAGTRG		0x30AE
#define VINT_EN			0x30AA

#define GAIN_LOW		0x3204
#define GAIN_HIGH		0x3205
#define GAINDLY			0x3212
#define BLKLEVEL_LOW		0x3254
#define BLKLEVEL_HIGH		0x3255
#define INCKSEL0		0x3089
#define INCKSEL1		0x308A
#define INCKSEL2		0x308B
#define INCKSEL3		0x308C
#define IMX_418C		0x418C

#define SHS1_LOW		0x308D
#define SHS1_MID		0x308E
#define SHS1_HIGH		0x308F

#define FID0_ROIV1ON_ROIH1ON	0x3300
#define FID0_ROIPH1_LOW		0x3310
#define FID0_ROIPH1_HIGH	0x3311
#define FID0_ROIPV1_LOW		0x3312
#define FID0_ROIPV1_HIGH	0x3313
#define FID0_ROIWH1_LOW		0x3314
#define FID0_ROIWH1_HIGH	0x3315
#define FID0_ROIWV1_LOW		0x3316
#define FID0_ROIWV1_HIGH	0x3317

#define MIPIC_AREA3W_LOW	0x4182
#define MIPIC_AREA3W_HIGH	0x4183

#define SENSOR_INFO		0x3149


/* Resolutions of implemented frame modes */
#define IMX296_DEFAULT_WIDTH		1456
#define IMX296_DEFAULT_HEIGHT		1088

#define IMX296_H2V2_BINNING_WIDTH	728
#define IMX296_H2V2_BINNING_HEIGHT	544

#define IMX296_CROP_1280x720_WIDTH	1280
#define IMX296_CROP_1280x720_HEIGHT	720


/* Special values for the write table function */
#define IMX296_TABLE_WAIT_MS	0
#define IMX296_TABLE_END	1
#define IMX296_WAIT_MS		10

#define IMX296_MIN_FRAME_LENGTH_DELTA 30

typedef struct reg_8 imx296_reg;


/* Tables for the write table function */
static const imx296_reg imx296_stop[] = {

	{XMSTA, 0x01},
	{IMX296_TABLE_WAIT_MS, IMX296_WAIT_MS},

	{STANDBY, 0x01},
	{IMX296_TABLE_END, 0x00}
};

static const imx296_reg imx296_init_settings[] = {

	{GAIN_LOW,		0x00},
	{GAIN_HIGH,		0x00},
	{GAINDLY,		0x09},
	{INCKSEL0,		0x80},
	{INCKSEL1,		0x0B},
	{INCKSEL2,		0x80},
	{INCKSEL3,		0x08},
	{IMX_418C,		0x74},

	{IMX_3A00,		0x80},
	{IMX_3005,		0xF0},
	{IMX_350B,		0x0F},
	{IMX_300D,		0x00},
	{IMX_400E,		0x58},
	{VMAX_LOW,		0x5E},
	{VMAX_MID,		0x04},
	{HMAX_LOW,		0x4C},
	{IMX_4014,		0x1C},
	{HMAX_HIGH,		0x04},
	{IMX_3516,		0x77},
	{IMX_321A,		0x00},
	{IMX_3226,		0x02},
	{IMX_3832,		0xF5},
	{IMX_3833,		0x00},
	{IMX_3541,		0x72},
	{IMX_4041,		0x2A},
	{IMX_3D48,		0xA3},
	{IMX_3D49,		0x00},
	{IMX_3D4A,		0x85},
	{IMX_3D4B,		0x00},
	{IMX_3256,		0x01},
	{IMX_3758,		0xA3},
	{IMX_3759,		0x00},
	{IMX_375A,		0x85},
	{IMX_375B,		0x00},
	{IMX_3165,		0x00},
	{IMX_3169,		0x10},
	{IMX_316A,		0x02},
	{IMX_4174,		0x00},
	{PULSE2,		0x08},
	{IMX_3090,		0x04},
	{IMX_3094,		0x04},
	{IMX_3098,		0x04},
	{IMX_309E,		0x04},
	{IMX_30A0,		0x04},
	{IMX_30A1,		0x3C},
	{IMX_38A2,		0xF6},
	{IMX_40A2,		0x06},
	{IMX_38A3,		0x00},
	{IMX_30A4,		0x5F},
	{IMX_30A8,		0x91},
	{IMX_30AC,		0x28},
	{IMX_30AF,		0x09},
	{IMX_40C1,		0xF6},
	{IMX_40C7,		0x0F},
	{IMX_31C8,		0xF3},
	{IMX_40C8,		0x00},
	{IMX_31D0,		0xF4},
	{IMX_30DF,		0x00},
	{VINT_EN,		0x00},

	{IMX296_TABLE_END, 0x00}
};

static const imx296_reg mode_1456x1088[] = {

	{FID0_ROIV1ON_ROIH1ON,	0x00},
	{VMAX_LOW,		0x5E},
	{VMAX_MID,		0x04},
	{GAIN_LOW,		0x00},
	{GAIN_HIGH,		0x00},

	{IMX296_TABLE_WAIT_MS, IMX296_WAIT_MS},
	{IMX296_TABLE_END, 0x00}
};

static const imx296_reg mode_1280x720[] = {

	{IMX_300D,		0x00},
	{VMAX_LOW,		0xEB},
	{VMAX_MID,		0x02},
	{HMAX_LOW,		0x4C},
	{HMAX_HIGH,		0x04},
	{FID0_ROIV1ON_ROIH1ON,	0x03},
	{FID0_ROIPH1_LOW,	0x58},
	{FID0_ROIPH1_HIGH,	0x00},
	{FID0_ROIPV1_LOW,	0xB8},
	{FID0_ROIPV1_HIGH,	0x00},
	{FID0_ROIWH1_LOW,	0x00},
	{FID0_ROIWH1_HIGH,	0x05},
	{FID0_ROIWV1_LOW,	0xD0},
	{FID0_ROIWV1_HIGH,	0x02},
	{MIPIC_AREA3W_LOW,	0xD0},
	{MIPIC_AREA3W_HIGH,	0x02},

	{IMX296_TABLE_WAIT_MS, IMX296_WAIT_MS},
	{IMX296_TABLE_END, 0x00}
};

static const imx296_reg mode_h2v2_binning[] = {

	{FID0_ROIV1ON_ROIH1ON,	0x00},
	{IMX_300D,		0x22},
	{VMAX_LOW,		0x3B},
	{VMAX_MID,		0x02},
	{HMAX_LOW,		0x2E},
	{HMAX_HIGH,		0x04},
	{MIPIC_AREA3W_LOW,	0x20},
	{MIPIC_AREA3W_HIGH,	0x02},

	{IMX296_TABLE_WAIT_MS, IMX296_WAIT_MS},
	{IMX296_TABLE_END, 0x00}
};

/* Enum of available frame modes */
enum {

	IMX296_MODE_1456x1088,
	IMX296_MODE_1280x720,
	IMX296_MODE_H2V2_BINNING,

	IMX296_INIT_SETTINGS,

	IMX296_MODE_STOP_STREAM,
};

/* Connecting frame modes to mode tables */
static const imx296_reg *mode_table[] = {

	[IMX296_MODE_1456x1088] = mode_1456x1088,
	[IMX296_MODE_1280x720] = mode_1280x720,
	[IMX296_MODE_H2V2_BINNING] = mode_h2v2_binning,

	[IMX296_INIT_SETTINGS] = imx296_init_settings,

	[IMX296_MODE_STOP_STREAM] = imx296_stop,
};

/* Framerates of available frame modes */
static const int imx296_60fps[] = {
	60,
};

static const int imx296_90fps[] = {
	90,
};

static const int imx296_121fps[] = {
	121,
};

/* Connecting resolutions, framerates and mode tables */

static const struct camera_common_frmfmt imx296_frmfmt[] = {
	{
		.size = {IMX296_DEFAULT_WIDTH, IMX296_DEFAULT_HEIGHT},
		.framerates = imx296_60fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX296_MODE_1456x1088
	},
	{
		.size = {IMX296_CROP_1280x720_WIDTH, IMX296_CROP_1280x720_HEIGHT},
		.framerates = imx296_90fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX296_MODE_1280x720
	},
	{
		.size = {IMX296_H2V2_BINNING_WIDTH, IMX296_H2V2_BINNING_HEIGHT},
		.framerates = imx296_121fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = IMX296_MODE_H2V2_BINNING
	},
};

#endif
