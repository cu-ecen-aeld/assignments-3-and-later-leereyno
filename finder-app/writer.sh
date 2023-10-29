#!/bin/bash

if [ $# -lt 2 ] ; then
	echo -e "\nOne or both arguments are missing\n"
	exit 1
fi

writefile=$1
writestr=$2

filedir=$(dirname $writefile)

[ -d $filedir ] || mkdir -p $filedir

echo $writestr > $writefile

if [ $? -ne 0 ] ; then
	echo -e "\nCannot write file \"$writefile\"\n"
	exit 1
fi

