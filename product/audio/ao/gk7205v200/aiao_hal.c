/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <common.h>
#include <command.h>
#include <version.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
	//#include <asm/sizes.h>
#include <config.h>

#include "type.h"
#include "board.h"
#include "aiao_hal.h"
#include "aiao_reg.h"
#include "aiao_ext.h"

#define IO_ADDRESS(x) (x)
	//#define DEBUG_AO

	static AIO_STATE_S s_stAioState[AIO_MAX_NUM];
	static GK_UL u32AioBase;
	static GK_UL u32CrgBase;
	void *g_acodec_ctrl0_reg = NULL;
	void *g_acodec_fast_reg = NULL;

#define AIAO_HAL_WRITE_REG(u32Offset, value) \
	(*(volatile GK_U32*)((GK_UL)u32AioBase + (GK_U32)u32Offset) = (value))

#define AIAO_HAL_READ_REG(u32Offset) \
	(*(volatile GK_U32*)((GK_UL)u32AioBase + (GK_U32)u32Offset))

#define AIAO_HAL_WRITE_CRG_REG(u32RegAddr, value) \
	(*(volatile GK_U32*)((GK_UL)u32CrgBase + (GK_U32)(u32RegAddr - CRG_REGS_ADDR)) = (value))

#define AIAO_HAL_READ_CRG_REG(u32RegAddr) \
	(*(volatile GK_U32*)((GK_UL)u32CrgBase + (GK_U32)(u32RegAddr - CRG_REGS_ADDR)))

	GK_U32 AIAO_HAL_ReadReg(GK_U32 u32Offset)
	{
		return (*(volatile GK_U32*)((GK_UL)u32AioBase + (GK_U32)u32Offset));
	}

	GK_VOID AIAO_HAL_WriteReg(GK_U32 u32Offset, GK_U32 u32Value)
	{
		*(volatile GK_U32*)((GK_UL)(u32AioBase) + (GK_U32)(u32Offset)) = u32Value;
	}

	static inline void acodec_Ctr0setbit(
			unsigned long value,
			unsigned long offset,
			unsigned long addr)
	{
		unsigned long t, mask;

		mask = 1 << offset;
		t = readl((GK_U32*)((gk_uintptr_t)addr));
		t &= ~mask;
		t |= (value << offset) & mask;
		writel(t, (GK_U32*)((gk_uintptr_t)addr));
	}

	GK_S32 AIAO_HAL_SysInit(GK_VOID)
	{
		u32AioBase = (GK_U32)IO_ADDRESS(AIAO_REG_BASE);
		if (0 == u32AioBase)
		{
			return GK_FAILURE;
		}

		u32CrgBase = (GK_U32)IO_ADDRESS(CRG_REGS_ADDR);
		if (0 == u32CrgBase)
		{
			return GK_FAILURE;
		}

		return GK_SUCCESS;
	}

	GK_S32 AIAO_HAL_CheckAiAoClk(const AIO_ATTR_S *pstAttr0, const AIO_ATTR_S *pstAttr1)
	{
		GK_U32 u32ChnCnt0,u32ChnCnt1;

		if(1 == pstAttr0->u32ChnCnt)
		{
			u32ChnCnt0 = 2;
		}
		else
		{
			u32ChnCnt0 = pstAttr0->u32ChnCnt;
		}

		if(1 == pstAttr1->u32ChnCnt)
		{
			u32ChnCnt1 = 2;
		}
		else
		{
			u32ChnCnt1 = pstAttr1->u32ChnCnt;
		}

		if ((u32ChnCnt0 * AIO_GET_BITCNT(pstAttr0->enBitwidth))
				!= (u32ChnCnt1 * AIO_GET_BITCNT(pstAttr1->enBitwidth)))
		{
			printf("(chncnt*bitwidth)Fs of AI and AO should be same when u32ClkSel=1\n");
			return GK_FAILURE;
		}

		if (pstAttr0->enSamplerate != pstAttr1->enSamplerate)
		{
			printf("samplerate of AI and AO should be same when u32ClkSel=1\n");
			return GK_FAILURE;
		}

		return GK_SUCCESS;
	}

	static GK_S32 AIAOGetMclkCfg(AUDIO_DEV AudioDev, const AIO_ATTR_S *pstAttr,
			GK_S32 *ps32MClkSel, GK_BOOL *pbMclkSetted)
	{

		*pbMclkSetted = GK_FALSE;

		switch (pstAttr->enSamplerate)
		{
			case AUDIO_SAMPLE_RATE_8000:
			case AUDIO_SAMPLE_RATE_16000:
			case AUDIO_SAMPLE_RATE_32000:
				{
					if ((AUDIO_BIT_WIDTH_24 == pstAttr->enBitwidth)
							||((AUDIO_BIT_WIDTH_16 == pstAttr->enBitwidth)&&(2 == pstAttr->u32EXFlag)))
					{
						*ps32MClkSel  = AIO_MCLK_48K_1800;
					}
					else
					{
						*ps32MClkSel  = AIO_MCLK_32K_1800;
					}
					break;
				}

			case AUDIO_SAMPLE_RATE_12000:
			case AUDIO_SAMPLE_RATE_24000:
			case AUDIO_SAMPLE_RATE_48000:
				{
					*ps32MClkSel  = AIO_MCLK_48K_1800;
					break;
				}

			case AUDIO_SAMPLE_RATE_11025:
			case AUDIO_SAMPLE_RATE_22050:
			case AUDIO_SAMPLE_RATE_44100:
				{
					*ps32MClkSel  = AIO_MCLK_441K_1800;
					break;
				}
			default:
				{
					printf( "not support this sample rate \n");
					return -1;
				}
		}

		return GK_SUCCESS;
	}

	static GK_S32 AIAOGetBclkFsclkDivCfg(const AIO_ATTR_S *pstAttr,
			GK_S32 s32MClkSel, GK_S32 *ps32BClkSel, GK_S32 *ps32LrClkSel)
	{
		GK_U32 u32MclkRateNum;
		GK_U32 u32FsBit;

		if(1 == pstAttr->u32ChnCnt)
		{
			if ((AUDIO_BIT_WIDTH_16 == pstAttr->enBitwidth)&&(2 == pstAttr->u32EXFlag))
			{
				u32FsBit = 2 * AIO_GET_BITCNT(AUDIO_BIT_WIDTH_24);
			}
			else
			{
				u32FsBit = 2 * AIO_GET_BITCNT(pstAttr->enBitwidth);
			}
		}
		else
		{
			if ((AUDIO_BIT_WIDTH_16 == pstAttr->enBitwidth)&&(2 == pstAttr->u32EXFlag))
			{
				u32FsBit = pstAttr->u32ChnCnt * AIO_GET_BITCNT(AUDIO_BIT_WIDTH_24);
			}
			else
			{
				u32FsBit = pstAttr->u32ChnCnt * AIO_GET_BITCNT(pstAttr->enBitwidth);
			}
		}
		switch (s32MClkSel)
		{
			case AIO_MCLK_48K_1800:
				{
					u32MclkRateNum = 48000 * 256;
					break;
				}

			case AIO_MCLK_32K_1800:
				{
					u32MclkRateNum = 32000 * 256;
					break;
				}

			case AIO_MCLK_441K_1800:
				{
					u32MclkRateNum = 44100 * 256;
					break;
				}

			default:
				{
					printf("not support this mclk\n");
					return GK_FAILURE;
				}
		}

		if (0 != (u32MclkRateNum % (u32FsBit * pstAttr->enSamplerate)))
		{

			printf( "can not get the bclk_division ratio\n");
			return GK_FAILURE;
		}

		switch (u32MclkRateNum / (u32FsBit * pstAttr->enSamplerate))
		{

			case 1:
				*ps32BClkSel = SYS_AIO_BS_CLK1;
				break;
			case 2:
				*ps32BClkSel = SYS_AIO_BS_CLK2;
				break;
			case 3:
				*ps32BClkSel = SYS_AIO_BS_CLK3;
				break;
			case 4:
				*ps32BClkSel = SYS_AIO_BS_CLK4;
				break;
			case 6:
				*ps32BClkSel = SYS_AIO_BS_CLK6;
				break;
			case 8:
				*ps32BClkSel = SYS_AIO_BS_CLK8;
				break;
			case 12:
				*ps32BClkSel = SYS_AIO_BS_CLK12;
				break;
			case 16:
				*ps32BClkSel = SYS_AIO_BS_CLK16;
				break;
			case 24:
				*ps32BClkSel = SYS_AIO_BS_CLK24;
				break;
			case 32:
				*ps32BClkSel = SYS_AIO_BS_CLK32;
				break;
			case 48:
				*ps32BClkSel = SYS_AIO_BS_CLK48;
				break;
			case 64:
				*ps32BClkSel = SYS_AIO_BS_CLK64;
				break;
			default:
				printf("not support this bclk_division ratio\n");
				return GK_FAILURE;
		}

		switch (u32FsBit)
		{
			case 256:
				{
					*ps32LrClkSel = SYS_AIO_SAMPLE_CLK256;
					break;
				}
			case 128:
				{
					*ps32LrClkSel = SYS_AIO_SAMPLE_CLK128;
					break;
				}
			case 64:
				{
					*ps32LrClkSel = SYS_AIO_SAMPLE_CLK64;
					break;
				}
			case 32:
				{
					*ps32LrClkSel = SYS_AIO_SAMPLE_CLK32;
					break;
				}
			case 16:
				{
					*ps32LrClkSel = SYS_AIO_SAMPLE_CLK16;
					break;
				}
			default:
				{
					printf("not support this fsclk_division ratio\n");
					return GK_FAILURE;
				}
		}

		return GK_SUCCESS;
	}

	GK_VOID AOP_HAL_IntEn(GK_U32 u32ChnId, GK_BOOL bEn)
	{
		U_AIAO_INT_ENA unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf( "AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_READ_REG(AIAO_INT_ENA_REG);

		switch (u32ChnId)
		{
			case 0:
				unTmp.bits.tx_ch0_int_ena = bEn;
				break;
#if 0
			case 1:
				unTmp.bits.tx_ch1_int_ena = bEn;
#endif
				break;
			default:
				break;
		}

		AIAO_HAL_WRITE_REG(AIAO_INT_ENA_REG, unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d AIAO_INT_ENA_REG = 0x%0x \n",__func__, __LINE__,  unTmp.u32);
#endif
	}


	GK_VOID AOP_HAL_SetBufferAddr(GK_U32 u32ChnId, GK_U32 u32Value)
	{
		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		AIAO_HAL_WriteReg(AOP_BUFF_SADDR_REG(u32ChnId), u32Value);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_BUFF_SADDR_REG(u32ChnId), u32Value);
#endif
	}

	GK_VOID AOP_HAL_SetBufferSize(GK_U32 u32ChnId, GK_U32 u32Value)
	{
		U_TX_BUFF_SIZE unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_ReadReg(AOP_BUFF_SIZE_REG(u32ChnId));

		unTmp.bits.tx_buff_size = u32Value;

		AIAO_HAL_WriteReg(AOP_BUFF_SIZE_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_BUFF_SIZE_REG(u32ChnId), unTmp.u32);
#endif
	}

	GK_VOID AOP_HAL_SetBuffWptr(GK_U32 u32ChnId, GK_U32 u32Value)
	{
		U_TX_BUFF_WPTR unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_ReadReg(AOP_BUFF_WPTR_REG(u32ChnId));

		unTmp.bits.tx_buff_wptr = u32Value;

		AIAO_HAL_WriteReg(AOP_BUFF_WPTR_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_BUFF_WPTR_REG(u32ChnId), unTmp.u32);
#endif
	}

	GK_VOID AOP_HAL_SetBuffRptr(GK_U32 u32ChnId, GK_U32 u32Value)
	{
		U_TX_BUFF_RPTR unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_ReadReg(AOP_BUFF_RPTR_REG(u32ChnId));

		unTmp.bits.tx_buff_rptr = u32Value;

		AIAO_HAL_WriteReg(AOP_BUFF_RPTR_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_BUFF_RPTR_REG(u32ChnId), unTmp.u32);
#endif
	}


	GK_VOID AOP_HAL_SetTransSize(GK_U32 u32ChnId, GK_U32 u32Value)
	{
		U_TX_TRANS_SIZE unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_ReadReg(AOP_TRANS_SIZE_REG(u32ChnId));

		unTmp.bits.tx_trans_size = u32Value;

		AIAO_HAL_WriteReg(AOP_TRANS_SIZE_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_TRANS_SIZE_REG(u32ChnId), unTmp.u32);
#endif
	}

	GK_VOID AOP_HAL_SetTxStart(GK_U32 u32ChnId, GK_BOOL bEn)
	{
		U_TX_DSP_CTRL unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_READ_REG(AOP_CTRL_REG(u32ChnId));

		unTmp.bits.tx_enable = bEn;

		AIAO_HAL_WRITE_REG(AOP_CTRL_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_CTRL_REG(u32ChnId), unTmp.u32);
#endif
	}

	GK_U32 AOP_HAL_GetDisDone(GK_U32 u32ChnId)
	{
		U_TX_DSP_CTRL unTmp;

		GK_U32 u32Status = 0;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_ReadReg(AOP_CTRL_REG(u32ChnId));

		u32Status = unTmp.bits.tx_disable_done;

		return u32Status;
	}

	GK_VOID AOP_HAL_SetChildIntMask(GK_U32 u32ChnId)
	{
		U_TX_INT_ENA unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_READ_REG(AOP_INT_ENA_REG(u32ChnId));

		unTmp.bits.tx_trans_int_ena       = 1;
		unTmp.bits.tx_empty_int_ena       = 1;
		unTmp.bits.tx_alempty_int_ena     = 0;
		unTmp.bits.tx_bfifo_empty_int_ena = 1;
		unTmp.bits.tx_ififo_empty_int_ena = 1;
		unTmp.bits.tx_stop_int_ena        = 0;
		unTmp.bits.tx_mfade_int_ena       = 0;
		unTmp.bits.tx_dat_break_int_ena   = 0;
		unTmp.bits.Reserved_0             = 0;
		AIAO_HAL_WRITE_REG(AOP_INT_ENA_REG(u32ChnId), unTmp.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_INT_ENA_REG(u32ChnId), unTmp.u32);
#endif
	}

	GK_VOID AOP_HAL_ClrChildIntAllStatus(GK_U32 u32ChnId)
	{
		U_TX_INT_CLR u32IntMask;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			//return GK_FAILURE;
		}

		u32IntMask.bits.tx_trans_int_clear       = 1;
		u32IntMask.bits.tx_empty_int_clear       = 1;
		u32IntMask.bits.tx_alempty_int_clear     = 1;
		u32IntMask.bits.tx_bfifo_empty_int_clear = 1;
		u32IntMask.bits.tx_ififo_empty_int_clear = 1;
		u32IntMask.bits.tx_stop_int_clear        = 1;
		u32IntMask.bits.tx_mfade_int_clear       = 1;
		u32IntMask.bits.tx_dat_break_int_clear   = 1;

		AIAO_HAL_WRITE_REG(AOP_INT_CLR_REG(u32ChnId), u32IntMask.u32);

	}
	/**************/
	GK_S32 AOP_HAL_SetVolume(GK_U32 u32ChnId, GK_S32 s32VolumeDb)
	{
		U_TX_DSP_CTRL unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			return GK_FAILURE;
		}

		/*0x7F->6db 0x7E->5db 0x29->-80db 0~0x28 mute */
		if(-121 > s32VolumeDb || 6 < s32VolumeDb)
		{
			printf("AoDev%u volume %d error\n", u32ChnId,s32VolumeDb);
			return GK_FAILURE;
		}

		unTmp.u32 = AIAO_HAL_READ_REG(AOP_CTRL_REG(u32ChnId));

		unTmp.bits.volume = 0x7F -(6 -s32VolumeDb);

		AIAO_HAL_WRITE_REG(AOP_CTRL_REG(u32ChnId), unTmp.u32);

		return GK_SUCCESS;
	}
