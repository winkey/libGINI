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
 
#ifndef _LIBGIDS_H
#define _LIBGIDS_H

#include <time.h>
#include <stdio.h>

typedef enum {
	POSITIVE,
	NEGATIVE
} GINI_xdir_enum;

typedef GINI_xdir_enum GINI_ydir_enum;

typedef enum {
	NOSWAP,
	SWAP
} GINI_swapxy_enum;

typedef struct {
	GINI_xdir_enum xdir;
	GINI_ydir_enum ydir;
	GINI_swapxy_enum swapxy;
} GINI_Scanning_mode;
	

typedef struct {
	int nx;
	int ny;
	double lat1;
	double lon1;
	int Reserved;
	double loV;
	double Dx;
	double Dy;
	int centerflag;
	GINI_Scanning_mode scanningmode;
	float tanlat;
} GINI_Polar;

typedef GINI_Polar GINI_Lambert;

typedef struct {
	int nx;
	int ny;
	double lat1;
	double lon1;
	int resolution;
	double lat2;
	double lon2;
	double Dx;
	double Dy;
	GINI_Scanning_mode scanningmode;
	double latin;
} GINI_Mercator;

typedef enum {
	GCOMPRESSION_NONE,
	GCOMPRESSION
} GINI_compression_enum;

typedef enum {
	NAV_CAL_NONE,
	NAV_CAL,
	NAV,
	CAL,
} GINI_nav_cal_enum;

typedef enum {
	MERCATOR = 1,
	LAMBERT = 3,
	POLAR = 5,
} GINI_Proj_enum;


typedef struct {
	int Source;
	int Creating_entity;
	int Sector_identifier;
	int Physical_element;
	int num_lines;
	int pixels_per_line;
	struct tm ctime;
	int Hundredths_of_seconds;
	int proj;
	union {
		GINI_Polar polar;
		GINI_Lambert lambert;
		GINI_Mercator mercator;
	};
	int resolution;
	GINI_compression_enum compression; //(1=compressed, 0=no compression)
	int PDB_version;
	size_t PDB_size;
	GINI_nav_cal_enum nav_cal;
	char Reserved[465];
} GINI_pdb;

typedef char * GINI_ids;

typedef struct {
	char nws[50];
	GINI_pdb pdb;
	GINI_ids ids;
} GINI;

 
FILE *GINI_open(char *filename);

void GINI_close(FILE *fp);

void GINI_read (FILE *fp, GINI *data);

void GINI_free(GINI *data);

void GINI_print(GINI *data, char *prefix);


#endif /* _LIBGIDS_H */

