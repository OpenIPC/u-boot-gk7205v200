/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <version.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <config.h>

#include "type.h"
#include "audio_ao.h"
#include "acodec_def.h"
#include "acodec.h"

#define IO_ADDRESS(x) (x)
//#define DEBUG_ACODEC

void *g_acodec_crg_reg = NULL;

static unsigned long ulAcodecBase;

unsigned int ACODEC_HAL_ReadReg(unsigned int u32Offset)
{
	return (*(volatile unsigned int*)((unsigned long)ulAcodecBase + (unsigned int)u32Offset));
}

void ACODEC_HAL_WriteReg(unsigned int u32Offset, unsigned int u32Value)
{
	*(volatile unsigned int*)((unsigned long)(ulAcodecBase) + (unsigned int)(u32Offset)) = u32Value;
#ifdef DEBUG_ACODEC
	printf("### fun:%s line:%d ulAcodecBase :%8lx u32Offset :%x value: %8x\n", __FUNCTION__, __LINE__, ulAcodecBase, u32Offset, u32Value);
#endif
}

void ACODEC_HAL_DUMP(unsigned int u32Offset)
{
	printf("addr: %8lx  value: %x\n",((unsigned long)ulAcodecBase + (unsigned int)u32Offset),\
			ACODEC_HAL_ReadReg( u32Offset));
}


static inline void acodec_RegWrite32(unsigned long value, unsigned long mask,
		unsigned long addr)
{
	unsigned long t;

	t = readl((const volatile void*)addr);
	t &= ~mask;
	t |= value & mask;
	writel(t, (volatile void*)addr);
}

static inline void acodec_regsetbit(
		unsigned long value,
		unsigned long offset,
		unsigned long addr)
{
	unsigned long t, mask;

	mask = 1 << offset;
	t = readl(addr);
	t &= ~mask;
	t |= (value << offset) & mask;
	writel(t, addr);
}

static inline void GK_RegRead(unsigned long *pvalue, unsigned long addr)
{
	*pvalue = readl((const volatile void *)addr);
}

static void acodec_ana_power_on(void)
{
	ACODEC_ANAREG0_U   acodec_anareg0;
	ACODEC_ANAREG1_U   acodec_anareg1;
	ACODEC_ANAREG2_U   acodec_anareg2;
	ACODEC_ANAREG3_U   acodec_anareg3;
	ACODEC_ANAREG4_U   acodec_anareg4;
	ACODEC_ANAREG5_U   acodec_anareg5;

#if 1
	acodec_anareg0.ul32 = ACODEC_ANAREG0_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);
	acodec_anareg1.ul32 = ACODEC_ANAREG1_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, acodec_anareg1.ul32);
	acodec_anareg2.ul32 = ACODEC_ANAREG2_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG2_ADDR, acodec_anareg2.ul32);
	acodec_anareg3.ul32 = ACODEC_ANAREG3_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);
	acodec_anareg4.ul32 = ACODEC_ANAREG4_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG4_ADDR, acodec_anareg4.ul32);
	acodec_anareg5.ul32 = ACODEC_ANAREG5_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG5_ADDR, acodec_anareg5.ul32);
