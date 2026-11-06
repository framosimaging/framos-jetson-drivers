/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (c) 2026 Framos. All rights reserved.
 *
 * fr_vb1943_mode_tbls.h - vb1943 sensor mode tables
 */

#ifndef __fr_VB1943_TABLES__
#define __fr_VB1943_TABLES__


/* STATUS group - base 0x0000, read-only */
#define DEVICE_MODEL_ID                          0x0000
#define DEVICE_REVISION                          0x0004
#define UI_REVISION                              0x0008
#define DEVICE_VARIANT                           0x000A
#define ROM_REVISION                             0x000C
#define CFA_SELECTION                            0x000E
#define DEVICE_NUMBER_1                          0x0010
#define DEVICE_NUMBER_2                          0x0014
#define DEVICE_NUMBER_3                          0x0018
#define DEVICE_NUMBER_4                          0x001C
#define DEVICE_NUMBER_5                          0x0020
#define SYSTEM_FSM_STATE                         0x0044
#define MIPI_LANE_NB                             0x0045
#define SYSTEM_WARNING                           0x0046
#define SYSTEM_ERROR                             0x0048
#define FWPATCH_REVISION                         0x004A
#define VTIMING_PATCH                            0x004E
#define TEMPERATURE_1                            0x006A
#define TEMPERATURE_2                            0x006C
#define FRAME_COUNTER                            0x006E
#define CURRENT_CONTEXT                          0x0072
#define NEXT_CONTEXT                             0x0073
#define LOOP_ELEMENT                             0x0074
#define CURRENT_ELEMENT                          0x0075
#define I2C_ADDRESS                              0x0076
#define ORIENTATION_STATUS                       0x0077
#define VT_CTRL_STATUS                           0x0078
#define OUTPUT_FORMAT                            0x0079
#define VIRTUAL_CHANNEL_STATUS                   0x007A
#define ROI_SELECTION_STATUS                     0x007B
#define ROI_A_OFFSET_STATUS                      0x007C
#define ROI_A_SIZE_STATUS                        0x0080
#define ROI_B_OFFSET_STATUS                      0x0084
#define ROI_B_SIZE_STATUS                        0x0088
#define ROI_C_OFFSET_STATUS                      0x008C
#define ROI_C_SIZE_STATUS                        0x0090
#define ROI_D_OFFSET_STATUS                      0x0094
#define ROI_D_SIZE_STATUS                        0x0098
#define ROI_A_DT_STATUS                          0x009C
#define ROI_B_DT_STATUS                          0x009D
#define ROI_C_DT_STATUS                          0x009E
#define ROI_D_DT_STATUS                          0x009F
#define PATGEN_CTRL_STATUS                       0x00A0
#define VT_ANALOG_GAIN                           0x00A2
#define INTEGRATION_TIME_PRIMARY_STATUS          0x00A4
#define INTEGRATION_TIME_IR_STATUS               0x00A6
#define INTEGRATION_TIME_SHORT_STATUS            0x00A8
#define DIGITAL_GAIN_R_STATUS                    0x00AA
#define DIGITAL_GAIN_G_STATUS                    0x00AC
#define DIGITAL_GAIN_B_STATUS                    0x00AE
#define DIGITAL_GAIN_IR_STATUS                   0x00B0
#define LINE_LENGTH_STATUS                       0x00B2
#define FRAME_LENGTH_STATUS                      0x00B4
#define READOUT_CTRL                             0x00B8
#define GPIO_STATUS                              0x00B9
#define PWL_STATUS                               0x00BA
#define IMAGE_CONFIG                             0x00BC
#define SHUTTER_MODE                             0x00BD
#define HDR                                      0x00BE
#define CFABLEND_ALPHA_R_STATUS                  0x00C0
#define CFABLEND_ALPHA_B_STATUS                  0x00C2
#define CFABLEND_ALPHA_GIB_STATUS                0x00C4
#define PLL_STATUS                               0x00C6
#define PEDESTAL_STATUS                          0x00D6
#define OIF_LANE_PHY_MAP_STATUS                  0x016C
#define NVM_STATUS                               0x0204
#define SYSTEM_PLL_CLK                           0x0228
#define PIXEL_CLK                                0x022C
#define MCU_CLK                                  0x0230
#define CLK_SYS_PLL_MULT                         0x0234
#define CLK_MIPI_PLL_MULT                        0x0235
#define FRAME_RATE                               0x0236
#define HUBBLEEYES_DMP_KNEE1                     0x02C0
#define HUBBLEEYES_DMP_KNEE2                     0x02C4
#define EXPO_STAT_SELECTION_STATUS               0x02C9
#define STAT_ROI_WIDTH_OFFSET_STATUS             0x02CA
#define STAT_ROI_HEIGHT_OFFSET_STATUS            0x02CC
#define STAT_ROI_WIDTH_STATUS                    0x02CE
#define STAT_ROI_HEIGHT_STATUS                   0x02D0
#define CHANNEL_STAT_CFG_STATUS                  0x02D2
#define HISTO_CFG_STATUS                         0x02D3

