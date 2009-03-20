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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <gdal.h>
#include <ogr_srs_api.h>
#include "../src/libGINI.h"
#include "../src/error.h"

#include "gdalcode.h"

int main (int argc, char **argv) {
	FILE *GINI_fp;
	
	char *ginifile = NULL;
	char *format = NULL;
	char *outfile = NULL;
	int threshhold = 0;
	int i, m;
	
	GINI data = {};
	
	GDALDatasetH in_DS;
	GDALDatasetH out_DS;
	OGRSpatialReferenceH in_SRS;
	OGRSpatialReferenceH out_SRS;
	GDALRasterBandH in_Band;
	GDALRasterBandH out_Band;
	
	int opt;
	
	while (0 < (opt = getopt(argc, argv, "g:f:o:t:h"))) {
		
		switch (opt) {
			case 'g':
				ginifile = optarg;
				break;
			
			case 'f':
				format = optarg;
				break;
			
			case 'o':
				outfile = optarg;
				break;
			
			case 't':
				threshhold = atoi(optarg);
				break;
			
			case 'h':
			case '?':
			default:
				fprintf(stderr,
								"USAGE: %s <-g gini file> <-o outfile> [-f format>] [-t 0-255]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	
	if (!outfile || !ginifile || threshhold < 0 || threshhold > 255) {
		fprintf(stderr,	"USAGE: %s <-g gini file> <-o outfile> [-f format>] [-t 0-255]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (!format)
		format = "gtiff";
	
	GINI_fp = GINI_open(ginifile);
	GINI_read(GINI_fp, &data);
	
	GDALAllRegister();
	
	/*
	in_DS = gdal_open_mem(data.ids, "Byte", data.pdb.pixels_per_line,
												data.pdb.num_lines);
	
  in_SRS = set_projection(in_DS, &(data.pdb));
  
	in_Band = get_band(in_DS, 1);
	*/
	
	out_DS = gdal_create(format, outfile, data.pdb.pixels_per_line,
											 data.pdb.num_lines);
	
	out_SRS = set_projection(out_DS, &(data.pdb));
	
	out_Band = get_band(out_DS, 1);
	
	/***** set threshold *****/
	
	for (i = 0; i < data.pdb.pixels_per_line * data.pdb.num_lines ; i++) {
		if ((unsigned int) data.ids[i] <= threshhold)
			data.ids[i] = 0;
	}
	
	for (i = data.pdb.num_lines - 1, m = 0; i >= 0 ; i--, m++) {
		
		GDALRasterIO(out_Band, GF_Write, 0, m,
								 data.pdb.pixels_per_line, 1,
								 data.ids + i * data.pdb.pixels_per_line,
								 data.pdb.pixels_per_line, 1,
								 GDT_Byte, 0, 0 );   
	}
	
	GDALClose(out_DS);
	OSRDestroySpatialReference(out_SRS);
	GDALDestroyDriverManager();
	
	GINI_free(&data);
	GINI_close(GINI_fp);

	return EXIT_SUCCESS;
}
