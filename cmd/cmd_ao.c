/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*
 * audio output Support
 */
#include <common.h>
#include <command.h>

#include "audio_ao.h"

extern int acodec_i2s_set(AUDIO_SAMPLE_RATE_E enSample);
extern int acodec_device_init(void);
extern int acodec_device_exit(void);
extern int start_ao(unsigned int u32Addr, unsigned int u32Size,
		    AUDIO_SAMPLE_RATE_E enSample, unsigned int u32Chncnt, unsigned int u32Vol);
extern int stop_ao(void);
extern void amp_mute(void);
extern void amp_unmute(void);

int do_startao(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int addr, size, vol;
	AUDIO_SAMPLE_RATE_E samplelist[]= {
		AUDIO_SAMPLE_RATE_8000,
		AUDIO_SAMPLE_RATE_11025,
		AUDIO_SAMPLE_RATE_12000,
		AUDIO_SAMPLE_RATE_16000,
		AUDIO_SAMPLE_RATE_22050,
		AUDIO_SAMPLE_RATE_24000,
		AUDIO_SAMPLE_RATE_32000,
		AUDIO_SAMPLE_RATE_44100,
		AUDIO_SAMPLE_RATE_48000
	};

	AUDIO_SAMPLE_RATE_E enSample = AUDIO_SAMPLE_RATE_8000;
	unsigned int ChnCnt;
	unsigned int i;
	unsigned int bflag = 0;

	if (argc < 6) {
		printf("Insufficient parameter!\n");
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}

	addr = (unsigned int)simple_strtoul(argv[1], NULL, 10);
	size = (unsigned int)simple_strtoul(argv[2], NULL, 10);
	enSample  = (unsigned int)simple_strtoul(argv[3], NULL, 10);
	ChnCnt  = (unsigned int)simple_strtoul(argv[4], NULL, 10);
	vol = (unsigned int)simple_strtoul(argv[5], NULL, 10);

	if (addr%32) {
		printf("[Error] Invalid addr parameter:0x%0x, address should be aligned by 32Byte!\n", addr);
		return -1;
	}
	if (size%32) {
		printf("[Warning] Invalid size parameter:0x%0x, size should be aligned by 32Byte!\n", size);
		return -1;
	}

	if (size == 0) {
		printf("[Warning] Invalid size parameter:0x%0x, size should be greater than 0!\n", size);
		return -1;
	}

	for (i=0; i<sizeof(samplelist)/sizeof(samplelist[0]); i++) {
		if (enSample == samplelist[i]) {
			bflag = 1;
			break;
		}
	}

	if (bflag == 0) {
		printf("[Warning] Invalid samplerate parameter.\n");
		return -1;
	}

	if (vol > 6) {
		printf("[Warning] Invalid volume parameter, range:[0, 6]dB!\n");
		return -1;
	}

	if ((ChnCnt == 0) || (ChnCnt > 2)) {
		printf("[Warning] Invalid channelnum parameter, range:[1, 2].\n");
		return -1;
	}

	acodec_device_init();
	acodec_i2s_set(enSample);
	udelay(100 * 1000);
	start_ao(addr, size, enSample, ChnCnt, vol);
	amp_unmute();

	printf("ao dev start ok!\n");

	return 0;
}

int do_stopao(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	amp_mute();
	stop_ao();
	//acodec_device_exit();

	printf("ao dev closed!\n");

	return 0;
}

U_BOOT_CMD(
	startao,    10,	1,  do_startao,
	"startao   - open interface of ao device.\n"
	"\t- startao [addr size samplerate channelnum volume]",
	"\nargs: [addr size samplerate channelnum volume]\n"
	"\t-<addr> : address of raw audio data,align by 32Byte\n"
	"\t-<size>: size of raw audio data,align by 32Byte\n"
	"\t-<samplerate>: sample rate of raw audio data\n"
	"\t-<channelnum>: channel number of raw audio data\n"
	"\t-<volume>: audio output volume default:0dB, range:0 ~ 6dB\n"
);

U_BOOT_CMD(
	stopao,    1,	0,  do_stopao,
	"stopao   - close interface of ao device.\n",
	""
);




