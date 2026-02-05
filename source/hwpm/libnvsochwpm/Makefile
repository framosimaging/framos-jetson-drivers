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
CC = gcc
endif

SOURCES = \
	common/log.c \
	os/lnx/nv_soc_hwpm_lnx.c \
	nv_soc_hwpm.c

OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

CFLAGS = -Wall -Wextra -Werror=missing-prototypes -Wswitch -Wformat
CFLAGS += -Wchar-subscripts -Wparentheses  -Wtrigraphs -Wpointer-arith
CFLAGS += -Wmissing-declarations -Wredundant-decls  -Wundef -Wmain
CFLAGS += -Wreturn-type -Wmultichar  -Wunused -Wmissing-braces -Werror

LDFLAGS = -Wall -Wl,--version-script=os/lnx/libnvsochwpm.map

INCLUDES = -I. -I./include -I../include

# This library requires dma_buf headers, which may not be available by default
# under linux include dir. User can specify this argument to point the dma_buf
# header files.
ifneq ($(NV_SOURCE),)
INCLUDES += -I$(NV_SOURCE)/core/include
endif

all: libnvsochwpm.so

# Shared Libs
libnvsochwpm.so: $(OBJECTS)
	$(CC) $(LDFLAGS) -shared -fpic -Wl,-soname,libnvsochwpm.so $^ -o $@
	rm -f $(OBJECTS)

# Objects
$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -std=gnu99 -fpic -c $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f libnvsochwpm.so*
