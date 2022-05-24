/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#define BSPETH_FEMAC
#include "bspeth.h"
#include "mac.h"
#include "ctrl.h"
#include "glb.h"
#include "sys.h"
#include "mii_drv.h"
#include <config.h>
#include <miiphy.h>
#include <net.h>
/*************************************************************************/
int bspeth_mdiobus_driver_init(struct bspeth_netdev_local *ld);
void bspeth_mdiobus_driver_exit(struct bspeth_netdev_local *ld);

/* Used when trying to connect to a specific phy (mii bus id:phy device id) */
/* refer linux-2.6.23 ./include/linux/phy.h line 76 */
char mdio_bus_name[MAX_PHY_NAME_LEN];

#define MAC_LEN 6

struct bspeth_netdev_local bspeth_devs_priv[MAX_PORT] = {
	{
		.port = UP_PORT,
		.mii_name = mdio_bus_name,
		.phy_addr = SFV_PHY_U,
		.phy_intf = BSPETH_MII_RMII_MODE_U,
	},
	{
		.port = DOWN_PORT,
		.mii_name = mdio_bus_name,
		.phy_addr = SFV_PHY_D,
		.phy_intf = BSPETH_MII_RMII_MODE_D,
	},
};

static int set_mac_address(const char *mac)
{
	u32 val;

	val = ((u32)mac[0] << 8) | (u32)mac[1]; /* mac 0 [8:15], mac 1 [0:7] */
	writel(val, REG_BASE_SF + GLB_HOSTMAC_H16);
	/* mac 2 [24:31], mac 3 [16:23], mac 4 [8:15], mac 5 [0:7] */
	val = ((u32)mac[2] << 24) | ((u32)mac[3] << 16) | ((u32)mac[4] << 8) | (u32)mac[5];
	writel(val, REG_BASE_SF + GLB_HOSTMAC_L32);

	return 0;
}

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
static int bspeth_init_hw_desc_queue(struct bspeth_netdev_local *ld)
{
	struct bspeth_frame_desc *queue_phy_addr = NULL;
	int i;

	const int size = BSPETH_HW_DESC_DEPTH * sizeof(struct bspeth_frame_desc);
	/* init rx fq */
	queue_phy_addr = (struct bspeth_frame_desc *)malloc(size);
	if (queue_phy_addr == NULL) {
		printf("alloc rx fq error!\n");
		return 1;
	}

	memset((void *)queue_phy_addr, 0, size);
	ld->bspeth_desc_head = queue_phy_addr;
	ld->desc_hw_offset = 0;
	ld->desc_rec_offset = 0;

	for (i = 0; i < BSPETH_HW_DESC_DEPTH; i++) {
		queue_phy_addr[i].frm_addr = (unsigned long)memalign(PKTALIGN, PKTSIZE_ALIGN);
		queue_phy_addr[i].frm_len = 0;
	}
	return 0;
}
#endif

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
static int bspeth_destroy_hw_desc_queue(struct bspeth_netdev_local *ld)
{
	struct bspeth_frame_desc *queue_phy_addr = ld->bspeth_desc_head;
	int i;

	if (queue_phy_addr != NULL) {
		for (i = 0; i < BSPETH_HW_DESC_DEPTH; i++)
			free((void *)queue_phy_addr[i].frm_addr);

		free(ld->bspeth_desc_head);
		ld->bspeth_desc_head = NULL;
	}
	ld->desc_hw_offset = 0;
	ld->desc_rec_offset = 0;

	return 0;
}
#endif

int eth_set_host_mac_address(struct eth_device *dev)
{
	unsigned char mac[MAC_LEN];

	memset(mac, 0, sizeof(mac));

	if (!eth_getenv_enetaddr("ethaddr", mac)) {
		printf("MAC address invalid!\n");
#ifdef CONFIG_NET_RANDOM_ETHADDR
		net_random_ethaddr(mac);
		printf("Set Random MAC address!\n");
		eth_setenv_enetaddr("ethaddr", mac);
#endif
	}

	set_mac_address((char *)mac);
	memcpy(dev->enetaddr, mac, MAC_LEN);

	return 0;
}

static void phy_print_status(struct bspeth_netdev_local *ld, unsigned int stat)
{
	printf("%s : phy status change : LINK=%s : DUPLEX=%s : SPEED=%s\n",
		   (ld->port == UP_PORT) ? "eth0" : "eth1",
		   (stat & BSPETH_LINKED) ? "UP" : "DOWN",
		   (stat & BSPETH_DUP_FULL) ? "FULL" : "HALF",
		   (stat & BSPETH_SPD_100M) ? "100M" : "10M");
}

