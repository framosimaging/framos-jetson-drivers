// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023, NVIDIA Corporation.
 */

#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/host1x-next.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/seq_file.h>
#include <linux/version.h>

#include <soc/tegra/virt/hv-ivc.h>

#include "drm.h"

#ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
#include <uapi/linux/tegra-soc-hwpm-uapi.h>
#endif

#define TEGRA_VHOST_CMD_CONNECT			0
#define TEGRA_VHOST_CMD_DISCONNECT		1
#define TEGRA_VHOST_CMD_SUSPEND			2
#define TEGRA_VHOST_CMD_RESUME			3
#define TEGRA_VHOST_CMD_GET_CONNECTION_ID	4

struct tegra_vhost_connect_params {
	u32 module;
	u64 connection_id;
};

struct tegra_vhost_cmd_msg {
	u32 cmd;
	int ret;
	u64 connection_id;
	struct tegra_vhost_connect_params connect;
};

struct virt_engine {
	struct device *dev;
	int connection_id;

	struct tegra_drm_client client;

	struct dentry *actmon_debugfs_dir;
	struct clk *clk;
};

static DEFINE_MUTEX(ivc_cookie_lock);
static struct tegra_hv_ivc_cookie *ivc_cookie;
static struct kref ivc_ref;

static inline struct virt_engine *to_virt_engine(struct tegra_drm_client *client)
{
	return container_of(client, struct virt_engine, client);
}

static int virt_engine_init(struct host1x_client *client)
{
	struct tegra_drm_client *drm = host1x_to_drm_client(client);
	struct drm_device *dev = dev_get_drvdata(client->host);
	struct tegra_drm *tegra = dev->dev_private;
	int err;

	err = tegra_drm_register_client(tegra, drm);
	if (err < 0)
		return err;

	/*
	 * Inherit the DMA parameters (such as maximum segment size) from the
	 * parent host1x device.
	 */
	client->dev->dma_parms = client->host->dma_parms;

	return 0;
}

static int virt_engine_exit(struct host1x_client *client)
{
	struct tegra_drm_client *drm = host1x_to_drm_client(client);
	struct drm_device *dev = dev_get_drvdata(client->host);
	struct tegra_drm *tegra = dev->dev_private;
	int err;

	/* avoid a dangling pointer just in case this disappears */
	client->dev->dma_parms = NULL;

	err = tegra_drm_unregister_client(tegra, drm);
	if (err < 0)
		return err;

	return 0;
}

static const struct host1x_client_ops virt_engine_client_ops = {
	.init = virt_engine_init,
	.exit = virt_engine_exit,
};

static int virt_engine_open_channel(struct tegra_drm_client *client,
				    struct tegra_drm_context *context)
{
	return -EINVAL;
}

static void virt_engine_close_channel(struct tegra_drm_context *context)
{
}

static int virt_engine_can_use_memory_ctx(struct tegra_drm_client *client, bool *supported)
{
	*supported = true;

	return 0;
}

static int virt_engine_has_job_timestamping(struct tegra_drm_client *client, bool *supported)
{
	struct virt_engine *virt = to_virt_engine(client);

	switch (virt->client.base.class) {
	case HOST1X_CLASS_NVJPG:
	case HOST1X_CLASS_NVJPG1:
		*supported = false;
		break;
	default:
		*supported = true;
		break;
	}

	return 0;
}

static const struct tegra_drm_client_ops virt_engine_ops = {
	.open_channel = virt_engine_open_channel,
	.close_channel = virt_engine_close_channel,
	.submit = tegra_drm_submit,
	.get_streamid_offset = tegra_drm_get_streamid_offset_thi,
	.can_use_memory_ctx = virt_engine_can_use_memory_ctx,
	.has_job_timestamping = virt_engine_has_job_timestamping,
};

static int virt_engine_setup_ivc(struct virt_engine *virt)
{
	struct device_node *host1x_dn = virt->dev->parent->of_node;
	struct device_node *hv;
	u32 ivc_instance;
	int err;

	mutex_lock(&ivc_cookie_lock);

	if (ivc_cookie) {
		err = PTR_ERR_OR_ZERO(ivc_cookie);
		if (!err)
			kref_get(&ivc_ref);
		mutex_unlock(&ivc_cookie_lock);
		return err;
	}

	hv = of_parse_phandle(host1x_dn, "nvidia,server-ivc", 0);
	if (!hv) {
		dev_err(virt->dev, "nvidia,server-ivc not configured\n");
		return -EINVAL;
	}

	err = of_property_read_u32_index(host1x_dn, "nvidia,server-ivc", 1, &ivc_instance);
	if (err) {
		dev_err(virt->dev, "nvidia,server-ivc not configured\n");
		return -EINVAL;
	}

	ivc_cookie = tegra_hv_ivc_reserve(hv, ivc_instance, NULL);
	if (IS_ERR(ivc_cookie)) {
		dev_err(virt->dev, "IVC channel reservation failed: %ld\n", PTR_ERR(ivc_cookie));
		return PTR_ERR(ivc_cookie);
	}

	tegra_hv_ivc_channel_reset(ivc_cookie);

	while (tegra_hv_ivc_channel_notified(ivc_cookie))
		;

	kref_init(&ivc_ref);

	mutex_unlock(&ivc_cookie_lock);

	return 0;
}

