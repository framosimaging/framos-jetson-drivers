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

#include "soc_mode_e_buffer.h"
#include "hwpm_record_format.h"

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <deque>

SocModeEBuffer::SocModeEBuffer(nv_soc_hwpm_api_table& api_table, nv_soc_hwpm_session session)
    : m_api_table(api_table)
    , m_session(session)
    , m_record_format(RecordFormatType::ModeE)
    , m_num_valid_records(0)
    , m_num_overflow_records(0)
    , m_num_pma_records(0)
    , m_num_samples(0)
    , m_delayed_sample_detected(false)
    , m_merged_samples_detected(false)
    , m_sum_counter_values(0)
    , m_zero_timestamp_detected(false)
    , m_reversed_trigger_count_detected(false)
    , m_local_trigger_bookmark_mismatch(false)
    , m_first_pma_timestamp(0)
    , m_last_pma_timestamp(0)
    , m_first_sys0_timestamp(0)
    , m_last_sys0_timestamp(0)
    , m_pma_buffer(nullptr)
    , m_membytes_buffer(nullptr)
    , m_max_records(0)
    , m_unread_head(0)
    , m_pma_buffer_size(0)
    , m_pma_buffer_cpu_va(nullptr)
    , m_mem_bytes_buffer_cpu_va(nullptr)
{
}

SocModeEBuffer::~SocModeEBuffer()
{
}

bool SocModeEBuffer::Initialize()
{
	nv_soc_hwpm_session_attribute session_attr;

	session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE;
	if (m_api_table.nv_soc_hwpm_session_get_info_fn(
		m_session,
		session_attr,
		sizeof(m_pma_buffer_size),
		&m_pma_buffer_size)) {
		printf("ERROR: SOC HWPM session get info buffer size failed!\n");
		return false;
	}

	session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA;
	if (m_api_table.nv_soc_hwpm_session_get_info_fn(
		m_session,
		session_attr,
		sizeof(m_pma_buffer_cpu_va),
		&m_pma_buffer_cpu_va)) {
		printf("ERROR: SOC HWPM session get info stream buffer ptr failed!\n");
		return false;
	}

	session_attr = NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA;
	if (m_api_table.nv_soc_hwpm_session_get_info_fn(
		m_session,
		session_attr,
		sizeof(m_mem_bytes_buffer_cpu_va),
		&m_mem_bytes_buffer_cpu_va)) {
		printf("ERROR: SOC HWPM session get info mem bytes buffer ptr failed!\n");
		return false;
	}

	m_max_records = m_pma_buffer_size / sizeof(ModeERecordRaw);

	return true;
}

void SocModeEBuffer::ResetParsedData()
{
    m_num_valid_records = 0;
    m_num_overflow_records = 0;
    m_num_pma_records = 0;
    m_num_samples = 0;
    m_delayed_sample_detected = false;
    m_merged_samples_detected = false;
    m_sum_counter_values = 0;
    m_num_local_triggers = 0;
    m_zero_timestamp_detected = false;
    m_reversed_trigger_count_detected = false;
    m_local_trigger_bookmark_mismatch = false;
    m_perfmon_id_trigger_count_map.clear();

    m_first_pma_timestamp = 0;
    m_last_pma_timestamp = 0;
    m_first_sys0_timestamp = 0;
    m_last_sys0_timestamp = 0;
}

void SocModeEBuffer::SetRecordFormat(const RecordFormatType record_format_type)
{
	m_record_format = record_format_type;
}

