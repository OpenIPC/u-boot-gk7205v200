/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "../drivers/usb/gadget/udc3/usb3_drv.h"
#include "common.h"

#include <common.h>
#include <malloc.h>
#include <command.h>

#include <spi.h>
#include <spi_flash.h>
#include <jffs2/jffs2.h>
#include <linux/mtd/mtd.h>

static unsigned int usb_open_flag = 0;
static char command[256] = {0};

static int udc_request(void)
{
	extern int get_eventbuf_count(usb3_device_t *dev);
	extern void dcache_disable(void);
	extern void usb3_common_init(usb3_device_t *dev, volatile uint8_t *base);
	extern void phy_usb_init(int index);
	extern void usb3_init(usb3_device_t *dev);
	extern void dcache_enable(void);
	extern int usb_stop(void);
	extern void usb3_handle_event(usb3_device_t *dev);

	dcache_disable();
	usb3_device_t *usb3_dev;
	struct usb_device_descriptor *usb3_dev_desc;
	usb3_dev = (usb3_device_t *)malloc(sizeof(usb3_device_t));
	if (usb3_dev == NULL) {
		debug("usb3_dev: out of memory\n");
		return -ENOMEM;
	}
	usb3_memset((void *)usb3_dev, 0, sizeof(usb3_device_t));
	usb3_dev_desc = (usb_device_descriptor_t *)malloc(sizeof(
						struct usb_device_descriptor));
	usb3_pcd_t *pcd = &usb3_dev->pcd;
	usb3_pcd_ep_t *ep = &pcd->in_ep;
	usb3_pcd_req_t *req = &ep->req;
	req->bufdma = (uint8_t *)malloc(512);
	usb_info("size of usb3_dev %d\n", sizeof(*usb3_dev));
	usb3_dev->base = (volatile uint8_t *)USB3_CTRL_REG_BASE;

	uint8_t string_manu[] = {'G', 0, 'o', 0, 'k', 0, 'e', 0};
	uint8_t string_prod[] = {'U', 0, 'S', 0, 'B', 0, 'B', 0, 'u', 0, 'r', 0, 'n', 0};
	usb3_dev->string_manu_len = sizeof(string_manu);
	usb3_dev->string_prod_len = sizeof(string_prod);
	usb3_dev->dev_desc = usb3_dev_desc;
	memcpy(usb3_dev->string_manu, string_manu, usb3_dev->string_manu_len);
	memcpy(usb3_dev->string_prod, string_prod, usb3_dev->string_prod_len);
	usb3_dev->pcd.ep0_setup_desc = (usb3_dma_desc_t *)
				((uintptr_t)(usb3_dev->pcd.ep0_setup + 15) & (uint32_t)(~15));
	usb3_dev->pcd.ep0_in_desc = (usb3_dma_desc_t *)
				((uintptr_t)(usb3_dev->pcd.ep0_in + 15) & (uint32_t)(~15));
	usb3_dev->pcd.ep0_out_desc = (usb3_dma_desc_t *)
				((uintptr_t)(usb3_dev->pcd.ep0_out + 15) & (uint32_t)(~15));
	usb3_dev->pcd.in_ep.ep_desc = (usb3_dma_desc_t *)
				((uintptr_t)(usb3_dev->pcd.in_ep.epx_desc + 15) & (uint32_t)(~15));
	usb3_dev->pcd.out_ep.ep_desc = (usb3_dma_desc_t *)
				((uintptr_t)(usb3_dev->pcd.out_ep.epx_desc + 15) & (uint32_t)(~15));

	/* Release usb3.0 controller */

	phy_usb_init(0);

	/* Get usb3.0 version number */
	usb3_dev->snpsid = usb3_rd32((volatile uint32_t *)
				(usb3_dev->base + USB3_CORE_REG_BASE + USB3_CORE_GSNPSID_REG_OFFSET));

	/* Initialize usb3.0 core */
	usb3_common_init(usb3_dev, usb3_dev->base + USB3_CORE_REG_BASE);

	/* Initialize usb3.0 pcd */
	usb3_init(usb3_dev);

	usb_info("usb init done\n");

	ulong time_start = get_timer(0);

	/* inital the send buffer */
	extern char tx_state[200];
	memset(tx_state, 0, sizeof(tx_state));

	while (usb_open_flag) {
		usb3_handle_event(usb3_dev);

		if (0 != get_eventbuf_count(usb3_dev)) {
			time_start = get_timer(0);
			continue;
		}

		if (get_timer(time_start) > 5000) {
			printf("the USB has no data for about 5s, stop the USB Device\n");
			break;
		}
	}

	usb_open_flag = 0;

	phy_usb_init(0);
	usb_stop();

	if (req->bufdma != NULL) {
		free(req->bufdma);
		req->bufdma = NULL;
	}

	if (usb3_dev_desc != NULL) {
		free(usb3_dev_desc);
		usb3_dev_desc = NULL;
	}

	if (usb3_dev != NULL) {
		free(usb3_dev);
		usb3_dev = NULL;
	}

	dcache_enable();

	return 0;
}

