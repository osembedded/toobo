/**
 * [constants_u.h] [Part of the TOOBO Project]
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
 * This file contains any 'application wide' constants or macros that 
 * needs to be used throughout the application.
 * The '_u' in the filename indicates that this file is a utility file.
 **/

#ifndef __CONSTANTS_U_H__
#define __CONSTANTS_U_H__

#ifndef TRUE
#define TRUE  (1)
#define FALSE (!TRUE)
#endif /* TRUE */

#ifndef SUCCESS
#define SUCESS (TRUE)
#define FAILURE (FALSE)
#endif /* SUCCESS */

#endif /* __CONSTANTS_U_H__ */