// Copied from Perfkit\Shared\Perfkit\Tests\Emulation\SOC\Tests\SocCrossTriggerTest\Src\SocModeEBuffer.cpp
#define LOCAL_TRIGGER_BOOKMARK_VAL 0xBEEF
void SocModeEBuffer::ParseRecords()
{
	ResetParsedData();

	// init buffer parsing variables
	uint32_t record_idx_lo = m_unread_head;
	uint32_t record_idx_hi = m_max_records;
	bool valid_record_end_detected = false;

	// Buffer wraparound requires two iterations
	for (uint32_t circular_buffer_segment = 0; circular_buffer_segment < 2;
		circular_buffer_segment++) {
		// Iterate records in buffer
		for (uint32_t ii = record_idx_lo; ii < record_idx_hi; ii++) {
			auto p_record = (ModeERecordRaw*)(m_pma_buffer_cpu_va) + ii;
			auto p_record_common_prefix = (PmRecordSocCommonPrefix*)(p_record);
			uint32_t record_perfmon_id = p_record_common_prefix->GetPerfmonId();

			if (record_perfmon_id == PMA_PerfmonId) { // PMA Record
				const PmaRecordSoc* p_pma_record =
					(const PmaRecordSoc*)(p_record_common_prefix);
				const uint64_t timestamp = p_pma_record->GetPtimer();
				if (!m_first_pma_timestamp) {
					m_first_pma_timestamp = timestamp;
				}
				m_last_pma_timestamp = timestamp;
				m_num_pma_records++;
			} else if (record_perfmon_id) { // Mode C/E Record
				if (m_record_format == RecordFormatType::ModeC) {
					auto p_mode_c_record = (ModeCRecordVolta_12x16*)p_record;
					uint16_t sample_count = p_mode_c_record->GetSampleCount();
					bool delayed_sampled = p_mode_c_record->IsDelayedSampled();

					// Mode C record does not have TOTAL_TRIG_RCV
					if (!sample_count && !delayed_sampled) {
						m_num_overflow_records++;
					}

					m_num_valid_records++;
					m_num_samples += sample_count;

					if (sample_count > 1) {
						m_merged_samples_detected = true;
					}
					if (delayed_sampled) {
						m_delayed_sample_detected = true;
					}
					for (uint32_t counter_idx = 0; counter_idx < 12;
						counter_idx++) {
						m_sum_counter_values += p_mode_c_record->counter[counter_idx];
					}

					// Mode C record does not have TOTAL_TRIG_RCV
					m_perfmon_id_trigger_count_map.emplace(record_perfmon_id, 0);

					// Local trigger field checks
					const uint16_t local_trigger_count =
						p_mode_c_record->counter[10];
					m_num_local_triggers += local_trigger_count;

					const uint16_t local_trigger_bookmark =
						p_mode_c_record->counter[11];
					if (local_trigger_bookmark != LOCAL_TRIGGER_BOOKMARK_VAL) {
						m_local_trigger_bookmark_mismatch = true;
					}

					// TODO: Timestamp monotonic increment check
					if (!p_mode_c_record->GetTimestamp()) {
						m_zero_timestamp_detected = true;
					}
				} else if (m_record_format == RecordFormatType::ModeE) {
					auto p_mode_e_record = (ModeERecordVolta*)p_record;
					uint16_t sample_count = p_mode_e_record->GetSampleCount();
					bool delayed_sampled = p_mode_e_record->IsDelayedSampled();
					const uint32_t total_trigger_count = p_mode_e_record->zero2;
					auto map_entry = m_perfmon_id_trigger_count_map.find(
						record_perfmon_id);

					// sys0 timestamp
					if (record_perfmon_id == 0x70B) { // FIXME: halify!!!
						uint64_t timestamp = p_mode_e_record->GetTimestamp();
						if (!m_first_sys0_timestamp) {
							m_first_sys0_timestamp = timestamp;
						}
						m_last_sys0_timestamp = timestamp;
					}

					if (!sample_count && !delayed_sampled &&
						(map_entry == m_perfmon_id_trigger_count_map.end() ||
						map_entry->second == total_trigger_count)) {
						m_num_overflow_records++;
					}

					m_num_valid_records++;
					m_num_samples += sample_count;

					if (sample_count > 1) {
						m_merged_samples_detected = true;
					}
					if (delayed_sampled) {
						m_delayed_sample_detected = true;
					}
					m_sum_counter_values += p_mode_e_record->counter[0] +
						p_mode_e_record->counter[1] +
						p_mode_e_record->counter[2] +
						p_mode_e_record->counter[3];

					// TOTAL_TRIG_RCV monotonic check
					if (map_entry != m_perfmon_id_trigger_count_map.end()) {
						if (map_entry->second >= total_trigger_count) {
							m_reversed_trigger_count_detected = true;
						}
						map_entry->second = total_trigger_count;
					} else {
						m_perfmon_id_trigger_count_map.emplace(record_perfmon_id,
							total_trigger_count);
					}

					// Local trigger field checks
					const uint16_t local_trigger_count =
						p_mode_e_record->zero3 & 0xFFFF;
					m_num_local_triggers += local_trigger_count;

					const uint16_t local_trigger_bookmark =
						p_mode_e_record->zero3 >> 16;
					if (local_trigger_bookmark != LOCAL_TRIGGER_BOOKMARK_VAL) {
						m_local_trigger_bookmark_mismatch = true;
					}

					// TODO: Timestamp monotonic increment check
					if (!p_mode_e_record->GetTimestamp()) {
						m_zero_timestamp_detected = true;
					}
				} else if (m_record_format == RecordFormatType::ModeE_userdata) {
					m_num_valid_records++;
					m_perfmon_id_trigger_count_map.emplace(record_perfmon_id, 0);
				}
			} else {
				// Reach the end of valid records
				valid_record_end_detected = true;
				break;
			}
		}

		if (valid_record_end_detected) {
			// Early escape when buffer wraparound does not occur
			break;
		} else {
			// Buffer wraparound detected! Alter buffer segment bounds
			record_idx_lo = 0;
			record_idx_hi = m_unread_head;
		}
	}
}