#endif

	/************* depop *************/

	/* 1. */

	/* 2. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.rstn = 0x1;
	acodec_anareg3.bits.pd_dac_clk = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 3. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pop_rst = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 4. */

	/* 5. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_ctcm_rx = 0x0;
	acodec_anareg0.bits.pd_ctcm_tx = 0x0;
	acodec_anareg0.bits.pd_dac_vref = 0x0;
	acodec_anareg0.bits.pd_ibias = 0x0;
	acodec_anareg0.bits.pd_ldo = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 6. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pd_dacl_dff = 0x0;
	acodec_anareg3.bits.pd_dacr_dff = 0x0;
	acodec_anareg3.bits.mute_dacl = 0x0;
	acodec_anareg3.bits.mute_dacr = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 7. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pu_pop_pullb_reg = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 8. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_vref = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);
	acodec_anareg2.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG2_ADDR);
	acodec_anareg2.bits.vref_pu_pdb = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG2_ADDR, acodec_anareg2.ul32);

	udelay(20 * 1000);

	/* 9. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pop_rst = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 10. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_lineoutl = 0x0;
	acodec_anareg0.bits.pd_lineoutr = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	udelay(10 * 1000);

	/* 11. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pop_lineout_pull_en = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 12. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_micbias1 = 0x0;
	acodec_anareg0.bits.pd_micbias2 = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 13. */
	acodec_anareg1.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG1_ADDR);
	acodec_anareg1.bits.pd_rctune = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, acodec_anareg1.ul32);

	/* 14. */
	acodec_anareg1.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG1_ADDR);
	acodec_anareg1.bits.en_rctune = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, acodec_anareg1.ul32);

	udelay(1 * 1000);

	/* 15. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_linein_l = 0x0;
	acodec_anareg0.bits.pd_linein_r = 0x0;
	acodec_anareg0.bits.pd_adcl = 0x0;
	acodec_anareg0.bits.pd_adcr = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 16. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.rstb_dac = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

}

static void acodec_ana_power_down(void)
{
	int i = 0;
	unsigned int value = 0x0;
	ACODEC_ANAREG0_U   acodec_anareg0;
	ACODEC_ANAREG1_U   acodec_anareg1;
	ACODEC_ANAREG2_U   acodec_anareg2;
	ACODEC_ANAREG3_U   acodec_anareg3;
	ACODEC_ANAREG4_U   acodec_anareg4;
	ACODEC_ANAREG5_U   acodec_anareg5;

	/************* depop *************/

	/* 1. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_linein_l = 0x1;
	acodec_anareg0.bits.pd_linein_r = 0x1;
	acodec_anareg0.bits.pd_adcl = 0x1;
	acodec_anareg0.bits.pd_adcr = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 2. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_micbias1 = 0x1;
	acodec_anareg0.bits.pd_micbias2 = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 3. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.rstb_dac = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 4. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pop_lineout_pull_en = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 5. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_lineoutl = 0x1;
	acodec_anareg0.bits.pd_lineoutr = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 6. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_dac_vref = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	/* 7. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_vref = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	value = 0x800;
	for (i = 0; i < 12; i++)
	{
		acodec_anareg4.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG4_ADDR);
		acodec_anareg4.bits.vref_pd_res_sel = acodec_anareg4.bits.vref_pd_res_sel | value;
		ACODEC_HAL_WriteReg(ACODEC_ANAREG4_ADDR, acodec_anareg4.ul32);
		value = value >> 1;
		udelay(1 * 1000);
	}

	acodec_anareg2.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG2_ADDR);
	acodec_anareg2.bits.vref_pu_pdb = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG2_ADDR, acodec_anareg2.ul32);

	/* 8. */
	value = 0x80;
	for (i = 0; i < 8; i++)
	{
		acodec_anareg4.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG4_ADDR);
		acodec_anareg4.bits.pop_res_sel = acodec_anareg4.bits.pop_res_sel | value;
		ACODEC_HAL_WriteReg(ACODEC_ANAREG4_ADDR, acodec_anareg4.ul32);
		value = value >> 1;
		udelay(1 * 1000);
	}

	/* 9. */
	acodec_anareg3.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG3_ADDR);
	acodec_anareg3.bits.pu_pop_pullb_reg = 0x0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	/* 10. */
	acodec_anareg0.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG0_ADDR);
	acodec_anareg0.bits.pd_ctcm_rx = 0x1;
	acodec_anareg0.bits.pd_ctcm_tx = 0x1;
	acodec_anareg0.bits.pd_ibias = 0x1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

#if 1
	acodec_anareg0.ul32 = ACODEC_ANAREG0_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);
	acodec_anareg1.ul32 = ACODEC_ANAREG1_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, acodec_anareg1.ul32);
	acodec_anareg2.ul32 = ACODEC_ANAREG2_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG2_ADDR, acodec_anareg2.ul32);
	acodec_anareg3.ul32 = ACODEC_ANAREG3_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);
	acodec_anareg4.ul32 = ACODEC_ANAREG4_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG4_ADDR, acodec_anareg4.ul32);
	acodec_anareg5.ul32 = ACODEC_ANAREG5_DEFAULT;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG5_ADDR, acodec_anareg5.ul32);
#endif
}


static int acodec_soft_reset(void)
{
	ACODEC_DIGCTRL1_U   acodec_digctrl1;
	ACODEC_DIGCTRL2_U   acodec_digctrl2;
	ACODEC_DIGCTRL3_U   acodec_digctrl3;
	ACODEC_DIGCTRL4_U   acodec_digctrl4;
	ACODEC_DIGCTRL5_U   acodec_digctrl5;

	ACODEC_ANAREG0_U   acodec_anareg0;
	ACODEC_ANAREG1_U   acodec_anareg1;
	ACODEC_ANAREG2_U   acodec_anareg2;
	ACODEC_ANAREG3_U   acodec_anareg3;
	ACODEC_ANAREG4_U   acodec_anareg4;
	ACODEC_ANAREG5_U   acodec_anareg5;

	acodec_anareg0.ul32 = 0x1C1C0000;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG0_ADDR, acodec_anareg0.ul32);

	acodec_anareg1.ul32 = 0xDF605E66;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, acodec_anareg1.ul32);

	acodec_anareg2.ul32 = 0x00255544;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG2_ADDR, acodec_anareg2.ul32);

	acodec_anareg3.ul32 = 0x072D0451;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG3_ADDR, acodec_anareg3.ul32);

	acodec_anareg4.ul32 = 0x00000000;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG4_ADDR, acodec_anareg4.ul32);

	acodec_anareg5.ul32 = 0x00000000;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG5_ADDR, acodec_anareg5.ul32);

	acodec_digctrl1.ul32 = 0xff035a00;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL1_ADDR, acodec_digctrl1.ul32);

	acodec_digctrl2.ul32 = 0x08000001;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL2_ADDR, acodec_digctrl2.ul32);

	acodec_digctrl3.ul32 = 0x06062424;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL3_ADDR, acodec_digctrl3.ul32);

	acodec_digctrl4.ul32 = 0x1e1ec001;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL4_ADDR, acodec_digctrl4.ul32);

	acodec_digctrl5.ul32 = 0x24242424;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL5_ADDR, acodec_digctrl5.ul32);

	return 0;
}

