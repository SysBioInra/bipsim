#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# initial information about the project
AC_PREREQ([2.63])
AC_INIT([BiPSim], [1.0])

# automake initialization
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])

# check if the source folder is correct
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Check for command line options
# Checks for command-line options
AC_ARG_ENABLE([pre-check],
	[ --enable-pre-check	enable precondition checks],
	[pre_check=${enableval}], [pre_check=no])

AC_ARG_ENABLE([post-check],
	[ --enable-post-check	enable postcondition checks],
	[post_check=${enableval}], [post_check=no])

AC_ARG_ENABLE([warnings],
	[ --enable-warnings	enable warning display],
	[warnings=${enableval}], [warnings=no])

AC_ARG_ENABLE([boost],
	[ --enable-boost	enable use of boost libraries],
	[boost=${enableval}], [boost=yes])

AC_ARG_ENABLE([unit-tests],
	[ --enable-unit-tests	enable unit tests],
	[unit_tests=${enableval}], [unit_tests=no])

AC_ARG_ENABLE([integration-tests],
	[ --enable-integration-tests	enable integration tests],
	[integration_tests=${enableval}], [integration_tests=no])

AC_ARG_ENABLE([python-binding],
	[ --enable-python-binding	enable python binding],
	[python_binding=${enableval}], [python_binding=no])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for libraries.
# Boost
AM_CONDITIONAL([HAVE_BOOST], [test "x${boost}" = xyes])
AM_CONDITIONAL([HAVE_UNIT_TESTS], [test "x${unit_tests}" = xyes])
AM_CONDITIONAL([HAVE_INTEGRATION_TESTS], [test "x${integration_tests}" = xyes])
AM_CONDITIONAL([HAVE_PYTHON_BINDING], [test "x${python_binding}" = xyes])

AM_COND_IF([HAVE_BOOST],
	   [AX_BOOST_BASE(
		 [1.41],
		 [],
		 [AC_MSG_ERROR([Boost was not found in your system or is outdated, consider configuring without boost functionalities (option --disable-boost).])]
		 )])
# AM_COND_IF([HAVE_BOOST],[AX_BOOST_SERIALIZATION])
if test "x${boost}" = xyes; then
	if test "x${unit_tests}" = xyes; then
		AX_BOOST_UNIT_TEST_FRAMEWORK
	fi
	if test "x${python_binding}" = xyes; then
		AX_BOOST_PYTHON
	fi
fi

# add approriate define flags
if test "x${pre_check}" = xyes; then
  AC_DEFINE([CHECK_PRECONDITIONS], 1, [Precondition check enabled])
fi

if test "x${post_check}" = xyes; then
  AC_DEFINE([CHECK_POSTCONDITIONS], 1, [Postcondition check enabled])
fi

if test "x${unit_tests}" = xyes; then
   AC_DEFINE([PERFORM_TESTS], 1, [Tests enabled])
fi

if test "x${integration_tests}" = xyes; then
   AC_DEFINE([PERFORM_TESTS], 1, [Tests enabled])
fi

if test "x${warnings}" = xyes; then
   AC_DEFINE([DISPLAY_WARNINGS], 1, [Display warning messages])
fi

# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile
                 src/Makefile
		 tests/Makefile
		 tests/unit_tests/Makefile
		 tests/integration/Makefile
		 python/Makefile])

# generate the final Makefile etc.
AC_OUTPUT
