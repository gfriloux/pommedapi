bin_PROGRAMS += src/bin/pommedapi

src_bin_pommedapi_SOURCES = \
   src/bin/main.c \
   src/bin/pommedapi.c \
   src/bin/pommedapi.h \
   src/bin/list.c \
   src/bin/run.c
src_bin_pommedapi_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS) -DDATA_DIR=\"$(datadir)\"
src_bin_pommedapi_LDFLAGS = $(POMMEDAPI_LIBS)
src_bin_pommedapi_LDADD = \
   src/lib/libgeneric.la \
   src/lib/libserialize.la \
   src/lib/libhttp.la \
   src/lib/libtest.la \
   src/lib/librendering.la
