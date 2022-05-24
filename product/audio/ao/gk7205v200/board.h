/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_BOARD_H__
#define __GK_BOARD_H__

/***************************************/
#define DDR_BUS_FR                (310000000) /* ddr bus freq:310M */

/***************************************/
#define DDRC0_REG_ADDR            0x120d0000 /* base addr of DDRCB */
#define DDRC_REGS_SIZE            0x10000

#define DDRC0_REG_STAT_CFG_ADDR   (DDRC0_REG_ADDR + 0x260)
#define DDRC0_REG_STAT_WRITE_ADDR (DDRC0_REG_ADDR + 0x264)
#define DDRC0_REG_STAT_READ_ADDR  (DDRC0_REG_ADDR + 0x268)

/***************************************/
#define OTP_REGS_ADDR             0x100A0000
#define OTP_REGS_SIZE             0X10000

/***************************************/
#define CRG_REGS_ADDR             0x12010000
#define CRG_REGS_SIZE             0X10000

/***************************************/
#define SYS_REGS_ADDR             0x12020000
#define SYS_REGS_SIZE             0x8000

/***************************************/
#define MISC_REGS_ADDR            0x12028000
#define MISC_REGS_SIZE            0x8000

/***************************************/
#define VOU_REGS_ADDR             0x11280000
#define VOU_REGS_SIZE             0x40000

/***************************************/
#define VGS0_REGS_ADDR            0x11300000
#define VGS_REGS_SIZE             0x10000

/***************************************/
#define GDC0_BASE_ADDR            0x11110000
#define GDC_REGS_SIZE             0x10000

/***************************************/
#define VPSS0_REGS_ADDR           0x11400000
#define VPSS_REGS_SIZE            0x10000

/***************************************/
#define VI_CAP0_REGS_ADDR         0x11000000
#define VI_PROC0_REGS_ADDR        0x11200000

/***************************************/
#define VEDU_0_REGS_ADDR          0x11410000
#define VEDU_REGS_SIZE            0x10000

/***************************************/
#define JPEGU_REGS_ADDR           0x11420000
#define JPEGU_REGS_SIZE           0x10000

/****************************************/
/* 0x11260000 ~ 0x1126FFFF */
#define JPEGD_REGS_ADDR           (0x11260000)
#define JPEGD_REGS_SIZE           0x10000

/***************************************/
#define IVE_REGS_ADDR             0x11320000
#define IVE_REGS_SIZE             0x10000
#define IVE_CRG_RESET_REGS_ADDR   0x120100c0

#define SVP_NNIE_0_REGS_ADDR      0x11100000
#define SVP_NNIE_0_REGS_SIZE      0x10000

/***************************************/
/* Interrupt Request Number  */
#define VOU_IRQ_NR                (40 + 32)
#define VOU1_IRQ_NR               (41 + 32)
#define VI_CAP0_IRQ_NR            (43 + 32)
#define VI_PROC0_IRQ_NR           (44 + 32)
#define VPSS0_IRQ_NR              (46 + 32)
#define VGS0_IRQ_NR               (49 + 32)
#define AIO_IRQ_NR                (42 + 32)
#define VEDU_0_IRQ_NR             (47 + 32)
#define JPEGU_IRQ_NR              (48 + 32)
#define IVE_IRQ_NR                (51 + 32)

