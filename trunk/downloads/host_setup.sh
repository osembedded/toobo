#!/bin/bash

## This script will help you download and extract the appropriate 
## kernel source and toolchains for your build.

## Directory variables
TOP_LEVEL_DIR=..
SCRIPTS_DIR=$TOP_LEVEL_DIR/scripts
DOWNLOAD_DIR=$TOP_LEVEL_DIR/downloads
KERNEL_SRC_DIR=$TOP_LEVEL_DIR/kernel
TOOLCHAIN_DIR=$TOP_LEVEL_DIR/toolchains

## Script names
DOWNLOAD_SCRIPT=download_to_directory.sh

## URIs
BASE_URI=http://toobo.googlecode.com/files
KERNEL_ARCHIVE_NAME=tskernel-2.6.21-ts-src.tar.gz
TOOLCHAIN_ARCHIVE_NAME=crosstool-linux26-3.4.4-2005q3-2.tar.gz

## Download the kernel source
echo "Downloading the kernel source..."
sh $SCRIPTS_DIR/$DOWNLOAD_SCRIPT $BASE_URI/$KERNEL_ARCHIVE_NAME $DOWNLOAD_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while downloading the kernel source!"
    exit 1
fi

## Extract the kernel source to the appropriate directory
echo "Extracting the Kernel source..."
tar -xzf $KERNEL_ARCHIVE_NAME -C $KERNEL_SRC_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while extracting the kernel source!"
    exit 1
fi

## Download the toolchain
echo "Downloading the toolchain source..."
sh $SCRIPTS_DIR/$DOWNLOAD_SCRIPT $BASE_URI/$TOOLCHAIN_ARCHIVE_NAME $DOWNLOAD_DIR
if [ $? -ne 0 ]
then
    echo "Something went wrong while downloading the toolchain!"
    exit 1
fi

echo "Extracting the Toolchain..."
## Extract the toolchain to the appropriate directory.
tar -xzf $TOOLCHAIN_ARCHIVE_NAME -C $TOOLCHAIN_DIR

if [ $? -ne 0 ]
then
    echo "Something went wrong while extracting the toolchain!"
    exit 1
fi

echo "Setup Complete!!"

