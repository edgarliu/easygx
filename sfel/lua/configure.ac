#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(lua, 5.2)

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([lapi.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(lua, 5.2)
#LT_INIT([enable-shared])

#AM_GNU_GETTEXT

AC_CYGWIN
AC_MINGW32

LT_CURRENT=1
LT_AGE=0
LT_REVISION=1
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_AGE)
AC_SUBST(LT_REVISION)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

AC_MSG_NOTICE([checking host_os $host_os])
AM_CONDITIONAL([MINGW32],[test x$host_os = xmingw32])

case "${host_os}" in
	*linux*)
	CFLAGS="$CFLAGS -DLUA_USE_LINUX"
	;;
	*mingw32*)
	CFLAGS="$CFLAGS -DLUA_BUILD_AS_DLL"
	;;
esac

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strchr])


AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_CONFIG_FILES([Makefile
		liblua.pc])

AC_OUTPUT
