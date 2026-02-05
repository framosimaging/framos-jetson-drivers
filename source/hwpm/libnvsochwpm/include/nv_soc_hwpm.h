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
 * nv_soc_hwpm.h:
 * This is the userspace API header for the Tegra SOC HWPM driver.
 */

#ifndef __NV_SOC_HWPM_H__
#define __NV_SOC_HWPM_H__

#include <stddef.h>
#include <stdint.h>

#define NV_SOC_HWPM_API_EXPORT

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief List of IPs supported by HWPM.
 * 
 * The list of supported IPs are varied across different SoCs. This enum
 * provides a flattened list of all IPs supported by HWPM.
 * 
 * @note this enum is duplicated from driver header.
 */
typedef enum {
	NV_SOC_HWPM_IP_VI,
	NV_SOC_HWPM_IP_ISP,
	NV_SOC_HWPM_IP_VIC,
	NV_SOC_HWPM_IP_OFA,
	NV_SOC_HWPM_IP_PVA,
	NV_SOC_HWPM_IP_NVDLA,
	NV_SOC_HWPM_IP_MGBE,
	NV_SOC_HWPM_IP_SCF,
	NV_SOC_HWPM_IP_NVDEC,
	NV_SOC_HWPM_IP_NVENC,
	NV_SOC_HWPM_IP_PCIE,
	NV_SOC_HWPM_IP_DISPLAY,
	NV_SOC_HWPM_IP_MSS_CHANNEL,
	NV_SOC_HWPM_IP_MSS_GPU_HUB,
	NV_SOC_HWPM_IP_MSS_ISO_NISO_HUBS,
	NV_SOC_HWPM_IP_MSS_MCF,
	NV_SOC_HWPM_IP_APE,
	NV_SOC_HWPM_IP_C2C,
	NV_SOC_HWPM_IP_SMMU,
	NV_SOC_HWPM_IP_CL2,
	NV_SOC_HWPM_IP_NVLCTRL,
	NV_SOC_HWPM_IP_NVLRX,
	NV_SOC_HWPM_IP_NVLTX,
	NV_SOC_HWPM_IP_MSS_HUB,
	NV_SOC_HWPM_IP_MCF_SOC,
	NV_SOC_HWPM_IP_MCF_C2C,
	NV_SOC_HWPM_IP_MCF_CLINK,
	NV_SOC_HWPM_IP_MCF_CORE,
	NV_SOC_HWPM_IP_MCF_OCU,
	NV_SOC_HWPM_IP_PCIE_XTLQ,
	NV_SOC_HWPM_IP_PCIE_XTLRC,
	NV_SOC_HWPM_IP_PCIE_XALRC,
	NV_SOC_HWPM_IP_UCF_MSW,
	NV_SOC_HWPM_IP_UCF_PSW,
	NV_SOC_HWPM_IP_UCF_CSW,
	NV_SOC_HWPM_IP_UCF_HUB,
	NV_SOC_HWPM_IP_UCF_SCB,
	NV_SOC_HWPM_IP_CPU,		/* CPU instance 0-31 */
	NV_SOC_HWPM_IP_CPU_EXT_0,	/* CPU (extended) instance 32-63 */
	NV_SOC_HWPM_IP_CPU_EXT_1,	/* CPU (extended) instance 64-95 */
	NV_SOC_HWPM_IP_CPU_EXT_2,	/* CPU (extended) instance 96-127 */
	NV_SOC_HWPM_IP_NVTHERM,
	NV_SOC_HWPM_IP_CSN,		/* CSN instance 0-31 */
	NV_SOC_HWPM_IP_CSN_EXT_0,	/* CSN (extended) instance 32-63 */
	NV_SOC_HWPM_IP_CSNH,
	NV_SOC_HWPM_NUM_IPS
} nv_soc_hwpm_ip;

/**
 * @brief The resources which can be reserved for profiling.
 * 
 * The list of supported resources are varied across different SoCs. This enum
 * provides a flattened list of all resources supported by HWPM.
 * 
 * @note this enum is duplicated from driver header.
 */