/* CMD group - base 0x0514 */
#define SYSTEM_UP                                0x0514
#define BOOT                                     0x0515
#define SW_STBY                                  0x0516
#define STREAMING                                0x0517

/* SENSOR_SETTINGS group - base 0x0734 */
#define EXT_CLOCK                                0x0734
#define MIPI_DATA_RATE                           0x0738
#define NVM_CTRL                                 0x073C
#define NVM_START_ADDRESS                        0x073D
#define NVM_NB_OF_WORDS                          0x073E
#define DEVICE_COMMS_CTRL                        0x0740
#define LANE_NB_SEL                              0x0743

/* STREAM_STATICS group - base 0x090C */
#define ROI_A_WIDTH_OFFSET                       0x090C
#define ROI_A_HEIGHT_OFFSET                      0x090E
#define ROI_A_WIDTH                              0x0910
#define ROI_A_HEIGHT                             0x0912
#define ROI_A_DT                                 0x0914
#define NB_FRAMES_TO_SEND                        0x0915
#define ROI_B_WIDTH_OFFSET                       0x0916
#define ROI_B_HEIGHT_OFFSET                      0x0918
#define ROI_B_WIDTH                              0x091A
#define ROI_B_HEIGHT                             0x091C
#define ROI_B_DT                                 0x091E
#define ROI_C_WIDTH_OFFSET                       0x0920
#define ROI_C_HEIGHT_OFFSET                      0x0922
#define ROI_C_WIDTH                              0x0924
#define ROI_C_HEIGHT                             0x0926
#define ROI_C_DT                                 0x0928
#define ROI_D_WIDTH_OFFSET                       0x092A
#define ROI_D_HEIGHT_OFFSET                      0x092C
#define ROI_D_WIDTH                              0x092E
#define ROI_D_HEIGHT                             0x0930
#define ROI_D_DT                                 0x0932
#define LINE_LENGTH                              0x0934
#define ORIENTATION                              0x0937
#define PATGEN_CTRL                              0x0938
#define OIF_LANE_PHY_MAP                         0x093A
#define OIF_LANE_PHY_SWAP                        0x093B
#define OIF_INTERPACKET_DELAY                    0x093C
#define PWL_EXPO_BIAS                            0x0AC0
#define FSYNC_IN_DELAY                           0x0AC4
#define VT_CTRL                                  0x0AC6
#define OIF_ISL_ENABLE                           0x0AC7
#define OIF_STATUS_LINE_CFG                      0x0AC8
#define VSYNC_START_DELAY                        0x0AC9
#define VSYNC_END_DELAY                          0x0ACA
#define STROBE_START_DELAY                       0x0ACB
#define STROBE_END_DELAY                         0x0ACC
#define PWM_CTRL                                 0x0AD0
#define GPIO_0_CTRL                              0x0AD4
#define GPIO_1_CTRL                              0x0AD5
#define GPIO_2_CTRL                              0x0AD6
#define GPIO_3_CTRL                              0x0AD7
#define CONTEXT_SWITCH_LOOP_ELEMENT              0x0AE4
#define HDR_GHOST_FLICKER_CTRL                   0x0AEC
#define CFA_BLENDER_ALPHA_CTRL                   0x0AED
#define EXPO_STAT_CTRL                           0x0AEE
#define HDEFCOR_ENABLE                           0x0AF2
#define DARKCAL_ENABLE                           0x0AF3
#define OIF_DT_STAT                              0x0AF4
#define CFABLEND_ALPHA_R                         0x0AF6
#define CFABLEND_ALPHA_B                         0x0AF8
#define CFABLEND_ALPHA_GIB                       0x0AFA
#define OIF_DT_ISL                               0x0AFC
#define CONTEXT_SWITCH_SEQUENCE_VECTOR_0         0x0ADC
#define CONTEXT_SWITCH_SEQUENCE_VECTOR_1         0x0AE0

