/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Tegra T194 Graphics Host NVCSI 2
 *
 * Copyright (c) 2017-2022 NVIDIA Corporation.  All rights reserved.
 */

#ifndef __NVHOST_NVCSI_T194_H__
#define __NVHOST_NVCSI_T194_H__

struct file_operations;
struct platform_device;

extern const struct file_operations tegra194_nvcsi_ctrl_ops;

int t194_nvcsi_early_probe(struct platform_device *pdev);
int t194_nvcsi_late_probe(struct platform_device *pdev);

#endif
