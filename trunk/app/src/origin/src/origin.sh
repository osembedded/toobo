#!/bin/sh
##########################################################################
# origin.sh [Part of the TOOBO project]
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
# Shell script to insmod any custom driver module for toobo
# Usage -
#         ./origin PATH_TO_MODEL_SPECIFIC_SCRIPT_FILE
#
# This script will be the first 'application' level script to 
# be executed when toobo starts up. Depending on what version 
# of toobo is being started, this will point to the appropriate 
# model specific config file. (passed in as parameter)
#
# Files involved with origin.sh
# origin.sh - The core script file that starts the model specific origin file.
# origin.common.sh - The core script file that is executed by origin.sh before 
#                    executing any model specific scripts.
#
# PATH_TO_MODEL_SPECIFIC_SCRIPT_FILE (or)
# origin.toobo.v.<model number>.sh - The model specific script file that is 
#                    executed to start the ball rolling for that model. The 
#                    <model number> is in the format '1.0' or '3.12' etc...
#

### Start of Functions ###
### End of Functions ###

### Start of 'code' ###
clear;
echo ">]] origin - v1.0";
if [ "$1" ]
    then
    sh origin.common.sh;
    sh `echo $1`;
    else
    echo origin Usage:;
    echo "     ./origin PATH_TO_MODEL_SPECIFIC_SCRIPT_FILE"
    echo
fi

### End of 'code' ###

exit 0
