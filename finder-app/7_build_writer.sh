#!/bin/sh 

THISDIR=$1

cd $THISDIR

make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- -B writer