#if 0
	GK_S32 AOP_HAL_SetMute(GK_U32 u32ChnId, GK_BOOL bMute,  AUDIO_FADE_S *pstFade)
	{
		U_TX_DSP_CTRL unTmp;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%d is invalid!\n", u32ChnId);
			return GK_FAILURE;
		}

		if((GK_TRUE != bMute) && (GK_FALSE != bMute))
		{
			printf("AoDev%d set mute param error\n", u32ChnId);
			return GK_FAILURE;
		}

		if(GK_NULL != pstFade)
		{
			if((GK_TRUE != pstFade->bFade) && (GK_FALSE != pstFade->bFade))
			{
				printf("AoDev%d set mute param error\n", u32ChnId);
				return GK_FAILURE;
			}

			if((AUDIO_FADE_RATE_BUTT <= pstFade->enFadeInRate) || ( AUDIO_FADE_RATE_BUTT <= pstFade->enFadeOutRate))
			{
				printf("AoDev%d set mute param error\n", u32ChnId);
				return GK_FAILURE;
			}
		}

		unTmp.u32 = AIAO_HAL_READ_REG(AOP_CTRL_REG(u32ChnId));

		unTmp.bits.mute_en = bMute;

		if(GK_NULL != pstFade)
		{
			unTmp.bits.mute_fade_en  = pstFade->bFade;
			unTmp.bits.fade_in_rate  = pstFade->enFadeInRate;
			unTmp.bits.fade_out_rate = pstFade->enFadeOutRate;
		}

		AIAO_HAL_WRITE_REG(AOP_CTRL_REG(u32ChnId), unTmp.u32);

		return GK_SUCCESS;
	}