/* STREAM_CTX0_STATIC group - absolute addresses */
#define CTX0_SENSOR_CONFIGURATION                0x0B40
#define CTX0_VIRTUAL_CHANNEL                     0x0B44
#define CTX0_FRAME_LENGTH                        0x0B46
#define CTX0_ROI_SELECTION                       0x0B48
#define CTX0_GPIO_CTRL                           0x0B49
#define CTX0_DUSTER_CTRL                         0x0B4A
#define CTX0_PWL_SEL                             0x0B4B
#define CTX0_EXPO_STAT_SELECTION                 0x0B4C
#define CTX0_STAT_ROI_WIDTH_OFFSET               0x0B4E
#define CTX0_STAT_ROI_HEIGHT_OFFSET              0x0B50
#define CTX0_STAT_ROI_WIDTH                      0x0B52
#define CTX0_STAT_ROI_HEIGHT                     0x0B54
#define CTX0_CHANNEL_STAT_CFG                    0x0B68
#define CTX0_HISTO_CFG                           0x0B69
#define CTX0_DARKCAL_CTRL                        0x0B6C
#define CTX0_DARKCAL_PEDESTAL                    0x0B6E
#define CTX0_DUSTER_NOISE_REDUC_BLEND            0x0B70

/* STREAM_CTX1_STATIC group - absolute addresses                              */
/* -------------------------------------------------------------------------- */
#define CTX1_SENSOR_CONFIGURATION                0x0B88
#define CTX1_VIRTUAL_CHANNEL                     0x0B8C
#define CTX1_FRAME_LENGTH                        0x0B8E
#define CTX1_ROI_SELECTION                       0x0B90
#define CTX1_GPIO_CTRL                           0x0B91
#define CTX1_DUSTER_CTRL                         0x0B92
#define CTX1_PWL_SEL                             0x0B93
#define CTX1_EXPO_STAT_SELECTION                 0x0B94
#define CTX1_STAT_ROI_WIDTH_OFFSET               0x0B96
#define CTX1_STAT_ROI_HEIGHT_OFFSET              0x0B98
#define CTX1_STAT_ROI_WIDTH                      0x0B9A
#define CTX1_STAT_ROI_HEIGHT                     0x0B9C
#define CTX1_CHANNEL_STAT_CFG                    0x0BB0
#define CTX1_HISTO_CFG                           0x0BB1
#define CTX1_DARKCAL_CTRL                        0x0BB4
#define CTX1_DARKCAL_PEDESTAL                    0x0BB6
#define CTX1_DUSTER_NOISE_REDUC_BLEND            0x0BB8

/* STREAM_CTX2_STATIC group - absolute addresses */
#define CTX2_SENSOR_CONFIGURATION                0x0BD0
#define CTX2_VIRTUAL_CHANNEL                     0x0BD4
#define CTX2_FRAME_LENGTH                        0x0BD6
#define CTX2_ROI_SELECTION                       0x0BD8
#define CTX2_GPIO_CTRL                           0x0BD9
#define CTX2_DUSTER_CTRL                         0x0BDA
#define CTX2_PWL_SEL                             0x0BDB
#define CTX2_EXPO_STAT_SELECTION                 0x0BDC
#define CTX2_STAT_ROI_WIDTH_OFFSET               0x0BDE
#define CTX2_STAT_ROI_HEIGHT_OFFSET              0x0BE0
#define CTX2_STAT_ROI_WIDTH                      0x0BE2
#define CTX2_STAT_ROI_HEIGHT                     0x0BE4
#define CTX2_CHANNEL_STAT_CFG                    0x0BF8
#define CTX2_HISTO_CFG                           0x0BF9
#define CTX2_DARKCAL_CTRL                        0x0BFC
#define CTX2_DARKCAL_PEDESTAL                    0x0BFE
#define CTX2_DUSTER_NOISE_REDUC_BLEND            0x0C00