static void bspeth_adjust_link(struct bspeth_netdev_local *ld)
{
	u32 stat = 0;
	int timeout_us = 1000;
	/* this env phy_link_time used to solve the difference phy auto-negotiation time of  various phys */
	char *timeout = getenv("phy_link_time");
	if (timeout != NULL) {
		timeout_us = simple_strtol(timeout, 0, 10); /* Base10 */
		if (timeout_us < 0)
			timeout_us = 1000; /* delay 1000us */
	}
retry:
	udelay(1);

	stat |= miiphy_link(ld->mii_name, ld->phy_addr) ? BSPETH_LINKED : 0;
	stat |= miiphy_duplex(ld->mii_name, ld->phy_addr) == FULL ? BSPETH_DUP_FULL : 0;
	stat |= miiphy_speed(ld->mii_name, ld->phy_addr) == _100BASET ? BSPETH_SPD_100M : 0;
	if (--timeout_us && !(stat & BSPETH_LINKED))
		goto retry;

	if (stat != ld->link_stat) {
		bspeth_set_linkstat(ld, stat);
		phy_print_status(ld, stat);
		ld->link_stat = stat;
		bspeth_set_mii_mode(ld, ld->phy_intf);
	}

	set_phy_valtage();
}

static int bspeth_net_open(struct bspeth_netdev_local *ld)
{
	/* enable sys-ctrl-en and clk-en  */
	bspeth_sys_startup();

	/* setup hardware tx dep */
	bspeth_writel_bits(ld, BSPETH_HW_TXQ_DEPTH, ud_reg_name(GLB_QLEN_SET), BITS_TXQ_DEP);

	/* setup hardware rx dep */
	bspeth_writel_bits(ld, BSPETH_HW_RXQ_DEPTH, ud_reg_name(GLB_QLEN_SET), BITS_RXQ_DEP);

	ld->link_stat = 0;
	bspeth_adjust_link(ld);

	bspeth_irq_enable(ld, ud_bit_name(BSPETH_INT_RX_RDY));

	return 0;
}

static int bspeth_net_close(struct bspeth_netdev_local *ld)
{
	bspeth_glb_preinit_dummy(ld);

	bspeth_sys_allstop();

	return 0;
}

static int bspeth_dev_probe_init(struct bspeth_netdev_local *ld)
{
	ld->iobase_phys = REG_BASE_SF;

	bspeth_glb_preinit_dummy(ld);

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
	bspeth_init_hw_desc_queue(ld);
#endif

	bspeth_sys_allstop();

	return 0;
}

static int bspeth_dev_remove(struct bspeth_netdev_local *ld)
{
#ifdef BSPETH_RX_QUEUE_MULTI_DESC
	bspeth_destroy_hw_desc_queue(ld);
#endif

	return 0;
}

static void bspeth_get_phy_intf(struct bspeth_netdev_local *ld)
{
	char *mdio_intf = NULL;

	/* get mdio interface from env.FORMAT: mdio_intf=mii or mdio_intf=rmii */
	mdio_intf = getenv("mdio_intf");
	if (mdio_intf != NULL) {
		if (!strncmp(mdio_intf, "mii", strlen("mii"))) {
			ld->phy_intf = INTERFACE_MODE_MII;
		} else if (!strncmp(mdio_intf, "rmii", strlen("rmii"))) {
			ld->phy_intf = INTERFACE_MODE_RMII;
		} else {
			printf("Invalid mdio_intf, should be mii or rmii.\n");
			ld->phy_intf = INTERFACE_MODE_RMII;
		}
	}
}

static void bspeth_get_phy_addr(struct bspeth_netdev_local *ld)
{
	char *phyaddr = NULL;
	char addr_name[MAX_PORT][10] = { "phyaddru", "phyaddrd" }; /* max name size is 10 */

	if (ld->port >= MAX_PORT)
		return;

	/* get phy addr of up port */
	phyaddr = getenv(addr_name[ld->port]);
	if (phyaddr != NULL) {
		unsigned long tmp = simple_strtoul(phyaddr, 0, 10); /* use Base10 */
		/* check phyaddr > 0x1f */
		if (tmp >= 0x1f) {
			printf("Detected env '%s' had been set greater"
				   "than 0x1f,this may not correct.\n",
				   addr_name[ld->port]);
			return;
		}
		ld->phy_addr = (unsigned char)tmp;
	} else {
#if defined(INNER_PHY)
		if (ld->port == UP_PORT) {
#if defined(INNER_PHY_ADDR_U)
			ld->phy_addr = INNER_PHY_ADDR_U;
#endif
		} else {
#if defined(INNER_PHY_ADDR_D)
			ld->phy_addr = INNER_PHY_ADDR_D;
#endif
		}
#endif
	}
}

