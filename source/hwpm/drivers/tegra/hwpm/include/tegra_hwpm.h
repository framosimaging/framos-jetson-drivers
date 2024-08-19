/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2021-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
 */

#ifndef TEGRA_HWPM_H
#define TEGRA_HWPM_H

#include <tegra_hwpm_types.h>

#define TEGRA_HWPM_IP_INACTIVE	~(0U)

/* These macro values should match TEGRA_SOC_HWPM_IP_STATUS_* */
#define TEGRA_HWPM_IP_STATUS_VALID	0U
#define TEGRA_HWPM_IP_STATUS_INVALID	1U

/* These macro values should match TEGRA_SOC_HWPM_RESOURCE_STATUS_* */
#define TEGRA_HWPM_RESOURCE_STATUS_INVALID	0U
#define TEGRA_HWPM_RESOURCE_STATUS_VALID	1U

#define TEGRA_HWPM_FUSE_PRODUCTION_MODE_MASK		BIT(0)
#define TEGRA_HWPM_FUSE_SECURITY_MODE_MASK		BIT(1)
#define TEGRA_HWPM_FUSE_HWPM_GLOBAL_DISABLE_MASK	BIT(2)

#ifdef __KERNEL__
struct tegra_hwpm_os_linux;
#else
#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))
struct tegra_hwpm_os_qnx;
#endif
struct tegra_hwpm_mem_mgmt;
struct tegra_hwpm_allowlist_map;
enum tegra_soc_hwpm_ip_reg_op;

/*
 * This is a copy of enum tegra_soc_hwpm_ip uapi structure.
 * It is not a hard requirement as tegra_soc_hwpm_ip is translated to
 * tegra_hwpm_ip_enum before each use.
 */
enum tegra_hwpm_ip_enum {
	TEGRA_HWPM_IP_VI,
	TEGRA_HWPM_IP_ISP,
	TEGRA_HWPM_IP_VIC,
	TEGRA_HWPM_IP_OFA,
	TEGRA_HWPM_IP_PVA,
	TEGRA_HWPM_IP_NVDLA,
	TEGRA_HWPM_IP_MGBE,
	TEGRA_HWPM_IP_SCF,
	TEGRA_HWPM_IP_NVDEC,
	TEGRA_HWPM_IP_NVENC,
	TEGRA_HWPM_IP_PCIE,
	TEGRA_HWPM_IP_DISPLAY,
	TEGRA_HWPM_IP_MSS_CHANNEL,
	TEGRA_HWPM_IP_MSS_GPU_HUB,
	TEGRA_HWPM_IP_MSS_ISO_NISO_HUBS,
	TEGRA_HWPM_IP_MSS_MCF,
	TEGRA_HWPM_IP_APE,
	TEGRA_HWPM_IP_C2C,
	TERGA_HWPM_NUM_IPS
};

/*
 * This is a copy of enum tegra_soc_hwpm_resource uapi structure.
 * It is not a hard requirement as tegra_soc_hwpm_resource is translated to
 * tegra_hwpm_resource_enum before each use.
 */
enum tegra_hwpm_resource_enum {
	TEGRA_HWPM_RESOURCE_VI,
	TEGRA_HWPM_RESOURCE_ISP,
	TEGRA_HWPM_RESOURCE_VIC,
	TEGRA_HWPM_RESOURCE_OFA,
	TEGRA_HWPM_RESOURCE_PVA,
	TEGRA_HWPM_RESOURCE_NVDLA,
	TEGRA_HWPM_RESOURCE_MGBE,
	TEGRA_HWPM_RESOURCE_SCF,
	TEGRA_HWPM_RESOURCE_NVDEC,
	TEGRA_HWPM_RESOURCE_NVENC,
	TEGRA_HWPM_RESOURCE_PCIE,
	TEGRA_HWPM_RESOURCE_DISPLAY,
	TEGRA_HWPM_RESOURCE_MSS_CHANNEL,
	TEGRA_HWPM_RESOURCE_MSS_GPU_HUB,
	TEGRA_HWPM_RESOURCE_MSS_ISO_NISO_HUBS,
	TEGRA_HWPM_RESOURCE_MSS_MCF,
	TEGRA_HWPM_RESOURCE_PMA,
	TEGRA_HWPM_RESOURCE_CMD_SLICE_RTR,
	TEGRA_HWPM_RESOURCE_APE,
	TEGRA_HWPM_RESOURCE_C2C,
	TERGA_HWPM_NUM_RESOURCES
};

