/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <mmc.h>
#include <common.h>
#include "auto_update.h"
static int mmc_stor_init(void)
{
	struct mmc *mmc;
	int dev_num;
#ifdef CONFIG_EMMC
	int *target_dev = get_target_dev_value();
	dev_num = *target_dev;
#else
	dev_num = 0;
#endif
	mmc = find_mmc_device(dev_num);
	if (mmc == NULL) {
		printf("No mmc driver found!\n");
		return -1;
	}

	if (((unsigned long)mmc->block_dev.vendor[0] == 0) ||
			((unsigned long)mmc->block_dev.product[0] == 0)) {
		printf("*No SD card found!\n");
		return -1;
	}
	return 0;
}

static void mmc_stor_exit(void)
{
}
