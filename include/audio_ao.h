/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __AUDIO_AO_H__
#define __AUDIO_AO_H__

typedef enum AUDIO_SAMPLE_RATE_E {
	AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
	AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/
	AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
	AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
	AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
	AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
	AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
	AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
	AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
	AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E;

#endif

