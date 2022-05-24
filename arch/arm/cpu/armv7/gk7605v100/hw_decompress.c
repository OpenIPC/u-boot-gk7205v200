/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <config.h>
#include <linux/kconfig.h>
#include <asm/io.h>
#include <compiler.h>
#include <cpu_common.h>

#define error(_s)               uart_early_puts(_s)
#define putstr(_s)              uart_early_puts(_s)

#define PAGE_SIZE       4096
#define page_nr(x) (((x) + PAGE_SIZE - 1) / PAGE_SIZE)

/* The base address for emar */
#define HW_DEC_REG_BASE_ADDR    (GZIP_REG_BASE)

/* The global init registers for emar interface */
#define EAMR_RID_REG_OFST   0x0108UL
#define EAMR_ROSD_REG_OFST  0x010CUL
#define EAMR_WID_REG_OFST   0x0110UL
#define EAMR_WOSD_REG_OFST  0x0114UL

/* The enable register */
#define EAMR_WORK_EN_REG_OFST 0x0100UL

#define DPRS_DATA_SRC_BADDR 0x2040UL
#define DPRS_DATA_SRC_LEN   0x2044UL

/* Decompress parameter reigsters for page address */
#define DPRS_DATA_RTN_BADDR 0x2020UL
#define DPRS_DATA_RTN_LEN   0x2024UL

/* Decompress parameter registers for page data */
#define DPRS_DATA_INFO_BADDR 0x2028UL
#define DPRS_DATA_INFO_LEN  0x202CUL

#define DPRS_DATA_CRC32     0x2030UL

#define CRC_CHECK_EN        0x4000UL

/* The status registers */
#define BUF_INFO            0x2080UL
#define DPRS_RTN_INFO       0x2084UL
#define DPRS_RTN_LEN        0x2088UL
#define BUF_INFO_CLR        0x2090UL
#define RLT_INFO_CLR        0x2094UL

/* The intr registers */
#define INT_EN_REG_ADDR     0x0128UL
#define INT_STATUS_REG_ADDR 0x0124UL
#define INT_CLEAR_REG_ADDR  0x0130UL

#define TASK_MODE_REG       0x2134UL

/* Define the union u_dprs_data_buf_info */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    buf_len: 24; /* [23..0] */
		unsigned int    buf_id: 2; /* [25..24] */
		unsigned int    reserved_1: 2; /* [27..26] */
		unsigned int    eop: 1; /* [28] */
		unsigned int    sop: 1; /* [29] */
		unsigned int    reserved_0: 1; /* [30] */
		unsigned int    mode: 1; /* [31] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} u_dprs_data_buf_info;

typedef union {
	struct {
		unsigned int buf_id: 2; /* [1:0] */
		unsigned int rsv: 29;   /* [30:2] */
		unsigned int aval_flg: 1;   /* [31] */
	} bits;
	unsigned int u32;
} u_buf_status;

typedef union {
	struct {
		unsigned int err_info: 8;   /* [7:0] */
		unsigned int rsv: 23;   /* [30:8] */
		unsigned int aval_flg: 1;   /* [31] */
	} bits;

	unsigned int u32;
} u_dprs_rtn_status;

/* Define the union U_INT_EN */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    task_intrpt_en: 1; /* [0] */
		unsigned int    block_intrpt_en: 1; /* [1] */
		unsigned int    reserved_0: 30; /* [31..2] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} u_intr_en;

typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    task_intrpt: 1; /* [0] */
		unsigned int    block_intrpt: 1; /* [1] */
		unsigned int    reserved_0: 30; /* [31..2] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} u_intr_status;

typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    task_intrpt_clr: 1; /* [0] */
		unsigned int    block_intrpt_clr: 1; /* [1] */
		unsigned int    reserved_0: 30; /* [31..2] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} u_intr_clr;


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


unsigned int hw_dec_type;
unsigned int hw_dec_sop;
unsigned int hw_dec_eop;
unsigned int hw_dec_cur_blk;
unsigned int hw_blk_total_num;

void hw_dec_sop_eop_first_set(int block_num)
{
	if (block_num == 1) {
		hw_dec_sop = 1;
		hw_dec_eop = 1;
	} else {
		hw_dec_sop = 1;
		hw_dec_eop = 0;
	}

	hw_dec_cur_blk = 0;
	hw_blk_total_num = block_num;
}

static inline void hw_dec_work_en_set(int work_en_flg)
{
	/* Enable the emar */
	writel(work_en_flg, HW_DEC_REG_BASE_ADDR + EAMR_WORK_EN_REG_OFST);
}

