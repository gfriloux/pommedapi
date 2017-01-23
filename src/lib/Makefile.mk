MAINTAINERCLEANFILES += \
   src/lib/{gfile,json,serialize}/*.gc{no,da} \
   src/lib/json/Json_Common[_Azy,}.{c,h}

EXTRA_DIST += src/lib/json/json.azy
BUILT_SOURCES += .sources_json
.sources_json: src/lib/json/json.azy
	azy_parser -H -p -o $(top_srcdir)/src/lib/json $(top_srcdir)/src/lib/json/json.azy

noinst_LTLIBRARIES = \
   src/lib/libgfile.la \
   src/lib/libserialize.la \
   src/lib/libhttp.la \
   src/lib/libtemplate.la \
   src/lib/libtest.la \
   src/lib/librendering.la

src_lib_libgfile_la_SOURCES = \
   src/lib/gfile/gfile_main.c \
   src/lib/gfile/gfile_private.h \
   src/include/Gfile.h
src_lib_libgfile_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_libgfile_la_LDFLAGS = $(POMMEDAPI_LIBS)

src_lib_libserialize_la_SOURCES = \
   src/lib/serialize/serialize_main.c \
   src/lib/serialize/serialize_private.h \
   src/lib/json/Json_Common.c \
   src/lib/json/Json_Common.h \
   src/lib/json/Json_Common_Azy.c \
   src/lib/json/Json_Common_Azy.h \
   src/include/Serialize.h
src_lib_libserialize_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_libserialize_la_LDFLAGS = $(POMMEDAPI_LIBS)
src_lib_libserialize_la_LIBADD = \
   src/lib/libgfile.la

src_lib_libhttp_la_SOURCES = \
   src/lib/http/http_main.c \
   src/lib/http/http_events.c \
   src/lib/http/http_private.h \
   src/include/Http.h
src_lib_libhttp_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_libhttp_la_LDFLAGS = $(POMMEDAPI_LIBS)

src_lib_libtemplate_la_SOURCES = \
   src/lib/template/template_main.c \
   src/lib/template/template_private.h \
   src/include/Template.h
src_lib_libtemplate_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_libtemplate_la_LDFLAGS = $(POMMEDAPI_LIBS)

src_lib_libtest_la_SOURCES = \
   src/lib/test/test_main.c \
   src/lib/test/test_private.h \
   src/lib/test/test_run.c \
   src/lib/test/test_utils.c \
   src/include/Test.h
src_lib_libtest_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_libtest_la_LDFLAGS = $(POMMEDAPI_LIBS)
src_lib_libtest_la_LIBADD = \
   src/lib/libgfile.la

src_lib_librendering_la_SOURCES = \
   src/lib/rendering/rendering_main.c \
   src/lib/rendering/rendering_test.c \
   src/lib/rendering/rendering_copy.c \
   src/lib/rendering/rendering_utils.c \
   src/lib/rendering/rendering_private.h \
   src/include/Rendering.h
src_lib_librendering_la_CFLAGS = $(POMMEDAPI_CFLAGS) $(AM_CPPFLAGS)
src_lib_librendering_la_LDFLAGS = $(POMMEDAPI_LIBS)
src_lib_librendering_la_LIBADD = \
   src/lib/libtest.la \
   src/lib/libtemplate.la \
   src/lib/libgfile.la
