/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
 
#ifndef __FMC100_SPI_H__
#define __FMC100_SPI_H__

#include <spi_flash.h>
#include <linux/mtd/mtd.h>

struct spi_flash *fmc100_spi_nor_probe(struct mtd_info_ex **);
struct mtd_info_ex *fmc100_get_spi_nor_info(struct spi_flash *);

#endif

