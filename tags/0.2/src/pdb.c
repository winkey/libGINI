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
#include "pdb.h"
#include "get.h"
#include "error.h"

#define PREFIX_LEN 4096
/*******************************************************************************
3.4 Grid Definition for Mercator Grids

Octet 	CONTENT & MEANING
17-18 	Nx - Number of points along a latitude circle
19-20 	Ny - Number of points along a longitude meridian
21-23 	La1 - Latitude of first grid point
24-26 	Lo1 - Longitude of first grid point
27 			Resolution flag
				Bit 1 = 0 - Direction increments not given,
								1 - Direction increments given
28-30 	La2 - latitude of last grid point
31-33 	Lo2 - longitude of last grid point
34-35 	Di - the longitudinal direction increment (see note 2)
36-37 	Dj - the latitudinal direction increment (see note 2)
38 			Scanning mode
39-41 	Latin - The latitude(s) at which the Mercator projection cylinder
								intersects the earth. 

Notes:

   1. Latitude and longitude are in 10-4 (ten-thousandths)
   2. Set to 0
   3. Latitude values are limited to the range 0 - 900,000.
			Bit 1 is set to 1 to indicate south latitude.
   4. Longitude values are limited to the range 0 - 3,600,000.
			Bit one is set to 1 to indicate west longitude.
   5. The latitude and longitude of the last grid point should always be given.
*******************************************************************************/

unsigned char *parse_Mercator(unsigned char *buf, GINI_Mercator *m) {
	int temp;
	
	m->nx = GET2(buf);
	m->ny = GET2(buf);
	m->lat1 = SGET3(buf);
	m->lat1 /= 10000;
	m->lon1 = SGET3(buf);
	m->lon1 /= 10000;
	m->resolution = GET1(buf);
	m->lat2 = SGET3(buf);
	m->lat2 /= 10000;
	m->lon2 = SGET3(buf);
	m->lon2 /= 10000;
	m->Dx = GET2(buf);
	m->Dy = GET2(buf);
	
	temp = GET1(buf);
	m->scanningmode.xdir = temp & 0x01;
	m->scanningmode.ydir = temp & 0x02;
	m->scanningmode.swapxy = temp & 0x04;
	
	m->latin = SGET3(buf);
	m->latin /= 10000;
	
	return buf;
}

/*******************************************************************************
3.1 Projection Definition for Polar Stereographic and Lambert Conformal Images

Octet 	CONTENT & MEANING
17-18 	Nx - Number of points along x-axis
19-20 	Ny - Number of points along y-axis
21-23 	La1 - Latitude of first grid point
24-26 	Lo1 - Longitude of first grid point
27			Reserved - currently 0
28-30 	Lov - The orientation of the grid; i.e., the east longitude value of the meridian which is parallel to the y-axis (or columns of the grid) along which latitude increases as the y-coordinate increases. (Note: The orientation longitude may, or may not, appear within a particular grid.)
31-33 	Dx - the X-direction increment (see note 2)
34-36 	Dy - the Y-direction increment (see note 2)
37			Projection center flag (see note 5)
38			Scanning mode
39-41 	Latin - The latitude at which the Lambert Conformal projection is tangent to the earth.  Set to 0 for stereographic projections.

Notes:

   1. Latitude and longitude are in 10-4 (ten-thousandths)
   2. Increments are in units of tenths of meters, at the tangent latitude
			(60 degree for stereographic) circle nearest to the pole in the projection
			plane.
   3. Latitude values are limited to the range 0 - 900,000.
			Bit 1 is set to 1 to indicate south latitude.
   4. Longitude values are limited to the range 0 - 3,600,000.
			Bit one is set to 1 to indicate west longitude.
   5. Projection Center Flag -

      Bit 1 set to 0 if the North pole is on the projection plane.
      Bit 1 set to 1 if the South pole is on the projection plane.
*******************************************************************************/

unsigned char *parse_Polar(unsigned char *buf, GINI_Polar *p) {
	int temp;
	
	p->nx = GET2(buf);
	p->ny = GET2(buf);
	p->lat1 = SGET3(buf);
	p->lat1 /= 10000;
	p->lon1 = SGET3(buf);
	p->lon1 /= 10000;
	p->Reserved = GET1(buf);
	p->loV = SGET3(buf);
	p->loV /= 10000;
	p->Dx = GET3(buf);
	p->Dx /= 10;
	p->Dy = GET3(buf);
	p->Dy /= 10;
	p->centerflag = GET1(buf);
	
	temp = GET1(buf);
	p->scanningmode.xdir = temp & 0x01;
	p->scanningmode.ydir = temp & 0x02;
	p->scanningmode.swapxy = temp & 0x04;
	
	p->tanlat = SGET3(buf);
	p->tanlat /= 10000;
	
	return buf;
}