typedef enum {
	NV_SOC_HWPM_RESOURCE_VI,
	NV_SOC_HWPM_RESOURCE_ISP,
	NV_SOC_HWPM_RESOURCE_VIC,
	NV_SOC_HWPM_RESOURCE_OFA,
	NV_SOC_HWPM_RESOURCE_PVA,
	NV_SOC_HWPM_RESOURCE_NVDLA,
	NV_SOC_HWPM_RESOURCE_MGBE,
	NV_SOC_HWPM_RESOURCE_SCF,
	NV_SOC_HWPM_RESOURCE_NVDEC,
	NV_SOC_HWPM_RESOURCE_NVENC,
	NV_SOC_HWPM_RESOURCE_PCIE,
	NV_SOC_HWPM_RESOURCE_DISPLAY,
	NV_SOC_HWPM_RESOURCE_MSS_CHANNEL,
	NV_SOC_HWPM_RESOURCE_MSS_GPU_HUB,
	NV_SOC_HWPM_RESOURCE_MSS_ISO_NISO_HUBS,
	NV_SOC_HWPM_RESOURCE_MSS_MCF,

	/*
	 * - SYS0 PERMON in RPG_PMG
	 * - PERFMUX: PMA_CHANNEL_PERFMUX_CONFIG_SECURE
	 */
	NV_SOC_HWPM_RESOURCE_PMA,

	/*
	 * - PMA: Everything except PMA_CHANNEL_PERFMUX_CONFIG_SECURE
	 * - RTR: Entire aperture
	 */
	NV_SOC_HWPM_RESOURCE_CMD_SLICE_RTR,
	NV_SOC_HWPM_RESOURCE_APE,
	NV_SOC_HWPM_RESOURCE_C2C,
	NV_SOC_HWPM_RESOURCE_SMMU,
	NV_SOC_HWPM_RESOURCE_CL2,
	NV_SOC_HWPM_RESOURCE_NVLCTRL,
	NV_SOC_HWPM_RESOURCE_NVLRX,
	NV_SOC_HWPM_RESOURCE_NVLTX,
	NV_SOC_HWPM_RESOURCE_MSS_HUB,
	NV_SOC_HWPM_RESOURCE_MCF_SOC,
	NV_SOC_HWPM_RESOURCE_MCF_C2C,
	NV_SOC_HWPM_RESOURCE_MCF_CLINK,
	NV_SOC_HWPM_RESOURCE_MCF_CORE,
	NV_SOC_HWPM_RESOURCE_MCF_OCU,
	NV_SOC_HWPM_RESOURCE_PCIE_XTLQ,
	NV_SOC_HWPM_RESOURCE_PCIE_XTLRC,
	NV_SOC_HWPM_RESOURCE_PCIE_XALRC,
	NV_SOC_HWPM_RESOURCE_UCF_MSW,
	NV_SOC_HWPM_RESOURCE_UCF_PSW,
	NV_SOC_HWPM_RESOURCE_UCF_CSW,
	NV_SOC_HWPM_RESOURCE_UCF_HUB,
	NV_SOC_HWPM_RESOURCE_UCF_SCB,
	NV_SOC_HWPM_RESOURCE_CPU,	/* CPU instance 0-31 */
	NV_SOC_HWPM_RESOURCE_CPU_EXT_0,	/* CPU (extended) instance 32-63 */
	NV_SOC_HWPM_RESOURCE_CPU_EXT_1,	/* CPU (extended) instance 64-95 */
	NV_SOC_HWPM_RESOURCE_CPU_EXT_2,	/* CPU (extended) instance 96-127 */
	NV_SOC_HWPM_RESOURCE_NVTHERM,
	NV_SOC_HWPM_RESOURCE_CSN,	/* CSN instance 0-31 */
	NV_SOC_HWPM_RESOURCE_CSN_EXT_0,	/* CSN (extended) instance 32-63 */
	NV_SOC_HWPM_RESOURCE_CSNH,
	NV_SOC_HWPM_NUM_RESOURCES
} nv_soc_hwpm_resource;

/**
 * @brief Initialize the NV_SOC_HWPM library.
 *
 * The call to this library will enumerate the supported devices. This function
 * should be the first to be called before other API. There is no reference
 * counting here, so caller needs to make sure it is only called once.
 * 
 * Internally it will enumerate the available HWPM driver device nodes, hence it
 * will need to open connection to the driver and makes some IOCTLs.
 * After it's done enumerating, the driver connection will be closed.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -ENOMEM: unable to allocate memory for internal data structures.
 *          -ENOENT: can't find HWPM device node.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_init(void);

/**
 * @brief Finalize the NV_SOC_HWPM library.
 *
 * It will cleanup internal data structures, including any allocated session, and
 * close all connections to driver.
 *
 * This function should be called at the end of execution.
 */
