CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

## Currently this system only supports unix like operating systems.
IF ( NOT ${CMAKE_HOST_UNIX} )
  MESSAGE("This build is supported only on UNIX like platforms")
  RETURN()
ENDIF ( NOT ${CMAKE_HOST_UNIX} )

PROJECT(toobo)

## Setup Kernel version
SET ( KERNEL_NAME linux-2.6.21-ts )

## Setup which Toolchain Version to use.
SET ( OABI true )

SET ( CMAKE_DIR cmake )

## Setup directory variables
INCLUDE ( ${CMAKE_DIR}/paths.cmake )

## Setup the toolchain
INCLUDE ( ${CMAKE_DIR}/toolchain.cmake )

## Setup file-name variables
SET ( KERNEL_IMAGE_NAME zImage )

## Environment variables
IF ( DEFINED OABI )
  MESSAGE ("Using Old ABI Toolchain!")
## Note with the 2.6 kernel, the default old eabi toolchain provided by embedded arm i.e. 
## gcc-3.3.4-glibc-2.3.2 doesn't work. Instead using gcc-4.0.2-glibc-2.3.2 from crosstool download.
#  SET ( CROSS_COMPILE ${TOOLCHAIN_DIR}/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux- )
  SET ( CROSS_COMPILE ${TOOLCHAIN_DIR}/gcc-4.0.2-glibc-2.3.2/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu- )
ELSEIF ( DEFINED EABI )
  MESSAGE ("Using New ABI Toolchain!")
  SET ( CROSS_COMPILE ${TOOLCHAIN_DIR}/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi- )
ENDIF ( DEFINED OABI )

## Build the kernel if needed.
INCLUDE ( ${CMAKE_DIR}/build_kernel.cmake )

## Once the Kernel is built, build the driver modules.
INCLUDE ( ${CMAKE_DIR}/build_drivers.cmake )

## Doesn't work. seems to go into a loop...
ADD_CUSTOM_TARGET (mrproper
  COMMAND cd ${KERNEL_DIR};
  COMMAND make mrproper;
  )

ADD_SUBDIRECTORY(app)
ADD_SUBDIRECTORY(tests)
