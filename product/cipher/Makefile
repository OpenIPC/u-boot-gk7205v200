#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

cflags-y     += -I$(srctree)/product/cipher/v2/api
cflags-y     += -I$(srctree)/product/cipher/v2/drv/platform
cflags-y     += -I$(srctree)/product/cipher/v2/drv/rng
cflags-y     += -I$(srctree)/product/cipher/v2/drv/rsa
cflags-y     += -I$(srctree)/product/cipher/v2/drv/compat
cflags-y     += -I$(srctree)/product/cipher/v2/drv/sm2
cflags-y     += -I$(srctree)/product/cipher/v2/drv/spacc
cflags-y     += -I$(srctree)/product/cipher/v2/drv/include
cflags-y     += -I$(srctree)/product/opt

ccflags-y  += $(cflags-y)
HOSTCFLAGS += $(cflags-y)
CPPFLAGS   += $(cflags-y)

obj-y    += v2/api/ree_mpi_cipher.o \
            v2/api/ree_mpi_hash.o \
            v2/api/ree_mpi_rng.o \
            v2/api/ree_mpi_rsa.o \
            v2/drv/drv_cipher_intf.o \
            v2/drv/rng/drv_rng.o \
            v2/drv/rsa/drv_rsa.o \
            v2/drv/spacc/spacc_body.o \
            v2/drv/spacc/spacc_intf.o \
            v2/drv/compat/drv_klad.o \
            v2/drv/compat/drv_compat.o \
            v2/drv/platform/cipher_adapt.o
