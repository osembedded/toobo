ROOT_PATH=/home/vijay
COMPILER_PATH=/opt/crosstool/gcc-3.3.4-glibc-2.3.2/bin
CC=$(COMPILER_PATH)/arm-linux-gcc
INCLUDE_PATH=$(ROOT_PATH)/proj/kernel/linux24/include/linux

all: eeprom.o
	$(CC) -o eep eeprom.o

eeprom.o: eeprom.c
	$(CC) -o eeprom.o -c eeprom.c

clean:
	rm *.o eep