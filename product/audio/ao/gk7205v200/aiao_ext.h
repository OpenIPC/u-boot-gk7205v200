/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __AIAO_EXT_H__
#define __AIAO_EXT_H__

typedef struct AIO_DRV_DEV_CTX_S
{
	struct
	{
		GK_U32          u32BufSize;
		GK_U32          u32PhyAddr;
		GK_U8*          pu8VirAddr;

		GK_U32          u32RptrOffSet;
		GK_U32          u32WptrOffSet;

	} stCirBuf;

	AIO_ATTR_S          stAioAttr;
	GK_BOOL             bEnable;

	GK_U64              u64LastPts;
	GK_U32              u32FrmTime;
	GK_U32              u32MaxFrmTime;
	GK_U32              u32IsrTime;
	GK_U32              u32MaxIsrTime;
	GK_U32              u32AioFifoLen;
	GK_U32              u32FifoLenBase;
	GK_U32              u32FifoShift;
	GK_U32              u32TransLen;

	GK_S32              as32ChnIndex[AIO_MAX_CHN_NUM];

	GK_U32              u32IntCnt;
	GK_U32              u32fifoIntCnt;
	GK_U32              u32buffIntCnt;
	AUDIO_TRACK_MODE_E  enTrackMode;
	GK_BOOL             bMute;
	AUDIO_FADE_S        stFade;
	GK_S32              s32Volume;
	GK_BOOL             bMicInl;
	GK_BOOL             bMicInr;
}AIO_DRV_DEV_CTX_S;

#endif
