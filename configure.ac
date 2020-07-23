#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([simppru], [1.0], [https://github.com/VedantParanjape/simppru/issues])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_PROG_RANLIB
AM_PROG_AR
AC_PROG_YACC
AC_PROG_LEX

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h libintl.h limits.h locale.h netdb.h stddef.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2 gethostbyaddr gethostbyname localeconv memmove memset pow select socket sqrt strchr strdup strerror strndup strstr strtol])

AC_CONFIG_FILES([Makefile
                 third-party/cJSON/Makefile
                 third-party/vec/Makefile
                 src/Makefile])
AC_OUTPUT
	
