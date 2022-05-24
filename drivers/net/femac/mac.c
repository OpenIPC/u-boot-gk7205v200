/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#define BSPETH_FEMAC

#include "bspeth.h"
#include "mac.h"

static u32 _set_linkstat(struct bspeth_netdev_local *ld, u32 mode)
{
	u32 old;

	old = bspeth_readl_bits(ld, ud_reg_name(MAC_PORTSET), BITS_MACSTAT);
	bspeth_writel_bits(ld, mode, ud_reg_name(MAC_PORTSET), BITS_MACSTAT);

	return old;
}

static u32 _set_negmode(struct bspeth_netdev_local *ld, u32 mode)
{
	u32 old;

	old = bspeth_readl_bits(ld, ud_reg_name(MAC_PORTSEL), BITS_NEGMODE);
	bspeth_writel_bits(ld, mode, ud_reg_name(MAC_PORTSEL), BITS_NEGMODE);

	return old;
}

static u32 _get_negmode(struct bspeth_netdev_local *ld)
{
	u32 val;

	val = bspeth_readl_bits(ld, ud_reg_name(MAC_PORTSEL), BITS_NEGMODE);
	return val;
}

void bspeth_set_linkstat(struct bspeth_netdev_local *ld, u32 mode)
{
	local_lock(ld);
	(void)_set_linkstat(ld, mode);
	local_unlock(ld);
}

u32 bspeth_get_linkstat(struct bspeth_netdev_local *ld)
{
	u32 val;

	local_lock(ld);
	val = bspeth_readl_bits(ld, ud_reg_name(MAC_RO_STAT), BITS_MACSTAT);
	local_unlock(ld);
	return val;
}

void bspeth_set_mac_leadcode_cnt_limit(struct bspeth_netdev_local *ld, u32 cnt)
{
	local_lock(ld);
	(void)bspeth_readl_bits(ld, ud_reg_name(MAC_TX_IPGCTRL),
						   BITS_PRE_CNT_LIMIT);
	bspeth_writel_bits(ld, cnt, ud_reg_name(MAC_TX_IPGCTRL),
					  BITS_PRE_CNT_LIMIT);
	local_unlock(ld);
}

void bspeth_set_mac_trans_interval_bits(struct bspeth_netdev_local *ld, u32 nbits)
{
	u32 linkstat, negmode;

	local_lock(ld);

	negmode = _set_negmode(ld, BSPETH_NEGMODE_CPUSET);
	linkstat = _set_linkstat(ld, 0);
	mdelay(1);

	(void)bspeth_readl_bits(ld, ud_reg_name(MAC_TX_IPGCTRL), BITS_IPG);
	bspeth_writel_bits(ld, nbits, ud_reg_name(MAC_TX_IPGCTRL), BITS_IPG);
	udelay(100); /* delay 100us */

	_set_negmode(ld, negmode);
	_set_linkstat(ld, linkstat);

	local_unlock(ld);
}

void bspeth_set_mac_fc_interval(struct bspeth_netdev_local *ld, u32 para)
{
	local_lock(ld);
	(void)bspeth_readl_bits(ld, ud_reg_name(MAC_TX_IPGCTRL), BITS_FC_INTER);
	bspeth_writel_bits(ld, para, ud_reg_name(MAC_TX_IPGCTRL), BITS_FC_INTER);
	local_unlock(ld);
}

void bspeth_set_negmode(struct bspeth_netdev_local *ld, u32 mode)
{
	local_lock(ld);
	_set_negmode(ld, mode);
	local_unlock(ld);
}

u32 bspeth_get_negmode(struct bspeth_netdev_local *ld)
{
	u32 val;

	local_lock(ld);
	val = _get_negmode(ld);
	local_unlock(ld);
	return val;
}

void bspeth_set_mii_mode(struct bspeth_netdev_local *ld, u32 mode)
{
	local_lock(ld);
	(void)bspeth_readl_bits(ld, ud_reg_name(MAC_PORTSEL), BITS_MII_MODE);
	bspeth_writel_bits(ld, mode, ud_reg_name(MAC_PORTSEL), BITS_MII_MODE);
	local_unlock(ld);
}
