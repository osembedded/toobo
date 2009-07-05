#!/bin/bash
##########################################################################
# download_to_directory.sh [Part of the TOOBO project]
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
# Shell script to download a given file to a particular directory.
# Usage -
#         ./download_to_directory.sh <uri> <directory_to_save>
#

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