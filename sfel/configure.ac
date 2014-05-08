#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(libsfel, 1.0)

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/sfel.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(libsfel, 1.3)
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

AC_ARG_WITH([xml],
            [AS_HELP_STRING([--with-xml],[support xml library @<:@default=pugixml@:>@])],
            [],[with_xml=pugixml])
case "${with_xml}" in 
	rapidxml)
	AC_DEFINE_UNQUOTED([ENABLE_RAPIDXML], [1],[Support With rapidxml])
	;;
	pugixml)
	AC_DEFINE_UNQUOTED([ENABLE_PUGIXML], [1],[Support With pugixml])
	;;
	mxml)
	AC_DEFINE_UNQUOTED([ENABLE_MXML], [1],[Support With mxml])
	;;
esac

AC_MSG_NOTICE([checking with_xml $with_xml])
AM_CONDITIONAL([ENABLE_RAPIDXML],[test x$with_xml = xrapidxml])
AM_CONDITIONAL([ENABLE_PUGIXML],[test x$with_xml = xpugixml])
AM_CONDITIONAL([ENABLE_MXML],[test x$with_xml = xmxml])

# Checks for libraries.
case "${host_os}" in
	*linux*)
	AC_CHECK_LIB(pthread,pthread_create, ,AC_MSG_ERROR([*** Unable to find pthread Library with thread support]),)
	;;
esac
AC_CHECK_LIB(sfpr,sfpr_hash_new, ,AC_MSG_ERROR([*** Unable to find sfpr Library with basic library support]),`pkg-config sfpr --libs`)

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strchr])

AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_ARG_ENABLE(debug,
		[ --enable-debug			turn on debug],
		CFLAGS="$CFLAGS -DLOG_NOTICE")

if test -d "$srcdir/lua"; then
	AC_CONFIG_SUBDIRS([lua])
	AC_DEFINE_UNQUOTED([HAVE_LUA], [1],[Support With Lua])
	have_lua="yes"
fi
AM_CONDITIONAL(HAVE_LUA,[test x$have_lua = xyes])

AC_CONFIG_FILES([Makefile
	tests/Makefile
	sfel.pc])

AC_OUTPUT
