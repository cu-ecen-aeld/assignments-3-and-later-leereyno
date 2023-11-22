#!/bin/sh

THISDIR=$1
OUTDIR=$2

# Program interpreter
sudo cp -av ${THISDIR}/dependencies/ld-2.31.so ${OUTDIR}/rootfs/lib/ld-linux-aarch64.so.1

# Shared libraries
sudo cp -av ${THISDIR}/dependencies/libm-2.31.so ${OUTDIR}/rootfs/lib64/libm.so.6
sudo cp -av ${THISDIR}/dependencies/libresolv-2.31.so ${OUTDIR}/rootfs/lib64/libresolv.so.2
sudo cp -av ${THISDIR}/dependencies/libc-2.31.so ${OUTDIR}/rootfs/lib64/libc.so.6




