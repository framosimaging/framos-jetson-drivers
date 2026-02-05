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

#ifndef HWPM_RECORD_FORMAT_H
#define HWPM_RECORD_FORMAT_H

#include <assert.h>
#include <stdint.h>

// Copied from /sw/devtools/Agora/Dev/Perfkit/Shared/Perfkit/Modules/Target/Counters/Hwpm/Inc/Perfkit/Counters/Hwpm/TargetMicroPassHwpm.h

// =============================================================================
// Mode E Record
// =============================================================================

// Basic mode E record in the native HW layout
struct ModeERecordRaw
{
    union
    {
        uint64_t timestamp;
        struct
        {
            uint32_t timestamp_lo;
            uint8_t timestamp_hi;
            uint8_t perfmon_id;
            uint16_t smplcnt_ds_sz;
        };
    };

    // counter results
    union
    {
        uint32_t counter[4];
        struct
        {
            uint32_t event;
            uint32_t trig0;
            uint32_t trig1;
            uint32_t sampl;
        };
    };

    uint32_t zero2;
    uint32_t zero3;
};

/*  Imported from \\hw\nvgpu_gvlit1\ip\perf\hwpm\2.0\defs\public\registers\pri_perf_pmm.ref with below corrections
    1) timestamp[40:32] => timestamp[39:32]

        15                     8 7                     0
        .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----.
0x00    |                timestamp[15:0]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x02    |                timestamp[31:16]               |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x04    |    PERFMONID[7:0]     |   timestamp[39:32]    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x06    |0 |0 |SZ|DS|PMID[10:8]|     SMPLCNT[8:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x08    |                  count0[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0A    |                  count0[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0C    |                  count1[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0E    |                  count1[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x10    |                  count2[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x12    |                  count2[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x14    |                  count3[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x16    |                  count3[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x18    |           pm local trigger B count            |  <-- or TOTAL_TRIG_RCV[15:0] if PMLOCALTRIGB_EN_DISABLE
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1A    |                   bookmark B                  |  <-- or TOTAL_TRIG_RCV[31:0] if PMLOCALTRIGB_EN_DISABLE
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1C    |           pm local trigger A count            |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1E    |                   bookmark A                  |
        `--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----'
*/

struct ModeERecordVolta : ModeERecordRaw
{
    static const uint64_t TimestampMask = 0x000000ffffffffffull;  // lower 40 bits

    uint64_t GetTimestamp() const
    {
        const uint64_t timestampMasked = timestamp & TimestampMask;
        return timestampMasked;
    }

    uint16_t GetSampleCount() const
    {
        uint16_t sampleCount = smplcnt_ds_sz & 0x01FF;
        return sampleCount;
    }

    uint32_t GetPerfmonId() const
    {
        uint32_t perfmonId_lsb = perfmon_id;
        uint32_t perfmonId_msb = (smplcnt_ds_sz & 0xe00) >> 1;
        uint32_t perfmonId = perfmonId_msb | perfmonId_lsb;
        return perfmonId;
    }

    uint32_t GetTriggerCount() const
    {
        return zero2;
    }

    bool IsDelayedSampled() const
    {
        uint8_t ds = (smplcnt_ds_sz >> 12) & 0x1;
        return !!ds;
    }

    bool Is32B() const
    {
        uint8_t sz = (smplcnt_ds_sz >> 13) & 0x1;
        return !sz;
    }
};

// Basic mode E userdata record in the native HW layout
struct ModeERecordUserData
{
	union
	{
		uint32_t data0_3;
		struct
		{
			uint8_t data0;
			uint8_t data1;
			uint8_t data2;
			uint8_t data3;
		};
	};

	union
	{
		uint32_t meta;
		struct
		{
			uint8_t cnt_d0_2;
			uint8_t perfmon_id;
			uint16_t d3_11_pid_sd_tm;
		};
	};

	union
	{
		uint32_t data4_7;
		struct
		{
			uint8_t data4;
			uint8_t data5;
			uint8_t data6;
			uint8_t data7;
		};
	};

	union
	{
		uint32_t data8_11;
		struct
		{
			uint8_t data8;
			uint8_t data9;
			uint8_t data10;
			uint8_t data11;
		};
	};

	union
	{
		uint32_t data12_15;
		struct
		{
			uint8_t data12;
			uint8_t data13;
			uint8_t data14;
			uint8_t data15;
		};
	};

	union
	{
		uint32_t data16_19;
		struct
		{
			uint8_t data16;
			uint8_t data17;
			uint8_t data18;
			uint8_t data19;
		};
	};

	union
	{
		uint32_t data20_23;
		struct
		{
			uint8_t data20;
			uint8_t data21;
			uint8_t data22;
			uint8_t data23;
		};
	};

