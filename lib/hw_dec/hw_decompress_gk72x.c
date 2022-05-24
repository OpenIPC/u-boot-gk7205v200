/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <asm/io.h>
#include <config.h>

#define PERI_CRG100 0x190

#define GZIP_OFFSET PERI_CRG100
#define GZIP_CLKEN (0x1<<0)

#ifndef GZIP_REG_BASE
#define GZIP_REG_BASE   0x11310000
#endif

#define HW_DEC_REG_BASE_ADDR  (GZIP_REG_BASE)

static void disable_decompress_clock(void)
{
	unsigned int regval;

	regval = readl(CRG_REG_BASE + GZIP_OFFSET);
	regval &= ~GZIP_CLKEN;
	writel(regval, CRG_REG_BASE + GZIP_OFFSET);
}

static void enable_decompress_clock(void)
{
	unsigned int regval;

	regval = readl(CRG_REG_BASE + GZIP_OFFSET);
	regval |= GZIP_CLKEN;
	writel(regval, CRG_REG_BASE + GZIP_OFFSET);
}
