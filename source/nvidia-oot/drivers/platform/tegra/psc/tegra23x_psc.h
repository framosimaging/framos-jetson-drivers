// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2020-2023, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

struct mbox_controller;

#if defined(CONFIG_DEBUG_FS)
int psc_debugfs_create(struct platform_device *pdev, struct mbox_controller *mbox);
void psc_debugfs_remove(struct platform_device *pdev);
#else
/* We should probably never be here as this driver is useless without debugfs */
static inline int psc_debugfs_create(struct platform_device *pdev, struct mbox_controller *mbox)
{
	return 0;
}
static inline void psc_debugfs_remove(struct platform_device *pdev) { return; }
#endif

struct mbox_chan *psc_mbox_request_channel0(struct mbox_controller *mbox, struct mbox_client *cl);

#if IS_ENABLED(CONFIG_NUMA)
#define PSC_HAVE_NUMA
#endif

#if defined(PSC_HAVE_NUMA)
u32 readl0(void __iomem *addr);
void writel0(u32 value, void __iomem *addr);
#else
#define readl0(addr) readl((addr))
#define writel0(value, addr) writel((value), (addr))
#endif
