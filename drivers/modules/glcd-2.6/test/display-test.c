#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> /* random number gen */

#include <sys/ioctl.h>
#include <sys/types.h>

#include "glcd_ioctl_common.h"
#include "display-test.h"
#include "toobo_base2.h"
//#include "toobo_base.h"

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
void read_display(void); /* Dummy call */
int ioctl_cmd (int cmd, void* argp);

/*
 *  Misc Function Prototypes
 */
int get_random_number(unsigned int seed);
int gen_random_pix(unsigned int seed, int *pen_x, int *pen_y);
void spin_life_box (int times);
void draw_rect(int posx,
               int posy,
               int width,
               int height);

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

//      draw_rect(128-32, 32, 32, 32);
      spin_life_box(400);

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
      DBG("\nCould not open %s: %s (%d)!", PATH2DEV, strerror(err), err);
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

void read_display(void)
{
   int  bytes_read = 0;
   char dummy[20];
   int ii = 0;
   /* Dummy call */
   TRACE_IN;

   if(-1 != (bytes_read = read(display.handle, &dummy[0], 4)))
   {
      printf("\n Read Success! bytes_read = %d.", bytes_read);
      for(ii = 0; ii< bytes_read; ii++)
      {
         printf("\n ii: %d - 0x%x. (%c)", ii, dummy[ii], dummy[ii]);
      }
   }
   else
   {
      printf("\n Read Failure! bytes_read = %d.", bytes_read);
   }

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

void draw_rect(int posx,
               int posy,
               int width,
               int height){
   GLCD_DRAW rdata;

   rdata.img = NULL;
   rdata.img_height = height;
   rdata.img_width = width;
   rdata.pen_x = posx;
   rdata.pen_y = posy;

   DBG("\n%s(): Drawing square with params: x:%d, y:%d, width:%d, height:%d.",
       __FUNCTION__,
       posx,
       posy,
       width,
       height);

   if(NULL != (rdata.img = malloc(width * height * sizeof(unsigned char))))
   {
      memset(rdata.img, 1, width * height * sizeof(unsigned char));

      {
         int ii = 0;
         int count = 0;
         for(ii=0; ii<width*height; ii++){
            if(rdata.img[ii] == 1){
               count++;
            }
         }
         DBG("\nNumber of bits ON is : %d.", count);
      }
      
      if(ioctl_cmd(IOCTL_LCD_DRAW, &rdata) != SUCCESS)
      {
         DBG("\n%s(): Could not draw on display!", __FUNCTION__);         
      }

      // free the memory
      free(rdata.img);
   }
   else{
      DBG("\n%s(): Could not malloc!", __FUNCTION__);         
   }
}