uint32_t SocModeEBuffer::GetNumValidRecords()
{
	ParseRecords();
	return m_num_valid_records;
}

uint32_t SocModeEBuffer::GetNumOverflowRecords()
{
	ParseRecords();
	return m_num_overflow_records;
}

uint32_t SocModeEBuffer::GetNumPmaRecords()
{
	ParseRecords();
	return m_num_pma_records;
}

uint32_t SocModeEBuffer::GetNumSamples()
{
	ParseRecords();
	return m_num_samples;
}

bool SocModeEBuffer::IsDelayedSampleDetected()
{
	ParseRecords();
	return m_delayed_sample_detected;
}

bool SocModeEBuffer::IsMergedSamplesDetected()
{
	ParseRecords();
	return m_merged_samples_detected;
}

uint64_t SocModeEBuffer::GetCounterValueSum()
{
	ParseRecords();
	return m_sum_counter_values;
}

uint32_t SocModeEBuffer::GetNumUniquePerfmonID()
{
	ParseRecords();
	return m_perfmon_id_trigger_count_map.size();
}

uint32_t SocModeEBuffer::GetNumLocalTriggers()
{
	ParseRecords();
	return m_num_local_triggers;
}

bool SocModeEBuffer::IsZeroTimestampDetected()
{
	ParseRecords();
	return m_zero_timestamp_detected;
}

bool SocModeEBuffer::IsReversedTriggerCountDetected()
{
	ParseRecords();
	return m_reversed_trigger_count_detected;
}

bool SocModeEBuffer::IsLocalTriggerBookmarkMismatchDetected()
{
	ParseRecords();
	return m_local_trigger_bookmark_mismatch;
}

uint32_t SocModeEBuffer::GetMemBytes()
{
	// Emulation: Wait for in-flight mem_bytes to arrive.
	usleep(10000);

	auto* p_mem_bytes_addr = (uint32_t*)(m_mem_bytes_buffer_cpu_va);
	return *p_mem_bytes_addr;
}

uint64_t SocModeEBuffer::GetPmaRecordElapsedTime()
{
	ParseRecords();
	return (m_last_pma_timestamp - m_first_pma_timestamp);
}

uint64_t SocModeEBuffer::GetSysRecordElapsedCycles()
{
	ParseRecords();
	return (m_last_sys0_timestamp - m_first_sys0_timestamp);
}

uint64_t SocModeEBuffer::GetFirstPmaTimestamp()
{
	ParseRecords();
	return m_first_pma_timestamp;
}

uint64_t SocModeEBuffer::GetLastPmaTimestamp()
{
	ParseRecords();
	return m_last_pma_timestamp;
}

void SocModeEBuffer::PrintRecord(
	PmRecordSocCommonPrefix* record,
	bool is_pma_record,
	enum RecordFormatType format_type)
{
	char str_buffer[256];

	bool is_mode_c = format_type == RecordFormatType::ModeC;
	bool is_mode_e = format_type == RecordFormatType::ModeE;
	bool is_mode_e_userdata = format_type == RecordFormatType::ModeE_userdata;

