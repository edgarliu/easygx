#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(libsfpr, 1.0)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([hash/sfpr_hash.c])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(libsfpr, 1.3)
#LT_INIT([enable-shared])

AC_CYGWIN
AC_MINGW32

AC_MSG_NOTICE([checking host_os $host_os])
AM_CONDITIONAL([MINGW32],[test x$host_os = xmingw32])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

# Checks for libraries.
case "${host_os}" in
	*linux*)
	AC_CHECK_LIB(pthread,pthread_create, ,AC_MSG_ERROR([*** Unable to find pthread Library with thread support]),)
	;;
	*mingw*)
	;;
esac

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT32_T

# Checks for library functions.
haveselect=no
AC_CHECK_FUNC(select,[haveselect=yes],)
AM_CONDITIONAL([HAVE_SELECT],[test x$haveselect = xyes])

haveepoll=no
AC_CHECK_FUNC(epoll_create,[haveepoll=yes],)
AM_CONDITIONAL([HAVE_EPOLL],[test x$haveepoll = xyes])

AC_CHECK_FUNCS([memset strchr])

AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_ARG_ENABLE(debug,
		[ --enable-debug			turn on debug],
		CFLAGS="$CFLAGS -DLOG_NOTICE")

AC_CONFIG_FILES([Makefile
	tests/Makefile
	sfpr.pc])

AC_OUTPUT
