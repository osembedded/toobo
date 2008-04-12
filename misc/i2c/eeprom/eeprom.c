#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include "eeprom.h"
#include <linux/i2c-dev.h>

#define DBG(x...) printf(x)
#define TRACE_IN  printf("\n%s(+):", __FUNCTION__)
#define TRACE_OUT printf("\n%s(-):", __FUNCTION__)

struct i2c_rdwr_ioctl_data myi2cdata;


static int open_i2c(char *device);
static int close_i2c(int fileid);
static void scan_i2c_bus(int file);
static int set_slave(int file, int address);

// main should typically be in an application function...
// here for simplicity
#define TEST_STR "abcdefghijklmnop"
int main (void)
{
  int fd = 0;
  char mybuf[16 + 1] = {0};

  TRACE_IN;

  fd = open_i2c("/dev/i2c/0");

  //scan_i2c_bus(fd);

  //clear the eeprom before beginning.
  clear_24lc16b_eeprom(fd);

  write_eeprom(fd,
	       0x50,
	       0,
	       TEST_STR,
	       sizeof(TEST_STR));

  // sleep for a while until write completes in eeprom
  usleep(100 * 1);

  read_eeprom(fd,
	      0x50,
	      5,
	      &mybuf[0],
	      16);

  mybuf[16] = 0;
  DBG("\nRead back: %s.", mybuf);

  close_i2c(fd);
  TRACE_OUT;
}

static int open_i2c(char *device)
{
  int file = -1;
  TRACE_IN;

  if ((file = open(device, O_RDWR)) < 0)
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
  TRACE_IN;
  if(fileid)
    {
      close(fileid);
    }
  TRACE_OUT;
}

static void scan_i2c_bus(int file)
{
  int port;
  TRACE_IN;
  
  for (port=0; port<127; port++)
    {
      int res;
      if (set_slave(file, port) != -1) {
	res = i2c_smbus_read_byte(file);
	if (res >= 0)
	  {
	    printf("i2c chip found at: %x, val = %d\n", port, res);
	  }
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

#define MAX_BUF_SIZE 16
#define ADDRESS_BYTE 1
static unsigned char priv_read_buf[MAX_BUF_SIZE];
/* Note: The EEPROM allows us to read all 256 byts in one go.
But we will follow what write does and read only 16 bytes or 
one page at a time. */
unsigned int read_eeprom(int file, 
			 int dev_addr, 
			 unsigned char addr, /* only b/w 0-15 */
			 unsigned char *buf,
			 int num_bytes)
{
  int retval = 0; // bytes read
  unsigned int data_read = 0;
  TRACE_IN;

  if(buf != NULL &&
     num_bytes > 0)
    {
      if(set_slave(file, dev_addr) != -1)
	{
	  if(num_bytes > MAX_BUF_SIZE)
	    {
	      num_bytes = MAX_BUF_SIZE;
	      DBG("\n Attempting to read more than %d bytes!", MAX_BUF_SIZE);
	    }

	  retval = write(file, &addr, 1);
	  if(retval > 0)
	    {
	      DBG("\n Success: writing %d bytes. \n", retval);
	    }
	  
	  retval = read(file, buf, num_bytes);
	  if(retval > 0)
	    {
	      DBG("\n Success: read %d bytes. \n", retval);
	    }
	}
    }

  TRACE_OUT;
  return retval;
}

static unsigned char priv_write_buf[MAX_BUF_SIZE + ADDRESS_BYTE];
unsigned char write_eeprom(int file, 
			   int dev_addr, 
			   unsigned char addr, /* only b/w 0-15 */
			   unsigned char *buf,
			   int num_bytes)
{
  int retval = 0;

  TRACE_IN;
  if(buf != NULL &&
     num_bytes > 0)
    {
      if(set_slave(file, dev_addr) != -1)
	{
	  if(num_bytes > MAX_BUF_SIZE)
	    {
	      num_bytes = MAX_BUF_SIZE;
	      DBG("\n Attempting to write more than 16 bytes!");
	    }

	  // copy the address byte to the first byte position.
	  priv_write_buf[0] = addr;

	  // copy the data after the address byte.
	  memcpy(&priv_write_buf[1], buf, num_bytes);

	  // write address byte + num_bytes
	  retval = write(file, &priv_write_buf[0], num_bytes + 1);

	  if(retval > 0)
	    {
	      DBG("\n Success: writing %d bytes. \n", retval);
	    }
	}
    }

  TRACE_OUT;
  return retval;
}

void clear_24lc16b_eeprom(int fd)
{
  int page = 0;
  unsigned char blank[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  TRACE_IN;

  for(page = EE_PAGE1; page <= EE_PAGE8; page++)
    {
      write_eeprom(fd,
		   page,
		   0,
		   blank,
		   sizeof(blank));
      
      // sleep for a while until write completes in eeprom
      usleep(100 * 1);
    }

  TRACE_OUT;
}
