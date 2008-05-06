ROOT_PATH=/home/vijay
COMPILER_PATH=/opt/crosstool/gcc-3.3.4-glibc-2.3.2/bin
CC=$(COMPILER_PATH)/arm-linux-gcc
INCLUDE_PATH=$(ROOT_PATH)/proj/kernel/linux24/include/linux

all: i2c_scan.o
	$(CC) -o i2c_scan i2c_scan.o

eeprom.o: eeprom.c
	$(CC) -o i2c_scan.o -c i2c_scan.c

clean:
	rm *.o i2c_scan
