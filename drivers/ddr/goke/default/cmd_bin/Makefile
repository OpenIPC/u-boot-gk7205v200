sinclude $(TOPDIR)/arch/arm/cpu/$(CPU)/config.mk       # include architecture dependend rules
################################################################################

PWD      := $(shell pwd)
OPPDIR   := $(subst $(TOPDIR),,$(PWD))

CC       := $(CROSS_COMPILE)gcc
AR       := $(CROSS_COMPILE)ar
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy
OBJDUMP  := $(CROSS_COMPILE)objdump


################################################################################
DDR_CMD     := ddr_cmd

CMD_TEXT_BASE := $(shell grep '^\#define.*DDR_TRAINING_RUN_STACK' $(TOPDIR)/drivers/ddr/goke/$(SOC)/ddr_training_custom.h|awk '{print $$3}')
STACK_POINT   := $(CMD_TEXT_BASE)

COBJS       := ddr_training_uart.o ../../$(SOC)/ddr_training_custom.o ddr_training_cmd.o ../ddr_training_impl.o ../ddr_training_ctl.o ../ddr_training_console.o
DEPS        := $(COBJS:.o=.d) $(START:.o=.d)


CFLAGS   := -Os -pipe  \
	-DCMD_TEXT_BASE=$(CMD_TEXT_BASE) -DSTACK_POINT=$(STACK_POINT) \
	-fno-builtin -ffreestanding -I./ -I$(TOPDIR)/include -I../ \
	-DDDR_TRAINING_CMD -I$(TOPDIR)/drivers/ddr/goke/$(SOC)/

CFLAGS += $(PLATFORM_RELFLAGS) $(PLATFORM_CPPFLAGS)

START       := cmd_entry_32.o
LDS_SCRIPT  := ddr_cmd_32.lds
################################################################################

.PHONY: $(DDR_CMD).bin
all: $(DDR_CMD).bin

$(DDR_CMD).bin: $(DDR_CMD).elf
	$(OBJCOPY) -O srec $(PWD)/$(DDR_CMD).elf $(DDR_CMD).srec
	$(OBJCOPY) --gap-fill=0xff -O binary $(PWD)/$(DDR_CMD).elf $@

$(DDR_CMD).elf: $(START) $(COBJS) $(LDS_SCRIPT)
	$(LD) -Bstatic -T $(LDS_SCRIPT) -Ttext $(CMD_TEXT_BASE) $(START) \
		$(COBJS) -Map $(DDR_CMD).map -o $@

%.o : %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c $*.S

%.o : %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes -fno-stack-protector \
		-o $@ -c $*.c

ifneq ("$(MAKECMDGOALS)","clean")
sinclude $(DEPS)
endif

%.d : %.c
	set -e; $(CC) $(CFLAGS) -MM $< | sed 's,$*.o:,$*.o $*.d:,g' > $@

%.d : %.S
	set -e; $(CC) $(CFLAGS) -MM $< | sed 's,$*.o:,$*.o $*.d:,g' > $@
