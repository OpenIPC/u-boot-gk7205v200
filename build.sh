#!/bin/bash

export CROSS_COMPILE=arm-linux-gnueabi-
mkdir -p output

for soc in gk7205v300; do

make clean
cp configs/${soc}_defconfig .config
cp reg_info_${soc}.bin .reg
make -j8

make SHELL=/bin/bash -C tools/hi_gzip

make u-boot-z.bin
cp u-boot-${soc}.bin output/u-boot-${soc}-universal.bin

done
