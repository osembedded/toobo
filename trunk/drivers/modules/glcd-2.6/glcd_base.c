/**
 * glcd_base.c [Part of the TOOBO project]
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

/**
PORTA EPGIO 0:7 - lcd data port 0:7
  -- These are EGPIO but we do not want the 'E'
     functionality. So we need to turn off the 
     interrupt features. Set GPIOAIntEn to 0s.
  -- Configure these as output to begin with.
     i.e. set them (PADDR) to all 1s for output.
  -- When you need to read back, set these (PADDR)
   n  to all 0s for input.    

PORTB EGPIO 14:15 - lcd chip select signal for IC1, IC2
  -- These are EGPIO but we do not want the 'E'
     functionality. So we need to turn off the 
     interrupt features. Set GPIOBIntEn to 0s.
  -- Configure these as output to begin with.
     i.e. set the (PBDDR:7:6) to all 1s for outputs.
BGPIO_15  - PB7  - CS1  0x01.. ....
BGPIO_14  - PB6  - CS2  0x10.. ....

PORTH
  -- Set GPIOFIntEn to 0s to disable interrupts.
  -- All these control pins are output only. Set 
     PHDDR (data dir reg) to 1s for output.
HGPIO_3 *1*         - LCD_EN
HGPIO_4 *2*         - LCD_DI (DI is same as RS)
HGPIO_5 *3*         - LCD_RW

0x.. RW DI _ EN ...
**/

/*
 *  Kernel Header files
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <asm/io.h>

/*
 *  local Header files
 */
#include "glcd_common.h"
#include "glcd_ports.h"

/*
 *  Defines
 */
/* Help prevent updating all of the display pixels
   everytime to draw a small region. Faster display refresh. */
#define REDRAW_ENHANCEMENT

// lcd commands
#define BUSY_FLAG     0x80
#define DISPLAY_FLAG  0x20
#define RESET_FLAG    0x10

#define DISPLAY_OFF   0x3E // turn off display
#define DISPLAY_ON    0x3F // turn on display

// lcd page selection 
#define SEL_PAGEA     (*(pbdr) |= 0x40, *(pbdr) &= 0x7F)
#define SEL_PAGEB     (*(pbdr) |= 0x80, *(pbdr) &= 0xBF)
#define SEL_PAGE_ALL  (*(pbdr) |= 0xC0)

// These delay values are calibrated for the EP9301 
// CPU running at 166 Mhz, but should work also at 200 Mhz
#define SETUP	15
#define PULSE	36
#define HOLD	22

// NOT SURE WHAT THIS IS...
#define CHAR_HEIGHT 16 //16

/* LCD related */
#define PHYS_LCD_WIDTH (64 * 2)
#define PHYS_LCD_HEIGHT (64)
#define PHYS_VPAGES 8
#define LCD_WIDTH      128
#define LCD_HEIGHT     (64/sizeof(char))
// Local Display Ram for LCD
char display_ram [LCD_WIDTH] [LCD_HEIGHT];
char dirty_ram [LCD_WIDTH] [LCD_HEIGHT];


// The following is of the format
// asm volatile {code: output operand list: input operand list: clobber list};
//                      operand 0              operand 1          operand 2
// volatile tells the compiler not to optimize the code.
// "\n" makes the assembler listing readable by inserting new lines.

#define COUNTDOWN(x)	asm volatile ( \
  "1:\n"\
  "subs %1, %1, #1;\n"\
  "bne 1b;\n"\
  : "=r" ((x)) : "r" ((x)) \
);

typedef enum LCD_PG{
  PAGE_A,
  PAGE_B,
  PAGE_ALL,
}LCD_PAGE;

typedef enum _SET_RESET{
  RESET = 0,
  SET,
}SET_RESET;

/*
 *  Static Variables
 */
volatile unsigned int *padr;
volatile unsigned int *paddr;
volatile unsigned int *pbdr;
volatile unsigned int *pbddr;
volatile unsigned int *phdr;
volatile unsigned int *phddr;

/*
 *  Function Prototypes
 */
static int _get_iomem(unsigned long start, unsigned long len, char *desc);
static void __command(unsigned int cmd);
static void __lcdwait(void);
static void __lcd_writedata(LCD_PAGE page, char data);

// these may be exported later as we proceed
static char __set_yaddr(char addr);
static char __set_xaddr(char addr);
static char __set_start_line(char addr);

