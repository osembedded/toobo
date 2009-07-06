#!/bin/bash
#########################################################################
# patch_source.sh [Part of the TOOBO project]
# 
# Copyright 2008 Vijay Jayaraman (osembedded@gmail.com) 
# 
# This program is free software: you can redistribute it and/or modify 
# it under the terms of the GNU Lesser General Public License as published by 
# the Free Software Foundation, either version 3 of the License, or 
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
# GNU Lesser General Public License for more details. 
# 
# You should have received a copy of the GNU Lesser General Public License 
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
##########################################################################
#
# Shell script to patch a source directory with diff files from a patch directory
# Usage -
#         ./patch_source.sh <source_dir_to_patch> <patch_dir>
#
# Note  - 
# This script assumes that the patch needs to be applied from the directory 
# immediately above the source directory. i.e. the parent directory of the 
# source directory
#

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
if [ $? -ne 0 ]
then
echo "Error while patching..."
exit 1
fi
done