// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022-2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/delay.h>

#include <tegra_hwpm.h>
#include <tegra_hwpm_timers.h>

int tegra_hwpm_timeout_init_impl(struct tegra_soc_hwpm *hwpm,
	struct tegra_hwpm_timeout *timeout, u32 retries)
{
	timeout->max_attempts = retries;
	timeout->attempted = 0;

	return 0;
}

int tegra_hwpm_timeout_expired_impl(struct tegra_soc_hwpm *hwpm,
	struct tegra_hwpm_timeout *timeout)
{
	if (timeout->attempted >= timeout->max_attempts) {
		return -ETIMEDOUT;
	}

	timeout->attempted++;
	return 0;
}

void tegra_hwpm_msleep_impl(unsigned int msecs)
{
	msleep(msecs);
}
