/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __HW_DECOMPRESS_V1_H__
#define __HW_DECOMPRESS_V1_H__

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

extern unsigned int hw_dec_type;

void hw_dec_init(void);
int hw_dec_decompress(unsigned char *dst, int *dstlen,
		      unsigned char *src, int srclen,
		      void *unused);
void hw_dec_uinit(void);

#endif