/* STREAM_CTX3_STATIC group - absolute addresses */
#define CTX3_SENSOR_CONFIGURATION                0x0C18
#define CTX3_VIRTUAL_CHANNEL                     0x0C1C
#define CTX3_FRAME_LENGTH                        0x0C1E
#define CTX3_ROI_SELECTION                       0x0C20
#define CTX3_GPIO_CTRL                           0x0C21
#define CTX3_DUSTER_CTRL                         0x0C22
#define CTX3_PWL_SEL                             0x0C23
#define CTX3_EXPO_STAT_SELECTION                 0x0C24
#define CTX3_STAT_ROI_WIDTH_OFFSET               0x0C26
#define CTX3_STAT_ROI_HEIGHT_OFFSET              0x0C28
#define CTX3_STAT_ROI_WIDTH                      0x0C2A
#define CTX3_STAT_ROI_HEIGHT                     0x0C2C
#define CTX3_CHANNEL_STAT_CFG                    0x0C40
#define CTX3_HISTO_CFG                           0x0C41
#define CTX3_DARKCAL_CTRL                        0x0C44
#define CTX3_DARKCAL_PEDESTAL                    0x0C46
#define CTX3_DUSTER_NOISE_REDUC_BLEND            0x0C48

/* STREAM_CTX0_DYNAMIC group - absolute addresses */
#define CTX0_GROUP_PARAMETER_HOLD                0x0C78
#define CTX0_ANALOG_GAIN                         0x0C79
#define CTX0_INTEGRATION_TIME_PRIMARY            0x0C7A
#define CTX0_INTEGRATION_TIME_IR                 0x0C7C
#define CTX0_INTEGRATION_TIME_SHORT              0x0C7E
#define CTX0_DIGITAL_GAIN_R                      0x0C80
#define CTX0_DIGITAL_GAIN_G                      0x0C82
#define CTX0_DIGITAL_GAIN_B                      0x0C84
#define CTX0_DIGITAL_GAIN_IR                     0x0C86
#define CTX0_DUSTER_DEF_COR_RATIO                0x0C88
#define CTX0_DUSTER_NOISE_RED_RATIO              0x0C8A

/* STREAM_CTX1_DYNAMIC group - absolute addresses */
#define CTX1_GROUP_PARAMETER_HOLD                0x0CA0
#define CTX1_ANALOG_GAIN                         0x0CA1
#define CTX1_INTEGRATION_TIME_PRIMARY            0x0CA2
#define CTX1_INTEGRATION_TIME_IR                 0x0CA4
#define CTX1_INTEGRATION_TIME_SHORT              0x0CA6
#define CTX1_DIGITAL_GAIN_R                      0x0CA8
#define CTX1_DIGITAL_GAIN_G                      0x0CAA
#define CTX1_DIGITAL_GAIN_B                      0x0CAC
#define CTX1_DIGITAL_GAIN_IR                     0x0CAE
#define CTX1_DUSTER_DEF_COR_RATIO                0x0CB0
#define CTX1_DUSTER_NOISE_RED_RATIO              0x0CB2

/* STREAM_CTX2_DYNAMIC group - absolute addresses */
#define CTX2_GROUP_PARAMETER_HOLD                0x0CC8
#define CTX2_ANALOG_GAIN                         0x0CC9
#define CTX2_INTEGRATION_TIME_PRIMARY            0x0CCA
#define CTX2_INTEGRATION_TIME_IR                 0x0CCC
#define CTX2_INTEGRATION_TIME_SHORT              0x0CCE
#define CTX2_DIGITAL_GAIN_R                      0x0CD0
#define CTX2_DIGITAL_GAIN_G                      0x0CD2
#define CTX2_DIGITAL_GAIN_B                      0x0CD4
#define CTX2_DIGITAL_GAIN_IR                     0x0CD6
#define CTX2_DUSTER_DEF_COR_RATIO                0x0CD8
#define CTX2_DUSTER_NOISE_RED_RATIO              0x0CDA

/* STREAM_CTX3_DYNAMIC group - absolute addresses */
#define CTX3_GROUP_PARAMETER_HOLD                0x0CF0
#define CTX3_ANALOG_GAIN                         0x0CF1
#define CTX3_INTEGRATION_TIME_PRIMARY            0x0CF2
#define CTX3_INTEGRATION_TIME_IR                 0x0CF4
#define CTX3_INTEGRATION_TIME_SHORT              0x0CF6
#define CTX3_DIGITAL_GAIN_R                      0x0CF8
#define CTX3_DIGITAL_GAIN_G                      0x0CFA
#define CTX3_DIGITAL_GAIN_B                      0x0CFC
#define CTX3_DIGITAL_GAIN_IR                     0x0CFE
#define CTX3_DUSTER_DEF_COR_RATIO                0x0D00
#define CTX3_DUSTER_NOISE_RED_RATIO              0x0D02

/* OTP_CTM_MIRROR group - base 0x0EC4 */
#define OTP_I2C_ADDRESS                          0x0EC4
#define CTM_AREA_BASE                            0x0EC8