static inline void hw_dec_rtn_baddr_set(unsigned int addr)
{
	writel(addr, HW_DEC_REG_BASE_ADDR + DPRS_DATA_RTN_BADDR);
}

static inline void hw_dec_dprs_data_baddr_set(unsigned int addr)
{
	writel(addr, HW_DEC_REG_BASE_ADDR + DPRS_DATA_INFO_BADDR);
}

static inline void hw_dec_data_rtn_len_set(unsigned int len)
{
	writel(len, HW_DEC_REG_BASE_ADDR + DPRS_DATA_RTN_LEN);
}

static inline void hw_dec_dprs_data_len_set(unsigned int len)
{
	writel(len, HW_DEC_REG_BASE_ADDR + DPRS_DATA_INFO_LEN);
}

static inline void hw_dec_crc_check_en(unsigned int crc_en)
{
	writel(crc_en, HW_DEC_REG_BASE_ADDR + CRC_CHECK_EN);
}

static inline void hw_dec_data_crc32_set(unsigned int crc32)
{
	writel(crc32, HW_DEC_REG_BASE_ADDR + DPRS_DATA_CRC32);
}

static inline unsigned int hw_dec_buf_status_get(void)
{
	return readl(HW_DEC_REG_BASE_ADDR + BUF_INFO);
}

static inline unsigned int hw_dec_dprs_rtn_status_get(void)
{
	return readl(HW_DEC_REG_BASE_ADDR + DPRS_RTN_INFO);
}

static inline void hw_dec_buf_status_clr(void)
{
	writel(0x1, HW_DEC_REG_BASE_ADDR + BUF_INFO_CLR);
}

static inline void hw_dec_dprs_rtn_status_clr(void)
{
	writel(0x1, HW_DEC_REG_BASE_ADDR + RLT_INFO_CLR);
}

static void hw_dec_intr_en_set(int blk_intr_en, int task_intr_en)
{
	u_intr_en intr_en;
	intr_en.bits.task_intrpt_en = task_intr_en;
	intr_en.bits.block_intrpt_en = blk_intr_en;
	writel(intr_en.u32, HW_DEC_REG_BASE_ADDR + INT_EN_REG_ADDR);
}

static inline unsigned int hw_dec_intr_status_get(void)
{
	return readl(HW_DEC_REG_BASE_ADDR + INT_STATUS_REG_ADDR);
}

static void hw_dec_block_intr_status_clr(void)
{
	u_intr_clr intr_clr;

	intr_clr.u32 = readl(HW_DEC_REG_BASE_ADDR + INT_CLEAR_REG_ADDR);
	intr_clr.bits.block_intrpt_clr = 0x1;
	writel(intr_clr.u32, HW_DEC_REG_BASE_ADDR + INT_CLEAR_REG_ADDR);
}

static void hw_dec_task_intr_status_clr(void)
{
	u_intr_clr intr_clr;

	intr_clr.u32 = readl(HW_DEC_REG_BASE_ADDR + INT_CLEAR_REG_ADDR);
	intr_clr.bits.task_intrpt_clr = 0x1;
	writel(intr_clr.u32, HW_DEC_REG_BASE_ADDR + INT_CLEAR_REG_ADDR);
}

