/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * MAX96792 deserializer driver
 */

#ifndef __fr_MAX96792_H__
#define __fr_MAX96792_H__

#include <media/gmsl-link.h>


int max96792_setup_link(struct device *dev, struct device *s_dev);

int max96792_setup_control(struct device *dev, struct device *s_dev);

int max96792_reset_control(struct device *dev, struct device *s_dev);

int max96792_sdev_register(struct device *dev, struct gmsl_link_ctx *g_ctx);

int max96792_sdev_unregister(struct device *dev, struct device *s_dev);

int max96792_setup_streaming(struct device *dev, struct device *s_dev,
					struct camera_common_data *s_data);

int max96792_start_streaming(struct device *dev, struct device *s_dev);

int max96792_stop_streaming(struct device *dev, struct device *s_dev);

int max96792_power_on(struct device *dev, struct gmsl_link_ctx *g_ctx);

void max96792_power_off(struct device *dev, struct gmsl_link_ctx *g_ctx);

int max96792_gmsl_setup(struct device *dev);

int max96792_xvs_setup(struct device *dev, bool direction);

enum {
	max96792_OUT,
	max96792_IN,
};

#endif
