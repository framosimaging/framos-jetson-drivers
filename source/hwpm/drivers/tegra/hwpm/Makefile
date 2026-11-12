# -*- mode: makefile -*-
#
# Copyright (c) 2022-2025, NVIDIA CORPORATION & AFFILIATES.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
# Tegra SOC HWPM
#

GCOV_PROFILE := y

ifeq ($(origin srctree.hwpm), undefined)
srctree.hwpm := $(abspath $(shell dirname $(lastword $(MAKEFILE_LIST))))/../../..
endif

ifdef CONFIG_TEGRA_KLEAF_BUILD
srctree.nvconftest := $(abspath $(NV_BUILD_KERNEL_NVCONFTEST_OUT))
endif

# For OOT builds, set required config flags
ifeq ($(CONFIG_TEGRA_OOT_MODULE),m)
NVHWPM_OBJ = m

CONFIG_TEGRA_HWPM_OOT := y
ccflags-y += -DCONFIG_TEGRA_HWPM_OOT

CONFIG_TEGRA_FUSE_UPSTREAM := y
ccflags-y += -DCONFIG_TEGRA_FUSE_UPSTREAM

ifneq ($(srctree.nvconftest),)
ccflags-y += -DCONFIG_TEGRA_HWPM_CONFTEST
ccflags-y += -I$(srctree.nvconftest)
endif

else # CONFIG_TEGRA_OOT_MODULE != m
NVHWPM_OBJ = y
endif # CONFIG_TEGRA_OOT_MODULE

# Include paths
ccflags-y += -I$(srctree.hwpm)/include
ccflags-y += -I$(srctree.hwpm)/drivers/tegra/hwpm/include
ccflags-y += -I$(srctree.hwpm)/drivers/tegra/hwpm

# Validate build config to add HWPM module support
ifeq ($(NV_BUILD_CONFIGURATION_IS_SAFETY),1)
nvhwpm-objs := tegra_hwpm_mock.o
else ifeq ($(CONFIG_TEGRA_LINUX_PROD),1)
nvhwpm-objs := tegra_hwpm_mock.o
else ifneq ($(CONFIG_ARCH_TEGRA),y)
nvhwpm-objs := tegra_hwpm_mock.o
else
# Add required objects to nvhwpm object variable
include $(srctree.hwpm)/drivers/tegra/hwpm/Makefile.hwpm.sources
endif

obj-${NVHWPM_OBJ} += nvhwpm.o

ifdef CONFIG_TEGRA_KLEAF_BUILD
KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
M ?= $(shell pwd)

modules modules_install:
	make -C $(KERNEL_SRC) M=$(M) $(ccflags) CONFIG_TEGRA_OOT_MODULE=m $(@)

clean:
	make -C $(KERNEL_SRC) M=$(M) CONFIG_TEGRA_OOT_MODULE=m clean
else
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) $(ccflags) CONFIG_TEGRA_OOT_MODULE=m modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) CONFIG_TEGRA_OOT_MODULE=m clean
endif
