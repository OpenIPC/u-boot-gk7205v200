/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef DDR_TRAINING_CUSTOM_H
#define DDR_TRAINING_CUSTOM_H

/* config DDRC, PHY, DDRT typte */
#define DDR_DDRC_V520_CONFIG
#define DDR_PHY_T28_CONFIG
#define DDR_DDRT_V2_0_SHF1_CONFIG

/* config special item */
#define DDR_VREF_TRAINING_CONFIG
#define DDR_VREF_WITHOUT_BDL_CONFIG
//#define DDR_PCODE_TRAINING_CONFIG

#define DDR_WL_TRAINING_DISABLE
#define DDR_GATE_TRAINING_DISABLE
#define DDR_TRAINING_UART_DISABLE

/* Disable write dm
#define DDR_WRITE_DM_DISABLE */

#define DDR_PHY_NUM              1 /* phy number */

#define DDR_DMC_PER_PHY_MAX      2 /* dmc number per phy max */

#define DDR_AXI_SWITCH_NUM       4 /* ddr training axi switch number */

/* config DDRC, PHY, DDRT base address */
/* [CUSTOM] DDR PHY0 base register */
#define DDR_REG_BASE_PHY0		0x120dc000
/* [CUSTOM] DDR PHY1 base register
#define DDR_REG_BASE_PHY1		0x120de000 */
/* [CUSTOM] DDR DMC0 base register */
#define DDR_REG_BASE_DMC0		0x120d8000
/* [CUSTOM] DDR DMC1 base register */
#define DDR_REG_BASE_DMC1		0x120d8000

#ifdef DDR_REG_BASE_PHY1
/* [CUSTOM] DDR DMC2 base register */
#define DDR_REG_BASE_DMC2		0x120d9000
/* [CUSTOM] DDR DMC3 base register */
#define DDR_REG_BASE_DMC3		0x120d9000
#endif

/* [CUSTOM] DDR DDRT base register */
#define DDR_REG_BASE_DDRT		0x11330000
/* [CUSTOM] DDR training item system control */
#define DDR_REG_BASE_SYSCTRL		0x12020000
#define DDR_REG_BASE_AXI		0x120d0000
/* Serial Configuration */
#define DDR_REG_BASE_UART0		0x12040000

/* config offset address */
/* Assume sysctrl offset address for DDR training as follows,
if not please define. */
/* [CUSTOM] ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN			0xa8
/* [CUSTOM] PHY2 ddrt reversed data
#define SYSCTRL_DDRT_PATTERN_SEC		0xac */
/* [CUSTOM] ddr training item */
#define SYSCTRL_DDR_TRAINING_CFG		0xa0
#define SYSCTRL_DDR_TRAINING_CFG_SEC		0xa4
/* [CUSTOM] ddr training version flag */
#define SYSCTRL_DDR_TRAINING_VERSION_FLAG	0xb4
/* [CUSTOM] ddr training stat */
#define SYSCTRL_DDR_TRAINING_STAT		0xb0

/* [CUSTOM] ddr hw training item */
#define SYSCTRL_DDR_HW_PHY0_RANK0		0x90
#define SYSCTRL_DDR_HW_PHY0_RANK1		0x94
#if 0
/* PHY1 hw training item */
#define SYSCTRL_DDR_HW_PHY1_RANK0		0x98
#define SYSCTRL_DDR_HW_PHY1_RANK1		0x9c
#endif

/* config other special */
/* [CUSTOM] DDR training start address. MEM_BASE_DDR */
#define DDRT_CFG_BASE_ADDR		0x40000000
/* [CUSTOM] SRAM start address.
NOTE: Makefile will parse it, plase define it as Hex. eg: 0xFFFF0C00 */
#define DDR_TRAINING_RUN_STACK		0x04010c00

#define DDR_RELATE_REG_DECLARE
#define DDR_TRAINING_SAVE_REG_FUNC(relate_reg, mask) \
	ddr_training_save_reg_custom(relate_reg, mask)

#define DDR_TRAINING_RESTORE_REG_FUNC(relate_reg) \
	ddr_training_restore_reg_custom(relate_reg)

struct tr_custom_reg {
	unsigned int ive_ddrt_mst_sel;
	unsigned int ddrt_clk_reg;
	unsigned int phy0_age_compst_en;
	unsigned int phy1_age_compst_en;
};
void ddr_training_save_reg_custom(void *relate_reg, unsigned int mask);
void ddr_training_restore_reg_custom(void *relate_reg);
int ddr_get_cksel(void);
#endif /* DDR_TRAINING_CUSTOM_H */
