# SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

KERNEL_HEADERS ?= /lib/modules/$(shell uname -r)/build
KERNEL_OUTPUT ?= ${KERNEL_HEADERS}

MAKEFILE_DIR := $(abspath $(shell dirname $(lastword $(MAKEFILE_LIST))))
NVIDIA_HEADERS ?= ${MAKEFILE_DIR}/out/nvidia-linux-header
NVIDIA_CONFTEST ?= ${MAKEFILE_DIR}/out/nvidia-conftest

ifneq ($(words $(subst :, ,$(MAKEFILE_DIR))), 1)
$(error source directory cannot contain spaces or colons)
endif

NPROC ?= $(shell nproc)

ifeq ("$(wildcard $(KERNEL_OUTPUT))","")
$(error kernel headers/output directory "$(KERNEL_OUTPUT)" does not exist!)
endif

.PHONY : help modules dtbs modules_install

# help is default target!
help:
	@echo   "====================================================================================================================="
	@echo   "Usage:"
	@echo   "   make modules          # to build NVIDIA OOT drivers"
	@echo   "   make dtbs             # to build NVIDIA DTBs"
	@echo   "   make modules_install  # to install drivers to the INSTALL_MOD_PATH"
	@echo   "====================================================================================================================="

modules_install: hwpm modules
	@sudo depmod -a

conftest:
ifeq ($(MAKECMDGOALS), modules)
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - conftest ..."
	@echo   "================================================================================"
	mkdir -p $(NVIDIA_CONFTEST)/nvidia;
	cp -av $(MAKEFILE_DIR)/nvidia-oot/scripts/conftest/* $(NVIDIA_CONFTEST)/nvidia/;
	$(MAKE) -j $(NPROC) ARCH=arm64 \
		src=$(NVIDIA_CONFTEST)/nvidia obj=$(NVIDIA_CONFTEST)/nvidia \
		CC=$(CROSS_COMPILE)gcc LD=$(CROSS_COMPILE)ld \
		NV_KERNEL_SOURCES=$(KERNEL_HEADERS) \
		NV_KERNEL_OUTPUT=$(KERNEL_OUTPUT) \
		-f $(NVIDIA_CONFTEST)/nvidia/Makefile
endif

hwpm: conftest
	@if [ ! -d "$(MAKEFILE_DIR)/hwpm" ] ; then \
		echo "Directory hwpm is not found, exiting.."; \
		false; \
	fi
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - hwpm ..."
	@echo   "================================================================================"
	$(MAKE) -j $(NPROC) ARCH=arm64 \
		-C $(KERNEL_OUTPUT) \
		M=$(MAKEFILE_DIR)/hwpm/drivers/tegra/hwpm \
		CONFIG_TEGRA_OOT_MODULE=m \
		srctree.hwpm=$(MAKEFILE_DIR)/hwpm \
		srctree.nvconftest=$(NVIDIA_CONFTEST) \
		$(MAKECMDGOALS)

modules: conftest hwpm
	@if [ ! -d "$(MAKEFILE_DIR)/nvidia-oot" ] ; then \
		echo "Directory nvidia-oot is not found, exiting.."; \
		false; \
	fi
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - compile nvidia-oot ..."
	@echo   "================================================================================"
	$(MAKE) -j $(NPROC) ARCH=arm64 \
		-C $(KERNEL_OUTPUT) \
		M=$(MAKEFILE_DIR)/nvidia-oot \
		CONFIG_TEGRA_OOT_MODULE=m \
		srctree.nvidia-oot=$(MAKEFILE_DIR)/nvidia-oot \
		srctree.hwpm=$(MAKEFILE_DIR)/hwpm \
		srctree.nvconftest=$(NVIDIA_CONFTEST) \
		KBUILD_EXTRA_SYMBOLS=$(MAKEFILE_DIR)/hwpm/drivers/tegra/hwpm/Module.symvers \
		$(MAKECMDGOALS)

dtbs:
	@echo   "================================================================================"
	@echo   "make compile dtbs ..."
	@echo   "================================================================================"
	TEGRA_TOP=$(MAKEFILE_DIR) \
	srctree=$(KERNEL_HEADERS) \
	objtree=$(KERNEL_OUTPUT) \
	oottree=$(MAKEFILE_DIR)/nvidia-oot \
	HOSTCC=gcc \
	$(MAKE) -f $(MAKEFILE_DIR)/nvidia-oot/scripts/Makefile.build \
		obj=$(MAKEFILE_DIR)/nvidia-oot/device-tree/platform/generic-dts \
		dtbs
	@echo   "================================================================================"
	@echo   "DTBs compiled successfully."
	@echo   "================================================================================"

