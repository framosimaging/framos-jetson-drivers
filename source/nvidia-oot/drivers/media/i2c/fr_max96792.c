// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2024 Framos. All rights reserved.
 *
 * fr_max96792.c - framos max96792.c GMSL Deserializer driver
 */

//#define DEBUG 1

#include <nvidia/conftest.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <media/camera_common.h>
#include <linux/module.h>
#include <media/fr_max96792.h>
#include <media/fr_sensor_common.h>

/* register specifics */
#define MAX96792_DST_CSI_MODE_ADDR	0x330
#define MAX96792_LANE_MAP1_ADDR		0x333
#define MAX96792_LANE_MAP2_ADDR		0x334

#define MAX96792_LANE_CTRL0_ADDR	0x40A
#define MAX96792_LANE_CTRL1_ADDR	0x44A
#define MAX96792_LANE_CTRL2_ADDR	0x48A
#define MAX96792_LANE_CTRL3_ADDR	0x4CA

#define MAX96792_PIPE_X_SRC_0_MAP_ADDR	0x40D
#define MAX96792_PIPE_X_DST_0_MAP_ADDR	0x40E
#define MAX96792_PIPE_X_SRC_1_MAP_ADDR	0x40F
#define MAX96792_PIPE_X_DST_1_MAP_ADDR	0x410
#define MAX96792_PIPE_X_SRC_2_MAP_ADDR	0x411
#define MAX96792_PIPE_X_DST_2_MAP_ADDR	0x412
#define MAX96792_PIPE_X_SRC_3_MAP_ADDR	0x413
#define MAX96792_PIPE_X_DST_3_MAP_ADDR	0x414

#define MAX96792_CTRL0_ADDR		0x10

#define MAX96792_CSI_MODE_4X2		0x1
#define MAX96792_CSI_MODE_2X4		0x4
#define MAX96792_LANE_MAP1_4X2		0x44
#define MAX96792_LANE_MAP2_4X2		0x44
#define MAX96792_LANE_MAP1_2X4		0x4E
#define MAX96792_LANE_MAP2_2X4		0xE4

#define MAX96792_LANE_CTRL_MAP(num_lanes) \
	(((num_lanes) << 6) & 0xF0)

#define MAX96792_ALLPHYS_NOSTDBY	0xF0
#define MAX96792_ST_ID_SEL_INVALID	0xF

#define MAX96792_PHY1_CLK		0x2C

#define MAX96792_RESET_ALL		0x80

#define MAX96792_MAX_SOURCES		2

#define MAX96792_MAX_PIPES		4

#define MAX96792_PIPE_X			0
#define MAX96792_PIPE_Y			1
#define MAX96792_PIPE_Z			2
#define MAX96792_PIPE_U			3
#define MAX96792_PIPE_INVALID		0xF

#define MAX96792_CSI_CTRL_0		0
#define MAX96792_CSI_CTRL_1		1
#define MAX96792_CSI_CTRL_2		2
#define MAX96792_CSI_CTRL_3		3

#define MAX96792_INVAL_ST_ID		0xFF

#define MAX96792_RESET_ST_ID		0x00

#define GPIO_OUT_DIS			0x01
#define GPIO_TX_EN			(0x01 << 1)
#define GPIO_RX_EN			(0x01 << 2)

#define VIDEO_PIPE_EN			0x160
#define VIDEO_PIPE_SEL			0x161


struct max96792_source_ctx {
	struct gmsl_link_ctx *g_ctx;
	bool st_enabled;
};

struct pipe_ctx {
	u32 id;
	u32 dt_type;
	u32 dst_csi_ctrl;
	u32 st_count;
	u32 st_id_sel;
};

struct max96792 {
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	u32 num_src;
	u32 max_src;
	u32 num_src_found;
	u32 src_link;
	bool splitter_enabled;
	struct max96792_source_ctx sources[MAX96792_MAX_SOURCES];
	struct mutex lock;
	u32 sdev_ref;
	bool lane_setup;
	bool link_setup;
	struct pipe_ctx pipe[MAX96792_MAX_PIPES];
	u8 csi_mode;
	u8 lane_mp1;
	u8 lane_mp2;
	int reset_gpio;
	int pw_ref;
	struct regulator *vdd_cam_1v2;
};