#endif
	static GK_S32 AOPSetSysCtl(AUDIO_DEV AudioDevId, AIO_TYPE_E enAioType, const AIO_ATTR_S *pstAttr)
	{
		GK_S32 s32MClkSel = 0, s32LrClkSel = 0, s32BClkSel = 0;
		GK_BOOL bMclkSetted;
		GK_S32 s32Ret = GK_SUCCESS;

		U_I2S_CRG_CFG0 I2SCrg_cfg0;
		U_I2S_CRG_CFG1 I2SCrg_cfg1;
		U_AIAO_SWITCH_TX_BCLK switch_tx_bclk;

		I2SCrg_cfg1.u32 = AIAO_HAL_READ_REG(AOP_I2S_REG_CFG1(AudioDevId));
#ifdef DEBUG_AO
		printf("%s %d read 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_I2S_REG_CFG1(AudioDevId), I2SCrg_cfg1.u32);
#endif
		/* get clock value */
		s32Ret = AIAOGetMclkCfg(AudioDevId, pstAttr, &s32MClkSel, &bMclkSetted);
		s32Ret |= AIAOGetBclkFsclkDivCfg(pstAttr, s32MClkSel, &s32BClkSel, &s32LrClkSel);
		if (GK_SUCCESS != s32Ret)
		{
			return GK_FAILURE;
		}

		I2SCrg_cfg0.bits.aiao_mclk_div = s32MClkSel;
		I2SCrg_cfg0.bits.Reserved_0    = 0;
		AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG0(AudioDevId),I2SCrg_cfg0.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_I2S_REG_CFG0(AudioDevId), I2SCrg_cfg0.u32);
#endif
		I2SCrg_cfg1.bits.aiao_bclk_div      = s32BClkSel;
		I2SCrg_cfg1.bits.aiao_fsclk_div     = s32LrClkSel;

		if(AIO_IS_MASTER_MODE(pstAttr->enWorkmode))
		{
			I2SCrg_cfg1.bits.aiao_bclk_oen = 0;
			I2SCrg_cfg1.bits.aiao_bclk_sel = 0;
		}
		else if(AIO_IS_SLAVE_MODE(pstAttr->enWorkmode))
		{
			I2SCrg_cfg1.bits.aiao_bclk_oen = 1;
			I2SCrg_cfg1.bits.aiao_bclk_sel = 1;
		}
		else
		{
			printf( "invalid aio workmode:%d\n", pstAttr->enWorkmode);
			return GK_FAILURE;
		}

		I2SCrg_cfg1.bits.aiao_cken          = 1;
		I2SCrg_cfg1.bits.aiao_srst_req      = 0;
		//I2SCrg_cfg1.bits.aiao_bclk_oen      = 0;
		//I2SCrg_cfg1.bits.aiao_bclk_sel      = 0;
		I2SCrg_cfg1.bits.aiao_bclkin_pctrl  = 0;
		I2SCrg_cfg1.bits.aiao_bclkout_pctrl = 0;

		AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG1(AudioDevId),I2SCrg_cfg1.u32 );
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_I2S_REG_CFG1(AudioDevId), I2SCrg_cfg1.u32);
#endif

		switch_tx_bclk.u32 = AIAO_HAL_READ_REG(AOP_SWITCH_TX_BCLK);
		switch(AudioDevId)
		{
			case 0:
				{
					switch_tx_bclk.bits.inner_bclk_ws_sel_tx_00 = 0x8;
					break;
				}
#if 0
			case 1:
				{
					switch_tx_bclk.bits.inner_bclk_ws_sel_tx_01 = 0x9;
					break;
				}
#endif
			default:
				{
					printf("invalid ao dev:%d\n",AudioDevId);
					return GK_FAILURE;
				}
		}
		AIAO_HAL_WRITE_REG(AOP_SWITCH_TX_BCLK, switch_tx_bclk.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x2065002c switch_tx_bclk.u32:0x%0x \n",__func__, __LINE__, switch_tx_bclk.u32);
#endif

		return s32Ret;
	}


	GK_S32 CHECK_AO_ATTR(const AIO_ATTR_S *pAttr)
	{
		if (AIO_IS_MASTER_MODE(pAttr->enWorkmode))
		{
			if (pAttr->enSamplerate != AUDIO_SAMPLE_RATE_8000\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_12000\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_11025\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_16000\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_22050\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_24000\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_32000\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_44100\
					&& pAttr->enSamplerate != AUDIO_SAMPLE_RATE_48000)
			{
				printf( "invalid enSamplerate\n");
				return GK_FAILURE;
			}
		}

		if ( pAttr->u32ChnCnt != 1 && pAttr->u32ChnCnt != 2 )
		{
			printf( "invalid u32ChnCnt:%u\n", pAttr->u32ChnCnt);
			return GK_FAILURE;
		}

		if(1 == pAttr->u32ChnCnt && AUDIO_SOUND_MODE_STEREO == pAttr->enSoundmode)
		{
			printf("when chn=1,can't use STEREO mode\n");
			return GK_FAILURE;
		}

		if ( pAttr->enBitwidth != AUDIO_BIT_WIDTH_16)
		{
			printf("invalid enBitwidth:%d\n",pAttr->enBitwidth);
			return GK_FAILURE;
		}

		if (AIO_IS_I2S_MODE(pAttr->enWorkmode))
		{
			if (pAttr->u32ChnCnt * AIO_GET_BITCNT(pAttr->enBitwidth) > AIO_ONE_FIFO_BITWIDTH*2)
			{
				printf("I2S mode, not support chncnt:%u * bitwidth:%dbit \n",
						pAttr->u32ChnCnt, AIO_GET_BITCNT(pAttr->enBitwidth));
				return GK_FAILURE;
			}

		}
		else if (AIO_IS_PCM_MODE(pAttr->enWorkmode))
		{
			if (pAttr->u32ChnCnt * AIO_GET_BITCNT(pAttr->enBitwidth) > 256)
			{
				printf( "PCM mode, not support chncnt:%u * bitwidth:%dbit \n",
						pAttr->u32ChnCnt, AIO_GET_BITCNT(pAttr->enBitwidth));
				return GK_FAILURE;
			}
		}

		if (pAttr->enWorkmode < 0 || pAttr->enWorkmode >= AIO_MODE_BUTT)
		{
			printf( "invalid enWorkmode\n");
			return GK_FAILURE;
		}
		if (pAttr->enSoundmode < 0 || pAttr->enSoundmode >= AUDIO_SOUND_MODE_BUTT)
		{
			printf("invalid enSoundmode\n");
			return GK_FAILURE;
		}
		if (pAttr->u32FrmNum > MAX_AUDIO_FRAME_NUM || pAttr->u32FrmNum < 2)
		{
			printf("invalid u32FrmNum\n");
			return GK_FAILURE;
		}
		if (pAttr->u32PtNumPerFrm > MAX_AO_POINT_NUM || pAttr->u32PtNumPerFrm < MIN_AUDIO_POINT_NUM)
		{
			printf("u32PtNumPerFrm %u is invalid!\n", pAttr->u32PtNumPerFrm);
			return GK_FAILURE;
		}
		if (pAttr->u32ClkSel != 0 && pAttr->u32ClkSel != 1)
		{
			printf("invalid u32ClkSel\n");
			return GK_FAILURE;
		}

		return GK_SUCCESS;
	}


	GK_S32 AOP_HAL_SetDevAttr(GK_U32 u32ChnId, AIO_ATTR_S *pstAttr)
	{
		GK_S32 s32Ret;
		AIO_ATTR_S *pstAiAttr = NULL;
		U_TX_IF_ATTRI stAopAttrReg;
		U_TX_DSP_CTRL   stAopCtrlReg;

		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%u is invalid!\n", u32ChnId);
			return GK_FAILURE;
		}

		/* check AO attr */
		s32Ret = CHECK_AO_ATTR(pstAttr);
		if (GK_SUCCESS != s32Ret)
		{
			return s32Ret;
		}

		if ((1 == pstAttr->u32ClkSel)&& (0 == u32ChnId) )
		{
			pstAiAttr = &s_stAioState[u32ChnId].stAiAttr;
			s32Ret = AIAO_HAL_CheckAiAoClk(pstAiAttr, pstAttr);
			if (GK_SUCCESS != s32Ret)
			{
				return GK_FAILURE;
			}
		}

		s32Ret = AOPSetSysCtl(u32ChnId, AIO_TYPE_AO, pstAttr);
		if (GK_SUCCESS != s32Ret)
		{
			return s32Ret;
		}

		stAopAttrReg.u32 = AIAO_HAL_READ_REG(AOP_INF_ATTRI_REG(u32ChnId));
		stAopCtrlReg.u32 = AIAO_HAL_READ_REG(AOP_CTRL_REG(u32ChnId));

		stAopAttrReg.bits.tx_underflow_ctrl = 1;  /* send the last data */
		stAopAttrReg.bits.Reserved_1 = 0;
		stAopAttrReg.bits.Reserved_0 = 0;

		if (AIO_MODE_I2S_SLAVE == pstAttr->enWorkmode
				|| AIO_MODE_I2S_MASTER == pstAttr->enWorkmode)
		{
			stAopAttrReg.bits.tx_mode  = 0; /* I2S mode */
			stAopAttrReg.bits.tx_sd_offset = 0;
		}
		else if (AIO_MODE_PCM_SLAVE_STD == pstAttr->enWorkmode
				|| AIO_MODE_PCM_MASTER_STD == pstAttr->enWorkmode)
		{
			stAopAttrReg.bits.tx_mode  = 1; /* PCM standard mode */
			stAopAttrReg.bits.tx_sd_offset = 1;
		}
		else if (AIO_MODE_PCM_SLAVE_NSTD == pstAttr->enWorkmode
				|| AIO_MODE_PCM_MASTER_NSTD == pstAttr->enWorkmode)
		{
			stAopAttrReg.bits.tx_mode  = 1; /* PCM non standard mode */
			stAopAttrReg.bits.tx_sd_offset = 0;
		}
		else
		{
			return GK_FAILURE;
		}

		if (1 == pstAttr->u32ChnCnt)
		{
			stAopAttrReg.bits.tx_ch_num = 0;
		}
		else if (2 == pstAttr->u32ChnCnt)
		{
			stAopAttrReg.bits.tx_ch_num = 1;
		}
		else if (8 == pstAttr->u32ChnCnt)
		{
			stAopAttrReg.bits.tx_ch_num = 3;
		}
		else
		{
			return GK_FAILURE;
		}

		if (AUDIO_BIT_WIDTH_8 == pstAttr->enBitwidth)
		{
			stAopAttrReg.bits.tx_i2s_precision  = 0;
		}
		else if (AUDIO_BIT_WIDTH_16 == pstAttr->enBitwidth)
		{
			stAopAttrReg.bits.tx_i2s_precision  = 1;
		}
		else if (AUDIO_BIT_WIDTH_24 == pstAttr->enBitwidth)
		{
			stAopAttrReg.bits.tx_i2s_precision  = 2;
		}
		else
		{
			return GK_FAILURE;
		}

		switch(u32ChnId)
		{
			case 0:
				stAopAttrReg.bits.tx_sd_source_sel = 0;
				break;
#if 0
			case 1:
				stAopAttrReg.bits.tx_sd_source_sel = 1;
				break;
#endif
			default:
				return GK_FAILURE;
		}
		stAopAttrReg.bits.tx_sd0_sel = 0;
		stAopAttrReg.bits.tx_sd1_sel = 1;
		stAopAttrReg.bits.tx_sd2_sel = 2;
		stAopAttrReg.bits.tx_sd3_sel = 3;

		stAopAttrReg.bits.tx_trackmode = 0;
		AIAO_HAL_WRITE_REG(AOP_INF_ATTRI_REG(u32ChnId), stAopAttrReg.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_INF_ATTRI_REG(u32ChnId), stAopAttrReg.u32);
