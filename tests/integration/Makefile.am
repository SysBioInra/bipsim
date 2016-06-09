
#########################
#         FLAGS         #
#########################
#
AM_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/src
AM_LDFLAGS = $(BOOST_LDFLAGS)

#########################
# THE PROGRAMS TO BUILD #
#########################
#
# the programs to build (the names of the final binaries)
check_PROGRAMS =
check_PROGRAMS += tRNA_consumption/test missing_tRNA/test

# declare scripts to be used by `make check`
TESTS =
TESTS += tRNA_consumption/run_test.sh missing_tRNA/run_test.sh

# we use the convenience library created in src to link to the simulator
tRNA_consumption_test_SOURCES = tRNA_consumption/test.cpp
tRNA_consumption_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libsrc.a

missing_tRNA_test_SOURCES = missing_tRNA/test.cpp
missing_tRNA_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libsrc.a

