# SPDX-License-Identifier: GPL-2.0-only
# Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

subdir-ccflags-y += -Werror

obj-m += fr_common.o
fr_common-objs := fr_sensor_common.o ./../../i2c/fr_i2c_generic.o

obj-m += fr_lifmd_lvds2mipi_1.o

obj-m += max9295.o
obj-m += max9296.o
obj-m += fr_max96792.o
obj-m += fr_max96793.o
ifeq ($(findstring ack_src,$(NV_BUILD_KERNEL_OPTIONS)),)
obj-m += max96712.o

ifdef CONFIG_MEDIA_SUPPORT
obj-m += ar1335_common.o
obj-m += lt6911uxc.o
obj-m += nv_imx185.o
obj-m += nv_imx219.o
obj-m += nv_imx274.o
obj-m += nv_imx318.o
obj-m += nv_imx390.o
obj-m += nv_imx477.o
obj-m += nv_ov5693.o
obj-m += nv_ar0234.o
obj-m += nv_hawk_owl.o
obj-m += fr_imx296.o
obj-m += fr_imx304.o
obj-m += fr_imx335.o
obj-m += fr_imx412.o
obj-m += fr_imx464.o
obj-m += fr_imx530.o
obj-m += fr_imx565.o
obj-m += fr_imx568.o
obj-m += fr_imx577.o
obj-m += fr_imx585.o
obj-m += fr_imx662.o
obj-m += fr_imx675.o
obj-m += fr_imx676.o
obj-m += fr_imx678.o
obj-m += fr_imx715.o
obj-m += fr_imx900.o
endif

obj-m += pca9570.o
obj-m += virtual_i2c_mux.o
endif