#endif
		stAopCtrlReg.bits.mute_en      = 0;
		stAopCtrlReg.bits.mute_fade_en = 0;
		stAopCtrlReg.bits.Reserved_3   = 0;
		stAopCtrlReg.bits.volume       = 0x79;  /*0db*/
		stAopCtrlReg.bits.Reserved_2   = 0;
		stAopCtrlReg.bits.fade_in_rate = 0;
		stAopCtrlReg.bits.fade_out_rate= 0;
		stAopCtrlReg.bits.Reserved_1   = 0;
		stAopCtrlReg.bits.bypass_en    = 0;
		stAopCtrlReg.bits.tx_enable    = 0;
		stAopCtrlReg.bits.Reserved_0   = 0;

		AIAO_HAL_WRITE_REG(AOP_CTRL_REG(u32ChnId), stAopCtrlReg.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_CTRL_REG(u32ChnId), stAopCtrlReg.u32);
#endif

		return GK_SUCCESS;
	}
#if 0
	GK_S32 AOP_HAL_ClrAoAttr(GK_U32 u32ChnId)
	{
		if (u32ChnId >= AO_DEV_MAX_NUM)
		{
			printf("AoDev%d is invalid!\n", u32ChnId);
			return GK_FAILURE;
		}

		return GK_SUCCESS;
	}
