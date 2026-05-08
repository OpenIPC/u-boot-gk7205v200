PWD           = $(shell pwd)
SRCDIR        ?=
OUTDIR        ?=

################################################################################
CC       := $(CROSS_COMPILE)gcc
AR       := $(CROSS_COMPILE)ar
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy


################################################################################
BOOT     := u-boot-$(SOC)
TEXTBASE := 0x40700000

CFLAGS   :=-Os -fno-builtin -ffreestanding \
	-D__KERNEL__ -DTEXT_BASE=$(TEXTBASE) \
	-I$(SRCDIR)/include \
	-I$(OUTDIR)/include \
	-I$(SRCDIR)/drivers/ddr/goke/default \
	-I$(SRCDIR)/drivers/ddr/goke/$(SOC) \
	-I$(SRCDIR)/arch/arm/include \
	-I$(SRCDIR)/lib/hw_dec \
	-fno-pic -ffunction-sections \
	-fdata-sections -fno-common -ffixed-r9    \
	-fno-common -pipe -march=armv7-a \
	-Wall -Wstrict-prototypes -fno-stack-protector \
	-D__LINUX_ARM_ARCH__=7 -D__ARM__ \
	-DCONFIG_MMC\
	$(MKFLAGS) -fno-strict-aliasing

################################################################################
COBJS := start.o \
	lowlevel_init_v300.o \
	init_registers.o \
	emmc_boot.o \
	uart.o \
	ddr_training_impl.o \
	ddr_training_ctl.o \
	ddr_training_boot.o \
	ddr_training_custom.o \
	ddr_training_console.o \
	hw_decompress.o \
	startup.o \
	image_data.o \
	div0.o \
	reset.o

REG := $(wildcard $(OUTDIR)/*.reg $(OUTDIR)/.reg)

################################################################################
.PHONY: $(BOOT).bin
$(BOOT).bin: $(BOOT).tmp regfile
	@dd if=./$(BOOT).tmp of=./tmp1 bs=1 count=64 2>/dev/null
	@dd if=$(REG) of=./tmp2 bs=5120 conv=sync 2>/dev/null
	@dd if=./$(BOOT).tmp of=./tmp3 bs=1 skip=5184 2>/dev/null
	@cat tmp1 tmp2 tmp3 > $(BOOT).bin
	@rm -f tmp1 tmp2 tmp3
	@chmod 754 $(BOOT).bin
	@cp -fv $@ $(OUTDIR)
	@echo $(BOOT).bin is Ready.

$(BOOT).tmp: $(BOOT).elf
	$(OBJCOPY) -O srec $< $(BOOT).srec
	$(OBJCOPY) -j .text -O binary $< $(BOOT).text
	$(OBJCOPY) --gap-fill=0xff -O binary $< $@

$(BOOT).elf: LNOBJS image_data.gzip $(COBJS)
	$(LD) -Bstatic -T u-boot.lds -Ttext $(TEXTBASE) \
		$(COBJS) -Map $(BOOT).map -o $@
	$(OBJDUMP) -d  $@ > $@.asm

LNOBJS:
	@ln -sf ../../../../../drivers/ddr/goke/default/ddr_training_impl.o .
	@ln -sf ../../../../../drivers/ddr/goke/default/ddr_training_ctl.o .
	@ln -sf ../../../../../drivers/ddr/goke/default/ddr_training_boot.o .
	@ln -sf ../../../../../drivers/ddr/goke/default/ddr_training_console.o .
	@ln -sf ../../../../../drivers/ddr/goke/$(SOC)/ddr_training_custom.o .
	@ln -sf ../../../lib/div0.o .

.PHONY: regfile
regfile:
	@if [ "$(words $(REG))" = "0" ]; then ( \
		echo '***' Need '.reg' or '*.reg' file in directory $(OUTDIR); \
		exit 1; \
	) fi
	@if [ "$(words $(REG))" != "1" ]; then ( \
		echo '***' Found multi '.reg' or '*.reg' file in directory $(OUTDIR); \
		echo '***' Files: $(notdir $(REG)); \
		exit 1; \
	) fi

################################################################################

# -1 : --fast      -9 : --best
image_data.gzip: $(BINIMAGE)
	$(OUTDIR)/tools/hi_gzip/bin/gzip -fNqc -7 $< > $@

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes \
		-fno-stack-protector -o $@ $< -c

%.o: %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

image_data.o: image_data.S image_data.gzip
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c
