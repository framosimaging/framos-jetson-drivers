/* SPDX-License-Identifier: GPL-2.0-only */
/**
 * Copyright (c) 2014-2023, NVIDIA CORPORATION. All rights reserved.
 */

#ifndef __TEGRA_NVADSP_ARAM_MANAGER_H
#define __TEGRA_NVADSP_ARAM_MANAGER_H

#include "mem_manager.h"

int nvadsp_aram_init(unsigned long addr, unsigned long size);
void nvadsp_aram_exit(void);
#endif /* __TEGRA_NVADSP_ARAM_MANAGER_H */
