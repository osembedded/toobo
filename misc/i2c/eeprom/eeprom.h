/**
 * File: eeprom.h
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

#ifndef __EEPROM_H__
#define __EEPROM_H__

typedef enum {
  EE_PAGE1 = 0x50,
  EE_PAGE2,
  EE_PAGE3,
  EE_PAGE4,
  EE_PAGE5,
  EE_PAGE6,
  EE_PAGE7,
  EE_PAGE8
}EE_PAGE;


void clear_24lc16b_eeprom(int file);

unsigned int read_eeprom(int file, 
			 int dev_addr, 
			 unsigned char addr, /* only b/w 0-15 */
			 unsigned char *buf,
			 int num_bytes);

unsigned char write_eeprom(int file, 
			   int dev_addr, 
			   unsigned char addr, /* only b/w 0-15 */
			   unsigned char *buf,
			   int num_bytes);

#endif /* __EEPROM_H__ */
