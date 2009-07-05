cmake_minimum_required(VERSION 2.6)

## Currently this system only supports unix like operating systems.
if ( NOT ${CMAKE_HOST_UNIX} )
  EchoString("This build is supported only on UNIX like platforms")
  return()
endif ( NOT ${CMAKE_HOST_UNIX} )

project(toobo)

## Setup directory variables
SET ( SCRIPT_DIR ${CMAKE_HOME_DIRECTORY}/scripts )
SET ( DRIVERS_DIR ${CMAKE_HOME_DIRECTORY}/drivers )
SET ( KERNEL_SRC_DIR ${CMAKE_HOME_DIRECTORY}/kernel )
SET ( TOOLCHAIN_DIR ${CMAKE_HOME_DIRECTORY}/toolchains/arm-none-linux-gnueabi/bin/ )
SET ( APP_SRC_DIR ${CMAKE_HOME_DIRECTORY}/app )
SET ( DOWNLOADS_DIR ${CMAKE_HOME_DIRECTORY}/downloads )

### Set up Env variables
Set(KERNEL_DIR kernel/linux-2.6.21-ts)

add_subdirectory(drivers)
add_subdirectory(app)