	union
	{
		uint32_t data24_27;
		struct
		{
			uint8_t data24;
			uint8_t data25;
			uint8_t data26;
			uint8_t data27;
		};
	};

	/* The number of bytes in data0-27.*/
	uint32_t GetCount() const
	{
		uint32_t count = cnt_d0_2 & 0x1F;
        	return count;
	}

	uint32_t GetPerfmonId() const
	{
		uint32_t perfmonId_lsb = perfmon_id;
		uint32_t perfmonId_msb = (d3_11_pid_sd_tm & 0xe00) >> 1;
		uint32_t perfmonId = perfmonId_msb | perfmonId_lsb;
		return perfmonId;
	}

	uint32_t GetDropped() const
	{
		uint32_t dropped_lsb = cnt_d0_2 >> 5U;
		uint32_t dropped_msb = (d3_11_pid_sd_tm & 0x1FFU) << 3;
		uint32_t dropped = dropped_msb | dropped_lsb;
		return dropped;
	}

	uint32_t GetSD() const
	{
		uint8_t sd = (d3_11_pid_sd_tm >> 12U) & 0x1U;
		return sd;
	}

	uint32_t GetTM() const
	{
		uint8_t sd = (d3_11_pid_sd_tm >> 14U) & 0x1U;
		return sd;
	}
};

/*  MODE E user data packet
Mode E while in USERDATA mode, see NV_PERF_PMM_CONTROL2_MODEE_USERDATA_ENABLED --

Detailed description : http://p4viewer.nvidia.com/get/hw/doc/gpu/maxwell/maxwell/design/IAS/Maxwell_HWPM_IAS.doc, section 3.4.

        15                     8 7                     0
        .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----.
0x00    |       data1[7:0]      |     data0[7:0]        |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x02    |       data3[7:0]      |     data2[7:0]        |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x04    |    PERFMONID[7:0]     | d[2:0] |   cnt[4:0]   | d[2:0] is lower 3 bits of dropped[13:0] field
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x06    |0 |TM|0 |SD|PI[10:8] |     dropped[11:3]       | PI is upper bits of PERFMONID
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x08    |        data5[7:0]     |     data4[7:0]        |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0A    |        data7[7:0]     |     data6[7:0]        |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0C    |        data9[7:0]     |     data8[7:0]        |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0E    |        data11[7:0]    |     data10[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x10    |        data13[7:0]    |     data12[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x12    |        data15[7:0]    |     data14[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x14    |        data17[7:0]    |     data16[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x16    |        data19[7:0]    |     data18[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x18    |        data21[7:0]    |     data20[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1A    |        data23[7:0]    |     data22[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1C    |        data25[7:0]    |     data24[7:0]       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1E    |        data27[7:0]    |     data26[7:0]       |
        `--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----'

dropped[11:0] is the number of dropped bytes due to backpressure after this record.
  There is no counting of bytes dropped before this record.  The only time we drop
  bytes before a record is in the interval between
  - PM trigger start, AND
  - First byte of packet (userdata_start)
cnt[4:0] is the number of valid bytes populated in this record.
PERFMONID(lsb) (PI= PERFMONID msb) DS has the same meaning as mode C/E records.
TM is a field spcific only to UserData mode, indicating a missed perf
  trigger. TM is not context switched and is reset to 0 
*/

// =============================================================================
// Mode C Record
// =============================================================================

// Basic Mode C record in the native HW layout
struct ModeCRecordRaw
{
    uint16_t timestamp_15_0;

    union
    {
        uint16_t timestamp_31_16;
        struct
        {
            uint8_t timestamp_23_16;
            uint8_t total_trig_rcv_7_0;
        };
    };

    union
    {
        uint8_t timestamp_39_32;
        uint8_t total_trig_rcv_15_8;
    };

    uint8_t perfmon_id;
    uint16_t smplcnt_ds_sz_or_pmid;

    union
    {
        uint16_t counter[12];
        struct
        {
            uint16_t counter_2B[4];
            uint32_t counter_4B[4];
        };
    };
};

