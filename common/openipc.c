/*
 * On-boot firmware integrity scan.
 *
 * Ported from the hi3516cv6xx U-Boot (common/openipc.c). Scans SPI NOR for a
 * valid kernel and rootfs and sets the environment variable "bootfail" when
 * either is missing or corrupt. The boot script (see fwrecovery in
 * gk-common.h) reads that flag and TFTP-reflashes instead of executing garbage
 * or looping forever on a half-written partition -- the state an interrupted
 * sysupgrade leaves behind, which otherwise needs a UART reflash to recover.
 *
 * Deliberately conservative: a failed SPI probe skips the check rather than
 * declaring the firmware bad, so a probe glitch never triggers a needless
 * recovery.
 */
#include <common.h>
#include <spi.h>
#include <spi_flash.h>
#include <image.h>
#include <malloc.h>
#include <asm/unaligned.h>
#include <openipc.h>

#ifndef CONFIG_SF_DEFAULT_SPEED
#define CONFIG_SF_DEFAULT_SPEED 1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
#define CONFIG_SF_DEFAULT_MODE SPI_MODE_3
#endif

/* Start just past the environment; step and bound by the erase block and the
 * region a kernel+rootfs pair can occupy on the 8/16 MB NOR layouts. */
#define SCAN_FROM   (CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define SCAN_STEP   0x10000
#define SCAN_LIMIT  0x400000

#define OIPC_FDT_MAGIC 0xd00dfeed
#define SQSH_MAGIC_BE  0x73717368
#define SQSH_MAGIC_LE  0x68737173

static int check_kernel(const void *buf)
{
	const image_header_t *hdr = (const image_header_t *)buf;

	if (image_check_magic(hdr))
		return 1;
	if (fdt_magic(buf) == OIPC_FDT_MAGIC && fdt_check_header(buf) == 0)
		return 1;
	return 0;
}

static int check_rootfs(const void *buf)
{
	u32 magic = get_unaligned_be32(buf);

	if (magic == SQSH_MAGIC_BE || magic == SQSH_MAGIC_LE)
		return 1;
	if (memcmp(buf, "UBI#", 4) == 0)
		return 1;
	return 0;
}

int firmware_scan(void)
{
	struct spi_flash *flash;
	ulong off, kern = 0, root = 0;
	u8 *buf;

	flash = spi_flash_probe(0, 0, CONFIG_SF_DEFAULT_SPEED,
				CONFIG_SF_DEFAULT_MODE);
	if (!flash) {
		printf("firmware_scan: SPI NOR probe failed; skipping check\n");
		return -1;
	}

	buf = malloc(SCAN_STEP);
	if (!buf) {
		spi_flash_free(flash);
		return -1;
	}

	for (off = SCAN_FROM; off < SCAN_LIMIT && off < flash->size;
	     off += SCAN_STEP) {
		if (spi_flash_read(flash, off, SCAN_STEP, buf))
			continue;

		/* Skip a uniformly blank (all 0x00 or all 0xFF) block. */
		if (buf[0] == 0x00 || buf[0] == 0xFF) {
			int i, uniform = 1;

			for (i = 1; i < 32; i++) {
				if (buf[i] != buf[0]) {
					uniform = 0;
					break;
				}
			}
			if (uniform)
				continue;
		}

		if (!kern && check_kernel(buf)) {
			kern = off;
			printf("firmware_scan: kernel at 0x%08lx\n", off);
		}
		if (!root && check_rootfs(buf)) {
			root = off;
			printf("firmware_scan: rootfs at 0x%08lx\n", off);
		}
		if (kern && root)
			break;
	}

	free(buf);
	spi_flash_free(flash);

	if (kern && root) {
		setenv("bootfail", NULL);
	} else {
		printf("firmware_scan: firmware absent/corrupt "
		       "(kernel=0x%lx rootfs=0x%lx) -> recovery\n", kern, root);
		setenv("bootfail", "1");
	}
	return 0;
}
