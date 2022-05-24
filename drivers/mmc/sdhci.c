/*
 * Copyright 2011, Marvell Semiconductor Inc.
 * Lei Wen <leiwen@marvell.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Back ported to the 8xx platform (from the 8260 platform) by
 * Murray.Jensen@cmst.csiro.au, 27-Jan-01.
 */

#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <mmc.h>
#include <sdhci.h>

#if defined(CONFIG_FIXED_SDHCI_ALIGNED_BUFFER)
void *aligned_buffer = (void *)CONFIG_FIXED_SDHCI_ALIGNED_BUFFER;
#else
void *aligned_buffer;
#endif

static void sdhci_dumpregs(struct sdhci_host *host)
{
	printf("=========== REGISTER DUMP (mmc%d)===========\n", host->index);

	printf("Sys addr: 0x%08x | Version:  0x%08x\n",
			sdhci_readl(host, SDHCI_DMA_ADDRESS),
			sdhci_readw(host, SDHCI_HOST_VERSION));
	printf("Blk size: 0x%08x | Blk cnt:  0x%08x\n",
			sdhci_readw(host, SDHCI_BLOCK_SIZE),
			sdhci_readw(host, SDHCI_BLOCK_COUNT));
	printf("Argument: 0x%08x | Trn mode: 0x%08x\n",
			sdhci_readl(host, SDHCI_ARGUMENT),
			sdhci_readw(host, SDHCI_TRANSFER_MODE));
	printf("Present:  0x%08x | Host ctl: 0x%08x\n",
			sdhci_readl(host, SDHCI_PRESENT_STATE),
			sdhci_readb(host, SDHCI_HOST_CONTROL));
	printf("Power:    0x%08x | Blk gap:  0x%08x\n",
			sdhci_readb(host, SDHCI_POWER_CONTROL),
			sdhci_readb(host, SDHCI_BLOCK_GAP_CONTROL));
	printf("Wake-up:  0x%08x | Clock:    0x%08x\n",
			sdhci_readb(host, SDHCI_WAKE_UP_CONTROL),
			sdhci_readw(host, SDHCI_CLOCK_CONTROL));
	printf("Timeout:  0x%08x | Int stat: 0x%08x\n",
			sdhci_readb(host, SDHCI_TIMEOUT_CONTROL),
			sdhci_readl(host, SDHCI_INT_STATUS));
	printf("Int enab: 0x%08x | Sig enab: 0x%08x\n",
			sdhci_readl(host, SDHCI_INT_ENABLE),
			sdhci_readl(host, SDHCI_SIGNAL_ENABLE));
	printf("ACMD err: 0x%08x | Slot int: 0x%08x\n",
			sdhci_readw(host, SDHCI_ACMD12_ERR),
			sdhci_readw(host, SDHCI_SLOT_INT_STATUS));
	printf("Caps:     0x%08x | Caps_1:   0x%08x\n",
			sdhci_readl(host, SDHCI_CAPABILITIES),
			sdhci_readl(host, SDHCI_CAPABILITIES_1));
	printf("Cmd:      0x%08x | Max curr: 0x%08x\n",
			sdhci_readw(host, SDHCI_COMMAND),
			sdhci_readl(host, SDHCI_MAX_CURRENT));
	printf("Host ctl2: 0x%08x | ADMA Err: 0x%08x\n",
			sdhci_readw(host, SDHCI_HOST_CONTROL2),
			sdhci_readl(host, SDHCI_ADMA_ERROR));

	printf(" ADMA Ptr: 0x%08x_%08x\n",
			sdhci_readl(host, SDHCI_ADMA_ADDRESS_HI),
			sdhci_readl(host, SDHCI_ADMA_ADDRESS));

	printf("===========================================\n");
}