/****************/
#define SYS_PERCTL0_ADDR          (0x0 + SYS_REGS_ADDR)
#define SYS_PERCTL1_ADDR          (0x4 + SYS_REGS_ADDR)
#define SYS_PERCTL2_ADDR          (0x8 + SYS_REGS_ADDR)
#define SYS_PERCTL3_ADDR          (0xc + SYS_REGS_ADDR)
#define SYS_PERCTL4_ADDR          (0x10 + SYS_REGS_ADDR)
#define SYS_PERCTL5_ADDR          (0x14 + SYS_REGS_ADDR)
#define SYS_PERCTL6_ADDR          (0x18 + SYS_REGS_ADDR)
#define SYS_PERCTL7_ADDR          (0x1c + SYS_REGS_ADDR)
#define SYS_PERCTL8_ADDR          (0x20 + SYS_REGS_ADDR)
#define SYS_PERCTL9_ADDR          (0x24 + SYS_REGS_ADDR)
#define SYS_PERCTL10_ADDR         (0x28 + SYS_REGS_ADDR)
#define SYS_PERCTL11_ADDR         (0x2C + SYS_REGS_ADDR)
#define SYS_PERCTL12_ADDR         (0x30 + SYS_REGS_ADDR)
#define SYS_PERCTL13_ADDR         (0x34 + SYS_REGS_ADDR)
#define SYS_PERCTL14_ADDR         (0x38 + SYS_REGS_ADDR)
#define SYS_PERCTL15_ADDR         (0x3C + SYS_REGS_ADDR)
#define SYS_PERCTL16_ADDR         (0x40 + SYS_REGS_ADDR)
#define SYS_PERCTL17_ADDR         (0x44 + SYS_REGS_ADDR)
#define SYS_PERCTL18_ADDR         (0x48 + SYS_REGS_ADDR)
#define SYS_PERCTL19_ADDR         (0x4c + SYS_REGS_ADDR)
#define SYS_PERCTL20_ADDR         (0x50 + SYS_REGS_ADDR)
#define SYS_PERCTL21_ADDR         (0x54 + SYS_REGS_ADDR)
#define SYS_PERCTL22_ADDR         (0x58 + SYS_REGS_ADDR)
#define SYS_PERCTL23_ADDR         (0x5c + SYS_REGS_ADDR)
#define SYS_PERCTL24_ADDR         (0x60 + SYS_REGS_ADDR)
#define SYS_PERCTL25_ADDR         (0x64 + SYS_REGS_ADDR)
#define SYS_PERCTL26_ADDR         (0x68 + SYS_REGS_ADDR)
#define SYS_PERCTL27_ADDR         (0x6C + SYS_REGS_ADDR)
#define SYS_PERCTL28_ADDR         (0x70 + SYS_REGS_ADDR)
#define SYS_PERCTL29_ADDR         (0x74 + SYS_REGS_ADDR)
#define SYS_PERCTL30_ADDR         (0x78 + SYS_REGS_ADDR)
#define SYS_PERCTL31_ADDR         (0x7C + SYS_REGS_ADDR)
#define SYS_PERCTL32_ADDR         (0x80 + SYS_REGS_ADDR)
#define SYS_PERCTL33_ADDR         (0x84 + SYS_REGS_ADDR)
#define SYS_PERCTL34_ADDR         (0x88 + SYS_REGS_ADDR)
#define SYS_PERCTL35_ADDR         (0x8C + SYS_REGS_ADDR)
#define SYS_PERCTL36_ADDR         (0x90 + SYS_REGS_ADDR)
#define SYS_PERCTL37_ADDR         (0x94 + SYS_REGS_ADDR)
#define SYS_PERCTL38_ADDR         (0x98 + SYS_REGS_ADDR)
#define SYS_PERCTL39_ADDR         (0x9C + SYS_REGS_ADDR)
#define SYS_PERCTL40_ADDR         (0xa0 + SYS_REGS_ADDR)
#define SYS_PERCTL41_ADDR         (0xa4 + SYS_REGS_ADDR)
#define SYS_PERCTL42_ADDR         (0xa8 + SYS_REGS_ADDR)
#define SYS_PERCTL43_ADDR         (0xaC + SYS_REGS_ADDR)
#define SYS_PERCTL44_ADDR         (0xb0 + SYS_REGS_ADDR)
#define SYS_PERCTL45_ADDR         (0xb4 + SYS_REGS_ADDR)
#define SYS_PERCTL46_ADDR         (0xb8 + SYS_REGS_ADDR)
#define SYS_PERCTL47_ADDR         (0xbC + SYS_REGS_ADDR)
#define SYS_PERCTL48_ADDR         (0xc0 + SYS_REGS_ADDR)
#define SYS_PERCTL49_ADDR         (0xc4 + SYS_REGS_ADDR)
#define SYS_PERCTL50_ADDR         (0xc8 + SYS_REGS_ADDR)
#define SYS_PERCTL51_ADDR         (0xcC + SYS_REGS_ADDR)
#define SYS_PERCTL52_ADDR         (0xd0 + SYS_REGS_ADDR)
#define SYS_PERCTL53_ADDR         (0xd4 + SYS_REGS_ADDR)
#define SYS_PERCTL54_ADDR         (0xd8 + SYS_REGS_ADDR)
#define SYS_PERCTL55_ADDR         (0xdC + SYS_REGS_ADDR)
#define SYS_PERCTL56_ADDR         (0xe0 + SYS_REGS_ADDR)
#define SYS_PERCTL57_ADDR         (0xe4 + SYS_REGS_ADDR)
#define SYS_PERCTL58_ADDR         (0xe8 + SYS_REGS_ADDR)
#define SYS_PERCTL59_ADDR         (0xeC + SYS_REGS_ADDR)
#define SYS_PERCTL60_ADDR         (0xf0 + SYS_REGS_ADDR)
#define SYS_PERCTL61_ADDR         (0xf4 + SYS_REGS_ADDR)
#define SYS_PERCTL62_ADDR         (0xf8 + SYS_REGS_ADDR)
#define SYS_PERCTL63_ADDR         (0xfC + SYS_REGS_ADDR)
#define SYS_PERCTL64_ADDR         (0x100 + SYS_REGS_ADDR)
#define SYS_PERCTL65_ADDR         (0x104 + SYS_REGS_ADDR)
#define SYS_PERCTL66_ADDR         (0x108 + SYS_REGS_ADDR)
#define SYS_PERCTL67_ADDR         (0x10c + SYS_REGS_ADDR)
#define SYS_PERCTL68_ADDR         (0x110 + SYS_REGS_ADDR)
#define SYS_PERCTL69_ADDR         (0x114 + SYS_REGS_ADDR)
#define SYS_PERCTL70_ADDR         (0x118 + SYS_REGS_ADDR)
#define SYS_PERCTL71_ADDR         (0x11c + SYS_REGS_ADDR)
#define SYS_PERCTL72_ADDR         (0x120 + SYS_REGS_ADDR)
#define SYS_PERCTL73_ADDR         (0x124 + SYS_REGS_ADDR)
#define SYS_PERCTL74_ADDR         (0x128 + SYS_REGS_ADDR)
#define SYS_PERCTL75_ADDR         (0x12C + SYS_REGS_ADDR)
#define SYS_PERCTL76_ADDR         (0x130 + SYS_REGS_ADDR)
#define SYS_PERCTL77_ADDR         (0x134 + SYS_REGS_ADDR)
#define SYS_PERCTL78_ADDR         (0x138 + SYS_REGS_ADDR)
#define SYS_PERCTL79_ADDR         (0x13C + SYS_REGS_ADDR)
#define SYS_PERCTL80_ADDR         (0x140 + SYS_REGS_ADDR)
#define SYS_PERCTL81_ADDR         (0x144 + SYS_REGS_ADDR)
#define SYS_PERCTL82_ADDR         (0x148 + SYS_REGS_ADDR)
#define SYS_PERCTL83_ADDR         (0x14c + SYS_REGS_ADDR)
#define SYS_PERCTL84_ADDR         (0x150 + SYS_REGS_ADDR)
#define SYS_PERCTL85_ADDR         (0x154 + SYS_REGS_ADDR)
#define SYS_PERCTL86_ADDR         (0x158 + SYS_REGS_ADDR)
#define SYS_PERCTL87_ADDR         (0x15c + SYS_REGS_ADDR)
#define SYS_PERCTL88_ADDR         (0x160 + SYS_REGS_ADDR)
#define SYS_PERCTL89_ADDR         (0x164 + SYS_REGS_ADDR)
#define SYS_PERCTL90_ADDR         (0x168 + SYS_REGS_ADDR)
#define SYS_PERCTL91_ADDR         (0x16C + SYS_REGS_ADDR)
#define SYS_PERCTL92_ADDR         (0x170 + SYS_REGS_ADDR)
#define SYS_PERCTL93_ADDR         (0x174 + SYS_REGS_ADDR)
#define SYS_PERCTL94_ADDR         (0x178 + SYS_REGS_ADDR)
#define SYS_PERCTL95_ADDR         (0x17C + SYS_REGS_ADDR)
#define SYS_PERCTL96_ADDR         (0x180 + SYS_REGS_ADDR)
#define SYS_PERCTL97_ADDR         (0x184 + SYS_REGS_ADDR)
#define SYS_PERCTL98_ADDR         (0x188 + SYS_REGS_ADDR)
#define SYS_PERCTL99_ADDR         (0x18C + SYS_REGS_ADDR)
#define SYS_PERCTL100_ADDR        (0x190 + SYS_REGS_ADDR)
#define SYS_PERCTL101_ADDR        (0x194 + SYS_REGS_ADDR)
#define SYS_PERCTL102_ADDR        (0x198 + SYS_REGS_ADDR)
#define SYS_PERCTL103_ADDR        (0x19C + SYS_REGS_ADDR)
#define SYS_PERCTL104_ADDR        (0x1a0 + SYS_REGS_ADDR)
#define SYS_PERCTL105_ADDR        (0x1a4 + SYS_REGS_ADDR)
#define SYS_PERCTL106_ADDR        (0x1a8 + SYS_REGS_ADDR)
#define SYS_PERCTL107_ADDR        (0x1aC + SYS_REGS_ADDR)
#define SYS_PERCTL108_ADDR        (0x1b0 + SYS_REGS_ADDR)
#define SYS_PERCTL109_ADDR        (0x1b4 + SYS_REGS_ADDR)
#define SYS_PERCTL110_ADDR        (0x1b8 + SYS_REGS_ADDR)
#define SYS_PERCTL111_ADDR        (0x1bC + SYS_REGS_ADDR)
#define SYS_PERCTL112_ADDR        (0x1c0 + SYS_REGS_ADDR)
#define SYS_PERCTL113_ADDR        (0x1c4 + SYS_REGS_ADDR)
#define SYS_PERCTL114_ADDR        (0x1c8 + SYS_REGS_ADDR)
#define SYS_PERCTL115_ADDR        (0x1cC + SYS_REGS_ADDR)
#define SYS_PERCTL116_ADDR        (0x1d0 + SYS_REGS_ADDR)
#define SYS_PERCTL117_ADDR        (0x1d4 + SYS_REGS_ADDR)
#define SYS_PERCTL118_ADDR        (0x1d8 + SYS_REGS_ADDR)
#define SYS_PERCTL119_ADDR        (0x1dC + SYS_REGS_ADDR)
#define SYS_PERCTL120_ADDR        (0x1e0 + SYS_REGS_ADDR)
#define SYS_PERCTL121_ADDR        (0x1e4 + SYS_REGS_ADDR)
#define SYS_PERCTL122_ADDR        (0x1e8 + SYS_REGS_ADDR)
#define SYS_PERCTL123_ADDR        (0x1eC + SYS_REGS_ADDR)
#define SYS_PERCTL124_ADDR        (0x1f0 + SYS_REGS_ADDR)
#define SYS_PERCTL125_ADDR        (0x1f4 + SYS_REGS_ADDR)
#define SYS_PERCTL126_ADDR        (0x1f8 + SYS_REGS_ADDR)
#define SYS_PERCTL127_ADDR        (0x1fC + SYS_REGS_ADDR)
/****************/

