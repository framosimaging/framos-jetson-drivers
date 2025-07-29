# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

ccflags-y += -Werror


nvhost-vi-tpg-t19x-objs += tpg_t19x.o
ifeq ($(findstring ack_src,$(NV_BUILD_KERNEL_OPTIONS)),)
obj-m += nvhost-vi-tpg-t19x.o
endif
