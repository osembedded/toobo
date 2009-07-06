CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

## Currently this system only supports unix like operating systems.
IF ( NOT ${CMAKE_HOST_UNIX} )
  MESSAGE("This build is supported only on UNIX like platforms")
  RETURN()
ENDIF ( NOT ${CMAKE_HOST_UNIX} )

PROJECT(toobo)

## Setup Kernel version
SET ( KERNEL_NAME linux-2.6.21-ts )

## Setup directory variables
SET ( SCRIPT_DIR ${CMAKE_HOME_DIRECTORY}/scripts )
SET ( DRIVERS_DIR ${CMAKE_HOME_DIRECTORY}/drivers )
SET ( KERNEL_SOURCES_DIR ${CMAKE_HOME_DIRECTORY}/kernel )
SET ( TOOLCHAIN_DIR ${CMAKE_HOME_DIRECTORY}/toolchains )
SET ( APP_SRC_DIR ${CMAKE_HOME_DIRECTORY}/app )
SET ( DOWNLOADS_DIR ${CMAKE_HOME_DIRECTORY}/downloads )
SET ( KERNEL_DIR ${KERNEL_SOURCES_DIR}/${KERNEL_NAME} )
SET ( KERNEL_IMAGE_DIR ${KERNEL_DIR}/arch/arm/boot )

## Setup file-name variables
SET ( KERNEL_IMAGE_NAME zImage )

## Environment variables
SET ( CROSS_COMPILE ${TOOLCHAIN_DIR}/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi- )

## Build the kernel first if not already built.
IF ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )
 MESSAGE("Kernel already built. Skipping...")
ELSE ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )
 MESSAGE("Building Kernel...")
 EXECUTE_PROCESS( COMMAND ${SCRIPT_DIR}/compile_kernel.sh ${CROSS_COMPILE}
   WORKING_DIRECTORY ${KERNEL_DIR})
ENDIF ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )

ADD_SUBDIRECTORY(drivers)
ADD_SUBDIRECTORY(app)