static int max96792_write_reg(struct device *dev,
	u16 addr, u8 val)
{
	struct max96792 *priv;
	int err;

	priv = dev_get_drvdata(dev);

	err = regmap_write(priv->regmap, addr, val);
	if (err)
		dev_err(dev,
		"%s:i2c write failed, 0x%x = %x\n",
		__func__, addr, val);

	usleep_range(100, 110);

	return err;
}

static int max96792_get_sdev_idx(struct device *dev,
					struct device *s_dev, int *idx)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int i;
	int err = 0;

	mutex_lock(&priv->lock);
	for (i = 0; i < priv->max_src; i++) {
		if (priv->sources[i].g_ctx->s_dev == s_dev)
			break;
	}
	if (i == priv->max_src) {
		dev_err(dev, "no sdev found\n");
		err = -EINVAL;
		goto ret;
	}

	if (idx)
		*idx = i;

ret:
	mutex_unlock(&priv->lock);
	return err;
}

static void max96792_pipes_reset(struct max96792 *priv)
{
	struct pipe_ctx pipe_defaults[] = {
		{MAX96792_PIPE_X, GMSL_CSI_DT_RAW_12,
			MAX96792_CSI_CTRL_1, 0, MAX96792_INVAL_ST_ID},
		{MAX96792_PIPE_Y, GMSL_CSI_DT_RAW_12,
			MAX96792_CSI_CTRL_1, 0, MAX96792_INVAL_ST_ID},
		{MAX96792_PIPE_Z, GMSL_CSI_DT_EMBED,
			MAX96792_CSI_CTRL_1, 0, MAX96792_INVAL_ST_ID},
		{MAX96792_PIPE_U, GMSL_CSI_DT_EMBED,
			MAX96792_CSI_CTRL_1, 0, MAX96792_INVAL_ST_ID}
	};

	memcpy(priv->pipe, pipe_defaults, sizeof(pipe_defaults));
}

static void max96792_reset_ctx(struct max96792 *priv)
{
	int i;

	priv->link_setup = false;
	priv->lane_setup = false;
	priv->num_src_found = 0;
	priv->src_link = 0;
	priv->splitter_enabled = false;
	max96792_pipes_reset(priv);
	for (i = 0; i < priv->num_src; i++)
		priv->sources[i].st_enabled = false;
}

int max96792_power_on(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;

	mutex_lock(&priv->lock);
	if (priv->pw_ref == 0) {
		usleep_range(1, 2);
		if (priv->reset_gpio)
			cam_gpio_ctrl(dev, priv->reset_gpio, 1, 1);
		usleep_range(30, 50);

		if (priv->vdd_cam_1v2) {
			err = regulator_enable(priv->vdd_cam_1v2);
			if (unlikely(err))
				goto ret;
		}

		msleep(2000);
	}

	priv->pw_ref++;

ret:
	mutex_unlock(&priv->lock);
	usleep_range(1000, 1100);
	return err;
}
EXPORT_SYMBOL(max96792_power_on);

void max96792_power_off(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct max96792 *priv = dev_get_drvdata(dev);

	mutex_lock(&priv->lock);
	priv->pw_ref--;

	if (priv->pw_ref == 0) {
		usleep_range(1, 2);
		if (priv->reset_gpio)
			cam_gpio_ctrl(dev, priv->reset_gpio, 0, 1);

		if (priv->vdd_cam_1v2)
			regulator_disable(priv->vdd_cam_1v2);
	}

	mutex_unlock(&priv->lock);
}
EXPORT_SYMBOL(max96792_power_off);

static int max96792_write_link(struct device *dev, u32 link)
{

	if (link == GMSL_SERDES_CSI_LINK_A) {
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x21);
		dev_dbg(dev, "%s: reset ONE SHOT!!!!\n", __func__);
		dev_dbg(dev, "%s: GMSL_SERDES_CSI_LINK_A\n", __func__);
	} else if (link == GMSL_SERDES_CSI_LINK_B) {
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x02);
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x22);
		dev_dbg(dev, "%s: GMSL_SERDES_CSI_LINK_B\n", __func__);
	} else {
		dev_err(dev, "%s: invalid gmsl link\n", __func__);
		return -EINVAL;
	}

	msleep(100);

	return 0;
}