	if (is_pma_record)
	{
		auto p_pma_record = (PmaRecordSoc*)(record);
		sprintf(str_buffer,
			"[ PMA ] PTIMER %9" PRIu64 ", DR %d, TRIG %d, START %d, END %d, BKMRK %d\n",
			p_pma_record->GetPtimer(),
			p_pma_record->IsDroppedRecord(),
			p_pma_record->GetTotalTriggerCount(),
			p_pma_record->GetStartTriggerCount(),
			p_pma_record->GetStopTriggerCount(),
			p_pma_record->GetBookmark()
		);
	}
	else if (is_mode_c)
	{
		auto p_mode_c_record = (ModeCRecordVolta_12x16*)(record);
		sprintf(str_buffer,
			"[MODEC] PERFMON %3x, TS %9" PRIu64 ", DS %d, SMPLCNT %d, C0 %d, C1 %d, C2 %d, C3 %d, C4 %d, C5 %d, C6 %d, C7 %d, C8 %d, C9 %d, C10 %d, C11 %d\n",
			p_mode_c_record->GetPerfmonId(),
			p_mode_c_record->GetTimestamp(),
			p_mode_c_record->IsDelayedSampled(),
			p_mode_c_record->GetSampleCount(),
			p_mode_c_record->counter[0],
			p_mode_c_record->counter[1],
			p_mode_c_record->counter[2],
			p_mode_c_record->counter[3],
			p_mode_c_record->counter[4],
			p_mode_c_record->counter[5],
			p_mode_c_record->counter[6],
			p_mode_c_record->counter[7],
			p_mode_c_record->counter[8],
			p_mode_c_record->counter[9],
			p_mode_c_record->counter[10],
			p_mode_c_record->counter[11]
		);
	}
	else if (is_mode_e)
	{
		auto p_mode_e_record = (ModeERecordVolta*)(record);
		sprintf(str_buffer,
			"[MODEE] PERFMON %3x, TS %9" PRIu64 ", DS %d, SMPLCNT %d, EVENT %d, TRIG0 %d, TRIG1 %d, SAMPL %d, ZERO2 %x, ZERO3 %x\n",
			p_mode_e_record->GetPerfmonId(),
			p_mode_e_record->GetTimestamp(),
			p_mode_e_record->IsDelayedSampled(),
			p_mode_e_record->GetSampleCount(),
			p_mode_e_record->event,
			p_mode_e_record->trig0,
			p_mode_e_record->trig1,
			p_mode_e_record->sampl,
			p_mode_e_record->zero2,
			p_mode_e_record->zero3
		);
	}
	else if (is_mode_e_userdata)
	{
		auto p_mode_e_userdata_record = (ModeERecordUserData*)(record);
		sprintf(str_buffer,
			"[MODEE_UD] PERFMON %3x, COUNT %4u, DROPPED %d, SD %d, TM %d, DATA 0-3 0x%x, DATA 4-7 0x%x, DATA 8-11 0x%x, DATA 12-15 0x%x, DATA 16-19 0x%x, DATA 20-23 0x%x, DATA 24-27 0x%x\n",
			p_mode_e_userdata_record->GetPerfmonId(),
			p_mode_e_userdata_record->GetCount(),
			p_mode_e_userdata_record->GetDropped(),
			p_mode_e_userdata_record->GetSD(),
			p_mode_e_userdata_record->GetTM(),
			p_mode_e_userdata_record->data0_3,
			p_mode_e_userdata_record->data4_7,
			p_mode_e_userdata_record->data8_11,
			p_mode_e_userdata_record->data12_15,
			p_mode_e_userdata_record->data16_19,
			p_mode_e_userdata_record->data20_23,
			p_mode_e_userdata_record->data24_27
		);
	}
	std::cerr << str_buffer;
}