/*
 * This structure is copy of struct tegra_soc_hwpm_ip_ops uapi structure.
 * This is not a hard requirement as each value from tegra_soc_hwpm_ip_ops
 * is copied to struct tegra_hwpm_ip_ops.
 */
struct tegra_hwpm_ip_ops {
	/*
	 * Opaque ip device handle used for callback from
	 * SOC HWPM driver to IP drivers. This handle can be used
	 * to access IP driver functionality with the callbacks.
	 */
	void *ip_dev;
	/*
	 * hwpm_ip_pm is callback function to disable/enable
	 * IP driver power management. Before SOC HWPM doing
	 * perf measuremnts, this callback is called with
	 * "disable = true ", so that IP driver will disable IP specific
	 * power management to keep IP driver responsive. Once SOC HWPM is
	 * done with perf measurement, this callaback is called
	 * with "disable = false", so that IP driver can restore back
	 * it's orignal power management.
	 */
	int (*hwpm_ip_pm)(void *dev, bool disable);
	/*
	 * hwpm_ip_reg_op is callback function to do IP
	 * register 32 bit read or write.
	 * For read:
	 *      input : dev - IP device handle
	 *      input : reg_op - TEGRA_SOC_HWPM_IP_REG_OP_READ
	 *      input : inst_element_index - element index within IP instance
	 *      input : reg_offset - register offset
	 *      output: reg_data - u32 read value
	 * For write:
	 *      input : dev - IP device handle
	 *      input : reg_op - TEGRA_SOC_HWPM_IP_REG_OP_WRITE
	 *      input : inst_element_index - element index within IP instance
	 *      input : reg_offset - register offset
	 *      output: reg_data -  u32 write value
	 * Return:
	 *      reg_op success / failure
	 */
	int (*hwpm_ip_reg_op)(void *dev,
				enum tegra_soc_hwpm_ip_reg_op reg_op,
				u32 inst_element_index, u64 reg_offset,
				u32 *reg_data);
	/*
	 * fd - is used to store the file descriptor of the IP devctl node in QNX.
	 * Default is -1, which indicates the IP has no debug node enabled for Reg ops.
	 * Set to 1 if IP has debug node enabled for Reg ops.
	 */
	int fd;
};

/* There are 3 types of HWPM components/apertures */
#define TEGRA_HWPM_APERTURE_TYPE_PERFMUX	0U
#define TEGRA_HWPM_APERTURE_TYPE_BROADCAST	1U
#define TEGRA_HWPM_APERTURE_TYPE_PERFMON	2U
#define TEGRA_HWPM_APERTURE_TYPE_MAX		3U

/*
 * Devices handled by HWPM driver can be divided into 2 categories
 * - HWPM : Components in HWPM device address space
 *          All perfmons, PMA and RTR perfmuxes
 * - IP : Components in IP address space
 *        IP perfmuxes
 *
 * This enum defines MACROS to specify an element to be HWPM or IP
 * and the specific aperture.
 */
enum tegra_hwpm_element_type {
	HWPM_ELEMENT_INVALID,
	HWPM_ELEMENT_PERFMON,
	HWPM_ELEMENT_PERFMUX,
	IP_ELEMENT_PERFMUX,
	IP_ELEMENT_BROADCAST,
};

