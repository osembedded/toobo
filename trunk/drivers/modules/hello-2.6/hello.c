/**
 * [hello.c] [Part of the TOOBO Project]
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

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>

static int __init init_hello(void);
static void __exit cleanup_hello(void);

MODULE_AUTHOR("Vijay Jayaraman");
MODULE_DESCRIPTION("Helloworld test driver.");
MODULE_SUPPORTED_DEVICE("hello");
MODULE_LICENSE("LGPL");
module_init(init_hello);
module_exit(cleanup_hello);


#define TRACE_IN printk(KERN_INFO "%s():(+)\n", __FUNCTION__);
#define TRACE_OUT printk(KERN_INFO "%s():(-)\n", __FUNCTION__);
#define DBG(fmt...) printk(KERN_INFO fmt)

static dev_t device_numbers;
#define FIRST_MINOR 0
#define NUM_DEVICES 1

static int __init init_hello(void) {
   TRACE_IN;

   /* Proceed with registering the char device */
   if(0 == alloc_chrdev_region(&device_numbers, FIRST_MINOR, NUM_DEVICES, "Hello")){
      DBG("Registered a Character device successfully!\n");
   }

   return 0;
}

static void __exit cleanup_hello(void) {
   TRACE_IN;

   /* Proceed with de-registering the char device */
   unregister_chrdev_region(device_numbers, NUM_DEVICES);

}

