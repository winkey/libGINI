## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g

EXTRA_DIST = \
	$(include_HEADERS)\
	$(includes_HEADERS)

lib_LTLIBRARIES = \
	libGINI.la

libGINI_la_SOURCES = \
	libGINI.h   \
	pdb.c   \
	get.h   \
	ids.c   \
	main.c   \
	error.h   \
	pdb.h

libGINI_la_LDFLAGS = \
	-lz

