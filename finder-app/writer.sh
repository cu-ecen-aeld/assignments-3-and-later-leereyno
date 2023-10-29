#!/bin/bash

# Blame : Lee Reynolds

if [ $# -lt 2 ] ; then
	echo -e "\nOne or both arguments are missing\n"
	exit 1
fi

writefile=$1		# The full path to a file that we'll be writing to (e.g. /tmp/this/subdir/filename.txt)
writestr=$2			# String we will be writing into this file, overwriting anything already in the file

filedir=$(dirname $writefile) # What is the path to our file? (e.g. /tmp/this/subdir )

[ -d $filedir ] || mkdir -p $filedir # If the directory path does not exist, then create it

echo $writestr > $writefile	# Save our string out to our file

if [ $? -ne 0 ] ; then
	echo -e "\nCannot write file \"$writefile\"\n"
	exit 1
fi

