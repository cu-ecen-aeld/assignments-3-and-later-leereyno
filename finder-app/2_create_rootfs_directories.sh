#!/bin/sh

OUTDIR=$1

cd $OUTDIR

mkdir rootfs

cd rootfs

mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var

mkdir -p usr/bin usr/lib usr/sbin

mkdir -p var/log

mkdir -p home/conf

cd dev

[ -e null ] || sudo mknod -m 666 null c 1 3
[ -e console ] || sudo mknod -m 600 console c 5 1
