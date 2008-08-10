#!/bin/sh
##########################################################################
# inscript.sh [Part of the TOOBO project]
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
#         ./inscript PATH_TO_GEN_SETTINGS_FILE
#
# This script will automatically insmod the required driver
# modules and figure out which device node needs to be created 
# in /dev/ by looking at the /proc/devices file. It will also 
# set the right permissions and groups for the devices.
#
# Files involved with inscript.sh
# inscript.sh - The core script file that helps insmod drivers
# general.settings - This file contains generic information on the 
#                    future files that are to be parsed.
# {filename}.drv.params - These are the metadata files that contain 
#                         information on how to insert the modules 
#                         and create device nodes to successfully 
#                         insert the drivers into the kernel. These
#                         need to reside in the same folder as the 
#                         general.settings file. The enabled field 
#                         inside this file tells the inscript whether 
#                         to use the file or not.
#
# ADVANTAGES:
# The general idea is that, each version of toobo will have a 
# folder with the relevant files i.e. a general.settings file and 
# multiple {filename}.drv.params files. Module names, device names,
# ko file path all are confiurable using this approach. We will just 
# have the makefile in tooboo's build process point to the appropriate 
# folder as when the inscript is passed a parameter.
#
# DRAWBACKS:
# The current downside to this approach is that the order in which 
# the modules are inserted is not customizable. We could rename the 
# {filename}.drv.params with a new format i.e say 
# {order}.{filename}.drv.params where {order} could be any number.
# This way we can control the sequence in which the modules are 
# inserted. But this would be a nightmare as far as source conrol 
# is concerned. So... this approach is not recommended and future 
# versions of inscript will support module loading order.

### Start of Functions ###
parseValue(){
    if [[ "$1" && "$2" ]]
	then
	parseValueResult="";  # reset it # dedicated var to store the result.
	parseValueQuery="$2"; # dedicated var to store the query.
	parseValueFile="$1";
	parseValueResult=`awk -F":" '/'$parseValueQuery'/ {print $2}' "$parseValueFile"`;
    fi
}

# The diff between retValue and parseValue is that the 
# retValue function can be directly assigned to a variable.
# whereas in parseValue, we need 2 lines. one to execute parseValue
# and a second one to assing parseValueResult to the desired variable.
# retValue makes it easy to assign parsed values to variables.
# example:
#   newvar=`retValue metaname`;
retValue(){
    parseValue $1 $2;
    echo $parseValueResult;
}

# Removes any old nodes that already exists in the target.
removeOldNode(){
    echo "Removing any old Device Node: " $1;
    if [ -a $1 ]
    then
	rm -f $1;
    fi
}

findMajorNumber(){
    FMN_DEVNAME=$1;
    #echo "findMajorNumber "$1;
    #echo "## awk '/([0-255]?) (\<'$1'\>)/ {print $1}' /proc/devices"
    MAJOR_NUM=`awk '/'$FMN_DEVNAME'/ {print $1}' /proc/devices`;
}

createNewNode(){
    NNAME=$1;
    NTYPE=$2;
    NMAJOR_NUM=$3;
    MINOR_NUM=0;

    # example - mknod /dev/$device c $major 0
    # echo $NNAME $NTYPE $NMAJOR_NUM $MINOR_NUM;
    mknod $NNAME $NTYPE $NMAJOR_NUM $MINOR_NUM;

}

# Removes any old nodes
# Inserts the new module
# Finds out the major number from /proc/devices
# Creates the node in the required path appropriately
# Assigns group and permissions as per requirement
insertModules(){
    MOD_METANAME=`retValue $1 metaname`;
    MOD_KONAME=`retValue $1 koname`;
    MOD_ENABLED=`retValue $1 enabled`;
    if [[ $MOD_ENABLED == true ]]
    then
	echo "Current Module being processed - $MOD_METANAME ( $MOD_KONAME )";
	MOD_NODENAME=`retValue $1 nodename`;
	MOD_DEVTYPE=`retValue $1 devtype`;
	MOD_SRCPATH=`retValue $1 modsrcpath`;
	MOD_GROUP=`retValue $1 devgroup`;
	MOD_PERM=`retValue $1 devperm`;
	MOD_PARAMS=`retValue $1 modparams`;
	MOD_DEVNAME=`retValue $1 devname`;
	
	# Remove any old nodes
	removeOldNode $MOD_NODENAME;

	# Insert the new module
	echo "insmod "$MOD_SRCPATH/$MOD_KONAME;
	insmod $MOD_SRCPATH/$MOD_KONAME;
	
	# Find out the MAJOR number of from /proc/devices
	#awk '/([0-255]?) (\<usb\>)/ {print $1}' /proc/devices
	# MOD_NAME_WOKO=`echo ${MOD_NAME%.*ko}`;
	findMajorNumber $MOD_DEVNAME;
	# echo "MAJOR number is " $MAJOR_NUM;

        # Create new node for the major number obtained
	createNewNode $MOD_NODENAME $MOD_DEVTYPE $MAJOR_NUM;

	# Change Permissions & Group
	
    else
	echo "Skipping Module - $MOD_METANAME ( $MOD_KONAME )";
    fi

}

# Find all the settings file in the directory 
# we are interested in.
findDriverSettingFiles(){
    echo;
}

handleFormat_1_0(){
#    echo handleFormat_1_0;
    BASEPATH=`retValue $1 basepath`;
    GS_FILE_PATH=$1;
    GS_FOLDER=`echo ${GS_FILE_PATH%gen*ngs}`;
#    echo GS_FOLDER is $GS_FOLDER;

    # find drv.params files in the GS_FOLDER directory 
    # and loop through each file to figure out which 
    # modules to insert;
    for PARAM_FILE in `find $GS_FOLDER -name *.drv.params`
    do
	#echo $PARAM_FILE;
        # From this point forward, we do not need to 
        # pass the original file name for gen.settings.
	insertModules $PARAM_FILE;
    done;
}

# $1 here is the full path name to general.settings file
# the uesr provides via command line.
processGenSettingsFile(){
    echo ">]] Processing general.settings file - " $1
    GS_METANAME=`retValue $1 metaname`
    GS_FORMAT=`retValue $1 genSettingsFormat`;
    echo $GS_METANAME format: $GS_FORMAT;

    if [[ $GS_FORMAT == 1.0 ]]
    then
	echo Format $GS_FORMAT Supported.;
	handleFormat_1_0 $1;
    else
	echo Format $GS_FORMAT NOT supported!!;
    fi
}

### End of Functions ###

### Start of 'code' ###
clear;
echo ">]] inscript - v1.0";
if [ "$1" ]
    then
    processGenSettingsFile $1;
    else
    echo inscript Usage:;
    echo "    ./inscript PATH_TO_GEN_SETTINGS_FILE"
    echo
fi

### End of 'code' ###

exit 0