NV_SOC_HWPM_API_EXPORT void nv_soc_hwpm_exit(void);

/**
 * @brief General system attributes. See \ref nv_soc_hwpm_system_get_info.
 */
typedef enum {
	/* The major version of the library. */
	NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MAJOR,
	/* The minor version of the library. */
	NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MINOR,
} nv_soc_hwpm_system_attribute;

/**
 * @brief Get system-level information about the SOC HWPM library.
 *
 * This function retrieves system-level information based on the specified
 * attribute.
 *
 * @param[in] attribute The system attribute to query. See @ref
 *                      nv_soc_hwpm_system_attribute.
 *                      - NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MAJOR: info buffer
 *                        should fit uint16_t.
 *                      - NV_SOC_HWPM_SYSTEM_ATTRIBUTE_VERSION_MINOR: info buffer
 *                        should fit uint16_t.
 * @param[in] info_size The size of the \p info buffer.
 * @param[out] info Pointer to user/application allocated buffer to store the
 *                  attribute value.
 *
 * @return 0 on success, negative errno on failure:
 *         -EINVAL: if attribute is invalid or info is NULL.
 */
int nv_soc_hwpm_system_get_info(
	nv_soc_hwpm_system_attribute attribute, uint32_t info_size, void* info);

/**
 * @brief Struct containing opaque handle to an NV_SOC_HWPM device.
 * 
 * An NV_SOC_HWPM device corresponds to a HWPM device node in the system.
 * 
 * The user will need to use this handle to do the following:
 * - gather information such as the SOC information, the socket the node is
 *   associated with, and the IP, resources available in the socket.
 * - create a HWPM session in the socket.
 */
typedef struct { uint64_t handle; } nv_soc_hwpm_device;

/**
 * @brief Get list of NV_SOC_HWPM devices.
 *
 * User will need to call this function twice:
 *   - first by giving a non NULL count and set its value to zero. This will
 *     populate count with the number of available devices.
 *   - second by giving non NULL valid pointers on both parameters, with non
 *     zero count. This will populate devices array with the SOC HWPM device
 *     handles for subsequent usage.
 *
 * @param[inout] count Pointer to variable to store the number of available
 *                     devices.
 *
 * @param[out] devices NV_SOC_HWPM device array. The size of the array should
 *                     be equal or greater than the value in \p count.
 *
 * @return 0 on success, negative errno on failure:
 *         -EINVAL: if one of the arguments is invalid.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_get_devices(
	uint32_t *count, nv_soc_hwpm_device *devices);

/* Tegra SOC chip id */
typedef enum {
	TEGRA_SOC_HWPM_CHIP_ID_T241 = 0x500,
	TEGRA_SOC_HWPM_CHIP_ID_T410 = 0x410,
	TEGRA_SOC_HWPM_CHIP_ID_COUNT = 1
} tegra_soc_hwpm_chip_id;

/* Tegra SOC platform type */
typedef enum {
	TEGRA_SOC_HWPM_PLATFORM_SILICON = 0x0,
	TEGRA_SOC_HWPM_PLATFORM_PRESI_QT = 0x1,
	TEGRA_SOC_HWPM_PLATFORM_PRESI_VDK = 0x8,
	TEGRA_SOC_HWPM_PLATFORM_PRESI_VSP = 0x9
} tegra_soc_hwpm_platform;

/**
 * @brief Device attributes. See \ref nv_soc_hwpm_device_get_info.
 */
typedef enum {
	/* The Tegra SOC chip id. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_CHIP_ID,
	/* The Tegra SOC revision number. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_REVISION,
	/* The Tegra SOC platform type. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM,
	/* The Tegra SOC socket id.  */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_SOCKET,
	/* The number of available IPs. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT,
	/* Array containing the id of available IPs. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_LIST,
	/* The number of available resources. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT,
	/* Array containing the id of available resources. */
	NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST,
} nv_soc_hwpm_device_attribute;

