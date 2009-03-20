## File created by the gnome-build tools


www = @WWW@
wwwdisk = @WWWDISK@

bin_SCRIPTS = \
	satz.sh

CLEANFILES = $(bin_SCRIPTS)

EXTRA_DIST = \
	satz.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
satz.sh: satz.in Makefile
	$(do_subst) < $(srcdir)/satz.in > satz.sh
	chmod +x satz.sh