static void release_ivc_cookie(struct kref *ref)
{
	(void)ref;

	tegra_hv_ivc_unreserve(ivc_cookie);
	mutex_unlock(&ivc_cookie_lock);
}

static void virt_engine_cleanup(void)
{
	kref_put_mutex(&ivc_ref, release_ivc_cookie, &ivc_cookie_lock);
}

static int virt_engine_transfer(void *data, u32 size)
{
	int err;

	mutex_lock(&ivc_cookie_lock);

	while (!tegra_hv_ivc_can_write(ivc_cookie))
		;

	err = tegra_hv_ivc_write(ivc_cookie, data, size);
	if (err != size) {
		mutex_unlock(&ivc_cookie_lock);
		return -ENOMEM;
	}

	while (!tegra_hv_ivc_can_read(ivc_cookie))
		;

	err = tegra_hv_ivc_read(ivc_cookie, data, size);
	if (err != size) {
		mutex_unlock(&ivc_cookie_lock);
		return -EIO;
	}

	mutex_unlock(&ivc_cookie_lock);
	return 0;
}

static int virt_engine_connect(struct virt_engine *virt, u32 module_id)
{
	struct tegra_vhost_cmd_msg msg = { 0 };
	int err;

	msg.cmd = TEGRA_VHOST_CMD_CONNECT;
	msg.connect.module = module_id;

	err = virt_engine_transfer(&msg, sizeof(msg));
	if (err < 0)
		return err;

	return msg.connect.connection_id;
}

static int virt_engine_suspend(struct device *dev)
{
	struct virt_engine *virt = dev_get_drvdata(dev);
	struct tegra_vhost_cmd_msg msg = { 0 };

	msg.cmd = TEGRA_VHOST_CMD_SUSPEND;
	msg.connection_id = virt->connection_id;

	return virt_engine_transfer(&msg, sizeof(msg));
}

static int virt_engine_resume(struct device *dev)
{
	struct virt_engine *virt = dev_get_drvdata(dev);
	struct tegra_vhost_cmd_msg msg = { 0 };

	msg.cmd = TEGRA_VHOST_CMD_RESUME;
	msg.connection_id = virt->connection_id;

	return virt_engine_transfer(&msg, sizeof(msg));
}