#ifdef CONFIG_SDHCI_ADMA
static void sdhci_adma_write_desc(void *desc,
		dma_addr_t addr, int len, unsigned int cmd)
{
#ifdef CONFIG_PHYS_64BIT
	struct sdhci_adma2_64_desc *dma_desc = desc;
#else
	struct sdhci_adma2_32_desc *dma_desc = desc;
#endif

	/* 32-bit and 64-bit descriptors have these members in same position */
	dma_desc->cmd = cpu_to_le16(cmd);
	dma_desc->len = cpu_to_le16(len);
	dma_desc->addr_lo = cpu_to_le32((u32)addr);

#ifdef CONFIG_PHYS_64BIT
	dma_desc->addr_hi = cpu_to_le32((u64)addr >> 32);
#endif
}

#define ADMA2_TRAN_VALID	0x21
#define ADMA2_NOP_END_VALID	0x3

static int sdhci_adma_table_pre(struct sdhci_host *host,
		struct mmc_data *data, unsigned int trans_bytes)
{
	dma_addr_t addr;
	void *desc = host->adma_table;
	unsigned int left = trans_bytes;
	int len;
	int is_aligned = 1;

	addr = (dma_addr_t)(uintptr_t)(data->flags == MMC_DATA_READ ?
			data->src : data->dest);

	/*
	 * If dma buffer isn't cache line aligned, set is_aligned to be zero,
	 * and return.
	 */
	if ((addr & (CONFIG_SYS_CACHELINE_SIZE - 1)) != 0x0) {
		is_aligned = 0;
		goto exit;
	}

	while (left >= host->max_seg_size) {
		if (((addr & (SDHCI_DMA_BOUNDARY_SIZE - 1))
			+ host->max_seg_size) > SDHCI_DMA_BOUNDARY_SIZE) {
			len = SDHCI_DMA_BOUNDARY_SIZE -
				    (addr & (SDHCI_DMA_BOUNDARY_SIZE - 1));
		} else
			len = host->max_seg_size;
		sdhci_adma_write_desc(desc, addr, len, ADMA2_TRAN_VALID);
		addr += len;
		left -= len;
		desc += host->desc_sz;
	}

	if (left) {
		if (((addr & (SDHCI_DMA_BOUNDARY_SIZE - 1))
				+ left) > SDHCI_DMA_BOUNDARY_SIZE) {
			len = SDHCI_DMA_BOUNDARY_SIZE -
				    (addr & (SDHCI_DMA_BOUNDARY_SIZE - 1));
			sdhci_adma_write_desc(desc, addr, len, ADMA2_TRAN_VALID);
			addr += len;
			left -= len;
			desc += host->desc_sz;
		}

		sdhci_adma_write_desc(desc, addr, left, ADMA2_TRAN_VALID);
		desc += host->desc_sz;
	}

	sdhci_adma_write_desc(desc, 0, 0, ADMA2_NOP_END_VALID);
exit:
	return is_aligned;
}

static int sdhci_prep_data(struct sdhci_host *host,
		struct mmc_data *data, unsigned int trans_bytes)
{
	unsigned char ctrl;
	unsigned long bytes;
	int is_aligned;

	/* If dma buffer isn't cache line aligned, don't use dma mode. */
	is_aligned = sdhci_adma_table_pre(host, data, trans_bytes);
	if (!is_aligned)
		goto exit;

	ctrl = sdhci_readb(host, SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_DMA_MASK;
#ifdef CONFIG_PHYS_64BIT
	ctrl |= SDHCI_CTRL_ADMA64;
#else
	ctrl |= SDHCI_CTRL_ADMA32;
#endif
	sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);

	sdhci_writel(host, (u32)(uintptr_t)host->adma_table, SDHCI_ADMA_ADDRESS);
#ifdef CONFIG_PHYS_64BIT
	sdhci_writel(host, (u64)(uintptr_t)host->adma_table >> 32, /* upper 32bits */
			SDHCI_ADMA_ADDRESS_HI);
#endif
	bytes = ALIGN(trans_bytes, CONFIG_SYS_CACHELINE_SIZE);
	if (data->flags != MMC_DATA_READ)
		flush_cache((unsigned long)(uintptr_t)data->src, bytes);
	else
		invalidate_dcache_range((unsigned long)(uintptr_t)data->dest,
				(unsigned long)(uintptr_t)data->dest + bytes);

