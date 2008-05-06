/**
 * i2c_scan.c [Part of the TOOBO project]
 * 
 * Copyright 2008 Vijay Jayaraman (osembedded@gmail.com) 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Lesser General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include "i2c_scan.h"
#include <linux/i2c-dev.h>

#define I2C_DEV_PATH "/dev/i2c/0"
#define STA015_ADDR  0x43
#define CS4341A_ADDR 0x10 // depends on jumper AD0/CS as well.

static int check_known_devices = 0; // whether to scan all port ranges

static int open_i2c(char *device);
static int close_i2c(int fileid);
static void scan_i2c_bus(int file);
static int set_slave(int file, int address);

int main (int argc, char** argv)
{
   int fd = 0;
   char mybuf[16 + 1] = {0};

   TRACE_IN;

   if(argc > 1)
   {
      DBG("\nScanning for known I2C devices(STA015 and CS4341A)...");
      check_known_devices = 1;
   }
   else
   {
      DBG("\nScanning for all devices...");
   }

   fd = open_i2c(I2C_DEV_PATH);

   // scan for devices in the i2c bus
   if(-1 != fd)
   {
      scan_i2c_bus(fd);
   }

   close_i2c(fd);
   TRACE_OUT;
   DBG("\n");
}

static int open_i2c(char *device)
{
   int file = -1;
   TRACE_IN;

   if ((file = open(device, O_RDWR)) == -1)
   {
      int err = errno;
      DBG("\nCould not open %s: %s!", device, strerror(err));
      return -1;
   }
  
   TRACE_OUT;
   return file;
}

static int close_i2c(int fileid)
{
   int retval = -1; 
   TRACE_IN;

   if(fileid != -1)
   {
      if((retval = close(fileid)) == -1)
      {
         int err = errno;
         DBG("\nCould not close (%d): %s!", fileid, strerror(err));
         return -1;
      }
   }

   TRACE_OUT;
   return retval;
}

static void scan_i2c_bus(int file)
{
   int port;
   TRACE_IN;
  
   for (port=0; port<127; port++)
   {
      int res;

      if(check_known_devices &&
         (port != STA015_ADDR &&
          port != CS4341A_ADDR))
         {
            continue;
         }

      usleep(500);
      if (set_slave(file, port) != -1) {
         res = i2c_smbus_read_byte(file);
         if (res >= 0)
         {
	    DBG("\ni2c chip found at: %x, val = %d", port, res);
         }
/*         else
           printf("\ni2c chip NOT found at: %x, val = %d", port, res); */
      }
   }
   TRACE_OUT;
}

static int set_slave(int file, int address)
{
   /* use FORCE so that we can read registers even when
      a driver is also running */
   if (ioctl(file, I2C_SLAVE_FORCE, address) < 0) {
      DBG("Error: Could not set address to %d: %s (fd=%d)",
          address, strerror(errno), file);
      return -1;
   }

   return 0;
}
