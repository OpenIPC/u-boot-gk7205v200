/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <common.h>
#include <command.h>

#ifdef CONFIG_GODNET
#include <godnet_vo.h>
#endif

#ifdef CONFIG_GODARM
#include <godarm_vo.h>
#endif

extern int load_jpeg(void);
extern int jpeg_decode(unsigned int format);

int do_jpgd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;
	unsigned int format;

	if (argc < 2) {
		printf("Insufficient parameter!\n");
		printf("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}

	format  = (unsigned int)simple_strtoul(argv[1], NULL, 10);
#ifdef RGB_OUTPUT_ENABLE
	if (format > 2) {
#else
	if (format != 0) {
#endif
		printf("Invalid parameter!\n");
		return -1;
	}

#ifdef CONFIG_SYS_LONGHELP
	printf("you should first set:\n%s\n", cmdtp->help);
#endif

	ret = load_jpeg();
	if (ret != 0) {
		printf("load jpeg err. \n");
		//todo return 0 or ret?
		return 0;
	}
	jpeg_decode(format);

	printf("decode jpeg!\n");

	return 0;
}

U_BOOT_CMD(
	decjpg,    CFG_MAXARGS, 1,  do_jpgd,
	"jpgd   - decode jpeg picture.\n"
	"decjpg [format]",
	"\nargs: [format]\n"
#ifdef RGB_OUTPUT_ENABLE
	"\t-<format> : 0: semi-plannar yvu420, 1: ARGB1555, 2: ARGB8888\n"
#else
	"\t-<format> : 0: semi-plannar yvu420\n"
#endif
	"\t- setenv jpeg_addr     0x--------\n"
	"\t- setenv jpeg_size     0x--------\n"
	"\t- setenv vobuf         0x--------\n"
	"\t- setenv jpeg_emar_buf 0x--------\n"
);


