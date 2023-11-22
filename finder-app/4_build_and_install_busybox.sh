#!/bin/sh

OUTDIR=$1

export CONFIG_PREFIX=${OUTDIR}/rootfs

make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu-

make \
   CONFIG_PREFIX=$CONFIG_PREFIX \
   ARCH=arm64 \
   CROSS_COMPILE=aarch64-none-linux-gnu- \
   install