/**
 * @brief Get device information.
 *
 * This function retrieves device-specific information based on the specified
 * attribute.
 *
 * @param[in] device Handle to NV_SOC_HWPM device.
 *
 * @param[in] attribute The device attribute to query. See @ref
 *                      nv_soc_hwpm_device_attribute.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_CHIP_ID: the info
 *                        buffer should fit @ref tegra_soc_hwpm_chip_id.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_REVISION: the info
 *                        buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_PLATFORM: the info
 *                        buffer should fit @ref tegra_soc_hwpm_platform.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_SOC_SOCKET: the info
 *                        buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT: the
 *                        info buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_LIST: the
 *                        info buffer should point to an array of
 *                        nv_soc_hwpm_ip. The size of the array should fit
 *                        the value of @ref
 *                        NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_COUNT:
 *                        the info buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_DEVICE_ATTRIBUTE_RESOURCE_AVAILABLE_LIST:
 *                        the info buffer should point to an array of
 *                        nv_soc_hwpm_resource. The size of the array should
 *                        fit the value of @ref
 *                        NV_SOC_HWPM_DEVICE_ATTRIBUTE_IP_AVAILABLE_COUNT.
 * @param[in] info_size The size of the \p info buffer.
 * @param[out] info Pointer to user/application allocated buffer to store the
 *                  attribute value.
 *
 * @return 0 on success, negative errno on failure:
 *         -EINVAL: if device is invalid, attribute is invalid, info_size does
 *         not fit the attribute value, info is NULL.
 */
int nv_soc_hwpm_device_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_device_attribute attribute,
	uint32_t info_size, void* info);

/**
 * @brief IP attributes. See \ref nv_soc_hwpm_ip_get_info.
 */
typedef enum {
	/**
	 * The IP availability status. 1 means IP is available, 0 means IP is
	 * unavailable.
	 */
	NV_SOC_HWPM_IP_ATTRIBUTE_IS_AVAILABLE,
	/* The IP instance/element max count. */
	NV_SOC_HWPM_IP_ATTRIBUTE_INST_MAX_COUNT,
	/**
	 * The flatten IP instance/element floorsweeping mask. The LSB is the
	 * lowest element/instance. The number of bits is equal to the
	 * @ref NV_SOC_HWPM_IP_ATTRIBUTE_INST_MAX_COUNT.
	 */
	NV_SOC_HWPM_IP_ATTRIBUTE_FS_MASK,
} nv_soc_hwpm_ip_attribute;

/**
 * @brief Get information about a specific IP.
 *
 * @param[in] device The SOC HWPM device handle.
 * @param[in] ip The IP to query information about.
 * @param[in] attribute The IP attribute to query. See @ref nv_soc_hwpm_ip_attribute.
 *                      - NV_SOC_HWPM_IP_ATTRIBUTE_IS_AVAILABLE: value should fit
 *                        uint32_t.
 *                      - NV_SOC_HWPM_IP_ATTRIBUTE_INST_MAX_COUNT: value should
 *                        fit uint32_t.
 *                      - NV_SOC_HWPM_IP_ATTRIBUTE_FS_MASK: value should fit
 *                        the number of bits equal to the value of @ref
 *                        NV_SOC_HWPM_IP_ATTRIBUTE_INST_MAX_COUNT.
 * @param[in] info_size The size of the \p info buffer.
 * @param[out] info Pointer to user/application allocated buffer to store the
 *                  attribute value.
 *
 * @return 0 on success, negative errno on failure:
 *         -EINVAL: if device is invalid, ip is invalid, attribute is invalid,
 *         info_size does not fit the attribute value, or info is NULL.
 */
int nv_soc_hwpm_ip_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_ip ip,
	nv_soc_hwpm_ip_attribute attribute, uint32_t info_size, void* info);

/**
 * @brief Resource attributes. See \ref nv_soc_hwpm_resource_get_info.
 */
typedef enum {
    /**
     * The resource availability status. The output type of this attribute is
     * uint32_t. 1 means resource is available, 0 means unavailable.
     */
    NV_SOC_HWPM_RESOURCE_ATTRIBUTE_IS_AVAILABLE,
} nv_soc_hwpm_resource_attribute;

