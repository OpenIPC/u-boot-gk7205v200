/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <common.h>
#include <config.h>
#include <environment.h>
#include <cpu_common.h>

struct env_common_func_t {
	int (*saveenv)(void);
	void (*env_relocate_spec)(void);
	char *env_name_spec;
};

#ifdef CONFIG_ENV_IS_IN_NAND

#ifdef CONFIG_CMD_NAND

static struct env_common_func_t nand_env_cmn_func = {
	.saveenv = nand_saveenv,
	.env_relocate_spec = nand_env_relocate_spec,
	.env_name_spec = "NAND",
};
#endif

#else

#endif /* CONFIG_ENV_IS_IN_NAND */

#ifdef CONFIG_ENV_IS_IN_SPI_FLASH

static struct env_common_func_t sf_env_cmn_func = {
	.saveenv = sf_saveenv,
	.env_relocate_spec = sf_env_relocate_spec,
	.env_name_spec = "SPI Flash",
};
#else
static struct env_common_func_t sf_env_cmn_func = {0};

#endif /* CONFIG_ENV_IS_IN_SPI_FLASH */

#ifdef CONFIG_ENV_IS_IN_MMC

static struct env_common_func_t emmc_env_cmn_func = {
	.saveenv = emmc_saveenv,
	.env_relocate_spec = emmc_env_relocate_spec,
	.env_name_spec = "eMMC Flash",
};
#else
static struct env_common_func_t emmc_env_cmn_func = {0};

#endif /* CONFIG_ENV_IS_IN_EMMC_FLASH */

#ifdef CONFIG_ENV_IS_IN_UFS

static struct env_common_func_t ufs_env_cmn_func = {
	.saveenv = ufs_saveenv,
	.env_relocate_spec = ufs_env_relocate_spec,
	.env_name_spec = "UFS Flash",
};
#else
static struct env_common_func_t ufs_env_cmn_func = {0};

#endif /* CONFIG_ENV_IS_IN_UFS_FLASH */
char *env_name_spec;
env_t *env_ptr;
struct env_common_func_t *env_cmn_func = NULL;

int saveenv(void)
{
	return env_cmn_func ? env_cmn_func->saveenv() : -1;
}

void env_relocate_spec(void)
{
	switch (get_boot_media()) {
	default:
		env_cmn_func = NULL;
		break;
	case BOOT_MEDIA_NAND:
#ifdef CONFIG_CMD_NAND
		env_cmn_func = &nand_env_cmn_func;
#else
		env_cmn_func = NULL;
#endif
		break;
	case BOOT_MEDIA_SPIFLASH:
		env_cmn_func = &sf_env_cmn_func;
		break;
	case BOOT_MEDIA_EMMC:
		env_cmn_func = &emmc_env_cmn_func;
		break;
	case BOOT_MEDIA_UFS:
		env_cmn_func = &ufs_env_cmn_func;
		break;
	case BOOT_MEDIA_UNKNOWN:
		env_cmn_func = NULL;
		break;
	}

	if (env_cmn_func && !env_cmn_func->env_name_spec)
		env_cmn_func = NULL;

	/* unknow start media */
	if (!env_cmn_func)
		return;

	env_name_spec = env_cmn_func->env_name_spec;

	env_cmn_func->env_relocate_spec();
}

DECLARE_GLOBAL_DATA_PTR;

int env_init(void)
{
	/* Device isn't usable before relocation */
	gd->env_addr = (ulong)(uintptr_t)&default_environment[0];
	gd->env_valid = 1;

	return 0;
}

