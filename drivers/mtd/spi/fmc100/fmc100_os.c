/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*****************************************************************************/
#include "fmc100_os.h"
#include <common.h>
#include <linux/mtd/mtd.h>
#include <asm/io.h>

/*****************************************************************************/
static struct fmc_host fmc100_host;
static struct mtd_info_ex fmc100_spi_nor_info = {.type = 0, };

/*****************************************************************************/
static void fmc100_driver_shutdown(void)
{	
	unsigned int start_up_addr_mode = get_fmc_boot_mode();
	if (start_up_addr_mode == SPI_NOR_ADDR_MODE_3_BYTES) {
		int ix;
		struct fmc_host *host = &fmc100_host;
		struct fmc_spi *spi = host->spi;
		struct mtd_info_ex *spi_nor_info = &fmc100_spi_nor_info;

		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);

		for (ix = 0; ix < spi_nor_info->numchips; ix++, spi++) {
			/* 4 byte addr mode */
			if (spi->addrcycle == 4) {
				spi->driver->wait_ready(spi);
				spi->driver->entry_4addr(spi, DISABLE);
			}
		}
	}
}

/*****************************************************************************/
static int fmc100_driver_probe(void)
{
	int ret;
	struct fmc_host *host = &fmc100_host;

	fmc_pr(BT_DBG, "\t|*-Start SPI nor flash driver probe\n");

	/* Fmc ip version check */
	ret = fmc_ip_ver_check();
	if (ret) {
		fmc_pr(BT_DBG, "\t|*-IP version unknown, result: %d\n", ret);
		return ret;
	}

	fmc_pr(BT_DBG, "\t||-SPI nor host init\n");
	memset((char *)host, 0, sizeof(struct fmc_host));
	ret = fmc100_spi_nor_init(host);
	if (ret) {
		fmc_pr(BT_DBG, "Error: SPI Nor init failed, ret: %d\n", ret);
		goto end;
	}

end:
	fmc_pr(BT_DBG, "\t|*-End SPI nor flash driver probe\n");

	return ret;
}

/*****************************************************************************/
struct mtd_info_ex *fmc100_get_spi_nor_info(struct spi_flash *spi_nor_flash)
{
	if (fmc100_spi_nor_info.type == 0) {
		if (fmc100_spi_nor_probe(NULL) == NULL)
			return NULL;
	}

	return &fmc100_spi_nor_info;
}

/*****************************************************************************/
static void fmc100_probe_spi_size(struct spi_flash *spi_nor_flash)
{
	struct fmc_host *host = &fmc100_host;
	struct fmc_spi *spi = host->spi;
	unsigned int ix;
	unsigned int total = 0;
	struct mtd_info_ex *spi_nor_info = host->spi_nor_info;

	fmc_pr(BT_DBG, "\t|*-Start probe SPI nor flash total size\n");
	for (ix = 0; ix < spi_nor_info->numchips; ix++, spi++) {
		fmc_pr(BT_DBG, "\t||-SPI nor flash[%d]: %dMB\n", ix,
		       (u_int)byte_to_mb(spi->chipsize));
		total += spi->chipsize;
	}

	spi_nor_flash->size = total;

	fmc_pr(BT_DBG, "\t|*-Probe SPI nor total size: %dMB, chip num: %d\n",
	       byte_to_mb(spi_nor_flash->size), spi_nor_info->numchips);
}

/*****************************************************************************/
struct spi_flash *fmc100_spi_nor_probe(struct mtd_info_ex **spi_nor_info)
{
	static struct spi_flash *spi_nor_flash = NULL;

	fmc_pr(BT_DBG, "\t*-Start SPI Nor flash probe\n");

	if (spi_nor_flash) {
		fmc_pr(BT_DBG, "\t*-SPI Nor flash is initialized.\n");
		return spi_nor_flash;
	}

	/* Check current SPI device type whether SPI nor */
	fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);

	fmc_pr(BT_DBG, "\t|-SPI Nor flash driver probe\n");
	if (!fmc100_driver_probe()) {
		struct fmc_host *host = &fmc100_host;

		fmc_pr(BT_DBG, "\t|-SPI nor flash scanning\n");
		host->spi_nor_info = &fmc100_spi_nor_info;
		spi_nor_flash = fmc100_spi_nor_scan(host);
		if (spi_nor_flash) {
			*spi_nor_info =
				fmc100_get_spi_nor_info(spi_nor_flash);

			fmc100_probe_spi_size(spi_nor_flash);
#ifndef CONFIG_MINI_BOOT
			printf("SPI Nor total size: %uMB\n",
			       byte_to_mb(spi_nor_flash->size));
#endif
			fmc_pr(BT_DBG, "\t|-Add func hook for Reset cmd\n");
			add_shutdown(fmc100_driver_shutdown);

			goto end;
		}
	}

	spi_nor_flash = NULL;
	fmc100_spi_nor_info.type = 0;

end:
	/* Change SPI device type to default */
	fmc_dev_type_switch(FLASH_TYPE_DEFAULT);

	fmc_pr(BT_DBG, "\t*-End SPI Nor flash probe\n");

	return spi_nor_flash;
}

/*****************************************************************************/
#ifdef CONFIG_SPI_BLOCK_PROTECT
void fmc100_spi_flash_lock(unsigned char cmp, unsigned char level,
				unsigned char op)
{
	struct fmc_host *host = &fmc100_host;
	struct spi_flash *nor = host->spi_nor_flash;

	host->cmp = cmp;

	if (op == BP_OP_GET) {
		puts("Get spi lock information\n");
		if (host->level) {
			if (host->level == nor->bp_level_max)
				puts("all blocks are locked.\n");
			else
				printf("level: %d\n", host->level);
			printf("Spi is locked. lock address[0 => %#x]\n",
			       host->end_addr);
		} else {
			puts("all blocks are unlocked.\n");
		}

		return;
	}

	if (op == BP_OP_SET) {
		if (level) {
			if (level == nor->bp_level_max)
				puts("lock all blocks.\n");
			else
				printf("lock level: %d\n", level);
		} else {
			puts("unlock all block.\n");
		}

		fmc100_spi_lock(host, level);
		return;
	}

	printf("%s ERROR: Invalid optin argument!", __func__);
}
/*****************************************************************************/
#endif /* CONFIG_SPI_BLOCK_PROTECT */
