/**
 * [display-test.h] [Part of the TOOBO Project]
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

#ifndef __DISPLAY_TEST_H__
#define __DISPLAY_TEST_H__

#define DBG(x...) printf(x)
#define TRACE_IN  printf("\n%s(+):", __FUNCTION__)
#define TRACE_OUT /* printf("\n%s(-):", __FUNCTION__)*/

#ifndef SUCCESS
#define SUCCESS 0
#define FAILURE -1
#endif

#endif /* __DISPLAY_TEST_H__ */
