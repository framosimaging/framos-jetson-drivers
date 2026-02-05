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

#ifndef T410_TEST_H
#define T410_TEST_H

#include "nv_soc_hwpm_test.h"

#include <vector>
#include <cstdint>

#define T410_MAX_SOCKETS 2

class T410Tests : public NvSocHwpmTests
{
public:
	T410Tests();
	~T410Tests() override;

protected:
	struct PmaConfigurationParams {
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

	struct PmmConfigurationParams {
		enum Mode {
			MODE_B,
			MODE_C,
			MODE_E,
			MODE_E_USERDATA
		};

		PmmConfigurationParams()
		{
			mode = MODE_B;
			perfmon_idx = 0;
			perfmon_base = 0;
			enable_local_triggering = false;
			enable_overflow_priming = false;
			collect_one = false;
		}

		std::vector<uint32_t> expected_sig_val;

		Mode mode;
		uint32_t perfmon_idx;
		uint64_t perfmon_base;

		bool enable_local_triggering;
		bool enable_overflow_priming;
		bool collect_one;
	};

	void SetUp(void) override;
	void TearDown(void) override;

	void GetDevices(void);

	void TestRegopsRead(nv_soc_hwpm_session session,
		uint64_t pma_record_buffer_pma_va,
		size_t record_buffer_size);
	void TestRegopsWrite(nv_soc_hwpm_session session);

	void RegOpWrite32(
		nv_soc_hwpm_session session, uint64_t address, uint32_t value, uint32_t mask);
	void RegOpRead32(
		nv_soc_hwpm_session session, uint64_t address, uint32_t *value);

	void SetupPma(nv_soc_hwpm_session session, const PmaConfigurationParams &params);
	void EnablePmaStreaming(nv_soc_hwpm_session session, const PmaConfigurationParams &params);
	void SetupPmm(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void SetupWatchbusPma(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void SetupWatchbusNvtherm(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void SetupWatchbusCsnMbn(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void SetupWatchbusIpmu(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void TeardownPma(nv_soc_hwpm_session session);
	void TeardownPmm(nv_soc_hwpm_session session, const PmmConfigurationParams &params);
	void TeardownPerfmux(nv_soc_hwpm_session session);
	void IssuePmaTrigger(
		nv_soc_hwpm_session session,
		bool halt_before_trigger = false,
		bool halt_after_trigger = false,
		uint32_t delay_after_trigger = 0);
	void HarvestCounters(
		nv_soc_hwpm_session session,
		const PmmConfigurationParams &params,
		const uint32_t sig_val[4]);

	void InitPmmParams(nv_soc_hwpm_resource resource, PmmConfigurationParams &params);
	void ModeBTest(nv_soc_hwpm_resource resource);
	void ModeETest(nv_soc_hwpm_resource resource);
	void ModeETestUserData(nv_soc_hwpm_resource resource);

	nv_soc_hwpm_device t410_dev[T410_MAX_SOCKETS];
	uint32_t t410_dev_count;
};

#endif // T410_TEST_H