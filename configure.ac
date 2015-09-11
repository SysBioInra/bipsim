#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# initial information about the project
AC_PREREQ([2.63])
AC_INIT([myBacteria], [1.0] )

# automake initialization
AM_INIT_AUTOMAKE

# check if the source folder is correct
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# Boost
AC_ARG_ENABLE([boost],
	[ --enable-boost	enable use of boost libraries],
	[boost=${enableval}], [boost=yes])
AM_CONDITIONAL([HAVE_BOOST], [test "x${boost}" = xyes])
AM_COND_IF([HAVE_BOOST],
	   [AX_BOOST_BASE([1.41],[],[AC_MSG_ERROR([Boost was not found in your system, consider configuring without boost functionalities.])])])

AM_COND_IF([HAVE_BOOST],[AX_BOOST_SERIALIZATION])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile
                 src/Makefile
		 tests/Makefile])

# Checks for command-line options
AC_ARG_ENABLE([pre-check],
	[ --enable-pre-check	enable precondition checks],
	[pre_check=${enableval}], [pre_check=no])

AC_ARG_ENABLE([post-check],
	[ --enable-post-check	enable postcondition checks],
	[post_check=${enableval}], [post_check=no])

AC_ARG_ENABLE([inv-check],
	[ --enable-inv-check	enable invariant checks],
	[inv_check=${enableval}], [inv_check=no])

if test "x${pre_check}" = xyes; then
   AC_DEFINE([CHECK_PRECONDITIONS], 1, [Precondition check enabled])
fi

if test "x${post_check}" = xyes; then
   AC_DEFINE([CHECK_POSTCONDITIONS], 1, [Postcondition check enabled])
fi

if test "x${inv_check}" = xyes; then
   AC_DEFINE([CHECK_INVARIANTS], 1, [Invariant check enabled])
fi

# generate the final Makefile etc.
AC_OUTPUT