static int do_usbtftp_download(int argc, char *const argv[])
{
	if (strlen(argv[1]) + strlen(argv[2]) + 15 >= 256)
		return -1;

	char *endp = NULL;
	(void)simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	if (usb_open_flag == 1) {
		printf("usbtftp is running.\n");
		return 1;
	}

	memset(command, 0, sizeof(command));
	sprintf(command, "usbtftp %s %s", argv[1], argv[2]);
	usb_open_flag = 1;

	(void)udc_request();
	return 0;
}

#ifndef CONFIG_MMC
static struct spi_flash *spiflash = NULL;

static int do_flash_probe(void)
{
	unsigned int bus   = CONFIG_SF_DEFAULT_BUS;
	unsigned int cs    = CONFIG_SF_DEFAULT_CS;
	unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
	unsigned int mode  = CONFIG_SF_DEFAULT_MODE;

#ifdef CONFIG_DM_SPI_FLASH
	struct udevice *new = NULL;
	struct udevice *bus_dev = NULL;
	int ret;

	/* In DM mode defaults will be taken from DT */
	speed = 0, mode = 0;
#else
	struct spi_flash *new = NULL;
#endif

#ifdef CONFIG_DM_SPI_FLASH
	/* Remove the old device, otherwise probe will just be a nop */
	ret = spi_find_bus_and_cs(bus, cs, &bus_dev, &new);
	if (!ret)
		device_remove(new);
	spiflash = NULL;
	ret = spi_flash_probe_bus_cs(bus, cs, speed, mode, &new);
	if (ret) {
		printf("Failed to initialize SPI flash at %u:%u (error %d)\n", bus, cs, ret);
		return 1;
	}

	spiflash = dev_get_uclass_priv(new);
#else
	if (spiflash)
		spi_flash_free(spiflash);

	new = spi_flash_probe(bus, cs, speed, mode);
	if (!new) {
		printf("Failed to initialize SPI flash at %u:%u\n", bus, cs);
		return 1;
	}
	spiflash = new;
#endif

	return 0;
}

static unsigned char *membuf = NULL;
static unsigned int mem_len = 0;
static int do_spi_flash_read(int argc, char *const argv[])
{
	unsigned long size;
	char *endp = NULL;
	int ret;
	int dev = 0;
	loff_t offset, len, maxsize;

	if (argc < 3)
		return -1;

	if (spiflash == NULL) {
		printf("error, spiflash is null!\n");
		return -1;
	}

	if (mtd_arg_off(argv[1], &dev, &offset, &len, &maxsize, MTD_DEV_TYPE_NOR,
					spiflash->size)) {
		return -1;
	}

	size = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	/* Consistency checking */
	if (offset + size > spiflash->size) {
		printf("ERROR: attempting %s past flash size (%#x)\n", argv[0], spiflash->size);
		return 1;
	}

	len = size;

	membuf = (unsigned char *)malloc(size);
	if (!membuf) {
		puts("Failed to malloc memory\n");
		return 1;
	}

	ret = spi_flash_read(spiflash, offset, len, (void *)membuf);

	mem_len = size;
	return ret == 0 ? 0 : 1;
}

