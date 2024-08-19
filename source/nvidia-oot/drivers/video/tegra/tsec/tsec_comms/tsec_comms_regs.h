/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022-2023, NVIDIA CORPORATION.  All rights reserved.
 *
 * Tegra TSEC Module Support
 */

/*
 * Function naming determines intended use:
 *
 *     <x>_r(void) : Returns the offset for register <x>.
 *
 *     <x>_o(void) : Returns the offset for element <x>.
 *
 *     <x>_w(void) : Returns the word offset for word (4 byte) element <x>.
 *
 *     <x>_<y>_s(void) : Returns size of field <y> of register <x> in bits.
 *
 *     <x>_<y>_f(u32 v) : Returns a value based on 'v' which has been shifted
 *         and masked to place it at field <y> of register <x>.  This value
 *         can be |'d with others to produce a full register value for
 *         register <x>.
 *
 *     <x>_<y>_m(void) : Returns a mask for field <y> of register <x>.  This
 *         value can be ~'d and then &'d to clear the value of field <y> for
 *         register <x>.
 *
 *     <x>_<y>_<z>_f(void) : Returns the constant value <z> after being shifted
 *         to place it at field <y> of register <x>.  This value can be |'d
 *         with others to produce a full register value for <x>.
 *
 *     <x>_<y>_v(u32 r) : Returns the value of field <y> from a full register
 *         <x> value 'r' after being shifted to place its LSB at bit 0.
 *         This value is suitable for direct comparison with other unshifted
 *         values appropriate for use in field <y> of register <x>.
 *
 *     <x>_<y>_<z>_v(void) : Returns the constant value for <z> defined for
 *         field <y> of register <x>.  This value is suitable for direct
 *         comparison with unshifted values appropriate for use in field <y>
 *         of register <x>.
 */

#ifndef TSEC_COMMS_REGS_H
#define TSEC_COMMS_REGS_H

static inline u32 tsec_cmdq_head_r(u32 r)
{
	/* NV_PSEC_QUEUE_HEAD_0 */
	return (0x1c00+(r)*8);
}

static inline u32 tsec_cmdq_tail_r(u32 r)
{
	/* NV_PSEC_QUEUE_TAIL_0 */
	return (0x1c04+(r)*8);
}

static inline u32 tsec_msgq_head_r(u32 r)
{
	/* NV_PSEC_MSGQ_HEAD_0 */
	return (0x1c80+(r)*8);
}

static inline u32 tsec_msgq_tail_r(u32 r)
{
	/* NV_PSEC_MSGQ_TAIL_0 */
	return (0x1c84+(r)*8);
}

static inline u32 tsec_ememc_r(u32 r)
{
	/* NV_PSEC_EMEMC_0 */
	return (0x1ac0+(r)*8);
}

static inline u32 tsec_ememd_r(u32 r)
{
	/* NV_PSEC_EMEMD_0 */
	return (0x1ac4+(r)*8);
}

#endif /* TSEC_COMMS_REGS_H */