enum tegra_hwpm_funcs {
	TEGRA_HWPM_INIT_IP_STRUCTURES,
	TEGRA_HWPM_MATCH_BASE_ADDRESS,
	TEGRA_HWPM_FIND_GIVEN_ADDRESS,
	TEGRA_HWPM_UPDATE_IP_INST_MASK,
	TEGRA_HWPM_GET_ALIST_SIZE,
	TEGRA_HWPM_COMBINE_ALIST,
	TEGRA_HWPM_RESERVE_GIVEN_RESOURCE,
	TEGRA_HWPM_BIND_RESOURCES,
	TEGRA_HWPM_UNBIND_RESOURCES,
	TEGRA_HWPM_RELEASE_RESOURCES,
	TEGRA_HWPM_RELEASE_ROUTER,
	TEGRA_HWPM_RELEASE_IP_STRUCTURES
};

struct tegra_hwpm_func_args {
	u64 *alist;
	u64 full_alist_idx;
};

struct allowlist {
	u64 reg_offset;
	bool zero_at_init;
};

struct hwpm_ip_aperture {
	/*
	 * Indicates which domain (HWPM or IP) aperture belongs to,
	 * used for reverse mapping
	 */
	enum tegra_hwpm_element_type element_type;

	/*
	 * Element index : Index of this aperture within the instance
	 * This will be used to update element_fs_mask to indicate availability.
	 */
	u32 element_index_mask;

	/*
	 * Index of the element within the IP instance
	 * For perfmux entries, this index is passed to hwpm_ip_reg_op()
	 */
	u32 element_index;

	/*
	 * Device index corresponding to device node aperture address index
	 * in Device tree or ACPI table.
	 * This is used to map HWPM apertures only
	 */
	u32 device_index;

	/* MMIO device tree aperture - only populated for perfmon */
#ifdef __KERNEL__
	void __iomem *dt_mmio;
#else
	void *dt_mmio;
#endif
	/* DT tree name */
	char name[64];

	/*
	 * MMIO address for the aperture. This address range is present
	 * in the device node.
	 * MMIO addresses can be same as virtual aperture addresses.
	 */
	u64 start_pa;
	u64 end_pa;

	/*
	 * Virtual aperture address
	 * Regops addresses should be in this range.
	 */
	u64 start_abs_pa;
	u64 end_abs_pa;

	/*
	 * Base address of Perfmon Block
	 * All perfmon apertures have identical placement of registers
	 * HWPM read/write logic for perfmons refers to registers in the first
	 * perfmon block. Use this base address value to compute register
	 * offset in HWPM read/write functions.
	 */
	u64 base_pa;

	/* Allowlist */
	u64 alist_size;
	struct allowlist *alist;

	/*
	 * Fake registers for simulation where SOC HWPM is not implemented
	 * Use virtual aperture address values for allocation.
	 */
	u32 *fake_registers;
};

struct hwpm_ip_element_info {
	/* Number of elements per instance */
	u32 num_element_per_inst;

	/*
	 * Static elements in this instance corresponding to aperture
	 * Array size: num_element_per_inst
	 */
	struct hwpm_ip_aperture *element_static_array;

	/*
	 * Ascending instance address range corresponding to elements
	 * NOTE: It is possible that address range of elements in the instance
	 * are not in same sequential order as their indexes.
	 * For example, 0th element of an instance can have start address higher
	 * than element 1. In this case, range_start and range_end should be
	 * initialized in increasing order.
	 */
	u64 range_start;
	u64 range_end;

	/* Element physical address stride for each element of IP instance */
	u64 element_stride;

	/*
	 * Elements that can fit into instance address range.
	 * This gives number of indices in element_arr
	 */
	u32 element_slots;

	/*
	 * Dynamic elements array corresponding to this element
	 * Array size: element_slots pointers
	 */
	struct hwpm_ip_aperture **element_arr;
};

struct hwpm_ip_inst {
	/*
	 * HW inst index : HW instance index of this instance
	 * This mask builds hwpm_ip.inst_fs_mask indicating availability.
	 */
	u32 hw_inst_mask;

	/*
	 * An IP instance is a group of core elements of the IP
	 * Eg., channels in MSS, controllers in PCIe
	 * Performance tracking is counted by 0, 1 or more HWPM components
	 * (perfmux/perfmon) connected to each IP core element.
	 *
	 */
	u32 num_core_elements_per_inst;

