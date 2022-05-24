/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#define BSPETH_FEMAC

#include "bspeth.h"
#include "ctrl.h"

static inline u32 _bspeth_irq_enable(struct bspeth_netdev_local *ld, u32 irqs)
{
	u32 old = bspeth_readl(ld, GLB_RW_IRQ_ENA);

	bspeth_writel(ld, old | irqs, GLB_RW_IRQ_ENA);

	return old;
}

static inline u32 _bspeth_irq_disable(struct bspeth_netdev_local *ld, u32 irqs)
{
	u32 old = bspeth_readl(ld, GLB_RW_IRQ_ENA);

	bspeth_writel(ld, old & (~irqs), GLB_RW_IRQ_ENA);

	return old;
}

static inline u32 _bspeth_read_irqstatus(struct bspeth_netdev_local *ld)
{
	u32 status;

	status = bspeth_readl(ld, GLB_RO_IRQ_STAT);

	return status;
}

static inline int _test_xmit_queue_ready(struct bspeth_netdev_local *ld)
{
	return bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT),
							BITS_XMITQ_RDY);
}

static inline int _test_recv_queue_ready(struct bspeth_netdev_local *ld)
{
	return bspeth_readl_bits(ld, ud_reg_name(GLB_RO_QUEUE_STAT),
							BITS_RECVQ_RDY);
}

u32 bspeth_irq_enable(struct bspeth_netdev_local *ld, u32 irqs)
{
	u32 old;

	local_lock(ld);
	old = _bspeth_irq_enable(ld, irqs);
	local_unlock(ld);

	return old;
}

u32 bspeth_irq_disable(struct bspeth_netdev_local *ld, u32 irqs)
{
	u32 old;

	local_lock(ld);
	old = _bspeth_irq_disable(ld, irqs);
	local_unlock(ld);

	return old;
}

u32 bspeth_read_irqstatus(struct bspeth_netdev_local *ld)
{
	u32 status;

	local_lock(ld);
	status = _bspeth_read_irqstatus(ld);
	local_unlock(ld);

	return status;
}

u32 bspeth_read_raw_irqstatus(struct bspeth_netdev_local *ld)
{
	u32 status;

	local_lock(ld);
	status = bspeth_readl(ld, GLB_RO_IRQ_STAT);
	local_unlock(ld);

	return status;
}

u32 bspeth_clear_irqstatus(struct bspeth_netdev_local *ld, u32 irqs)
{
	u32 status;

	local_lock(ld);
	bspeth_writel(ld, irqs, GLB_RW_IRQ_RAW);
	status = _bspeth_read_irqstatus(ld);
	local_unlock(ld);

	return status;
}

u32 bspeth_set_endian_mode(struct bspeth_netdev_local *ld, u32 mode)
{
	u32 old;

	local_lock(ld);
	old = bspeth_readl_bits(ld, GLB_ENDIAN_MOD, BITS_ENDIAN);
	bspeth_writel_bits(ld, mode, GLB_ENDIAN_MOD, BITS_ENDIAN);
	local_unlock(ld);

	return old;
}

void hw_xmitq_setfd(struct bspeth_netdev_local *ld, struct bspeth_frame_desc fd)
{
	bspeth_writel(ld, fd.frm_addr, ud_reg_name(GLB_EQ_ADDR));
	bspeth_writel_bits(ld, fd.frm_len, ud_reg_name(GLB_EQFRM_LEN), BITS_TXINQ_LEN);
}

u32 bspeth_readl(struct bspeth_netdev_local *ld, u32 ofs)
{
	u32 reg = _readl((uintptr_t)(ld->iobase_phys + ofs));
	bspeth_trace(BSPETH_TRACE_ETH, "_readl(0x%08X) = 0x%08X",
		(u32)(ld->iobase_phys + ofs), reg);
	return reg;
}

void bspeth_writel(struct bspeth_netdev_local *ld, u32 v, u32 ofs)
{
	_writel(v, (uintptr_t)(ld->iobase_phys + ofs));
	bspeth_trace(BSPETH_TRACE_ETH, "_writel(0x%08X) = 0x%08X",
		(u32)(ld->iobase_phys + ofs), v);
}


u32 bspeth_readl_bits(struct bspeth_netdev_local *ld, u32 ofs, u32 bits_desc)
{
	u32 _bits_desc = bits_desc;
	u32 _shift = _bits_desc >> 16; /* shift 16 bit */
	u32 _mask = ((1 << (_bits_desc & 0x1F)) - 1) << _shift;
	u32 reg = (bspeth_readl(ld, ofs) & _mask) >> _shift;
	return reg;
}

void bspeth_writel_bits(struct bspeth_netdev_local *ld, u32 v, u32 ofs, u32 bits_desc)
{
	u32 _bits_desc = bits_desc;
	u32 _shift = _bits_desc >> 16; /* shift 16 bit */
	u32 _reg = bspeth_readl(ld, ofs);
	u32 _mask = ((1 << (_bits_desc & 0x1F)) - 1) << _shift;
	bspeth_writel(ld, (_reg & (~_mask)) | ((v << _shift) & _mask), ofs);
}

void bspeth_trace(int level, const char *fmt, ...)
{
	if (level >= BSPETH_TRACE_LEVEL) {
		va_list args;
		va_start(args, fmt);
		printf("bspeth_trace:");
		printf(fmt, args);
		printf("\n");
		va_end(args);
	}
}

void bspeth_error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("bspeth:");
	printf(fmt, args);
	printf("\n");
	va_end(args);
}

void bspeth_assert(bool cond)
{
	if (!cond)
		printf("Assert:bspeth:%s:%d\n", __FILE__, __LINE__);
}