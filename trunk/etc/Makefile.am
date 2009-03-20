## File created by the gnome-build tools


sysconf_DATA = \
	pqact.libGINI.conf

CLEANFILES = $(sysconf_DATA)

EXTRA_DIST = \
	pqact.libGINI.conf.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g'

pqact.libGINI.conf: pqact.libGINI.conf.in Makefile
	$(do_subst) < $(srcdir)/pqact.libGINI.conf.in > pqact.libGINI.conf
