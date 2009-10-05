## Trying to see if the no file found issue fixes when typing in export.
#export COMPILER_PATH=/home/vijay/toobo/toobo/toolchains/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-
#export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux-
export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/gcc-4.0.2-glibc-2.3.2/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-

export CC=$(CROSS_COMPILE)gcc

KERNELDIR ?= /home/vijay/toobo/toobo/kernel/linux-2.6

INCLUDE_PATH=$(KERNELDIR)/include/linux


all: htest

clean:
	rm *.o htest

