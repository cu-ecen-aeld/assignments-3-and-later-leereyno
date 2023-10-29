#!/bin/bash

# Blame : Lee Reynolds

if [ $# -lt 2 ] ; then
	echo -e "\nOne or both arguments are missing\n"
	exit 1
fi

filesdir=$1		# The directory where we will be searching for files that contain our search string
searchstr=$2	# The string we will be searching for

matchcount=0	# Grand total of all matches found, assume there are no matches

if [ ! -d $filesdir ] ; then
	echo -e "\nThe directory \"$filesdir\" does not appear to exist\n"
	exit 1
fi

# How many files are contained within our search directory and its subdirectories
# search explicitly for files, do not include subdirectories in our count
FILECOUNT=$(find $filesdir -type f | wc -l)

# Examine each file found within the $filesdir directory
for each in $(find $filesdir -type f) ; do

	# How many times does our search string appear in the file we're currently examining?
	thismatch=$(grep -c $searchstr $each)

	# add $thismatch to our grand total for all matches found
	matchcount=$(( matchcount + thismatch ))
done

echo "The number of files are $FILECOUNT and the number of matching lines are $matchcount"




