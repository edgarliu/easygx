AUTOMAKE_OPTIONS = foreign subdir-objects

INCLUDES = -Iinclude

AM_CFLAGS = -DLOG_ERROR #-DLOG_NOTICE -DLOG_INFO -DLOG_DEBUG

SUBDIRS = . tests

lib_LTLIBRARIES = libsfpr.la

if MINGW32
libsfpr_la_SOURCES = sfpr.c             \
	strings/sfpr_string.c                 \
	memory/sfpr_memory_pool.c             \
	hash/sfpr_hash.c                      \
	network/sfpr_network.c                \
	poll/select/sfpr_select.c             \
	queue/sfpr_queue.c                    \
	list/sfpr_list.c                      \
	stack/sfpr_stack.c                    \
	log/sfpr_log.c                        \
	crypt/aes.c                           \
	crypt/aes_core.c                      \
	crypt/aes_cbc.c                       \
	crypt/md5.c                           \
	crypt/des.c                           \
	crypt/sfpr_crypt.c                    \
	md5/sfpr_md5.c                        \
	locks/win32/sfpr_thread_cond.c        \
	locks/win32/sfpr_thread_mutex.c       \
	thread/win32/sfpr_thread.c            \
	thread/sfpr_thread_pool.c             \
	time/win32/sfpr_time.c                \
	time/sfpr_timecal.c                \
	timer/win32/sfpr_timer.c              \
	file_io/win32/sfpr_file_io.c          \
	file/win32/sfpr_file.c                \
	serial/win32/sfpr_serial.c

libsfpr_la_LDFLAGS = -shared -release $(VERSION) -no-undefined -lws2_32

else

if HAVE_EPOLL
SYS_SRC = poll/epoll/sfpr_epoll.c
else
if HAVE_SELECT
SYS_SRC = poll/select/sfpr_select.c
else
SYS_SRC = 
endif
endif

libsfpr_la_SOURCES = sfpr.c         \
	strings/sfpr_string.c							\
	memory/sfpr_memory_pool.c					\
	hash/sfpr_hash.c									\
	network/sfpr_network.c						\
	queue/sfpr_queue.c                \
	list/sfpr_list.c                  \
	stack/sfpr_stack.c                \
	log/sfpr_log.c                    \
	crypt/aes.c										    \
	crypt/aes_core.c                  \
	crypt/aes_cbc.c										\
	crypt/md5.c                       \
	crypt/des.c                       \
	crypt/sfpr_crypt.c                \
	locks/unix/sfpr_thread_cond.c     \
	locks/unix/sfpr_thread_mutex.c		\
	thread/unix/sfpr_thread.c         \
	thread/sfpr_thread_pool.c         \
	time/unix/sfpr_time.c             \
	time/sfpr_timecal.c                \
	timer/unix/sfpr_timer.c           \
	file_io/unix/sfpr_file_io.c       \
	file/unix/sfpr_file.c             \
	serial/unix/sfpr_serial.c         \
	$(SYS_SRC)

libsfpr_la_LDFLAGS = -shared -release $(VERSION) -lpthread -lrt
#libsfpr_la_LDFLAGS = -shared -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 

endif


EXTRA_DIST = AUTHORS README sfpr.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sfpr.pc

libsfprincludedir = $(includedir)
libsfprinclude_HEADERS = 				\
	include/sfpr_global.h 				\
	include/sfpr_string.h					\
	include/sfpr_memory_pool.h 		\
	include/sfpr_hash.h 					\
	include/sfpr_thread_cond.h		\
	include/sfpr_thread_mutex.h		\
	include/sfpr_proc_mutex.h			\
	include/sfpr_network.h				\
	include/sfpr_poll.h           \
	include/sfpr_queue.h          \
	include/sfpr_thread.h         \
	include/sfpr_thread_pool.h    \
	include/sfpr_list.h           \
	include/sfpr_time.h           \
	include/sfpr_timecal.h                \
	include/sfpr_timer.h          \
	include/sfpr_file_io.h        \
	include/sfpr_stack.h          \
	include/sfpr_serial.h         \
	include/sfpr_log.h            \
	include/sfpr_file.h           \
	include/sfpr_crypt.h

distdir = $(prefix)/dist