	bytes = ALIGN(host->adma_table_sz, CONFIG_SYS_CACHELINE_SIZE);
	flush_cache((unsigned long)(uintptr_t)host->adma_table, bytes);

exit:
	return is_aligned;
}
#endif

static void sdhci_reset(struct sdhci_host *host, u8 mask)
{
	unsigned long timeout;

	/* Wait max 100 ms */
	timeout = 100;
	sdhci_writeb(host, mask, SDHCI_SOFTWARE_RESET);
	while (sdhci_readb(host, SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout == 0) {
			printf("%s: Reset 0x%x never completed.\n",
			       __func__, (int)mask);
			return;
		}
		timeout--;
		udelay(1000);
	}
}

static void sdhci_cmd_done(struct sdhci_host *host, struct mmc_cmd *cmd)
{
	int i;
	if (cmd->resp_type & MMC_RSP_136) {
		/* CRC is stripped so we need to do some shifting. */
		for (i = 0; i < 4; i++) {
			cmd->response[i] = sdhci_readl(host,
					SDHCI_RESPONSE + (3-i)*4) << 8;
			if (i != 3)
				cmd->response[i] |= sdhci_readb(host,
						SDHCI_RESPONSE + (3-i)*4-1);
		}
	} else {
		cmd->response[0] = sdhci_readl(host, SDHCI_RESPONSE);
	}
}

static void sdhci_transfer_pio(struct sdhci_host *host, struct mmc_data *data)
{
	int i;
	char *offs;
	for (i = 0; i < data->blocksize; i += 4) {
		offs = data->dest + i;
		if (data->flags == MMC_DATA_READ)
			*(u32 *)offs = sdhci_readl(host, SDHCI_BUFFER);
		else
			sdhci_writel(host, *(u32 *)offs, SDHCI_BUFFER);
	}
}

static int sdhci_transfer_data(struct sdhci_host *host, struct mmc_data *data,
				unsigned int start_addr)
{
	unsigned int stat, rdy, mask, timeout;
#ifdef CONFIG_MMC_SDMA
	unsigned char ctrl;
	ctrl = sdhci_readb(host, SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_DMA_MASK;
	sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);
#endif

	timeout = 1000000;
	rdy = SDHCI_INT_SPACE_AVAIL | SDHCI_INT_DATA_AVAIL;
	mask = SDHCI_DATA_AVAILABLE | SDHCI_SPACE_AVAILABLE;
	do {
		stat = sdhci_readl(host, SDHCI_INT_STATUS);
		if (stat & SDHCI_INT_ERROR) {
			if (!host->is_tuning) {
				printf("%s: Error detected in status(0x%X)!\n",
						__func__, stat);
				sdhci_dumpregs(host);
			}
			return -EIO;
		}
		if (stat & rdy) {
			if (!(sdhci_readl(host, SDHCI_PRESENT_STATE) & mask))
				continue;
			sdhci_writel(host, rdy, SDHCI_INT_STATUS);
			sdhci_transfer_pio(host, data);
			data->dest += data->blocksize;
		}
#ifdef CONFIG_MMC_SDMA
		if (stat & SDHCI_INT_DMA_END) {
			sdhci_writel(host, SDHCI_INT_DMA_END, SDHCI_INT_STATUS);
			start_addr &= ~(SDHCI_DEFAULT_BOUNDARY_SIZE - 1);
			start_addr += SDHCI_DEFAULT_BOUNDARY_SIZE;
			sdhci_writel(host, start_addr, SDHCI_DMA_ADDRESS);
		}
#endif
		if (timeout-- > 0)
			udelay(10);
		else {
			printf("%s: Transfer data timeout\n", __func__);
			sdhci_dumpregs(host);
			return -ETIMEDOUT;
		}
	} while (!(stat & SDHCI_INT_DATA_END));

	return 0;
}

/*
 * No command will be sent by driver if card is busy, so driver must wait
 * for card ready state.
 * Every time when card is busy after timeout then (last) timeout value will be
 * increased twice but only if it doesn't exceed global defined maximum.
 * Each function call will use last timeout value.
 */
#define SDHCI_CMD_MAX_TIMEOUT			3200
#define SDHCI_CMD_DEFAULT_TIMEOUT		100
#define SDHCI_READ_STATUS_TIMEOUT		1000

