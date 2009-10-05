#export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-
#export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux-
export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/gcc-4.0.2-glibc-2.3.2/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-

ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
	module-objs := hello.o
else
#	KERNELDIR ?= /home/vijay/proj/base/kernel/linux-2.6.21-ts/
	KERNELDIR ?= /home/vijay/toobo/toobo/kernel/linux-2.6/
	PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o core .*.cmd *.ko *.mod.c .tmp_versions *.symvers hello
