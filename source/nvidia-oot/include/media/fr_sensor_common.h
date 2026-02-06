/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 */

#ifndef __fr_sensor_common__
#define __fr_sensor_common__

/* FSA hardware timeouts in useconcds */
#define FCON_RST_TIMEOUT	25000
#define FCON_SEQ_FLAG_TIMEOUT	40000
#define FCON_REG_DISC_TIMEOUT	200000


/* Enum describing available operation modes */
typedef enum {
	MASTER_MODE,
	SLAVE_MODE,
} operation_mode;

/* Enum describing available shutter modes */
typedef enum {
	NORMAL_EXPO,
	SEQ_TRIGGER,
	FAST_TRIGGER,
} shutter_mode;

/* Enum describing available sync modes */
typedef enum {
	NO_SYNC,
	INTERNAL_SYNC,
	EXTERNAL_SYNC,
} sync_mode;

/* Enum describing available communication modes */
typedef enum {
	UNICAST,
	BROADCAST,
} i2c_broadcast_ctrl;

/* Enum describing available v4l2 control value */
typedef enum {
	MIN,
	MAX,
	DEFAULT,
	CURRENT,
	NEW,
} v4l2_ctrl_value;

void fr_calc_lshift(u64 *val, u8 *lshift, s64 max);

s64 fr_get_v4l2_ctrl_value(struct tegracam_device *tc_dev,
				int ctrl_id, v4l2_ctrl_value ctrl_value);

void fr_update_ctrl_range(struct tegracam_device *tc_dev,
					int ctrl_id, u64 min, u64 max);

void fr_gpio_set(struct camera_common_data *s_data,
						unsigned int gpio, int val);

struct v4l2_ctrl *fr_find_v4l2_ctrl(struct tegracam_device *tc_dev, int ctrl_id);

void fr_get_gpio_ctrl(struct camera_common_pdata *board_priv_pdata);

int cam_gpio_register(struct device *dev, unsigned int pin_num);

void cam_gpio_deregister(struct device *dev, unsigned int pin_num);

int cam_gpio_ctrl(struct device *dev,
			unsigned int pin_num, int ref_inc, bool active_high);

int common_get_broadcast_client(struct tegracam_device *tc_dev,
			struct v4l2_ctrl *ctrl,
			const struct regmap_config *sensor_regmap_config);

#endif