/**
 * @brief Get information about a specific resource.
 *
 * @param[in] device The SOC HWPM device handle.
 * @param[in] resource The resource to query information about.
 * @param[in] attribute The resource attribute to query.
 *                      See @ref nv_soc_hwpm_resource_attribute.
 *                      - NV_SOC_HWPM_RESOURCE_ATTRIBUTE_IS_AVAILABLE: value should
 *                        fit uint32_t.
 * @param[in] info_size The size of the \p info buffer.
 * @param[out] info Pointer to user/application allocated buffer to store the
 *                  attribute value.
 *
 * @return 0 on success, negative errno on failure:
 *         -EINVAL: if device is invalid, resource is invalid, attribute is
 *         invalid, info_size does not fit the attribute value, or info is NULL.
 */
int nv_soc_hwpm_resource_get_info(
	nv_soc_hwpm_device device, nv_soc_hwpm_resource resource,
	nv_soc_hwpm_resource_attribute attribute, uint32_t info_size, void* info);

/**
 * @brief Struct containing opaque handle to an NV_SOC_HWPM session.
 * 
 * The user will need this handle to do the following:
 * - reserve the resources that will be used in the experiment
 * - manage the PMA buffer
 * - perform regops
 * - get PMA channel state
 * - working with cross trigger
 * - credit programming
 */
typedef struct { uint64_t handle; } nv_soc_hwpm_session;

/**
 * @brief Create a session on a device.
 *
 * Internally this will open connection to the corresponding driver device node
 * and keep it open until the session is freed.
 *
 * @param[in] device Handle to NV_SOC_HWPM device.
 *
 * @param[out] session Pointer to NV_SOC_HWPM session handle.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: one of the arguments is invalid.
 *          -ENOMEM: unable to create new session since the maximum session count
 *                   is reached.
 *          Other non zero status corresponds to failure when opening the driver
 *          device node.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_alloc(
        nv_soc_hwpm_device device, nv_soc_hwpm_session *session);

/**
 * @brief Free a session on a device.
 *
 * This will reset the state of a session, cleanup any allocated buffer, and
 * close connection to the driver device node.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_free(
	nv_soc_hwpm_session session);

/**
 * @brief Session attributes. See \ref nv_soc_hwpm_session_get_info.
 */
typedef enum {
	/* The device handle associated with the session. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_DEVICE,

	/**
	 * The session's start status. 1 means the session is started. 0 means
	 * session is not started.
	 */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_SESSION_STARTED,

	/**
	 * The session's PMA buffer allocation status. 1 means the buffers
	 * are allocated. 0 means buffers are not allocated.
	 */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_BUFFER_ALLOCATED,

	/* Size of the session's record buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE,
	/* CPU accessible address of the session's record buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA,
	/* PMA accessible addres of the session's record buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA,
	/* Opaque handle of the session's record buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_HANDLE,

	/* Size of the session's mem bytes buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_SIZE,
	/* CPU accessible address of the session's mem bytes buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA,
	/* PMA accessible addres of the session's mem bytes buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_PMA_VA,
	/* Opaque handle of the session's mem bytes buffer. */
	NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_HANDLE,
} nv_soc_hwpm_session_attribute;

/**
 * @brief Get info of an NV_SOC_HWPM session.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] attribute The session attribute to query.
 *                      See @ref nv_soc_hwpm_session_attribute.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_DEVICE: the info buffer
 *                        should fit @ref nv_soc_hwpm_device.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_SESSION_STARTED: the info
 *                        buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_BUFFER_ALLOCATED: the
 *                        info buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_SIZE:
 *                        the info buffer should fit size_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_CPU_VA:
 *                        the info buffer should fit (void*).
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_PMA_VA:
 *                        the info buffer should fit uint64_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_RECORD_BUFFER_HANDLE:
 *                        the info buffer should fit uint32_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_SIZE:
 *                        the info buffer should fit size_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_CPU_VA:
 *                        the info buffer should fit (void*).
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_PMA_VA:
 *                        the info buffer should fit uint64_t.
 *                      - NV_SOC_HWPM_SESSION_ATTRIBUTE_PMA_MEM_BYTES_BUFFER_HANDLE:
 *                        the info buffer should fit uint32_t.
 * @param[in] info_size The size of the \p info buffer.
 * @param[out] info Pointer to user/application allocated buffer to store the
 *                  attribute value.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, attribute is invalid, info_size does
 *                   not fit the attribute value, or info is NULL.
 */
int nv_soc_hwpm_session_get_info(
	nv_soc_hwpm_session session, nv_soc_hwpm_session_attribute attribute,
	uint32_t info_size, void* info);