/****************/
#define CRG_PERCTL0_ADDR          (0x0 + CRG_REGS_ADDR)
#define CRG_PERCTL1_ADDR          (0x4 + CRG_REGS_ADDR)
#define CRG_PERCTL2_ADDR          (0x8 + CRG_REGS_ADDR)
#define CRG_PERCTL3_ADDR          (0xc + CRG_REGS_ADDR)
#define CRG_PERCTL4_ADDR          (0x10 + CRG_REGS_ADDR)
#define CRG_PERCTL5_ADDR          (0x14 + CRG_REGS_ADDR)
#define CRG_PERCTL6_ADDR          (0x18 + CRG_REGS_ADDR)
#define CRG_PERCTL7_ADDR          (0x1c + CRG_REGS_ADDR)
#define CRG_PERCTL8_ADDR          (0x20 + CRG_REGS_ADDR)
#define CRG_PERCTL9_ADDR          (0x24 + CRG_REGS_ADDR)
#define CRG_PERCTL10_ADDR         (0x28 + CRG_REGS_ADDR)
#define CRG_PERCTL11_ADDR         (0x2C + CRG_REGS_ADDR)
#define CRG_PERCTL12_ADDR         (0x30 + CRG_REGS_ADDR)
#define CRG_PERCTL13_ADDR         (0x34 + CRG_REGS_ADDR)
#define CRG_PERCTL14_ADDR         (0x38 + CRG_REGS_ADDR)
#define CRG_PERCTL15_ADDR         (0x3C + CRG_REGS_ADDR)
#define CRG_PERCTL16_ADDR         (0x40 + CRG_REGS_ADDR)
#define CRG_PERCTL17_ADDR         (0x44 + CRG_REGS_ADDR)
#define CRG_PERCTL18_ADDR         (0x48 + CRG_REGS_ADDR)
#define CRG_PERCTL19_ADDR         (0x4c + CRG_REGS_ADDR)
#define CRG_PERCTL20_ADDR         (0x50 + CRG_REGS_ADDR)
#define CRG_PERCTL21_ADDR         (0x54 + CRG_REGS_ADDR)
#define CRG_PERCTL22_ADDR         (0x58 + CRG_REGS_ADDR)
#define CRG_PERCTL23_ADDR         (0x5c + CRG_REGS_ADDR)
#define CRG_PERCTL24_ADDR         (0x60 + CRG_REGS_ADDR)
#define CRG_PERCTL25_ADDR         (0x64 + CRG_REGS_ADDR)
#define CRG_PERCTL26_ADDR         (0x68 + CRG_REGS_ADDR)
#define CRG_PERCTL27_ADDR         (0x6C + CRG_REGS_ADDR)
#define CRG_PERCTL28_ADDR         (0x70 + CRG_REGS_ADDR)
#define CRG_PERCTL29_ADDR         (0x74 + CRG_REGS_ADDR)
#define CRG_PERCTL30_ADDR         (0x78 + CRG_REGS_ADDR)
#define CRG_PERCTL31_ADDR         (0x7C + CRG_REGS_ADDR)
#define CRG_PERCTL32_ADDR         (0x80 + CRG_REGS_ADDR)
#define CRG_PERCTL33_ADDR         (0x84 + CRG_REGS_ADDR)
#define CRG_PERCTL34_ADDR         (0x88 + CRG_REGS_ADDR)
#define CRG_PERCTL35_ADDR         (0x8C + CRG_REGS_ADDR)
#define CRG_PERCTL36_ADDR         (0x90 + CRG_REGS_ADDR)
#define CRG_PERCTL37_ADDR         (0x94 + CRG_REGS_ADDR)
#define CRG_PERCTL38_ADDR         (0x98 + CRG_REGS_ADDR)
#define CRG_PERCTL39_ADDR         (0x9C + CRG_REGS_ADDR)
#define CRG_PERCTL40_ADDR         (0xa0 + CRG_REGS_ADDR)
#define CRG_PERCTL41_ADDR         (0xa4 + CRG_REGS_ADDR)
#define CRG_PERCTL42_ADDR         (0xa8 + CRG_REGS_ADDR)
#define CRG_PERCTL43_ADDR         (0xaC + CRG_REGS_ADDR)
#define CRG_PERCTL44_ADDR         (0xb0 + CRG_REGS_ADDR)
#define CRG_PERCTL45_ADDR         (0xb4 + CRG_REGS_ADDR)
#define CRG_PERCTL46_ADDR         (0xb8 + CRG_REGS_ADDR)
#define CRG_PERCTL47_ADDR         (0xbC + CRG_REGS_ADDR)
#define CRG_PERCTL48_ADDR         (0xc0 + CRG_REGS_ADDR)
#define CRG_PERCTL49_ADDR         (0xc4 + CRG_REGS_ADDR)
#define CRG_PERCTL50_ADDR         (0xc8 + CRG_REGS_ADDR)
#define CRG_PERCTL51_ADDR         (0xcC + CRG_REGS_ADDR)
#define CRG_PERCTL52_ADDR         (0xd0 + CRG_REGS_ADDR)
#define CRG_PERCTL53_ADDR         (0xd4 + CRG_REGS_ADDR)
#define CRG_PERCTL54_ADDR         (0xd8 + CRG_REGS_ADDR)
#define CRG_PERCTL55_ADDR         (0xdC + CRG_REGS_ADDR)
#define CRG_PERCTL56_ADDR         (0xe0 + CRG_REGS_ADDR)
#define CRG_PERCTL57_ADDR         (0xe4 + CRG_REGS_ADDR)
#define CRG_PERCTL58_ADDR         (0xe8 + CRG_REGS_ADDR)
#define CRG_PERCTL59_ADDR         (0xeC + CRG_REGS_ADDR)
#define CRG_PERCTL60_ADDR         (0xf0 + CRG_REGS_ADDR)
#define CRG_PERCTL61_ADDR         (0xf4 + CRG_REGS_ADDR)
#define CRG_PERCTL62_ADDR         (0xf8 + CRG_REGS_ADDR)
#define CRG_PERCTL63_ADDR         (0xfC + CRG_REGS_ADDR)
#define CRG_PERCTL64_ADDR         (0x100 + CRG_REGS_ADDR)
#define CRG_PERCTL65_ADDR         (0x104 + CRG_REGS_ADDR)
#define CRG_PERCTL66_ADDR         (0x108 + CRG_REGS_ADDR)
#define CRG_PERCTL67_ADDR         (0x10c + CRG_REGS_ADDR)
#define CRG_PERCTL68_ADDR         (0x110 + CRG_REGS_ADDR)
#define CRG_PERCTL69_ADDR         (0x114 + CRG_REGS_ADDR)
#define CRG_PERCTL70_ADDR         (0x118 + CRG_REGS_ADDR)
#define CRG_PERCTL71_ADDR         (0x11c + CRG_REGS_ADDR)
#define CRG_PERCTL72_ADDR         (0x120 + CRG_REGS_ADDR)
#define CRG_PERCTL73_ADDR         (0x124 + CRG_REGS_ADDR)
#define CRG_PERCTL74_ADDR         (0x128 + CRG_REGS_ADDR)
#define CRG_PERCTL75_ADDR         (0x12C + CRG_REGS_ADDR)
#define CRG_PERCTL76_ADDR         (0x130 + CRG_REGS_ADDR)
#define CRG_PERCTL77_ADDR         (0x134 + CRG_REGS_ADDR)
#define CRG_PERCTL78_ADDR         (0x138 + CRG_REGS_ADDR)
#define CRG_PERCTL79_ADDR         (0x13C + CRG_REGS_ADDR)
#define CRG_PERCTL80_ADDR         (0x140 + CRG_REGS_ADDR)
#define CRG_PERCTL81_ADDR         (0x144 + CRG_REGS_ADDR)
#define CRG_PERCTL82_ADDR         (0x148 + CRG_REGS_ADDR)
#define CRG_PERCTL83_ADDR         (0x14c + CRG_REGS_ADDR)
#define CRG_PERCTL84_ADDR         (0x150 + CRG_REGS_ADDR)
#define CRG_PERCTL85_ADDR         (0x154 + CRG_REGS_ADDR)
#define CRG_PERCTL86_ADDR         (0x158 + CRG_REGS_ADDR)
#define CRG_PERCTL87_ADDR         (0x15c + CRG_REGS_ADDR)
#define CRG_PERCTL88_ADDR         (0x160 + CRG_REGS_ADDR)
#define CRG_PERCTL89_ADDR         (0x164 + CRG_REGS_ADDR)
#define CRG_PERCTL90_ADDR         (0x168 + CRG_REGS_ADDR)
#define CRG_PERCTL91_ADDR         (0x16C + CRG_REGS_ADDR)
#define CRG_PERCTL92_ADDR         (0x170 + CRG_REGS_ADDR)
#define CRG_PERCTL93_ADDR         (0x174 + CRG_REGS_ADDR)
#define CRG_PERCTL94_ADDR         (0x178 + CRG_REGS_ADDR)
#define CRG_PERCTL95_ADDR         (0x17C + CRG_REGS_ADDR)
#define CRG_PERCTL96_ADDR         (0x180 + CRG_REGS_ADDR)
#define CRG_PERCTL97_ADDR         (0x184 + CRG_REGS_ADDR)
#define CRG_PERCTL98_ADDR         (0x188 + CRG_REGS_ADDR)
#define CRG_PERCTL99_ADDR         (0x18C + CRG_REGS_ADDR)
#define CRG_PERCTL100_ADDR        (0x190 + CRG_REGS_ADDR)
#define CRG_PERCTL101_ADDR        (0x194 + CRG_REGS_ADDR)
#define CRG_PERCTL102_ADDR        (0x198 + CRG_REGS_ADDR)
#define CRG_PERCTL103_ADDR        (0x19C + CRG_REGS_ADDR)
#define CRG_PERCTL104_ADDR        (0x1a0 + CRG_REGS_ADDR)
#define CRG_PERCTL105_ADDR        (0x1a4 + CRG_REGS_ADDR)
#define CRG_PERCTL106_ADDR        (0x1a8 + CRG_REGS_ADDR)
#define CRG_PERCTL107_ADDR        (0x1aC + CRG_REGS_ADDR)
#define CRG_PERCTL108_ADDR        (0x1b0 + CRG_REGS_ADDR)
#define CRG_PERCTL109_ADDR        (0x1b4 + CRG_REGS_ADDR)
#define CRG_PERCTL110_ADDR        (0x1b8 + CRG_REGS_ADDR)
#define CRG_PERCTL111_ADDR        (0x1bC + CRG_REGS_ADDR)
#define CRG_PERCTL112_ADDR        (0x1c0 + CRG_REGS_ADDR)
#define CRG_PERCTL113_ADDR        (0x1c4 + CRG_REGS_ADDR)
#define CRG_PERCTL114_ADDR        (0x1c8 + CRG_REGS_ADDR)
#define CRG_PERCTL115_ADDR        (0x1cC + CRG_REGS_ADDR)
#define CRG_PERCTL116_ADDR        (0x1d0 + CRG_REGS_ADDR)
#define CRG_PERCTL117_ADDR        (0x1d4 + CRG_REGS_ADDR)
#define CRG_PERCTL118_ADDR        (0x1d8 + CRG_REGS_ADDR)
#define CRG_PERCTL119_ADDR        (0x1dC + CRG_REGS_ADDR)
#define CRG_PERCTL120_ADDR        (0x1e0 + CRG_REGS_ADDR)
#define CRG_PERCTL121_ADDR        (0x1e4 + CRG_REGS_ADDR)
#define CRG_PERCTL122_ADDR        (0x1e8 + CRG_REGS_ADDR)
#define CRG_PERCTL123_ADDR        (0x1eC + CRG_REGS_ADDR)
#define CRG_PERCTL124_ADDR        (0x1f0 + CRG_REGS_ADDR)
#define CRG_PERCTL125_ADDR        (0x1f4 + CRG_REGS_ADDR)
#define CRG_PERCTL126_ADDR        (0x1f8 + CRG_REGS_ADDR)
#define CRG_PERCTL127_ADDR        (0x1fC + CRG_REGS_ADDR)
/****************/

