/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*****************************************************************************/
static int spi_puya_entry_4addr(struct fmc_spi *spi, int enable)
{
	return 0;
}

/****************************************************************************/
/*
 *  enable QE bit if QUAD read write is supported by puya'a P25Q128H,it is as same as W25Q(128/256)FV
 *  opcode	type	name
 *   0x35	 RD	SR(S15-S8)
 *   0x31	 WR	SR(S15-S8)
 */
static int spi_puya_qe_enable(struct fmc_spi *spi)
{
	unsigned char status;
	unsigned char op;
	unsigned int regval;
	const char *str[] = {"Disable", "Enable"};
	struct fmc_host *host = (struct fmc_host *)spi->host;

	op = spi_is_quad(spi);

	fmc_pr(QE_DBG, "\t* Start SPI Nor %s Quad.\n", str[op]);

	status = spi_general_get_flash_register(spi, SPI_CMD_RDSR2);
	fmc_pr(QE_DBG, "\t  Read Status Register-2[%#x]%#x\n", SPI_CMD_RDSR2,
			status);
	if (spi_nor_get_qe_by_cr(status) == op) {
		fmc_pr(QE_DBG, "\t* Quad was %s status:%#x\n", str[op], status);
		goto QE_END;
	}

	spi->driver->write_enable(spi);
	if (op)
		status |= SPI_NOR_CR_QE_MASK;
	else
		status &= ~SPI_NOR_CR_QE_MASK;

	writeb(status, host->iobase);
	fmc_pr(QE_DBG, "\t  Write IO[%#lx]%#x\n", (uintptr_t)host->iobase,
			*(unsigned char *)host->iobase);

	/* There is new cmd for Write Status Register 2 by W25Q(128/256)FV */
	regval = fmc_cmd_cmd1(SPI_CMD_WRSR2);
	fmc_write(host, FMC_CMD, regval);
	fmc_pr(QE_DBG, "\t  Set CMD[%#x]%#x\n", FMC_CMD, regval);

	regval = op_cfg_fm_cs(spi->chipselect);
	fmc_write(host, FMC_OP_CFG, regval);
	fmc_pr(QE_DBG, "\t  Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

	regval = fmc_data_num_cnt(SPI_NOR_SR_LEN);
	fmc_write(host, FMC_DATA_NUM, regval);
	fmc_pr(QE_DBG, "\t  Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, regval);

	regval = fmc_op_cmd1_en(ENABLE) |
		fmc_op_write_data_en(ENABLE) | FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, regval);
	fmc_pr(QE_DBG, "\t  Set OP[%#x]%#x\n", FMC_OP, regval);

	fmc_cmd_wait_cpu_finish(host);

	/* wait the flash have switched quad mode success */
	spi->driver->wait_ready(spi);

	status = spi_general_get_flash_register(spi, SPI_CMD_RDSR2);
	fmc_pr(QE_DBG, "\t  Read Status Register-2[%#x]:%#x\n",
			SPI_CMD_RDSR2, status);
	if (spi_nor_get_qe_by_cr(status) == op)
		fmc_pr(QE_DBG, "\t  %s Quad success. status:%#x\n",
				str[op], status);
	else
		db_msg("Error: %s Quad failed! reg:%#x\n", str[op], status);
QE_END:
	return op;
}