unsigned char *parse_Lambert(unsigned char *buf, GINI_Lambert *l) {
	
	return parse_Polar(buf, l);
}

/*******************************************************************************
1. Product Definition Block
The product definition block is a 512 byte block which describes the image data.
 
Octet/Byte 	Description
01					Source - 1=NESDIS
02					Creating entity
03					Sector identifier
04					Physical element
05-06				Number of logical records (scan lines)
07-08				Size of logical record (pixels per scan line)
09					Valid Time - Year of century
10					Month
11					Day
12					Hour
13					Minute
14					Seconds 
15					Hundredths of seconds
16					Map projection indicator
17-41				Projection Definition (see below)
42					Image resolution
43					Data compression indicator (1=compressed, 0=no compression)
44					Version of PDB
45-46				Size of PDB in bytes
47					Navigation/Calibration indicator (0=nav/cal not included,
																							1=nav/cal included, 2=nav only,
																							3=cal only)
48-512			Reserved
*******************************************************************************/

unsigned char *parse_pdb (unsigned char *buf, GINI_pdb *p) {
		
	p->Source = GET1(buf);
	p->Creating_entity = GET1(buf);
	p->Sector_identifier = GET1(buf);
	p->Physical_element = GET1(buf);
	p->num_lines = GET2(buf);
	p->pixels_per_line = GET2(buf);
	
	p->ctime.tm_year = GET1(buf);
	p->ctime.tm_mon = GET1(buf) - 1;
	p->ctime.tm_mday = GET1(buf);
	p->ctime.tm_sec = GET1(buf);
	p->ctime.tm_min = GET1(buf);
	p->ctime.tm_hour = GET1(buf);
	
	mktime(&(p->ctime));
	p->Hundredths_of_seconds = GET1(buf);
	
	p->proj = GET1(buf);
	
	switch (p->proj) {
		case MERCATOR:
			buf = parse_Mercator(buf, &(p->mercator));
			break;
		
		case LAMBERT:
			buf = parse_Lambert(buf, &(p->lambert));
			break;
		
		case POLAR:
			buf = parse_Polar(buf, &(p->polar));
			break;
		
		default:
			ERROR("unknown projection");
			break;
	}
	
	p->resolution = GET1(buf);
	p->compression = GET1(buf);
	p->PDB_version = GET1(buf);
	p->PDB_size = GET2(buf);
	p->nav_cal = GET1(buf);
	//memcpy (p->Reserved, buf, 465); don't do this yet
	buf += 465;
	
	return buf;
}

void print_scanningmode(GINI_Scanning_mode *s,char *prefix) {
	switch (s->xdir) {
		case POSITIVE:
			printf("%s.scanningmode.xdir = POSITIVE\n", prefix);
			break;
		
		case NEGATIVE:
			printf("%s.scanningmode.xdir = NEGATIVE\n", prefix);
			break;
	}
	switch (s->ydir) {
		case POSITIVE:
			printf("%s.scanningmode.ydir = POSITIVE\n", prefix);
			break;
		
		case NEGATIVE:
			printf("%s.scanningmode.ydir = NEGATIVE\n", prefix);
			break;
	}
		
	switch (s->ydir) {
		case POSITIVE:
			printf("%s.scanningmode.ydir = POSITIVE\n", prefix);
			break;
		
		case NEGATIVE:
			printf("%s.scanningmode.ydir = NEGATIVE\n", prefix);
			break;
	}
		
	switch (s->swapxy) {
		case NOSWAP:
			printf("%s.scanningmode.swapxy = NOSWAP\n", prefix);
			break;
		
		case SWAP:
			printf("%s.scanningmode.swapxy = SWAP\n", prefix);
			break;
	}
	return;
}

