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
#ifndef T264_PMMSYS_SOC_HWPM_H
#define T264_PMMSYS_SOC_HWPM_H

#define pmmsys_perdomain_offset_v()                                (0x00001000U)
#define pmmsys_user_channel_register_stride_v()                    (0x00000020U)
#define pmmsys_num_user_command_slices_v()                         (0x00000002U)
#define pmmsys_num_cblocks_v()                                     (0x00000001U)
#define pmmsys_num_streaming_channels_v()                          (0x00000002U)
#define pmmsys_num_channels_per_cblock_v()                         (0x00000002U)
#define pmmsys_cblock_stride_v()                                   (0x00000020U)
#define pmmsys_channel_stride_v()                                  (0x00000010U)
#define pmmsys_dg_bitmap_array_size_v()                            (0x00000008U)
#define pmmsys_control_r(i)                           (0x160009cU + ((i)*4096U))
#define pmmsys_control_mode_f(v)                            (((v) & 0x7U) << 0U)
#define pmmsys_control_mode_m()                                     (0x7U << 0U)
#define pmmsys_control_mode_disable_v()                            (0x00000000U)
#define pmmsys_control_mode_disable_f()                                   (0x0U)
#define pmmsys_control_mode_a_v()                                  (0x00000001U)
#define pmmsys_control_mode_b_v()                                  (0x00000002U)
#define pmmsys_control_mode_c_v()                                  (0x00000003U)
#define pmmsys_control_mode_e_v()                                  (0x00000005U)
#define pmmsys_control_mode_null_v()                               (0x00000007U)
#define pmmsys_control_o()                                               (0x9cU)
#define pmmsys_enginestatus_r(i)                      (0x16000c8U + ((i)*4096U))
#define pmmsys_enginestatus_enable_f(v)                     (((v) & 0x1U) << 8U)
#define pmmsys_enginestatus_enable_m()                              (0x1U << 8U)
#define pmmsys_enginestatus_enable_out_v()                         (0x00000001U)
#define pmmsys_enginestatus_enable_out_f()                              (0x100U)
#define pmmsys_enginestatus_o()                                          (0xc8U)
#define pmmsys_secure_config_r(i)                     (0x160012cU + ((i)*4096U))
#define pmmsys_secure_config_o()                                        (0x12cU)
#define pmmsys_secure_config_cmd_slice_id_f(v)             (((v) & 0x1fU) << 0U)
#define pmmsys_secure_config_cmd_slice_id_m()                      (0x1fU << 0U)
#define pmmsys_secure_config_channel_id_f(v)                (((v) & 0x3U) << 8U)
#define pmmsys_secure_config_channel_id_m()                         (0x3U << 8U)
#define pmmsys_secure_config_cblock_id_f(v)                (((v) & 0xfU) << 11U)
#define pmmsys_secure_config_cblock_id_m()                         (0xfU << 11U)
#define pmmsys_secure_config_dg_idx_v(r)                  (((r) >> 16U) & 0xffU)
#define pmmsys_secure_config_mapped_f(v)                   (((v) & 0x1U) << 28U)
#define pmmsys_secure_config_mapped_m()                            (0x1U << 28U)
#define pmmsys_secure_config_mapped_false_f()                             (0x0U)
#define pmmsys_secure_config_mapped_true_f()                       (0x10000000U)
#define pmmsys_secure_config_use_prog_dg_idx_f(v)          (((v) & 0x1U) << 30U)
#define pmmsys_secure_config_use_prog_dg_idx_m()                   (0x1U << 30U)
#define pmmsys_secure_config_use_prog_dg_idx_false_f()                    (0x0U)
#define pmmsys_secure_config_use_prog_dg_idx_true_f()              (0x40000000U)
#define pmmsys_secure_config_command_pkt_decoder_f(v)      (((v) & 0x1U) << 31U)
#define pmmsys_secure_config_command_pkt_decoder_m()               (0x1U << 31U)
#define pmmsys_secure_config_command_pkt_decoder_disable_f()              (0x0U)
#define pmmsys_secure_config_command_pkt_decoder_enable_f()        (0x80000000U)
#define pmmsys_router_user_dgmap_status_secure_r(i)      (0x1612050U + ((i)*4U))
#define pmmsys_router_user_dgmap_status_secure__size_1_v()         (0x00000008U)
#define pmmsys_router_user_dgmap_status_secure_dg_s()                       (1U)
#define pmmsys_router_user_dgmap_status_secure_dg_not_mapped_v()   (0x00000000U)
#define pmmsys_router_user_dgmap_status_secure_dg_mapped_v()       (0x00000001U)
#define pmmsys_router_enginestatus_r()                              (0x1612080U)
#define pmmsys_router_enginestatus_status_f(v)              (((v) & 0x7U) << 0U)
#define pmmsys_router_enginestatus_status_m()                       (0x7U << 0U)
#define pmmsys_router_enginestatus_status_v(r)              (((r) >> 0U) & 0x7U)
#define pmmsys_router_enginestatus_status_empty_v()                (0x00000000U)
#define pmmsys_router_enginestatus_status_active_v()               (0x00000001U)
#define pmmsys_router_enginestatus_status_paused_v()               (0x00000002U)
#define pmmsys_router_enginestatus_status_quiescent_v()            (0x00000003U)
#define pmmsys_router_enginestatus_status_stalled_v()              (0x00000005U)
#define pmmsys_router_enginestatus_status_faulted_v()              (0x00000006U)
#define pmmsys_router_enginestatus_status_halted_v()               (0x00000007U)
#define pmmsys_router_enginestatus_merged_perfmon_status_f(v)\
				(((v) & 0x7U) << 8U)