#endif
	GK_S32 AOP_HAL_DevEnable(GK_U32 u32ChnId)
	{
		//int index = 0;
		//unsigned int *p_dac_reg1 = NULL;
		U_I2S_CRG_CFG1 I2SCrg_cfg1;

		AOP_HAL_SetChildIntMask(u32ChnId);

		AOP_HAL_IntEn(u32ChnId, GK_TRUE);

		/* open blck and ws */
		I2SCrg_cfg1.u32 = AIAO_HAL_READ_REG(AOP_I2S_REG_CFG1(u32ChnId));
		I2SCrg_cfg1.bits.aiao_bclk_en  = 1;
		I2SCrg_cfg1.bits.aiao_ws_en    = 1;
		I2SCrg_cfg1.bits.aiao_srst_req = 0;
		I2SCrg_cfg1.bits.aiao_cken     = 1;
		AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG1(u32ChnId), I2SCrg_cfg1.u32);
#ifdef DEBUG_AO
		printf("%s %d 0x%0x = 0x%0x \n",__func__, __LINE__, AOP_I2S_REG_CFG1(u32ChnId), I2SCrg_cfg1.u32);
#endif
		AOP_HAL_SetTxStart(u32ChnId, GK_TRUE);
#if 0
		/* popfree close */
		acodec_Ctr0setbit(0, 18, (unsigned long)g_acodec_ctrl0_reg);
		acodec_Ctr0setbit(0, 16, (unsigned long)g_acodec_ctrl0_reg);

		/* unmute */
		acodec_Ctr0setbit(0, 13, (unsigned long)g_acodec_ctrl0_reg);
		acodec_Ctr0setbit(0, 14, (unsigned long)g_acodec_ctrl0_reg);
