www = @WWW@
wwwdisk = @WWWDISK@
cgidisk = @CGIDISK@

cgidir = ${cgidisk}

cgi_DATA = \
	sat \
	sat_latest \
	satwrap \
	satwrap_latest \
	sat_wms

CLEANFILES = $(cgi_DATA)

EXTRA_DIST = \
	sat.in \
	sat_latest.in \
	satwrap.in \
	satwrap_latest.in \
	sat_wms.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]cgidisk[@],$(cgidisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
sat: sat.in Makefile
	$(do_subst) < $(srcdir)/sat.in > sat
	chmod +x sat

sat_latest: sat_latest.in Makefile
	$(do_subst) < $(srcdir)/sat_latest.in > sat_latest
	chmod +x sat_latest

satwrap: satwrap.in Makefile
	$(do_subst) < $(srcdir)/satwrap.in > satwrap
	chmod +x satwrap

satwrap_latest: satwrap_latest.in Makefile
	$(do_subst) < $(srcdir)/satwrap_latest.in > satwrap_latest
	chmod +x satwrap_latest

sat_wms: sat_wms.in Makefile
	$(do_subst) < $(srcdir)/sat_wms.in > sat_wms
	chmod +x sat_wms


