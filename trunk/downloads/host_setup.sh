#!/bin/bash

## This script will help you download and extract the appropriate 
## kernel source and toolchains for your build.

## Directory variables
TOP_LEVEL_DIR=..
SCRIPTS_DIR=$TOP_LEVEL_DIR/scripts
DOWNLOAD_DIR=$TOP_LEVEL_DIR/downloads
KERNEL_SOURCES_DIR=$TOP_LEVEL_DIR/kernel
PATCHES_DIR=$KERNEL_SOURCES_DIR/patches
TOOLCHAIN_DIR=$TOP_LEVEL_DIR/toolchains

## Script names
DOWNLOAD_SCRIPT=download_to_directory.sh
PATCH_SCRIPT=patch_source.sh

## Symbolic names
SYM_LINK_KERNEL_SRC=linux-2.6

## URIs
BASE_URI=http://toobo.googlecode.com/files
TAR_GZ=.tar.gz
KERNEL_ARCHIVE_NAME=linux-2.6.21-ts
TOOLCHAIN_ARCHIVE_NAME=crosstool-linux26-3.4.4-2005q3-2

## Download the kernel source
echo "Downloading the kernel source..."
sh $SCRIPTS_DIR/$DOWNLOAD_SCRIPT $BASE_URI/$KERNEL_ARCHIVE_NAME$TAR_GZ $DOWNLOAD_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while downloading the kernel source!"
    exit 1
fi

## Extract the kernel source to the appropriate directory
echo "Extracting the Kernel source..."
tar -xzf $KERNEL_ARCHIVE_NAME$TAR_GZ -C $KERNEL_SOURCES_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while extracting the kernel source!"
    exit 1
fi

## Create a symlink to the kernel source
echo "Creating symlink to kernel source"
ln -s $KERNEL_SOURCES_DIR/$KERNEL_ARCHIVE_NAME $KERNEL_SOURCES_DIR/$SYM_LINK_KERNEL_SRC

## Now patch the kernel if needed.
echo "Patching the Kernel source..."
sh $SCRIPTS_DIR/$PATCH_SCRIPT $KERNEL_SOURCES_DIR/$KERNEL_ARCHIVE_NAME $PATCHES_DIR/$KERNEL_ARCHIVE_NAME

## Download the toolchain
echo "Downloading the toolchain source..."
sh $SCRIPTS_DIR/$DOWNLOAD_SCRIPT $BASE_URI/$TOOLCHAIN_ARCHIVE_NAME$TAR_GZ $DOWNLOAD_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while downloading the toolchain!"
    exit 1
fi

echo "Extracting the Toolchain..."
## Extract the toolchain to the appropriate directory.
tar -xzf $TOOLCHAIN_ARCHIVE_NAME$TAR_GZ -C $TOOLCHAIN_DIR

if [ $? -ne 0 ]
then
    echo "Something went wrong while extracting the toolchain!"
    exit 1
fi

echo "Setup Complete!!"

