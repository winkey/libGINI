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
 
#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ERROR(x) {\
  fprintf(stderr, "ERROR:\n      file: %s\n      line: %d\n      function:  %s\n      %s\n",\
	        __FILE__, __LINE__, (x), strerror(errno));\
	exit(EXIT_FAILURE);\
}

#endif /* _ERROR_H */
