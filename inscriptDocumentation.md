#Idea and working of inscript.sh file

# Introduction #

This page would be updated as and when the inscript.sh file is modified especially if there are any functional changes. The inscript.sh file was created so that we can easily insert custom driver modules into the linux kernel for toobo. The organization of files and folders supporting inscript will hopefully help maintaining different versions of toobo easily.


# Details #
The directory structure surrounding inscript is as follows -

```
scripts-+
        |
        +--inscript.sh (core script file)
        |
        +-+-toobo.v.1.0 (directory)
        |  |
        |  +--general.settings
        |  +--glcd.drv.params
        |  +--audio.drv.params
        |
        +-+-toobo.v.2.0 (directory)
          |
          +--general.settings
          +--glcd.drv.params
          +--audio.drv.params
          +--keypad.drv.params
```

The following is an extract from the header section of the inscript.sh file.
```
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
```