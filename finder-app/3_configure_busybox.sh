#!/bin/sh

OUTDIR=$1

export CONFIG_PREFIX=${OUTDIR}/rootfs

make distclean

make defconfig

