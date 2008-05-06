ROOT_PATH=/home/vijay
COMPILER_PATH=/opt/crosstool/gcc-3.3.4-glibc-2.3.2/bin
CC=$(COMPILER_PATH)/arm-linux-gcc
INCLUDE_PATH=$(ROOT_PATH)/proj/kernel/linux24/include/linux

all: display-test.o
	$(CC) -o display-test display-test.o

display-test.o: display-test.c
	$(CC) -o display-test.o -c display-test.c

clean:
	rm *.o display-test