#ifdef CONFIG_DM_MMC_OPS
static int sdhci_send_command(struct udevice *dev, struct mmc_cmd *cmd,
			      struct mmc_data *data)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);

#else
static int sdhci_send_command(struct mmc *mmc, struct mmc_cmd *cmd,
			      struct mmc_data *data)
{
#endif
	struct sdhci_host *host = mmc->priv;
	unsigned int stat = 0;
	int ret = 0;
#if defined(CONFIG_SDHCI_ADMA) || defined(CONFIG_MMC_SDMA)
	unsigned int trans_bytes = 0;
	int is_aligned = 1;
#endif
	u32 mask, flags, mode;
	unsigned int time = 0;
	unsigned int start_addr = 0;
	unsigned int start;
	int mmc_dev = mmc_get_blk_desc(mmc)->devnum;

	/* Timeout unit - ms */
	static unsigned int cmd_timeout = SDHCI_CMD_DEFAULT_TIMEOUT;

	sdhci_writel(host, SDHCI_INT_ALL_MASK, SDHCI_INT_STATUS);
	mask = SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT;

	/* We shouldn't wait for data inihibit for stop commands, even
	   though they might use busy signaling */
	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		mask &= ~SDHCI_DATA_INHIBIT;

	while (sdhci_readl(host, SDHCI_PRESENT_STATE) & mask) {
		if (time >= cmd_timeout) {
			printf("%s: MMC: %d busy ", __func__, mmc_dev);
			if (2 * cmd_timeout <= SDHCI_CMD_MAX_TIMEOUT) {
				cmd_timeout += cmd_timeout;
				printf("timeout increasing to: %u ms.\n",
				       cmd_timeout);
			} else {
				puts("timeout.\n");
				return -ECOMM;
			}
		}
		time++;
		udelay(1000);
	}

