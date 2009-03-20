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
 
#ifndef _PDB_H
#define _PDB_H

char *Source_string[] = {
	"NONE",
	"NESDIS",
	""
};

char *Creating_entity_string[] = {
	"NONE",
	"NONE",
	"Miscellaneous",
	"JERS",
	"ERS/QuikSCAT/Scatterometer",
	"POES/NPOESS",
	"Composite",
	"DMSP",
	"GMS",
	"METEOSAT",
	"GOES-7 (H) Reserved for future use.",
	"GOES-8 (I)",
	"GOES-9 (J)",
	"GOES-10 (K)",
	"GOES-11 (L)",
	"GOES-12 (M)",
	"GOES-13 (N)",
	"GOES-14 (O)",
	"GOES-15 (P)",
	"GOES-16 (Q)",
	"",
};

char *Sector_identifier_string[] = {
"Northern Hemisphere Composite",
"East CONUS",
"West CONUS",
"Alaska Regional",
"Alaska National",
"Hawaii Regional",
"Hawaii National",
"Puerto Rico Regional",
"Puerto Rico National",
"Supernational",
"NH Composite - Meteosat/GOES E/ GOES W/GMS",
"Central CONUS",
"East Floater",
"West Floater",
"Central Floater",
"Polar Floater",
"",
};


char *Physical_element_string[] = {
	"NONE",
	"Imager Visible",
	"Imager 3.9 micron IR",
	"Imager 6.7/6.5 micron IR (WV)",
	"Imager 11 micron IR",
	"Imager 12 micron IR",
	"Imager 13 micron (IR)",
	"Imager 1.3 micron (IR)",
	"Reserved for future use",
	"Reserved for future use",
	"Reserved for future use",
	"Reserved for future use",
	"Reserved for future use",
	"Imager Based Derived Lifted Index (LI)",
	"Imager Based Derived Precipitable Water (PW)",
	"Imager Based Derived Surface Skin Temp (SFC Skin)",
	"Sounder Based Derived Lifted Index (LI)",
	"Sounder Based Derived Precipitable Water (PW)",
	"Sounder Based Derived Surface Skin Temp (SFC Skin)",
	"Derived Convective Available Potential Energy (CAPE)",
	"Derived land-sea temp",
	"Derived Wind Index(WINDEX)",
	"Derived Dry Microburst Potential Index (DMPI)",
	"Derived Microburst Day Potential Index (MDPI)",
	"Derived Convective Inhibition",
	"Derived Volcano Imagery",
	"Scatterometer Data",
	"Gridded Cloud Top Pressure or Height",
	"Gridded Cloud Amount",
	"Rain fall rate",
	"Surface wind speeds over oceans and Great Lakes",
	"Surface wetness",
	"Ice concentrations",
	"Ice type",
	"Ice edge",
	"Cloud water content",
	"Surface type",
	"Snow indicator",
	"Snow/water content",
	"Derived volcano imagery",
	"Reserved for future use",
	"Sounder 14.71 micron imagery",
	"Sounder 14.37 micron imagery",
	"Sounder 14.06 micron imagery",
	"Sounder 13.64 micron imagery",
	"Sounder 13.37 micron imagery",
	"Sounder 12.66 micron imagery",
	"Sounder 12.02 micron imagery",
	"Sounder 11.03 micron imagery",
	"Sounder 9.71 micron imagery",
	"Sounder 7.43 micron imagery",
	"Sounder 7.02 micron imagery",
	"Sounder 6.51 micron imagery",
	"Sounder 4.57 micron imagery",
	"Sounder 4.52 micron imagery",
	"Sounder 4.45 micron imagery",
	"Sounder 4.13 micron imagery",
	"Sounder 3.98 micron imagery",
	"Sounder 3.74 micron imagery",
	"Sounder Visible imagery",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	"Reserved for future products",
	""
};

char *proj_string[] = {
	"",
	"MERCATOR",
	"",
	"LAMBERT",
	"",
	"POLAR",
	"",
};

char *compression_string[] = {
	"no compression",
	"compressed",
	"",
};

char *nav_cal_string[] = {
	"nav/cal not included",
	"nav/cal included",
	"nav only",
	"cal only",
	""
};

#endif /* PDB_H */

