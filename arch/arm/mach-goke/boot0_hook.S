/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <linux/linkage.h>

VENDOR_BOOT0_HOOK

    .globl _blank_zone_start
_blank_zone_start:  .word __blank_zone_start
ENTRY(get_blank_start)
	ldr r0, _blank_zone_start
	ldr r1, =__image_copy_start
	sub r0, r0, r1
	adrl r1, _start
	add r0, r0, r1
	mov pc, lr
ENDPROC(get_blank_start)

ENTRY(get_code_start)
	adrl r0, _start
	mov pc, lr
ENDPROC(get_code_start)
