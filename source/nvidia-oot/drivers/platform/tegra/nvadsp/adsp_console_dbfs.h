/* SPDX-License-Identifier: GPL-2.0-only */
/**
 * Copyright (c) 2014-2023, NVIDIA CORPORATION. All rights reserved.
 */

#ifndef ADSP_CNSL_DBFS_H
#define ADSP_CNSL_DBFS_H

struct nvadsp_cnsl {
	struct device *dev;
	struct nvadsp_mbox shl_snd_mbox;
	struct nvadsp_mbox app_mbox;
};

int
adsp_create_cnsl(struct dentry *adsp_debugfs_root, struct nvadsp_cnsl *cnsl);

#endif /* ADSP_CNSL_DBFS_H */