	mask = SDHCI_INT_RESPONSE;
	if (!(cmd->resp_type & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->resp_type & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->resp_type & MMC_RSP_BUSY) {
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
		if (data)
			mask |= SDHCI_INT_DATA_END;
	} else
		flags = SDHCI_CMD_RESP_SHORT;

	if (cmd->resp_type & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->resp_type & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;
	if (data)
		flags |= SDHCI_CMD_DATA;

	/* Set Transfer mode regarding to data flag */
	if (data != 0) {
		sdhci_writeb(host, 0xe, SDHCI_TIMEOUT_CONTROL);
		mode = SDHCI_TRNS_BLK_CNT_EN;
#if defined(CONFIG_SDHCI_ADMA) || defined(CONFIG_MMC_SDMA)
		trans_bytes = data->blocks * data->blocksize;
#endif
		if (data->blocks > 1)
			mode |= SDHCI_TRNS_MULTI;

		if (data->flags == MMC_DATA_READ)
			mode |= SDHCI_TRNS_READ;

#ifdef CONFIG_SDHCI_ADMA
		/*
		 * If buffer is cache line aligned, use dma mode, otherwise,
		 * don't use dma mode.
		 */
		is_aligned = sdhci_prep_data(host, data, trans_bytes);
		if (is_aligned)
			mode |= SDHCI_TRNS_DMA;
#endif

#ifdef CONFIG_MMC_SDMA
		if (data->flags == MMC_DATA_READ)
			start_addr = (unsigned long)data->dest;
		else
			start_addr = (unsigned long)data->src;

		/*
		 * If buffer isn't cache line aligned , but defined
		 * CONFIG_FIXED_SDHCI_ALIGNED_BUFFER,
		 * use aligend_buffer to store the data for dma,
		 * otherwise, don't use dma mode.
		 */
		if ((start_addr & (CONFIG_SYS_CACHELINE_SIZE - 1)) != 0x0) {
#if defined(CONFIG_FIXED_SDHCI_ALIGNED_BUFFER)
			/*
			 * Always use this bounce-buffer when
			 * CONFIG_FIXED_SDHCI_ALIGNED_BUFFER is defined
			 */
			is_aligned = 0;
			start_addr = (unsigned long)aligned_buffer;
			if (data->flags != MMC_DATA_READ)
				memcpy(aligned_buffer, data->src, trans_bytes);
#else

			is_aligned = 0;
			goto no_dma;
#endif
		}


		sdhci_writel(host, start_addr, SDHCI_DMA_ADDRESS);
		mode |= SDHCI_TRNS_DMA;
no_dma:

#endif
		sdhci_writew(host, SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG,
					data->blocksize),
				SDHCI_BLOCK_SIZE);
		sdhci_writew(host, data->blocks, SDHCI_BLOCK_COUNT);
		sdhci_writew(host, mode, SDHCI_TRANSFER_MODE);
	} else if (cmd->resp_type & MMC_RSP_BUSY) {
		sdhci_writeb(host, 0xe, SDHCI_TIMEOUT_CONTROL);
	}

	sdhci_writel(host, cmd->cmdarg, SDHCI_ARGUMENT);

#ifdef CONFIG_MMC_SDMA
	/*
	 * Execute to here, if has defined CONFIG_FIXED_SDHCI_ALIGNED_BUFFER or
	 * is_aligned is 1, we should flush cache for start_addr. Otherwise, we do nothing.
	 */
#if defined(CONFIG_FIXED_SDHCI_ALIGNED_BUFFER)
	trans_bytes = ALIGN(trans_bytes, CONFIG_SYS_CACHELINE_SIZE);
	flush_cache(start_addr, trans_bytes);
#else
	if (is_aligned) {
		trans_bytes = ALIGN(trans_bytes, CONFIG_SYS_CACHELINE_SIZE);
		flush_cache(start_addr, trans_bytes);
	}
#endif

#endif
	sdhci_writew(host, SDHCI_MAKE_CMD(cmd->cmdidx, flags), SDHCI_COMMAND);
	start = get_timer(0);
	do {
		stat = sdhci_readl(host, SDHCI_INT_STATUS);
		if (stat & SDHCI_INT_ERROR)
			break;

		if (get_timer(start) >= SDHCI_READ_STATUS_TIMEOUT) {
			if (host->quirks & SDHCI_QUIRK_BROKEN_R1B) {
				return 0;
			} else {
				printf("%s: Timeout for status update!\n",
				       __func__);
				return -ETIMEDOUT;
			}
		}
	} while ((stat & mask) != mask);

	if ((stat & (SDHCI_INT_ERROR | mask)) == mask) {
		sdhci_cmd_done(host, cmd);
		sdhci_writel(host, mask, SDHCI_INT_STATUS);
	} else
		ret = -1;

	if (!ret && data)
		ret = sdhci_transfer_data(host, data, start_addr);

	if (host->quirks & SDHCI_QUIRK_WAIT_SEND_CMD)
		udelay(1000);

	stat = sdhci_readl(host, SDHCI_INT_STATUS);
	sdhci_writel(host, SDHCI_INT_ALL_MASK, SDHCI_INT_STATUS);
	if (!ret) {
		/*
		 * If used aligend_buffer, we should copy data from
		 * aliagned_buffer to dest buffer when executing dma
		 * read operation.
		 */
#if defined(CONFIG_FIXED_SDHCI_ALIGNED_BUFFER)
		if (!is_aligned && (data->flags == MMC_DATA_READ))
			memcpy(data->dest, aligned_buffer, trans_bytes);
#endif
		return 0;
	}

	sdhci_reset(host, SDHCI_RESET_CMD);
	sdhci_reset(host, SDHCI_RESET_DATA);
	if (stat & SDHCI_INT_TIMEOUT)
		return -ETIMEDOUT;
	else
		return -ECOMM;
}

static int sdhci_set_clock(struct mmc *mmc, unsigned int clock)
{
	struct sdhci_host *host = mmc->priv;
	unsigned int div, clk = 0, timeout, reg;

	/* Wait max 20 ms */
	timeout = 200;
	while (sdhci_readl(host, SDHCI_PRESENT_STATE) &
			   (SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT)) {
		if (timeout == 0) {
			printf("%s: Timeout to wait cmd & data inhibit\n",
			       __func__);
			return -EBUSY;
		}

		timeout--;
		udelay(100);
	}

	reg = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	reg &= ~(SDHCI_CLOCK_CARD_EN | SDHCI_CLOCK_INT_EN);
	sdhci_writew(host, reg, SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		return 0;

	if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) {
		/*
		 * Check if the Host Controller supports Programmable Clock
		 * Mode.
		 */
		if (host->clk_mul) {
			for (div = 1; div <= 1024; div++) {
				if ((mmc->cfg->f_max * host->clk_mul / div)
					<= clock)
					break;
			}

			/*
			 * Set Programmable Clock Mode in the Clock
			 * Control register.
			 */
			clk = SDHCI_PROG_CLOCK_MODE;
			div--;
		} else {
			/* Version 3.00 divisors must be a multiple of 2. */
			if (mmc->cfg->f_max <= clock) {
				div = 1;
			} else {
				for (div = 2;
				     div < SDHCI_MAX_DIV_SPEC_300;
				     div += 2) {
					if ((mmc->cfg->f_max / div) <= clock)
						break;
				}
			}
			div >>= 1;
		}
	} else {
		/* Version 2.00 divisors must be a power of 2. */
		for (div = 1; div < SDHCI_MAX_DIV_SPEC_200; div *= 2) {
			if ((mmc->cfg->f_max / div) <= clock)
				break;
		}
		div >>= 1;
	}

	if (host->set_clock)
		return host->set_clock(host, clock);

	clk |= (div & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
	clk |= ((div & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN)
		<< SDHCI_DIVIDER_HI_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	timeout = 20;
	while (!((clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printf("%s: Internal clock never stabilised.\n",
			       __func__);
			return -EBUSY;
		}
		timeout--;
		udelay(1000);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
	return 0;
}

static void sdhci_set_power(struct sdhci_host *host, unsigned short power)
{
	u8 pwr = 0;

	if (power != (unsigned short)-1) {
		switch (1 << power) {
		case MMC_VDD_165_195:
			pwr = SDHCI_POWER_180;
			break;
		case MMC_VDD_29_30:
		case MMC_VDD_30_31:
			pwr = SDHCI_POWER_300;
			break;
		case MMC_VDD_32_33:
		case MMC_VDD_33_34:
			pwr = SDHCI_POWER_330;
			break;
		}
	}

	if (pwr == 0) {
		sdhci_writeb(host, 0, SDHCI_POWER_CONTROL);
		return;
	}

	if (host->quirks & SDHCI_QUIRK_NO_SIMULT_VDD_AND_POWER)
		sdhci_writeb(host, pwr, SDHCI_POWER_CONTROL);

	pwr |= SDHCI_POWER_ON;

	sdhci_writeb(host, pwr, SDHCI_POWER_CONTROL);
}

static void sdhci_set_uhs_signaling(struct sdhci_host *host, u8 timing)
{
	u16 ctrl_2;

	ctrl_2 = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	/* Select Bus Speed Mode for host */
	ctrl_2 &= ~SDHCI_CTRL_UHS_MASK;
	if ((timing == MMC_TIMING_MMC_HS200) ||
			(timing == MMC_TIMING_UHS_SDR104))
		ctrl_2 |= SDHCI_CTRL_UHS_SDR104;
	else if (timing == MMC_TIMING_UHS_SDR12)
		ctrl_2 |= SDHCI_CTRL_UHS_SDR12;
	else if (timing == MMC_TIMING_UHS_SDR25)
		ctrl_2 |= SDHCI_CTRL_UHS_SDR25;
	else if (timing == MMC_TIMING_UHS_SDR50)
		ctrl_2 |= SDHCI_CTRL_UHS_SDR50;
	else if ((timing == MMC_TIMING_UHS_DDR50) ||
			(timing == MMC_TIMING_MMC_DDR52))
		ctrl_2 |= SDHCI_CTRL_UHS_DDR50;
	else if (timing == MMC_TIMING_MMC_HS400)
		ctrl_2 |= SDHCI_CTRL_HS400; /* Non-standard */
	sdhci_writew(host, ctrl_2, SDHCI_HOST_CONTROL2);
}

#ifdef CONFIG_DM_MMC_OPS
static int sdhci_set_ios(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);
#else
static void sdhci_set_ios(struct mmc *mmc)
{
#endif
	u32 ctrl;
	struct sdhci_host *host = mmc->priv;

	if (host->set_control_reg)
		host->set_control_reg(host);

	if (mmc->clock != host->clock)
		sdhci_set_clock(mmc, mmc->clock);

	/* Set bus width */
	ctrl = sdhci_readb(host, SDHCI_HOST_CONTROL);
	if (mmc->bus_width == 8) {
		ctrl &= ~SDHCI_CTRL_4BITBUS;
		if ((SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) ||
				(host->quirks & SDHCI_QUIRK_USE_WIDE8))
			ctrl |= SDHCI_CTRL_8BITBUS;
	} else {
		if ((SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) ||
				(host->quirks & SDHCI_QUIRK_USE_WIDE8))
			ctrl &= ~SDHCI_CTRL_8BITBUS;
		if (mmc->bus_width == 4)
			ctrl |= SDHCI_CTRL_4BITBUS;
		else
			ctrl &= ~SDHCI_CTRL_4BITBUS;
	}

	if (mmc->clock > 26000000)
		ctrl |= SDHCI_CTRL_HISPD;
	else
		ctrl &= ~SDHCI_CTRL_HISPD;

	if (host->quirks & SDHCI_QUIRK_NO_HISPD_BIT)
		ctrl &= ~SDHCI_CTRL_HISPD;

	sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);

	sdhci_set_uhs_signaling(host, mmc->timing);
#ifdef CONFIG_DM_MMC_OPS
	return 0;
#endif
}

static int sdhci_init(struct mmc *mmc)
{
	struct sdhci_host *host = mmc->priv;

	sdhci_reset(host, SDHCI_RESET_ALL);

	sdhci_set_power(host, fls(mmc->cfg->voltages) - 1);

	if (host->quirks & SDHCI_QUIRK_NO_CD) {
#if defined(CONFIG_PIC32_SDHCI)
		/* PIC32 SDHCI CD errata:
		 * - set CD_TEST and clear CD_TEST_INS bit
		 */
		sdhci_writeb(host, SDHCI_CTRL_CD_TEST, SDHCI_HOST_CONTROL);
#else
		unsigned int status;

		sdhci_writeb(host, SDHCI_CTRL_CD_TEST_INS | SDHCI_CTRL_CD_TEST,
			SDHCI_HOST_CONTROL);

		status = sdhci_readl(host, SDHCI_PRESENT_STATE);
		while ((!(status & SDHCI_CARD_PRESENT)) ||
		    (!(status & SDHCI_CARD_STATE_STABLE)) ||
		    (!(status & SDHCI_CARD_DETECT_PIN_LEVEL)))
			status = sdhci_readl(host, SDHCI_PRESENT_STATE);
#endif
	}

	if (host->priv_init)
		host->priv_init(host);

	/* Enable only interrupts served by the SD controller */
	sdhci_writel(host, SDHCI_INT_DATA_MASK | SDHCI_INT_CMD_MASK,
		     SDHCI_INT_ENABLE);
	/* Mask all sdhci interrupt sources */
	sdhci_writel(host, 0x0, SDHCI_SIGNAL_ENABLE);

	return 0;
}

static int sdhci_execute_tuning(struct mmc *mmc, unsigned int opcode)
{
	struct sdhci_host *host = mmc->priv;

	if (host->execute_tuning)
		return host->execute_tuning(host, opcode);

	return 0;
}

static int sdhci_card_busy(struct mmc *mmc)
{
	struct sdhci_host *host = mmc->priv;
	u32 present_state;

	/* Check whether DAT[0] is 0 */
	present_state = sdhci_readl(host, SDHCI_PRESENT_STATE);

	return !(present_state & SDHCI_DATA_0_LVL_MASK);
}

#ifdef CONFIG_DM_MMC_OPS
int sdhci_probe(struct udevice *dev)
{
	struct mmc *mmc = mmc_get_mmc_dev(dev);

	return sdhci_init(mmc);
}

const struct dm_mmc_ops sdhci_ops = {
	.send_cmd	= sdhci_send_command,
	.set_ios	= sdhci_set_ios,
};
#else
static const struct mmc_ops sdhci_ops = {
	.send_cmd	= sdhci_send_command,
	.set_ios	= sdhci_set_ios,
	.init		= sdhci_init,
	.hs400_enable_es = sdhci_hs400_enhanced_stobe,
	.execute_tuning = sdhci_execute_tuning,
	.card_busy = sdhci_card_busy,
};
#endif

int sdhci_setup_cfg(struct mmc_config *cfg, struct sdhci_host *host,
		u32 max_clk, u32 min_clk)
{
	u32 caps, caps_1;

	caps = sdhci_readl(host, SDHCI_CAPABILITIES);

#ifdef CONFIG_MMC_SDMA
	if (!(caps & SDHCI_CAN_DO_SDMA)) {
		printf("%s: Your controller doesn't support SDMA!!\n",
		       __func__);
		return -EINVAL;
	}
#endif
	if (host->quirks & SDHCI_QUIRK_REG32_RW)
		host->version =
			sdhci_readl(host, SDHCI_HOST_VERSION - 2) >> 16;
	else
		host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

	cfg->name = host->name;
#ifndef CONFIG_DM_MMC_OPS
	cfg->ops = &sdhci_ops;
#endif
	if (max_clk)
		cfg->f_max = max_clk;
	else {
		if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300)
			cfg->f_max = (caps & SDHCI_CLOCK_V3_BASE_MASK) >>
				SDHCI_CLOCK_BASE_SHIFT;
		else
			cfg->f_max = (caps & SDHCI_CLOCK_BASE_MASK) >>
				SDHCI_CLOCK_BASE_SHIFT;
		cfg->f_max *= 1000000;
	}
	if (cfg->f_max == 0) {
		printf("%s: Hardware doesn't specify base clock frequency\n",
		       __func__);
		return -EINVAL;
	}
	if (min_clk)
		cfg->f_min = min_clk;
	else {
		if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300)
			cfg->f_min = cfg->f_max / SDHCI_MAX_DIV_SPEC_300;
		else
			cfg->f_min = cfg->f_max / SDHCI_MAX_DIV_SPEC_200;
	}
	cfg->voltages = 0;
	if (caps & SDHCI_CAN_VDD_330)
		cfg->voltages |= MMC_VDD_32_33 | MMC_VDD_33_34;
	if (caps & SDHCI_CAN_VDD_300)
		cfg->voltages |= MMC_VDD_29_30 | MMC_VDD_30_31;
	if (caps & SDHCI_CAN_VDD_180)
		cfg->voltages |= MMC_VDD_165_195;

	if (host->quirks & SDHCI_QUIRK_BROKEN_VOLTAGE)
		cfg->voltages |= host->voltages;

	if (host->host_caps)
		cfg->host_caps |= host->host_caps;

	cfg->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;

	/*
	 * In case of Host Controller v3.00, find out whether clock
	 * multiplier is supported.
	 */
	if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) {
		caps_1 = sdhci_readl(host, SDHCI_CAPABILITIES_1);
		host->clk_mul = (caps_1 & SDHCI_CLOCK_MUL_MASK) >>
				SDHCI_CLOCK_MUL_SHIFT;
	}

	return 0;
}

#ifdef CONFIG_BLK
int sdhci_bind(struct udevice *dev, struct mmc *mmc, struct mmc_config *cfg)
{
	return mmc_bind(dev, mmc, cfg);
}
#else
int add_sdhci(struct sdhci_host *host, u32 max_clk, u32 min_clk)
{
	int ret;

	ret = sdhci_setup_cfg(&host->cfg, host, max_clk, min_clk);
	if (ret)
		return ret;

	host->mmc = mmc_create(&host->cfg, host);
	if (host->mmc == NULL) {
		printf("%s: mmc create fail!\n", __func__);
		return -ENOMEM;
	}

	return 0;
}
#endif
