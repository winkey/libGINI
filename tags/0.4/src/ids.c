/*
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

#include "libGINI.h"
#include "get.h"
#include "error.h"

unsigned char *parse_ids(unsigned char *buf, GINI_ids *ids, size_t bytes) {

	int i;
	GINI_ids d;
	
	if (!(*ids = malloc(bytes + 1)))
		ERROR("parse_ids");
	
	d = *ids;
	for (i = 0; i < bytes ; i++) {
		d[i] = GET1(buf);
	}
	
	return buf;
}

void print_ids(GINI_ids *ids, size_t bytes) {
	int i, j;
	GINI_ids d = *ids;
	
	for (i = 0 ; i < bytes ; i++) {
		for ( j = 0 ; j < 30 ; j++) {
			printf("%2x ", d[i]);
		}
		printf("\n");
	}
	
	return;
}

	
	
	