int max96792_setup_link(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;
	int i;

	err = max96792_get_sdev_idx(dev, s_dev, &i);
	if (err)
		return err;

	mutex_lock(&priv->lock);

	if (!priv->splitter_enabled) {
		err = max96792_write_link(dev,
				priv->sources[i].g_ctx->serdes_csi_link);
		if (err)
			goto ret;

		priv->link_setup = true;
	}

ret:
	mutex_unlock(&priv->lock);

	return err;
}
EXPORT_SYMBOL(max96792_setup_link);

int max96792_gmsl_setup(struct device *dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;

	dev_dbg(dev, "%s++\n", __func__);
	mutex_lock(&priv->lock);

	max96792_write_reg(dev, 0x01, 0x03);
	max96792_write_reg(dev, 0x04, 0xC3);
	max96792_write_reg(dev, 0x06, 0x1F);
	max96792_write_reg(dev, 0x28, 0x62);
	max96792_write_reg(dev, 0x2001, 0x01);
	max96792_write_reg(dev, 0x2101, 0x01);

	max96792_write_reg(dev, 0x443, 0x81);
	max96792_write_reg(dev, 0x444, 0x81);

#ifdef ENABLE_ERR_REPORTING
	max96792_write_reg(dev, 0x1A, 0x00);
	max96792_write_reg(dev, 0x1C, 0x00);
	max96792_write_reg(dev, 0x6E, 0x70);
	max96792_write_reg(dev, 0x76, 0x70);
	max96792_write_reg(dev, 0x7E, 0x70);
	max96792_write_reg(dev, 0x86, 0x70);
	max96792_write_reg(dev, 0x8E, 0x70);

	max96792_write_reg(dev, 0x340, 0x00);
	max96792_write_reg(dev, 0x578, 0x15);

	max96792_write_reg(dev, 0x3010, 0x00);
	max96792_write_reg(dev, 0x5010, 0x00);

	max96792_write_reg(dev, 0x5076, 0x00);
	max96792_write_reg(dev, 0x5086, 0x00);
	max96792_write_reg(dev, 0x508E, 0x00);
	max96792_write_reg(dev, 0x507E, 0x00);
#endif
	if (err)
		dev_err(dev, "gmsl config failed!\n");

	mutex_unlock(&priv->lock);

	return err;
}
EXPORT_SYMBOL(max96792_gmsl_setup);

#define PIPE_Y
//#define PIPE_Z

int max96792_setup_control(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;
	int i;

	err = max96792_get_sdev_idx(dev, s_dev, &i);
	if (err)
		return err;

	mutex_lock(&priv->lock);

	if (!priv->link_setup) {
		dev_err(dev, "%s: invalid state\n", __func__);
		err = -EINVAL;
		goto error;
	}

	if (priv->sources[i].g_ctx->serdev_found) {
		priv->num_src_found++;
		priv->src_link = priv->sources[i].g_ctx->serdes_csi_link;
	}

	if ((priv->max_src > 1U) &&
		(priv->num_src_found > 1U) &&
		(priv->splitter_enabled == false)) {
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x03);
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x23);

		priv->splitter_enabled = true;
		dev_dbg(dev, "%s: priv->splitter_enabled = %d\n", __func__,
						priv->splitter_enabled);
		msleep(100);
	}

	priv->sdev_ref++;

	if ((priv->sdev_ref == priv->max_src) && (priv->splitter_enabled == true) &&
		(priv->num_src_found > 0U) && (priv->num_src_found < priv->max_src)) {
		err = max96792_write_link(dev, priv->src_link);
		if (err)
			goto error;

		priv->splitter_enabled = false;
	}

#ifdef PIPE_Y
	max96792_write_reg(dev, 0x112, 0x30);
	max96792_write_reg(dev, VIDEO_PIPE_SEL, 0x01);
