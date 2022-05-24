/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <common.h>
#include <usb.h>
#include <linux/errno.h>

static int usb_stor_init(void)
{
	int ret = -1;
#ifndef CONFIG_GK_MC
try_again:
	if (usb_stop() < 0) {
		debug("usb_stop failed\n");
		return ret;
	}
	/* delay for 1000 ms */
	mdelay(1000);
	ret = usb_init();
	if (ret == -ESRCH)
		goto try_again;

	if (ret < 0) {
		debug("usb_init failed!\n");
		return ret;
	}

	/*
	 * check whether a storage device is attached (assume that it's
	 * a USB memory stick, since nothing else should be attached).
	 */
	ret = usb_stor_scan(0);
	if (ret == -1)
		debug("No USB device found. Not initialized!\n");
#endif
	return ret;
}

static void usb_stor_exit(void)
{
#ifndef CONFIG_GK_MC
	usb_stop();
#endif
}

