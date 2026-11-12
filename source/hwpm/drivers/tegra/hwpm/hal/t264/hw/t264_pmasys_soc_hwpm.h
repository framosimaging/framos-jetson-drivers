/* SPDX-License-Identifier: MIT */
/*
 * SPDX-FileCopyrightText: Copyright (c) 2023-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This is a generated file. Do not edit.
 */
/*
 * Function/Macro naming determines intended use:
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
#ifndef T264_PMASYS_SOC_HWPM_H
#define T264_PMASYS_SOC_HWPM_H

#define pmasys_channel_control_user_r(i,j)\
				(0x1610a10U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_control_user_update_bytes_f(v)      (((v) & 0x1U) << 16U)
#define pmasys_channel_control_user_update_bytes_m()               (0x1U << 16U)
#define pmasys_channel_control_user_update_bytes_doit_v()          (0x00000001U)
#define pmasys_channel_control_user_update_bytes_doit_f()             (0x10000U)
#define pmasys_channel_control_user_membuf_clear_status_m()         (0x1U << 1U)
#define pmasys_channel_control_user_membuf_clear_status_doit_f()          (0x2U)
#define pmasys_channel_mem_bump_r(i,j) (0x1610a14U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_outbase_r(i,j)  (0x1610a28U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_outbase_ptr_f(v)               (((v) & 0x7ffffffU) << 5U)
#define pmasys_channel_outbase_ptr_m()                        (0x7ffffffU << 5U)
#define pmasys_channel_outbase_ptr_v(r)               (((r) >> 5U) & 0x7ffffffU)
#define pmasys_channel_outbase_ptr_init_f()                               (0x0U)
#define pmasys_channel_outbaseupper_r(i,j)\
				(0x1610a2cU + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_outbaseupper_ptr_f(v)          (((v) & 0x1ffffffU) << 0U)
#define pmasys_channel_outbaseupper_ptr_m()                   (0x1ffffffU << 0U)
#define pmasys_channel_outbaseupper_ptr_v(r)          (((r) >> 0U) & 0x1ffffffU)
#define pmasys_channel_outbaseupper_ptr_init_f()                          (0x0U)
#define pmasys_channel_outsize_r(i,j)  (0x1610a30U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_outsize_numbytes_f(v)          (((v) & 0x7ffffffU) << 5U)
#define pmasys_channel_outsize_numbytes_m()                   (0x7ffffffU << 5U)
#define pmasys_channel_outsize_numbytes_init_f()                          (0x0U)
#define pmasys_channel_mem_head_r(i,j) (0x1610a34U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_mem_head_ptr_m()                       (0xfffffffU << 4U)
#define pmasys_channel_mem_head_ptr_init_f()                              (0x0U)
#define pmasys_channel_mem_bytes_r(i,j)\
				(0x1610a38U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_mem_bytes_numbytes_m()                 (0xfffffffU << 4U)
#define pmasys_channel_mem_bytes_numbytes_init_f()                        (0x0U)
#define pmasys_channel_mem_bytes_addr_r(i,j)\
				(0x1610a3cU + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_mem_bytes_addr_ptr_f(v)       (((v) & 0x3fffffffU) << 2U)
#define pmasys_channel_mem_bytes_addr_ptr_m()                (0x3fffffffU << 2U)
#define pmasys_channel_mem_bytes_addr_ptr_init_f()                        (0x0U)
#define pmasys_cblock_bpc_mem_block_r(i)                (0x1611e04U + ((i)*32U))
#define pmasys_cblock_bpc_mem_block_base_m()                 (0xffffffffU << 0U)
#define pmasys_cblock_bpc_mem_blockupper_r(i)           (0x1611e08U + ((i)*32U))
#define pmasys_cblock_bpc_mem_blockupper_valid_f(v)        (((v) & 0x1U) << 31U)
#define pmasys_cblock_bpc_mem_blockupper_valid_false_v()           (0x00000000U)
#define pmasys_cblock_bpc_mem_blockupper_valid_true_v()            (0x00000001U)
#define pmasys_channel_config_user_r(i,j)\
				(0x1610a24U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_config_user_stream_f(v)              (((v) & 0x1U) << 0U)
#define pmasys_channel_config_user_stream_m()                       (0x1U << 0U)
#define pmasys_channel_config_user_stream_disable_f()                     (0x0U)
#define pmasys_channel_config_user_coalesce_timeout_cycles_f(v)\
				(((v) & 0x7U) << 24U)
#define pmasys_channel_config_user_coalesce_timeout_cycles_m()     (0x7U << 24U)
#define pmasys_channel_config_user_coalesce_timeout_cycles__prod_v()\
				(0x00000004U)
#define pmasys_channel_config_user_coalesce_timeout_cycles__prod_f()\
				(0x4000000U)
#define pmasys_channel_status_r(i,j)   (0x1610a00U + ((i) * 128U)) + ((j) * 64U)
#define pmasys_channel_status_engine_status_m()                     (0x7U << 0U)
#define pmasys_channel_status_engine_status_empty_v()              (0x00000000U)
#define pmasys_channel_status_engine_status_empty_f()                     (0x0U)
#define pmasys_channel_status_engine_status_active_v()             (0x00000001U)
#define pmasys_channel_status_engine_status_paused_v()             (0x00000002U)
#define pmasys_channel_status_engine_status_quiescent_v()          (0x00000003U)
#define pmasys_channel_status_engine_status_stalled_v()            (0x00000005U)
#define pmasys_channel_status_engine_status_faulted_v()            (0x00000006U)
#define pmasys_channel_status_engine_status_halted_v()             (0x00000007U)
#define pmasys_channel_status_membuf_status_f(v)           (((v) & 0x1U) << 16U)
#define pmasys_channel_status_membuf_status_m()                    (0x1U << 16U)
#define pmasys_channel_status_membuf_status_v(r)           (((r) >> 16U) & 0x1U)
#define pmasys_channel_status_membuf_status_overflowed_v()         (0x00000001U)
#define pmasys_channel_status_membuf_status_init_f()                      (0x0U)
#define pmasys_command_slice_trigger_start_mask0_r(i)  (0x1611128U + ((i)*144U))
#define pmasys_command_slice_trigger_start_mask0_engine_m()  (0xffffffffU << 0U)
#define pmasys_command_slice_trigger_start_mask0_engine_init_f()          (0x0U)
#define pmasys_command_slice_trigger_start_mask1_r(i)  (0x161112cU + ((i)*144U))
#define pmasys_command_slice_trigger_start_mask1_engine_m()  (0xffffffffU << 0U)
#define pmasys_command_slice_trigger_start_mask1_engine_init_f()          (0x0U)
#define pmasys_command_slice_trigger_stop_mask0_r(i)   (0x1611130U + ((i)*144U))
#define pmasys_command_slice_trigger_stop_mask0_engine_m()   (0xffffffffU << 0U)
#define pmasys_command_slice_trigger_stop_mask0_engine_init_f()           (0x0U)
#define pmasys_command_slice_trigger_stop_mask1_r(i)   (0x1611134U + ((i)*144U))
#define pmasys_command_slice_trigger_stop_mask1_engine_m()   (0xffffffffU << 0U)
#define pmasys_command_slice_trigger_stop_mask1_engine_init_f()           (0x0U)
#define pmasys_command_slice_trigger_config_user_r(i)  (0x161111cU + ((i)*144U))
#define pmasys_command_slice_trigger_config_user_pma_pulse_f(v)\
				(((v) & 0x1U) << 0U)
#define pmasys_command_slice_trigger_config_user_pma_pulse_m()      (0x1U << 0U)
#define pmasys_command_slice_trigger_config_user_pma_pulse_disable_v()\
				(0x00000000U)
#define pmasys_command_slice_trigger_config_user_pma_pulse_disable_f()    (0x0U)
#define pmasys_command_slice_trigger_config_user_record_stream_f(v)\
				(((v) & 0x1U) << 8U)
#define pmasys_command_slice_trigger_config_user_record_stream_m()  (0x1U << 8U)
#define pmasys_command_slice_trigger_config_user_record_stream_disable_v()\
				(0x00000000U)
#define pmasys_command_slice_trigger_config_user_record_stream_disable_f()\
				(0x0U)
#define pmasys_streaming_capabilities1_r()                          (0x16109f4U)
#define pmasys_streaming_capabilities1_local_credits_f(v) (((v) & 0x1ffU) << 0U)
#define pmasys_streaming_capabilities1_local_credits_m()          (0x1ffU << 0U)
#define pmasys_streaming_capabilities1_local_credits_init_v()      (0x00000100U)
#define pmasys_streaming_capabilities1_total_credits_f(v) (((v) & 0x7ffU) << 9U)
#define pmasys_streaming_capabilities1_total_credits_m()          (0x7ffU << 9U)
#define pmasys_streaming_capabilities1_total_credits_v(r) (((r) >> 9U) & 0x7ffU)
#define pmasys_streaming_capabilities1_total_credits_init_f()         (0x20000U)
#define pmasys_command_slice_trigger_mask_secure0_r(i) (0x1611110U + ((i)*144U))
#define pmasys_command_slice_trigger_mask_secure0_engine_f(v)\
				(((v) & 0xffffffffU) << 0U)
#define pmasys_command_slice_trigger_mask_secure0_engine_m() (0xffffffffU << 0U)
#define pmasys_command_slice_record_select_secure_r(i) (0x1611180U + ((i)*144U))
#define pmasys_command_slice_record_select_secure_trigger_select_f(v)\
				(((v) & 0x3fU) << 0U)
#define pmasys_command_slice_record_select_secure_trigger_select_m()\
				(0x3fU << 0U)
#define pmasys_profiling_cg2_secure_r()                             (0x1610844U)
#define pmasys_profiling_cg2_secure_slcg_f(v)               (((v) & 0x1U) << 0U)
#define pmasys_profiling_cg2_secure_slcg_m()                        (0x1U << 0U)
#define pmasys_profiling_cg2_secure_slcg_enabled_v()               (0x00000000U)
#define pmasys_profiling_cg2_secure_slcg_enabled_f()                      (0x0U)
#define pmasys_profiling_cg2_secure_slcg__prod_v()                 (0x00000000U)
#define pmasys_profiling_cg2_secure_slcg__prod_f()                        (0x0U)
#define pmasys_profiling_cg2_secure_slcg_disabled_v()              (0x00000001U)
#define pmasys_profiling_cg2_secure_slcg_disabled_f()                     (0x1U)
#define pmasys_profiling_cg1_secure_r()                             (0x1610848U)
#define pmasys_profiling_cg1_secure_flcg_f(v)              (((v) & 0x1U) << 31U)
#define pmasys_profiling_cg1_secure_flcg_m()                       (0x1U << 31U)
#define pmasys_profiling_cg1_secure_flcg_enabled_v()               (0x00000001U)
#define pmasys_profiling_cg1_secure_flcg_enabled_f()               (0x80000000U)
#define pmasys_profiling_cg1_secure_flcg__prod_v()                 (0x00000001U)
#define pmasys_profiling_cg1_secure_flcg__prod_f()                 (0x80000000U)
#define pmasys_profiling_cg1_secure_flcg_disabled_v()              (0x00000000U)
#define pmasys_profiling_cg1_secure_flcg_disabled_f()                     (0x0U)
#endif /* T264_PMASYS_SOC_HWPM_H */
