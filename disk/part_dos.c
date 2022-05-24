/*
 * (C) Copyright 2001
 * Raymond Lo, lo@routefree.com
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Support for harddisk partitions.
 *
 * To be compatible with LinuxPPC and Apple we use the standard Apple
 * SCSI disk partitioning scheme. For more information see:
 * http://developer.apple.com/techpubs/mac/Devices/Devices-126.html#MARKER-14-92
 */

#include <common.h>
#include <command.h>
#include <ide.h>
#include <memalign.h>
#include "part_dos.h"

#ifdef HAVE_BLOCK_DEVICE

#define DOS_PART_DEFAULT_SECTOR 512

/* should this be configurable? It looks like it's not very common at all
 * to use large numbers of partitions */
#define MAX_EXT_PARTS 256

/* Convert char[4] in little endian format to the host format integer
 */
static inline unsigned int le32_to_int(unsigned char *le32)
{
    return ((le32[3] << 24) +
	    (le32[2] << 16) +
	    (le32[1] << 8) +
	     le32[0]
	   );
}

static inline int is_extended(int part_type)
{
    return (part_type == 0x5 ||
	    part_type == 0xf ||
	    part_type == 0x85);
}

static inline int is_bootable(dos_partition_t *p)
{
	return p->boot_ind == 0x80;
}

static void print_one_part(dos_partition_t *p, lbaint_t ext_part_sector,
			   int part_num, unsigned int disksig)
{
	lbaint_t lba_start = ext_part_sector + le32_to_int (p->start4);
	lbaint_t lba_size  = le32_to_int (p->size4);

	printf("%3d\t%-10" LBAFlength "u\t%-10" LBAFlength
		"u\t%08x-%02x\t%02x%s%s\n",
		part_num, lba_start, lba_size, disksig, part_num, p->sys_ind,
		(is_extended(p->sys_ind) ? " Extd" : ""),
		(is_bootable(p) ? " Boot" : ""));
}
#if 0
static int test_block_type(unsigned char *buffer)
{
	int slot;
	struct dos_partition *p;

	if((buffer[DOS_PART_MAGIC_OFFSET + 0] != 0x55) ||
	    (buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) ) {
		return (-1);
	} /* no DOS Signature at all */
	p = (struct dos_partition *)&buffer[DOS_PART_TBL_OFFSET];
	for (slot = 0; slot < 3; slot++) {
		if (p->boot_ind != 0 && p->boot_ind != 0x80) {
			if (!slot &&
			    (strncmp((char *)&buffer[DOS_PBR_FSTYPE_OFFSET],
				     "FAT", 3) == 0 ||
			     strncmp((char *)&buffer[DOS_PBR32_FSTYPE_OFFSET],
				     "FAT32", 5) == 0)) {
				return DOS_PBR; /* is PBR */
			} else {
				return -1;
			}
		}
	}
	return DOS_MBR;	    /* Is MBR */
}
#endif
static int test_block_type(unsigned char *buffer)
{
	int slot;
	struct dos_partition *p;
    int checkpbr = 0;

	if((buffer[DOS_PART_MAGIC_OFFSET + 0] != 0x55) ||
	    (buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) ) {
		return (-1);
	} /* no DOS Signature at all */
	p = (struct dos_partition *)&buffer[DOS_PART_TBL_OFFSET];
	for (slot = 0; slot < 3; slot++) {
        if(p->boot_ind != 0 && p->boot_ind != 0x80){
            //not valid mbr,need check PBR
            checkpbr = 1;
        }else if(p->boot_ind == 0){  //some cases MBR's DOS_PART_TBL_OFFSET value is 0, but paration is valid
            if(p->size4[0] == 0 && p->size4[1] == 0 && p->size4[2] == 0 && p->size4[3] == 0)
            {
                //not valid mbr paratition size, check PBR
                checkpbr = 1;
            }else{
                checkpbr = 0;
            }
        }else{
            checkpbr = 0;
        }

        if(checkpbr){
			if (!slot &&
			    (strncmp((char *)&buffer[DOS_PBR_FSTYPE_OFFSET],
				     "FAT", 3) == 0 ||
			     strncmp((char *)&buffer[DOS_PBR32_FSTYPE_OFFSET],
				     "FAT32", 5) == 0)) {
				return DOS_PBR; /* is PBR */
			} else {
				return -1;
			}
		}
	}
	return DOS_MBR;	    /* Is MBR */
}