/*      https://p4viewer.nvidia.com/get//hw/nvgpu/ip/perf/hwpm/4.1/defs/public/registers/pri_perf_pmm.ref

           _MODEC_4X16_4X32_DISABLE                          _MODEC_4X16_4X32_RECONFIGURE

        15                     8 7                     0    15                     8 7                      0
        .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----.   .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----.
0x00    |                timestamp[15:0]                |   |                timestamp[15:0]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x02    |                timestamp[31:16]               |   |                timestamp[31:16]               | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x02    |  TOTAL_TRIG_RCV[7:0]  |   timestamp[23:16]    |   |  TOTAL_TRIG_RCV[7:0]  |   timestamp[23:16]    | <-- or TOTAL_TRIG_RCV[7:0] only if TRIGRCV_IN_MODEC is set  
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x04    |    PERFMONID[7:0]     |   timestamp[39:32]    |   |    PERFMONID[7:0]     |   timestamp[39:32]    | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x04    |    PERFMONID[7:0]     | TOTAL_TRIG_RCV[15:8]  |   |    PERFMONID[7:0]     | TOTAL_TRIG_RCV[15:8]  | <-- or TOTAL_TRIG_RCV[15:8] only if TRIGRCV_IN_MODEC is set
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x06    |0 |0 |0 |DS|PMID[10:8]|     SMPLCNT[8:0]       |   |0 |0 |0 |DS|PMID[10:8]|     SMPLCNT[8:0]       | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x08    |                  count0[15:0]                 |   |                  count0[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0A    |                  count1[15:0]                 |   |                  count1[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0C    |                  count2[15:0]                 |   |                  count2[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x0E    |                  count3[15:0]                 |   |                  count3[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x10    |                  count4[15:0]                 |   |                  count4[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x12    |                  count5[15:0]                 |   |                  count4[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x14    |                  count6[15:0]                 |   |                  count6[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x16    |                  count7[15:0]                 |   |                  count6[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x18    |                  count8[15:0]                 |   |                  count8[15:0]                 |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1A    |                  count9[15:0]                 |   |                  count8[31:16]                |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1C    |                  count10[15:0]                |   |                  count10[15:0]                | <-- or 'pm local trigger A count' if NV_PERF_PMM_CONTROLB_PMLOCALTRIGA_EN is enabled 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x1E    |                  count11[15:0]                |   |                  count10[31:16]               | <-- or 'bookmark A' if NV_PERF_PMM_CONTROLB_PMLOCALTRIGA_EN is enabled
        `--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----'   `--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----'
*/

struct ModeCRecordVolta_12x16 : public ModeCRecordRaw
{
    static const uint64_t TimestampMask = 0x000000ffffffffffull;  // lower 40 bits(TRIGRCV_IN_MODEC_DISABLE)

    uint64_t GetTimestamp() const
    {
        const uint64_t timestamp64 = *(uint64_t*)(this);
        const uint64_t timestampMasked = timestamp64 & TimestampMask;
        return timestampMasked;
    }

    uint16_t GetSampleCount() const
    {
        uint16_t sampleCount = smplcnt_ds_sz_or_pmid & 0x01FF;
        return sampleCount;
    }

    uint32_t GetPerfmonId() const
    {
        const uint32_t perfmonId_lsb = perfmon_id;
        const uint32_t perfmonId_msb = (smplcnt_ds_sz_or_pmid & 0xe00) >> 1;
        const uint32_t perfmonId = perfmonId_msb | perfmonId_lsb;
        return perfmonId;
    }

    bool IsDelayedSampled() const
    {
        uint8_t ds = (smplcnt_ds_sz_or_pmid >> 12) & 0x1;
        return !!ds;
    }
};

/*
//hw/nvgpu/ip/perf/hwpm/4.1/defs/public/registers/pri_perf_pma.ref

APPENDIX D - PMA RECORD FORMAT 
 
PMA PERFMONID = 0x7FF  Other Perfmons should not be programmed to use that ID. 
 
        15                     8 7                     0 
        .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----. 
0x00    |                timestamp[15:0]                |            
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x02    |                timestamp[31:16]               | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x04    |    PERFMONID[7:0]     |   timestamp[40:32]    | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x06    |0 |0 |SZ|DR| PMID[10:8]|   unused              |   
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x08    |                  start[15:0]                  | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x0A    |                  start[31:16]                 | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x0C    |                  end[15:0]                    | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x0E    |                  end[31:16]                   | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x10    |                  total[15:0]                  |            
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x12    |                  total[31:16]                 | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x14    |                  bookmark[15:0]               | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x16    |                  bookmark[31:16]              | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x18    |                  PTIMER[10:0]  | 0  0  0  0  0| 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x1A    |                  PTIMER[26:11]                | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x1C    |                  PTIMER[42:27]                | 
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+ 
0x1E    | 0  0  0|         PTIMER[55:43]                | 
        `--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----' 

PTIMER - 56 bits (32 bit granulariy) global timer on fixed frequency clock
incoming from the SOC in gray code format.
The DR bit stands for dropped record. The PMA can only process 1 packet at a time, 
until it goes into the record buffer. All the triggers which come in during this period 
will not have packets generated for each of them.
If the PMA gets multiple triggers in a short window, then it only sends the packet for the first trigger.
The trigger counts are updated to account for the missed triggers.
After it completes processing the first packet, it then sends a packet when a new trigger comes in.
To indicate this, the DR bit will be set to 1 in the packet that is sent next. 
The DR bit will be seen as set when the difference in total trigger counts is more than 1 
in the current and the previous packet.
*/

