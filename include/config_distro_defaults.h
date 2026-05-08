/*
 * Copyright 2013-2014 Red Hat, Inc.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _CONFIG_CMD_DISTRO_DEFAULTS_H
#define _CONFIG_CMD_DISTRO_DEFAULTS_H

/*
 * List of all commands and options that when defined enables support for
 * features required by distros to support boards in a standardised and
 * consistent manner.
 */
#ifndef CONFIG_MINI_BOOT
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_PXE
#define CONFIG_BOOTP_SUBNETMASK

#define CONFIG_CMD_PXE

#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_LONGHELP
#define CONFIG_MENU
#define CONFIG_DOS_PARTITION
/*
 * CONFIG_EFI_PARTITION omitted: pulls in disk/part_efi.c whose
 * `static efi_guid_t system_guid = PARTITION_SYSTEM_GUID;` triggers
 * an "initializer element is not constant" error on this toolchain
 * (gcc treats the EFI_GUID() compound literal as non-constant for
 * static init). The gk SoCs boot from raw NOR/NAND, not GPT, so the
 * default doesn't need it. Boards that actually need it can set
 * CONFIG_EFI_PARTITION in their own header.
 */
#define CONFIG_ISO_PARTITION
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_ENV_VARS_UBOOT_CONFIG

#endif

#endif	/* _CONFIG_CMD_DISTRO_DEFAULTS_H */