static int actmon_debugfs_usage_show(struct seq_file *s, void *unused)
{
	struct virt_engine *virt = s->private;
	unsigned long rate;
	int cycles_per_actmon_sample;
	int count;

	rate = clk_get_rate(virt->clk);
	if (rate == 0) {
		seq_printf(s, "0\n");
		return 0;
	}

	count = host1x_actmon_read_avg_count(&virt->client.base);
	if (count < 0)
		return count;

	/* Based on configuration in NvHost Server */
#define ACTMON_SAMPLE_PERIOD_US		100
	/* Rate in MHz cancels out microseconds */
	cycles_per_actmon_sample = (rate / 1000000) * ACTMON_SAMPLE_PERIOD_US;

	seq_printf(s, "%d\n", (count * 1000) / cycles_per_actmon_sample);

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(actmon_debugfs_usage);

static void virt_engine_setup_actmon_debugfs(struct virt_engine *virt)
{
	const char *name;

	switch (virt->client.base.class) {
	case HOST1X_CLASS_VIC:
		name = "vic";
		break;
	case HOST1X_CLASS_NVENC:
		name = "msenc";
		break;
	case HOST1X_CLASS_NVDEC:
		name = "nvdec";
		break;
	default:
		return;
	}

	virt->actmon_debugfs_dir = debugfs_create_dir(name, NULL);
	debugfs_create_file("usage", S_IRUGO, virt->actmon_debugfs_dir, virt, &actmon_debugfs_usage_fops);
}

static void virt_engine_cleanup_actmon_debugfs(struct virt_engine *virt)
{
	debugfs_remove_recursive(virt->actmon_debugfs_dir);
}

static const struct of_device_id tegra_virt_engine_of_match[] = {
	{ .compatible = "nvidia,tegra234-host1x-virtual-engine" },
	{ },
};
MODULE_DEVICE_TABLE(of, tegra_virt_engine_of_match);

#ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
static u32 virt_engine_get_ip_index(const char *name)
{
	if (strstr(name, "vic")) {
		return (u32)TEGRA_SOC_HWPM_RESOURCE_VIC;
	} else if (strstr(name, "nvenc")) {
		return (u32)TEGRA_SOC_HWPM_RESOURCE_NVENC;
	} else if (strstr(name, "ofa")) {
		return (u32)TEGRA_SOC_HWPM_RESOURCE_OFA;
	} else if (strstr(name, "nvdec")) {
		return (u32)TEGRA_SOC_HWPM_RESOURCE_NVDEC;
	}
	return (u32)TERGA_SOC_HWPM_NUM_IPS;
}

static u32 virt_engine_extract_base_addr(struct platform_device *pdev)
{
	u32 hwpm_ip_index;
	u32 base_address;

	hwpm_ip_index = virt_engine_get_ip_index(pdev->name);
	if (hwpm_ip_index == TEGRA_SOC_HWPM_RESOURCE_VIC) {
		base_address = 0x15340000;
	} else if (hwpm_ip_index == TEGRA_SOC_HWPM_RESOURCE_NVENC) {
		base_address = 0x154c0000;
	} else if (hwpm_ip_index == TEGRA_SOC_HWPM_RESOURCE_OFA) {
		base_address = 0x15a50000;
	} else if (hwpm_ip_index == TEGRA_SOC_HWPM_RESOURCE_NVDEC) {
		base_address = 0x15480000;
	} else {
		dev_err(&pdev->dev, "IP Base address not found");
		return -ENOMEM;
	}

	return base_address;
}

static int virt_engine_ip_pm(void *ip_dev, bool disable)
{
	int err = 0;
	struct platform_device *pdev = (struct platform_device *)ip_dev;

	if (disable) {
		dev_warn(&pdev->dev, "Power Mgmt not impleted in vhost_engine."
				"IP expected to be ON");
	} else {
		dev_warn(&pdev->dev, "Power Mgmt not impleted in vhost_engine."
				"IP expected to be ON");
	}

	return err;
}

static int virt_engine_ip_reg_op(void *ip_dev,
		enum tegra_soc_hwpm_ip_reg_op reg_op,
		u32 inst_element_index, u64 reg_offset, u32 *reg_data)
{
	struct platform_device *pdev = (struct platform_device *)ip_dev;
	u32 base_address;

	base_address = virt_engine_extract_base_addr(pdev);
	if (base_address <= 0) {
		dev_err(&pdev->dev, "IP Base address not found. HWPM Reg_OP failed");
		return -ENOMEM;
	}

	if (reg_op == TEGRA_SOC_HWPM_IP_REG_OP_READ) {
		void __iomem *ptr = NULL;

		reg_offset = reg_offset + base_address;
		ptr = ioremap(reg_offset, 0x4);
		if (!ptr) {
			dev_err(&pdev->dev, "Failed to map IP Perfmux register (0x%llx)",
					reg_offset);
		}
		*reg_data = __raw_readl(ptr);
		iounmap(ptr);
	} else if (reg_op == TEGRA_SOC_HWPM_IP_REG_OP_WRITE) {
		void __iomem *ptr = NULL;

		reg_offset = reg_offset + base_address;

		ptr = ioremap(reg_offset, 0x4);
		if (!ptr) {
			dev_err(&pdev->dev, "Failed to map IP Perfmux register (0x%llx)",
					reg_offset);
		}
		__raw_writel(*reg_data, ptr);
		iounmap(ptr);
	}

	return 0;
}
#endif

static int virt_engine_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	u32 module_id, class;
	struct virt_engine *virt;
	int err;
#ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
        struct tegra_soc_hwpm_ip_ops hwpm_ip_ops;
        u32 hwpm_ip_index;
#endif

	err = of_property_read_u32(pdev->dev.of_node, "nvidia,module-id", &module_id);
	if (err < 0) {
		dev_err(dev, "could not read property nvidia,module-id: %d\n", err);
		return err;
	}

	err = of_property_read_u32(pdev->dev.of_node, "nvidia,class", &class);
	if (err < 0) {
		dev_err(dev, "could not read property nvidia,class: %d\n", err);
		return err;
	}

	/* inherit DMA mask from host1x parent */
	err = dma_coerce_mask_and_coherent(dev, *dev->parent->dma_mask);
	if (err < 0) {
		dev_err(dev, "failed to set DMA mask: %d\n", err);
		return err;
	}

	virt = devm_kzalloc(dev, sizeof(*virt), GFP_KERNEL);
	if (!virt)
		return -ENOMEM;

	platform_set_drvdata(pdev, virt);

	virt->clk = devm_clk_get_optional(&pdev->dev, NULL);
	if (IS_ERR(virt->clk)) {
		dev_err(dev, "could not get clock: %ld\n", PTR_ERR(virt->clk));
		return PTR_ERR(virt->clk);
	}

	INIT_LIST_HEAD(&virt->client.base.list);
	virt->client.base.ops = &virt_engine_client_ops;
	virt->client.base.dev = dev;
	virt->client.base.class = class;
	virt->client.base.syncpts = NULL;
	virt->client.base.num_syncpts = 0;
	virt->dev = dev;

	INIT_LIST_HEAD(&virt->client.list);
	virt->client.version = 0x23;
	virt->client.ops = &virt_engine_ops;

	err = host1x_client_register(&virt->client.base);
	if (err < 0) {
		dev_err(dev, "failed to register host1x client: %d\n", err);
		return err;
	}

	err = virt_engine_setup_ivc(virt);
	if (err < 0)
		goto unregister_client;

	/* Connect to HOST module. This doesn't really do anything but we need to do it first. */
	virt_engine_connect(virt, 1);

	virt->connection_id = virt_engine_connect(virt, module_id);
	if (virt->connection_id < 0) {
		dev_err(dev, "failed to register with server\n");
		err = -EIO;
		goto cleanup_ivc;
	}

	virt_engine_setup_actmon_debugfs(virt);

#ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
	hwpm_ip_index = virt_engine_get_ip_index(pdev->name);
	if (hwpm_ip_index != TERGA_SOC_HWPM_NUM_IPS) {
		hwpm_ip_ops.ip_dev = (void *)pdev;
		hwpm_ip_ops.ip_base_address = virt_engine_extract_base_addr(pdev);
		if (hwpm_ip_ops.ip_base_address <= 0) {
			dev_err(&pdev->dev,
					"IP Base address not found. HWPM-IP registration failed");
			return 0;
		}
		hwpm_ip_ops.resource_enum = hwpm_ip_index;
		hwpm_ip_ops.hwpm_ip_pm = &virt_engine_ip_pm;
		hwpm_ip_ops.hwpm_ip_reg_op = &virt_engine_ip_reg_op;
		tegra_soc_hwpm_ip_register(&hwpm_ip_ops);
	}
#endif
	return 0;

cleanup_ivc:
	virt_engine_cleanup();
unregister_client:
	host1x_client_unregister(&virt->client.base);

	return err;
}

