ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects
AM_MAKEFLAGS = --no-print-directory

MAINTAINERCLEANFILES = \
missing \
m4/libtool.m4 \
m4/ltoptions.m4 \
m4/ltsugar.m4 \
m4/ltversion.m4 \
m4/lt~obsolete.m4 \
ltmain.sh \
depcomp \
configure \
config.sub \
config.h.in \
config.guess \
config.cache-env \
compile \
aclocal.m4 \
Makefile.in \
ABOUT-NLS \
install-sh \
test-driver \
test-suite.log


AM_CPPFLAGS = -DDATA_DIR=\"$(datadir)\" @CPPFLAGS@ \
-DSYSCONF_DIR=\"$(sysconfdir)\" \
-DBIN_DIR=\"$(bindir)\" \
-I$(top_srcdir) -I$(top_srcdir)/src/include -I$(top_srcdir)/src/lib/json \
-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -D_LARGEFILE_SOURCE=1 -D__USE_LARGEFILE64=1

bin_PROGRAMS =
dist_bin_SCRIPTS =
noinst_PROGRAMS =

CLEANFILES =
DISTCLEANFILES =
EXTRA_DIST =
BUILT_SOURCES =

include src/lib/Makefile.mk
include src/bin/Makefile.mk

maintainer-clean-local:
	rm -rf autom4te.cache

clean-local:
	find . -name "*.gc??" -exec rm -f {} \;
	@rm -rf $(top_builddir)/coverage $(top_builddir)/analysis
