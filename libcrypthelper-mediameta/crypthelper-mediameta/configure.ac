AC_PREREQ(2.61)
AC_INIT([libcrypthelper_mediameta],1.0.0)
AM_INIT_AUTOMAKE([-Wall gnu foreign])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Check for programs
AM_PROG_AR
AM_PROG_AS
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
PKG_PROG_PKG_CONFIG

#basemachine
AC_MSG_CHECKING([which base machine to use])
AC_ARG_WITH([basemachine],
            [AS_HELP_STRING([--with-basemachine],
            [Specify the base machine])],
            [machine_selected="$withval"],
            [machine_selected=none])

AC_MSG_RESULT([$machine_selected])
AC_SUBST([MACHINE_ENABLED],[$machine_selected])

AC_CONFIG_FILES([ \
    Makefile \
    libs/libcrypthelper_mediameta.pc \
    libs/Makefile \
    tests/Makefile
])
AC_OUTPUT
