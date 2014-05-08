AUTOMAKE_OPTIONS = foreign

#注: 如果本文件中不包含 INCLUDES 和 LIBS 变量，那么编译时, 系统就会自动就把 AC_CHECK_LIB 指定的库加到 LIBS中。
#INCLUDES = -I. -I$(top_builddir)/include `pkg-config sfpr --cflags` `pkg-config sfel --cflags`
#LIBS = `pkg-config sfpr --libs` `pkg-config sfel --libs`
 
#AM_CFLAGS = -DLOG_ERROR -DLOG_NOTICE
#AM_LDFLAGS = 

lib_LTLIBRARIES = libeasygx.la

libeasygx_la_SOURCES = 

if MINGW32
libeasygx_la_LDFLAGS = -shared -release $(VERSION) -no-undefined
else
libeasygx_la_LDFLAGS = -shared -release $(VERSION)
endif

libeasygx_la_LIBADD = src/libguimain.la

SUBDIRS = 

if ENABLE_UCGUI
#INCLUDES += `pkg-config ucgui --cflags`
#LIBS += `pkg-config ucgui --libs`
SUBDIRS += ucgui
libeasygx_la_SOURCES += versionc.c
libeasygx_la_LIBADD += ucgui/libgui_ucgui.la
endif

if ENABLE_QT4
#INCLUDES += `pkg-config QtGui --cflags`
#LIBS +=  `pkg-config QtGui --libs`
libeasygx_la_SOURCES += versionp.cpp
SUBDIRS += qt
libeasygx_la_LIBADD += qt/libgui_qt.la
endif

if ENABLE_QT5
#INCLUDES += `pkg-config Qt5Gui --cflags`
#LIBS +=  `pkg-config Qt5Gui --libs`
libeasygx_la_SOURCES += versionp.cpp
SUBDIRS += qt
libeasygx_la_LIBADD += qt/libgui_qt.la
endif

if ENABLE_WINGDI
#LIBS +=  -luser32 -lgdi32
libeasygx_la_SOURCES += versionp.cpp
SUBDIRS += win32
libeasygx_la_LIBADD += win32/libgui_win32.la
endif

SUBDIRS += src . tests

EXTRA_DIST = easygx.pc.in Doxyfile setenv.sh include/easygx_core.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = easygx.pc

libeasygxincludedir = $(includedir)
libeasygxinclude_HEADERS =         \
	include/easygx.h                       \
	include/easygx_widget.h            \
	include/easygx_frame.h             \
	include/easygx_control.h            \
	include/easygx_message.h        \
	include/easygx_input.h              \
	include/easygx_draw.h              \
	include/easygx_portable.h         \
	include/easygx_sfel.h                \
	include/easygx_utils.h                \
	include/easygx_log.h
