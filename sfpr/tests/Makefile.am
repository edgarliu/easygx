
INCLUDES = -I$(top_builddir)/include
LIBS = -L$(top_builddir)

if MINGW32
AM_LDFLAGS = -lsfpr
else
AM_LDFLAGS = -lsfpr -lrt
endif

test_hash_SOURCES = test_hash.c
#test_hash_CFLAGS = -DTESTNUM=1 $(AM_CFLAGS)
#test_hash_LDADD = $(top_builddir)/libsfpr.la

test_memory_pool_SOURCES = test_memory_pool.c
test_network_client_SOURCES = test_network_client.c
test_network_server_SOURCES = test_network_server.c
test_queue_SOURCES = test_queue.c
test_newqueue_SOURCES = test_newqueue.c
test_time_SOURCES = test_time.c
test_list_SOURCES = test_list.c
test_dlist_SOURCES = test_dlist.c
test_thread_pool_SOURCES = test_thread_pool.c
test_file_io_SOURCES = test_file_io.c
test_stack_SOURCES = test_stack.c
test_serial_SOURCES = test_serial.c
test_log_SOURCES = test_log.c
test_crypt_SOURCES = test_crypt.c
test_cond_SOURCES = test_cond.c

bin_programs =          \
	test_serial

test_programs =         \
	test_hash             \
	test_memory_pool      \
	test_network_client   \
	test_network_server   \
	test_queue            \
	test_newqueue         \
	test_time             \
	test_list             \
	test_dlist			  \
	test_thread_pool      \
	test_file_io          \
	test_stack            \
	test_log							\
	test_crypt            \
	test_cond

bin_PROGRAMS = $(bin_programs)

check_PROGRAMS = $(test_programs)

all: $(check_PROGRAMS) $(bin_PROGRAMS)

TESTS = $(test_programs)
