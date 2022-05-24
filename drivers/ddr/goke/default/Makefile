ccflags-y += -Idrivers/ddr/goke/default -Idrivers/ddr/goke/$(SOC)

ifndef CONFIG_MINI_BOOT
obj-y += ddr_cmd_loc.o ddr_cmd_ctl.o cmd_ddr_training_v2.o
else
obj-y += ddr_cmd_ctl.o
endif

extra-y += ddr_training_impl.o ddr_training_ctl.o ddr_training_boot.o ddr_training_console.o

ifndef CONFIG_MINI_BOOT
$(obj)/ddr_cmd_loc.S: ddr_training_cmd_bin

ddr_training_cmd_bin:
	$(MAKE) -C $(CURDIR)/drivers/ddr/goke/default/cmd_bin TOPDIR=$(CURDIR) CROSS_COMPILE=$(CROSS_COMPILE) CPU=$(CPU) SOC=$(SOC)

endif
