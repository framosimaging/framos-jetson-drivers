// SPDX-License-Identifier: GPL-2.0-only
/*
 *
 *  Realtek Bluetooth USB download firmware driver
 *
 */
#ifndef __RTK_MISC_H__
#define __RTK_MISC_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/pm_runtime.h>
#include <linux/usb.h>
#include <linux/suspend.h>

/* Download LPS patch when host suspends or power off
 *   LPS patch name:  lps_rtl8xxx_fw
 *   LPS config name: lps_rtl8xxx_config
 * Download normal patch when host resume or power on */
/* #define RTKBT_SWITCH_PATCH */

/* RTKBT Power-on for sideband wake-up by LE Advertising from Remote. */
/* Note that it's necessary to apply TV FW Patch. */
/* #define RTKBT_SUSPEND_WAKEUP */
/* #define RTKBT_SHUTDOWN_WAKEUP */
#define RTKBT_POWERKEY_WAKEUP

/* RTKBT Power-on Whitelist for sideband wake-up by LE Advertising from Remote.
 * Note that it's necessary to apply TV FW Patch. */
/* #define RTKBT_TV_POWERON_WHITELIST */

#if 1
#define RTKBT_DBG(fmt, arg...) printk(KERN_DEBUG "rtk_btusb: " fmt "\n" , ## arg)
#define RTKBT_INFO(fmt, arg...) printk(KERN_INFO "rtk_btusb: " fmt "\n" , ## arg)
#define RTKBT_WARN(fmt, arg...) printk(KERN_DEBUG "rtk_btusb: " fmt "\n", ## arg)
#else
#define RTKBT_DBG(fmt, arg...)
#endif

#if 1
#define RTKBT_ERR(fmt, arg...) printk(KERN_DEBUG "rtk_btusb: " fmt "\n" , ## arg)
#else
#define RTKBT_ERR(fmt, arg...)
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 33)
#define USB_RPM
#endif

#define CONFIG_NEEDS_BINDING

/* If module is still powered when kernel suspended, there is no re-binding. */
#ifdef RTKBT_SWITCH_PATCH
#undef CONFIG_NEEDS_BINDING
#endif

/* USB SS */
#if (defined CONFIG_BTUSB_AUTOSUSPEND) && (defined USB_RPM)
#define BTUSB_RPM
#endif

#define PRINT_CMD_EVENT			0
#define PRINT_ACL_DATA			0

extern int patch_add(struct usb_interface *intf);
extern void patch_remove(struct usb_interface *intf);
extern int download_patch(struct usb_interface *intf);
extern void print_event(struct sk_buff *skb);
extern void print_command(struct sk_buff *skb);
extern void print_acl(struct sk_buff *skb, int dataOut);

#if defined RTKBT_SWITCH_PATCH || defined RTKBT_TV_POWERON_WHITELIST
int __rtk_send_hci_cmd(struct usb_device *udev, u8 *buf, u16 size);
#endif

#ifdef RTKBT_SWITCH_PATCH
#define RTLBT_CLOSE	(1 << 0)
struct api_context {
	u32			flags;
	struct completion	done;
	int			status;
};

int download_lps_patch(struct usb_interface *intf);
#endif

#if defined RTKBT_SUSPEND_WAKEUP || defined RTKBT_SHUTDOWN_WAKEUP || defined RTKBT_SWITCH_PATCH
int set_scan(struct usb_interface *intf);
#endif


#endif /* __RTK_MISC_H__ */