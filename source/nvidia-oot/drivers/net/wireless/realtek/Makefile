# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

ifeq ($(findstring ack_src,$(NV_BUILD_KERNEL_OPTIONS)),)
ifeq ($(VERSION).$(PATCHLEVEL),5.15)
ifneq ($(NV_OOT_REALTEK_RTL8822CE_SKIP_BUILD),y)
obj-m += rtl8822ce/
endif
endif
endif
