// SPDX-License-Identifier: GPL-2.0-only
// SPDX-FileCopyrightText: Copyright (c) 2025-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

#include <nvidia/conftest.h>

#include <linux/clk.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <soc/tegra/bpmp.h>
#include <soc/tegra/bpmp-abi.h>

#define TEGRA264_STRAP_NV_FUSE_CTRL_OPT_GPU		1U
#define TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVDEC		2U
#define TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVENC		3U
#define TEGRA264_STRAP_NV_FUSE_CTRL_OPT_OFA		4U
#define TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVJPG		5U

#define STRAP_MASK_PARAM_DEFAULT 0xFFFFFFFF

/* module parameters for T264 static power-gating straps */
uint gpu_pg_mask_param = STRAP_MASK_PARAM_DEFAULT;
module_param(gpu_pg_mask_param, uint, 0664);
MODULE_PARM_DESC(gpu_pg_mask_param, "T264 GPU GPC/TPC/FBP Power-Gating mask");

static uint nvdec_pg_mask_param = STRAP_MASK_PARAM_DEFAULT;
module_param(nvdec_pg_mask_param, uint, 0664);
MODULE_PARM_DESC(nvdec_pg_mask_param, "T264 NVDEC Power-Gating mask");

static uint nvenc_pg_mask_param = STRAP_MASK_PARAM_DEFAULT;
module_param(nvenc_pg_mask_param, uint, 0664);
MODULE_PARM_DESC(nvenc_pg_mask_param, "T264 NVENC Power-Gating mask");

static uint ofa_pg_mask_param = STRAP_MASK_PARAM_DEFAULT;
module_param(ofa_pg_mask_param, uint, 0664);
MODULE_PARM_DESC(ofa_pg_mask_param, "T264 OFA Power-Gating mask");

static uint nvjpg_pg_mask_param = STRAP_MASK_PARAM_DEFAULT;
module_param(nvjpg_pg_mask_param, uint, 0664);
MODULE_PARM_DESC(nvjpg_pg_mask_param, "T264 NVJPG Power-Gating mask");

struct tegra_gpu_pg_profile {
	struct kobj_attribute attr;
	uint32_t gpu_pg_mask;
	uint32_t strap_id;

	/* lock to protect the gpu_pg_mask */
	struct mutex lock;

	struct tegra_bpmp *gpu_pg_bpmp;
};

/* Static descriptor table – one entry per supported strap. */
struct tegra_strap_desc {
	uint32_t strap_id;
	const char *sysfs_name;
	uint *param;
};

static const struct tegra_strap_desc tegra264_strap_table[] = {
	{ TEGRA264_STRAP_NV_FUSE_CTRL_OPT_GPU,   "gpu_pg_mask",   &gpu_pg_mask_param   },
	{ TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVDEC, "nvdec_pg_mask", &nvdec_pg_mask_param },
	{ TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVENC, "nvenc_pg_mask", &nvenc_pg_mask_param },
	{ TEGRA264_STRAP_NV_FUSE_CTRL_OPT_OFA,   "ofa_pg_mask",   &ofa_pg_mask_param   },
	{ TEGRA264_STRAP_NV_FUSE_CTRL_OPT_NVJPG, "nvjpg_pg_mask", &nvjpg_pg_mask_param },
};

#define NUM_TEGRA264_STRAPS ARRAY_SIZE(tegra264_strap_table)

struct tegra_gpu_pg_profile_drv_data {
	struct kobject *gpu_static_pg_kobject;
	struct tegra_bpmp *gpu_pg_bpmp;
	struct tegra_gpu_pg_profile *gpu_pg_profile; /* array of NUM_TEGRA264_STRAPS */
};

static bool is_strap_param_set(uint param)
{
	return param != STRAP_MASK_PARAM_DEFAULT;
}

static ssize_t bpmp_set_gpu_pg_mask(struct tegra_bpmp *bpmp, uint32_t strap_id,
				     uint32_t gpu_pg_mask)
{
	struct mrq_strap_request req = { 0 };
	struct tegra_bpmp_message msg;
	int ret = 0;

	memset(&req, 0, sizeof(req));
	req.cmd = STRAP_SET;
	req.id = strap_id;
	req.value = gpu_pg_mask;

	memset(&msg, 0, sizeof(struct tegra_bpmp_message));
	msg.mrq = MRQ_STRAP;
	msg.tx.data = &req;
	msg.tx.size = sizeof(struct mrq_strap_request);
	msg.rx.data = NULL;
	msg.rx.size = 0;
	ret = tegra_bpmp_transfer(bpmp, &msg);

	if (ret != 0)
		ret = -EIO;

	return ret;
}

