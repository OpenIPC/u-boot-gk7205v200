/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __BSPETH_SYS_H__
#define __BSPETH_SYS_H__

void bspeth_sys_init(void);
void bspeth_sys_exit(void);

void bspeth_set_crg_phy_mode(unsigned char is_rmii_mode);
void set_inner_phy_addr(u32 phyaddr);
void set_efuse_unread(void);

void bspeth_sys_startup(void);
void bspeth_sys_allstop(void);

void set_phy_valtage(void);
#endif
