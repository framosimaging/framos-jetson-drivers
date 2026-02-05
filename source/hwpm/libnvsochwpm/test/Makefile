################################################################################
# SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: GPL-2.0-only
#
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
################################################################################

# Borrow the make environment from Linux kernel to support cross compilation.
ifneq ($(KERNEL_SOURCE),)
export srctree := $(KERNEL_SOURCE)
include $(srctree)/tools/scripts/Makefile.include
include $(srctree)/tools/scripts/Makefile.arch
else
CXX = g++
endif

SOURCES = \
	nv_soc_hwpm_test.cpp \
	t241_test.cpp \
	t410_test.cpp \
	soc_mode_e_buffer.cpp

OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -g
CXXFLAGS += -I. -I../include -I./include -I../

ifneq ($(NV_SOURCE),)
# Need to be built using this command:
# ARCH=arm64 CROSS_COMPILE=$P4_GCC/bin/aarch64-linux-gnu- NV_SOURCE=$TEGRA_TOP KERNEL_SOURCE=$TEGRA_TOP/kernel/kernel-pset make
# Example:
# ARCH=arm64 \
# CROSS_COMPILE=$P4ROOT/sw/mobile/tools/linux/linaro//gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- \
# NV_SOURCE=~/repo/platform-enablement-dev-main-20250212 \
# KERNEL_SOURCE=~/repo/platform-enablement-dev-main-20250212/kernel/kernel-pset make
CXXFLAGS += -I$(NV_SOURCE)/3rdparty/google/googletest/googletest/include
CXXFLAGS += -I$(NV_SOURCE)/3rdparty/google/googletest/googletest/

CXXFLAGS += -I$(NV_SOURCE)/hwinc-private/th500/66838280
CXXFLAGS += -I$(NV_SOURCE)/hwinc-private/tb500/84849639

GTEST_SOURCES += \
	$(NV_SOURCE)/3rdparty/google/googletest/googletest/src/gtest-all.cc \
	$(NV_SOURCE)/3rdparty/google/googletest/googletest/src/gtest_main.cc
GTEST_OBJECTS=$(foreach x, $(basename $(GTEST_SOURCES)), $(x).o)
else
# Need to install libgtest-dev
LDFLAGS += -lgtest -lgtest_main
endif

LDFLAGS += -lpthread -ldl

all: nv_soc_hwpm_test

nv_soc_hwpm_test: $(GTEST_OBJECTS) $(OBJECTS) 
	$(CXX) $(OBJECTS) $(GTEST_OBJECTS) $(LDFLAGS) -o $@
	rm -f $(OBJECTS) $(GTEST_OBJECTS)

$(OBJECTS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GTEST_OBJECTS): %.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(GTEST_OBJECTS)
	rm -f nv_soc_hwpm_test
