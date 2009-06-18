www = @WWW@
wwwdisk = @WWWDISK@
cgidisk = @CGIDISK@

cgidir = ${cgidisk}

cgi_DATA = \
	sat \
	satwrap \
	sat_wms

EXTRA_DIST = \
	sat.in \
	satwrap.in \
	sat_wms.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]cgidisk[@],$(cgidisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
sat: sat.in Makefile
	$(do_subst) < $(srcdir)/sat.in > sat
	chmod +x sat

satwrap: satwrap.in Makefile
	$(do_subst) < $(srcdir)/satwrap.in > satwrap
	chmod +x satwrap

sat_wms: sat_wms.in Makefile
	$(do_subst) < $(srcdir)/sat_wms.in > sat_wms
	chmod +x sat_wms