// This function parses the unread records in the PMA buffer and flushes them
// as a part of the KEEP_LATEST + buffer wraparound testing. Note that the
// parsing/flush operations are done in real time without stopping the PMA
// pulse and without doing MEM_BYTES and MEM_BUMP operations.
bool SocModeEBuffer::RealtimeParseFlush(SocRealtimeParseFlushData& stats, bool verbose)
{
    // clear output stats
    stats = {};

    // initialize buffer parsing and bookkeeping variables
    uint32_t record_idx_lo = m_unread_head;
    uint32_t record_idx_hi = m_max_records;
    bool valid_record_end_detected = false;
    uint64_t last_pma_timestamp = 0;
    uint32_t last_trigger_count = 0;

    // PART 1: Parse unread records
    // ============================

    // Buffer wraparound requires two iterations
    auto p_records = (ModeERecordRaw*)(m_pma_buffer_cpu_va); // FIXME: always Mode E?
    for (uint32_t circular_buffer_segment = 0; circular_buffer_segment < 2; circular_buffer_segment++)
    {
        // Iterate records in buffer
        for (uint32_t ii = record_idx_lo; ii < record_idx_hi; ii++)
        {
            auto p_record_common_prefix = (PmRecordSocCommonPrefix*)(p_records + ii);
            uint32_t record_perfmon_id = p_record_common_prefix->GetPerfmonId();

            if (record_perfmon_id)
            {
                if (record_perfmon_id == PMA_PerfmonId)
                {
                    auto p_pma_record = (PmaRecordSoc*)p_record_common_prefix;
                    uint64_t curr_ptimer = p_pma_record->GetPtimer();
                    uint32_t total_trig_cnt = p_pma_record->GetTotalTriggerCount();

                    if (!curr_ptimer)
                    {
                        // Assume that complete record is not streamed
                        stats.m_incomplete_record_detected = true;
                        if (verbose)
                        {
                            std::cerr << "Incomplete PMA record: ptimer == 0\n";
                            PrintRecord(
				p_record_common_prefix,
				/*is_pma_record*/ true,
				RecordFormatType::ModeC);
                        }
                        break;
                    }
                    else if (!total_trig_cnt)
                    {
                        // Assume that complete record is not streamed
                        stats.m_incomplete_record_detected = true;
                        if (verbose)
                        {
                            std::cerr << "Incomplete PMA record: totalTrigCnt == 0\n";
                            PrintRecord(
				p_record_common_prefix,
				/*is_pma_record*/ true,
				RecordFormatType::ModeC);
                        }
                        break;
                    }
                    else if (curr_ptimer <= last_pma_timestamp)
                    {
                        stats.m_malformed_record = true;
                        if (verbose)
                        {
                            std::cerr << "Malformed PMA record: ptimer " << curr_ptimer << " <= lastPtimer " << last_pma_timestamp << "\n";
                            PrintRecord(
				p_record_common_prefix,
				/*is_pma_record*/ true,
				RecordFormatType::ModeC);
                        }
                        break;
                    }
                    else if (total_trig_cnt <= last_trigger_count)
                    {
                        stats.m_malformed_record = true;
                        if (verbose)
                        {
                            std::cerr << "Malformed PMA record: totalTrigCnt " << (int)total_trig_cnt << " <= lastTriggerCount " << (int)last_trigger_count << "\n";
                            PrintRecord(p_record_common_prefix, /*is_pma_record*/true, RecordFormatType::ModeC);
                        }
                        break;
                    }

                    // Update Ptimer and trigger count for bookkeeping
                    last_pma_timestamp = curr_ptimer;
                    last_trigger_count = total_trig_cnt;
                }
                else
                {
                    auto p_mode_e_record = (ModeERecordVolta*)p_record_common_prefix;
                    uint64_t timestamp = p_mode_e_record->GetTimestamp();
                    const uint32_t total_trigger_count = p_mode_e_record->zero2;
                    auto map_entry = m_perfmon_id_trigger_count_map.find(record_perfmon_id);

                    if (!timestamp)
                    {
                        // Assume that complete record is not streamed
                        stats.m_incomplete_record_detected = true;
                        if (verbose)
                        {
                            std::cerr << "Incomplete ModeE record: timestamp == 0\n";
                            PrintRecord(
				p_record_common_prefix,
				/*is_pma_record*/ false,
				m_record_format);
                        }
                        break;
                    }
                    else if (!total_trigger_count)
                    {
                        // Assume that complete record is not streamed
                        stats.m_incomplete_record_detected = true;
                        if (verbose)
                        {
                            std::cerr << "Incomplete ModeE record: totalTriggerCount == 0\n";
                            PrintRecord(
				p_record_common_prefix,
				/*is_pma_record*/ false,
				m_record_format);
                        }
                        break;
                    }

                    if (map_entry != m_perfmon_id_trigger_count_map.end())
                    {
                        if (map_entry->second >= total_trigger_count)
                        {
                            stats.m_malformed_record = true;
                            if (verbose)
                            {
                                std::cerr << "Malformed ModeE record: totalTriggerCount " << (int)total_trigger_count << " <= lastTriggerCount " << (int)map_entry->second << "\n";
                                PrintRecord(
					p_record_common_prefix,
					/*is_pma_record*/ false,
					m_record_format);
                            }
                            break;
                        }
                        map_entry->second = total_trigger_count;
                    }
                    else
                    {
                        m_perfmon_id_trigger_count_map.emplace(record_perfmon_id, total_trigger_count);
                    }
                }

                // If we reached here, we consumed one more record
                stats.m_num_records_consumed++;
            }
            else
            {
                // Reach the end of valid records
                valid_record_end_detected = true;
                break;
            }
        }

        if (stats.m_malformed_record || stats.m_incomplete_record_detected || valid_record_end_detected)
        {
            // Early escape when incomplete/malformed record is read, or buffer wraparound does not occur
            break;
        }
        else
        {
            // Buffer wraparound detected! Alter buffer segment bounds for the second parsing loop
            stats.m_buffer_wraparound = true;
            record_idx_lo = 0;
            record_idx_hi = m_unread_head;
        }
    }

    // PART 2: Flush
    // =============

    if (stats.m_malformed_record || !stats.m_num_records_consumed)
    {
        return false;
    }

    uint32_t bytes_to_flush = stats.m_num_records_consumed * sizeof(ModeERecordRaw);
    if (m_unread_head + stats.m_num_records_consumed <= m_max_records)  // Single segment
    {
        memset(&p_records[m_unread_head], 0, bytes_to_flush);
	// Move unread head back to 0 if unreadHead + numRecordsConsumed == maxRecords
        m_unread_head = (m_unread_head + stats.m_num_records_consumed) % m_max_records;
    }
    else  // Two segments
    {
        int num_records_first_segment = m_max_records - m_unread_head;
        int num_records_second_segment = stats.m_num_records_consumed - num_records_first_segment;
        memset(&p_records[m_unread_head], 0, sizeof(ModeERecordRaw) * num_records_first_segment);
        memset(&p_records[0], 0, sizeof(ModeERecordRaw) * num_records_second_segment);
        m_unread_head = num_records_second_segment;
    }

    return true;
}