/* DEBUG group - base 0x1000 */
#define HDR_MERGE_KNEE_1                         0x105C
#define HDR_MERGE_KNEE_2                         0x1060
#define DPHYTX_CTRL                              0x108C
#define BOTTOM_STATUS_LINE_DISABLE               0x110C



/* Resolutions of implemented frame modes */
#define VB1943_DEFAULT_WIDTH			2560
#define VB1943_DEFAULT_HEIGHT			1984

#define VB1943_CROP_2064x1552_WIDTH		2064
#define VB1943_CROP_2064x1552_HEIGHT		1552

#define VB1943_CROP_1920x1080_WIDTH		1920
#define VB1943_CROP_1920x1080_HEIGHT		1080


#define VB1943_GS_RGB_NIR_RAW10	0x02	// CONFIG 2, global shutter, single exposure, RGB-NIR, RAW10
#define VB1943_GS_RGB_RAW10	0x06	// CONFIG 6, global shutter, single exposure, RGB, RAW10
#define VB1943_GS_NIR_RAW10	0x10	// CONFIG 16, global shutter, single exposure, IR, smart upscale, RAW10
#define VB1943_RS_RGB_NIR_RAW10	0x1B	// CONFIG 27, rolling shutter, sdr, RGB-NIR, RAW10
#define VB1943_RS_RGB_RAW10	0x1E	// CONFIG 30, rolling shutter, sdr, RGB, RAW10

#define VB1943_RS_RGB_NIR_RAW12	0x1C	// CONFIG 28, rolling shutter, sdr, RGB-NIR, RAW12
#define VB1943_RS_RGB_RAW12	0x1F	// CONFIG 31, rolling shutter, sdr, RGB, RAW12


/* Special values for the write table function */
#define VB1943_TABLE_WAIT_MS	0
#define VB1943_TABLE_END	1
#define VB1943_WAIT_MS		10

#define VB1943_TO_LOW_BYTE(x) (x & 0xFF)
#define VB1943_TO_MID_BYTE(x) (x >> 8)

typedef struct reg_8 vb1943_reg;


/* Tables for the write table function */
static const vb1943_reg vb1943_start[] = {

	{SW_STBY, 0x01},

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x00}
};

static const vb1943_reg vb1943_stop[] = {

	{STREAMING, 0x01},

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x00}
};

static const vb1943_reg vb1943_10bit_mode[] = {
	
	{ROI_A_DT, 0x2B}, // MIPI CSI-2 Data type RAW10

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x00}
};

static const vb1943_reg vb1943_12bit_mode[] = {

	{ROI_A_DT, 0x2C}, // MIPI CSI-2 Data type RAW12

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x00}
};

static const vb1943_reg vb1943_init_settings[] = {

	{LANE_NB_SEL,		0x00}, // 4 MIPI CSI Lanes
	{OIF_LANE_PHY_MAP,	0xE1}, // MIPI Lane Order: Lane2, Lane1, Lane3, Lane4
	{OIF_LANE_PHY_SWAP,	0x03}, // Lane1 & Lane2 Polarity Swap ON, Lane3, Lane4 & ClkLane Polarity Swap OFF
	{MIPI_DATA_RATE,	0x00}, // Data rate = 1500 Mbps
	{0x0739,		0x2F}, // Data rate = 1500 Mbps
	{0x073A,		0x68}, // Data rate = 1500 Mbps
	{0x073B,		0x59}, // Data rate = 1500 Mbps
	{OIF_ISL_ENABLE,	0x00},
	{BOTTOM_STATUS_LINE_DISABLE,	0x01},
	{CTX0_ROI_SELECTION,	0x01},
	{ORIENTATION,	0x00},

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END,	0x0000}
};

static const vb1943_reg mode_2560x1984[] = {
	{ROI_A_WIDTH_OFFSET,	0x00},	// X Offset 0
	{0x090D,		0x00},	// X Offset 0
	{ROI_A_HEIGHT_OFFSET,	0x00},	// Y Offset 0
	{0x090F,		0x00},	// Y Offset 0
	{ROI_A_WIDTH,		0x00},	// All Pixel width 2560 (0x0A00)
	{0x0911,		0x0A},	// All Pixel width 2560 (0x0A00)
	{ROI_A_HEIGHT,		0xC0},	// All Pixel height 1984 (0x07C0)
	{0x0913,		0x07},	// All Pixel height 1984 (0x07C0)

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x0000}
};

