/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * MAX96793 serializer driver
 */

#ifndef __fr_max96793_H__
#define __fr_max96793_H__

#include <media/gmsl-link.h>
#include <media/tegra_v4l2_camera.h>
#include <media/tegracam_core.h>


int max96793_setup_control(struct device *dev);

int max96793_reset_control(struct device *dev);

int max96793_sdev_pair(struct device *dev, struct gmsl_link_ctx *g_ctx);

int max96793_sdev_unpair(struct device *dev, struct device *s_dev);

int max96793_setup_streaming(struct device *dev,
				struct camera_common_data *s_data);

int max96793_gmsl_setup(struct device *dev);

int max96793_gpio10_xtrig1_setup(struct device *dev, char *image_sensor_type);

int max96793_xvs_setup(struct device *dev, bool direction);

enum {
	max96793_OUT,
	max96793_IN,
};

#endif
