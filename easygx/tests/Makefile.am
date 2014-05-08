AUTOMAKE_OPTIONS = foreign subdir-objects

INCLUDES = -I$(top_builddir) -I$(top_builddir)/include `pkg-config sfpr --cflags` `pkg-config sfel --cflags`
LIBS = $(top_builddir)/libeasygx.la `pkg-config sfpr --libs` `pkg-config sfel --libs`

AM_CFLAGS = $(CFLAGS) #-DLOG_ERROR
#AM_LDFLAGS =

if MINGW32
#AM_CFLAGS += -pipe -mwindows 
#AM_LDFLAGS = -Wl,--subsystem,windows
else
AM_LDFLAGS = -ldl
endif

easygx_SOURCES = easygx.c
easygx_api_SOURCES = easygx_api.c
easygx_msg_SOURCES = easygx_msg.c

bin_PROGRAMS =       \
	easygx_api      \
	easygx_msg      \
	easygx

test_programs = 

if ENABLE_WINGDI
test_programs += test_wingdi
test_wingdi_SOURCES = test_wingdi.cpp
endif

if ENABLE_QT4
bin_PROGRAMS += test_qt qt_demo1
qt_demo1_SOURCES = qt_demo1.cpp
qt_demo1_CPPFLAGS = `pkg-config QtGui --cflags`
qt_demo1_LDFLAGS = `pkg-config QtGui --libs`
test_qt_SOURCES = test_qt.cpp
test_qt_CPPFLAGS = `pkg-config QtGui --cflags`
test_qt_LDFLAGS = `pkg-config QtGui --libs`
endif

if ENABLE_QT5
bin_PROGRAMS += test_qt qt_demo1
qt_demo1_SOURCES = qt_demo1.cpp
qt_demo1_CXXFLAGS = `pkg-config Qt5Gui --cflags`  `pkg-config Qt5Widgets --cflags`
qt_demo1_LDFLAGS = `pkg-config Qt5Gui --libs`  `pkg-config Qt5Widgets --libs` 
test_qt_SOURCES = test_qt.cpp
test_qt_CPPFLAGS = `pkg-config Qt5Gui --cflags`  `pkg-config Qt5Widgets --cflags`
test_qt_LDFLAGS = `pkg-config Qt5Gui --libs`  `pkg-config Qt5Widgets --libs` 
endif

check_PROGRAMS = $(test_programs)

all: $(check_PROGRAMS) $(bin_PROGRAMS)

resdir = $(bindir)
res_DATA = mgwhelp.dll exchndl.dll easygx.xml

TESTS = $(test_programs)

EXTRA_DIST = $(res_DATA)
