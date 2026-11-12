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

#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <cstdint>

#include "nv_soc_hwpm_loader.hpp"
#include "hwpm_record_format.h"

enum RecordFormatType
{
    ModeC,
    ModeE,
    ModeE_userdata,
};

// realtime parse-flush
struct SocRealtimeParseFlushData
{
    uint32_t m_num_records_consumed;
    bool m_buffer_wraparound;
    bool m_incomplete_record_detected;
    bool m_malformed_record;
};

class SocModeEBuffer
{
	private:
		void ResetParsedData();
		void ParseRecords();
		void PrintRecord(PmRecordSocCommonPrefix* record, bool is_pma_record,
			enum RecordFormatType format_type);

		nv_soc_hwpm_api_table m_api_table;
		nv_soc_hwpm_session m_session;

		RecordFormatType m_record_format;

		// Parsed data
		uint32_t m_num_valid_records;
		uint32_t m_num_overflow_records;
		uint32_t m_num_pma_records;
		uint32_t m_num_samples;
		bool m_delayed_sample_detected;
		bool m_merged_samples_detected;
		uint64_t m_sum_counter_values;
		uint32_t m_num_local_triggers;
		bool m_zero_timestamp_detected;
		bool m_reversed_trigger_count_detected;
		bool m_local_trigger_bookmark_mismatch;
		std::unordered_map<uint32_t, uint32_t> m_perfmon_id_trigger_count_map;

		uint64_t m_first_pma_timestamp;
		uint64_t m_last_pma_timestamp;

		uint64_t m_first_sys0_timestamp;
		uint64_t m_last_sys0_timestamp;

		uint8_t* m_pma_buffer;
		uint8_t* m_membytes_buffer;

	protected:
		uint32_t m_max_records;
		uint32_t m_unread_head;
		size_t m_pma_buffer_size;

	public:
		void* m_pma_buffer_cpu_va;
		void* m_mem_bytes_buffer_cpu_va;

		SocModeEBuffer(
			nv_soc_hwpm_api_table& api_table,
			nv_soc_hwpm_session session);
		~SocModeEBuffer();
		bool Initialize();
		void SetRecordFormat(const RecordFormatType record_format_type);

		uint32_t GetNumValidRecords();
		uint32_t GetNumOverflowRecords();
		uint32_t GetNumPmaRecords();
		uint32_t GetNumSamples();
		bool IsDelayedSampleDetected();
		bool IsMergedSamplesDetected();
		uint64_t GetCounterValueSum();
		uint32_t GetNumUniquePerfmonID();
		uint32_t GetNumLocalTriggers();
		bool IsZeroTimestampDetected();
		bool IsReversedTriggerCountDetected();
		bool IsLocalTriggerBookmarkMismatchDetected();
		uint32_t GetMemBytes();
		uint64_t GetPmaRecordElapsedTime();
		uint64_t GetSysRecordElapsedCycles();
		uint64_t GetFirstPmaTimestamp();
		uint64_t GetLastPmaTimestamp();

		bool FlushRecordsInBuffer(const uint32_t bytes_to_flush);
		bool RealtimeParseFlush(SocRealtimeParseFlushData& stats, bool verbose);
		void PrintRecords(const size_t num_records_to_print) const;
		void PrintPmaRecords(const size_t num_records_to_print) const;
		void DumpBuffer();
};

