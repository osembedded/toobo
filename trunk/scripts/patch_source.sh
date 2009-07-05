#!/bin/bash
## This file will apply all the patch files from 
## the patch_dir to the source_dir
## Note: this script assumes that the patch needs 
## to be applied from the directory above the patch
## directory.

if [ $# -lt 2 ]
then
echo "Usage: $0 <source_dir> <patch_dir>"
exit 1
fi

## If the given directories exist...
if [ ! -d $1 ]
then
echo "Directory $1 does not exist!"
exit 1
fi

if [ ! -d $2 ]
then
echo "Directory $2 does not exist!"
exit 1
fi

echo "Source Dir: $1"
echo "Patch Dir: $2"

for i in `ls $2`
do
echo "Applying Patch: $i"
patch -p0 --directory=$1/.. < $2/$i
done