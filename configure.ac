##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##--##
m4_define([v_maj], [1])
m4_define([v_min], [1])
m4_define([v_mic], [2])

m4_define([v_ver], [v_maj.v_min.v_mic])

AC_PREREQ(2.5)
AC_INIT([pommedapi], [v_ver], [guillaume.friloux@amtrust.fr])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl

AC_DEFINE_UNQUOTED(VMAJ, [v_maj], [Major version])
AC_DEFINE_UNQUOTED(VMIN, [v_min], [Minor version])
AC_DEFINE_UNQUOTED(VMIC, [v_mic], [Micro version])

AC_PROG_LIBTOOL
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_MAKE_SET

PKG_PROG_PKG_CONFIG

AC_CONFIG_FILES([
Makefile
])

AM_CONFIG_HEADER([config.h])

POMMEDAPI_REQUIRES="eina ecore ecore-con eio azy"
PKG_CHECK_MODULES(POMMEDAPI, [$POMMEDAPI_REQUIRES], [build_pommedapi=yes], [build_pommedapi=no])

AC_OUTPUT([])

echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE_NAME $PACKAGE_VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "Build options :"
echo "  CFLAGS.......: $CFLAGS"
echo "  LDFLAGS......: $LDFLAGS"
if test "x${build_pommedapi}" == "xyes" ; then
  echo "You can now type make to build the project"
else
  echo "You're missing dependencies to build the project"
fi
