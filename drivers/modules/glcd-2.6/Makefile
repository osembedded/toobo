export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-

ifneq ($(KERNELRELEASE),)
	obj-m := graphiclcd.o
	graphiclcd-objs := glcd_base.o glcd.o
else
	KERNELDIR ?= /home/vijay/proj/base/kernel/linux-2.6.21-ts/
	PWD	:= $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o core .*.cmd *.ko *.mod.c .tmp_versions *.symvers
