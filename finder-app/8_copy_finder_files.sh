#!/bin/sh

THISDIR=$1
OUTDIR=$2

sudo cp -av ${THISDIR}/writer ${OUTDIR}/rootfs/home/
sudo cp -av ${THISDIR}/finder.sh ${OUTDIR}/rootfs/home/
sudo cp -av ${THISDIR}/finder-test.sh ${OUTDIR}/rootfs/home/
sudo cp -av ${THISDIR}/autorun-qemu.sh ${OUTDIR}/rootfs/home/

sudo cp -av ${THISDIR}/conf/assignment.txt ${OUTDIR}/rootfs/home/conf/
sudo cp -av ${THISDIR}/conf/username.txt ${OUTDIR}/rootfs/home/conf/

