/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

/*****************************************************************************/
#include "fmc100.h"
#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <errno.h>
#include <asm/arch/platform.h>

/*****************************************************************************/
int write_bp_area_check(struct fmc_host *host, u_int to, size_t len)
{
#ifdef CONFIG_SPI_BLOCK_PROTECT
	if (host->level) {
		if ((host->cmp == BP_CMP_TOP)
				&& ((to + len) > host->start_addr)) {
			puts("\n");
			db_msg("Reg write area[%#x => %#x] was locked\n",
					host->start_addr, (to + len));
			return -EINVAL;
		}

		if ((host->cmp == BP_CMP_BOTTOM) && (to < host->end_addr)) {
			unsigned end = ((to + len) > host->end_addr) ?
				host->end_addr : (to + len);

			puts("\n");
			db_msg("Reg write area[%#x => %#x] was locked\n", to,
					end);
			return -EINVAL;
		}
	}
#endif
	return 0;
}

/*****************************************************************************/
static void fmc100_dma_transfer(struct fmc_spi *spi,
			 unsigned int spi_start_addr, unsigned char *dma_buffer,
			 unsigned char rw_op, unsigned int size)
{
	unsigned char if_type = 0;
	unsigned char dummy = 0;
	unsigned char w_cmd = 0;
	unsigned char r_cmd = 0;
	unsigned int regval;
	struct fmc_host *host = spi->host;

	fmc_pr(DMA_DB, "\t\t *-Start dma transfer => [%#x], len[%#x], buf = %p\n",
	       spi_start_addr, size, dma_buffer);

	regval = FMC_INT_CLR_ALL;
	fmc_write(host, FMC_INT_CLR, regval);
	fmc_pr(DMA_DB, "\t\t   Set INT_CLR[%#x]%#x\n", FMC_INT_CLR, regval);

	regval = spi_start_addr;
	fmc_write(host, FMC_ADDRL, regval);
	fmc_pr(DMA_DB, "\t\t   Set ADDRL[%#x]%#x\n", FMC_ADDRL, regval);

	if (rw_op == RW_OP_WRITE) {
		if_type = spi->write->iftype;
		dummy = spi->write->dummy;
		w_cmd = spi->write->cmd;
	} else if (rw_op == RW_OP_READ) {
		if_type = spi->read->iftype;
		dummy = spi->read->dummy;
		r_cmd = spi->read->cmd;
	}

	regval = op_cfg_fm_cs(spi->chipselect) | OP_CFG_OEN_EN |
		 op_cfg_mem_if_type(if_type) | op_cfg_addr_num(spi->addrcycle) |
		 op_cfg_dummy_num(dummy);
	fmc_write(host, FMC_OP_CFG, regval);
	fmc_pr(DMA_DB, "\t\t   Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

	regval = fmc_dma_len_set(size);
	fmc_write(host, FMC_DMA_LEN, regval);
	fmc_pr(DMA_DB, "\t\t   Set DMA_LEN[%#x]%#x\n", FMC_DMA_LEN, regval);
	/* get hight 32 bits */
	regval = (((uintptr_t)dma_buffer & FMC_DMA_SADDRH_MASK) >> 32);
	fmc_write(host, FMC_DMA_SADDRH_D0, regval);
	fmc_pr(DMA_DB, "\t\t   Set DMA_SADDRH_D0[%#x]%#x\n", FMC_DMA_SADDRH_D0,
	       regval);

	regval = (unsigned int)((uintptr_t)dma_buffer);
	fmc_write(host, FMC_DMA_SADDR_D0, regval);
	fmc_pr(DMA_DB, "\t\t   Set DMA_SADDR_D0[%#x]%#x\n", FMC_DMA_SADDR_D0,
	       regval);

	regval = op_ctrl_rd_opcode(r_cmd) | op_ctrl_wr_opcode(w_cmd) |
		 op_ctrl_rw_op(rw_op) | OP_CTRL_DMA_OP_READY;

	fmc_write(host, FMC_OP_CTRL, regval);
	fmc_pr(DMA_DB, "\t\t   Set OP_CTRL[%#x]%#x\n", FMC_OP_CTRL, regval);

	fmc_dma_wait_int_finish(host);

	fmc_pr(DMA_DB, "\t\t *-End dma transfer.\n");
}

/*****************************************************************************/
#ifdef FMC100_SPI_NOR_SUPPORT_REG_READ
static char *fmc100_reg_read_buf(struct fmc_host *host,
				struct fmc_spi *spi,
				unsigned int spi_start_addr,
				unsigned int size,
				unsigned char *buffer)
{
	unsigned int regval;

	fmc_pr(DMA_DB, "* Start reg read, from:%#x len:%#x\n", spi_start_addr,
	       size);

	if (size > FMC100_REG_RD_MAX_SIZE)
		db_bug("reg read out of reg range.\n");

	fmc_write(host, FMC_ADDRL, spi_start_addr);
	fmc_pr(DMA_DB, "  Set ADDRL[%#x]%#x\n", FMC_ADDRL, spi_start_addr);

	regval = fmc_data_num_cnt(size);
	fmc_write(host, FMC_DATA_NUM, regval);
	fmc_pr(DMA_DB, "  Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, regval);

	regval = op_ctrl_rd_opcode(spi->read->cmd) |
		 op_ctrl_dma_op(OP_TYPE_REG) |
		 op_ctrl_rw_op(RW_OP_READ) |
		 OP_CTRL_DMA_OP_READY;
	fmc_write(host, FMC_OP_CTRL, regval);
	fmc_pr(DMA_DB, "  Set OP_CTRL[%#x]%#x\n", FMC_OP_CTRL, regval);

	fmc_cmd_wait_cpu_finish(host);

	memcpy(buffer, host->iobase, size);

	fmc_pr(DMA_DB, "*-End reg page read.\n");

	return (char*)buffer;
}

/*****************************************************************************/
static int fmc100_reg_read(struct spi_flash *spiflash, u_int from,
				size_t len, char *buf)
{
	int num;
	int chip_idx = 0; 
	int result = -EIO;
	unsigned char *ptr = (u_char *)buf;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;
	unsigned char *fmc_ip;

	if (((from + len) > spiflash->size) || (!len)) {
		db_msg("read area out of range or len is zero\n");
		return -EINVAL;
	}

	fmc_ip = get_fmc_ip();
	if (*fmc_ip) {
		printf("Warning: Fmc IP is busy, Please try again.\n");
		udelay(100); /* delay 100 us */
		return;
	} else {
		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);
		(*fmc_ip)++;
	}

	if (spi->driver->wait_ready(spi)) {
		db_msg("Error: Dma read wait ready fail!\n");
		result = -EBUSY;
		goto fail;
	}

	host->set_system_clock(spi->read, ENABLE);

	while (len > 0) {
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			chip_idx++;
			spi++;
			if ((chip_idx == CONFIG_SPI_NOR_MAX_CHIP_NUM) || (!spi->name)) {
				db_bug("read memory out of range.\n");
				goto fail;
			}
			if (spi->driver->wait_ready(spi))
				goto fail;
			host->set_system_clock(spi->read, ENABLE);
		}

		num = ((from + len) >= spi->chipsize) ? (spi->chipsize - from) : len;

		while (num >= FMC100_REG_RD_MAX_SIZE) {
			fmc100_reg_read_buf(host, spi,
					      from, FMC100_REG_RD_MAX_SIZE, ptr);
			ptr += FMC100_REG_RD_MAX_SIZE;
			from += FMC100_REG_RD_MAX_SIZE;
			len -= FMC100_REG_RD_MAX_SIZE;
			num -= FMC100_REG_RD_MAX_SIZE;
		}

		if (num) {
			fmc100_reg_read_buf(host, spi, from, num, ptr);
			from += num;
			ptr += num;
			len -= num;
		}
	}
	result = 0;

fail:
	(*fmc_ip)--;
	return result;
}
#endif
static int dma_cycle_op(struct spi_flash *spiflash, unsigned char rw_op,
				u_int from, size_t len, const void *buf)
{
	int op_len = 0;
	size_t num = 0;
	int chip_idx = 0;
	struct spi_op *op = NULL;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;

	if (rw_op == RW_OP_READ) {
		op_len = FMC100_DMA_RD_MAX_SIZE;
		op = spi->read;
	} else {
		op_len = FMC100_DMA_WR_MAX_SIZE;
		op = spi->write;
	}

	while (len) {
		num = ((len >= op_len) ? op_len : len);

		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			chip_idx++;
			spi++;
			if ((chip_idx == CONFIG_SPI_NOR_MAX_CHIP_NUM) || (!spi->name)) {
				db_bug("read memory out of range.\n");
				return -1;
			}

			if (spi->driver->wait_ready(spi)) {
				db_msg("Error: Dma op wait ready fail!!\n");
				return -EBUSY;
			}

			host->set_system_clock(op, ENABLE);
		}

		if (from + num > spi->chipsize)
			num = spi->chipsize - from;

		fmc100_dma_transfer(spi, from, (u_char *)buf,
				rw_op, num);
		from += num;
		buf  += num;
		len  -= num;
	}
	return 0;
}

