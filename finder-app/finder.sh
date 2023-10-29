#!/bin/bash

if [ $# -lt 2 ] ; then
	echo -e "\nOne or both arguments are missing\n"
	exit 1
fi

filesdir=$1
searchstr=$2

matchcount=0


if [ ! -d $filesdir ] ; then
	echo -e "\nThe directory \"$filesdir\" does not appear to exist\n"
	exit 1
fi

MESSAGE="The number of files are X and the number of matching lines are Y"

FILECOUNT=$(find $filesdir -type f | wc -l)

for each in $(find $filesdir -type f) ; do
	thismatch=$(grep -c $searchstr $each)
	matchcount=$(( matchcount + thismatch ))
done


echo "The number of files are $FILECOUNT and the number of matching lines are $matchcount"




