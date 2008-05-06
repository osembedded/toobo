/**
 * display-test.c [Part of the TOOBO Project]
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

#ifdef TEST_LIFE_BOX
#include <stdlib.h> /* random number gen */
#endif /* TEST_LIFE_BOX */

#include <sys/ioctl.h>
#include <sys/types.h>

#include "glcd_ioctl_common.h"
#include "display-test.h"
#include "toobo_base.h"

#define PATH2DEV "/dev/glcd0"

struct disp{
      int handle;
};

static struct disp display;
static GLCD_DRAW   draw_data = {(unsigned char *)&header_data[0], 64, 128,   0,   0};

/* 
 *  Function Prototypes
 */
int open_display(void);
int close_display(void);
int write_display(char* data, int len);
int ioctl_cmd (int cmd, void* argp);

/*
 *  Misc Function Prototypes
 */
int get_random_number(unsigned int seed);
int gen_random_pix(unsigned int seed, int *pen_x, int *pen_y);
void spin_life_box (int times);

int main (void)
{
   int retval = FAILURE;
   TRACE_IN;

   /* open the display device */
   if((retval = open_display()) == SUCCESS)
   {
      /* clear the display */
      if(ioctl_cmd(IOCTL_LCD_CLEAR, 0) != SUCCESS)
      {
         DBG("\n%s(): Could not clear LCD display!", __FUNCTION__);         
      }

      /* draw the image */
      if(ioctl_cmd(IOCTL_LCD_DRAW, &draw_data) != SUCCESS)
      {
         DBG("\n%s(): Could not draw on display!", __FUNCTION__);         
      }

#ifdef TEST_LIFE_BOX
      DBG("\n starting to spin lifebox");
      /* just for fun... */
      spin_life_box(400);
#endif /* TEST_LIFE_BOX */

      /* close it */
      if((retval = close_display()) != SUCCESS)
      {
         DBG("\n%s(): Could not Close display!", __FUNCTION__);
      }
   }
   else
   {
      DBG("\n%s(): Could not Open display!", __FUNCTION__);
   }

   TRACE_OUT;
   return retval;
}

int open_display(void)
{
   int retval = FAILURE;
   int file   = 0;
   TRACE_IN;
   
   if((file = open(PATH2DEV, O_RDWR)) != -1)
   {
      retval = SUCCESS;
      display.handle = file;
      DBG("\n%s(): open SUCCESS! display.handle = %d",__FUNCTION__, display.handle);
   }
   else
   {
      int err = errno;
      DBG("\nCould not open %s: %s!", PATH2DEV, strerror(err));
   }

   TRACE_OUT;
   return retval;
}


int close_display(void)
{
   int retval = FAILURE;
   TRACE_IN;

   if(display.handle >= 0)
   {
      if((retval = close(display.handle) != -1))
         {
            retval = SUCCESS;
            display.handle = 0;
            DBG("\n%s(): close SUCCESS!", __FUNCTION__);
         }
      else
      {
         int err = errno;
         DBG("\nCould not close %s: %s!", PATH2DEV, strerror(err));
      }
   }

   TRACE_OUT;
   return retval;
}

int write_display(char* data, int len)
{
   int retval = FAILURE;
   int bytes_written = 0;
   TRACE_IN;

   if(display.handle >= 0)
   {
      /* Not taking care of interrupted writes at this point... */
      if((bytes_written = write(display.handle, (const void *) data, len)) > 0)
      {
         retval = SUCCESS;
      }

      DBG("\nbytes_written = %d.", bytes_written);
   }

   TRACE_OUT;
   return retval;
}

int ioctl_cmd (int cmd, void* argp)
{
   int retval = FAILURE;

   // can't check for argp to be null here as 
   // it can actually be NULL.

   if(-1 != ioctl(display.handle, cmd, argp))
   {
      retval = SUCCESS;
   }
   else
   {
      int err = errno;
      printf("\n ioctl error: %d.", err, strerror(err));
   }

   return retval;
}


/**
   What is a LIFE_BOX?
   Well, it is just a fancy name for a recatangle where 
   a single pixel keeps moving randomly inside it. Kind 
   of looks cool. Maybe I can use something similar to 
   this in the future to tell if all processes are running 
   correctly without interruption. A software watchdog 
   could be changing the pixel when things are running fine.

   For now, it is just a fancy little thing I made up.
**/
#ifdef TEST_LIFE_BOX
int get_random_number(unsigned int seed)
{
   int retval = 0;

   srandom(seed);
   return ((int) random());
}

#define LIFE_BOX_MAXX (124)
#define LIFE_BOX_MAXY (62)
#define LIFE_BOX_WIDTH (8)
#define LIFE_BOX_HEIGHT (4)
#define LIFE_BOX_MINX (LIFE_BOX_MAXX - LIFE_BOX_WIDTH)
#define LIFE_BOX_MINY (LIFE_BOX_MAXY - LIFE_BOX_HEIGHT)
int gen_random_pix(unsigned int seed, int *pen_x, int *pen_y)
{
   int xx = 0;
   int yy = 0;

   if (pen_x == NULL ||
       pen_y == NULL)
      return -1;

   xx = get_random_number(seed);
   xx %= LIFE_BOX_WIDTH;
   *pen_x = LIFE_BOX_MINX + xx;

   yy = get_random_number(seed);
   yy %= LIFE_BOX_HEIGHT;
   *pen_y = LIFE_BOX_MINY + yy;

   return 0;
}

/* assumes that display open is success just for testing... */
void spin_life_box (int times)
{
   static int seed = 12092;
   static int prev_xx = 0;
   static int prev_yy = 0;
   static char first_time = 1;
   int ii     = 0;
   int pen_xx = 0;
   int pen_yy = 0;
   static unsigned char lifebox_live_pix = 0;
   static unsigned char lifebox_dead_pix = 1;
   static GLCD_DRAW lifebox_data = {
      &lifebox_live_pix,
      1,
      1,
      0,
      0};   

   for(ii = 0; ii < times; ii++)
   {
      prev_xx = pen_xx;
      prev_yy = pen_yy;

      if(0 == gen_random_pix(seed++, &pen_xx, &pen_yy))
      {
         if(prev_xx == pen_xx &&
            prev_yy == pen_yy)
            continue;

         if(!first_time)
         {
            /* fill the previous blank life pixel */
            lifebox_data.img = &lifebox_dead_pix;
            lifebox_data.pen_x = prev_xx;
            lifebox_data.pen_y = prev_yy;
            if(ioctl_cmd(IOCTL_LCD_DRAW, &lifebox_data) != SUCCESS)
            {
               DBG("\n%s(): Could not draw on display!", __FUNCTION__);         
            }
         }
         else
            first_time = 0;

         /* draw the blank life pixel */
         lifebox_data.img = &lifebox_live_pix;
         lifebox_data.pen_x = pen_xx;
         lifebox_data.pen_y = pen_yy;
         if(ioctl_cmd(IOCTL_LCD_DRAW, &lifebox_data) != SUCCESS)
         {
            DBG("\n%s(): Could not draw on display!", __FUNCTION__);         
         }
      }
      else
      {
         DBG("\n%s(): Could not get random pix!", __FUNCTION__);
      }

      /* bad bad... but gotto test this cool thing... */
      sleep(1);
   }

}

#endif /* TEST_LIFE_BOX */