static ssize_t gpu_pg_mask_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	struct tegra_gpu_pg_profile *gpu_pg_profile;

	gpu_pg_profile = container_of(attr, struct tegra_gpu_pg_profile, attr);

	return sprintf(buf, "%u\n", gpu_pg_profile->gpu_pg_mask);
}

static ssize_t gpu_pg_mask_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,
			     size_t count)
{
	struct tegra_gpu_pg_profile *gpu_pg_profile;
	uint32_t gpu_pg_mask;
	int ret = 0;

	gpu_pg_profile = container_of(attr, struct tegra_gpu_pg_profile, attr);

	ret = kstrtou32(buf, 0, &gpu_pg_mask);
	if (ret)
		return ret;

	mutex_lock(&gpu_pg_profile->lock);
	ret = bpmp_set_gpu_pg_mask(gpu_pg_profile->gpu_pg_bpmp, gpu_pg_profile->strap_id,
				   gpu_pg_mask);
	if (ret) {
		pr_warn("Failed to send BPMP MRQ for %s (strap %u)\n",
			gpu_pg_profile->attr.attr.name, gpu_pg_profile->strap_id);
		mutex_unlock(&gpu_pg_profile->lock);
		return ret;
	} else {
		gpu_pg_profile->gpu_pg_mask = gpu_pg_mask;
	}

	mutex_unlock(&gpu_pg_profile->lock);

	return count;
}

static const struct of_device_id of_nv_gpu_static_pg_match[] = {
	{ .compatible = "nvidia,gpu-static-pg", },
	{},
};
MODULE_DEVICE_TABLE(of, of_nv_gpu_static_pg_match);

static int gpu_static_pg_init(struct platform_device *pdev)
{
	struct tegra_gpu_pg_profile_drv_data *gpu_pg_profile_drv_data = platform_get_drvdata(pdev);
	struct tegra_gpu_pg_profile *profiles;
	int i, ret = 0;

	profiles = devm_kcalloc(&pdev->dev, NUM_TEGRA264_STRAPS, sizeof(*profiles), GFP_KERNEL);
	if (!profiles) {
		dev_err(&pdev->dev, "Failed to allocate gpu_pg_profile!\n");
		return -ENOMEM;
	}

	for (i = 0; i < NUM_TEGRA264_STRAPS; i++) {
		const struct tegra_strap_desc *desc = &tegra264_strap_table[i];
		struct tegra_gpu_pg_profile *p = &profiles[i];

		p->strap_id = desc->strap_id;
		p->gpu_pg_bpmp = gpu_pg_profile_drv_data->gpu_pg_bpmp;
		mutex_init(&p->lock);

		/* Send the PG mask to BPMP if the module param is set */
		if (is_strap_param_set(*desc->param)) {
			dev_info(&pdev->dev, "Sending %s via BPMP MRQ, value = %u.\n",
				 desc->sysfs_name, *desc->param);
			mutex_lock(&p->lock);
			ret = bpmp_set_gpu_pg_mask(gpu_pg_profile_drv_data->gpu_pg_bpmp,
						   p->strap_id, *desc->param);
			mutex_unlock(&p->lock);
			if (ret) {
				dev_err(&pdev->dev,
					"Failed to send the BPMP MRQ for %s param.\n",
					desc->sysfs_name);
				ret = -EINVAL;
				mutex_destroy(&p->lock);
				goto err_cleanup;
			}
			p->gpu_pg_mask = *desc->param;
		}

		p->attr.attr.name = kstrdup_const(desc->sysfs_name, GFP_KERNEL);
		if (!p->attr.attr.name) {
			dev_warn(&pdev->dev, "Couldn't allocate memory for %s\n", desc->sysfs_name);
			ret = -ENOMEM;
			mutex_destroy(&p->lock);
			goto err_cleanup;
		}

		sysfs_attr_init(&p->attr.attr);
		p->attr.attr.mode = 0664;
		p->attr.show = gpu_pg_mask_show;
		p->attr.store = gpu_pg_mask_store;
		if (sysfs_create_file(gpu_pg_profile_drv_data->gpu_static_pg_kobject,
				      &p->attr.attr)) {
			dev_warn(&pdev->dev, "Couldn't create %s sysfs\n", desc->sysfs_name);
			kfree_const(p->attr.attr.name);
			ret = -EINVAL;
			mutex_destroy(&p->lock);
			goto err_cleanup;
		}
	}

	gpu_pg_profile_drv_data->gpu_pg_profile = profiles;
	return 0;

err_cleanup:
	while (--i >= 0) {
		sysfs_remove_file(gpu_pg_profile_drv_data->gpu_static_pg_kobject,
				  &profiles[i].attr.attr);
		kfree_const(profiles[i].attr.attr.name);
		mutex_destroy(&profiles[i].lock);
	}
	devm_kfree(&pdev->dev, profiles);
	return ret;
}

