# Retired

This repository is no longer maintained and is archived.

U-Boot for the GK7205V200 family — GK7202V300, GK7205V200, GK7205V300 and
GK7605V100 — is built from [OpenIPC/u-boot](https://github.com/OpenIPC/u-boot),
branch [`openipc-hisilicon`](https://github.com/OpenIPC/u-boot/tree/openipc-hisilicon),
together with the other HiSilicon and Goke SoCs. It publishes per-flash-type
images, `u-boot-<soc>-nor.bin` and `u-boot-<soc>-nand.bin`, to the
[OpenIPC/firmware `latest` release](https://github.com/OpenIPC/firmware/releases/tag/latest).

The `u-boot-<soc>-universal.bin` images this repository built stay on that
release for older tools, but are no longer updated.

The original U-Boot README follows in `README`.
