/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023, NVIDIA CORPORATION.  All rights reserved.
 *
 * Tegra TSEC Module Support
 */

#ifndef TSEC_COMMS_CMDS_H
#define TSEC_COMMS_CMDS_H

struct RM_FLCN_QUEUE_HDR {
	u8      unitId;
	u8      size;
	u8      ctrlFlags;
	u8      seqNumId;
};

#define RM_FLCN_QUEUE_HDR_SIZE          sizeof(struct RM_FLCN_QUEUE_HDR)


#define RM_GSP_UNIT_REWIND      (0x00)
#define RM_GSP_UNIT_INIT        (0x02)
#define RM_GSP_UNIT_HDCP22WIRED (0x06)
#define RM_GSP_UNIT_END         (0x11)

#define RM_GSP_LOG_QUEUE_NUM    (2)

struct RM_GSP_INIT_MSG_GSP_INIT {
	u8      msgType;
	u8      numQueues;
	u16     osDebugEntryPoint;
	struct {
		u32     queueOffset;
		u16     queueSize;
		u8      queuePhyId;
		u8      queueLogId;
	} qInfo[RM_GSP_LOG_QUEUE_NUM];
	u32     rsvd1;
	u8      rsvd2;
	u8      status;
};

#endif /* TSEC_COMMS_CMDS_H */