#define MISC_CTL18_ADDR           (0x18 + MISC_REGS_ADDR)
#define MISC_CTL12C_ADDR          (0x12C + MISC_REGS_ADDR)
#define MISC_CTL98_ADDR           (0x98 + MISC_REGS_ADDR)
#define MISC_CTL9C_ADDR           (0x9C + MISC_REGS_ADDR)

/****************/

/* For the following function
 ** typedef GK_S32 FN_SYS_ViDivSel(GK_S32 s32ViDev, GK_U32 u32DivSel);
 ** typedef GK_S32 FN_SYS_ViClkSel(GK_S32 s32ViDev, GK_BOOL bSelf);
 */
#define SYS_VI_DIV_SEL2           0x00 /* 2 division  */
#define SYS_VI_DIV_SEL4           0x01 /* 4 division  */
#define SYS_VI_DIV_SEL1           0x02 /* no division */

/* For the following function
 ** typedef GK_S32 FN_SYS_VoDivSel(GK_S32 s32ViDev, GK_U32 u32DivSel);
 */
#define SYS_VO_DIV_SEL1           0x00 /* 1 division  */
#define SYS_VO_DIV_SEL2           0x01 /* 2 division  */
#define SYS_VO_DIV_SEL4           0x02 /* 4 division  */

