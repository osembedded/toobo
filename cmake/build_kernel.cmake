## Check to see if the toolchain and the kernel sources are in place first.
IF ( NOT EXISTS ${KERNEL_DIR} )
 MESSAGE("Please install the kernel source by running ./scripts/host_setup.sh")
 RETURN()
ENDIF ( NOT EXISTS ${KERNEL_DIR} )

IF ( NOT EXISTS ${TOOLCHAIN_BIN_DIR} )
 MESSAGE("Please install the toolchain by running ./scripts/host_setup.sh")
 RETURN()
ENDIF ( NOT EXISTS ${TOOLCHAIN_BIN_DIR} )

## Build the kernel first if not already built.
IF ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )
 MESSAGE("Kernel already built. Skipping...")
ELSE ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )
 MESSAGE("Building Kernel...")
 EXECUTE_PROCESS( COMMAND ${SCRIPT_DIR}/compile_kernel.sh ${CROSS_COMPILE}
   WORKING_DIRECTORY ${KERNEL_DIR})
ENDIF ( EXISTS ${KERNEL_IMAGE_DIR}/${KERNEL_IMAGE_NAME} )
