# SPDX-FileCopyrightText: Copyright (c) 2023-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

KERNEL_HEADERS ?= /lib/modules/$(shell uname -r)/build
KERNEL_OUTPUT ?= $(KERNEL_HEADERS)

MAKEFILE_DIR := $(abspath $(shell dirname $(lastword $(MAKEFILE_LIST))))
NVIDIA_CONFTEST ?= $(MAKEFILE_DIR)/out/nvidia-conftest
NVIDIA_DTS_BUILD_SCRIPTS ?= $(realpath $(MAKEFILE_DIR)/build/nvidia-public/devicetree)

ifdef OE_BUILD
  LD_BFD := $(LD)
else
  AR := $(CROSS_COMPILE)ar
  CC := $(CROSS_COMPILE)gcc
  CXX := $(CROSS_COMPILE)g++
  LD := $(CROSS_COMPILE)ld
  LD_BFD := $(CROSS_COMPILE)ld.bfd
  OBJCOPY := $(CROSS_COMPILE)objcopy
  NPROC ?= $(shell nproc)
  PARALLEL := -j$(NPROC)
endif

HOSTCC ?= gcc
V ?= 0


ifneq ($(words $(subst :, ,$(MAKEFILE_DIR))), 1)
  $(error source directory cannot contain spaces or colons)
endif


.PHONY : help modules modules_install clean conftest hwpm nvidia-oot

# help is default target!
help:
	@echo   "================================================================================"
	@echo   "Usage:"
	@echo   "   make modules          # to build NVIDIA OOT"
	@echo   "   make dtbs             # to build NVIDIA DTBs"
	@echo   "   make modules_install  # to install drivers to the INSTALL_MOD_PATH"
	@echo   "   make clean            # to make clean driver sources"
	@echo   "================================================================================"

modules: hwpm nvidia-oot
dtbs: nvidia-dtbs
modules_install: hwpm nvidia-oot
	@sudo depmod -a
clean: hwpm nvidia-oot nvidia-dtbs-clean conftest-clean


conftest:
ifeq ($(MAKECMDGOALS), modules)
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - conftest ..."
	@echo   "================================================================================"
	mkdir -p $(NVIDIA_CONFTEST)/nvidia;
	cp -av $(MAKEFILE_DIR)/nvidia-oot/scripts/conftest/* $(NVIDIA_CONFTEST)/nvidia/;
	$(MAKE) $(PARALLEL) ARCH=arm64 \
		src=$(NVIDIA_CONFTEST)/nvidia obj=$(NVIDIA_CONFTEST)/nvidia \
		CC="$(CC)" LD="$(LD)" \
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
	$(MAKE) $(PARALLEL) ARCH=arm64 \
		-C $(KERNEL_OUTPUT) \
		M=$(MAKEFILE_DIR)/hwpm/drivers/tegra/hwpm \
		CONFIG_TEGRA_OOT_MODULE=m \
		srctree.hwpm=$(MAKEFILE_DIR)/hwpm \
		srctree.nvconftest=$(NVIDIA_CONFTEST) \
		$(MAKECMDGOALS)

nvidia-oot: conftest hwpm
	@if [ ! -d "$(MAKEFILE_DIR)/nvidia-oot" ] ; then \
		echo "Directory nvidia-oot is not found, exiting.."; \
		false; \
	fi
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - nvidia-oot ..."
	@echo   "================================================================================"
	$(MAKE) $(PARALLEL) ARCH=arm64 \
		-C $(KERNEL_OUTPUT) \
		M=$(MAKEFILE_DIR)/nvidia-oot \
		CONFIG_TEGRA_OOT_MODULE=m \
		srctree.nvidia-oot=$(MAKEFILE_DIR)/nvidia-oot \
		srctree.hwpm=$(MAKEFILE_DIR)/hwpm \
		srctree.nvconftest=$(NVIDIA_CONFTEST) \
		kernel_name=${kernel_name} \
		system_type=l4t \
		KBUILD_EXTRA_SYMBOLS=$(MAKEFILE_DIR)/hwpm/drivers/tegra/hwpm/Module.symvers \
		$(MAKECMDGOALS)

nvidia-dtbs:
	@if [ ! -d "$(NVIDIA_DTS_BUILD_SCRIPTS)" ] ; then \
		echo "Directory $(NVIDIA_DTS_BUILD_SCRIPTS) is not found, exiting.."; \
		false; \
	fi
	@echo   "================================================================================"
	@echo   "make nvidia-dtbs ..."
	@echo   "================================================================================"
	TEGRA_TOP=$(MAKEFILE_DIR) \
	srctree=$(KERNEL_HEADERS) \
	objtree=$(KERNEL_OUTPUT) \
	HOSTCC="$(HOSTCC)" \
	DT_FLAVOR=generic \
	$(MAKE) -f $(NVIDIA_DTS_BUILD_SCRIPTS)/Makefile.build \
		obj=$(NVIDIA_DTS_BUILD_SCRIPTS) \
		dtbs
	@echo   "================================================================================"
	@echo   "DTBs compiled successfully."
	@echo   "================================================================================"

nvidia-dtbs-clean:
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - nvidia-dtbs ..."
	@echo   "================================================================================"
	rm -fr $(NVIDIA_DTS_BUILD_SCRIPTS)/generic-dtbs

conftest-clean:
	@echo   "================================================================================"
	@echo   "make $(MAKECMDGOALS) - conftest ..."
	@echo   "================================================================================"
	rm -fr $(NVIDIA_CONFTEST)
