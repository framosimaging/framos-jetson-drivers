# SPDX-FileCopyrightText: Copyright (c) 2023-2025, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: GPL-2.0-only
#
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

subdir-ccflags-y += -Werror
ifneq ($(NV_BUILD_SYSTEM_TYPE),embedded-linux)
obj-m += max9295.o
obj-m += max9296.o
ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
obj-m += max96712.o

obj-m += fr_common.o
fr_common-objs := fr_sensor_common.o ./../../i2c/fr_i2c_generic.o

obj-m += fr_lifmd_lvds2mipi_1.o
obj-m += fr_lifcl_var2fixed_1.o
obj-m += fr_max96792.o
obj-m += fr_max96793.o

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
obj-m += max929x.o
obj-m += max96712_tpg.o
obj-m += max96726_tpg.o
obj-m += fr_imx283.o
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
obj-m += fr_imx636.o
obj-m += fr_imx662.o
obj-m += fr_imx675.o
obj-m += fr_imx676.o
obj-m += fr_imx678.o
obj-m += fr_imx715.o
obj-m += fr_imx838.o
obj-m += fr_imx900.o
endif

obj-m += pca9570.o
obj-m += virtual_i2c_mux.o
endif
endif
