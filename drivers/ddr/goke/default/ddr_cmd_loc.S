/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

.section .image,#alloc

.globl ddr_training_cmd_start
ddr_training_cmd_start:
.incbin "drivers/ddr/goke/default/cmd_bin/ddr_cmd.bin"
.globl ddr_training_cmd_end
ddr_training_cmd_end:
