#!/bin/bash

if [ $# -lt 2 ]
then
echo "$0 : Please use : $0 <uri> <directory_to_save>"
exit 1
fi

echo "Downloading the file to $2"
mkdir -p $2
wget --directory-prefix=$2 $1

if [ $? -ne 0 ]
then
 # wget failed for some reason...
 echo "Downloading the file in $1 failed!"
 exit 1
fi

echo "Download and Save complete!"
exit 0