static int part_test_dos(struct blk_desc *dev_desc)
{
	ALLOC_CACHE_ALIGN_BUFFER(unsigned char, buffer, dev_desc->blksz);
    int ret = -1;
    lbaint_t start = 0;

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
    extern int target_dev;
    extern unsigned int dos_start_lba;
    if(dev_desc->devnum == target_dev){
        start = dos_start_lba;
    }
#endif
#endif

	if (blk_dread(dev_desc, start, 1, (ulong *)buffer) != 1)
		return -1;

	/*if (test_block_type(buffer) != DOS_MBR)
		return -1;*/

    ret = test_block_type(buffer);
    if((ret==DOS_PBR)||(ret==DOS_MBR)){
        return 0;
    }else{
        return -1;
    }
}

/*  Print a partition that is relative to its Extended partition table
 */
static void print_partition_extended(struct blk_desc *dev_desc,
				     lbaint_t ext_part_sector,
				     lbaint_t relative,
				     int part_num, unsigned int disksig)
{
	ALLOC_CACHE_ALIGN_BUFFER(unsigned char, buffer, dev_desc->blksz);
	dos_partition_t *pt;
	int i;
    lbaint_t start_sector = 0;

	/* set a maximum recursion level */
	if (part_num > MAX_EXT_PARTS)
	{
		printf("** Nested DOS partitions detected, stopping **\n");
		return;
	}

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
        extern int target_dev;
        extern unsigned int dos_start_lba;
        if(dev_desc->devnum == target_dev){
            start_sector = dos_start_lba;
        }
#endif
#endif

	if (blk_dread(dev_desc, ext_part_sector, 1, (ulong *)buffer) != 1) {
		printf ("** Can't read partition table on %d:" LBAFU " **\n",
			dev_desc->devnum, ext_part_sector);
		return;
	}
	i=test_block_type(buffer);
	if (i != DOS_MBR) {
		printf ("bad MBR sector signature 0x%02x%02x\n",
			buffer[DOS_PART_MAGIC_OFFSET],
			buffer[DOS_PART_MAGIC_OFFSET + 1]);
		return;
	}

	if (!ext_part_sector)
		disksig = le32_to_int(&buffer[DOS_PART_DISKSIG_OFFSET]);

	/* Print all primary/logical partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		/*
		 * fdisk does not show the extended partitions that
		 * are not in the MBR
		 */

		if ((pt->sys_ind != 0) &&
		    (ext_part_sector == 0 || !is_extended (pt->sys_ind)) ) {
			print_one_part(pt, ext_part_sector, part_num, disksig);
		}

		/* Reverse engr the fdisk part# assignment rule! */
		if ((ext_part_sector == start_sector) ||
		    (pt->sys_ind != 0 && !is_extended (pt->sys_ind)) ) {
			part_num++;
		}
	}

	/* Follows the extended partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		if (is_extended (pt->sys_ind)) {
			lbaint_t lba_start
				= le32_to_int (pt->start4) + relative;

			print_partition_extended(dev_desc, lba_start,
				ext_part_sector == start_sector  ? lba_start : relative,
				part_num, disksig);
		}
	}

	return;
}


/*  Print a partition that is relative to its Extended partition table
 */
