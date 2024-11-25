# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2022-2023, NVIDIA CORPORATION.  All rights reserved.

LINUXINCLUDE += -DCONFIG_TEGRA_HOST1X

nvhost-isp5-objs = isp5.o
ifeq ($(findstring ack_src,$(NV_BUILD_KERNEL_OPTIONS)),)
obj-m += nvhost-isp5.o
endif