/*****************************************************************************/
#ifndef FMC100_SPI_NOR_SUPPORT_REG_READ
static int fmc100_dma_read(struct spi_flash *spiflash, u_int from, size_t len,
				unsigned char *buf)
{
	int result = -EIO;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;
	unsigned char *fmc_ip = NULL;

	fmc_pr(RD_DBG, "\t|*-Start dma read, from:%#x len:%#lx\n", from, (long)len);

	if (((from + len) > spiflash->size) || (!len)) {
		db_msg("read area out of range[%#x].\n", spiflash->size);
		return -EINVAL;
	}

	fmc_ip = get_fmc_ip();
	if (*fmc_ip) {
		printf("Warning: Fmc IP is busy, Please try again.\n");
		udelay(100); /* delay 100 us */
		return -EBUSY;
	} else {
		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);
		(*fmc_ip)++;
	}

	if (spi->driver->wait_ready(spi)) {
		db_msg("Error: Dma read wait ready fail!\n");
		result = -EBUSY;
		goto fail;
	}

	host->set_system_clock(spi->read, ENABLE);

#ifndef CONFIG_SYS_DCACHE_OFF
	invalidate_dcache_range((uintptr_t)buf, (uintptr_t)(buf + len));
#endif
	if (dma_cycle_op(spiflash, RW_OP_READ, from, len, buf))
		goto fail;

#ifndef CONFIG_SYS_DCACHE_OFF
	invalidate_dcache_range((uintptr_t)buf, (uintptr_t)(buf + len));
#endif

	result = 0;
