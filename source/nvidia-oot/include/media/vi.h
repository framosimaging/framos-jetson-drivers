/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Tegra Graphics Host VI
 *
 * Copyright (c) 2012-2022, NVIDIA CORPORATION. All rights reserved.
 */

#ifndef __NVHOST_VI_H__
#define __NVHOST_VI_H__

#include <linux/clk/tegra.h>

#include <media/mc_common.h>

#define VI_CFG_INTERRUPT_MASK_0				0x8c
#define VI_CFG_INTERRUPT_STATUS_0			0x98

#define CSI_CSI_PIXEL_PARSER_A_INTERRUPT_MASK_0		0x850
#define CSI_CSI_PIXEL_PARSER_A_STATUS_0			0x854
#define PPA_FIFO_OVRF					(1 << 5)

#define CSI_CSI_PIXEL_PARSER_B_INTERRUPT_MASK_0		0x884
#define CSI_CSI_PIXEL_PARSER_B_STATUS_0			0x888
#define PPB_FIFO_OVRF					(1 << 5)

#define VI_CSI_0_ERROR_STATUS				0x184
#define VI_CSI_1_ERROR_STATUS				0x284
#define VI_CSI_0_WD_CTRL				0x18c
#define VI_CSI_1_WD_CTRL				0x28c
#define VI_CSI_0_ERROR_INT_MASK_0			0x188
#define VI_CSI_1_ERROR_INT_MASK_0			0x288

#ifdef TEGRA_21X_OR_HIGHER_CONFIG
#define VI_CSI_2_ERROR_STATUS				0x384
#define VI_CSI_3_ERROR_STATUS				0x484
#define VI_CSI_4_ERROR_STATUS				0x584
#define VI_CSI_5_ERROR_STATUS				0x684

#define VI_CSI_2_WD_CTRL				0x38c
#define VI_CSI_3_WD_CTRL				0x48c
#define VI_CSI_4_WD_CTRL				0x58c
#define VI_CSI_5_WD_CTRL				0x68c

#define VI_CSI_2_ERROR_INT_MASK_0			0x388
#define VI_CSI_3_ERROR_INT_MASK_0			0x488
#define VI_CSI_4_ERROR_INT_MASK_0			0x588
#define VI_CSI_5_ERROR_INT_MASK_0			0x688

#define CSI1_CSI_PIXEL_PARSER_A_INTERRUPT_MASK_0	0x1050
#define CSI1_CSI_PIXEL_PARSER_A_STATUS_0		0x1054
#define CSI1_CSI_PIXEL_PARSER_B_INTERRUPT_MASK_0	0x1084
#define CSI1_CSI_PIXEL_PARSER_B_STATUS_0		0x1088
#define CSI2_CSI_PIXEL_PARSER_A_INTERRUPT_MASK_0	0x1850
#define CSI2_CSI_PIXEL_PARSER_A_STATUS_0		0x1854
#define CSI2_CSI_PIXEL_PARSER_B_INTERRUPT_MASK_0	0x1884
#define CSI2_CSI_PIXEL_PARSER_B_STATUS_0		0x1888

#define NUM_VI_WATCHDOG					6
#else
#define NUM_VI_WATCHDOG					2
#endif

/*
XXX Beware, these defines are part of a patch that is (hopefully) a temporary implementation of accessing embedded data on jetson tx2.
It relies on a custom patch in the VI4 driver on Jetson TX2.
Most likely we will be able to implement a similar patch for Xavier and Nano platforms (VI2 and VI5).

The patch works by allocating some extra memory after each buffer, and the VI is modified to put the embedded data there.
The buffer zone is used to store the width and height of embedded data (2 bytes each),
but it has to be 0x100 bytes because that is the smallest increment by which the dma target addresses can be adjusted.
The embedded data is stored after the buffer "buffer zone".

buffer
+---------------------------------+
|pixel data                       |
|                                 |
+---------------------------------+
|buffer zone of 0x100 bytes       |
+---------------------------------+
|embedded data                    |
+---------------------------------+

 ASCII art generated using https://asciiflow.com/
 */

#define EMBEDDED_DATA_MAX_SIZE ((8*64*1024))
#define EMBEDDED_DATA_BUFFER_ZONE_SIZE ((0x100))

typedef void (*callback)(void *);

struct tegra_vi_stats {
	atomic_t overflow;
};

struct tegra_vi_mfi_ctx;

struct vi {
	struct tegra_camera *camera;
	struct platform_device *ndev;
	struct device *dev;
	struct tegra_vi_data *data;
	struct tegra_mc_vi mc_vi;
	struct tegra_csi_device csi;

	struct regulator *reg;
	struct dentry *debugdir;
	struct tegra_vi_stats vi_out;
	struct work_struct stats_work;
	struct tegra_vi_mfi_ctx *mfi_ctx;
	int vi_irq;
	uint vi_bypass_bw;
	uint max_bw;
	bool master_deinitialized;
	bool tpg_opened;
	bool sensor_opened;
	bool bypass;
};

extern const struct file_operations tegra_vi_ctrl_ops;
int nvhost_vi_prepare_poweroff(struct platform_device *);
int nvhost_vi_finalize_poweron(struct platform_device *);

void nvhost_vi_reset_all(struct platform_device *);
struct vi *tegra_vi_get(void);
int vi_v4l2_set_la(struct vi *tegra_vi, u32 vi_bypass_bw, bool is_ioctl);

int tegra_vi_register_mfi_cb(callback cb, void *cb_arg);
int tegra_vi_unregister_mfi_cb(void);

bool tegra_vi_has_mfi_callback(void);
int tegra_vi_mfi_event_notify(struct tegra_vi_mfi_ctx *mfi_ctx, u8 channel);
int tegra_vi_init_mfi(struct tegra_vi_mfi_ctx **mfi_ctx, u8 num_channels);
void tegra_vi_deinit_mfi(struct tegra_vi_mfi_ctx **mfi_ctx);
#endif