// copied from //sw/devtools/Agora/Dev/Perfkit/Shared/Perfkit/Modules/Target/Counters/Hwpm/Inc/Perfkit/Counters/Hwpm/TargetMicroPassHwpm.h

// PMA Record
enum { PMA_PerfmonId = 0x7FF }; // Other perfmons should not be programmed to use this ID.

struct PmaRecordRaw
{
    uint32_t timestamp_lo;
    uint8_t timestamp_hi;
    uint8_t perfmon_id;
    uint16_t sz_dr_pmid; // "dr" stands for dropped record

    uint32_t start_trigger_count;
    uint32_t stop_trigger_count;
    uint32_t total_trigger_count;

    uint32_t bookmark;
    uint64_t ptimer;
};

// adapted from PmaRecordTuring in //sw/devtools/Agora/Dev/Perfkit/Shared/Perfkit/Modules/Target/Counters/Hwpm/Inc/Perfkit/Counters/Hwpm/TargetMicroPassHwpm.h

struct PmaRecordSoc : public PmaRecordRaw
{
    uint32_t GetPerfmonId() const
    {
        uint32_t perfmonId_lsb = perfmon_id;
        uint32_t perfmonId_msb = (sz_dr_pmid & 0xe00) >> 1;  // there is an unused bit in position 8
        uint32_t perfmonId = perfmonId_msb | perfmonId_lsb;
        return perfmonId;
    }

    bool IsDroppedRecord() const
    {
        const uint8_t dr = (sz_dr_pmid >> 12) & 0x1;
        return !!dr;
    }

    // Number of start triggers sent to perfmons
    uint32_t GetStartTriggerCount() const
    {
        return start_trigger_count;
    }

    // Number of end/stop triggers sent to perfmons
    uint32_t GetStopTriggerCount() const
    {
        return stop_trigger_count;
    }

    // The total number of triggers including PMA pulses sent to perfmons
    uint32_t GetTotalTriggerCount() const
    {
        return total_trigger_count;
    }

    // The last value from the PmTrigger packet
    uint32_t GetBookmark() const
    {
        return bookmark;
    }

    // TODO: is the PTIMER value in gray code?
    // From empirical testing, it doesn't look that way.
    uint64_t GetPtimer() const
    {
        uint64_t ptimer_val = (ptimer & 0x1fffffffffffffe0ull) >> 5;
#if 0
        // http://morwenn.github.io/cpp-gray/converting-to-and-from-gray-code/
        ptimer_val = ptimer_val ^ (ptimer_val >> 32);
        ptimer_val = ptimer_val ^ (ptimer_val >> 16);
        ptimer_val = ptimer_val ^ (ptimer_val >> 8);
        ptimer_val = ptimer_val ^ (ptimer_val >> 4);
        ptimer_val = ptimer_val ^ (ptimer_val >> 2);
        ptimer_val = ptimer_val ^ (ptimer_val >> 1);
#endif
        return ptimer_val;
    }
};

// adapted from PmRecordVoltaCommonPrefix in //sw/devtools/Agora/Dev/Perfkit/Shared/Perfkit/Modules/Target/Counters/Hwpm/Inc/Perfkit/Counters/Hwpm/TargetMicroPassHwpm.h

struct PmRecordSocCommonPrefix
{
/*
        15                     8 7                     0
        .--+--+--+--+--+--+--+--+--+--+--+--+--+--+-----.
0x00    |                    data0                      |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x02    |                    data1                      |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x04    |    PERFMONID[7:0]     |        data2          |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
0x06    |   data4   |PMID[10:8]|         data3          |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
    uint16_t data0_2B;
    uint16_t data1_2B;
    uint8_t data2_1B;
    uint8_t perfmon_id;
    uint8_t data3_1B;
    uint8_t data4_pmid;

    uint32_t GetPerfmonId() const
    {
        const uint32_t perfmonId_lsb = perfmon_id;
        const uint32_t perfmonId_msb = (data4_pmid & 0xe) << 7;
        const uint32_t perfmonId = perfmonId_msb | perfmonId_lsb;
        return perfmonId;
    }
};
static_assert(sizeof(PmRecordSocCommonPrefix) == 8, "sizeof(PmRecordSocCommonPrefix) == 8B");

#endif