fail:
	(*fmc_ip)--;

	fmc_pr(RD_DBG, "\t|*-End dma read.\n");

	return result;
}
#endif
/*****************************************************************************/
void fmc100_read_ids(struct fmc_spi *spi, u_char cs, u_char *id)
{
	unsigned int reg;
	struct fmc_host *host = spi->host;

	fmc_pr(BT_DBG, "\t|||*-Start Read SPI(cs:%d) ID.\n", cs);

	reg = fmc_cmd_cmd1(SPI_CMD_RDID);
	fmc_write(host, FMC_CMD, reg);
	fmc_pr(BT_DBG, "\t||||-Set CMD[%#x]%#x\n", FMC_CMD, reg);

	reg = op_cfg_fm_cs(cs) | OP_CFG_OEN_EN;
	fmc_write(host, FMC_OP_CFG, reg);
	fmc_pr(BT_DBG, "\t||||-Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, reg);

	reg = fmc_data_num_cnt(MAX_SPI_NOR_ID_LEN);
	fmc_write(host, FMC_DATA_NUM, reg);
	fmc_pr(BT_DBG, "\t||||-Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, reg);

	reg = fmc_op_cmd1_en(ENABLE) |
	      fmc_op_read_data_en(ENABLE) |
	      FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, reg);
	fmc_pr(BT_DBG, "\t||||-Set OP[%#x]%#x\n", FMC_OP, reg);

	fmc_cmd_wait_cpu_finish(host);

	memcpy(id, host->iobase, MAX_SPI_NOR_ID_LEN);

	fmc_pr(BT_DBG, "\t|||*-Read CS: %d ID: %#X %#X %#X %#X %#X %#X\n",
	       cs, id[0], id[1], id[2], id[3], id[4], id[5]); /* id 1 2 3 4 5 6 */
}

/*****************************************************************************/
static int fmc100_reg_erase_one_block(struct fmc_host *host,
				struct fmc_spi *spi, unsigned int offset)
{
	unsigned int regval;

	fmc_pr(OP_DBG, "\t\t * Start erase one block, offset:%#x\n", offset);

	regval = spi->driver->wait_ready(spi);
	if (regval) {
		db_msg("Error: Erase wait ready fail! reg:%#x\n", regval);
		return 1;
	}

	spi->driver->write_enable(spi);

	host->set_system_clock(spi->erase, ENABLE);

	regval = fmc_cmd_cmd1(spi->erase->cmd);
	fmc_write(host, FMC_CMD, regval);
	fmc_pr(OP_DBG, "\t\t   Set CMD[%#x]%#x\n", FMC_CMD, regval);

	regval = offset;
	fmc_write(host, FMC_ADDRL, regval);
	fmc_pr(OP_DBG, "\t\t   Set ADDRL[%#x]%#x\n", FMC_ADDRL, regval);

	regval = op_cfg_fm_cs(spi->chipselect) |
		 OP_CFG_OEN_EN |
		 op_cfg_mem_if_type(spi->erase->iftype) |
		 op_cfg_addr_num(spi->addrcycle) |
		 op_cfg_dummy_num(spi->erase->dummy);
	fmc_write(host, FMC_OP_CFG, regval);
	fmc_pr(OP_DBG, "\t\t   Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, regval);

	regval = fmc_op_cmd1_en(ENABLE) |
		 fmc_op_addr_en(ENABLE) |
		 FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, regval);
	fmc_pr(OP_DBG, "\t\t   Set OP[%#x]%#x\n", FMC_OP, regval);

	fmc_cmd_wait_cpu_finish(host);

	fmc_pr(OP_DBG, "\t\t * End erase one block.\n");

	return 0;
}

/*****************************************************************************/
#ifndef FMC100_SPI_NOR_SUPPORT_REG_WRITE
static int fmc100_dma_write(struct spi_flash *spiflash, u_int to, size_t len,
				const unsigned char *buf)
{
	int result = -EIO;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;
	unsigned char *fmc_ip = NULL;

	fmc_pr(WR_DBG, "\n\t\t* Start dma write, to:%#x len:%#lx\n", to, (long)len);

	if (((to + len) > spiflash->size) || (!len)) {
		db_msg("write data out of range or len is zero.\n");
		return -EINVAL;
	}

#ifdef CONFIG_SPI_BLOCK_PROTECT
	if (host->level && (to < host->end_addr)) {
		printf("\nERROR: The DMA write area was locked.\n");
		return -EINVAL;
	}
#endif

	fmc_ip = get_fmc_ip();
	if (*fmc_ip) {
		printf("Warning: Fmc IP is busy, Please try again.\n");
		udelay(100); /* delay 100 us */
		return -EBUSY;
	} else {
		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);
		(*fmc_ip)++;
	}

	if (spi->driver->wait_ready(spi))
		goto fail;

	spi->driver->write_enable(spi);
	host->set_system_clock(spi->write, ENABLE);

#ifndef CONFIG_SYS_DCACHE_OFF
	flush_dcache_range((uintptr_t)buf, (uintptr_t)(buf + len));
#endif

	if (dma_cycle_op(spiflash, RW_OP_WRITE, to, len, buf))
		goto fail;

	result = 0;
fail:
	(*fmc_ip)--;
	fmc_pr(WR_DBG, "\t\t* End dma write.\n");
	return result;
}
#endif
/*****************************************************************************/
#ifdef FMC100_SPI_NOR_SUPPORT_REG_WRITE
static int fmc100_reg_write_buf(struct fmc_host *host,
				struct fmc_spi *spi, u_int spi_start_addr,
				size_t size, unsigned char *buffer)
{
	if (size > FMC100_DMA_WR_MAX_SIZE)
		db_bug("reg read out of reg range.\n");

	if (spi->driver->wait_ready(spi))
		return 1;

	memcpy((char *)host->iobase, buffer, size);

	spi->driver->write_enable(spi);

	fmc_write(host, FMC_INT_CLR, FMC_INT_CLR_ALL);

	fmc_write(host, FMC_CMD, fmc_cmd_cmd1(spi->write->cmd));

	fmc_write(host, FMC_OP_CFG, op_cfg_fm_cs(spi->chipselect) |
			op_cfg_mem_if_type(spi->write->iftype) | OP_CFG_OEN_EN);

	fmc_write(host, FMC_ADDRL, spi_start_addr);

	fmc_write(host, FMC_OP_CTRL, (op_ctrl_wr_opcode(spi->write->cmd) |
					op_ctrl_dma_op(OP_TYPE_REG) |
					op_ctrl_rw_op(RW_OP_WRITE) |
					OP_CTRL_DMA_OP_READY));

	FMC_DMA_WAIT_CPU_FINISH(host);

	return 0;
}

static int cycle_write(struct fmc_spi *spi, u_int to, size_t len,
				unsigned char *ptr, int num)
{
	int chip_idx = 0;

	while (to >= spi->chipsize) {
		to -= spi->chipsize;
		chip_idx++;
		spi++;
		if ((chip_idx == CONFIG_SPI_NOR_MAX_CHIP_NUM) || (!spi->name)) {
			db_bug("write memory out of range.\n");
			return -1;
		}

		if (spi->driver->wait_ready(spi))
			return -1;

		host->set_system_clock(spi->write, ENABLE);
	}

	if (fmc100_reg_write_buf(host, spi, to, num, ptr))
		return -1;

	to += num;
	ptr += num;
	len -= num;

	return 0;
}
/*****************************************************************************/
static int fmc100_reg_write(struct spi_flash *spiflash, u_int to, size_t len,
				const unsigned char *buf)
{
	int num;
	int result = -EIO;
	unsigned char *ptr = (unsigned char *)buf;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;
	unsigned char *fmc_ip;

	if ((to + len) > spiflash->size) {
		db_msg("write data out of range.\n");
		return -EINVAL;
	}

	if (!len) {
		db_msg("write length is 0.\n");
		return 0;
	}

	if (write_bp_area_check(host, to, len))
		return -EINVAL;

	fmc_ip = get_fmc_ip();
	if (*fmc_ip) {
		printf("Warning: Fmc IP is busy, Please try again.\n");
		udelay(100); /* delay 100 us */
		return;
	} else {
		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);
		(*fmc_ip)++;
	}

	if (spi->driver->wait_ready(spi))
		goto fail;

	host->set_system_clock(spi->write, ENABLE);

	if (to & FMC100_DMA_WR_MASK) {
		num = FMC100_DMA_WR_MAX_SIZE - (to & FMC100_DMA_WR_MASK);
		if (num > (int)len)
			num = (int)len;

		if (cycle_write(spi, to, len, ptr, num)) {
			result = -1;
			goto fail;
		}
	}

	while (len > 0) {
		num = ((len >= FMC100_DMA_WR_MAX_SIZE) ?
		       FMC100_DMA_WR_MAX_SIZE : len);

		if (cycle_write(spi, to, len, ptr, num)) {
			result = -1;
			goto fail;
		}
	}
	result = 0;