#endif
		return GK_SUCCESS;

	}

	GK_VOID AOP_HAL_DevDisable(GK_U32 u32ChnId)
	{
		GK_U32 u32Cnt = 0;
		U_I2S_CRG_CFG1 I2SCrg_cfg1;

		AOP_HAL_SetTxStart(u32ChnId, GK_FALSE);

		AOP_HAL_IntEn(u32ChnId, GK_FALSE);

		AOP_HAL_ClrChildIntAllStatus(u32ChnId);

		while(!AOP_HAL_GetDisDone(u32ChnId))
		{
			if (u32Cnt++ > 100)
			{
				I2SCrg_cfg1.u32 = AIAO_HAL_READ_REG(AOP_I2S_REG_CFG1(u32ChnId));
				I2SCrg_cfg1.bits.aiao_srst_req = 1;
				I2SCrg_cfg1.bits.aiao_cken     = 0;
				AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG1(u32ChnId), I2SCrg_cfg1.u32);

				udelay(10000);

				I2SCrg_cfg1.u32 = AIAO_HAL_READ_REG(AOP_I2S_REG_CFG1(u32ChnId));
				I2SCrg_cfg1.bits.aiao_cken     = 1;
				I2SCrg_cfg1.bits.aiao_srst_req = 0;
				AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG1(u32ChnId), I2SCrg_cfg1.u32);
				printf("Can't get ao stop flag. Ao clock is not config, please check your clock!\n");
				break;
			}
			udelay(10000);
		}
