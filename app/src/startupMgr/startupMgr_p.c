/**
 * [startupMgr_p.c] [Part of the TOOBO Project]
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
 * Purpose:
 * This file contains code for the startupMgr process.
 * The '_p' in the filename indicates this. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "startupMgr.h"

static ENVS envs;

void load_envs(void){
   envs.model_name = getenv("MODEL_NAME");
   envs.model_version = getenv("MODEL_VERSION");
}


int main (int argc, char** argv){

   printf(">]] StartupMgr: Rolling... \n");

   load_envs();
   if(envs.model_version != NULL &&
      envs.model_name != NULL){
      printf(">]] Toobo version %s - %s \n",
             envs.model_version,
             envs.model_name);
   }

   while(1){

      sleep(10);
   }

   exit(0);
}



