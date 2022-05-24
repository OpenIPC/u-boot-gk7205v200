/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include <config.h>
#include "bspeth.h"
#include "sys.h"

#define BSPETH_CRG_REG (CRG_REG_BASE + REG_ETH_CRG)


#define ETH_SOFT_RESET bit(0)
#define ETH_CLK_ENABLE bit(1)


#if defined(SFV_RESET_PHY_BY_CRG)
#define ETH_EXTERNAL_PHY_RESET bit(3)
#endif

#define ETH_CORE_CLK_SELECT_54M bit(7)

#if ((defined CONFIG_TARGET_GK7205V200) || (defined CONFIG_TARGET_GK7205V300) || \
	(defined CONFIG_TARGET_GK7202V300) || (defined CONFIG_TARGET_GK7605V100))

#define BSPETH_SYSCTL_REG        (SYS_CTRL_REG_BASE + MISC_CTRL9)
#define ETH_INTERNAL_PHY_RESET  bit(3)
#define ETH_INTERNAL_PHY_CLK_EN bit(2)
#endif

#define BSPETH_CRG_INPHY_CLK_REG   BSPETH_CRG_REG
#define BSPETH_CRG_EXTPHY_CLK_REG  BSPETH_CRG_REG


#if 0
void set_efuse_unread(void)
{
	u32 reg_value;

	reg_value = readl(BSPETH_EFUSE_REG);
	reg_value |= 0x1;
	writel(reg_value, BSPETH_EFUSE_REG);

	reg_value = readl(BSPETH_EFUSE_DATA_REG);
	reg_value |= 0x1;
	writel(reg_value, BSPETH_EFUSE_DATA_REG);

	mdelay(300); /* delay 300ms */

	reg_value = readl(BSPETH_EFUSE_REG);
	reg_value &= ~0x1;
	writel(reg_value, BSPETH_EFUSE_REG);
}
#else
void set_efuse_unread(void)
{
}
#endif

#ifdef INNER_PHY
void set_inner_phy_addr(u32 phy_addr)
{
	u32 reg_value;

	reg_value = readl(BSPETH_SYSCTL_REG);
	reg_value &= ~0x1f;
	phy_addr &= 0x1f;
	reg_value |= phy_addr;
	writel(reg_value, BSPETH_SYSCTL_REG);
}
#else
void set_inner_phy_addr(u32 phyaddr)
{
}
#endif

#if 0
void revise_led_shine(void)
{
#define MDIO_RWCTRL      0x1100
#define tmp_mdio_ready() \
	(readl(REG_BASE_SF + MDIO_RWCTRL) & bit(15))

	unsigned int reg_value;
	/* select page 7 */
	do {
		reg_value = 0x0007235f;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());

	/* enable LED modify function */
	do {
		reg_value = 0xc03c2353;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());

	/* set green LED shine all the while when link up,
	 * yellow LED blink when data coming
	 */
	do {
		reg_value = 0x00382351;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());

	return;
}
#else
void revise_led_shine(void)
{
}
#endif

#if 0
void set_phy_valtage(void)
{
#define MDIO_RWCTRL      0x1100
#define tmp_mdio_ready() \
	(readl(REG_BASE_SF + MDIO_RWCTRL) & bit(15))

	unsigned int reg_value;
	/* select page 1 */
	do {
		reg_value = 0x0001235f;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());

	/* set fe-phy in lower valtage */
	do {
		reg_value = 0x94482352;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());

	/* select page 0 */
	do {
		reg_value = 0x0000235f;
		writel(reg_value, REG_BASE_SF + MDIO_RWCTRL);
		udelay(10); /* delay 10us */
	} while (!tmp_mdio_ready());
}
#else
void set_phy_valtage(void)
{
}
#endif

static void bspeth_reset(int rst)
{
	u32 val;

	val = _readl(BSPETH_CRG_REG);
	if (rst) {
		val |= ETH_SOFT_RESET;
	} else {
		val &= ~ETH_SOFT_RESET;
	}
	_writel(val, BSPETH_CRG_REG);

	udelay(100); /* delay 100us */
}

static inline void bspeth_clk_ena(void)
{
	u32 val = _readl(BSPETH_CRG_REG);
	val |= (ETH_CORE_CLK_SELECT_54M | ETH_CLK_ENABLE);
	_writel(val, BSPETH_CRG_REG);
}

static inline void bspeth_clk_dis(void)
{
	u32 val = _readl(BSPETH_CRG_REG);
	val &= ~ETH_CLK_ENABLE;
	_writel(val, BSPETH_CRG_REG);
}