fail:
	(*fmc_ip)--;
	return result;
}
#endif /* FMC100_SPI_NOR_SUPPORT_REG_WRITE */

/*****************************************************************************/
static int fmc100_reg_erase(struct spi_flash *spiflash, u_int offset,
				size_t length)
{
	int chip_idx = 0;
	struct fmc_host *host = spiflash_to_host(spiflash);
	struct fmc_spi *spi = host->spi;
	unsigned char *fmc_ip = NULL;

	if (offset + length > spiflash->size) {
		db_msg("Error: Erase area out of range of mtd.\n");
		return -EINVAL;
	}

	if (offset & (spi->erasesize - 1)) {
		db_msg("Error: Erase start address is not alignment.\n");
		return -EINVAL;
	}

	if (length & (spi->erasesize - 1)) {
		db_msg("Error: Erase length is not alignment.\n");
		return -EINVAL;
	}

#ifdef CONFIG_SPI_BLOCK_PROTECT
	if ((host->level) && (offset < host->end_addr)) {
		printf("\nERROR: The erase area was locked.\n");
		return -EINVAL;
	}
#endif
	fmc_ip = get_fmc_ip();
	if (*fmc_ip) {
		printf("Warning: Fmc IP is busy, Please try again.\n");
		udelay(100); /* delay 100 us */
		return -EBUSY;
	} else {
		fmc_dev_type_switch(FLASH_TYPE_SPI_NOR);
		(*fmc_ip)++;
	}

	while (length) {
		if (spi->chipsize <= offset) {
			offset -= spi->chipsize;
			chip_idx++;
			spi++;
			if ((chip_idx == CONFIG_SPI_NOR_MAX_CHIP_NUM) || (!spi->name)) {
				db_bug("Error: Erase memory out of range.\n");
				(*fmc_ip)--;
				return -1;
			}
		}

		if (fmc100_reg_erase_one_block(host, spi, offset)) {
			(*fmc_ip)--;
			return -1;
		}

		offset += spi->erase->size;
		length -= spi->erase->size;
	}

	(*fmc_ip)--;

	return 0;
}

/*****************************************************************************/
static void spi_nor_func_hook(struct fmc_host *host)
{
	struct spi_flash *spi_nor_flash = host->spi_nor_flash;

#ifdef CONFIG_SPI_BLOCK_PROTECT
	host->cmp = BP_CMP_UPDATE_FLAG;
	fmc100_get_bp_lock_level(host);
	spi_nor_flash->lock = fmc100_spi_flash_lock;
#endif
	fmc_pr(BT_DBG, "\t||-Initial spi_flash structure\n");
	spi_nor_flash->name = "bsp_fmc";
	spi_nor_flash->erase = fmc100_reg_erase;
#ifdef FMC100_SPI_NOR_SUPPORT_REG_WRITE
	spi_nor_flash->write = fmc100_reg_write;
#else
	spi_nor_flash->write = fmc100_dma_write;
#endif

#ifdef FMC100_SPI_NOR_SUPPORT_REG_READ
	spi_nor_flash->read = fmc100_reg_read;
#else
	spi_nor_flash->read = fmc100_dma_read;
#endif
}

/*****************************************************************************/
struct spi_flash *fmc100_spi_nor_scan(struct fmc_host *host)
{
	unsigned char cs;
	struct spi_flash *spi_nor_flash = host->spi_nor_flash;
	struct fmc_spi *spi = host->spi;
	struct mtd_info_ex *spi_nor_info = host->spi_nor_info;

	fmc_pr(BT_DBG, "\t|*-Start Scan SPI nor flash\n");

	spi_nor_flash->size = 0;

	for (cs = 0; cs < CONFIG_SPI_NOR_MAX_CHIP_NUM; cs++)
		host->spi[cs].host = host;

	fmc_pr(BT_DBG, "\t||-Initial mtd_info_ex structure\n");
	memset(spi_nor_info, 0, sizeof(struct mtd_info_ex));

	if (!fmc_spi_nor_probe(spi_nor_info, spi)) {
#ifndef CONFIG_SPI_NOR_QUIET_TEST
		printf("Can't find a valid spi nor flash chip.\n");
#endif
		return NULL;
	}

	if (spi->addrcycle == SPI_NOR_4BYTE_ADDR_LEN) {
		host->set_host_addr_mode(host, ENABLE);
		fmc_pr(AC_DBG, "\t* Controller entry 4-byte mode.\n");
	}

	spi_nor_func_hook(host);

#ifdef CONFIG_DTR_MODE_SUPPORT
	if (spi->dtr_mode_support) {
		int ret;

		host->dtr_training_flag = 0;
		/* setting DTR mode dummy and traning */
		ret = spi_dtr_dummy_training_set(host, ENABLE);
		/* If training setting fail, must reset back to SDR mode,
		 * Note: logic auto turn on DTR when read
		 *       auto turn off DTR when write and erase */
		if (ret) {
			printf("Reset to STR mode.\n");
			/* switch DTR mode to SDR mode */
			fmc_dtr_mode_ctrl(spi, DISABLE);
			spi_dtr_to_sdr_switch(spi);
			spi_dtr_dummy_training_set(host, DISABLE);
		}
	}
#endif

	fmc_pr(BT_DBG, "\t|*-Found SPI nor flash: %s\n", spi_nor_info->name);

	return spi_nor_flash;
}