#if 0
		/* popfree open */
		acodec_Ctr0setbit(1, 18, (unsigned long)g_acodec_ctrl0_reg);
		acodec_Ctr0setbit(1, 16, (unsigned long)g_acodec_ctrl0_reg);

		/* mute */
		acodec_Ctr0setbit(1, 13, (unsigned long)g_acodec_ctrl0_reg);
		acodec_Ctr0setbit(1, 14, (unsigned long)g_acodec_ctrl0_reg);
#endif

		I2SCrg_cfg1.u32 = AIAO_HAL_READ_REG(AOP_I2S_REG_CFG1(u32ChnId));
		I2SCrg_cfg1.bits.aiao_bclk_en  = 0;
		I2SCrg_cfg1.bits.aiao_ws_en    = 0;
		I2SCrg_cfg1.bits.aiao_cken      = 0;
		I2SCrg_cfg1.bits.aiao_srst_req = 1;
		AIAO_HAL_WRITE_REG(AOP_I2S_REG_CFG1(u32ChnId), I2SCrg_cfg1.u32);
	}

	GK_S32 SYS_HAL_ApllClkEn(GK_BOOL bClkEn)
	{
		if(bClkEn == GK_TRUE)
		{

		}
		return 0;
	}


	GK_S32 SYS_HAL_AioClkSel(GK_U32 u32ClkSel)
	{
		acodec_Ctr0setbit(u32ClkSel << 5, 0x1 << 5, IO_ADDRESS(CRG_PERCTL103_ADDR));

		return 0;
	}

	GK_S32 SYS_HAL_AioResetSel(GK_BOOL bReset)
	{
		GK_U32 u32Tmp = (bReset == GK_TRUE) ? 1 : 0;

		acodec_Ctr0setbit(u32Tmp, 0, IO_ADDRESS(CRG_PERCTL103_ADDR));

		return 0;
	}

	GK_S32 SYS_HAL_AioClkEn(GK_BOOL bClkEn)
	{
		GK_U32 u32Tmp = (bClkEn == GK_TRUE) ? 1 : 0;

		acodec_Ctr0setbit(u32Tmp, 1, IO_ADDRESS(CRG_PERCTL103_ADDR));
		acodec_Ctr0setbit(u32Tmp, 2, IO_ADDRESS(CRG_PERCTL103_ADDR));

		return 0;
	}

	GK_S32 SYS_HAL_AcodecResetSel(GK_BOOL bReset)
	{
		GK_U32 u32Tmp = (bReset == GK_TRUE) ? 1 : 0;

		acodec_Ctr0setbit(u32Tmp, 3, IO_ADDRESS(CRG_PERCTL103_ADDR));
		acodec_Ctr0setbit(u32Tmp, 4, IO_ADDRESS(CRG_PERCTL103_ADDR));

		return 0;
	}

	int AIAO_ModInit(void)
	{
#if 1
		GK_BOOL bReset;
		GK_BOOL bClkEn;

		bReset = GK_FALSE;
		bClkEn = GK_TRUE;

		SYS_HAL_AioClkEn(bClkEn);
		SYS_HAL_AioResetSel(bReset);
#endif
		return GK_SUCCESS;
	}


	void  AIAO_ModExit(void)
	{
		GK_BOOL bReset = GK_TRUE;
		GK_BOOL bClkEn = GK_FALSE;

		SYS_HAL_AioResetSel(bReset);
		SYS_HAL_AioClkEn(bClkEn);

		return ;
	}


