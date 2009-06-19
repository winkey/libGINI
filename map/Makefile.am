www = @WWW@
wwwdisk = @WWWDISK@
cgidisk = @CGIDISK@

mapdir = ${wwwdisk}/sat

map_DATA = \
	sat.map

EXTRA_DIST = \
	sat.map.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]cgidisk[@],$(cgidisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
sat.map: sat.map.in Makefile
	$(do_subst) < $(srcdir)/sat.map.in > sat.map