// 'medium' level lcd draw functions
// need to clean the names up a bit later...
void write_pix(int pen_x, int pen_y, int height, int width);
void draw_pix(int pen_x, int pen_y, int height, int width);
void negate_image(unsigned int height, unsigned int width);
void pixel(SET_RESET op, int xx, int yy);
void write_buffer(unsigned char * bitmap,
		  int height,
		  int width,
		  unsigned int pen_x,
		  unsigned int pen_y,
		  unsigned char draw_bottom_first);
#ifdef REDRAW_ENHANCEMENT
int dirty_area (int cur_x, int cur_y,
                int pen_x, int pen_y, 
                int height, int width);
#endif

/*
 *  Function Definition
 */
int glcd_get_iomem(void)
{
   int retval = -1;
   int mem_padr  = FALSE;
   int mem_paddr = FALSE;

   TRACE;
   // get hold of the requierd iomem
   if(0 == _get_iomem(PHYS_ADDR_PADR, 8, "glcd: PADR-PBDR"))
   {
      mem_padr = TRUE;
      if(0 == _get_iomem(PHYS_ADDR_PADDR, 8, "glcd: PADDR-PBDDR"))
      {
         mem_paddr = TRUE;
         if(0 == _get_iomem(PHYS_ADDR_PHDR, 8, "glcd: PORTH"))
         {
            retval = 0;
         }
      }
   }

   if(retval != 0)
   {
      if(mem_padr == TRUE)
         release_mem_region(PHYS_ADDR_PADR, 8);

      if(mem_paddr == TRUE)
         release_mem_region(PHYS_ADDR_PADDR, 8);
   }
   else
   {
      /* map phys memory to virtual memory else your code may crash */
      padr   = (unsigned int *) __ioremap(PHYS_ADDR_PADR, 4, 0);
      paddr  = (unsigned int *) __ioremap(PHYS_ADDR_PADDR, 4, 0);
      pbdr   = (unsigned int *) __ioremap(PHYS_ADDR_PBDR, 4, 0);
      pbddr  = (unsigned int *) __ioremap(PHYS_ADDR_PBDDR, 4, 0);
      phdr   = (unsigned int *) __ioremap(PHYS_ADDR_PHDR, 4, 0);
      phddr  = (unsigned int *) __ioremap(PHYS_ADDR_PHDDR, 4, 0);
   }
   return retval;
}

int glcd_release_iomem(void)
{
   TRACE;
   release_mem_region(PHYS_ADDR_PADR, 8);
   release_mem_region(PHYS_ADDR_PADDR, 8);
   release_mem_region(PHYS_ADDR_PHDR, 8);   
   return 0; /* always a success. */
}

static int _get_iomem(unsigned long start, unsigned long len, char *desc)
{
   if(len == 0 || 
      desc == NULL)
      return -1;

   if( check_mem_region(start, len) )
   {
      printk(KERN_ERR "glcd: memory already in use\n");
      return -EBUSY;
   }
   else
   {
      request_mem_region(start, len, desc);
      return 0;
   }
}   

/*
 *  IOCTL functions
 */
void __lcd_clear(void)
{
  int ii = 0;
  int nextline = 0;

  /* clear the display ram. Not doing so on clear would leave 
     a residue on screen the next time something is updated! */ 
  memset(&display_ram[0][0], 0x00, sizeof(display_ram));

  for(nextline = 0; nextline < 8; nextline++)
    {
      // set display line to clear
      __command(__set_xaddr(nextline));

      for(ii = 0; ii < 64; ii++)
	{
	  __lcd_writedata(PAGE_ALL, 0x00);
	}
    }
}

/*
 *  Low Level LCD routines.
 */

static void __command(unsigned int cmd)
{
  int ii = 0;

  // NOTE: Fron the half baked data sheet, loosk like 
  // write doesn't require two cycles... It is written
  // from the MPU to data register and later to ddram 'automatically'

  // make sure lcd is not busy
  __lcdwait();

  // select both controller chips
  SEL_PAGE_ALL;

  // setup data port as output for writing.
  *(paddr) = 0xFF;

  // for writing command, RS is 0, RW is 0
  // 0x..00 ....
  *(phdr) &= 0xCF;

  // wait for the setup time
  ii = SETUP;
  COUNTDOWN(ii);

  // enable the lcd
  // 0x.... 1...
  *(phdr) |= 0x08;

  // Send the data...
  *(padr) = cmd;

  // Wait for a pulse width for lcd enable... [moved down...]
  ii = PULSE;
  COUNTDOWN(ii);

  // disable the lcd
  // 0x.... 0...
  *(phdr) &= 0xF7;

  // wait for the hold time...
  ii = HOLD;
  COUNTDOWN(ii);

  // reset the data port...
  // *(padr) = 0x00; [not resetting it...]

}


