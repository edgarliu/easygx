AUTOMAKE_OPTIONS = foreign #subdir-objects

INCLUDES = -I. -I$(top_builddir)/include `pkg-config sfpr --cflags`
LIBS = `pkg-config sfpr --libs`

LIBS +=  -luser32 -lgdi32 -lComctl32

noinst_LTLIBRARIES = libgui_win32.la

libgui_win32_la_LDFLAGS = -shared
libgui_win32_la_SOURCES =                 \
	easygx_core_.cpp                           \
	easygx_widget_.cpp                      \
	easygx_frame_.cpp                       \
	easygx_control_.cpp                      \
	easygx_draw_.cpp

EXTRA_DIST = easygx_private.h
