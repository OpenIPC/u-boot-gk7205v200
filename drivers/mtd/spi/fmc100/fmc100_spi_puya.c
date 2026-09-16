/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*
 * PY25Q128HA_Datasheet V2.2, "Status Register" and "Configure Register":
 *   SR1  b7 SRP0  b6..b2 BP4..BP0   b1 WEL(v)  b0 WIP(ro)
 *   SR2  b7 SUS(ro)  b6 CMP  b5..b3 LB3..LB1(otp)  b2 EP_FAIL(ro)
 *        b1 QE  b0 SRP1
 *   SR3  b7 HOLD/RST  b6 DRV1  b5 DRV0  b2 WPS  b1 DC(v)  b0 DLP(v)
 * Only the bits that protect the array are touched below.
 */
#define SPI_NOR_SR1_PROT_MASK	0xfc	/* SRP0 | BP4..BP0 */
#define SPI_NOR_SR2_PROT_MASK	0x41	/* CMP | SRP1 */
#define SPI_NOR_SR3_WPS_SHIFT	2
#define SPI_NOR_SR3_WPS_MASK	(1 << SPI_NOR_SR3_WPS_SHIFT)

/*****************************************************************************/
static void spi_puya_send_cmd(struct fmc_spi *spi, unsigned char cmd)
{
	unsigned int regval;
	struct fmc_host *host = (struct fmc_host *)spi->host;

	regval = fmc_cmd_cmd1(cmd);
	fmc_write(host, FMC_CMD, regval);
	fmc_pr(BP_DBG, "\t  Set CMD[%#x]%#x\n", FMC_CMD, regval);

	regval = op_cfg_fm_cs(spi->chipselect) | OP_CFG_OEN_EN;
	fmc_write(host, FMC_OP_CFG, regval);
	fmc_pr(BP_DBG, "\t  Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

	regval = fmc_op_cmd1_en(ENABLE) | FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, regval);
	fmc_pr(BP_DBG, "\t  Set OP[%#x]%#x\n", FMC_OP, regval);

	fmc_cmd_wait_cpu_finish(host);
}

/*****************************************************************************/
static void spi_puya_write_status_reg(struct fmc_spi *spi, unsigned char cmd,
				      unsigned char val)
{
	unsigned int regval;
	struct fmc_host *host = (struct fmc_host *)spi->host;

	spi->driver->write_enable(spi);

	writeb(val, host->iobase);
	fmc_pr(BP_DBG, "\t  Write IO[%#lx]%#x\n", (uintptr_t)host->iobase,
	       *(unsigned char *)host->iobase);

	regval = fmc_cmd_cmd1(cmd);
	fmc_write(host, FMC_CMD, regval);
	fmc_pr(BP_DBG, "\t  Set CMD[%#x]%#x\n", FMC_CMD, regval);

	regval = op_cfg_fm_cs(spi->chipselect) | OP_CFG_OEN_EN;
	fmc_write(host, FMC_OP_CFG, regval);
	fmc_pr(BP_DBG, "\t  Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

	regval = fmc_data_num_cnt(SPI_NOR_SR_LEN);
	fmc_write(host, FMC_DATA_NUM, regval);
	fmc_pr(BP_DBG, "\t  Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, regval);

	regval = fmc_op_cmd1_en(ENABLE) |
		fmc_op_write_data_en(ENABLE) | FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, regval);
	fmc_pr(BP_DBG, "\t  Set OP[%#x]%#x\n", FMC_OP, regval);

	fmc_cmd_wait_cpu_finish(host);
	spi->driver->wait_ready(spi);
}

/*****************************************************************************/
/*
 * A Puya part shipped with SR3.WPS=1 is protected by the individual per-block
 * lock bits, which power up locked (datasheet Table 6-3 note 2) - sf lock only
 * drives the BP level, so every erase and write is silently discarded. Issue
 * Global Block Unlock (98h) while WPS is still 1, since that is the only mode
 * in which the per-block bits exist, then clear WPS and the BP/CMP protection.
 *
 * Mask exactly the bits that protect the array. Writing SR2 back as a flat zero
 * also cleared QE, which spi_puya_qe_enable() then set again on the way out of
 * probe - two non-volatile status-register writes on every single boot, for a
 * bit the unlock never needed to touch.
 */
static void spi_puya_global_unlock(struct fmc_spi *spi)
{
	unsigned char sr1, sr2, sr3;

	fmc_pr(BP_DBG, "\t* Puya force global unlock\n");

	sr3 = spi_general_get_flash_register(spi, SPI_CMD_RDSR3);
	if (sr3 & SPI_NOR_SR3_WPS_MASK) {
#ifndef CONFIG_MINI_BOOT
		printf("Puya SPI nor: WPS set, unlocking all blocks\n");
#endif
		spi->driver->write_enable(spi);
		spi_puya_send_cmd(spi, SPI_CMD_GBULK);
		spi->driver->wait_ready(spi);

		fmc_pr(BP_DBG, "\t  SR3 [%#x] -> [%#x] (clear WPS)\n", sr3,
		       sr3 & ~SPI_NOR_SR3_WPS_MASK);
		spi_puya_write_status_reg(spi, SPI_CMD_WRSR3,
					  sr3 & ~SPI_NOR_SR3_WPS_MASK);
	}

	sr1 = spi_general_get_flash_register(spi, SPI_CMD_RDSR);
	if (sr1 & SPI_NOR_SR1_PROT_MASK) {
		fmc_pr(BP_DBG, "\t  SR1 [%#x] -> [%#x]\n", sr1,
		       sr1 & ~SPI_NOR_SR1_PROT_MASK);
		spi_puya_write_status_reg(spi, SPI_CMD_WRSR,
					  sr1 & ~SPI_NOR_SR1_PROT_MASK);
	}

	sr2 = spi_general_get_flash_register(spi, SPI_CMD_RDSR2);
	if (sr2 & SPI_NOR_SR2_PROT_MASK) {
		fmc_pr(BP_DBG, "\t  SR2 [%#x] -> [%#x]\n", sr2,
		       sr2 & ~SPI_NOR_SR2_PROT_MASK);
		spi_puya_write_status_reg(spi, SPI_CMD_WRSR2,
					  sr2 & ~SPI_NOR_SR2_PROT_MASK);
	}

	/*
	 * Nothing above can report a rejected write: write_enable() and
	 * wait_ready() return values this driver has always discarded, and
	 * fmc_cmd_wait_cpu_finish() only logs a timeout. Read the protection
	 * back and say so, rather than let sf erase and sf write go on
	 * answering OK for operations the chip ignores - #18, item 3.
	 */
	sr1 = spi_general_get_flash_register(spi, SPI_CMD_RDSR);
	sr2 = spi_general_get_flash_register(spi, SPI_CMD_RDSR2);
	sr3 = spi_general_get_flash_register(spi, SPI_CMD_RDSR3);
	if ((sr1 & SPI_NOR_SR1_PROT_MASK) || (sr2 & SPI_NOR_SR2_PROT_MASK) ||
	    (sr3 & SPI_NOR_SR3_WPS_MASK))
		printf("Puya SPI nor: STILL PROTECTED, SR1[%#x] SR2[%#x] "
		       "SR3[%#x] - erase and write will be discarded\n",
		       sr1, sr2, sr3);
}

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
