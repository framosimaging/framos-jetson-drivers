/* SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef NV_SOC_HWPM_NV_SOC_HWPM_TEST_H
#define NV_SOC_HWPM_NV_SOC_HWPM_TEST_H

#include <gtest/gtest.h>
#include "nv_soc_hwpm_loader.hpp"

class NvSocHwpmTests : public ::testing::Test
{
public:
    NvSocHwpmTests();
    ~NvSocHwpmTests() override;
    //global start and stop
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    //individual start and stop
    void SetUp() override;
    void TearDown() override;

    void LoadNvSocHwpm();
    void UnloadNvSocHwpm();

    nv_soc_hwpm_api_table api_table;
};

#endif //NV_SOC_HWPM_NV_SOC_HWPM_TEST_H
