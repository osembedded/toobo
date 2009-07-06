#!/bin/bash
##########################################################################
# compile_kernel.sh [Part of the TOOBO project]
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
# Shell script that compiles the kernel
#
# Usage -
#         ./compile_kernel.sh <toolchain_dir>
#
# Note  -
#  This script needs to be run from the directory where the top 
#  level Makefile for the kernel can be found.
#

if [ $# -lt 1 ]
then
echo "Error: Usage: $0 <toolchain_dir>"
exit 1
fi

echo "CROSS_COMPILE prefix: $1"

export CROSS_COMPILE=$1 
make ts72xx_defconfig && make

if [ $? -ne 0 ]
then
    echo "Kernel compilation failed!"
    exit 1
fi

echo "Done Compiling Kernel"