bool SocModeEBuffer::FlushRecordsInBuffer(const uint32_t bytes_to_flush)
{
	// Empty flush
	if (!bytes_to_flush) {
		return true;
	}

	const uint32_t num_records_to_flush = bytes_to_flush / sizeof(ModeERecordRaw);
	auto p_records = (ModeERecordRaw*)(m_pma_buffer_cpu_va);
	if (m_unread_head + num_records_to_flush <= m_max_records)  // Single segment
	{
		memset(&p_records[m_unread_head], 0, bytes_to_flush);
		// Move unread head back to 0 if unreadHead + numRecordsToFlush == maxRecords
		m_unread_head = (m_unread_head + num_records_to_flush) % m_max_records;
	}
	else  // Two segments
	{
		int num_records_first_segment = m_max_records - m_unread_head;
		int num_records_second_segment = num_records_to_flush - num_records_first_segment;
		memset(&p_records[m_unread_head], 0, sizeof(ModeERecordRaw) * num_records_first_segment);
		memset(&p_records[0], 0, sizeof(ModeERecordRaw) * num_records_second_segment);
		m_unread_head = num_records_second_segment;
	}

	if (bytes_to_flush == m_pma_buffer_size) {
		std::cout << "WARNING: Buffer is full! Reset PMA MEM_HEAD to guarantee HW-SW consistency...\n";
		// TODO: reset PMA MEM_HEAD need regops
		return false;
	}

	// Report flushed bytes to HW
	nv_soc_hwpm_pma_channel_state_params param = {};
	param.in_mem_bump = bytes_to_flush;
	param.in_stream_mem_bytes = 0;
	param.in_check_overflow = 0;
	param.in_read_mem_head = 0;
	if (m_api_table.nv_soc_hwpm_session_set_get_pma_state_fn(
		m_session, &param)) {
		std::cerr << "ERROR: SOC HWPM session set get pma state mem bump failed!\n";
		return false;
	}

	return true;
}