static int acodec_geti2sfs(ACODEC_FS_E enAcodecFs)
{
	switch(enAcodecFs)
	{
		case ACODEC_FS_8000:
		case ACODEC_FS_11025:
		case ACODEC_FS_12000:
			return ACODEC_I2S_FS_8000;
		case ACODEC_FS_16000:
		case ACODEC_FS_22050:
		case ACODEC_FS_24000:
			return ACODEC_I2S_FS_16000;
		case ACODEC_FS_32000:
		case ACODEC_FS_44100:
		case ACODEC_FS_48000:
			return ACODEC_I2S_FS_32000;
		case ACODEC_FS_64000:
		case ACODEC_FS_96000:
			return ACODEC_I2S_FS_64000;
		default:
			printf("Unsupport samplerate %d.\n", enAcodecFs);
			return ACODEC_I2S_FS_BUTT;
	}
}

static int acodec_getadcmodesel(ACODEC_FS_E enAcodecFs)
{
	switch(enAcodecFs)
	{
		case ACODEC_FS_8000:
		case ACODEC_FS_16000:
		case ACODEC_FS_32000:
		case ACODEC_FS_64000:
			return ACODEC_ADC_MODESEL_4096;
		case ACODEC_FS_11025:
		case ACODEC_FS_12000:
		case ACODEC_FS_22050:
		case ACODEC_FS_24000:
		case ACODEC_FS_44100:
		case ACODEC_FS_48000:
		case ACODEC_FS_96000:
			return ACODEC_ADC_MODESEL_6144;
		default:
			printf("Unsupport samplerate %d.\n", enAcodecFs);
			return ACODEC_I2S_FS_BUTT;
	}
}

static ACODEC_FS_E acodec_getacodecfs(AUDIO_SAMPLE_RATE_E enSample)
{
	ACODEC_FS_E enAcodecFs = ACODEC_FS_BUTT;
	switch(enSample)
	{
		case AUDIO_SAMPLE_RATE_8000:
			enAcodecFs = ACODEC_FS_8000;
			break;

		case AUDIO_SAMPLE_RATE_11025:
			enAcodecFs = ACODEC_FS_11025;
			break;

		case AUDIO_SAMPLE_RATE_12000:
			enAcodecFs = ACODEC_FS_12000;
			break;

		case AUDIO_SAMPLE_RATE_16000:
			enAcodecFs = ACODEC_FS_16000;
			break;

		case AUDIO_SAMPLE_RATE_22050:
			enAcodecFs = ACODEC_FS_22050;
			break;

		case AUDIO_SAMPLE_RATE_24000:
			enAcodecFs = ACODEC_FS_24000;
			break;

		case AUDIO_SAMPLE_RATE_32000:
			enAcodecFs = ACODEC_FS_32000;
			break;

		case AUDIO_SAMPLE_RATE_44100:
			enAcodecFs = ACODEC_FS_44100;
			break;

		case AUDIO_SAMPLE_RATE_48000:
			enAcodecFs = ACODEC_FS_48000;
			break;

		default:
			printf("Unsupport samplerate %d.\n", enSample);
			break;
	}

	return enAcodecFs;
}


