/**
 * glcd_base.h
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
#ifndef __GLCD_BASE_H__
#define __GLCD_BASE_H__

int glcd_get_iomem(void);
int glcd_release_iomem(void);
void __lcd_init(void);
void __lcd_clear(void);

int __ioctl_lcd_setx(int xx);
int __ioctl_lcd_sety(int yy);

void lcd_draw_bitmap(unsigned char *bitmap, 
		     int pen_x, 
		     int pen_y, 
		     int height, 
		     int width,
		     unsigned char draw_bottom_first);

#endif /* __GLCD_BASE_H__ */