static int part_get_info_extended(struct blk_desc *dev_desc,
				  lbaint_t ext_part_sector, lbaint_t relative,
				  int part_num, int which_part,
				  disk_partition_t *info, unsigned int disksig)
{
	ALLOC_CACHE_ALIGN_BUFFER(unsigned char, buffer, dev_desc->blksz);
	dos_partition_t *pt;
	int i;
	int dos_type;
    lbaint_t start_sector = 0;

	/* set a maximum recursion level */
	if (part_num > MAX_EXT_PARTS)
	{
		printf("** Nested DOS partitions detected, stopping **\n");
		return -1;
	}

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
    extern int target_dev;
    extern unsigned int dos_start_lba;
    if(dev_desc->devnum == target_dev){
        start_sector = dos_start_lba;
    }
#endif
#endif
	if (blk_dread(dev_desc, ext_part_sector, 1, (ulong *)buffer) != 1) {
		printf ("** Can't read partition table on %d:" LBAFU " **\n",
			dev_desc->devnum, ext_part_sector);
		return -1;
	}
	if (buffer[DOS_PART_MAGIC_OFFSET] != 0x55 ||
		buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) {
		printf ("bad MBR sector signature 0x%02x%02x\n",
			buffer[DOS_PART_MAGIC_OFFSET],
			buffer[DOS_PART_MAGIC_OFFSET + 1]);
		return -1;
	}

#ifdef CONFIG_PARTITION_UUIDS
	if (!ext_part_sector)
		disksig = le32_to_int(&buffer[DOS_PART_DISKSIG_OFFSET]);
#endif

	/* Print all primary/logical partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		/*
		 * fdisk does not show the extended partitions that
		 * are not in the MBR
		 */
		if (((pt->boot_ind & ~0x80) == 0) &&
		    (pt->sys_ind != 0) &&
		    (part_num == which_part) &&
		    (is_extended(pt->sys_ind) == 0)) {
			info->blksz = DOS_PART_DEFAULT_SECTOR;
			info->start = (lbaint_t)(ext_part_sector +
					le32_to_int(pt->start4));
			info->size  = (lbaint_t)le32_to_int(pt->size4);
			part_set_generic_name(dev_desc, part_num,
					      (char *)info->name);
			/* sprintf(info->type, "%d, pt->sys_ind); */
			strcpy((char *)info->type, "U-Boot");
			info->bootable = is_bootable(pt);
#ifdef CONFIG_PARTITION_UUIDS
			sprintf(info->uuid, "%08x-%02x", disksig, part_num);
#endif
			return 0;
		}

		/* Reverse engr the fdisk part# assignment rule! */
		if ((ext_part_sector == start_sector) ||
		    (pt->sys_ind != 0 && !is_extended (pt->sys_ind)) ) {
			part_num++;
		}
	}

	/* Follows the extended partitions */
	pt = (dos_partition_t *) (buffer + DOS_PART_TBL_OFFSET);
	for (i = 0; i < 4; i++, pt++) {
		if (is_extended (pt->sys_ind)) {
			lbaint_t lba_start
				= le32_to_int (pt->start4) + relative;

			return part_get_info_extended(dev_desc, lba_start,
				 ext_part_sector == start_sector ? lba_start : relative,
				 part_num, which_part, info, disksig);
		}
	}

	/* Check for DOS PBR if no partition is found */
	dos_type = test_block_type(buffer);

	if (dos_type == DOS_PBR) {
		info->start = ext_part_sector;
		info->size = dev_desc->lba;
		info->blksz = DOS_PART_DEFAULT_SECTOR;
		info->bootable = 0;
		strcpy((char *)info->type, "U-Boot");
#ifdef CONFIG_PARTITION_UUIDS
		info->uuid[0] = 0;
#endif
		return 0;
	}

	return -1;
}

void part_print_dos(struct blk_desc *dev_desc)
{
	printf("Part\tStart Sector\tNum Sectors\tUUID\t\tType\n");
    lbaint_t start_sector = 0;

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
    extern int target_dev;
    extern unsigned int dos_start_lba;
    if(dev_desc->devnum == target_dev){
        start_sector = dos_start_lba;
    }
#endif
#endif
	print_partition_extended(dev_desc, start_sector, 0, 1, 0);
}

int part_get_info_dos(struct blk_desc *dev_desc, int part,
		      disk_partition_t *info)
{
    lbaint_t start_sector = 0;

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
    extern int target_dev;
    extern unsigned int dos_start_lba;
    if(dev_desc->devnum == target_dev){
        start_sector = dos_start_lba;
    }
#endif
#endif
	return part_get_info_extended(dev_desc, start_sector, 0, 1, part, info, 0);
}

int is_valid_dos_buf(void *buf)
{
	return test_block_type(buf) == DOS_MBR ? 0 : -1;
}

int write_mbr_partition(struct blk_desc *dev_desc, void *buf)
{
	if (is_valid_dos_buf(buf))
		return -1;

    lbaint_t start_sector = 0;

#if (CONFIG_AUTO_UPDATE == 1)
#ifdef CONFIG_EMMC
        extern int target_dev;
        extern unsigned int dos_start_lba;
        if(dev_desc->devnum == target_dev){
            start_sector = dos_start_lba;
        }
#endif
#endif
	/* write MBR */
	if (blk_dwrite(dev_desc, start_sector, 1, buf) != 1) {
		printf("%s: failed writing '%s' (1 blks at 0x0)\n",
		       __func__, "MBR");
		return 1;
	}

	return 0;
}

U_BOOT_PART_TYPE(dos) = {
	.name		= "DOS",
	.part_type	= PART_TYPE_DOS,
	.max_entries	= DOS_ENTRY_NUMBERS,
	.get_info	= part_get_info_ptr(part_get_info_dos),
	.print		= part_print_ptr(part_print_dos),
	.test		= part_test_dos,
};

#endif
