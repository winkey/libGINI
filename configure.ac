dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(libGINI, 0.4)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC






AM_PROG_LIBTOOL


AC_ARG_WITH(www,
  [AS_HELP_STRING([--with-www=someurl],
    [base url to use in kml output])],
  if test -n "$withval"
  then
    WWW="$withval"
    AC_SUBST(WWW)
  else
		[AC_MSG_FAILURE([--with-www=someurl MUST be specified])]
	fi,
  [AC_MSG_FAILURE([--with-www=someurl MUST be specified])]
)

AC_ARG_WITH(wwwdisk,
  [AS_HELP_STRING([--with-wwwdisk=/some/path],
    [base dir to output kml and images])],
  if test -n "$withval"
  then
    WWWDISK="$withval"
    AC_SUBST(WWWDISK)
  else
		[AC_MSG_FAILURE([--with-wwwdisk=/some/path MUST be specified])]
	fi,
  [AC_MSG_FAILURE([--with-wwwdisk=/some/path MUST be specified])]
)

AC_ARG_WITH(cgidisk,
  [AS_HELP_STRING([--with-cgidisk=/some/path],
    [base dir to output cgi scripts])],
  if test -n "$withval"
  then
    CGIDISK="$withval"
    AC_SUBST(CGIDISK)
  else
		[AC_MSG_FAILURE([--with-cgidisk=/some/path MUST be specified])]
	fi,
  [AC_MSG_FAILURE([--with-cgidisk=/some/path MUST be specified])]
)

AC_MSG_CHECKING([how to link PROJ.4 library])

AC_ARG_WITH(static_proj4,[  --with-static-proj4=ARG Compile with PROJ.4 statically (ARG=no or path)],,)

if test "x$with_static_proj4" = "xno" -o "x$with_static_proj4" = "x"; then

  PROJ_STATIC=no
  AC_MSG_RESULT([link dynamically.])

else

  PROJ_STATIC=yes
  AC_MSG_RESULT([link statically.])

  ORIG_LIBS="$LIBS"
  PROJ_LIB="-lproj"

  if test "x$with_static_proj4" = "xyes" ; then
    LIBS="$PROJ_LIB $ORIG_LIBS"
    AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
  else
  
    LIBS="-L$with_static_proj4/src $PROJ_LIB $ORIG_LIBS"
    AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)

    if test "$PROJ_STATIC" = "no"; then
      dnl Disable previous value cached by the previous AC_CHECK_LIB test :
      dnl AC_CHECK_LIB caches the result value for a couple (function, library)
      dnl in a variable named "ac_cv_lib_<library name>_<function name>".
      dnl In order to try to locate the library in different locations, using
      dnl different values of LIBS, we have to 'unset' this variable after
      dnl each negative test.

      unset ac_cv_lib_proj_pj_init

      LIBS="-L$with_static_proj4/lib $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="-L$with_static_proj4/src/.libs $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="-L$with_static_proj4 $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="$PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

  fi

  if test "$PROJ_STATIC" = "no"; then
    LIBS="$ORIG_LIBS"
  fi

  if test -r "$with_static_proj4/src/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4/src"
  elif test -r "$with_static_proj4/include/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4/include"
  elif test -r "$with_static_proj4/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4"
  fi
  
fi

AC_SUBST(PROJ_STATIC)
AC_SUBST(PROJ_INCLUDE)



AC_OUTPUT([
Makefile
apps/Makefile
apps/include/Makefile
cgi/Makefile
etc/Makefile
kml/Makefile
scripts/Makefile
src/Makefile

])