void SocModeEBuffer::PrintRecords(const size_t num_records_to_print) const
{
    char str_buffer[256];
    std::deque<std::string> record_strings;
    if (m_record_format == RecordFormatType::ModeC)
    {
        printf("No.  PerfmonID Elaps_cyc DS SmpCt  C0   C1   C2   C3   C4   C5   C6   C7   C8   C9  C10  C11 \n");
        printf("---- --------- --------- -- ----- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
    }
    else if (m_record_format == RecordFormatType::ModeE)
    {
        printf("No.  PerfmonID Elaps_cyc DS SmpCt Count0  Count1  Count2  Count3    TrgB     TrgA  \n");
        printf("---- --------- --------- -- ----- ------- ------- ------- ------- -------- --------\n");
    }
    else if (m_record_format == RecordFormatType::ModeE_userdata)
    {
	printf("No.  PerfmonID Count Dropped SD   TM DATA0-3 DATA4-7 DATA8-11 DATA12-15 DATA16-19 DATA20-23 DATA24-27\n");
        printf("---- --------- ----- ------- ---- -- ------- ------- -------- --------- --------- --------- ---------\n");
    }

    uint32_t record_idx_lo = m_unread_head;
    uint32_t record_idx_hi = m_max_records;
    bool valid_record_end_detected = false;

    for (uint32_t circular_buffer_segment = 0; circular_buffer_segment < 2; circular_buffer_segment++)
    {
        // Iterate records in buffer
        for (uint32_t ii = record_idx_lo; ii < record_idx_hi; ii++)
        {
            auto p_record = (ModeERecordRaw*)(m_pma_buffer_cpu_va) + ii;
            auto p_record_common_prefix = (PmRecordSocCommonPrefix*)(p_record);
            uint32_t perfmon_id = p_record_common_prefix->GetPerfmonId();

            if (perfmon_id && perfmon_id != PMA_PerfmonId) // Mode C/E Record
            {
                if (m_record_format == RecordFormatType::ModeC)
                {
                    auto p_mode_c_record = (ModeCRecordVolta_12x16*)p_record;
                    uint64_t timestamp = p_mode_c_record->GetTimestamp();
                    bool is_delayed_sampled = p_mode_c_record->IsDelayedSampled();
                    uint32_t sample_cnt = p_mode_c_record->GetSampleCount();

                    sprintf(str_buffer,
                        "%4d %9x %9" PRIu64 " %2x %5d %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n",
                        ii,
                        perfmon_id,
                        timestamp,
                        is_delayed_sampled,
                        sample_cnt,
                        p_mode_c_record->counter[0],
                        p_mode_c_record->counter[1],
                        p_mode_c_record->counter[2],
                        p_mode_c_record->counter[3],
                        p_mode_c_record->counter[4],
                        p_mode_c_record->counter[5],
                        p_mode_c_record->counter[6],
                        p_mode_c_record->counter[7],
                        p_mode_c_record->counter[8],
                        p_mode_c_record->counter[9],
                        p_mode_c_record->counter[10],
                        p_mode_c_record->counter[11]
                    );
                }
                else if (m_record_format == RecordFormatType::ModeE)
                {
                    auto p_mode_e_record = (ModeERecordVolta*)p_record;
                    uint64_t timestamp = p_mode_e_record->GetTimestamp();
                    bool is_delayed_sampled = p_mode_e_record->IsDelayedSampled();
                    uint32_t sample_cnt = p_mode_e_record->GetSampleCount();

                    sprintf(str_buffer,
                        "%4d %9x %9" PRIu64 " %2x %5d %7d %7d %7d %7d %8x %8x\n",
                        ii,
                        perfmon_id,
                        timestamp,
                        is_delayed_sampled,
                        sample_cnt,
                        p_mode_e_record->event,
                        p_mode_e_record->trig0,
                        p_mode_e_record->trig1,
                        p_mode_e_record->sampl,
                        p_mode_e_record->zero2,
                        p_mode_e_record->zero3
                    );
                }
                else if (m_record_format == RecordFormatType::ModeE_userdata)
                {
                    auto p_mode_e_userdata_record = (ModeERecordUserData*)(p_record);
                    sprintf(str_buffer,
                        "%4d %9x %5d %7d %4d %2d %7x %7x %8x %9x %9x %9x %9x\n",
			ii,
                        p_mode_e_userdata_record->GetPerfmonId(),
                        p_mode_e_userdata_record->GetCount(),
                        p_mode_e_userdata_record->GetDropped(),
                        p_mode_e_userdata_record->GetSD(),
                        p_mode_e_userdata_record->GetTM(),
                        p_mode_e_userdata_record->data0_3,
                        p_mode_e_userdata_record->data4_7,
                        p_mode_e_userdata_record->data8_11,
                        p_mode_e_userdata_record->data12_15,
                        p_mode_e_userdata_record->data16_19,
                        p_mode_e_userdata_record->data20_23,
                        p_mode_e_userdata_record->data24_27
                    );
                }
                record_strings.emplace_back(std::string(str_buffer));

                if (record_strings.size() > num_records_to_print)
                {
                    record_strings.pop_front();
                }
            }
            else if (perfmon_id == PMA_PerfmonId)
            {
                // Do not print PMA records in this function
                continue;
            }
            else
            {
                // Reach the end of valid records
                valid_record_end_detected = true;
                break;
            }
        }

        if (valid_record_end_detected)
        {
            // Early escape when buffer wraparound does not occur
            break;
        }
        else
        {
            // Buffer wraparound
            record_idx_lo = 0;
            record_idx_hi = m_unread_head;
        }
    }

    for (size_t ii = 0; ii < record_strings.size(); ii++)
    {
        std::cout << record_strings[ii];
    }
}

// Dump the complete buffer starting from beginning to end. Try to parse
// records if possible, but don't count on it. ONLY FOR DEBUG!
void SocModeEBuffer::DumpBuffer()
{
    auto p_records = (ModeERecordRaw*)(m_pma_buffer_cpu_va);
    auto p_record_common_prefix = (PmRecordSocCommonPrefix*)(p_records);
    auto perfmon_id = p_record_common_prefix->GetPerfmonId();

    // Print the first record
    if (perfmon_id == PMA_PerfmonId)
    {
        PrintRecord(
		p_record_common_prefix, true /* isPmaRecord */, m_record_format);
    }
    else
    {
        PrintRecord(
		p_record_common_prefix, false /* isPmaRecord */, m_record_format);
    }

    // Iterate through remaining records
    uint32_t matching_records = 0;
    for (uint32_t ii = 1; ii < m_max_records; ii++)
    {
        auto p_record = (ModeERecordRaw*)(m_pma_buffer_cpu_va) + ii;

        auto p_record_last = (ModeERecordRaw*)(m_pma_buffer_cpu_va) + ii - 1;
        if (memcmp(p_record, p_record_last, sizeof(ModeERecordRaw)) == 0)
        {
            // One more record identical to the previous one: just count it and continue
            ++matching_records;
        }
        else
        {
            // Record mismatch
            if (matching_records)
            {
                // If we have counted identical records, dump the count and reset it
                std::cout << matching_records << " more records identical to the previous one...\n";
                matching_records = 0;
            }

            // Print the new record
            auto p_record_common_prefix = (PmRecordSocCommonPrefix*)(p_record);
            auto perfmon_id = p_record_common_prefix->GetPerfmonId();
            if (perfmon_id == PMA_PerfmonId)
            {
                PrintRecord(
			p_record_common_prefix, true /* isPmaRecord */, m_record_format);
            }
            else
            {
                PrintRecord(
			p_record_common_prefix, false /* isPmaRecord */, m_record_format);
            }
        }
    }

    // If we have accumulated any identical records at the end, print it
    if (matching_records)
    {
        std::cout << matching_records << " more records identical to the previous one...\n";
    }
}

void SocModeEBuffer::PrintPmaRecords(const size_t num_records_to_print) const
{
	char str_buffer[256];
	std::deque<std::string> record_strings;
	printf("No.  Ptimer    DR TotalTrig StartTrig StopTrig  Bookmark \n");
	printf("---- --------- -- --------- --------- --------- ---------\n");

	uint32_t record_idx_lo = m_unread_head;
	uint32_t record_idx_hi = m_max_records;
	bool valid_record_end_detected = false;

	for (uint32_t circular_buffer_segment = 0;
		circular_buffer_segment < 2;
		circular_buffer_segment++)
	{
		// Iterate records in buffer
		for (uint32_t ii = record_idx_lo; ii < record_idx_hi; ii++)
		{
			auto p_records = (ModeERecordVolta*)(m_pma_buffer_cpu_va);
			auto p_record_common_prefix =
				(PmRecordSocCommonPrefix*)(&p_records[ii]);
			uint32_t perfmon_id = p_record_common_prefix->GetPerfmonId();

			if (perfmon_id == PMA_PerfmonId) // PMA Record
			{
				const PmaRecordSoc* p_pma_record =
					(const PmaRecordSoc*)(p_record_common_prefix);
				uint64_t timestamp = p_pma_record->GetPtimer();
				uint32_t total_trig_cnt =
					p_pma_record->GetTotalTriggerCount();
				uint32_t start_trig_cnt =
					p_pma_record->GetStartTriggerCount();
				uint32_t stop_trig_cnt =
					p_pma_record->GetStopTriggerCount();
				uint32_t bookmark = p_pma_record->GetBookmark();
				bool is_dropped_record =
					p_pma_record->IsDroppedRecord();

				sprintf(str_buffer,
					"%4d %9" PRIu64 " %2x %9d %9d %9d %9d\n",
					ii,
					timestamp,
					is_dropped_record,
					total_trig_cnt,
					start_trig_cnt,
					stop_trig_cnt,
					bookmark
				);
				record_strings.emplace_back(std::string(str_buffer));

				if (record_strings.size() > num_records_to_print)
				{
					record_strings.pop_front();
				}
			}
			else if (perfmon_id) // Mode C/E Record
			{
			}
			else
			{
				// Reach the end of valid records
				valid_record_end_detected = true;
				break;
			}
		}

		if (valid_record_end_detected)
		{
			// Early escape when buffer wraparound does not occur
			break;
		}
		else
		{
			// Buffer wraparound
			record_idx_lo = 0;
			record_idx_hi = m_unread_head;
		}
	}

	for (size_t ii = 0; ii < record_strings.size(); ii++)
	{
		std::cout << record_strings[ii];
	}
}