/*****************************************************************************/
static void fmc100_set_host_addr_mode(struct fmc_host *host, int enable)
{
	unsigned int regval = fmc_read(host, FMC_CFG);

	/* 1: SPI_NOR_ADDR_MODE_4_BYTES 0: SPI_NOR_ADDR_MODE_3_BYTES */
	if (enable)
		regval |= SPI_NOR_ADDR_MODE_MASK;
	else
		regval &= ~SPI_NOR_ADDR_MODE_MASK;

	fmc_write(host, FMC_CFG, regval);
}

/*****************************************************************************/
static int fmc100_host_init(struct fmc_host *host)
{
	unsigned int reg, flash_type;

	fmc_pr(BT_DBG, "\t|||*-Start SPI Nor host init\n");

	host->regbase = (void *)CONFIG_FMC_REG_BASE;
	host->iobase = (void *)CONFIG_FMC_BUFFER_BASE;

	reg = fmc_read(host, FMC_CFG);
	fmc_pr(BT_DBG, "\t||||-Get CFG[%#x]%#x\n", FMC_CFG, reg);
	flash_type = (reg & FLASH_SEL_MASK) >> FLASH_SEL_SHIFT;
	if (flash_type != FLASH_TYPE_SPI_NOR) {
		db_msg("Error: Flash type isn't SPI Nor. reg: %#x\n", reg);
		return -ENODEV;
	}

	if ((reg & OP_MODE_MASK) == OP_MODE_BOOT) {
		reg |= fmc_cfg_op_mode(OP_MODE_NORMAL);
		fmc_pr(BT_DBG, "\t||||-Controller enter normal mode\n");
		fmc_write(host, FMC_CFG, reg);
		fmc_pr(BT_DBG, "\t||||-Set CFG[%#x]%#x\n", FMC_CFG, reg);
	}

	host->set_system_clock = fmc_set_fmc_system_clock;
	host->set_host_addr_mode = fmc100_set_host_addr_mode;

	reg = timing_cfg_tcsh(CS_HOLD_TIME) |
	      timing_cfg_tcss(CS_SETUP_TIME) |
	      timing_cfg_tshsl(CS_DESELECT_TIME);
	fmc_write(host, FMC_SPI_TIMING_CFG, reg);
	fmc_pr(BT_DBG, "\t||||-Set TIMING[%#x]%#x\n", FMC_SPI_TIMING_CFG, reg);

	fmc_pr(BT_DBG, "\t|||*-End SPI Nor host init\n");

	return 0;
}

/*****************************************************************************/
int fmc100_spi_nor_init(struct fmc_host *host)
{
	int ret;

	fmc_pr(BT_DBG, "\t||*-Start fmc100 SPI Nor init\n");

	fmc_pr(BT_DBG, "\t|||-fmc100 host structure init\n");
	ret = fmc100_host_init(host);
	if (ret) {
		db_msg("Error: SPI Nor host init failed, result: %d\n", ret);
		return ret;
	}

	fmc_pr(BT_DBG, "\t|||-Set default system clock, Enable controller\n");
	if (host->set_system_clock)
		host->set_system_clock(NULL, ENABLE);

	fmc_pr(BT_DBG, "\t||*-End fmc100 SPI Nor init\n");

	return ret;
}

#ifdef CONFIG_SPI_BLOCK_PROTECT
/*****************************************************************************/
void spi_lock_update_address(struct fmc_host *host)
{
	unsigned int lock_level_max, erasesize, chipsize;
	unsigned char mid = host->spi_nor_info->ids[0];
	struct fmc_spi *spi = host->spi;

	if (!host->level) {
		host->end_addr = 0;
		fmc_pr(BP_DBG, "all blocks is unlocked.\n");
		return;
	}

	chipsize = spi->chipsize;
	erasesize = spi->erasesize;
	lock_level_max = host->spi_nor_flash[0].bp_level_max;

	switch (mid) {
	case MID_MXIC:
		if (spi->chipsize == _2M) {
			if ((host->level != lock_level_max) &&
			   (host->level != 1))
				host->end_addr = chipsize - (erasesize <<
					(lock_level_max - host->level - 1));
			else
				host->end_addr = chipsize;
			return;
		}

		if (spi->chipsize != _8M)
			break;
	/* general case */
	case MID_ESMT:
		/* this case is for ESMT and MXIC 8M devices */
		if (host->level != lock_level_max)
			host->end_addr = chipsize - (erasesize  <<
				(lock_level_max - host->level));
		else
			host->end_addr = chipsize;
		return;
	case MID_CFEON:
		if (host->level != lock_level_max)
			host->end_addr = chipsize - (erasesize  <<
							(host->level - 1));
		else
			host->end_addr = chipsize;
		return;
	default:
		break;
	}

	/* general case */
	host->end_addr = chipsize >> (lock_level_max - host->level);
}

