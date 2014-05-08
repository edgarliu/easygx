#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(easygx, 0.3)

AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/easygx_core.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(easygx,0.3)
AC_ENABLE_SHARED
AC_DISABLE_STATIC
#AM_GNU_GETTEXT

AC_CYGWIN
AC_MINGW32

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

AC_MSG_NOTICE([checking host_os $host_os])
AM_CONDITIONAL([MINGW32],[test x$host_os = xmingw32])

# Checks for libraries.
case "${host_os}" in
	*linux*)
	AC_CHECK_LIB(pthread,pthread_create, ,AC_MSG_ERROR([*** Unable to find pthread Library with thread support]),)
	AC_CHECK_LIB(dl,dlopen, ,AC_MSG_ERROR([*** Unable to find dl Library with dynamic library support]),)
	;;
	*mingw*)
	;;
esac

AC_ARG_WITH([gui],
            [AS_HELP_STRING([--with-gui],[support gui library @<:@default=wingdi@:>@])],
            [],[with_gui=wingdi])
case "${with_gui}" in 
	ucgui)
	AC_CHECK_LIB(ucgui,main, ,AC_MSG_ERROR([*** Unable to find ucgui Library with ucgui support]),`pkg-config ucgui --libs`)
	AC_DEFINE_UNQUOTED([ENABLE_UCGUI], [1],[Support With ucGui])
	AC_DEFINE_UNQUOTED([GUI_LIBARY], ["ucgui"],[Support With GUI Library])
	;;
	qt)
	AC_CHECK_LIB(QtCore,main,qtversion="v4", ,`pkg-config QtGui --libs  --short-errors`)
	AC_CHECK_LIB(Qt5Core,main,qtversion="v5" , ,`pkg-config Qt5Gui --libs  --short-errors`)
	if [ test "$qtversion" = "v4" ] ; then
		AC_DEFINE_UNQUOTED([ENABLE_QT4], [1],[Support With Qt4])	
		AC_DEFINE_UNQUOTED([GUI_LIBARY], ["qt4"],[Support With GUI Library])	
	elif [ test "$qtversion" = "v5" ] ; then
		AC_DEFINE_UNQUOTED([ENABLE_QT5], [1],[Support With Qt5])	
		AC_DEFINE_UNQUOTED([GUI_LIBARY], ["qt5"],[Support With GUI Library])
	else
		AC_MSG_ERROR([*** Unable to find Qt Library with Qt support])
	fi 
	;;
	gtk)
	;;
	wingdi)
	AC_CHECK_LIB(user32,main, ,AC_MSG_ERROR([*** Unable to find Windows GDI Library with Windows support]),)
	AC_CHECK_LIB(gdi32,main, ,AC_MSG_ERROR([*** Unable to find Windows GDI Library with Windows support]),)
	AC_DEFINE_UNQUOTED([ENABLE_WINGDI], [1],[Support With Windows GDI])	
	AC_DEFINE_UNQUOTED([GUI_LIBARY], ["windows gdi"],[Support With GUI Library])
	;;
	*)
		AC_MSG_ERROR([*** Unknown Library: $with_gui])
	;;
esac

AC_MSG_NOTICE([checking with_gui $with_gui])
AM_CONDITIONAL([ENABLE_UCGUI],[test x$with_gui = xucgui])
AM_CONDITIONAL([ENABLE_QT4],[test x$qtversion = xv4])
AM_CONDITIONAL([ENABLE_QT5],[test x$qtversion = xv5])
AM_CONDITIONAL([ENABLE_WINGDI],[test x$with_gui = xwingdi])

AC_CHECK_LIB(sfpr,main, ,AC_MSG_ERROR([*** Unable to find sfpr Library]),`pkg-config sfpr --libs`)
AC_CHECK_LIB(sfel,main, ,AC_MSG_ERROR([*** Unable to find sfel Library]),`pkg-config sfel --libs`)

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

AC_CONFIG_FILES([Makefile
	src/Makefile
	win32/Makefile
	qt/Makefile
	ucgui/Makefile
	tests/Makefile
	easygx.pc])

AC_OUTPUT
