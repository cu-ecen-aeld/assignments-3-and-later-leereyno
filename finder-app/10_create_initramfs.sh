#!/bin/sh

OUTDIR=$1

cd $OUTDIR/rootfs

find . | cpio -H newc -ov --owner root:root > ../initramfs.cpio

cd ..

gzip -f initramfs.cpio


