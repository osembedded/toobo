/**
 * [types_u.h] [Part of the TOOBO Project]
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
 * This file must be included to defined the types we want to use in the 
 * application. This should help us be architecture independent in the future.
 * The '_u' in the filename indicates that this file is a utility file.
 **/

#ifndef __TYPES_U_H__
#define __TYPES_U_H__

typedef int              INT32;
typedef unsigned int     UINT32;
typedef char             CHAR;
typedef unsigned char    UCHAR;
typedef long             LONG;
typedef unsigned long    ULONG;
typedef short            SHORT;
typedef unsigned short   USHORT;
typedef void             VOID;
typedef boolean          BOOL;

#endif /* __TYPES_U_H__ */
