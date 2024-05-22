ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS  = $(GLIB_CFLAGS) -include glib.h -Dstrlcpy=g_strlcpy -Dstrlcat=g_strlcat

# Build libhealthmon.so
lib_LTLIBRARIES        = libhealthmon.la

libhealthmon_la_SOURCES   = libhealthmon.c

libhealthmon_la_CPPFLAGS  = -I$(top_srcdir)
libhealthmon_la_CPPFLAGS += -std=c++11 -Wall -Wno-unused-parameter
libhealthmon_la_CPPFLAGS += $(AM_CPPFLAGS) -Wno-deprecated-declarations

libhealthmon_la_LDFLAGS    = -ldl
libhealthmon_la_LIBADD     = $(GLIB_LIBS)

# Export header
libhealthmon_includedir      = $(includedir)
libhealthmon_include_HEADERS = $(top_srcdir)/libhealthmon.h

# Build  healthmon utility
root_sbindir           = "/sbin"
root_sbin_PROGRAMS     = healthmon
healthmon_CC = @CC@
healthmon_SOURCES  = healthmon.c
healthmon_LDADD = libhealthmon.la

healthmon_CPPFLAGS  = -I$(top_srcdir)
healthmon_CPPFLAGS += -std=c++11 -Wall -Wno-unused-parameter
healthmon_CPPFLAGS += $(AM_CPPFLAGS) -Wno-deprecated-declarations

#healthmon_LDADD     = libhealthmon.la