/*****************************************************************************/
void fmc100_get_bp_lock_level(struct fmc_host *host)
{
	struct fmc_spi *spi = host->spi;
	unsigned char mid = host->spi_nor_info->ids[0];
	unsigned int lock_level_max;

	fmc_pr(BP_DBG, "Get manufacturer ID: [%#x]\n", mid);

	/* match the manufacture ID to get the block protect info */
	switch (mid) {
	case MID_GD:
	case MID_ESMT:
	case MID_CFEON:
	case MID_SPANSION:
	case MID_FM:
	case MID_XTX:
	case MID_SK:
		host->bp_num = BP_NUM_3;
		host->level = fmc100_bp_to_level(host);
		break;
	case MID_WINBOND:
		if (spi->chipsize <= _16M) {
			host->bp_num = BP_NUM_3;
			host->level = fmc100_bp_to_level(host);
		} else {
			host->bp_num = BP_NUM_4;
			host->level = fmc100_bp_to_level(host);
		}
		break;
	case MID_MXIC:
		if (spi->chipsize <= _8M) {
			host->bp_num = BP_NUM_3;
			host->level = fmc100_bp_to_level(host);
		} else {
			host->bp_num = BP_NUM_4;
			host->level = fmc100_bp_to_level(host);
		}
		break;
	case MID_MICRON:
	case MID_PARAGON:
		return;
	default:
		goto usage;
	}

	/* this branch only for initialization */
	if (host->cmp == BP_CMP_UPDATE_FLAG) {
		/* get the max block protect level of current manufacture ID */
		if (host->bp_num == BP_NUM_4) {
			lock_level_max = lock_level_max(host->bp_num) - BP_NUM_5;
			/* just for MXIC(16M), the max lock level is 9 */
			if ((mid == MID_MXIC) && (spi->chipsize == _16M))
				lock_level_max--;
		} else {
			lock_level_max = lock_level_max(host->bp_num);
		}

		host->spi_nor_flash[0].bp_level_max = lock_level_max;
		fmc_pr(BP_DBG, "Get the max bp level: [%d]\n", lock_level_max);

		spi_lock_update_address(host);
		if (host->end_addr)
			printf("Spi is locked. lock address[0 => %#x]\n",
			       host->end_addr);
	}
	return;
usage:
	db_msg("Error:The ID: %#x isn't in the BP table,\n", mid);
	db_msg("Current device can't not protect\n");
}
/*****************************************************************************/
unsigned short fmc100_set_spi_lock_info(struct fmc_host *host)
{
	unsigned short val;
	unsigned char mid = host->spi_nor_info->ids[0];
	struct fmc_spi *spi = host->spi;

	fmc_pr(BP_DBG, "Get manufacturer ID: [%#x]\n", mid);

	/* match the manufacture ID to get the block protect set info */
	switch (mid) {
	case MID_SPANSION:
		val = fmc100_handle_bp_rdcr_info(host, SPI_CMD_RDCR);
		break;
	case MID_MXIC:
		if (spi->chipsize < _16M)
			val = fmc100_handle_bp_rdsr_info(host, SPI_CMD_RDSR);
		else
			val = fmc100_handle_bp_rdcr_info(host,
							   SPI_CMD_RDCR_MX);
		break;
	case MID_GD:
	case MID_ESMT:
	case MID_CFEON:
	case MID_WINBOND:
	case MID_FM:
	case MID_XTX:
	case MID_SK:
		val = fmc100_handle_bp_rdsr_info(host, SPI_CMD_RDSR);
		break;
	default:
		goto usage;
	}

	return val;
usage:
	db_msg("Error: The manufacture ID is change,\n Pleaer check!!\n");
	db_msg("Error: The ID: %#x isn't in the BP table,\n", mid);
	return 1;
}

/*****************************************************************************/
unsigned short fmc100_handle_bp_rdcr_info(struct fmc_host *host, u_char cmd)
{
	unsigned char status, config;
	struct fmc_spi *spi = host->spi;
	unsigned char mid = host->spi_nor_info->ids[0];

	/* this macro definition is for determining the writing length */
	host->bt_loc = BT_LOC_RDCR;
	spi->driver->wait_ready(spi);

	/* get the block protect B/P info in config register */
	config = spi_general_get_flash_register(spi, cmd);
	fmc_pr(BP_DBG, "Get Config Register[%#x]\n", config);

	/* the location of T/B bit in config register is different.
	 SPANSION is 5th and the MXIC(16/32M) is 3th */
	if (mid == MID_SPANSION) {
		config = spi_bp_bottom_rdcr_set_s(config);
		fmc_pr(BP_DBG, "Set Config Register[%#x]\n", config);
	} else {
		config = spi_bp_bottom_rdcr_set(config);
		fmc_pr(BP_DBG, "Set Config Register[%#x]\n", config);
	}

	/* get the block protect level info in status register */
	status = spi_general_get_flash_register(spi, SPI_CMD_RDSR);
	fmc_pr(BP_DBG, "Get Status Register[%#x]\n", status);

	return ((unsigned short)config << SPI_NOR_CR_SHIFT) | status;
}
/*****************************************************************************/
unsigned short fmc100_handle_bp_rdsr_info(struct fmc_host *host,
				u_char cmd)
{
	unsigned char val;
	struct fmc_spi *spi = host->spi;
	unsigned char mid = host->spi_nor_info->ids[0];

	/* this macro definition is for determining the writing length */
	host->bt_loc = BT_LOC_RDSR;
	spi->driver->wait_ready(spi);

	/* get the block protect level and B/T info in status register */
	val = spi_general_get_flash_register(spi, cmd);
	fmc_pr(BP_DBG, "Get Status Register[%#x]\n", val);
	if (mid == MID_CFEON)
		val &= spi_bp_bottom_rdsr_set_0(host->bp_num);
	else
		val |= spi_bp_bottom_rdsr_set_1(host->bp_num);
	fmc_pr(BP_DBG, "Set Config Register[%#x]\n", val);

	return val;
}
/*****************************************************************************/
static void fmc100_set_bp_val(struct fmc_host *host, unsigned short val)
{
	unsigned int reg;
	struct fmc_spi *spi = host->spi;

	reg = fmc_read(host, FMC_GLOBAL_CFG);
	if (reg & FMC_GLOBAL_CFG_WP_ENABLE) {
		fmc_pr(BP_DBG, " Hardware protected enable!, reg[%#x]\n", reg);
		reg &= ~FMC_GLOBAL_CFG_WP_ENABLE;
		fmc_write(host, FMC_GLOBAL_CFG, reg);
		val &= ~SPI_NOR_SR_SRWD_MASK;
		fmc_pr(BP_DBG, "Disable SR[%d]:SRWD and WP#\n",
				SPI_NOR_SR_SRWD_SHIFT);
	}

	if (host->bt_loc == BT_LOC_RDSR) {
		writeb(val, host->iobase);
		fmc_pr(BP_DBG, "Write IO[%p]%#x\n", host->iobase,
				*(unsigned char *)host->iobase);
	} else {
		writew(val, host->iobase);
		fmc_pr(BP_DBG, "Write IO[%p]%#x\n", host->iobase,
				*(unsigned short *)host->iobase);
	}

	reg = fmc_cmd_cmd1(SPI_CMD_WRSR);
	fmc_write(host, FMC_CMD, reg);
	fmc_pr(BP_DBG, " Set CMD[%#x]%#x\n", FMC_CMD, reg);

	reg = op_cfg_fm_cs(spi->chipselect) | OP_CFG_OEN_EN;
	fmc_write(host, FMC_OP_CFG, reg);
	fmc_pr(BP_DBG, " Set OP_CFG[%#x]%#x\n", FMC_OP_CFG, reg);

	if (host->bt_loc == BT_LOC_RDSR)
		reg = fmc_data_num_cnt(SPI_NOR_SR_LEN);
	else
		reg = fmc_data_num_cnt(SPI_NOR_SR_LEN + SPI_NOR_CR_LEN);

	fmc_write(host, FMC_DATA_NUM, reg);
	fmc_pr(BP_DBG, " Set DATA_NUM[%#x]%#x\n", FMC_DATA_NUM, reg);

	reg = fmc_op_cmd1_en(ENABLE) 		|
		fmc_op_write_data_en(ENABLE) 	|
		FMC_OP_REG_OP_START;
	fmc_write(host, FMC_OP, reg);
	fmc_pr(BP_DBG, " Set OP[%#x]%#x\n", FMC_OP, reg);
}