static int virt_engine_remove(struct platform_device *pdev)
{
	struct virt_engine *virt_engine = platform_get_drvdata(pdev);
#ifndef CONFIG_TEGRA_SYSTEM_TYPE_ACK
	struct tegra_soc_hwpm_ip_ops hwpm_ip_ops;
	u32 hwpm_ip_index = virt_engine_get_ip_index(pdev->name);

	if (hwpm_ip_index != TERGA_SOC_HWPM_NUM_IPS) {
		hwpm_ip_ops.ip_dev = (void *)pdev;
		hwpm_ip_ops.ip_base_address = virt_engine_extract_base_addr(pdev);
		if (hwpm_ip_ops.ip_base_address <= 0) {
			dev_err(&pdev->dev,
					"IP Base address not found. HWPM-IP Un-register failed");
			return 0;
		}
		hwpm_ip_ops.resource_enum = hwpm_ip_index;
		tegra_soc_hwpm_ip_unregister(&hwpm_ip_ops);
	}
#endif
	virt_engine_cleanup_actmon_debugfs(virt_engine);
	virt_engine_cleanup();

	host1x_client_unregister(&virt_engine->client.base);

	return 0;
}

static const struct dev_pm_ops virt_engine_pm_ops = {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
	SYSTEM_SLEEP_PM_OPS(virt_engine_suspend, virt_engine_resume)
#else
	SET_SYSTEM_SLEEP_PM_OPS(virt_engine_suspend, virt_engine_resume)
#endif
};

struct platform_driver tegra_virt_engine_driver = {
	.driver = {
		.name = "tegra-host1x-virtual-engine",
		.of_match_table = tegra_virt_engine_of_match,
		.pm = &virt_engine_pm_ops,
	},
	.probe = virt_engine_probe,
	.remove = virt_engine_remove,
};
