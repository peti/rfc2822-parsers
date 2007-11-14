dnl configure.ac for librfc2822

AC_PREREQ(2.59)
AC_INIT([librfc2822], [2.0], [simons@cryp.to])
AM_INIT_AUTOMAKE([foreign 1.9])
AC_CONFIG_SRCDIR([rfc2822/base.hpp])

dnl Get rid of the lousy '-g -O2' defaults.
CFLAGS=${CFLAGS}
CXXFLAGS=${CXXFLAGS}

dnl Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

dnl Write results.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
