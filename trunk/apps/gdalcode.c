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

/***************************************************************************
 *            gdalcode.c
 *
 *  Wed Jan 16 11:25:01 2008
 *  Copyright  2008  winkey
 *  <rush@winkey.org>
 ****************************************************************************/

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
#include <gdal.h>
#include <gdal_alg.h>
#include <cpl_conv.h>
#include <ogr_api.h>
#include <ogr_srs_api.h>

#include "../src/libGINI.h"
#include "gdalcode.h"
#include "../src/error.h"

#define DEBUG 0

/*******************************************************************************
	function to open the raster file
	
	Args:
						float			pointer to the float to open
						width			width of the raster in pixels
						height		height of the raster in lines
	
	returns:
						the gdal dataset
*******************************************************************************/

GDALDatasetH gdal_open_mem(
	void *raster,
	char *datatype,
	int width,
	int height)
{
	GDALDatasetH result = NULL;
	char myfilename[300];
	
	snprintf(myfilename, sizeof(myfilename),
					 "MEM:::DATAPOINTER=%p,PIXELS=%i,LINES=%i,BANDS=1,DATATYPE=%s",
					 raster, width, height, datatype);
	
	if (!(result = GDALOpen(myfilename, GA_ReadOnly)))
		ERROR("raster_open");
	
	return result;
}

/*******************************************************************************
	function to transform the origins to the units the projection uses
*******************************************************************************/

void transform_origin(
	OGRSpatialReferenceH hSrcSRS,
	OGRSpatialReferenceH hDstSRS,
	double *MinX,
	double *MaxY)
{
											
	OGRCoordinateTransformationH transform = NULL;
	
	if (!(transform = OCTNewCoordinateTransformation(hSrcSRS, hDstSRS)))
		fprintf(stderr, "WARNING: failed to create Transformation\n");
	else {
		if (!OCTTransform(transform, 1, MinX, MaxY, NULL))
			fprintf(stderr, "WARNING: Transformation failed\n");
		
		OCTDestroyCoordinateTransformation(transform);
	}
	
	return;
}

/*******************************************************************************
	function to set the projection in the gdal dataset
*******************************************************************************/

void gdal_set_projection(
	GDALDatasetH hDS,
	OGRSpatialReferenceH hSRS)
{
	char *wkt;
	
	OSRExportToWkt(hSRS, &wkt);
	if (DEBUG) fprintf (stderr, "%s\n", wkt);
	GDALSetProjection(hDS, wkt);
	
	free (wkt);
	return;
}

/*******************************************************************************
	function to set the raster geotransform
	
	Args:
						ds				gdal dataset
						xo				x origin
						xsize			x pixel size
						xrot			x rotation
						yo				y origin
						ysize			y pixel size
						yrot			y rotation
	
	returns:
						nothing
*******************************************************************************/

void set_geotransform(
	GDALDatasetH hDS,
	double xo,
	double xsize,
	double xrot,
	double yo,
	double ysize,
	double yrot)


{
	double padfTransform[6];
	
	padfTransform[0] = xo;     /* X Origin (top left corner) */
	padfTransform[1] = xsize;      /* X Pixel size */
	padfTransform[2] = xrot;
	padfTransform[3] = yo;     /* Y Origin (top left corner) */
	padfTransform[4] = yrot;     
	padfTransform[5] = ysize;      /* Y Pixel Size */
	
	GDALSetGeoTransform(hDS, padfTransform);
	
	if (DEBUG) {
		if(padfTransform[2] == 0.0 && padfTransform[4] == 0.0) {
			fprintf(stderr, "Origin = (%.15f,%.15f)\n",
							padfTransform[0], padfTransform[3]);
			fprintf(stderr, "Pixel Size = (%.15f,%.15f)\n",
							padfTransform[1], padfTransform[5]);
		}
		else
			fprintf(stderr, "GeoTransform =\n  %.16g, %.16g, %.16g\n  %.16g, %.16g, %.16g\n",
							padfTransform[0], padfTransform[1], padfTransform[2],
							padfTransform[3], padfTransform[4], padfTransform[5]);
	}
	return;
}

/*******************************************************************************
	function to set the raster projection
	
	args:
						hDS			the gdal dataset
						pdb			the gini pds info struct
	
	returns:
						nothing
				
*******************************************************************************/

