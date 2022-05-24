/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "bspeth.h"
#include "mdio.h"
#include "mii_drv.h"
#include <config.h>

/* MDIO Bus Interface */
static int bspeth_mdiobus_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)bus->priv;

	return bspeth_mdio_read(ld, addr, reg);
}

static int bspeth_mdiobus_write(struct mii_dev *bus, int addr, int devad,
		  int reg, u16 value)
{
	struct bspeth_netdev_local *ld = (struct bspeth_netdev_local *)bus->priv;

	bspeth_mdio_write(ld, addr, reg, value);

	return 0;
}

#define PHY_ID_KSZ8051 0x00221550
#define PHY_ID_KSZ8081 0x00221560
#define PHY_ID_MASK    0xFFFFFFF0

static bool get_fephy_id(const char *devname, unsigned char phyaddr, u32 *phy_id)
{
	u16 id1 = 0;
	u16 id2 = 0;

	if (miiphy_read(devname, phyaddr, MII_PHYSID1, &id1)) {
		printf("%s,%d:PHY_PHYIDR1 read failed!\n", __func__, __LINE__);
		return false;
	}
	if (miiphy_read(devname, phyaddr, MII_PHYSID2, &id2)) {
		printf("%s,%d:PHY_PHYIDR2 read failed!\n", __func__, __LINE__);
		return false;
	}

	*phy_id = (id1 & 0xffff) << 16; /* high 16 bit */
	*phy_id |= (id2 & 0xffff);

	/* If the phy_id is all Fs, there is no device there */
	if (*phy_id == 0xffffffff || *phy_id == 0 || *phy_id == 0xFFFF || *phy_id == 0xFFFF0000) {
		return false;
	}

	return true;
}

bool phy_detected(const char *devname, unsigned char phyaddr)
{
	u32 phy_id = 0;

	if (!get_fephy_id(devname, phyaddr, &phy_id)) return false;

	/* run this at RMII mode */
	if (BSPETH_MII_RMII_MODE_U == 1) {
		/* PHY-KSZ8051RNL */
		if ((phy_id & PHY_ID_MASK) == PHY_ID_KSZ8051) {
			unsigned short reg = 0;

			if (miiphy_read(devname, phyaddr, 0x1F, &reg)) {
				printf("PHY 0x1F read failed\n");
				return false;
			}
			reg |= bit(7); /* bit7:set phy RMII 50MHz clk; */
			if (miiphy_write(devname, phyaddr, 0x1F, reg)) {
				printf("PHY 0x1F write failed\n");
				return false;
			}

			if (miiphy_read(devname, phyaddr, 0x16, &reg)) {
				printf("PHY 0x16 read failed\n");
				return false;
			}
			reg |= bit(1); /* set phy RMII override; */
			if (miiphy_write(devname, phyaddr, 0x16, reg)) {
				printf("PHY 0x16 write failed\n");
				return false;
			}
		}

		/* PHY-KSZ8081 */
		if ((phy_id & PHY_ID_MASK) == PHY_ID_KSZ8081) {
			unsigned short val = 0;

			if (miiphy_read(devname, phyaddr, 0x1F, &val) != 0) {
				printf("PHY 0x1F read failed\n");
				return false;
			};
			val |= bit(7); /* bit7:set phy RMII 50MHz clk; */
			if (miiphy_write(devname, phyaddr, 0x1F, val) != 0) {
				printf("PHY 0x1F write failed\n");
				return false;
			}
		}
	}
	return true;
}

static int g_mdio_registered;

int bspeth_mdiobus_driver_init(struct bspeth_netdev_local *ld)
{
	memset(ld->mii_name, 0, MAX_PHY_NAME_LEN);
	snprintf(ld->mii_name, MAX_PHY_NAME_LEN, "mii_bspeth");
	ld->mdio_frqdiv = ETH_MDIO_FRQDIV;
#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	if (!g_mdio_registered) {
		struct mii_dev *bus = mdio_alloc();

		if (!bus) {
			printf("Failed to allocate MDIO bus\n");
			return -ENOMEM;
		}

		bus->priv = ld;
		bus->read = bspeth_mdiobus_read;
		bus->write = bspeth_mdiobus_write;
		snprintf(bus->name, sizeof(bus->name), ld->mii_name);

		bspeth_mdio_init(ld);

		if (mdio_register(bus)) {
			mdio_free(bus);
			return -1;
		}

		miiphy_set_current_dev(ld->mii_name);
		g_mdio_registered = 1;
	}
#endif
	return 0;
}

void bspeth_mdiobus_driver_exit(struct bspeth_netdev_local *ld)
{
	/* add this to avoid the first time to use eth will print 'No such device: XXXXX' message. */
	if (!miiphy_get_current_dev()) {
		return;
	}

	bspeth_mdio_exit(ld);
}