static void bspeth_exit(struct bspeth_netdev_local *ld)
{
	bspeth_mdiobus_driver_exit(ld);

	bspeth_sys_exit();
}

int bspeth_init(struct eth_device *dev, bd_t *bd)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)dev->priv;
	int ret;
	int count = 30;

	bspeth_get_phy_intf(ld);

	bspeth_get_phy_addr(ld);
#ifdef INNER_PHY
	if (ld->port == UP_PORT)
		set_inner_phy_addr(ld->phy_addr);
#endif

	printf(OSDRV_MODULE_VERSION_STRING "\n");

	set_efuse_unread();

	/* sys-func-sel */
	bspeth_sys_init();

	/* register MDIO bus to uboot */
	if (bspeth_mdiobus_driver_init(ld) != 0) {
		goto _error_bspeth_init;
	}

	if (phy_detected(ld->mii_name, ld->phy_addr) == false) {
		goto _error_bspeth_init;
	}

	miiphy_reset(ld->mii_name, ld->phy_addr);

	ret = bspeth_dev_probe_init(ld);
	if (ret) {
		bspeth_error("register Ether netdevice"
					" driver failed!");
		goto _error_bspeth_init;
	}

	eth_set_host_mac_address(dev);

	while (--count >= 0) {
		/* open UpEther net dev */
		bspeth_net_open(ld);
		if (ld->link_stat & BSPETH_LINKED) {
			return 0;
		}
	}

	printf("PHY not link.\n");

_error_bspeth_init:
	bspeth_mdiobus_driver_exit(ld);
	bspeth_net_close(ld);

	return -1;
}

#ifdef BSPETH_RX_QUEUE_MULTI_DESC
static int bspeth_recv(struct eth_device *dev)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)dev->priv;
	int recvq_ready, hw_offset, rec_offset;
	int timeout_us = 10000;
	struct bspeth_frame_desc *fd;
	struct bspeth_frame_desc receive_fd;

	fd = ld->bspeth_desc_head;
	hw_offset = ld->desc_hw_offset;
	rec_offset = ld->desc_rec_offset;

	/* check this we can add a Rx addr */
	recvq_ready = bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT), BITS_RECVQ_RDY);
	if (!recvq_ready)
		bspeth_trace(BSPETH_TRACE_DRV, "hw can't add a rx addr.");

	while (recvq_ready &&
		   ((hw_offset + 1) % BSPETH_HW_DESC_DEPTH != rec_offset)) {
		receive_fd = fd[hw_offset];
		invalidate_dcache_range(receive_fd.frm_addr,
								ALIGN(receive_fd.frm_addr + PKTSIZE_ALIGN, ARCH_DMA_MINALIGN));
		hw_recvq_setfd(ld, receive_fd);

		hw_offset = (hw_offset + 1) % BSPETH_HW_DESC_DEPTH;

		recvq_ready =
			bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT),
							 BITS_RECVQ_RDY);
	}
	ld->desc_hw_offset = hw_offset;

	/* receive packed, loop in NetLoop */
	while (--timeout_us && !is_recv_packet_rx(ld))
		udelay(1);

	if (is_recv_packet_rx(ld)) {
		receive_fd = fd[rec_offset];

		receive_fd.frm_len = hw_get_rxpkg_len(ld);
		hw_set_rxpkg_finish(ld);

		rec_offset = (rec_offset + 1) % BSPETH_HW_DESC_DEPTH;
		ld->desc_rec_offset = rec_offset;

		if (bspeth_invalid_rxpkg_len(receive_fd.frm_len)) {
			bspeth_error("frm_len invalid (%u)", receive_fd.frm_len);
			goto _error_exit;
		}

		invalidate_dcache_range(receive_fd.frm_addr,
								ALIGN(receive_fd.frm_addr + receive_fd.frm_len, ARCH_DMA_MINALIGN));
		memcpy((void *)net_rx_packets[0], (void *)receive_fd.frm_addr,
			   receive_fd.frm_len);

		/* Pass the packet up to the protocol layers. */
		net_process_received_packet(net_rx_packets[0], receive_fd.frm_len);

		return 0;
	} else {
		bspeth_trace(BSPETH_TRACE_DRV, "hw rx timeout.");
	}

