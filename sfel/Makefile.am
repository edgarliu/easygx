AUTOMAKE_OPTIONS = foreign subdir-objects 

INCLUDES = -Iinclude `pkg-config sfpr --cflags`
LIBS =  `pkg-config sfpr --libs`

if HAVE_LUA
INCLUDES += -Ilua
LIBS += -Llua -llua
endif

AM_CFLAGS = -DLOG_ERROR #-DLOG_NOTICE -DLOG_INFO -DLOG_DEBUG

#AM_LDFLAGS = 

SUBDIRS= $(subdirs) . tests

lib_LTLIBRARIES = libsfel.la

Portable_Sources = 
if ENABLE_PUGIXML
Portable_Sources += \
	pugixml/sfel_portable.cpp   \
	pugixml/pugixml.cpp	
endif

if ENABLE_RAPIDXML
Portable_Sources += \
	rapidxml/sfel_portable.cpp
endif

libsfel_la_SOURCES =          \
		src/sfel.c                     \
		src/sfel_inst.c              \
		src/sfel_ref.c                \
		src/sfel_dsfel.c             \
		src/sfel_lua.c                \
		$(Portable_Sources) 

if MINGW32
libsfel_la_CXXFLAGS = -fpermissive
libsfel_la_LDFLAGS = -shared -release $(VERSION) -no-undefined
else
libsfel_la_LDFLAGS = -shared -release $(VERSION)
endif
#libsfel_la_LDFLAGS = -shared -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

EXTRA_DIST = sfel.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sfel.pc

libsfelincludedir = $(includedir)
libsfelinclude_HEADERS =      \
	include/sfel.h                    \
	include/sfel_global.h           \
	include/sfel_xml.h              \
	include/sfel_dsfel.h
