AUTOMAKE_OPTIONS = foreign #subdir-objects

INCLUDES = -I. -I$(top_builddir)/include `pkg-config sfpr --cflags`
LIBS = `pkg-config sfpr --libs`

INCLUDES += `pkg-config ucgui --cflags`
LIBS += `pkg-config ucgui --libs`

noinst_LTLIBRARIES = libgui_ucgui.la

libgui_ucgui_la_LDFLAGS = -shared
libgui_ucgui_la_SOURCES =                   \
	easygx_core_.c                           \
	easygx_widget_.c                      \
	easygx_frame_.c                       \
	easygx_control_.c                     \
	easygx_draw_.c

EXTRA_DIST = easygx_private.h
