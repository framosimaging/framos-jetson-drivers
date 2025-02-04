# SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
# SPDX-License-Identifier: GPL-2.0-only

obj-m += nvidia/

ifdef CONFIG_PCI
obj-m += marvell/
obj-m += microchip/
ifeq ($(VERSION).$(PATCHLEVEL),5.15)
obj-m += realtek/
endif
endif
