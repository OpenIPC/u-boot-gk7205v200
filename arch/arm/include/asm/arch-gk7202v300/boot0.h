/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __BOOT0_H
#define __BOOT0_H

#define ARM_SOC_BOOT0_HOOK

/* BOOT0 header information */
#define VENDOR_BOOT0_HOOK   \
	__blank_zone_start : \
	.fill 1024*8,1,0;   \
	__blank_zone_end :

#include <../mach-goke/boot0_hook.S>
#endif /* __BOOT0_H */
