bin_PROGRAMS += src/bin/pommedapi

src_bin_pommedapi_SOURCES = \
   src/bin/main.c \
   src/bin/pommedapi.c \
   src/bin/pommedapi.h \
   src/bin/test.c \
   src/bin/test_list.c \
   src/bin/test_query.c \
   src/bin/utils.c
src_bin_pommedapi_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_bin_pommedapi_LDFLAGS = $(POMMEDAPI_LIBS)
src_bin_pommedapi_LDADD = \
   src/lib/libgfile.la \
   src/lib/libserialize.la \
   src/lib/libhttp.la