	/* Element details specific to this instance */
	struct hwpm_ip_element_info element_info[TEGRA_HWPM_APERTURE_TYPE_MAX];

	/*
	 * IP ops are specific for an instance, used for perfmux and broadcast
	 * register accesses.
	 */
	struct tegra_hwpm_ip_ops ip_ops;

	/*
	 * Some IPs set fuses to indicate floorsweeping info on platforms.
	 * This mask will contain fuse fs info if any.
	 */
	u32 fuse_fs_mask;

	/*
	 * An IP contains perfmux-perfmon groups that correspond to each other.
	 * If a perfmux is present, it indicates that the corresponding
	 * perfmon is present.
	 * This mask is usually updated based on available perfmuxes.
	 * (except for SCF).
	 */
	u32 element_fs_mask;

	/*
	 * dev_name corresponds to the name of the IP debug node that has been
	 * exposed to perform Regops Read/write operation
	 */
	char dev_name[64];
};

struct hwpm_ip_inst_per_aperture_info {
	/*
	 * Ascending IP address range corresponding to instances
	 * NOTE: It is possible that address range of IP instances
	 * are not in same sequential order as their indexes.
	 * For example, 0th instance of an IP can have start address higher
	 * than instance 1. In this case, range_start and range_end should be
	 * initialized in increasing order.
	 */
	u64 range_start;
	u64 range_end;

	/* Aperture address range for each IP instance */
	u64 inst_stride;

	/*
	 * Aperture instances that can fit into IP aperture address range.
	 * This gives number of entries in inst_arr
	 */
	u32 inst_slots;

	/* IP inst aperture array */
	struct hwpm_ip_inst **inst_arr;
};

struct hwpm_ip {
	/* Number of instances */
	u32 num_instances;

	/* Static array of IP instances */
	struct hwpm_ip_inst *ip_inst_static_array;

	/* Instance info corresponding to apertures in this IP */
	struct hwpm_ip_inst_per_aperture_info inst_aperture_info[
		TEGRA_HWPM_APERTURE_TYPE_MAX];

	/*
	 * Indicates fuses this IP depends on
	 * If fuse corresponding to the mask is blown,
	 * set override_enable = true
	 */
	u32 dependent_fuse_mask;

	/* Override IP config based on fuse value */
	bool override_enable;

	/*
	 * IP floorsweep info based on hw index of aperture
	 * NOTE: This mask needs to based on hw instance index because
	 * hwpm driver clients use hw instance index to find aperture
	 * info (start/end address) from hw manual.
	 */
	u32 inst_fs_mask;

	/*
	 * Resource status can be: TEGRA_HWPM_RESOURCE_STATUS_*
	 * - invalid:  resource is not available to be reserved
	 * - valid:    resource exists on the chip
	 * - reserved: resource is reserved
	 * - fault:    resource faulted during reservation
	 */
	u32 resource_status;
	bool reserved;
};

struct tegra_soc_hwpm;

struct tegra_soc_hwpm_chip {
	/* Array of pointers to active IP structures */
	struct hwpm_ip **chip_ips;

	/* Chip HALs */
	bool (*validate_secondary_hals)(struct tegra_soc_hwpm *hwpm);
#ifdef __KERNEL__
	int (*clk_rst_prepare)(struct tegra_hwpm_os_linux *hwpm_linux);
	int (*clk_rst_set_rate_enable)(struct tegra_hwpm_os_linux *hwpm_linux);
	int (*clk_rst_disable)(struct tegra_hwpm_os_linux *hwpm_linux);
	void (*clk_rst_release)(struct tegra_hwpm_os_linux *hwpm_linux);
#else
        int (*clk_rst_prepare)(struct tegra_hwpm_os_qnx *hwpm_qnx);
        int (*clk_rst_set_rate_enable)(struct tegra_hwpm_os_qnx *hwpm_qnx);
        int (*clk_rst_disable)(struct tegra_hwpm_os_qnx *hwpm_qnx);
        void (*clk_rst_release)(struct tegra_hwpm_os_qnx *hwpm_qnx);
#endif
	bool (*is_ip_active)(struct tegra_soc_hwpm *hwpm,
	u32 ip_enum, u32 *config_ip_index);
	bool (*is_resource_active)(struct tegra_soc_hwpm *hwpm,
	u32 res_enum, u32 *config_ip_index);