static const vb1943_reg mode_crop_2064x1552[] = {

	{ROI_A_WIDTH_OFFSET,	0xF8},	// X Offset (2560-2064)/2=248
	{0x090D,		0x00},	// X Offset (2560-2064)/2=248
	{ROI_A_HEIGHT_OFFSET,	0xD8},	// Y Offset (1984-1552)/2=216
	{0x090F,		0x00},	// Y Offset (1984-1552)/2=216
	{ROI_A_WIDTH,		0x10},	// Crop width 2064 (0x0810)
	{0x0911,		0x08},	// Crop width 2064 (0x0810)
	{ROI_A_HEIGHT,		0x10},	// Crop height 1552 (0x0610)
	{0x0913,		0x06},	// Crop height 1552 (0x0610)

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x0000}
};

static const vb1943_reg mode_crop_1920x1080[] = {

	{ROI_A_WIDTH_OFFSET,	0x40},	// X Offset (2560-1920)/2=320
	{0x090D,		0x01},	// X Offset (2560-1920)/2=320
	{ROI_A_HEIGHT_OFFSET,	0xC4},	// Y Offset (1984-1080)/2=452
	{0x090F,		0x01},	// Y Offset (1984-1080)/2=452
	{ROI_A_WIDTH,		0x80},	// Crop width 1920 (0x0780)
	{0x0911,		0x07},	// Crop width 1920 (0x0780)
	{ROI_A_HEIGHT,		0x38},	// Crop height 1080 (0x0438)
	{0x0913,		0x04},	// Crop height 1080 (0x0438)

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x0000}
};

static const vb1943_reg mode_enable_pattern_generator[] = {

	{PATGEN_CTRL,	0x01},

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x0000}
};

static const vb1943_reg mode_disable_pattern_generator[] = {

	{PATGEN_CTRL,		0x00},
	{PATGEN_CTRL + 1,	0x00},

	{VB1943_TABLE_WAIT_MS, VB1943_WAIT_MS},
	{VB1943_TABLE_END, 0x0000}
};

/* Enum of available frame modes */
enum {

	VB1943_MODE_2560x1984,
	VB1943_MODE_CROP_2064x1552,
	VB1943_MODE_CROP_1920x1080,

	VB1943_10BIT_MODE,
	VB1943_12BIT_MODE,

	VB1943_EN_PATTERN_GEN,
	VB1943_DIS_PATTERN_GEN,

	VB1943_INIT_SETTINGS,
	VB1943_MODE_START_STREAM,
	VB1943_MODE_STOP_STREAM,
};

/*
typedef enum {

	VB1943_1500_MBPS,
} data_rate_mode;
*/

/* Connecting frame modes to mode tables */
static const vb1943_reg *mode_table[] = {

	[VB1943_MODE_2560x1984] = mode_2560x1984,
	[VB1943_MODE_CROP_2064x1552] = mode_crop_2064x1552,
	[VB1943_MODE_CROP_1920x1080] = mode_crop_1920x1080,

	[VB1943_EN_PATTERN_GEN] = mode_enable_pattern_generator,
	[VB1943_DIS_PATTERN_GEN] = mode_disable_pattern_generator,

	[VB1943_10BIT_MODE] = vb1943_10bit_mode,
	[VB1943_12BIT_MODE] = vb1943_12bit_mode,

	[VB1943_INIT_SETTINGS] = vb1943_init_settings,

	[VB1943_MODE_START_STREAM] = vb1943_start,
	[VB1943_MODE_STOP_STREAM] = vb1943_stop,
};

/* Framerates of available frame modes */

static const int vb1943_100fps[] = {
	100,
};

/** Connecting resolutions, framerates and mode tables */
static const struct camera_common_frmfmt vb1943_frmfmt[] = {
	{
		.size = {VB1943_DEFAULT_WIDTH, VB1943_DEFAULT_HEIGHT},
		.framerates = vb1943_100fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = VB1943_MODE_2560x1984
	},
	{
		.size = {VB1943_CROP_2064x1552_WIDTH, VB1943_CROP_2064x1552_HEIGHT},
		.framerates = vb1943_100fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = VB1943_MODE_CROP_2064x1552
	},
	{
		.size = {VB1943_CROP_1920x1080_WIDTH, VB1943_CROP_1920x1080_HEIGHT},
		.framerates = vb1943_100fps,
		.num_framerates = 1,
		.hdr_en = false,
		.mode = VB1943_MODE_CROP_1920x1080
	}
};

#endif
