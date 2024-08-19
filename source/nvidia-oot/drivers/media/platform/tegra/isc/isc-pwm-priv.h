/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2016-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 */

#ifndef __ISC_PWM_PRIV_H__
#define __ISC_PWM_PRIV_H__

struct isc_pwm_info {
	struct pwm_chip chip;
	struct pwm_device *pwm;
	atomic_t in_use;
	struct mutex mutex;
	bool force_on;
};

#endif  /* __ISC_PWM_PRIV_H__ */
