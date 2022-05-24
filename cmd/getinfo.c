/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <common.h>
#include <command.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <version.h>

#include <asm/io.h>

#if defined(CONFIG_FMC_SPI_NAND) \
	|| defined(CONFIG_FMC_NAND)
#include <nfc_common.h>
#endif

/*****************************************************************************/
#if defined(CONFIG_FMC_SPI_NAND) || defined(CONFIG_FMC_NAND) || \
	defined(CONFIG_FMC_SPI_NOR)
static int print_mtd_info(const struct mtd_info_ex *mtd_info,const char *cmd)
{
	unsigned int len, ix;

	if (mtd_info == NULL || mtd_info->type == 0) { /* no find spi/nand */
		printf("no find ");
		if (*cmd == 's')
			printf("spi");
		else if (*cmd == 'n')
			printf("nand");
		printf("\n");
	} else {
		printf("Block:%sB ", ultohstr(mtd_info->erasesize));
		printf("Chip:%sB*%d ",
			   ultohstr(mtd_info->chipsize),
			   mtd_info->numchips);

		if (*cmd == 'n') {
			printf("Page:%sB ", ultohstr(mtd_info->pagesize));
			printf("OOB:%sB ", ultohstr(mtd_info->oobsize));
#if defined(CONFIG_FMC_SPI_NAND) || defined(CONFIG_FMC_NAND)
#ifdef CONFIG_CMD_NAND
			printf("ECC:%s ", nand_ecc_name(mtd_info->ecctype));
#endif
#else
			printf("ECC:%s ",
				   get_ecctype_str((mtd_info->ecctype & 0x7)));
#endif
		}
		printf("\nID:");

		len = (mtd_info->id_length > 8 ? 8 : mtd_info->id_length); /* 8:len of max id */
		for (ix = 0; ix < len; ix++)
			printf("0x%02X ", mtd_info->ids[ix]);
		printf("\nName:\"%s\"\n", mtd_info->name);
	}

	return 0;
}
#endif

static int do_getinfo(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	char *cmd = NULL;

	if (argc < 2) { /* 2:len of argv */
		cmd_usage(cmdtp);
		return -1;
	}

	cmd = argv[1];
	if (strcmp(cmd, "bootmode") == 0) {
		switch (get_boot_media()) {
		default:
		case BOOT_MEDIA_UNKNOWN:
			printf("Boot from unknown device,"
				   " please check your hardware config.\n");
			return -1;

		case BOOT_MEDIA_NAND:
			printf("nand\n");
			break;

		case BOOT_MEDIA_SPIFLASH:
			printf("spi\n");
			break;

		case BOOT_MEDIA_EMMC:
			printf("emmc\n");
			break;

		case BOOT_MEDIA_UFS:
			printf("ufs\n");
			break;
		}
		return 0;
	}

	if (strcmp(cmd, "version") == 0) {
		printf("version: %s\n", U_BOOT_VERSION);
		return 0;
	}

#if defined(CONFIG_FMC_SPI_NAND) || defined(CONFIG_FMC_NAND)
	if (strcmp(cmd, "nand") == 0) {
		struct mtd_info_ex *mtd_info = NULL;
		mtd_info = get_nand_info();
		return print_mtd_info(mtd_info, cmd);
	}
#endif

#ifdef CONFIG_FMC_SPI_NOR
	if (strcmp(cmd, "spi") == 0) {
		struct mtd_info_ex *mtd_info = NULL;
		mtd_info = get_spiflash_info();
		return print_mtd_info(mtd_info, cmd);
	}
#endif

	return 0;
}

U_BOOT_CMD(
	getinfo, CONFIG_SYS_MAXARGS, 1, do_getinfo,
	"print hardware information",
	"bootmode - get start memeory type e.g. nand/spi etc\n"
	"getinfo nand - get nand flash information\n"
	"getinfo spi - get spi flash information\n"
	"getinfo version - get system version\n"
);
