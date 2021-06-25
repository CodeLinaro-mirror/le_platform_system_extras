ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS     = procrank

procrank_SOURCES = procrank.c

procrank_CPPFLAGS  = @LIBPAGEMAP_CFLAGS@ @LIBCUTILS_CFLAGS@

procrank_CFLAGS  = -Wall -Wno-unused-parameter
procrank_CFLAGS += -include signal.h
procrank_CFLAGS += -include stdio.h
procrank_CFLAGS += -include memory.h
procrank_CFLAGS += -include unistd.h
procrank_CFLAGS += -D_GNU_SOURCE

procrank_LDADD     = @LIBPAGEMAP_LIBS@ @LIBCUTILS_LIBS@
