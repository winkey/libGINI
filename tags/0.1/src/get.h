/*
 * libNIDS
 * Copyright (C) Brian Case 2008 <rush@gisweather.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#ifndef _GET_H
#define _GET_H


/*******************************************************************************
	macro to get a 1 byte big endian int from a buffer
*******************************************************************************/

#define GET1(x) (\
	(unsigned char)(x++)[0]\
)

/*******************************************************************************
	macro to get a 2 byte big endian int from a buffer
*******************************************************************************/

#define GET2(x) (\
	(unsigned char)(x++)[0] * 0x100 +\
	(unsigned char)(x++)[1]\
)

/*******************************************************************************
	macro to get a 3 byte big endian int from a buffer
*******************************************************************************/

#define GET3(x) (\
	(unsigned char)(x++)[0] * 0x10000 + \
	(unsigned char)(x++)[1] * 0x100 +\
	(unsigned char)(x++)[2]\
)
									 
/*******************************************************************************
	macro to get a 4 byte big endian int from a buffer
*******************************************************************************/

#define GET4(x) (\
	(unsigned char)(x++)[0] * 0x1000000 +\
	(unsigned char)(x++)[1] * 0x10000 + \
	(unsigned char)(x++)[2] * 0x100 +\
	(unsigned char)(x++)[3]\
)

/*******************************************************************************
	macro to get a signed 1 byte big endian int from a buffer
*******************************************************************************/

#define SGET1(x) (\
	((unsigned char)(x)[0] & 0x80 ? -1 : 1) * \
	((unsigned char)(x++)[0]) ^ \
	((unsigned char)(x)[0] & 0x80 ? 0x8000 : 0)\
)

/*******************************************************************************
	macro to get a signed 2 byte big endian int from a buffer
*******************************************************************************/

#define SGET2(x) (\
	((unsigned char)(x)[0] & 0x80 ? -1 : 1) * \
	((unsigned char)(x++)[0] * 0x100 +\
	 (unsigned char)(x++)[1]) ^ \
	((unsigned char)(x)[0] & 0x80 ? 0x8000 : 0)\
)

/*******************************************************************************
	macro to get a signed 3 byte big endian int from a buffer
*******************************************************************************/

#define SGET3(x) (\
	((unsigned char)(x)[0] & 0x80 ? -1 : 1) * \
	((unsigned char)(x++)[0] * 0x10000 +\
	 (unsigned char)(x++)[1] * 0x100 +\
	 (unsigned char)(x++)[2]) ^ \
	((unsigned char)(x)[0] & 0x80 ? 0x800000 : 0)\
)

									 
/*******************************************************************************
	macro to get a signed 4 byte big endian int from a buffer
*******************************************************************************/

#define SGET4(x) (\
	((unsigned char)(x)[0] & 0x80 ? -1 : 1) * \
	((unsigned char)(x++)[0] * 0x1000000 +\
	 (unsigned char)(x++)[1] * 0x10000 +\
	 (unsigned char)(x++)[2] * 0x100 +\
	 (unsigned char)(x++)[3]) ^ \
	((unsigned char)(x)[0] & 0x80 ? 0x80000000 : 0)\
)


#endif /* _GET_H */

 
