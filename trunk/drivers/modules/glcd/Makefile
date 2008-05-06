ROOT_PATH=/home/vijay
COMPILER_PATH=/opt/crosstool/gcc-3.3.4-glibc-2.3.2/bin
CC=$(COMPILER_PATH)/arm-linux-gcc
LD=$(COMPILER_PATH)/arm-linux-ld
INCLUDE_PATH=$(ROOT_PATH)/proj/base/kernel/linux24/include/linux
KERNELDIR=$(ROOT_PATH)/proj/base/kernel/linux24

include $(KERNELDIR)/.config

CFLAGS = -D__KERNEL__ -DMODULE -DEXPORT_SYMTAB -I$(KERNELDIR)/include -O -Wall

#ifdef CONFIG_SMP
	CFLAGS += -D__SMP__ -DSMP
#endif

all:glcd.o glcd_base.o
	$(LD) -r -o glcd glcd.o glcd_base.o

clean:
	rm *.o glcd