static void gpu_static_pg_deinit(struct platform_device *pdev)
{
	struct tegra_gpu_pg_profile_drv_data *gpu_pg_profile_drv_data;
	struct tegra_gpu_pg_profile *profiles;
	int i;

	gpu_pg_profile_drv_data = platform_get_drvdata(pdev);
	profiles = gpu_pg_profile_drv_data->gpu_pg_profile;

	if (!profiles)
		return;

	for (i = 0; i < NUM_TEGRA264_STRAPS; i++) {
		sysfs_remove_file(gpu_pg_profile_drv_data->gpu_static_pg_kobject,
				  &profiles[i].attr.attr);
		kfree_const(profiles[i].attr.attr.name);
		mutex_destroy(&profiles[i].lock);
	}

	devm_kfree(&pdev->dev, profiles);
	gpu_pg_profile_drv_data->gpu_pg_profile = NULL;
}

static int gpu_static_pg_probe(struct platform_device *pdev)
{
	struct tegra_gpu_pg_profile_drv_data *gpu_pg_profile_drv_data;
	struct kobject *gpu_static_pg_kobject;
	struct tegra_bpmp *bpmp;
	int ret = 0;

	gpu_pg_profile_drv_data = \
		devm_kzalloc(&pdev->dev, sizeof(struct tegra_gpu_pg_profile_drv_data), GFP_KERNEL);

	if (!gpu_pg_profile_drv_data) {
		dev_err(&pdev->dev, "Failed to allocate gpu_pg_profile_drv_data!\n");
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, gpu_pg_profile_drv_data);

	/* Get the corresponding BPMP instance */
	bpmp = tegra_bpmp_get(&pdev->dev);
	if (IS_ERR(bpmp)) {
		dev_err(&pdev->dev, "Failed to get BPMP instance!\n");
		devm_kfree(&pdev->dev, gpu_pg_profile_drv_data);
		gpu_pg_profile_drv_data = NULL;
		return PTR_ERR(bpmp);
	}
	gpu_pg_profile_drv_data->gpu_pg_bpmp = bpmp;

	gpu_static_pg_kobject = kobject_create_and_add("gpu_static_pg", kernel_kobj);
	if (!gpu_static_pg_kobject) {
		dev_err(&pdev->dev, "Failed to create gpu_static_pg sysfs!\n");
		ret = -ENOMEM;
		goto put_bpmp;
	}
	gpu_pg_profile_drv_data->gpu_static_pg_kobject = gpu_static_pg_kobject;

	/* Initialize tegra_gpu_pg_profile and corresponding sysfs */
	ret = gpu_static_pg_init(pdev);
	if (ret) {
		dev_warn(&pdev->dev, "Failed to initialize GPU PG mask!\n");
		goto put_kobject;
	}


	return ret;

put_kobject:
	kobject_put(gpu_static_pg_kobject);
	gpu_static_pg_kobject = NULL;

put_bpmp:
	tegra_bpmp_put(bpmp);
	devm_kfree(&pdev->dev, gpu_pg_profile_drv_data);
	gpu_pg_profile_drv_data = NULL;

	return ret;
}

static int gpu_static_pg_remove(struct platform_device *pdev)
{
	struct tegra_gpu_pg_profile_drv_data *gpu_pg_profile_drv_data = platform_get_drvdata(pdev);

	gpu_static_pg_deinit(pdev);

	kobject_put(gpu_pg_profile_drv_data->gpu_static_pg_kobject);
	tegra_bpmp_put(gpu_pg_profile_drv_data->gpu_pg_bpmp);
	devm_kfree(&pdev->dev, gpu_pg_profile_drv_data);
	gpu_pg_profile_drv_data = NULL;

	return 0;
}

#if defined(NV_PLATFORM_DRIVER_STRUCT_REMOVE_RETURNS_VOID) /* Linux v6.11 */
static void gpu_static_pg_remove_wrapper(struct platform_device *pdev)
{
	gpu_static_pg_remove(pdev);
}
#else
static int gpu_static_pg_remove_wrapper(struct platform_device *pdev)
{
	return gpu_static_pg_remove(pdev);
}
#endif

static struct platform_driver nv_gpu_static_pg_driver = {
	.probe = gpu_static_pg_probe,
	.remove = gpu_static_pg_remove_wrapper,
	.driver = {
		.name = "nv-gpu-static-pg",
		.of_match_table = of_nv_gpu_static_pg_match,
	},
};

module_platform_driver(nv_gpu_static_pg_driver);

MODULE_AUTHOR("Shao-Chun Kao <shaochunk@nvidia.com>");
MODULE_DESCRIPTION("NVIDIA GPU static power-gating driver which receives the GPU PG mask from the user");
MODULE_LICENSE("GPL v2");
