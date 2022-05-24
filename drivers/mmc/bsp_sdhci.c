/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

static void bsp_enable_card_clk(struct sdhci_host *host)
{
	u16 clk;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void bsp_disable_card_clk(struct sdhci_host *host)
{
	u16 clk;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk &= ~SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void bsp_enable_internal_clk(struct sdhci_host *host)
{
	u16 clk;
	u16 timeout = 20;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk |= SDHCI_CLOCK_INT_EN | SDHCI_CLOCK_PLL_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	while (!(clk & SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printf("%s: Internal clock never stabilised.\n",
					__func__);
			return;
		}
		timeout--;
		udelay(1000); /* delay 1000us */
		clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	}
}

static void __maybe_unused bsp_disable_internal_clk(struct sdhci_host *host)
{
	u16 clk;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk &= ~SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void bsp_set_drv_phase(struct sdhci_host *host, unsigned int phase)
{
	uintptr_t crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_DRV_DLL_CTRL : REG_SDIO0_DRV_DLL_CTRL;
	reg = readl(crg_addr);
	reg &= ~SDIO_DRV_PHASE_SEL_MASK;
	reg |= sdio_drv_sel(phase);
	writel(reg, crg_addr);
}

static void bsp_enable_sample(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_AT_CTRL);
	reg |= SDHCI_SAMPLE_EN;
	sdhci_writel(host, reg, SDHCI_AT_CTRL);
}

static void bsp_set_sampl_phase(struct sdhci_host *host, unsigned int phase)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_AT_STAT);
	reg &= ~SDHCI_PHASE_SEL_MASK;
	reg |= phase;
	sdhci_writel(host, reg, SDHCI_AT_STAT);
}

static void bsp_wait_sampl_dll_slave_ready(struct sdhci_host *host)
{
	unsigned int reg;
	unsigned int timeout = 20;
	uintptr_t reg_addr;

	reg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_SAMPL_DLL_STATUS : REG_SDIO0_SAMPL_DLL_STATUS;
	do {
		reg = readl(reg_addr);
		if (reg & SDIO_SAMPL_DLL_SLAVE_READY)
			return;

		udelay(1000); /* delay 1000us */
		timeout--;
	} while (timeout > 0);

	printf("sdhci: SAMPL DLL slave not ready.\n");
}

