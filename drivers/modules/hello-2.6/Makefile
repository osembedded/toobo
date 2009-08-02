export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-

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