/**
 * @brief Reserve the resources that will be monitored by a session.
 *
 * This will perform the IOCTLs to reserve the resources to the driver.
 * Note that since the driver does not expose interface to release a resource,
 * the reserved resources can not be changed until the session is freed.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] res_count The number of element in \p res_ids
 *
 * @param[in] res_ids Array containing the resource id.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, res_count is invalid, res_ids is
 *                   NULL, res_ids contain unavailable resource id, or the session
 *                   has already started.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_reserve_resources(
        nv_soc_hwpm_session session, uint32_t res_count,
        const nv_soc_hwpm_resource *res_ids);

/**
 * @brief Set all supported resources to be monitored to a session.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, or the session has already started.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_reserve_all_resources(
        nv_soc_hwpm_session session);

/* Tegra SOC HWPM PMA buffer cache coherency type */
typedef enum
{
	TEGRA_SOC_HWPM_COHERENCY_TYPE_UNCACHED,
	TEGRA_SOC_HWPM_COHERENCY_TYPE_CACHED,
	TEGRA_SOC_HWPM_COHERENCY_TYPE_COUNT
} tegra_soc_hwpm_coherency_type;

/**
 * @brief Struct containing NV_SOC_HWPM PMA buffer allocation parameters.
 */
typedef struct {
	size_t size;
	void* cpu_va;
	tegra_soc_hwpm_coherency_type coherency_type;
} nv_soc_hwpm_pma_buffer_params;

/**
 * @brief Allocate memory for PMA channel.
 *
 * After a successful allocation, the user can call nv_soc_hwpm_session_get_info
 * to get the allocation result, i.e nv_soc_hwpm_session_info::record_buffer
 * and nv_soc_hwpm_session_info::mem_bytes_buffer.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] record_buffer_params Pointer to PMA record buffer allocation
 *                                 param structure.
 *
 * @param[in] mem_bytes_buffer_params Pointer to PMA mem_bytes buffer allocation
 *                                 param structure.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, a prior allocation has been made, or
 *                   the session has already started.
 *          -EIO: error on IOCTLs to the driver.
 *          -ENOMEM: failure on allocating memory.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_alloc_pma(
        nv_soc_hwpm_session session,
        const nv_soc_hwpm_pma_buffer_params *record_buffer_params);

/**
 * @brief Struct containing parameter to update and query PMA channel state.
 */
typedef struct {
	uint64_t in_mem_bump;
	uint8_t in_stream_mem_bytes;
	uint8_t in_read_mem_head;
	uint8_t in_check_overflow;
	uint64_t out_mem_head;
	uint8_t out_overflowed;
} nv_soc_hwpm_pma_channel_state_params;

/**
 * @brief Set and get PMA channel state.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] param Structure containing the input/output arguments.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, param is NULL, or the session has
 *                   not started yet.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_set_get_pma_state(
	nv_soc_hwpm_session session,
	nv_soc_hwpm_pma_channel_state_params* param);

/* Tegra SOC HWPM enums for querying various info */
typedef enum
{
	/* Read credits information */
	TEGRA_SOC_HWPM_GET_TYPE_HS_CREDITS,
	/* Read total HS credits*/
	TEGRA_SOC_HWPM_GET_TYPE_TOTAL_HS_CREDITS,
	TEGRA_SOC_HWPM_GET_TYPE_COUNT,
} tegra_soc_hwpm_get_type;

/**
 * @brief Credit programming interface to get HS credits.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] type credit or total HS credits.
 *
 * @param[in] num_hs_credits Pointer to output variable containing HS credits.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, type is invalid, num_hs_credits
 *                   is NULL, or the session has not started yet.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_get_hs_credits(
	nv_soc_hwpm_session session,
	tegra_soc_hwpm_get_type type,
	uint32_t* num_hs_credits);

/**
 * @brief Struct containing parameter to configure HS credit.
*/
typedef struct {
	uint8_t cblock_idx;
	uint32_t num_credits_per_chiplet;
} nv_soc_hwpm_config_hs_credit_params;

