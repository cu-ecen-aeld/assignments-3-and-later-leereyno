#!/bin/sh

OUTDIR=$1
LASTDIR=$(pwd)

#cd $OUTDIR

#if [ ! -e linux-5.1.10.tar.gz ] ; then
#	curl -O https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.1.10.tar.gz
#fi

#if [ ! -d linux-5.1.10 ] ; then
#	tar xf linux-5.1.10.tar.gz
#fi

#cd linux-5.1.10

make clean

make distclean

make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- mrproper

make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- defconfig

make -j $(nproc) ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- Image 2>&1 | tee ../_build

make -j $(nproc) ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- dtbs 2>&1 | tee ../_devicetree

LASTDIR=$(pwd)

cd $OUTDIR

sudo cp -av ${LASTDIR}/arch/arm64/boot/Image .

#cp linux-5.1.10/arch/arm64/boot/Image .
