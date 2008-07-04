/**
 * [htest.c] [Part of the TOOBO Project]
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
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define SUCCESS 0
#define FAILURE (!SUCCESS)
#define PATH2DEV "/dev/hello0"
#define TRACE_IN printf("%s(): %d (+)\n", __FUNCTION__, __LINE__);
#define TRACE_OUT printf("%s(): %d (-)\n", __FUNCTION__, __LINE__);
#define DBG(fmt...) printf(fmt)

int fd = 0;

int open_hello(void);
int close_hello(void);

int main (void){
   int retval = FAILURE;
   TRACE_IN;

   if(SUCCESS == open_hello()){
      DBG("Successfully opened /dev/hello0!\n");
      if(SUCCESS == close_hello()){
         DBG("Successfully closed /dev/hello0!\n");

         retval = SUCCESS;
      }
   }

   if(SUCCESS == retval){
         DBG("Test Successful!\n");
   }
   else{
         DBG("Test Failure!!!\n");      
   }

   TRACE_OUT;
   return retval;
}

int open_hello(void){
   int retval = FAILURE;
   int err = 0;

   TRACE_IN;
   if(-1 != (fd = open(PATH2DEV, O_RDWR))){
      DBG("fd returned is %d.\n", fd);
      retval = SUCCESS;
   }
   else{
      err = errno;
      DBG("file opening error: %s (%d)\n", strerror(err), err);
   }

   TRACE_OUT;
   return retval;
}

int close_hello(void){
   int retval = FAILURE;
   int err = 0;

   TRACE_IN;
   if(-1 != close(fd)){
      DBG("fd closed successfully!\n");
	  retval = SUCCESS;
   }
   else{
      err = errno;
      DBG("file close error: %s (%d)\n", strerror(err), err);
   }

   TRACE_OUT;
   return retval;
}

