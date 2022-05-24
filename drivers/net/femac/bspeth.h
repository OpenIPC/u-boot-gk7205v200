/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __BSPETH_H__
#define __BSPETH_H__

#include <config.h>
#include <common.h>
#include <command.h>

#include <linux/list.h>
#include <asm/io.h>

#include <malloc.h> /* malloc, free, realloc */

#include <net.h>
#include <miiphy.h>
#include <asm/arch/platform.h>

#define OSDRV_MODULE_VERSION_STRING "Vendor ETH net controler"

#ifndef bit
#define bit(nr) (1UL << (nr))
#endif
#define mdelay(n) udelay((n)*1000)
/* ***********************************************************
 *
 * Global varibles and defintions
 *
 * ***********************************************************
 */

/* configuerable values */

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
#define BSPETH_HW_DESC_DEPTH 72
#endif

#define ETH_MDIO_FRQDIV 2

/* mdiobus device name, such as platform device name */
#define BSPETH_MDIOBUS_NAME "bspeth_mdiobus"

/* eth device name, such as platform device name */
#define BSPETH_FEMAC_NAME     "gk_femac"
#define MAX_PHY_NAME_LEN      16
#define BSPETH_MAX_QUEUE_DEPTH 64
#ifdef BSPETH_RX_QUEUE_MULTI_DESC
#define BSPETH_HW_RXQ_DEPTH 52 /* uboot */
#else
#define BSPETH_HW_RXQ_DEPTH 1 /* uboot */
#endif
#define BSPETH_HW_TXQ_DEPTH 1 /* uboot */

#define BSPETH_MAX_FRAME_SIZE PKTSIZE_ALIGN /* 1536 */

#define BSPETH_TRACE_ETH		2
#define BSPETH_TRACE_MDIO	4
#define BSPETH_TRACE_DRV		7
#define BSPETH_TRACE_LEVEL	8

/* Error number */
#define BSPETH_E_QUEUE (-1)
#define BSPETH_E_BUSY  (-2)
#define BSPETH_E_FULL  (-3)
#define BSPETH_E_EMPTY (-4)

struct bspeth_frame_desc {
	unsigned long frm_addr; /* required by the controler */
	unsigned int frm_len : 11; /* required by the controler */
};

#define bspeth_trace_fd(level, fd) bspeth_trace(level, \
	#fd "<%p>={ .frm_addr=%08lx, .frm_len=%d}",      \
	&(fd), (fd).frm_addr, (fd).frm_len)

/* port */
#define UP_PORT		0
#define DOWN_PORT	1
#define MAX_PORT	2

enum if_mode {
	INTERFACE_MODE_MII,
	INTERFACE_MODE_RMII
};

struct bspeth_netdev_local {
	unsigned long iobase_phys; /* physical io addr */
	int port : 1; /* 0 => up port,    1 => down port */

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
	int desc_hw_offset; /* the offset where we feed hw */
	int desc_rec_offset; /* the offset where we receve the package */
	struct bspeth_frame_desc *bspeth_desc_head;
#endif

	u32 link_stat;
	char *mii_name;
	unsigned char phy_addr;
	enum if_mode phy_intf;

	/* mdio_bus freq-div, 1 for 1/100, 0 for 1/50 */
	u32 mdio_frqdiv;
};

/* ***********************************************************
 *
 * Only for internal used!
 *
 * ***********************************************************
 */

/* read/write IO */

#define _readl(c)     ({ u32 __v = le32_to_cpu(__raw_readl(c)); __v; })
#define _writel(v, c) __raw_writel(cpu_to_le32(v), c)

#define mk_bits(shift, nbits) ((((shift) & 0x1F) << 16) | ((nbits) & 0x1F))

u32 bspeth_readl(struct bspeth_netdev_local *ld, u32 ofs);
void bspeth_writel(struct bspeth_netdev_local *ld, u32 v, u32 ofs);
u32 bspeth_readl_bits(struct bspeth_netdev_local *ld, u32 ofs, u32 bits_desc);
void bspeth_writel_bits(struct bspeth_netdev_local *ld, u32 v, u32 ofs, u32 bits_desc);

void bspeth_trace(int level, const char *fmt, ...);
void bspeth_error(const char *fmt, ...);
void bspeth_assert(bool cond);

#define local_lock_init(ld)
#define local_lock_exit(ld)
#define local_lock(ld)
#define local_unlock(ld)

#define ud_reg_name(name) ((ld->port == UP_PORT) ? U_##name : D_##name)
#define ud_bit_name(name) ((ld->port == UP_PORT) ? name##_U : name##_D)

#endif