void print_Mercator(GINI_Mercator *m, char *prefix) {
	char myprefix[PREFIX_LEN];
	
	snprintf(myprefix, PREFIX_LEN, "%s.mercator", prefix);
	
	printf("%s.nx = %i\n", myprefix, m->nx);
	printf("%s.ny = %i\n", myprefix, m->ny);
	printf("%s.lat1 = %g\n", myprefix, m->lat1);
	printf("%s.lon1 = %g\n", myprefix, m->lon1);
	printf("%s.resolution = %i\n", myprefix, m->resolution);
	printf("%s.lat2 = %g\n", myprefix, m->lat2);
	printf("%s.lon2 = %g\n", myprefix, m->lon2);
	printf("%s.Dx = %g\n", myprefix, m->Dx);
	printf("%s.Dy = %g\n", myprefix, m->Dy);
	
	print_scanningmode (&(m->scanningmode), myprefix);
	
	printf("%s.latin = %g\n", myprefix, m->latin);
	
	return;
}

void print_Polar(GINI_Polar *p, char *prefix) {
	char myprefix[PREFIX_LEN];
	
	snprintf(myprefix, PREFIX_LEN, "%s.polar", prefix);
	
	printf("%s.nx = %i\n", myprefix, p->nx);
	printf("%s.ny = %i\n", myprefix, p->ny);
	printf("%s.lat1 = %g\n", myprefix, p->lat1);
	printf("%s.lon1 = %g\n", myprefix, p->lon1);
	printf("%s.Reserved = %i\n", myprefix, p->Reserved);
	printf("%s.loV = %g\n", myprefix, p->loV);
	printf("%s.Dx = %g\n", myprefix, p->Dx);
	printf("%s.Dy = %g\n", myprefix, p->Dy);
	printf("%s.centerflag = %i\n", myprefix, p->centerflag);
	
	print_scanningmode (&(p->scanningmode), myprefix);
	
	printf("%s.tanlat = %g\n", myprefix, p->tanlat);
	
	return;
}

void print_Lambert(GINI_Lambert *l, char *prefix) {
	char myprefix[PREFIX_LEN];
	
	snprintf(myprefix, PREFIX_LEN, "%s.lambert", prefix);
	
	printf("%s.nx = %i\n", myprefix, l->nx);
	printf("%s.ny = %i\n", myprefix, l->ny);
	printf("%s.lat1 = %g\n", myprefix, l->lat1);
	printf("%s.lon1 = %g\n", myprefix, l->lon1);
	printf("%s.Reserved = %i\n", myprefix, l->Reserved);
	printf("%s.loV = %g\n", myprefix, l->loV);
	printf("%s.Dx = %g\n", myprefix, l->Dx);
	printf("%s.Dy = %g\n", myprefix, l->Dy);
	printf("%s.centerflag = %i\n", myprefix, l->centerflag);
	
	print_scanningmode (&(l->scanningmode), myprefix);
	
	printf("%s.tanlat = %g\n", myprefix, l->tanlat);
}


	
void print_pdb(GINI_pdb *p, char *prefix) {
	char myprefix[PREFIX_LEN];
	
	snprintf(myprefix, PREFIX_LEN, "%s.pdb", prefix);
		

	printf("%s.Source = %s\n", myprefix, Source_string[p->Source]);
	printf("%s.Creating_entity = %s\n", myprefix,
				 Creating_entity_string[p->Creating_entity]);
	printf("%s.Sector_identifier = %s\n", myprefix,
				 Sector_identifier_string[p->Sector_identifier]);
	printf("%s.Physical_element = %s\n", myprefix,
				 Physical_element_string[p->Physical_element]);
	printf("%s.num_lines = %i\n", myprefix, p->num_lines);
	printf("%s.pixels_per_line = %i\n", myprefix, p->pixels_per_line);
	printf("%s.ctime = %s\n", myprefix, asctime(&p->ctime));
	printf("%s.Hundredths_of_seconds = %i\n", myprefix, p->Hundredths_of_seconds);
	
	printf("%s.proj = %s\n", myprefix, proj_string[p->proj]);
	
	
	switch (p->proj) {
		case MERCATOR:
			print_Mercator(&(p->mercator), myprefix);
			break;
		
		case LAMBERT:
			print_Lambert(&(p->lambert), myprefix);
			break;
		
		case POLAR:
			print_Polar(&(p->polar), myprefix);
			break;
		
		default:
			ERROR("unknown projection");
			break;
	}
	
	printf("%s.resolution = %u\n", prefix, p->resolution);
	printf("%s.compression = %u\n", prefix, p->compression);
	printf("%s.PDB_version = %i\n", prefix, p->PDB_version);
	printf("%s.PDB_size = %u\n", prefix, p->PDB_size);
	printf("%s.nav_cal = %u\n", prefix, p->nav_cal);
	
	return;
	
}
