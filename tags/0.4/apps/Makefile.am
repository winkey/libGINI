
bin_PROGRAMS = \
	GINI_info\
	GINI2gdal

GINI2gdal_SOURCES = \
	GINI2gdal.c\
	gdalcode.c \
	gdalcode.h

GINI2gdal_LDFLAGS =       \
	-L../src/.libs       \
	-lGINI       \
	-lgdal       \
	-lz

GINI_info_SOURCES = \
	GINI_info.c\
	../src/libGINI.h

GINI_info_LDFLAGS = \
	-L../src/.libs \
	-lGINI \
	-lz

SUBDIRS = \
	include

## File created by the gnome-build tools