/*****************************************************************************/
static void fmc100_set_bp_level(struct fmc_host *host, unsigned char level)
{
	unsigned char old_level;
	unsigned short val;
	struct fmc_spi *spi = host->spi;
	unsigned char mid = host->spi_nor_info->ids[0];

	fmc_pr(BP_DBG, "* Start BP bottom level %d\n", level);

	val = fmc100_set_spi_lock_info(host);
	old_level = host->level;
	fmc_pr(BP_DBG, "  Read CR:SR[%#x]\n", val);

	if (old_level != level) {
		if (host->bp_num == BP_NUM_3)
			val &= ~SPI_NOR_SR_BP_MASK_3;
		else
			val &= ~SPI_NOR_SR_BP_MASK_4;
		val |= level << SPI_NOR_SR_BP0_SHIFT;
		fmc_pr(BP_DBG, "Set Status Register[%#x]\n", val);
	} else {
		fmc_pr(BP_DBG, "NOTES: old_level[%#x] = level[%#x]\n",
		       old_level, level);
		return;
	}

	if (((mid == MID_ESMT) || ((mid == MID_MXIC)
				&& (spi->chipsize < _16M))) && (level == 0)) {
		val &= spi_bp_bottom_rdsr_set_0(host->bp_num);
		fmc_pr(BP_DBG, "set level = 0[PB3 = 0]:[%#x]\n", val);
	}

	spi->driver->write_enable(spi);
	fmc100_set_bp_val(host, val);
	fmc_cmd_wait_cpu_finish(host);
	fmc_pr(BP_DBG, "* Set BP level end.\n");
}

/*****************************************************************************/
void fmc100_spi_lock(struct fmc_host *host, unsigned char level)
{
	unsigned char current_level;

	fmc100_set_bp_level(host, level);

	/* check if we have set successfully or not */
	current_level = fmc100_bp_to_level(host);
	if (current_level != level) {
		db_msg("Error: Current lock level: %d, but set value: %d\n",
		       current_level, level);
		return;
	}

	host->level = level;
	spi_lock_update_address(host);

	if (host->end_addr)
		printf("Spi is locked. lock address[0 => %#x]\n",
		       host->end_addr);

	return;
}

/*****************************************************************************/
unsigned char fmc100_bp_to_level(struct fmc_host *host)
{
	unsigned char val;
	unsigned char level;
	struct fmc_spi *spi = host->spi;

	spi->driver->wait_ready(spi);
	val = spi_general_get_flash_register(spi, SPI_CMD_RDSR);
	fmc_pr(BP_DBG, "Get Status Register[%#x]\n", val);

	fmc_pr(BP_DBG, "the bp_num[%d]\n", host->bp_num);

	if (host->bp_num == BP_NUM_3)
		level = (val & SPI_NOR_SR_BP_MASK_3) >> SPI_NOR_SR_BP0_SHIFT;
	else
		level = (val & SPI_NOR_SR_BP_MASK_4) >> SPI_NOR_SR_BP0_SHIFT;

	fmc_pr(BP_DBG, "the current level[%d]\n", level);

	return level;
}
/*****************************************************************************/
#endif /* CONFIG_SPI_BLOCK_PROTECT */