#endif
#ifdef PIPE_Z
	max96792_write_reg(dev, 0x4B3, 0x10);
	max96792_write_reg(dev, 0x124, 0x03);

#endif

	max96792_write_reg(dev, 0x31D, 0x38);
	max96792_write_reg(dev, 0x320, 0x38);

	/* robust operation
	 * max96792_write_reg(dev, 0x143F, 0x3D);
	 * max96792_write_reg(dev, 0x153F, 0x3D);
	 * max96792_write_reg(dev, 0x143E, 0xFD);
	 * max96792_write_reg(dev, 0x153E, 0xFD);
	 * max96792_write_reg(dev, 0x14AD, 0x68);
	 * max96792_write_reg(dev, 0x15AD, 0x68);
	 * max96792_write_reg(dev, 0x14AC, 0xA8);
	 * max96792_write_reg(dev, 0x15AC, 0xA8);
	 * max96792_write_reg(dev, 0x1418, 0x07);
	 * max96792_write_reg(dev, 0x1518, 0x07);
	 * max96792_write_reg(dev, 0x141F, 0xC2);
	 * max96792_write_reg(dev, 0x151F, 0xC2);
	 * max96792_write_reg(dev, 0x148C, 0x10);
	 * max96792_write_reg(dev, 0x158C, 0x10);
	 * max96792_write_reg(dev, 0x1498, 0xC0);
	 * max96792_write_reg(dev, 0x1598, 0xC0);
	 * max96792_write_reg(dev, 0x1446, 0x01);
	 * max96792_write_reg(dev, 0x1546, 0x01);
	 * max96792_write_reg(dev, 0x1445, 0x81);
	 * max96792_write_reg(dev, 0x1545, 0x81);
	 * max96792_write_reg(dev, 0x140B, 0x44);
	 * max96792_write_reg(dev, 0x150B, 0x44);
	 * max96792_write_reg(dev, 0x140A, 0x08);
	 * max96792_write_reg(dev, 0x150A, 0x08);
	 * max96792_write_reg(dev, 0x1431, 0x18);
	 * max96792_write_reg(dev, 0x1531, 0x18);
	 * max96792_write_reg(dev, 0x1421, 0x08);
	 * max96792_write_reg(dev, 0x1521, 0x08);
	 * max96792_write_reg(dev, 0x14A5, 0x70);
	 * max96792_write_reg(dev, 0x15A5, 0x70);
	 *
	 * msleep(100);
	 * max96792_write_reg(dev, MAX96792_CTRL0_ADDR, 0x21);
	 */

	max96792_write_reg(dev, 0x40, 0x16);

	max96792_write_reg(dev, 0x2C5, 0x80 | GPIO_OUT_DIS | GPIO_TX_EN);
	max96792_write_reg(dev, 0x2C6, 0x6F);

	max96792_write_reg(dev, 0x2C8, 0x80 | GPIO_OUT_DIS | GPIO_TX_EN);

error:
	mutex_unlock(&priv->lock);
	return err;
}
EXPORT_SYMBOL(max96792_setup_control);

int max96792_xvs_setup(struct device *dev, bool direction)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;

	mutex_lock(&priv->lock);

	if (direction == max96792_OUT) {
		err = max96792_write_reg(dev, 0x2B0, 0x80 | GPIO_RX_EN);
		err |= max96792_write_reg(dev, 0x2B1, 0xA0);
		err |= max96792_write_reg(dev, 0x2B2, 0x70);
	} else {
		err = max96792_write_reg(dev, 0x2B0, 0x80 | GPIO_OUT_DIS | GPIO_TX_EN);
		err |= max96792_write_reg(dev, 0x2B1, 0x70);
		err |= max96792_write_reg(dev, 0x2B2, 0x40);
	}

	mutex_unlock(&priv->lock);
	return err;
}
EXPORT_SYMBOL(max96792_xvs_setup);

int max96792_reset_control(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;

	mutex_lock(&priv->lock);
	if (!priv->sdev_ref) {
		dev_info(dev, "%s: dev is already in reset state\n", __func__);
		goto ret;
	}

	priv->sdev_ref--;
	if (priv->sdev_ref == 0) {
		max96792_reset_ctx(priv);
		max96792_write_reg(dev, MAX96792_CTRL0_ADDR, MAX96792_RESET_ALL);

		msleep(100);
	}

ret:
	mutex_unlock(&priv->lock);

	return err;
}
EXPORT_SYMBOL(max96792_reset_control);

