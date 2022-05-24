sinclude $(TOPDIR)/config.mk

#LIB := $(obj)libao.a

ifeq ($(CONFIG_PRODUCTNAME), "gk7205v200")
	cflags-y  += -DAO_PRODUCT_GK7205V200
else ifeq ($(CONFIG_PRODUCTNAME), "gk7205v300")
	cflags-y  += -DAO_PRODUCT_GK7205V300
else ifeq ($(CONFIG_PRODUCTNAME), "gk7202v300")
	cflags-y  += -DAO_PRODUCT_GK7202V300
else ifeq ($(CONFIG_PRODUCTNAME), "gk7605v100")
	cflags-y  += -DAO_PRODUCT_GK7605V100
endif

ccflags-y  += $(cflags-y)
HOSTCFLAGS += $(cflags-y)
CPPFLAGS   += $(cflags-y)

obj-y += amp.o
obj-y += aiao_hal.o
obj-y += ao.o

#COBJS	:= $(COBJS-y)
#SRCS	:= $(COBJS:.o=.c)
#OBJS	:= $(addprefix $(obj),$(COBJS))

#all:	$(LIB)

#$(LIB):	$(obj).depend $(OBJS)
#	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
#include $(SRCTREE)/rules.mk

#sinclude $(obj).depend

#########################################################################