void __lcd_init(void)
{
   TRACE;

   // Set the Control pins to their default values.
   // Only LCD_EN needs to be low. Everything else is DONT CARE.
   // data reg 0x.... 0...
   // ddir reg 0x..11 1...
   *(phdr)  = *(phdr) & 0xF7;
   *(phddr) = *(phddr) | 0x38;

   // Do not select any LCD controller chip to start with
   // Set the LCD controller select pins to output 0x11.. ....
   *(pbdr) &= 0x3F;
   *(pbddr) |= 0xC0;

   // Wait for lcd first
   __lcdwait();

   // Select both pages and initialize
   SEL_PAGE_ALL;

   // Set X,Y address to 0
   // Set Display start Line to 0
   __command(__set_xaddr(0));
   __command(__set_yaddr(0));
   __command(__set_start_line(0));

   // Turn the display ON
   __command(DISPLAY_ON);

   // clear the lcd
   __lcd_clear();   
}

static void __lcdwait(void)
{
  int ii = 0;
  unsigned int data = 0;
  int busy = 1;

  // wait until the busy flag is cleared...
  // as well as the reset flag is cleared...
  // dummy read is not necessary for status read.
  do
    {
      // configure data pins as inputs
      // PADDIR -> 0x0000 0000
      *(paddr) = 0x00;

      // Select both the chips. From what I understand
      // from the manual, both + one more pin need to be high.
      SEL_PAGE_ALL;

      // get the RS and RW ready for read
      // 0x..10 ....
      // 0x..1. ....
      // 0x...0 ....
      *(phdr) |= 0x20;
      *(phdr) &= 0xEF;

      // wait for setup time
      ii = SETUP;
      COUNTDOWN(ii);

      // enable lcd operation
      // 0x.... 1...
      *(phdr) |= 0x08;

      ii = PULSE;
      COUNTDOWN(ii);

      // read data pins
      data = *(padr);

      // disable lcd operation
      // 0x.... 0...
      *(phdr) &= 0xF7;      

      // wait for hold time
      ii = HOLD;
      COUNTDOWN(ii);

      // clear the data pins
      *(padr) = 0x00;

      // update the busy flag 0x1... ....
      busy = ( (data & BUSY_FLAG) ||
               (data & RESET_FLAG) )?1:0;
	}while(busy);

  //printf("\n data read back finally is 0x%x. \n", data);
}

static void __lcd_writedata(LCD_PAGE page, char data)
{
  int ii = 0;

  // First wait for the LCD
  __lcdwait();

  // Select the page to write to
  switch(page)
    {
    case PAGE_A:
      SEL_PAGEA;
      break;
      
    case PAGE_B:
      SEL_PAGEB;
      break;

    case PAGE_ALL:
    default: //intentional fall thru...
    SEL_PAGE_ALL;
    break;
    }

  // Setup Data port as all outputs.
  *(paddr) |= 0xFF;

  // RS is 1 and RW is 0.
  // 0x..01 ....
  *(phdr) &= 0xDF;
  *(phdr) |= 0x10;

  // wait for the setup time
  ii = SETUP;
  COUNTDOWN(ii);

  // enable the lcd
  // 0x.... 1...
  *(phdr) |= 0x08;

  // Send the data...
  *(padr) = data;

  // Wait for a pulse width for lcd enable... [moved down...]
  ii = PULSE;
  COUNTDOWN(ii);

  // disable the lcd
  // 0x.... 0...
  *(phdr) &= 0xF7;

  // wait for the hold time...
  ii = HOLD;
  COUNTDOWN(ii);
}


char __set_yaddr(char addr)
{
  addr &= 0x7F;
  addr |= 0x40;
  return addr;
}

char __set_xaddr(char addr)
{
   addr &= 0xBF;
   addr |= 0xB8;
   return addr;
}

char __set_start_line(char addr)
{
  addr |= 0xC0;
  return addr;
}


/*
 *  LCD draw routines.
 */

// Set a pixel
void pixel(SET_RESET op, int xx, int yy)
{
  int dramx = 0;
  int dramy = 0;
  int dramy_bit = 0;
  char page = 0;

  if(xx > PHYS_LCD_WIDTH ||
     yy > PHYS_LCD_HEIGHT)
    {
      return;
    }

  dramx = xx;
  dramy = (yy/PHYS_VPAGES);

  dramy_bit = yy % PHYS_VPAGES;

  // read what is in the page currently.
  page = display_ram[dramx][dramy];

  switch(op)
    {
    case SET:
      // what will page be after the new set.
      page |= (1 << dramy_bit);
      break;
    case RESET:
      // what will page be after the re-set.
      page &= ~(1 << dramy_bit);
      break;
    default:
       printk(KERN_ERR "Unknown option in pixel()!");
      break;
    }

  // write back the page.
  display_ram[dramx][dramy] = page;

}

