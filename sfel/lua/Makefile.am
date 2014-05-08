AUTOMAKE_OPTIONS = foreign

AM_CFLAGS = -O2 -Wall -DLUA_COMPAT_ALL

#AM_LDFLAGS = 

lib_LTLIBRARIES = liblua.la

CORE_C=	lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c \
	lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c \
	ltm.c lundump.c lvm.c lzio.c
LIB_C=	lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c \
	lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c

liblua_la_SOURCES =  $(CORE_C) $(LIB_C)

if MINGW32
liblua_la_CFLAGS = $(AM_CFLAGS)
liblua_la_LDFLAGS = -shared -no-undefined -release $(VERSION)
else
liblua_la_LDFLAGS = -shared -release $(VERSION)
#liblua_la_LDFLAGS = -shared -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 
endif

EXTRA_DIST = liblua.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liblua.pc 

libluaincludedir = $(includedir)/lua
libluainclude_HEADERS =          \
	lapi.h             \
	lua.h              \
	lualib.h           \
	lauxlib.h
	
