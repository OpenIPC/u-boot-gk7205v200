#include <common.h>
#include <command.h>
#include <version.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <config.h>

#include "type.h"
#include "board.h"
#include "aiao_hal.h"

int start_ao( unsigned int u32Addr, unsigned int u32Size,
		AUDIO_SAMPLE_RATE_E enSample, unsigned int u32Chncnt, unsigned int u32Vol)
{
	GK_U32 u32ChnId = 0;
	GK_U32 u32TransSize = 0;
	AIO_ATTR_S stAttr;

	if(u32Chncnt == 1)
	{
		stAttr.enSoundmode = AUDIO_SOUND_MODE_MONO;
	}
	else if(u32Chncnt == 2)
	{
		stAttr.enSoundmode = AUDIO_SOUND_MODE_STEREO;
	}
	else
	{
		printf("u32Chncnt:%u is invalid!\n", u32Chncnt);
		return GK_FAILURE;
	}
	stAttr.enBitwidth = AUDIO_BIT_WIDTH_16;
	stAttr.enSamplerate = enSample;
	stAttr.u32ChnCnt = u32Chncnt;
	stAttr.u32ClkSel = 0;
	stAttr.u32EXFlag = 0;
	stAttr.u32FrmNum = 30;
	stAttr.u32PtNumPerFrm = 320;
	stAttr.enWorkmode = AIO_MODE_I2S_MASTER;
	u32TransSize = stAttr.u32PtNumPerFrm<<stAttr.enBitwidth;

	AIAO_ModInit();
	AIAO_HAL_SysInit();
	AOP_HAL_SetDevAttr(u32ChnId, &stAttr);
	AOP_HAL_SetBufferAddr(u32ChnId, u32Addr);
	AOP_HAL_SetBufferSize(u32ChnId, u32Size);
	AOP_HAL_SetBuffWptr(u32ChnId, u32Size - 32);
	AOP_HAL_SetBuffRptr(u32ChnId, 0);
	AOP_HAL_SetTransSize(u32ChnId, u32TransSize);
	AOP_HAL_SetVolume(u32ChnId, u32Vol);
	AOP_HAL_DevEnable(u32ChnId);

	return GK_SUCCESS;
}

int stop_ao(void)
{
	GK_U32 u32ChnId = 0;
	AOP_HAL_DevDisable(u32ChnId);
	AIAO_ModExit();

	return GK_SUCCESS;
}

