/**
 * glcd.c [Part of the TOOBO project]
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

/*
 *  Kernel Header files
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/ioctl.h>

/*
 *  Local Header files
 */
#include "glcd.h"
#include "glcd_common.h"
#include "glcd_base.h"
#include "glcd_ioctl_common.h" /* Shared between app and driver */

/*
 *  Struct defines
 */
static struct _glcdinfo{
      char iomem_obtained;
}_ginfo;

/*
 *  Defines
 */
#define DRIVER_NAME "glcd"

/*
 *  Function Prototypes
 */
static int  __init glcd_init(void);
static void __exit glcd_cleanup(void);
static int glcd_open(struct inode* inp, struct file* filp);
static int glcd_close(struct inode* inp, struct file* filp);
//static ssize_t glcd_read(struct file* filp, char* data, size_t size, loff_t* offset);
//static ssize_t glcd_write(struct file* filp, const char *ptr, size_t size, loff_t* offset);
static int glcd_ioctl(struct inode* inp, struct file* filp, unsigned int cmd, unsigned long data);

// helper functions
static int handle_ioctl( unsigned int cmd, unsigned long data);

/*
 *  Static variables
 */
static int major_num = 0;
static struct file_operations glcd_fops = {
  owner: THIS_MODULE,
  open:     glcd_open,
  release:  glcd_close,
  ioctl:    glcd_ioctl,
};

/*
 *  Kernel module specific code
 */
module_init(glcd_init);
module_exit(glcd_cleanup);
MODULE_AUTHOR("Vijay Jayaraman");
MODULE_DESCRIPTION("GLCD Driver for the TS7250 board");
MODULE_SUPPORTED_DEVICE("GLCD");
MODULE_LICENSE("LGPL");

static int __init glcd_init(void)
{
   TRACE;

   if((major_num = register_chrdev(0, DRIVER_NAME, &glcd_fops)) < 0)
   {
      ERR("register_chrdev failed (%d) for %s.", major_num, DRIVER_NAME);
      return -EIO;
   }
   else
   {
      DBG("Successfully registered char driver(%s). Major Number alloted: %d.", DRIVER_NAME, major_num);

      /* Obtain any iomem needed. */
      if(glcd_get_iomem() == 0)
      {
         _ginfo.iomem_obtained = TRUE;
         DBG("glcd_open(): Successfully obtained mem region!");

         /* initialize the lcd */
         __lcd_init();
      }
   }

   return 0;
}

static void __exit glcd_cleanup(void)
{
   TRACE;

   if(major_num > 0 &&
      unregister_chrdev(major_num, "glcd") < 0)
   {
      ERR("unregister_chrdev failed for a major num of %d.", major_num);
   }
   else
   {
      DBG("Successfully un-registered char driver(%s) with major number %d.", DRIVER_NAME, major_num);
   }

   /* Release any iomem obtained. */
   if(_ginfo.iomem_obtained == TRUE)
   {
      glcd_release_iomem();
   }

   return;
}


/*
 *  File Ops functions.
 */
static int glcd_open(struct inode* inp, struct file* filp)
{
   int retval = 0;
   /* Inc usage conunt */
   MOD_INC_USE_COUNT;

   TRACE;

   /* check for device errors */

   /* initialize the device */
 
   /* identify the minor number & update f_op pointer */
   DBG("glcd_open(): minor number is: %d.", MINOR(inp->i_rdev));

   /* allocate and fill any data structure to put in filp->private_data */

   return retval;
}

static int glcd_close(struct inode* inp, struct file* filp)
{
   MOD_DEC_USE_COUNT;
   TRACE;   
   return 0;
}

static int glcd_ioctl(struct inode* inp, struct file* filp, unsigned int cmd, unsigned long data)
{
   int retval = -1;

   TRACE;
   printk(KERN_ERR "ioctl");

   retval = handle_ioctl(cmd, data);

   return retval;
}


/*
 *  'helper' functions
 */
static int handle_ioctl(unsigned int cmd, unsigned long data)
{
   TRACE;

   printk(KERN_ERR "handle_ioctl: cmd = %du.",cmd);

   switch(cmd)
   {
      case IOCTL_LCD_CLEAR:
         __lcd_clear();
         return 0;
         break;

      case IOCTL_LCD_SETX:
            return __ioctl_lcd_setx((int)data);
         break;

      case IOCTL_LCD_SETY:
            return __ioctl_lcd_sety((int)data);
         break;

      case IOCTL_LCD_DRAW:
      {
         int img_size;
         GLCD_DRAW data_in = {0};
         unsigned char* tmp = NULL;

         // copy the data from the user space
         if(0 != copy_from_user(&data_in, (void *) data, sizeof(GLCD_DRAW)))
         {
            printk(KERN_ERR "getting data for LCD DRAW failed");
            return -EFAULT;
         }

         img_size = (data_in.img_height) * (data_in.img_width);
         tmp = kmalloc(img_size, GFP_KERNEL);

         if(tmp == NULL)
            return -ENOMEM;
         
         // copy from user space.
         if(0 != copy_from_user(tmp, data_in.img, img_size))
            return -EAGAIN;

         lcd_draw_bitmap(tmp,
                         data_in.pen_x,
                         data_in.pen_y,
                         data_in.img_height,
                         data_in.img_width,
                         0);

         kfree(tmp);
         return 0;
         break;
      }

      default:
         printk(KERN_ERR "glcd: Unexpected ioctl cmd: %d.", cmd);
         return -EINVAL;
         break;
   }
}