static int frame_count = 0;
#define FRAME_LENGTH (200)
static int do_upload(uint8_t *const buff, unsigned int *bufflen)
{
	unsigned char *head = (unsigned char *)buff;
	*bufflen = 0;

	/* the tail frame has been sent, send again */
	if (frame_count == -1) {
		head[0] = 0xED;
		*bufflen = 1;
		return 0;
	}

	if (frame_count == 0) {
		head[0] = 0xFE;
		head[1] = (mem_len >> 24) & 0xFF;
		head[2] = (mem_len >> 16) & 0xFF;
		head[3] = (mem_len >> 8) & 0xFF;
		head[4] = mem_len & 0xFF;

		head[5] = (FRAME_LENGTH >> 24) & 0xFF;
		head[6] = (FRAME_LENGTH >> 16) & 0xFF;
		head[7] = (FRAME_LENGTH >> 8) & 0xFF;
		head[8] = FRAME_LENGTH & 0xFF;

		*bufflen = 9;
		frame_count++;
		return 0;
	}

	if (FRAME_LENGTH * frame_count <= mem_len) {
		head[0] = 0xDA;
		*bufflen = FRAME_LENGTH;
	} else if ((FRAME_LENGTH * frame_count > mem_len) &&
			   (FRAME_LENGTH * (frame_count - 1) < mem_len)) {
		head[0] = 0xDA;
		*bufflen = mem_len - FRAME_LENGTH * (frame_count - 1);
	} else if ((FRAME_LENGTH * (frame_count - 1) >= mem_len)) {
		head[0] = 0xED;
		*bufflen = 0;
		frame_count = -1;
	}

	if (*bufflen != 0) {
		memcpy(head + 1, membuf +
			   FRAME_LENGTH * (frame_count - 1), *bufflen);
		frame_count++;
	}

	/* cmd is one byte */
	*bufflen = *bufflen + 1;
	return 0;
}
#endif

static int do_usbtftp_upload(int argc, char *const argv[])
{
	if (strlen(argv[1]) + strlen(argv[2]) + 15 >= 256)
		return -1;

	if (usb_open_flag == 1) {
		printf("usbtftp is running.\n");
		return -1;
	}

#ifndef CONFIG_MMC
	int ret;

	ret = do_flash_probe();
	if (ret != 0)
		goto done;

	ret = do_spi_flash_read(argc, argv);
	if (ret != 0)
		goto done;

	frame_count = 0;

	typedef int (*USB3_HANDLE_REQUEST)(uint8_t *const buff,
						unsigned int *bufflen);
	extern void SetUSB3CallBackFunc(USB3_HANDLE_REQUEST func);
	SetUSB3CallBackFunc(do_upload);

	memset(command, 0, sizeof(command));
	sprintf(command, "usbtftp %s %s %s", argv[1], argv[2], argv[3]);
	usb_open_flag = 1;

	(void)udc_request();

	frame_count = 0;

	SetUSB3CallBackFunc(NULL);

done:

#ifdef CONFIG_DM_SPI_FLASH
	spiflash = NULL;
#else
	if (spiflash) {
		spi_flash_free(spiflash);
		spiflash = NULL;
	}
#endif

	if (membuf) {
		free(membuf);
		membuf = NULL;
	}
	mem_len = 0;

	return ret;
#else
	printf("usbtftp upoload can not support emmc now.\n");
	return -1;
#endif
}


static int do_usbtftp(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	if (strncmp(argv[1], "start", 5) == 0) {
		if (usb_open_flag == 0)
			goto usage;

		printf(" %s\n", command);
		goto done;
	}

	if (strncmp(argv[1], "error", 5) == 0) {
		if (usb_open_flag == 0)
			goto usage;

		usb_open_flag = 0;
		printf("usbtftp error\n");
		goto done;
	}

	if (strncmp(argv[1], "end", 3) == 0) {
		if (usb_open_flag == 0)
			goto usage;

		usb_open_flag = 0;
		printf("usbtftp end\n");
		goto done;
	}

	if (argc == 3) {
		ret = do_usbtftp_download(argc, argv);
		goto done;
	}

	if (argc == 4) {
		ret =  do_usbtftp_upload(argc, argv);
		goto done;
	}

usage:
	return CMD_RET_USAGE;

done:
	return ret;
}

U_BOOT_CMD(
	usbtftp, 4, 0, do_usbtftp,
	"download or upload image using USB protocol",
	"command to download or upload image using USB protocol\n"
	"usbtftp addr file len - upload `len(hex)' bytes starting at addr to file\n"
	"usbtftp addr file     - download file to addr\n"
);


