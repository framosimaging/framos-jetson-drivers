# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

# Free-standing Tegra Camera Kernel Tests
sensor_kernel_tests-m += sensor_dt_test.o
sensor_kernel_tests-m += sensor_dt_test_nodes.o

# Tegra Camera Kernel Tests Utilities
obj-m += utils/tegracam_log.o

# Sensor Kernel Tests Module
obj-m += sensor_kernel_tests.o
sensor_kernel_tests-m += modules/sensor_kernel_tests_core.o
sensor_kernel_tests-m += modules/sensor_kernel_tests_runner.o
