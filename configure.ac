#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.65)
AC_INIT(cbtl, 0.01, leandro.costa@gmail.com)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

CPPFLAGS='-g -Wall -Werror -O'
CFLAGS=
CXXFLAGS=

LDFLAGS='-lpthread'
INCLUDES='-I$(top_srcdir)/src -I$(top_srcdir)/src/tests/gtest/include'

AC_SUBST(LDFLAGS)
AC_SUBST(INCLUDES)

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 rmdir strcasecmp strchr strdup strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/examples/Makefile
                 src/tests/Makefile
                 src/tests/cbt/Makefile])
AC_OUTPUT