	u32 (*get_rtr_int_idx)(struct tegra_soc_hwpm *hwpm);
	u32 (*get_ip_max_idx)(struct tegra_soc_hwpm *hwpm);

	int (*extract_ip_ops)(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u64 base_address,
	struct tegra_hwpm_ip_ops *ip_ops, bool available);
	int (*force_enable_ips)(struct tegra_soc_hwpm *hwpm);
	int (*validate_current_config)(struct tegra_soc_hwpm *hwpm);
	int (*get_fs_info)(struct tegra_soc_hwpm *hwpm,
	u32 ip_enum, u64 *fs_mask, u8 *ip_status);
	int (*get_resource_info)(struct tegra_soc_hwpm *hwpm,
	u32 resource_enum, u8 *status);

	int (*init_prod_values)(struct tegra_soc_hwpm *hwpm);
	int (*disable_cg)(struct tegra_soc_hwpm *hwpm);
	int (*enable_cg)(struct tegra_soc_hwpm *hwpm);

	int (*reserve_rtr)(struct tegra_soc_hwpm *hwpm);
	int (*release_rtr)(struct tegra_soc_hwpm *hwpm);

	int (*check_status)(struct tegra_soc_hwpm *hwpm);
	int (*disable_triggers)(struct tegra_soc_hwpm *hwpm);
	int (*perfmon_enable)(struct tegra_soc_hwpm *hwpm,
	struct hwpm_ip_aperture *perfmon);
	int (*perfmon_disable)(struct tegra_soc_hwpm *hwpm,
		struct hwpm_ip_aperture *perfmon);
	int (*perfmux_disable)(struct tegra_soc_hwpm *hwpm,
		struct hwpm_ip_aperture *perfmux);

	int (*disable_mem_mgmt)(struct tegra_soc_hwpm *hwpm);
	int (*enable_mem_mgmt)(struct tegra_soc_hwpm *hwpm);
	int (*invalidate_mem_config)(struct tegra_soc_hwpm *hwpm);
	int (*stream_mem_bytes)(struct tegra_soc_hwpm *hwpm);
	int (*disable_pma_streaming)(struct tegra_soc_hwpm *hwpm);
	int (*update_mem_bytes_get_ptr)(struct tegra_soc_hwpm *hwpm,
		u64 mem_bump);
	u64 (*get_mem_bytes_put_ptr)(struct tegra_soc_hwpm *hwpm);
	bool (*membuf_overflow_status)(struct tegra_soc_hwpm *hwpm);

	size_t (*get_alist_buf_size)(struct tegra_soc_hwpm *hwpm);
	int (*zero_alist_regs)(struct tegra_soc_hwpm *hwpm,
		struct hwpm_ip_inst *ip_inst,
		struct hwpm_ip_aperture *aperture);
	int (*copy_alist)(struct tegra_soc_hwpm *hwpm,
		struct hwpm_ip_aperture *aperture,
		u64 *full_alist,
		u64 *full_alist_idx);
	bool (*check_alist)(struct tegra_soc_hwpm *hwpm,
		struct hwpm_ip_aperture *aperture, u64 phys_addr);
};

/* Driver struct */
struct tegra_soc_hwpm {
	/* Active chip info */
	struct tegra_soc_hwpm_chip *active_chip;

	/* Memory Management */
	struct tegra_hwpm_mem_mgmt *mem_mgmt;
	struct tegra_hwpm_allowlist_map *alist_map;

	/* SW State */
	u32 dbg_mask;
	bool bind_completed;
	bool device_opened;
	bool fake_registers_enabled;
};

#endif /* TEGRA_HWPM_H */
