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

#ifndef T241_TEST_H
#define T241_TEST_H

#include "nv_soc_hwpm_test.h"

#define T241_MAX_SOCKETS 4

class T241Tests : public NvSocHwpmTests
{
public:
	T241Tests();
	~T241Tests() override;

protected:
	struct PmaConfigurationParams
	{
		PmaConfigurationParams()
		{
			enable_streaming = false;
			pulse_interval = 0;
			enable_pma_record = false;
			keep_latest = false;
		}

		bool enable_streaming;
		uint32_t pulse_interval;
		bool enable_pma_record;
		bool keep_latest;
	};

	struct PmmConfigurationParams
	{
		enum Mode {
			MODE_B,
			MODE_C,
			MODE_E
		};

		PmmConfigurationParams()
		{
			mode = MODE_B;
			perfmon_idx = 0;
			enable_local_triggering = false;
			enable_overflow_priming = false;
			collect_one = false;
		}


		Mode mode;
		uint32_t perfmon_idx;

		bool enable_local_triggering;
		bool enable_overflow_priming;
		bool collect_one;
	};

	void SetUp() override;
	void TearDown() override;

	void GetDevices();

	void TestRegopsRead(nv_soc_hwpm_session session,
		uint64_t pma_record_buffer_pma_va,
		size_t record_buffer_size);
	void TestRegopsWrite(nv_soc_hwpm_session session);

	void RegOpWrite32(
		nv_soc_hwpm_session session, uint64_t address, uint32_t value, uint32_t mask);
	void RegOpRead32(
		nv_soc_hwpm_session session, uint64_t address, uint32_t *value);

	void SetupPma(nv_soc_hwpm_session session, const PmaConfigurationParams& params);
	void EnablePmaStreaming(nv_soc_hwpm_session session, const PmaConfigurationParams& params);
	void SetupPmm(nv_soc_hwpm_session session, const PmmConfigurationParams& params);
	void SetupWatchbus(nv_soc_hwpm_session session, const PmmConfigurationParams& params);
	void TeardownPma(nv_soc_hwpm_session session);
	void TeardownPmm(nv_soc_hwpm_session session, const PmmConfigurationParams& params);
	void TeardownPerfmux(nv_soc_hwpm_session session);
	void IssuePmaTrigger(nv_soc_hwpm_session session);
	void HarvestCounters(
		nv_soc_hwpm_session session,
		const PmmConfigurationParams& params,
		const uint32_t sig_val[4]);

	nv_soc_hwpm_device t241_dev[T241_MAX_SOCKETS];
	uint32_t t241_dev_count;
};

#endif // T241_TEST_H