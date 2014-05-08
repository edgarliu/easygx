AUTOMAKE_OPTIONS = foreign #subdir-objects

INCLUDES = -I. -I$(top_builddir)/include `pkg-config sfpr --cflags`
LIBS = `pkg-config sfpr --libs`

if ENABLE_QT4
INCLUDES += `pkg-config QtGui --cflags`
LIBS +=  `pkg-config QtGui --libs`
endif

if ENABLE_QT5
INCLUDES += `pkg-config Qt5Gui --cflags`  `pkg-config Qt5Widgets --cflags` 
LIBS +=  `pkg-config Qt5Gui --libs`  `pkg-config Qt5Widgets --libs` 
endif

noinst_LTLIBRARIES = libgui_qt.la

libgui_qt_la_LDFLAGS = -shared
libgui_qt_la_SOURCES =                       \
	easygx_core_.cpp                       \
	easygx_widget_.cpp                   \
	easygx_frame_.cpp                    \
	easygx_control_.cpp                  \
	easygx_draw_.cpp

nodist_libgui_qt_la_SOURCES =      \
	easygx_class_moc.cpp


if ENABLE_QT4
easygx_class_moc.cpp:easygx_class.h
	moc-qt4 easygx_class.h -o easygx_class_moc.cpp
endif

if ENABLE_QT5
easygx_class_moc.cpp:easygx_class.h
	moc easygx_class.h -o easygx_class_moc.cpp
endif

CLEANFILES = easygx_class_moc.cpp
DISTCLEANFILES = easygx_class_moc.cpp

EXTRA_DIST = easygx_class.h easygx_private.h
