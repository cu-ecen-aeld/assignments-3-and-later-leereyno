#!/bin/sh

OUTDIR=$1

cd ${OUTDIR}/rootfs/dev

[ -e null ] || sudo mknod -m 666 null c 1 3
[ -e console ] || sudo mknod -m 600 console c 5 1