#define pmmsys_router_enginestatus_merged_perfmon_status_m()        (0x7U << 8U)
#define pmmsys_router_enginestatus_merged_perfmon_status_v(r)\
				(((r) >> 8U) & 0x7U)
#define pmmsys_router_profiling_dg_cg1_secure_r()                   (0x1612094U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_f(v)    (((v) & 0x1U) << 31U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_m()             (0x1U << 31U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg__prod_v()       (0x00000001U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg__prod_f()       (0x80000000U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_disabled_v()    (0x00000000U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_disabled_f()           (0x0U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_enabled_v()     (0x00000001U)
#define pmmsys_router_profiling_dg_cg1_secure_flcg_enabled_f()     (0x80000000U)
#define pmmsys_router_profiling_cg1_secure_r()                      (0x1612098U)
#define pmmsys_router_profiling_cg1_secure_flcg_f(v)       (((v) & 0x1U) << 31U)
#define pmmsys_router_profiling_cg1_secure_flcg_m()                (0x1U << 31U)
#define pmmsys_router_profiling_cg1_secure_flcg__prod_v()          (0x00000001U)
#define pmmsys_router_profiling_cg1_secure_flcg__prod_f()          (0x80000000U)
#define pmmsys_router_profiling_cg1_secure_flcg_disabled_v()       (0x00000000U)
#define pmmsys_router_profiling_cg1_secure_flcg_disabled_f()              (0x0U)
#define pmmsys_router_profiling_cg1_secure_flcg_enabled_v()        (0x00000001U)
#define pmmsys_router_profiling_cg1_secure_flcg_enabled_f()        (0x80000000U)
#define pmmsys_router_perfmon_cg2_secure_r()                        (0x161209cU)
#define pmmsys_router_perfmon_cg2_secure_slcg_f(v)         (((v) & 0x1U) << 31U)
#define pmmsys_router_perfmon_cg2_secure_slcg_m()                  (0x1U << 31U)
#define pmmsys_router_perfmon_cg2_secure_slcg__prod_v()            (0x00000000U)
#define pmmsys_router_perfmon_cg2_secure_slcg__prod_f()                   (0x0U)
#define pmmsys_router_perfmon_cg2_secure_slcg_disabled_v()         (0x00000001U)
#define pmmsys_router_perfmon_cg2_secure_slcg_disabled_f()         (0x80000000U)
#define pmmsys_router_perfmon_cg2_secure_slcg_enabled_v()          (0x00000000U)
#define pmmsys_router_perfmon_cg2_secure_slcg_enabled_f()                 (0x0U)
#define pmmsys_router_profiling_cg2_secure_r()                      (0x1612090U)
#define pmmsys_router_profiling_cg2_secure_slcg_f(v)       (((v) & 0x1U) << 31U)
#define pmmsys_router_profiling_cg2_secure_slcg_m()                (0x1U << 31U)
#define pmmsys_router_profiling_cg2_secure_slcg__prod_v()          (0x00000000U)
#define pmmsys_router_profiling_cg2_secure_slcg__prod_f()                 (0x0U)
#define pmmsys_router_profiling_cg2_secure_slcg_disabled_v()       (0x00000001U)
#define pmmsys_router_profiling_cg2_secure_slcg_disabled_f()       (0x80000000U)
#define pmmsys_router_profiling_cg2_secure_slcg_enabled_v()        (0x00000000U)
#define pmmsys_router_profiling_cg2_secure_slcg_enabled_f()               (0x0U)
#define pmmsys_user_channel_config_secure_r(i,j)\
				(0x16120b8U + ((i) * 32U)) + ((j) * 16U)
#define pmmsys_user_channel_config_secure_hs_credits_m()          (0x1ffU << 0U)
#define pmmsys_user_channel_config_secure_hs_credits_init_f()             (0x0U)
#endif /* T264_PMMSYS_SOC_HWPM_H */