int max96792_sdev_register(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct max96792 *priv = NULL;
	int i;
	int err = 0;

	if (!dev || !g_ctx || !g_ctx->s_dev) {
		dev_err(dev, "%s: invalid input params\n", __func__);
		return -EINVAL;
	}

	priv = dev_get_drvdata(dev);

	mutex_lock(&priv->lock);

	if (priv->num_src > priv->max_src) {
		dev_err(dev,
			"%s: MAX96792 inputs size exhausted\n", __func__);
		err = -ENOMEM;
		goto error;
	}

	if (!((priv->csi_mode == MAX96792_CSI_MODE_2X4) ?
			((g_ctx->csi_mode == GMSL_CSI_1X4_MODE) ||
				(g_ctx->csi_mode == GMSL_CSI_2X4_MODE)) :
			((g_ctx->csi_mode == GMSL_CSI_2X2_MODE) ||
				(g_ctx->csi_mode == GMSL_CSI_4X2_MODE)))) {
		dev_err(dev,
			"%s: csi mode not supported\n", __func__);
		err = -EINVAL;
		goto error;
	}

	for (i = 0; i < priv->num_src; i++) {
		if (g_ctx->serdes_csi_link ==
			priv->sources[i].g_ctx->serdes_csi_link) {
			dev_err(dev,
				"%s: serdes csi link is in use\n", __func__);
			err = -EINVAL;
			goto error;
		}

		if (g_ctx->num_csi_lanes !=
				priv->sources[i].g_ctx->num_csi_lanes) {
			dev_err(dev,
				"%s: csi num lanes mismatch\n", __func__);
			err = -EINVAL;
			goto error;
		}
	}

	priv->sources[priv->num_src].g_ctx = g_ctx;
	priv->sources[priv->num_src].st_enabled = false;

	priv->num_src++;

error:
	mutex_unlock(&priv->lock);
	return err;
}
EXPORT_SYMBOL(max96792_sdev_register);

int max96792_sdev_unregister(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = NULL;
	int err = 0;
	int i = 0;

	if (!dev || !s_dev) {
		dev_err(dev, "%s: invalid input params\n", __func__);
		return -EINVAL;
	}

	priv = dev_get_drvdata(dev);
	mutex_lock(&priv->lock);

	if (priv->num_src == 0) {
		dev_err(dev, "%s: no source found\n", __func__);
		err = -ENODATA;
		goto error;
	}

	for (i = 0; i < priv->num_src; i++) {
		if (s_dev == priv->sources[i].g_ctx->s_dev) {
			priv->sources[i].g_ctx = NULL;
			priv->num_src--;
			break;
		}
	}

	if (i == priv->num_src) {
		dev_err(dev,
			"%s: requested device not found\n", __func__);
		err = -EINVAL;
		goto error;
	}

error:
	mutex_unlock(&priv->lock);
	return err;
}
EXPORT_SYMBOL(max96792_sdev_unregister);

struct reg_pair {
	u16 addr;
	u8 val;
};

int max96792_start_streaming(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	int err = 0;
	int i = 0;

	err = max96792_get_sdev_idx(dev, s_dev, &i);
	if (err)
		return err;

	mutex_lock(&priv->lock);

#ifdef PIPE_Y
	max96792_write_reg(dev, 0x112, 0x30);
	msleep(100);
	max96792_write_reg(dev, 0x112, 0x31);
#endif
#ifdef PIPE_Z
	max96792_write_reg(dev, 0x124, 0x20);
	msleep(100);
	max96792_write_reg(dev, 0x124, 0x21);
#endif

	mutex_unlock(&priv->lock);

	return 0;
}
EXPORT_SYMBOL(max96792_start_streaming);

