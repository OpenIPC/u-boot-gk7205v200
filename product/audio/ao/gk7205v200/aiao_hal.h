#ifndef __AIAO_HAL_H__
#define __AIAO_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "audio_ao.h"

#define AIO_DATA_WIDTH_8BIT     0x0
#define AIO_DATA_WIDTH_16BIT    0x1
#define AIO_DATA_WIDTH_18BIT    0x2
#define AIO_DATA_WIDTH_20BIT    0x3
#define AIO_DATA_WIDTH_24BIT    0x4
#define AIO_DATA_WIDTH_32BIT    0x5

#define AIO_PCM_DATA_WIDTH_8BIT     0x0
#define AIO_PCM_DATA_WIDTH_16BIT    0x1
#define AIO_PCM_DATA_WIDTH_32BIT    0x2
#define AIO_PCM_DATA_WIDTH_64BIT    0x3
#define AIO_PCM_DATA_WIDTH_128BIT   0x4


#define AIO_CHN_NUM_2CHN        0x0
#define AIO_CHN_NUM_4CHN        0x1
#define AIO_CHN_NUM_8CHN        0x2
#define AIO_CHN_NUM_16CHN       0x3

#define AIO_ONE_FIFO_BITWIDTH       128
#define AIO_GET_BITCNT(enBitwidth)  (8<<(enBitwidth))
#define AIO_TIME_DIFF_US(a, b) (((a.tv_sec) - (b.tv_sec))*1000000 + ((a.tv_usec) - (b.tv_usec)))

#define AIO_IS_PCM_MODE(mode) \
	(AIO_MODE_PCM_SLAVE_STD == (mode)  || AIO_MODE_PCM_SLAVE_NSTD == (mode)  \
	 || AIO_MODE_PCM_MASTER_STD == (mode) || AIO_MODE_PCM_MASTER_NSTD == (mode))
#define AIO_IS_I2S_MODE(mode) \
	(AIO_MODE_I2S_MASTER == (mode) || AIO_MODE_I2S_SLAVE == (mode))
#define AIO_IS_MASTER_MODE(mode) \
	(AIO_MODE_I2S_MASTER == (mode) || AIO_MODE_PCM_MASTER_STD == (mode) || AIO_MODE_PCM_MASTER_NSTD == (mode))
#define AIO_IS_SLAVE_MODE(mode) \
	(AIO_MODE_I2S_SLAVE == (mode) || AIO_MODE_PCM_SLAVE_STD == (mode) || AIO_MODE_PCM_SLAVE_NSTD == (mode))


#define AIP0_INTMASK    1
#define AOP0_INTMASK    1
#define AOP1_INTMASK    1

#define AI_DEV_MAX_NUM          1
#define AO_DEV_MIN_NUM          0
#define AO_DEV_MAX_NUM          1
#define AIO_MAX_NUM             1
#define AIO_MAX_CHN_NUM         2

#define MAX_AUDIO_FRAME_NUM    50       /*max count of audio frame in Buffer */
#define MAX_AUDIO_POINT_BYTES  4        /*max bytes of one sample point(now 32bit max)*/

#define MAX_VOICE_POINT_NUM    480      /*max sample per frame for voice encode */

#define MAX_AUDIO_POINT_NUM    2048     /*max sample per frame for all encoder(aacplus:2048)*/
#define MAX_AO_POINT_NUM       4096     /* from h3:support 4096 framelen*/
#define MIN_AUDIO_POINT_NUM    80       /*min sample per frame*/
#define MAX_AI_POINT_NUM    2048     /*max sample per frame for all encoder(aacplus:2048)*/

	/*max length of audio frame by bytes, one frame contain many sample point */
#define MAX_AUDIO_FRAME_LEN    (MAX_AUDIO_POINT_BYTES*MAX_AO_POINT_NUM)

	/* 1800M */
	/*(MCLK0/AIO Clock Source Frequency) x 2^27*/