void lcd_draw_bitmap(unsigned char *bitmap, 
		     int pen_x, 
		     int pen_y, 
		     int height, 
		     int width,
		     unsigned char draw_bottom_first)
{
//   TRACE;

  // format the data into the way we want.
  write_buffer(bitmap, height, width, pen_x, pen_y, draw_bottom_first);

  // write the data into the lcd
  draw_pix(pen_x, pen_y, height, width);

}

void write_buffer(unsigned char * bitmap,
		  int height,
		  int width,
		  unsigned int pen_x,
		  unsigned int pen_y,
		  unsigned char draw_bottom_first)
{
  int xx = 0;
  int yy = 0;
  int idx = 0;
  
//  TRACE;
  if(((pen_x + width) > PHYS_LCD_WIDTH) ||
     ((pen_y + height) > PHYS_LCD_HEIGHT))
    {
      return;
    }

  //DBG("\nheight = %d, width = %d", height, width);

  if(draw_bottom_first == FALSE)
    {
      for(yy = 0; yy < height ; yy++)
	{
	  for(xx = 0; xx < width; xx++)
	    {
	      if(bitmap[idx] == 0)
		{
		  pixel(RESET, xx + pen_x, yy + pen_y);
		}
	      else
		{
		  pixel(SET, xx + pen_x, yy + pen_y);
		}
	      idx++;
	    }
	}
    }
  else
    {
      for(yy = CHAR_HEIGHT - height; yy < CHAR_HEIGHT; yy++)
	{
	  for(xx = 0; xx < width; xx++)
	    {
	      if(bitmap[idx] == 0)
		{
		  pixel(RESET, xx + pen_x, yy + pen_y);
		}
	      else
		{
		  pixel(SET, xx + pen_x, yy + pen_y);
		}
	      idx++;
	    }
	}
    }
}

void draw_pix(int pen_x, int pen_y, int height, int width)
{
  // Disable the LCD
#ifndef REDRAW_ENHANCEMENT
  SEL_PAGE_ALL;
  __command(DISPLAY_OFF);
#endif 

  //write to LCD DDRAM
  write_pix( pen_x,
             pen_y,
             height,
             width);

  // Enable the LCD
#ifndef REDRAW_ENHANCEMENT
  SEL_PAGE_ALL;
  __command(DISPLAY_ON);
#endif
}

#ifdef REDRAW_ENHANCEMENT
int dirty_area (int cur_x, int cur_y,
                int pen_x, int pen_y, 
                int height, int width){

   if((cur_x >= pen_x && cur_x < (pen_x + width)) &&
      (cur_y >= (pen_y/8) && cur_y < ((pen_y/8) + height))){
      return true;
   }
   else{
      return false;
   }
}
#endif

#define HALF_LCD_WIDTH (LCD_WIDTH / 2)
void write_pix(int pen_x, int pen_y, int height, int width)
{
  int xx = 0;
  int yy = 0;
  
  for(yy = 0; yy < 8; yy++)
  {
    SEL_PAGE_ALL;
    __command(__set_xaddr(yy));
    for (xx = 0; xx < LCD_WIDTH; xx++)
      {
	if(xx < HALF_LCD_WIDTH)
	  {
#ifdef REDRAW_ENHANCEMENT
            if(dirty_area(xx, yy, pen_x, pen_y, height, width))
#endif
            {
               SEL_PAGEA;
               __command(__set_yaddr(xx));
               __lcd_writedata(PAGE_A, display_ram[xx][yy]);
            }
	  }
	else
	  {
#ifdef REDRAW_ENHANCEMENT
             if(dirty_area(xx, yy, pen_x, pen_y, height, width))
#endif
             {
                SEL_PAGEB;
                __command(__set_yaddr(xx-HALF_LCD_WIDTH));
                __lcd_writedata(PAGE_B, display_ram[xx][yy]);
             }
	  }
      }
  }
}

int __ioctl_lcd_setx(int xx)
{
   if(xx < 0 ||
      xx >= PHYS_LCD_WIDTH)
      return -EINVAL;

   __command(__set_xaddr(xx));

   return 0;
}

int __ioctl_lcd_sety(int yy)
{
   if(yy < 0 ||
      yy >= PHYS_LCD_HEIGHT)
      return -EINVAL;

   __command(__set_xaddr(yy));

   return 0;
}