/**
 * @brief Credit programming interface to configure HS credits.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] param_count Number of element in \p params.
 *
 * @param[in] params Structure containing the array of credit param.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, param_count is invalid, params is
 *                   NULL, or the session has not started yet.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_config_hs_credits(
	nv_soc_hwpm_session session,
	uint32_t param_count,
	const nv_soc_hwpm_config_hs_credit_params* params);

/**
 * @brief Starting session by binding the resources.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, or the session has already started.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_start(
	nv_soc_hwpm_session session);

/* Tegra SOC HWPM enums for regops cmd */
typedef enum {
	NV_SOC_HWPM_REG_OPS_CMD_INVALID,
	NV_SOC_HWPM_REG_OPS_CMD_READ32,
	NV_SOC_HWPM_REG_OPS_CMD_READ64,
	NV_SOC_HWPM_REG_OPS_CMD_WRITE32,
	NV_SOC_HWPM_REG_OPS_CMD_WRITE64,
	NV_SOC_HWPM_REG_OPS_CMD_COUNT
} nv_soc_hwpm_reg_ops_cmd;

/* Tegra SOC HWPM enums for regops status */
typedef enum {
	NV_SOC_HWPM_REG_OPS_STATUS_SUCCESS,
	NV_SOC_HWPM_REG_OPS_STATUS_INVALID_CMD,
	NV_SOC_HWPM_REG_OPS_STATUS_INVALID_OFFSET,
	NV_SOC_HWPM_REG_OPS_STATUS_INSUFFICIENT_PERMISSIONS,
	NV_SOC_HWPM_REG_OPS_STATUS_COUNT
} nv_soc_hwpm_reg_ops_status;

/* Tegra SOC HWPM enums for regops validation */
typedef enum {
        NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_INVALID,
        NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_FAIL_ON_FIRST_ERROR,
        NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_CONTINUE_ON_ERROR,
        NV_SOC_HWPM_REG_OPS_VALIDATION_MODE_COUNT
} nv_soc_hwpm_reg_ops_validation_mode;

/**
 * @brief Struct containing parameter to perform regops.
 */
typedef struct {
	/* register offset, as per kernel team this can be 40 bits */
        uint64_t in_offset;

	union {
            uint32_t in_out_val32;
            uint64_t in_out_val64;
        };

        union {
            uint32_t in_mask32;
            uint64_t in_mask64;
        };

        nv_soc_hwpm_reg_ops_cmd in_cmd;
        nv_soc_hwpm_reg_ops_status out_status;
} nv_soc_hwpm_reg_ops_params;

/**
 * @brief Function to read/write HWPM registers.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] param_count Number of element in \p params.
 *
 * @param[in] params Structure containing the array of regops param.
 *
 * @param[in] mode Validation mode.
 *
 * @param[out] all_reg_ops_passed Pointer to output variable, 1 means all the
 *                                operations are successful, 0 means one or
 *                                more are failed.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, param_count is invalid, params is
 *                   NULL, or the session has not started yet.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_regops(
	nv_soc_hwpm_session session,
	const size_t param_count,
	nv_soc_hwpm_reg_ops_params* params,
	nv_soc_hwpm_reg_ops_validation_mode mode,
	int* all_reg_ops_passed);

/* Tegra SOC HWPM trigger session type */
typedef enum {
	NV_SOC_HWPM_TRIGGER_SESSION_TYPE_INVALID,
	NV_SOC_HWPM_TRIGGER_SESSION_TYPE_PROFILER, /* maps to Start-Stop triggers */
	NV_SOC_HWPM_TRIGGER_SESSION_TYPE_SAMPLER, /* maps to Periodic triggers */
	NV_SOC_HWPM_TRIGGER_SESSION_TYPE_COUNT
} nv_soc_hwpm_trigger_session_type;

/**
 * @brief Trigger mask setup interface.
 *
 * Used for programming cross-trigger config in the SECURE trigger mask
 * registers.
 *
 * @param[in] session Handle to NV_SOC_HWPM session.
 *
 * @param[in] enable_cross_trigger 1 to enable and 0 to disable cross trigger.
 *
 * @param[in] session_type Trigger session type.
 *
 * @returns This function returns 0 on success, negative errno on failure:
 *          -EINVAL: if session is invalid, or the session has not started yet.
 *          -EIO: error on IOCTLs to the driver.
 */
NV_SOC_HWPM_API_EXPORT int nv_soc_hwpm_session_setup_trigger(
	nv_soc_hwpm_session session,
	int enable_cross_trigger,
	nv_soc_hwpm_trigger_session_type session_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__NV_SOC_HWPM_H__*/