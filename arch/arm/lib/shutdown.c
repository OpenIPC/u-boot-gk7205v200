/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <common.h>

#define CFG_MAX_SHUTDOWN       10

static struct shutdown_ctrl {
	int count;
	void (*shutdown[CFG_MAX_SHUTDOWN])(void);
} shutdown_ctrl = {0, {0}, };

void add_shutdown(void (*shutdown)(void))
{
	if (shutdown_ctrl.count >= CFG_MAX_SHUTDOWN) {
		printf("Can't add shutdown function,"
			   "Please increase CFG_MAX_SHUTDOWN count\n");
		return;
	}
	shutdown_ctrl.shutdown[shutdown_ctrl.count++]
		= shutdown;
}

void do_shutdown(void)
{
	int ix;
	for (ix = 0; ix < shutdown_ctrl.count; ix++)
		shutdown_ctrl.shutdown[ix]();
}
