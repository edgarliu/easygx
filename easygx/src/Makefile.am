AUTOMAKE_OPTIONS = foreign  subdir-objects

INCLUDES = -I. -I$(top_builddir)/include `pkg-config sfpr --cflags` `pkg-config sfel --cflags`
LIBS = `pkg-config sfpr --libs` `pkg-config sfel --libs`

#AM_CFLAGS = $(CFLAGS) -DLOG_DEBUG -DLOG_ERROR
#AM_LDFLAGS =

noinst_LTLIBRARIES = libguimain.la

libguimain_la_LDFLAGS = -shared

libguimain_la_SOURCES = \
	easygx_core.c                           \
	easygx_widget.c                       \
	easygx_message.c                   \
	easygx_frame.c                        \
	easygx_control.c                       \
	easygx_sfel.c                            \
	easygx_draw.c                          \
	easygx_log.c                             \
	easygx_utils.c
