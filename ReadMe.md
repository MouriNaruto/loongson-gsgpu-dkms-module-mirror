# loongson-gsgpu-dkms-module

The source code of Loongson GSGPU DKMS module.

## Notice

First of all, I need to clarify I am not a Loongson user or a Loongson employee.

One of my friends told me Loongson had done a closed source kernel module but
noted with GPL for a new GPU chip from them, and they are trying their best to
analyze it via reverse engineering.

I was interested about that and I had downloaded the package from the Loongnix
software repository. I found they have provide the full source code of them.

It looks like the module is based on the AMD GPU driver. I have tried my best
to find the copyright information from Loongson and finally I have found the
copyright information in several files.

## Information

- [Original Package](Package/gsgpu-dkms_1.0.0_loongarch64.deb)
  - File Name: gsgpu-dkms_1.0.0_loongarch64.deb
  - SHA-256: 6dfa818f9a10c67450c3224d331a70dbeee534b578a313747c8b38ee999f8b57
  - Original Link: http://pkg.loongnix.cn/loongnix/pool/main/g/gsgpu-dkms/gsgpu-dkms_1.0.0_loongarch64.deb
  - Withdrawn Date: 18:16 of November 17, 2022 (UTC+8)
- [Extracted source code](Source/gsgpu-1.0.0)

## Afterword

Have a nice day and hope this repository can help someone to improve the
hardware support for the community (a.k.a New World) LoongArch ecosystem.
