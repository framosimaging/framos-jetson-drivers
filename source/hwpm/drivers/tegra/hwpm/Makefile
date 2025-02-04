# Copyright (c) 2022-2023, NVIDIA CORPORATION. All rights reserved.
#
# Tegra SOC HWPM
#

GCOV_PROFILE := y

ifeq ($(origin srctree.hwpm), undefined)
srctree.hwpm := $(abspath $(shell dirname $(lastword $(MAKEFILE_LIST))))/../../..
endif

CONFIG_TEGRA_SOC_HWPM := y
ccflags-y += -DCONFIG_TEGRA_SOC_HWPM

CONFIG_TEGRA_T234_HWPM := y
ccflags-y += -DCONFIG_TEGRA_T234_HWPM

NVHWPM_OBJ = m

# For OOT builds, set required config flags
ifeq ($(CONFIG_TEGRA_OOT_MODULE),m)
CONFIG_TEGRA_HWPM_OOT := y
ccflags-y += -DCONFIG_TEGRA_HWPM_OOT

CONFIG_TEGRA_FUSE_UPSTREAM := y
ccflags-y += -DCONFIG_TEGRA_FUSE_UPSTREAM

LINUXINCLUDE += -I$(srctree.nvconftest)
LINUXINCLUDE += -I$(srctree.hwpm)/include
LINUXINCLUDE += -I$(srctree.hwpm)/drivers/tegra/hwpm/include
LINUXINCLUDE += -I$(srctree.hwpm)/drivers/tegra/hwpm

else
ccflags-y += -I$(srctree.nvidia)/include
ccflags-y += -I$(srctree.hwpm)/include
ccflags-y += -I$(srctree.hwpm)/drivers/tegra/hwpm/include
ccflags-y += -I$(srctree.hwpm)/drivers/tegra/hwpm

endif

ifeq ($(NV_BUILD_CONFIGURATION_IS_SAFETY),1)
obj-${NVHWPM_OBJ} += tegra_hwpm_mock.o
else
# Add required objects to nvhwpm object variable
include $(srctree.hwpm)/drivers/tegra/hwpm/Makefile.sources

obj-${NVHWPM_OBJ} += nvhwpm.o

endif