int max96792_stop_streaming(struct device *dev, struct device *s_dev)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	struct gmsl_link_ctx *g_ctx;
	int err = 0;
	int i = 0;

	err = max96792_get_sdev_idx(dev, s_dev, &i);
	if (err)
		return err;

	mutex_lock(&priv->lock);
	g_ctx = priv->sources[i].g_ctx;

	mutex_unlock(&priv->lock);

	return 0;
}
EXPORT_SYMBOL(max96792_stop_streaming);

int max96792_setup_streaming(struct device *dev, struct device *s_dev,
					struct camera_common_data *s_data)
{
	struct max96792 *priv = dev_get_drvdata(dev);
	struct gmsl_link_ctx *g_ctx;
	int err = 0;
	int i = 0;
	u16 lane_ctrl_addr;

	err = max96792_get_sdev_idx(dev, s_dev, &i);
	if (err)
		return err;

	mutex_lock(&priv->lock);

	g_ctx = priv->sources[i].g_ctx;

	switch (g_ctx->dst_csi_port) {
	case GMSL_CSI_PORT_A:
	case GMSL_CSI_PORT_D:
		lane_ctrl_addr = MAX96792_LANE_CTRL1_ADDR;
		break;
	case GMSL_CSI_PORT_B:
	case GMSL_CSI_PORT_E:
		lane_ctrl_addr = MAX96792_LANE_CTRL2_ADDR;
		break;
	case GMSL_CSI_PORT_C:
		lane_ctrl_addr = MAX96792_LANE_CTRL0_ADDR;
		break;
	case GMSL_CSI_PORT_F:
		lane_ctrl_addr = MAX96792_LANE_CTRL3_ADDR;
		break;
	default:
		dev_err(dev, "%s: invalid gmsl csi port!\n", __func__);
		err = -EINVAL;
		goto ret;
	};

	dev_dbg(dev, "%s: lane_ctrl_addr: %x\n", __func__, lane_ctrl_addr);
	max96792_write_reg(dev, lane_ctrl_addr,
		(MAX96792_LANE_CTRL_MAP(g_ctx->num_csi_lanes-1) | 0x10));


	if (!priv->lane_setup) {
		max96792_write_reg(dev,
			MAX96792_LANE_MAP1_ADDR, priv->lane_mp1);
		max96792_write_reg(dev,
			MAX96792_LANE_MAP2_ADDR, priv->lane_mp2);

		priv->lane_setup = true;
	}

	if (!strcmp(s_data->pdata->gmsl, "gmsl")) {
		if (g_ctx->num_csi_lanes == 4)
			max96792_write_reg(dev, 0x474, 0x19);
		else
			max96792_write_reg(dev, 0x474, 0x09);
	} else
		max96792_write_reg(dev, 0x474, 0x08);

ret:
	mutex_unlock(&priv->lock);
	return err;
}
EXPORT_SYMBOL(max96792_setup_streaming);

const struct of_device_id max96792_of_match[] = {
	{ .compatible = "framos,max96792", },
	{ },
};
MODULE_DEVICE_TABLE(of, max96792_of_match);

static int max96792_parse_dt(struct max96792 *priv,
				struct i2c_client *client)
{
	struct device_node *max96792_node = client->dev.of_node;
	int err = 0;
	const char *str_value;
	int value;
	const struct of_device_id *match;
	struct device_node *i2c_mux_ch_node;

	if (!max96792_node)
		return -EINVAL;

	match = of_match_device(max96792_of_match, &client->dev);
	if (!match) {
		dev_err(&client->dev, "Failed to find matching dt id\n");
		return -EFAULT;
	}

	err = of_property_read_string(max96792_node, "csi-mode", &str_value);
	if (err < 0) {
		dev_err(&client->dev, "csi-mode property not found\n");
		return err;
	}

	if (!strcmp(str_value, "2x4")) {
		priv->csi_mode = MAX96792_CSI_MODE_2X4;
		priv->lane_mp1 = MAX96792_LANE_MAP1_2X4;
		priv->lane_mp2 = MAX96792_LANE_MAP2_2X4;
	} else if (!strcmp(str_value, "4x2")) {
		priv->csi_mode = MAX96792_CSI_MODE_4X2;
		priv->lane_mp1 = MAX96792_LANE_MAP1_4X2;
		priv->lane_mp2 = MAX96792_LANE_MAP2_4X2;
	} else {
		dev_err(&client->dev, "invalid csi mode\n");
		return -EINVAL;
	}