int hw_dec_intr_proc(int irq, void *para)
{
	u_buf_status buf_status;
	u_intr_status intr_status;
	u_dprs_rtn_status dprs_status;
	int ret = 0;

	intr_status.u32 = hw_dec_intr_status_get();
	if (intr_status.bits.block_intrpt) {
		buf_status.u32 = hw_dec_buf_status_get();
		if (buf_status.bits.aval_flg)
			hw_dec_buf_status_clr();

		hw_dec_block_intr_status_clr();
	}

	if (intr_status.bits.task_intrpt) {
		dprs_status.u32 = hw_dec_dprs_rtn_status_get();
		if (dprs_status.bits.aval_flg) {
			if (dprs_status.bits.err_info)
				ret = -2; /* -2:failed */

			hw_dec_dprs_rtn_status_clr();
		}

		hw_dec_task_intr_status_clr();
		goto out;
	}

	ret = -1;
out:
	return ret;
}

void hw_dec_start(unsigned int src_baddr,
			unsigned int dst_baddr,
			unsigned int src_len,
			unsigned int dst_len,
			unsigned int crc_en,
			unsigned int crc32,
			unsigned int dec_type)
{
	unsigned int val;

	if (hw_dec_sop) {
		if (!dec_type) {
			/* set the parameters of output buffer */
			hw_dec_rtn_baddr_set(dst_baddr);
			hw_dec_data_rtn_len_set(dst_len);
		} else {
			/* set the parameter of output buffer */
			hw_dec_dprs_data_baddr_set(dst_baddr);
			hw_dec_dprs_data_len_set(page_nr(dst_len) * 4); /* 4:Align */
		}
	}

	/* set the parameter of input buffer */
	writel(src_baddr, HW_DEC_REG_BASE_ADDR + DPRS_DATA_SRC_BADDR);

	val = src_len |
		(hw_dec_sop << 28) | (hw_dec_eop << 29) | (!dec_type << 31); /* 28,29,31 Move Left bit */
	writel(val, HW_DEC_REG_BASE_ADDR + DPRS_DATA_SRC_LEN);

	hw_dec_crc_check_en(crc_en);
}

static inline void delay(unsigned int num)
{
	volatile unsigned int i;

	for (i = 0; i < (100 * num); i++) /* 100: Cycle */
		__asm__ __volatile__("nop");
}

int hw_dec_wait_finish(void)
{
	int ret;
	int times = 0;

	do {
		ret = hw_dec_intr_proc(HW_DEC_INTR, NULL);
		times++;
		if (times > 2000000) { /* 2000000 ms */
			error("hardware decompress overtime!\n");
			break;
		}
		delay(1);
	} while (-1 == ret);

	return ret;
}

int hw_dec_decompress(unsigned char *dst, int *dstlen,
			unsigned char *src, int srclen,
			void *unused)
{
	int ret;

	hw_dec_sop_eop_first_set(1);
	hw_dec_start((unsigned int)(uintptr_t)src, (unsigned int)(uintptr_t)dst,
				 srclen, *dstlen, 1, 0, hw_dec_type);

	ret = hw_dec_wait_finish();

	*dstlen = readl(HW_DEC_REG_BASE_ADDR + DPRS_RTN_LEN);

	if (ret)
		return -1;

	return 0;
}

void hw_dec_init(void)
{
	/* enable decompress clock */
	enable_decompress_clock();
	/* Init the emar interface */
	writel(0, HW_DEC_REG_BASE_ADDR + EAMR_RID_REG_OFST);
	writel(0x3, HW_DEC_REG_BASE_ADDR + EAMR_ROSD_REG_OFST);
	writel(0, HW_DEC_REG_BASE_ADDR + EAMR_WID_REG_OFST);
	writel(0x3, HW_DEC_REG_BASE_ADDR + EAMR_WOSD_REG_OFST);

	/* Enable interrupt */
	hw_dec_intr_en_set(0x1, 0x1);

	/* Enable emar */
	hw_dec_work_en_set(0x1);
}

void hw_dec_uinit(void)
{
	hw_dec_work_en_set(0x0);
	hw_dec_intr_en_set(0x0, 0x0);

	/* disable decompress clock */
	disable_decompress_clock();
}

