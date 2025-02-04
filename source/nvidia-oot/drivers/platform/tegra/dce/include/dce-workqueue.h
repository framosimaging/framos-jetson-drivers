/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023, NVIDIA CORPORATION.  All rights reserved.
 */

#ifndef DCE_WORKQUEUE_H
#define DCE_WORKQUEUE_H

#include <linux/workqueue.h>

struct dce_work_struct {
	struct tegra_dce *d;
	struct work_struct work;
	void (*dce_work_fn)(struct tegra_dce *d);
};

int dce_init_work(struct tegra_dce *d,
		   struct dce_work_struct *work,
		   void (*work_fn)(struct tegra_dce *d));
void dce_schedule_work(struct dce_work_struct *work);

#endif