	err = of_property_read_u32(max96792_node, "max-src", &value);
	if (err < 0) {
		dev_err(&client->dev, "No max-src info\n");
		return err;
	}
	priv->max_src = value;

	i2c_mux_ch_node = of_get_parent(max96792_node);
	if (!i2c_mux_ch_node) {
		dev_err(&client->dev, "i2c mux channel node not found in dt\n");
		return -EFAULT;
	}
	priv->reset_gpio = of_get_named_gpio(i2c_mux_ch_node, "reset-gpios", 0);
	err = cam_gpio_register(&client->dev, priv->reset_gpio);

	if (priv->reset_gpio < 0) {
		dev_err(&client->dev, "reset_gpio not found %d\n", err);
		return err;
	}

	if (of_get_property(max96792_node, "vdd_cam_1v2-supply", NULL)) {
		priv->vdd_cam_1v2 = regulator_get(&client->dev, "vdd_cam_1v2");
		if (IS_ERR(priv->vdd_cam_1v2)) {
			dev_err(&client->dev,
				"vdd_cam_1v2 regulator get failed\n");
			err = PTR_ERR(priv->vdd_cam_1v2);
			priv->vdd_cam_1v2 = NULL;
			return err;
		}
	} else {
		priv->vdd_cam_1v2 = NULL;
	}

	return 0;
}

static struct regmap_config max96792_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
};

#if defined(NV_I2C_DRIVER_STRUCT_PROBE_WITHOUT_I2C_DEVICE_ID_ARG) /* Linux 6.3 */
static int max96792_probe(struct i2c_client *client)
#else
static int max96792_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
#endif
{
	struct max96792 *priv;
	int err = 0;

	dev_info(&client->dev, "[MAX96792]: probing GMSL Deserializer\n");

	priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
	priv->i2c_client = client;
	priv->regmap = devm_regmap_init_i2c(priv->i2c_client,
				&max96792_regmap_config);
	if (IS_ERR(priv->regmap)) {
		dev_err(&client->dev,
			"regmap init failed: %ld\n", PTR_ERR(priv->regmap));
		return -ENODEV;
	}

	err = max96792_parse_dt(priv, client);
	if (err) {
		dev_err(&client->dev, "unable to parse dt\n");
		return -EFAULT;
	}

	max96792_pipes_reset(priv);

	if (priv->max_src > MAX96792_MAX_SOURCES) {
		dev_err(&client->dev,
			"max sources more than currently supported\n");
		return -EINVAL;
	}

	mutex_init(&priv->lock);

	dev_set_drvdata(&client->dev, priv);

	dev_info(&client->dev, "%s: success\n", __func__);

	return err;
}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
static int max96792_remove(struct i2c_client *client)
#else
static void max96792_remove(struct i2c_client *client)
#endif
{
	struct max96792 *priv;

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		dev_dbg(&client->dev, "Removed max96792 module device\n");
		mutex_destroy(&priv->lock);
		devm_kfree(&client->dev, priv);
		client = NULL;
	}

#if defined(NV_I2C_DRIVER_STRUCT_REMOVE_RETURN_TYPE_INT) /* Linux 6.1 */
	return 0;
#endif
}

static const struct i2c_device_id max96792_id[] = {
	{ "max96792", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, max96792_id);

static struct i2c_driver max96792_i2c_driver = {
	.driver = {
		.name = "max96792",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(max96792_of_match),
	},
	.probe = max96792_probe,
	.remove = max96792_remove,
	.id_table = max96792_id,
};

static int __init max96792_init(void)
{
	return i2c_add_driver(&max96792_i2c_driver);
}

static void __exit max96792_exit(void)
{
	i2c_del_driver(&max96792_i2c_driver);
}

module_init(max96792_init);
module_exit(max96792_exit);

MODULE_DESCRIPTION("GMSL Deserializer driver for max96792");
MODULE_AUTHOR("FRAMOS GmbH");
MODULE_LICENSE("GPL v2");
