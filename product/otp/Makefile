
cflags-y += -I$(srctree)/product/cipher/v2/api
cflags-y += -I$(srctree)/product/cipher/v2/drv/platform
cflags-y += -I$(srctree)/product/cipher/v2/drv/rng
cflags-y += -I$(srctree)/product/cipher/v2/drv/rsa
cflags-y += -I$(srctree)/product/cipher/v2/drv/compat
cflags-y += -I$(srctree)/product/cipher/v2/drv/spacc
cflags-y += -I$(srctree)/product/cipher/v2/drv/include
cflags-y += -I$(srctree)/product/otp

ccflags-y  += $(cflags-y)
HOSTCFLAGS += $(cflags-y)
CPPFLAGS   += $(cflags-y)

obj-y    += hal_otp.o
