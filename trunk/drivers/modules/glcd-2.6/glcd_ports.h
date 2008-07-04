/**
 * glcd_ports.h
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

#ifndef __GLCD_PORTS_H__
#define __GLCD_PORTS_H__

#define GPIOBASE	0x80840000

#define PHYS_ADDR_PADR  0x80840000
#define PHYS_ADDR_PBDR  0x80840004
#define PHYS_ADDR_PHDR  0x80840040
#define PHYS_ADDR_PADDR 0x80840010
#define PHYS_ADDR_PBDDR 0x80840014
#define PHYS_ADDR_PHDDR 0x80840044


#define PADR	0
#define PADDR	(0x10 / sizeof(unsigned int))
#define PAMASK  0x7F  

// lcd chip selector (page selector) 2 bit
#define PBDR    (0x04 / sizeof(unsigned int))
#define PBDDR   (0x14 / sizeof(unsigned int))

// lcd control registers
#define PHDR	(0x40 / sizeof(unsigned int))
#define PHDDR	(0x44 / sizeof(unsigned int))


#endif /* __GLCD_PORTS_H__ */
