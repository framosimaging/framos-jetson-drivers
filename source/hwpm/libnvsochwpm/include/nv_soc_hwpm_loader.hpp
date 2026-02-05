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

#ifndef __NV_SOC_HWPM_LOADER_H__
#define __NV_SOC_HWPM_LOADER_H__

#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>

#include "nv_soc_hwpm.h"

typedef struct {
        void* handle;
        decltype(nv_soc_hwpm_init)* nv_soc_hwpm_init_fn;
        decltype(nv_soc_hwpm_exit)* nv_soc_hwpm_exit_fn;
        decltype(nv_soc_hwpm_system_get_info)* nv_soc_hwpm_system_get_info_fn;
        decltype(nv_soc_hwpm_get_devices)* nv_soc_hwpm_get_devices_fn;
        decltype(nv_soc_hwpm_device_get_info)* nv_soc_hwpm_device_get_info_fn;
        decltype(nv_soc_hwpm_ip_get_info)* nv_soc_hwpm_ip_get_info_fn;
        decltype(nv_soc_hwpm_resource_get_info)* nv_soc_hwpm_resource_get_info_fn;
        decltype(nv_soc_hwpm_session_alloc)* nv_soc_hwpm_session_alloc_fn;
        decltype(nv_soc_hwpm_session_free)* nv_soc_hwpm_session_free_fn;
        decltype(nv_soc_hwpm_session_get_info)* nv_soc_hwpm_session_get_info_fn;
        decltype(nv_soc_hwpm_session_reserve_resources)* nv_soc_hwpm_session_reserve_resources_fn;
        decltype(nv_soc_hwpm_session_reserve_all_resources)* nv_soc_hwpm_session_reserve_all_resources_fn;
        decltype(nv_soc_hwpm_session_alloc_pma)* nv_soc_hwpm_session_alloc_pma_fn;
        decltype(nv_soc_hwpm_session_set_get_pma_state)* nv_soc_hwpm_session_set_get_pma_state_fn;
        decltype(nv_soc_hwpm_session_get_hs_credits)* nv_soc_hwpm_session_get_hs_credits_fn;
        decltype(nv_soc_hwpm_session_config_hs_credits)* nv_soc_hwpm_session_config_hs_credits_fn;
        decltype(nv_soc_hwpm_session_start)* nv_soc_hwpm_session_start_fn;
        decltype(nv_soc_hwpm_session_regops)* nv_soc_hwpm_session_regops_fn;
        decltype(nv_soc_hwpm_session_setup_trigger)* nv_soc_hwpm_session_setup_trigger_fn;
} nv_soc_hwpm_api_table;

#define XSTR(a) STR(a)
#define STR(a) #a
#define GET_HWPM_SYM(table, name) GET_HWPM_SYM_FN(table, name, name ## _fn)
#define GET_HWPM_SYM_FN(table, name, name_fn) \
	table->name_fn = (decltype(name)* )dlsym(table->handle, XSTR(name)); \
	if (table->name_fn == NULL)  { \
		fprintf (stderr, "%s\n", dlerror()); \
		return -EINVAL; \
	}

static inline int load_nv_soc_hwpm(const char* path, nv_soc_hwpm_api_table* table) {
	if (table == NULL)
		return -EINVAL;

	if (table->handle != NULL) {
		fprintf (stderr, "NvSocHwpm already loaded\n");
		return -EINVAL;
	}

        table->handle = dlopen (path, RTLD_LAZY);
        if (!table->handle) {
                fprintf (stderr, "Failed to load NvSocHwpm: %s\n", dlerror());
                return -EINVAL;
        }

	GET_HWPM_SYM(table, nv_soc_hwpm_init);
	GET_HWPM_SYM(table, nv_soc_hwpm_exit);
	GET_HWPM_SYM(table, nv_soc_hwpm_system_get_info);
	GET_HWPM_SYM(table, nv_soc_hwpm_get_devices);
	GET_HWPM_SYM(table, nv_soc_hwpm_device_get_info);
	GET_HWPM_SYM(table, nv_soc_hwpm_ip_get_info);
	GET_HWPM_SYM(table, nv_soc_hwpm_resource_get_info);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_alloc);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_free);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_get_info);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_reserve_resources);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_reserve_all_resources);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_alloc_pma);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_set_get_pma_state);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_get_hs_credits);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_config_hs_credits);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_start);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_regops);
	GET_HWPM_SYM(table, nv_soc_hwpm_session_setup_trigger);

        return 0;
}

static inline int unload_nv_soc_hwpm(nv_soc_hwpm_api_table* table) {
	if (table == NULL)
		return -EINVAL;

	if (table->handle == NULL)
		return 0;

	dlclose(table->handle);
	table->handle = NULL;
	return 0;
}

#endif /* __NV_SOC_HWPM_LOADER_H__ */