int acodec_i2s_set(AUDIO_SAMPLE_RATE_E enSample)
{
	ACODEC_FS_E enAcodecFs = ACODEC_FS_BUTT;
	ACODEC_DIGCTRL1_U   unDigctrl1;
	ACODEC_ANAREG1_U    unAnaReg1;

	enAcodecFs = acodec_getacodecfs(enSample);
	if(enAcodecFs == ACODEC_FS_BUTT)
	{
		printf("%s: not support enSample:%d.\n", __FUNCTION__, enSample);
		return -1;
	}

	unDigctrl1.ul32 = ACODEC_HAL_ReadReg(ACODEC_DIGCTRL1_ADDR);
	unDigctrl1.bits.i2s1_fs_sel = acodec_geti2sfs(enAcodecFs);
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL1_ADDR, unDigctrl1.ul32);

	unAnaReg1.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG1_ADDR);
	unAnaReg1.bits.mode_adcr = acodec_getadcmodesel(enAcodecFs);
	unAnaReg1.bits.mode_adcl = acodec_getadcmodesel(enAcodecFs);
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, unAnaReg1.ul32);

	//rctune
	unAnaReg1.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG1_ADDR);
	unAnaReg1.bits.en_rctune = 0;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, unAnaReg1.ul32);

	udelay(30);
	unAnaReg1.ul32 = ACODEC_HAL_ReadReg(ACODEC_ANAREG1_ADDR);
	unAnaReg1.bits.en_rctune = 1;
	ACODEC_HAL_WriteReg(ACODEC_ANAREG1_ADDR, unAnaReg1.ul32);

	return 0;
}

int acodec_device_init(void)
{
	AUDIO_REG_1_U       acodec_audio_reg;
	ACODEC_DIGCTRL1_U   acodec_digctrl1;
	ACODEC_DIGCTRL2_U   acodec_digctrl2;
	ACODEC_DIGCTRL3_U   acodec_digctrl3;

	unsigned int aiao_crg = 0;
	unsigned int aiao_cfg = 0;

	ulAcodecBase = (unsigned int)IO_ADDRESS(ACODEC_REGS_BASE);
	if (ulAcodecBase == 0)
	{
		printf("could not ioremap acodec regs!");
		return -1;
	}

	g_acodec_crg_reg = (void *)IO_ADDRESS(ACODEC_REGS_CRG);
	if (g_acodec_crg_reg == NULL)
	{
		printf("could not ioremap acodec regs!");
		return -1;
	}

	udelay(1 * 1000);

	// audio crg
	acodec_regsetbit(1, 1, (unsigned long)((gk_uintptr_t)g_acodec_crg_reg));
	acodec_regsetbit(1, 2, (unsigned long)((gk_uintptr_t)g_acodec_crg_reg));

	// aiao clk
	aiao_crg = (unsigned int)IO_ADDRESS(AIAO_CLK_TX0_CRG);
	*(volatile unsigned int*)((unsigned long)(aiao_crg)) = 0x00152EF0;
	aiao_cfg = (unsigned int)IO_ADDRESS(AIAO_CLK_TX0_CFG);
	*(volatile unsigned int*)((unsigned long)(aiao_cfg)) = 0x00000115;

	udelay(50 * 1000);

	// acodec mux
	acodec_audio_reg.ul32 = 0xf;
	ACODEC_HAL_WriteReg(ACODEC_AUDIO_REG, acodec_audio_reg.ul32);

	// digctrl
	acodec_digctrl1.ul32 = 0xff035a00;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL1_ADDR, acodec_digctrl1.ul32);

	acodec_digctrl2.ul32 = 0x08000001;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL2_ADDR, acodec_digctrl2.ul32);

	acodec_digctrl3.ul32 = 0x7e7e2424;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL3_ADDR, acodec_digctrl3.ul32);

	udelay(1 * 1000);

	acodec_ana_power_on();

	udelay(1 * 1000);

	acodec_soft_reset();

	printf("acodec inited!\n");

	return 0;
}

int acodec_device_exit(void)
{
	ACODEC_DIGCTRL3_U   acodec_digctrl3;

	unsigned int aiao_crg = 0;
	unsigned int aiao_cfg = 0;

	acodec_soft_reset();

	udelay(1 * 1000);

	acodec_ana_power_down();

	// digctrl
	acodec_digctrl3.ul32 = 0x7f7f2424;
	ACODEC_HAL_WriteReg(ACODEC_DIGCTRL3_ADDR, acodec_digctrl3.ul32);

	// audio crg
	acodec_regsetbit(0, 1, (unsigned long)((gk_uintptr_t)g_acodec_crg_reg));
	acodec_regsetbit(0, 2, (unsigned long)((gk_uintptr_t)g_acodec_crg_reg));

	// aiao clk
	aiao_crg = (unsigned int)IO_ADDRESS(AIAO_CLK_TX0_CRG);
	*(volatile unsigned int*)((unsigned long)(aiao_crg)) = 0x00152EF0;
	aiao_cfg = (unsigned int)IO_ADDRESS(AIAO_CLK_TX0_CFG);
	*(volatile unsigned int*)((unsigned long)(aiao_cfg)) = 0x00000115;

	printf("acodec exited!\n");
	return 0;
}
