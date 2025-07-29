# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2022-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
#
# Display Controller Engine code.
#
GCOV_PROFILE := y

ccflags-y += -I$(srctree.nvidia-oot)/drivers/platform/tegra/dce/include
ccflags-y += -Werror

obj-m += tegra-dce.o
tegra-dce-objs += \
	dce-ast.o \
	dce-reset.o \
	dce-hsp-smb.o \
	dce-hsp-ss.o \
	dce-worker.o \
	dce-fsm.o \
	dce-init-deinit.o \
	dce-mailbox.o \
	dce-bootstrap.o \
	dce-admin.o \
	dce-ipc.o \
	dce-ipc-signal.o \
	dce-client-ipc.o \
	dce-module.o \
	dce-pm.o \
	dce-util-common.o

ifeq ($(CONFIG_DEBUG_FS),y)
tegra-dce-objs += \
	dce-debug.o \
	dce-admin-debug.o \
	dce-debug-perf.o
endif
