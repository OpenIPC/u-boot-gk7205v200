/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __BSPETH_MDIO_H__
#define __BSPETH_MDIO_H__

#include "bspeth.h"

#ifdef BSPETH_FEMAC

#define MDIO_RWCTRL      0x1100
#define MDIO_RO_DATA     0x1104
#define U_MDIO_PHYADDR   0x0108
#define D_MDIO_PHYADDR   0x2108
#define U_MDIO_RO_STAT   0x010C
#define D_MDIO_RO_STAT   0x210C
#define U_MDIO_ANEG_CTRL 0x0110
#define D_MDIO_ANEG_CTRL 0x2110
#define U_MDIO_IRQENA    0x0114
#define D_MDIO_IRQENA    0x2114

#define mdio_mk_rwctl(cpu_data_in, finish, rw, phy_exaddr, frq_div, phy_regnum) \
	(((cpu_data_in) << 16) | (((finish) & 0x01) << 15) | (((rw) & 0x01) << 13) | \
	(((phy_exaddr) & 0x1F) << 8) | (((frq_div) & 0x7) << 5) | ((phy_regnum) & 0x1F))

/* hardware set bit'15 of MDIO_REG(0) if mdio ready */
#define test_mdio_ready(ld) (bspeth_readl(ld, MDIO_RWCTRL) & bit(15))

#define mdio_start_phyread(ld, phy_addr, regnum) \
	bspeth_writel(ld, mdio_mk_rwctl(0, 0, 0, phy_addr, (ld)->mdio_frqdiv, regnum), MDIO_RWCTRL)

#define mdio_get_phyread_val(ld) (bspeth_readl(ld, MDIO_RO_DATA) & 0xFFFF)

#define mdio_phywrite(ld, phy_addr, regnum, val) \
	bspeth_writel(ld, mdio_mk_rwctl(val, 0, 1, phy_addr, (ld)->mdio_frqdiv, regnum), MDIO_RWCTRL)

#endif

/* APIs */
int bspeth_mdio_read(struct bspeth_netdev_local *ld, int phy_addr, unsigned int regnum);
int bspeth_mdio_write(struct bspeth_netdev_local *ld, int phy_addr, int regnum, int val);
int bspeth_mdio_reset(struct bspeth_netdev_local *ld);
int bspeth_mdio_init(struct bspeth_netdev_local *ld);
void bspeth_mdio_exit(struct bspeth_netdev_local *ld);
#endif
