## File created by the gnome-build tools


www = @WWW@
wwwdisk = @WWWDISK@

wwwdir = ${wwwdisk}/kml

www_DATA = \
	satz.kml \
	satz-latest.kml
	
EXTRA_DIST = \
	satz.kml.in \
	satz-latest.kml.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
satz.kml: satz.kml.in Makefile
	$(do_subst) < $(srcdir)/satz.kml.in > satz.kml
satz-latest.kml: satz-latest.kml.in Makefile
	$(do_subst) < $(srcdir)/satz-latest.kml.in > satz-latest.kml
