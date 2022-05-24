#include <common.h>
#include <asm/io.h>

#include "amp_reg.h"

//#define DEBUG_AMP

static inline void amp_RegWrite32(unsigned long value, unsigned long mask,
		unsigned long addr)
{
	unsigned long t;

	t = readl((const volatile void*)addr);
	t &= ~mask;
	t |= value & mask;
	writel(t, (volatile void*)addr);

#ifdef DEBUG_AMP
	printf("### fun:%s line:%d addr :%8lx mask:%8lx value: %8lx\n", __FUNCTION__, __LINE__, addr, mask, value);
#endif
}

#if defined(AO_PRODUCT_GK7205V200)
static void amp_unmute_mux_gk7205v200_demo(void)
{
	// gk7205v200 demo
	amp_RegWrite32(0x1000, 0xffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7205v200_demo amp unmute ok!\n");
#endif
}

static void amp_mute_mux_gk7205v200_demo(void)
{
	// gk7205v200 demo
	amp_RegWrite32(0x1000, 0xffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7205v200_demo amp mute ok!\n");
#endif
}
#endif

#if defined(AO_PRODUCT_GK7205V300)
static void amp_unmute_mux_gk7205v300_demo(void)
{
	// gk7205v300 demo
	amp_RegWrite32(0x1000, 0xffffffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7205v300_demo amp unmute ok!\n");
#endif
}

static void amp_mute_mux_gk7205v300_demo(void)
{
	// gk7205v300 demo
	amp_RegWrite32(0x1000, 0xffffffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7205v300_demo amp mute ok!\n");
#endif
}
#endif

#if defined(AO_PRODUCT_GK7202V300)
static void amp_unmute_mux_gk7202v300_demo(void)
{
	// gk7202v300 demo
	amp_RegWrite32(0x1E02, 0xffffffff, IOCFG_AHB_GPIO1_5_ADDR);
	amp_RegWrite32(0x1 << 5, 0x1 << 5, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x1 << 5, 0x1 << 5, GPIO_REGS_ADDR + 0x1080);
#ifdef DEBUG_AMP
	printf("gk7202v300_demo amp unmute ok!\n");
#endif
}

static void amp_mute_mux_gk7202v300_demo(void)
{
	// gk7202v300 demo
	amp_RegWrite32(0x1E02, 0xffffffff, IOCFG_AHB_GPIO1_5_ADDR);
	amp_RegWrite32(0x0 << 5, 0x1 << 5, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x0 << 5, 0x1 << 5, GPIO_REGS_ADDR + 0x1080);
#ifdef DEBUG_AMP
	printf("gk7202v300_demo amp mute ok!\n");
#endif
}
#endif

#if defined(AO_PRODUCT_GK7605V100)
static void amp_unmute_mux_gk7605v100_demo(void)
{
	// gk7605v100 demo
	amp_RegWrite32(0x1000, 0xffffffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x1, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7605v100_demo amp unmute ok!\n");
#endif
}

static void amp_mute_mux_gk7605v100_demo(void)
{
	// gk7605v100 demo
	amp_RegWrite32(0x1000, 0xffffffff, IOCFG_AHB_GPIO1_0_ADDR);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1400);
	amp_RegWrite32(0x0, 0x1, GPIO_REGS_ADDR + 0x1004);
#ifdef DEBUG_AMP
	printf("gk7605v100_demo amp mute ok!\n");
#endif
}
#endif

void amp_unmute(void)
{
#if defined(AO_PRODUCT_GK7205V200)
	amp_unmute_mux_gk7205v200_demo();
#elif defined(AO_PRODUCT_GK7205V300)
	amp_unmute_mux_gk7205v300_demo();
#elif defined(AO_PRODUCT_GK7202V300)
	amp_unmute_mux_gk7202v300_demo();
#elif defined(AO_PRODUCT_GK7605V100)
	amp_unmute_mux_gk7605v100_demo();
#else
	printf("amp_unmute fail!\n");
#endif
}

void amp_mute(void)
{
#if defined(AO_PRODUCT_GK7205V200)
	amp_mute_mux_gk7205v200_demo();
#elif defined(AO_PRODUCT_GK7205V300)
	amp_mute_mux_gk7205v300_demo();
#elif defined(AO_PRODUCT_GK7202V300)
	amp_mute_mux_gk7202v300_demo();
#elif defined(AO_PRODUCT_GK7605V100)
	amp_mute_mux_gk7605v100_demo();
#else
	printf("amp_mute fail!\n");
#endif
}
