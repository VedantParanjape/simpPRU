#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libprurpmsg], [1.0], [https://github.com/VedantParanjape/simppru/issues])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([pru_virtqueue.c])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
cross_compiling = **yes**

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