/* For the following function
 ** typedef GK_S32 FN_SYS_AioSampleClkDivSel(GK_U32 u32DivSel);
 */
#define SYS_AIO_SAMPLE_CLK16      0x0 /* 16 division */
#define SYS_AIO_SAMPLE_CLK32      0x01 /* 32 division */
#define SYS_AIO_SAMPLE_CLK48      0x02 /* 48 division */
#define SYS_AIO_SAMPLE_CLK64      0x03 /* 64 division */
#define SYS_AIO_SAMPLE_CLK128     0x04 /* 128 division */
#define SYS_AIO_SAMPLE_CLK256     0x05 /* 256 division */

/* For the following function
 ** typedef GK_S32 FN_SYS_AioBitStreamClkDivSel(GK_U32 u32DivSel);
 */
#define SYS_AIO_BS_CLK1           0x00 /* 1 division */
#define SYS_AIO_BS_CLK2           0x02 /* 2 division */
#define SYS_AIO_BS_CLK3           0x01 /* 3 division */
#define SYS_AIO_BS_CLK4           0x03 /* 4 division */
#define SYS_AIO_BS_CLK6           0x04 /* 6 division */
#define SYS_AIO_BS_CLK8           0x05 /* 8 division */
#define SYS_AIO_BS_CLK12          0x06 /* 12 division */
#define SYS_AIO_BS_CLK16          0x07 /* 16 division */
#define SYS_AIO_BS_CLK24          0x08 /* 24 division */
#define SYS_AIO_BS_CLK32          0x09 /* 32 division */
#define SYS_AIO_BS_CLK48          0x0a /* 48 division */
#define SYS_AIO_BS_CLK64          0x0b /* 64 division */

#endif /* __GK_BOARD_H__ */