#ifdef CONFIG_DTR_MODE_SUPPORT
/*****************************************************************************/
int spi_dtr_dummy_training_set(struct fmc_host *host, int dtr_en)
{
	struct fmc_spi *spi = host->spi;
	int ret;

	switch (spi->dtr_cookie) {
	case DTR_MODE_SET_ODS:
		if (spi->driver->dtr_set_device)
			spi->driver->dtr_set_device(spi, dtr_en);
		break;
	case DTR_MODE_SET_NONE:
	default:
		break;
	}

	/* disable DTR mode without training */
	/* dtr dummy training is done, return it */
	if ((host->dtr_training_flag == 1) || (dtr_en == DISABLE))
		return 0;

	/* enable DTR mode and set sample point */
	fmc_dtr_mode_ctrl(spi, ENABLE);

	/* set training */
	ret = spi_dtr_training(host);
	if (ret) {
		fmc_pr(DTR_DB, " * Set dtr training fail.\n");
		return 1;
	}
	fmc_pr(DTR_DB, "* Set dtr and dummy end.\n");
	return 0;
}
/*****************************************************************************/
static int select_sample_point(unsigned char *status, int len)
{
	int ix = 0;
	unsigned int p_count = 0;
	unsigned int p_temp = 0;
	unsigned int reg = 0;

	if (len <= 0)
		return 0;

	/* select the best smaple point */
	for (ix = 0; ix < len;) {
		if (status[ix] == 1) {
			p_count++;
			ix++;
			if ((status[ix] == 0) && (p_count > p_temp)) {
				p_temp = p_count;
				p_count = 0;
				reg = ix - ((p_temp + 1) >> 1);
				fmc_pr(DTR_DB, "the sample point choice: %#x\n",
				       reg);
				break;
			}
			continue;
		}
		ix++;
	}
	return reg;
}
/*****************************************************************************/
static int dtr_training_handle(struct fmc_host *host)
{
	int ret = 0;
	int ix = 0;
	unsigned int reg = 0;
	unsigned int regval;
	unsigned char *buf = NULL;
	unsigned char status[DTR_TRAINING_POINT_NUM] = {0};
	struct fmc_spi *spi = host->spi;

	spi->driver->wait_ready(spi);

	/* set div 4 clock */
	host->set_system_clock(spi->read, ENABLE);

	buf = memalign(CONFIG_SYS_CACHELINE_SIZE, DTR_TRAINING_CMP_LEN);
	if (!buf)
		return -1;

	/* start training to check every sample point */
	regval = fmc_read(host, FMC_GLOBAL_CFG);
	for (ix = 0; ix < DTR_TRAINING_POINT_NUM; ix++) {
		regval = dtr_training_point_clr(regval);
		regval |= (ix << DTR_TRAINING_POINT_MASK);
		fmc_pr(DTR_DB, " setting the dtr training point:%d\n", ix);
		fmc_write(host, FMC_GLOBAL_CFG, regval);
		fmc_pr(DTR_DB, " Set dtr_training[%#x]%#x\n",
		       FMC_GLOBAL_CFG, regval);
		/* read */

#ifndef CONFIG_SYS_DCACHE_OFF
		invalidate_dcache_range((uintptr_t)buf,
					(uintptr_t)(buf + DTR_TRAINING_CMP_LEN));
#endif

		fmc100_dma_transfer(spi, DTR_TRAINING_CMP_ADDR_SHIFT, buf,
				      RW_OP_READ, DTR_TRAINING_CMP_LEN);

#ifndef CONFIG_SYS_DCACHE_OFF
		invalidate_dcache_range((uintptr_t)buf,
					(uintptr_t)(buf + DTR_TRAINING_CMP_LEN));
#endif

		ret = memcmp((const void *)buf,
			     (const void *)DTR_TRAINING_CMP_ADDR_S,
			     DTR_TRAINING_CMP_LEN);
		if (ret == 0) {
			/* Just to reduce the use of variables, no other reasion */
			reg = 1;
			status[ix] = 1; /* like */
			fmc_pr(DTR_DB, " status[%d] = 1\n", ix);
		}
		if (!reg && (ix == DTR_TRAINING_POINT_NUM - 1))
			goto fail_training;
	}

	kfree(buf);

	/* select the best smaple point */
	reg = 0;
	reg = select_sample_point(status, DTR_TRAINING_POINT_NUM);

	/* to set the best sample point */
	regval = dtr_training_point_clr(regval);
	regval |= (reg << DTR_TRAINING_POINT_MASK);
	fmc_pr(DTR_DB, " set the sample point[%#x]%#x\n",
	       FMC_GLOBAL_CFG, regval);
	fmc_write(host, FMC_GLOBAL_CFG, regval);

	/* training handle end */
	return regval;

fail_training:
	printf("Cannot find an useful sample point.\n");
	kfree(buf);
	return -1;
}
/*****************************************************************************/
unsigned int spi_dtr_training(struct fmc_host *host)
{
	int reg, cur_reg;

	fmc_pr(DTR_DB, "DTR traiining start ...\n");
	/* DTR traiining start */
	reg = dtr_training_handle(host);
	if (reg == -1) {
		host->dtr_training_flag = 0;
		printf("DTR traiining fail.\n");
		return 1;
	}
	fmc_pr(DTR_DB, "* DTR traiining end.\n");
	cur_reg = fmc_read(host, FMC_GLOBAL_CFG);
	/* to check whether training is done */
	if (cur_reg == reg) {
		host->dtr_training_flag = 1;
		fmc_pr(DTR_DB, "* Set dtr_training seccess.\n");
		return 0;
	}
	return 1;
}
/*****************************************************************************/
void fmc_dtr_mode_ctrl(struct fmc_spi *spi, int dtr_en)
{
	unsigned int regval;
	struct fmc_host *host = (struct fmc_host *)spi->host;

	host->dtr_mode_en = dtr_en;
	regval = fmc_read(host, FMC_GLOBAL_CFG);
	if (dtr_en == ENABLE) {
		/* enable DTR mode and set the DC value */
		regval |= (1 << DTR_MODE_REQUEST_SHIFT);
		fmc_write(host, FMC_GLOBAL_CFG, regval);
		fmc_pr(DTR_DB, " enable dtr mode[%#x]%#x\n",
		       FMC_GLOBAL_CFG, regval);
	} else {
		/* disable DTR mode */
		regval &= (~(1 << DTR_MODE_REQUEST_SHIFT));
		fmc_write(host, FMC_GLOBAL_CFG, regval);
		fmc_pr(DTR_DB, " disable dtr mode[%#x]%#x\n",
		       FMC_GLOBAL_CFG, regval);
	}
}
/*****************************************************************************/
void fmc_check_spi_dtr_support(struct fmc_spi *spi, u_char *ids, int len)
{
	unsigned char manu_id = 0;
	unsigned char dev_id = 0;

	if(len < 2){
		return;
	}
	
	manu_id = ids[0];
	dev_id = ids[1];

	spi->dtr_mode_support = 0;
	spi->dtr_cookie = DTR_MODE_SET_NONE;

	switch (manu_id) {
	case MID_MXIC:
		if (spi_mxic_check_spi_dtr_support(spi)) {
			spi->dtr_cookie = DTR_MODE_SET_ODS;
			goto dtr_on;
		}
		break;
	case MID_WINBOND:
		/* Device ID: 0x70 means support DTR Mode for Winbond */
		if (dev_id == DEVICE_ID_SUPPORT_DTR_WINBOND) {
			spi->dtr_mode_support = 1;
			goto dtr_on;
		}
		break;
	default:
		break;
	}

	fmc_pr(DTR_DB, "The Double Transfer Rate Read Mode isn't supported.\n");
	return;

dtr_on:
	fmc_pr(FMC_INFO, "The Double Transfer Rate Read Mode is supported.\n");
}
#endif /* CONFIG_DTR_MODE_SUPPORT */
/*****************************************************************************/
void fmc100_op_reg(struct fmc_spi *spi, unsigned char opcode,
				unsigned int len, unsigned char optype)
{
	struct fmc_host *host = (struct fmc_host *)spi->host;
	u32 regval;

	regval = fmc_cmd_cmd1(opcode);
	fmc_write(host, FMC_CMD, regval);

	regval = fmc_data_num_cnt(len);
	fmc_write(host, FMC_DATA_NUM, regval);

	regval = op_cfg_fm_cs(spi->chipselect) | OP_CFG_OEN_EN;
	fmc_write(host, FMC_OP_CFG, regval);

	regval = fmc_op_cmd1_en(ENABLE) | FMC_OP_REG_OP_START | optype;
	fmc_write(host, FMC_OP, regval);

	fmc_cmd_wait_cpu_finish(host);
}