static void bsp_enable_edge_tuning(struct sdhci_host *host)
{
	unsigned int reg;

	reg = readl(REG_EMMC_SAMPLB_DLL_CTRL);
	reg &= ~SDIO_SAMPLB_DLL_CLK_MASK;
	reg |= sdio_samplb_sel(8); /* sel 8 */
	writel(reg, REG_EMMC_SAMPLB_DLL_CTRL);

	reg = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	reg |= SDHCI_EDGE_DETECT_EN;
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

static void bsp_disable_edge_tuning(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	reg &= ~SDHCI_EDGE_DETECT_EN;
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

static void bsp_select_sampl_phase(struct sdhci_host *host, unsigned int phase)
{
	bsp_disable_card_clk(host);
	bsp_set_sampl_phase(host, phase);
	bsp_wait_sampl_dll_slave_ready(host);
	bsp_enable_card_clk(host);
	udelay(1);
}

static int bsp_send_tuning(struct sdhci_host *host, u32 opcode)
{
	int count, err;
	const int tuning_num = 1;

	count = 0;
	do {
		err = mmc_send_tuning(host->mmc, opcode);
		if (err)
			break;

		count++;
	} while (count < tuning_num);

	return err;
}

static int bsp_mmc_exec_tuning(struct sdhci_host *host, unsigned int opcode)
{
	unsigned int index, val;
	unsigned int edge_p2f, edge_f2p, start, end, phase;
	unsigned int fall, rise, fall_updat_flag;
	unsigned int found;
	unsigned int prev_found = 0;
	int err;
	int prev_err = 0;
	unsigned short ctrl;

	bsp_wait_drv_dll_lock(host);
	bsp_enable_sampl_dll_slave(host);
	bsp_enable_sample(host);
	bsp_enable_edge_tuning(host);
	host->is_tuning = 1;

	start = 0;
	end = PHASE_SCALE / EDGE_TUNING_PHASE_STEP;

	edge_p2f = start;
	edge_f2p = end;
	for (index = 0; index <= end; index++) {
		bsp_select_sampl_phase(host, index * EDGE_TUNING_PHASE_STEP);

		err = mmc_send_tuning(host->mmc, opcode);
		if (!err) {
			val = sdhci_readl(host, SDHCI_MULTI_CYCLE);
			found = val & SDHCI_FOUND_EDGE;
		} else {
			found = 1;
		}

		if (prev_found && !found)
			edge_f2p = index;
		else if (!prev_found && found)
			edge_p2f = index;

		if ((edge_p2f != start) && (edge_f2p != end))
			break;

		prev_found = found;
	}

	if ((edge_p2f == start) && (edge_f2p == end)) {
		printf("sdhci: tuning failed! can not found edge!\n");
		return -1;
	}

	bsp_disable_edge_tuning(host);

	start = edge_p2f * EDGE_TUNING_PHASE_STEP;
	end = edge_f2p * EDGE_TUNING_PHASE_STEP;
	if (end <= start)
		end += PHASE_SCALE;

	fall = start;
	rise = end;
	fall_updat_flag = 0;
	for (index = start; index <= end; index++) {
		bsp_select_sampl_phase(host, index % PHASE_SCALE);

		err = bsp_send_tuning(host, opcode);
		if (err)
			debug("sdhci: send tuning CMD%u fail! phase:%u err:%d\n",
				opcode, index, err);

		if (err && index == start) {
			if (!fall_updat_flag) {
				fall_updat_flag = 1;
				fall = start;
			}
		} else {
			if (!prev_err && err) {
				if (!fall_updat_flag) {
					fall_updat_flag = 1;
					fall = index;
				}
			}
		}

		if (prev_err && !err)
			rise = index;

		if (err && index == end)
			rise = end;

		prev_err = err;
	}

	phase = ((fall + rise) / 2 + PHASE_SCALE / 2) % PHASE_SCALE; /* 2 for cal average */

	printf("sdhci: tuning done! valid phase shift [%u, %u] Final Phase:%u\n",
			rise % PHASE_SCALE, fall % PHASE_SCALE, phase);

	host->tuning_phase = phase;
	bsp_select_sampl_phase(host, phase);

	ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	ctrl |= SDHCI_CTRL_TUNED_CLK;
	sdhci_writew(host, ctrl, SDHCI_HOST_CONTROL2);
	host->is_tuning = 0;

	return 0;
}

void sdhci_set_host_caps(struct sdhci_host *host)
{
	host->host_caps = MMC_MODE_HS | MMC_MODE_HS_52MHz |
			  MMC_MODE_HS200_1_8V | MMC_MODE_4BIT;

#if !(defined(CONFIG_TARGET_GK7205V200) || defined(CONFIG_TARGET_GK7202V300))
	host->host_caps |= MMC_MODE_HS400ES | MMC_MODE_HS400_1_8V |
			   MMC_MODE_8BIT;
#endif
}

int sdhci_add_port(int index, uintptr_t regbase, u32 type)
{
	struct sdhci_host *host = NULL;

	if (type == MMC_TYPE_MMC)
		emmc_hardware_init();
	else
		sd_hardware_init();

	host = calloc(1, sizeof(struct sdhci_host));
	if (host == NULL) {
		puts("sdhci_host malloc fail!\n");
		return -ENOMEM;
	}

	host->name = "sdhci";
	host->index = index;
	host->type = type;
	host->ioaddr = (void *)regbase;
	host->quirks = 0;
	host->set_clock = bsp_mmc_set_clk;
	host->priv_init = bsp_mmc_priv_init;
	host->set_control_reg = bsp_mmc_set_ioconfig;
	host->execute_tuning = bsp_mmc_exec_tuning;
	sdhci_set_host_caps(host);
#ifdef CONFIG_SDHCI_ADMA
	host->max_segs = 512; /* max seg 512 */
	host->max_seg_size = 65536; /* max seg size 65536 */
#if CONFIG_PHYS_64BIT
	host->desc_sz = sizeof(struct sdhci_adma2_64_desc);
#else
	host->desc_sz = sizeof(struct sdhci_adma2_32_desc);
#endif
	host->adma_table_sz = (host->max_segs + 2) * host->desc_sz; /* adma tables need 2 extra desc */
	host->adma_table = malloc(ALIGN(host->adma_table_sz, CONFIG_SYS_CACHELINE_SIZE));
	if (!host->adma_table) {
		puts("adma table malloc fail!\n");
		free(host);
		return -ENOMEM;
	}
	host->adma_table += CONFIG_SYS_CACHELINE_SIZE -
			    ((uintptr_t)host->adma_table &
			    (CONFIG_SYS_CACHELINE_SIZE - 1));
#endif
	add_sdhci(host, CONFIG_GK_SDHCI_MAX_FREQ, MIN_FREQ);
	return 0;
}

void sdhci_hs400_enhanced_stobe(struct mmc *mmc, bool enable)
{
	struct sdhci_host *host = mmc->priv;
	u32 ctrl;

	ctrl = sdhci_readl(host, SDHCI_EMMC_CTRL);
	if (enable)
		ctrl |= SDHCI_ENH_STROBE_EN;
	else
		ctrl &= ~SDHCI_ENH_STROBE_EN;

	sdhci_writel(host, ctrl, SDHCI_EMMC_CTRL);

#if defined(CONFIG_TARGET_GK7605V100) || defined(CONFIG_TARGET_GK7205V300)
	ctrl  = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	if (enable)
		ctrl |= SDHCI_CMD_DLY_EN;
	else
		ctrl &= ~SDHCI_CMD_DLY_EN;

	sdhci_writel(host, ctrl, SDHCI_MULTI_CYCLE);
#endif
}

static void print_mmcinfo(struct mmc *mmc)
{
	printf("MMC/SD Card:\n");
	printf("    MID:         0x%x\n", mmc->cid[0] >> 24); /* bit24 - 31 */
	printf("    Read Block:  %d Bytes\n", mmc->read_bl_len);
	printf("    Write Block: %d Bytes\n", mmc->write_bl_len);
	printf("    Chip Size:   %s Bytes (%s)\n",
			ultohstr(mmc->capacity),
			mmc->high_capacity ? "High Capacity" : "Low Capacity");
	printf("    Name:        \"%c%c%c%c%c\"\n",
			mmc->cid[0] & 0xff,           /* bit0   -  7 */
			(mmc->cid[1] >> 24),          /* bit24 - 32 */
			(mmc->cid[1] >> 16) & 0xff,   /* bit16 - 23 */
			(mmc->cid[1] >> 8) & 0xff,    /* bit8   - 15 */
			mmc->cid[1] & 0xff);          /* bit0   -  7 */

	printf("    Chip Type:   %s\n"
			"    Version:     %d.%d\n",
			IS_SD(mmc) ? "SD" : "MMC",
			EXTRACT_SDMMC_MAJOR_VERSION(mmc->version),
			EXTRACT_SDMMC_MINOR_VERSION(mmc->version));

	printf("    Speed:       %sHz\n", ultohstr(mmc->clock));
	printf("    Bus Width:   %dbit\n", mmc->bus_width);
	printf("    Mode:        %s\n", mmc->strobe_enhanced ? "HS400ES" :
			mmc->timing == MMC_TIMING_MMC_HS400 ? "HS400" :
			mmc->timing == MMC_TIMING_MMC_HS200 ? "HS200" : "HS");
}

int bsp_mmc_init(int dev_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	int ret;

	if (mmc == NULL) {
		printf("mmc device not found!!\n");
		return -EINVAL;
	}

	ret = mmc_init(mmc);
	if (ret)
		return ret;

	if (!IS_SD(mmc)) {
		print_mmcinfo(mmc);
		return mmc_set_boot_config(mmc);
	}

	return 0;
}

void printf_mmc(int dev_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);

	if (mmc != NULL)
		print_mmcinfo(mmc);
}