#define RADIUS 6371200.0
OGRSpatialReferenceH set_projection(
	GDALDatasetH *hDS,
	GINI_pdb *pdb)
{
	OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
	OGRSpatialReferenceH hSrsSRS = OSRNewSpatialReference(NULL);
	double MinX, MinY, Dx, Dy, Ny, Nx, MaxX, MaxY;
	
	OSRSetGeogCS(hSrsSRS, "Sphere", NULL, "Sphere", RADIUS, 0.0,
									"Greenwich", 0.0, NULL, 0.0);
	
	OSRSetGeogCS(hSRS, "Sphere", NULL, "Sphere", RADIUS, 0.0,
							 "Greenwich", 0.0, NULL, 0.0);
	
	switch(pdb->proj) {

		case MERCATOR:
			OSRSetMercator(hSRS, pdb->mercator.latin, 0.0, 1, 0.0, 0.0);
			
			MinX = pdb->mercator.lon1;
			MinY = pdb->mercator.lat1;
			MaxX = pdb->mercator.lon2;
			MaxY = pdb->mercator.lat2;
			
			Nx = pdb->mercator.nx;
			Ny = pdb->mercator.ny;

			if (!(pdb->mercator.resolution & 1)) {
				
				/***** does it wrap the dateline *****/
				
				if (MinX > MaxX) {
					MinX += 180.0;
					MaxX += 180.0;
				}
					
				transform_origin(hSrsSRS, hSRS, &MinX, &MinY);
				transform_origin(hSrsSRS, hSRS, &MaxX, &MaxY);
				Dx = (MaxX - MinX) / Nx;
				Dy = (MaxY - MinY) / Ny;
				MinX = pdb->mercator.lon1;
				MinY = pdb->mercator.lat1;
			}
			
			break;
		
		case POLAR:
			if (pdb->polar.centerflag & 1)
				OSRSetPS(hSRS, -60, pdb->polar.loV, 1.0, 0.0, 0.0);
			else
				OSRSetPS(hSRS, 60, pdb->polar.loV, 1.0, 0.0, 0.0);
				
			MinX = pdb->polar.lon1;
			MinY = pdb->polar.lat1;
			Dx = pdb->polar.Dx;
			Dy = pdb->polar.Dy;
			
			break;
		
		case LAMBERT:
			OSRSetLCC1SP(hSRS, pdb->lambert.tanlat, pdb->lambert.loV, 1.0, 0.0, 0.0);

			MinX = pdb->lambert.lon1;
			MinY = pdb->lambert.lat1;
			Dx = pdb->lambert.Dx;
			Dy = pdb->lambert.Dy;
			
			break;
	}

	transform_origin(hSrsSRS, hSRS, &MinX, &MinY);
	set_geotransform(hDS, MinX, Dx, 1, MinY, Dy, -1);
	
	

	gdal_set_projection(hDS, hSRS);
	
	/***** cleanup *****/
	
	OSRDestroySpatialReference(hSrsSRS);

	return hSRS;
}

/*******************************************************************************
	function to get the raster band
	
	args:
						hDS				gdal dataset
						band_num	the band number
	
	returns:
						the gdal raster band
*******************************************************************************/

GDALRasterBandH get_band(
	GDALDatasetH hDS,
	int band_num)
{
	GDALRasterBandH band = NULL;
	
	if (!(band = GDALGetRasterBand(hDS, band_num))) {
		fprintf (stderr, "Band %d does not exist on dataset.\n", band_num);
		exit(EXIT_FAILURE);
	}
	
	return band;
}

/*******************************************************************************
	function to create a new gdal dataset
	
	args:
						drivername	the format to create
						filename		the file to create
						xsize				the width of the raster
						ysize				the height of the raster
	
	returns:
						the gdal datasource
*******************************************************************************/

GDALDatasetH gdal_create (
	char *drivername,
	char *filename,
	int xsize,
	int ysize)
{
	char **papszMetadata;
	GDALDriverH hDriver;
	GDALDatasetH hDstDS;        
	char **papszOptions = NULL;
	
	if(!(hDriver = GDALGetDriverByName(drivername)))
		ERROR("GDALGetDriverByName");
	
	papszMetadata = GDALGetMetadata(hDriver, NULL);
	
	//if(!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE)) {
		if (!(hDstDS = GDALCreate(hDriver, filename, xsize, ysize, 1, GDT_Byte, papszOptions)))
			ERROR("GDALCreate");
	//.}
	
	return hDstDS;
}