#if defined(INNER_PHY)
static void bspeth_fephy_trim(void)
{
	/* To simplify internal FEPHY trim process,
	 * we just delay 300ms to wait FEPHY auto-trim completed.
	 * Not read trim data from EFUSE register.
	 */
	mdelay(350); /* delay 350ms */
}
#endif


static void bspeth_reset_internal_phy(void)
{
#ifdef INNER_PHY
	u32 rst;

	/* disable MDCK clock to make sure FEPHY reset success */
	bspeth_clk_dis();

	rst = readl(BSPETH_CRG_INPHY_CLK_REG);
	rst |= ETH_INTERNAL_PHY_CLK_EN;
	/* internal FEPHY only support MII mode */

	writel(rst, BSPETH_CRG_INPHY_CLK_REG);
	udelay(10); /* delay 10us */

	rst = _readl(BSPETH_CRG_INPHY_CLK_REG);
	rst |= ETH_INTERNAL_PHY_RESET;
	_writel(rst, BSPETH_CRG_INPHY_CLK_REG);
	/* delay at least 10ms */
	mdelay(15); /* delay 15ms */

	rst = _readl(BSPETH_CRG_INPHY_CLK_REG);
	rst &= ~ETH_INTERNAL_PHY_RESET;
	_writel(rst, BSPETH_CRG_INPHY_CLK_REG);
	/* delay at least 15ms for MDIO operation */
	mdelay(20); /* delay 20ms */

	bspeth_clk_ena();
	/* delay 5ms after enable MDCK to make sure FEPHY trim safe */
	mdelay(5); /* delay 5ms */
	bspeth_fephy_trim();
#endif
}

static void bspeth_reset_external_phy_by_crg(void)
{
#if defined(SFV_RESET_PHY_BY_CRG)
	u32 v;

	/************************************************/
	/* reset external phy with default reset pin */
	v = readl(BSPETH_CRG_EXTPHY_CLK_REG);
	v |= ETH_EXTERNAL_PHY_RESET;
	writel(v, BSPETH_CRG_EXTPHY_CLK_REG);

	mdelay(50); /* delay 50ms */

	/* then, cancel reset, and should delay some time */
	v = readl(BSPETH_CRG_EXTPHY_CLK_REG);
	v &= ~ETH_EXTERNAL_PHY_RESET;
	writel(v, BSPETH_CRG_EXTPHY_CLK_REG);

	mdelay(50); /* delay 50ms */
#endif
}

static void bspeth_reset_external_phy_by_gpio(void)
{
#ifdef SFV_RESET_GPIO_EN
	unsigned int val;
	/* gpiox[x] set to reset, then delay 200ms */
	val = __raw_readw(SFV_RESET_GPIO_BASE + SFV_RESET_GPIO_DIR);
	val |= (SFV_RESET_GPIO_DIR_OUT << SFV_RESET_GPIO_BIT);
	__raw_writew(val, SFV_RESET_GPIO_BASE + SFV_RESET_GPIO_DIR);
	__raw_writew(SFV_RESET_GPIO_DATA,
				 SFV_RESET_GPIO_BASE +
				 (4 << SFV_RESET_GPIO_BIT)); /* offset addr 4 */

	mdelay(200); /* delay 200ms */

	/* then,cancel reset,and should delay 200ms */
	val = __raw_readw(SFV_RESET_GPIO_BASE + SFV_RESET_GPIO_DIR);
	val |= (SFV_RESET_GPIO_DIR_OUT << SFV_RESET_GPIO_BIT);
	__raw_writew(val, SFV_RESET_GPIO_BASE + SFV_RESET_GPIO_DIR);
	__raw_writew(((!SFV_RESET_GPIO_DATA) << SFV_RESET_GPIO_BIT),
				 SFV_RESET_GPIO_BASE +
				 (4 << SFV_RESET_GPIO_BIT)); /* offset addr 4 */

	mdelay(20); /* delay 20ms */
#endif
}

static void bspeth_phy_reset(void)
{
	bspeth_reset_internal_phy();
	bspeth_reset_external_phy_by_crg();
	bspeth_reset_external_phy_by_gpio();
}

static void bspeth_funsel_config(void)
{
}

static void bspeth_funsel_restore(void)
{
}

/**************************************************/
void bspeth_sys_startup(void)
{
	bspeth_clk_ena();
	/* undo reset */
	bspeth_reset(0);
}

void bspeth_sys_allstop(void)
{
}

void bspeth_sys_init(void)
{
	bspeth_funsel_config();
	bspeth_sys_allstop();
	bspeth_clk_ena();
	bspeth_reset(1);
	bspeth_reset(0);
	bspeth_phy_reset();
	revise_led_shine();
}

void bspeth_sys_exit(void)
{
	bspeth_funsel_restore();
	bspeth_sys_allstop();
}
