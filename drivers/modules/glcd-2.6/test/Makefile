#export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-
export CROSS_COMPILE=/home/vijay/toobo/toobo/toolchains/gcc-4.0.2-glibc-2.3.2/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-

export CC=$(CROSS_COMPILE)gcc

KERNELDIR ?= /home/vijay/toobo/toobo/kernel/linux-2.6

INCLUDE_PATH=$(KERNELDIR)/include/linux

#all: display-test.o
#	$(CC) -o display-test display-test.o

#display-test.o: display-test.c
#	$(CC) -o display-test.o -c display-test.c

all: display-test

clean:
	rm *.o display-test