#define AIO_MCLK_48K_1800       0x000DFB24  /* 48k * 256 */
#define AIO_MCLK_441K_1800      0x000CD856  /* 44.1k * 256 */
#define AIO_MCLK_32K_1800       0x00095218  /* 32k * 256 */

	typedef GK_S32 AUDIO_DEV;

	typedef enum AIO_TYPE_E
	{
		AIO_TYPE_AI = 0,
		AIO_TYPE_AO,
		AIO_TYPE_BUTT
	} AIO_TYPE_E;

	typedef enum AIO_SOUND_MODE_E
	{
		AUDIO_SOUND_MODE_MONO   =0,/*mono*/
		AUDIO_SOUND_MODE_STEREO =1,/*stereo*/
		AUDIO_SOUND_MODE_BUTT
	} AUDIO_SOUND_MODE_E;

	typedef enum AIO_MODE_E
	{
		AIO_MODE_I2S_MASTER  = 0,   /* AIO I2S master mode */
		AIO_MODE_I2S_SLAVE,         /* AIO I2S slave mode */
		AIO_MODE_PCM_SLAVE_STD,     /* AIO PCM slave standard mode */
		AIO_MODE_PCM_SLAVE_NSTD,    /* AIO PCM slave non-standard mode */
		AIO_MODE_PCM_MASTER_STD,    /* AIO PCM master standard mode */
		AIO_MODE_PCM_MASTER_NSTD,   /* AIO PCM master non-standard mode */
		AIO_MODE_BUTT
	} AIO_MODE_E;

	typedef enum AUDIO_BIT_WIDTH_E
	{
		AUDIO_BIT_WIDTH_8   = 0,   /* 8bit width */
		AUDIO_BIT_WIDTH_16  = 1,   /* 16bit width*/
		AUDIO_BIT_WIDTH_24  = 2,   /* 24bit width*/
		AUDIO_BIT_WIDTH_BUTT,
	} AUDIO_BIT_WIDTH_E;

	typedef enum AUDIO_FADE_RATE_E
	{
		AUDIO_FADE_RATE_1    = 0,
		AUDIO_FADE_RATE_2    = 1,
		AUDIO_FADE_RATE_4    = 2,
		AUDIO_FADE_RATE_8   = 3,
		AUDIO_FADE_RATE_16  = 4,
		AUDIO_FADE_RATE_32  = 5,
		AUDIO_FADE_RATE_64  = 6,
		AUDIO_FADE_RATE_128 = 7,

		AUDIO_FADE_RATE_BUTT
	} AUDIO_FADE_RATE_E;

	typedef enum AUDIO_TRACK_MODE_E
	{
		AUDIO_TRACK_NORMAL      = 0,
		AUDIO_TRACK_BOTH_LEFT   = 1,
		AUDIO_TRACK_BOTH_RIGHT  = 2,
		AUDIO_TRACK_EXCHANGE    = 3,
		AUDIO_TRACK_MIX         = 4,
		AUDIO_TRACK_LEFT_MUTE   = 5,
		AUDIO_TRACK_RIGHT_MUTE  = 6,
		AUDIO_TRACK_BOTH_MUTE   = 7,

		AUDIO_TRACK_BUTT
	} AUDIO_TRACK_MODE_E;

	typedef struct AUDIO_FADE_S
	{
		GK_BOOL         bFade;
		AUDIO_FADE_RATE_E enFadeInRate;
		AUDIO_FADE_RATE_E enFadeOutRate;
	} AUDIO_FADE_S;

	typedef struct AIO_ATTR_S
	{
		AUDIO_SAMPLE_RATE_E enSamplerate;   /* sample rate */
		AUDIO_BIT_WIDTH_E   enBitwidth;     /* bitwidth */
		AIO_MODE_E          enWorkmode;     /* master or slave mode */
		AUDIO_SOUND_MODE_E  enSoundmode;    /* momo or steror */
		GK_U32              u32EXFlag;      /* expand 8bit to 16bit,use AI_EXPAND(only valid for AI 8bit) */
		GK_U32              u32FrmNum;      /* frame num in buf[2,MAX_AUDIO_FRAME_NUM] */
		GK_U32              u32PtNumPerFrm; /* point num per frame (80/160/240/320/480/1024/2048)
											   (ADPCM IMA should add 1 point, AMR only support 160) */
		GK_U32              u32ChnCnt;      /* channle number on FS, valid value:1/2/4/8 */
		GK_U32              u32ClkSel;      /* 0: AI and AO clock is separate
1: AI and AO clock is inseparate, AI use AO's clock
*/
	} AIO_ATTR_S;

	typedef struct AIO_STATE_S
	{
		GK_BOOL             bInitialized;   /*initialed flag*/
		GK_BOOL             bAiConfig;
		GK_BOOL             bAoConfig;
		AIO_ATTR_S          stAiAttr;
		AIO_ATTR_S          stAoAttr;
	} AIO_STATE_S;


	/*****************************************************************************
Description  : AIO IP Driver API
	 *****************************************************************************/
	GK_U32              AIO_HAL_ReadReg(GK_U32 u32Offset);
	GK_VOID             AIO_HAL_WriteReg(GK_U32 u32Offset, GK_U32 u32Value);

	GK_VOID             AOP_HAL_SetBufferAddr(GK_U32 u32ChnId, GK_U32 u32Value);
	GK_VOID             AOP_HAL_SetBufferSize(GK_U32 u32ChnId, GK_U32 u32Value);
	GK_VOID             AOP_HAL_SetBuffWptr(GK_U32 u32ChnId, GK_U32 u32Value);
	GK_VOID             AOP_HAL_SetBuffRptr(GK_U32 u32ChnId, GK_U32 u32Value);
	GK_VOID             AOP_HAL_SetTransSize(GK_U32 u32ChnId, GK_U32 u32Value);
	GK_S32              AOP_HAL_SetDevAttr(GK_U32 u32ChnId, AIO_ATTR_S *pstAttr);
	GK_S32              AOP_HAL_DevEnable(GK_U32 u32ChnId);
	GK_VOID             AOP_HAL_DevDisable(GK_U32 u32ChnId);
	GK_S32              AOP_HAL_SetVolume(GK_U32 u32ChnId, GK_S32 s32VolumeDb);

	int    AIAO_ModInit(void);
	void   AIAO_ModExit(void);
	GK_S32 AIAO_HAL_SysInit(GK_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