_error_exit:
	return -1;
}
#else
static int bspeth_recv(struct eth_device *dev)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)dev->priv;
	int recvq_ready;
	int timeout_us = 10000;
	struct bspeth_frame_desc fd = { 0 };

	/* check this we can add a Rx addr */
	recvq_ready = bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT), BITS_RECVQ_RDY);
	if (!recvq_ready)
		bspeth_trace(BSPETH_TRACE_DRV, "hw can't add a rx addr.");

	/* enable rx int */
	bspeth_irq_enable(ld, ud_bit_name(BSPETH_INT_RX_RDY));

	/* fill rx hwq fd */
	fd.frm_addr = (uintptr_t)net_rx_packets[0];
	fd.frm_len = 0;
	/* recv data will be put into DDR , in case of get the cache data, mush be invalid Dcache */
	invalidate_dcache_range(fd.frm_addr, ALIGN(fd.frm_addr + PKTSIZE_ALIGN, ARCH_DMA_MINALIGN));

	hw_recvq_setfd(ld, fd);

	/* receive packed, loop in NetLoop */
	while (--timeout_us && !is_recv_packet(ld))
		udelay(1);

	if (is_recv_packet(ld)) {
		fd.frm_len = hw_get_rxpkg_len(ld);
		hw_set_rxpkg_finish(ld);

		if (bspeth_invalid_rxpkg_len(fd.frm_len)) {
			bspeth_error("frm_len invalid (%u)", fd.frm_len);
			goto _error_exit;
		}

		invalidate_dcache_range(fd.frm_addr,
								ALIGN(fd.frm_addr + fd.frm_len, ARCH_DMA_MINALIGN));
		/* Pass the packet up to the protocol layers. */
		net_process_received_packet(net_rx_packets[0], fd.frm_len);

		return 0;
	} else {
		bspeth_trace(BSPETH_TRACE_DRV, "hw rx timeout.");
	}

_error_exit:
	return -1;
}
#endif

#define ETH_FCS_LEN 4 /* Octets in the FCS */
static int bspeth_send(struct eth_device *dev, void *packet, int length)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)dev->priv;
	unsigned int ints, xmitq_ready;
	unsigned int timeout_us = 3000;
	struct bspeth_frame_desc fd;

	/* check this we can add a Tx addr */
	xmitq_ready = bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT), BITS_XMITQ_RDY);
	if (!xmitq_ready) {
		bspeth_error("hw can't add a tx addr");
		goto _error_exit;
	}

	/* enable tx int */
	bspeth_irq_enable(ld, ud_bit_name(BSPETH_INT_TXQUE_RDY));

	flush_cache((uintptr_t)packet, ALIGN((unsigned int)length + ETH_FCS_LEN, ARCH_DMA_MINALIGN));
	/* fill tx hwq fd */
	fd.frm_addr = (uintptr_t)packet;
	fd.frm_len = length + ETH_FCS_LEN;
	hw_xmitq_setfd(ld, fd);

	do {
		udelay(1);
		ints = bspeth_read_irqstatus(ld);
	} while (--timeout_us && !(ints & ud_bit_name(BSPETH_INT_TXQUE_RDY)));

	bspeth_clear_irqstatus(ld, ints);

	if (!timeout_us) {
		bspeth_error("hw tx timeout");
		goto _error_exit;
	}

	return 0;

_error_exit:
	return -1;
}

static void bspeth_halt(struct eth_device *dev)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)dev->priv;

	bspeth_net_close(ld);

	bspeth_dev_remove(ld);

	bspeth_exit(ld);
}

static int bspeth_register_dev(unsigned char port_id)
{
	struct eth_device *dev;

	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return -1;
	memset(dev, 0, sizeof(*dev));

	dev->iobase = REG_BASE_SF;
	dev->init = bspeth_init;
	dev->halt = bspeth_halt;
	dev->send = bspeth_send;
	dev->recv = bspeth_recv;
	dev->priv = &bspeth_devs_priv[port_id];
	bspeth_devs_priv[port_id].iobase_phys = REG_BASE_SF;
	snprintf(dev->name, sizeof(dev->name) - 1, "eth%d", port_id);

	eth_register(dev);

	return 0;
}

int bspeth_initialize(bd_t *bis)
{
	int ret;

	ret = bspeth_register_dev(UP_PORT);
	if (ret)
		return ret;

	return 0;
}
