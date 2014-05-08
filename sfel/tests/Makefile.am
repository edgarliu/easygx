AUTOMAKE_OPTIONS = foreign 

INCLUDES = -I$(top_builddir) -I$(top_builddir)/include  `pkg-config sfpr --cflags`
LIBS = -L$(top_builddir) -lsfel  `pkg-config sfpr --libs`

if HAVE_LUA
INCLUDES += -I$(top_builddir)/lua
LIBS += -L$(top_builddir)/lua -llua
endif

#CFLAGS = $(AM_CFLAGS)
if MINGW32
comm_ldflag =
else
comm_ldflag = -ldl
endif

test_sfel_SOURCES = test_sfel.c
test_sfel_LDADD = $(comm_ldflag)
test_sfel1_SOURCES = test_sfel1.c
test_sfel1_LDADD = $(comm_ldflag)
test_inst_SOURCES = test_inst.c
test_inst_LDADD = $(comm_ldflag)
test_dsfel_SOURCES =test_dsfel.c
test_dsfel_LDADD = $(comm_ldflag)
test_pugixml_SOURCES =test_pugixml.cpp $(top_builddir)/pugixml/pugixml.cpp
test_pugixml_LDADD = $(comm_ldflag)
#test_hash_CFLAGS = -DTESTNUM=1 $(AM_CFLAGS)
#test_hash_LDADD = $(top_builddir)/libsfpr.la


test_programs =        \
		test_sfel          \
		test_sfel1	      \
		test_inst          \
		test_dsfel        \
		test_pugixml

check_PROGRAMS = $(test_programs)

all: $(check_PROGRAMS)

TESTS = $(test_programs)
