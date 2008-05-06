/**
 * glcd_ioctl_common.h [Part of the TOOBO project]
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
 *  This file is shared between the glcd driver 
 *  and any app that wants to use the glcd ioctl calls
 **/

#ifndef __GLCD_IOCTL_COMMON_H__
#define __GLCD_IOCTL_COMMON_H__

/*
 *  ioctl defines
 */

#define MAGIC_NUM        'v'
#define IOCTL_LCD_CLEAR  _IO(MAGIC_NUM, 0x01)
#define IOCTL_LCD_SETX   _IO(MAGIC_NUM, 0x02)
#define IOCTL_LCD_SETY   _IO(MAGIC_NUM, 0x03)
#define IOCTL_LCD_DRAW   _IO(MAGIC_NUM, 0x04)

/*
 *  ioctl structs
 */
typedef struct _glcd_draw{
      unsigned char* img;
      int img_height;
      int img_width;
      int pen_x;
      int pen_y;
}GLCD_DRAW;

#endif /* __GLCD_IOCTL_COMMON_H__ */
