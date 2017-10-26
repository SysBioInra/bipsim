
#########################
#         FLAGS         #
#########################
#
AM_CPPFLAGS = -I$(top_srcdir)/src $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

#########################
# THE PROGRAMS TO BUILD #
#########################
#
# the programs to build (the names of the final binaries)
check_PROGRAMS =
check_PROGRAMS += biasedwheel_test inputdata_test randomhandler_test transformationtable_test producttable_test cellstate_test compositiontable_test ratetree_test rategroup_test hybridratecontainer_test partialstrand_test sitegroup_test sequenceoccupation_test freechemical_test boundchemical_test chemicalreaction_test boundunitlist_test

# declare programs to be part of `make check`
TESTS = $(check_PROGRAMS)

# we use the library created in src to link to the simulator
biasedwheel_test_SOURCES = biasedwheel_test.cpp
biasedwheel_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

inputdata_test_SOURCES = inputdata_test.cpp
inputdata_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

randomhandler_test_SOURCES = randomhandler_test.cpp
randomhandler_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

transformationtable_test_SOURCES = transformationtable_test.cpp
transformationtable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

producttable_test_SOURCES = producttable_test.cpp
producttable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

cellstate_test_SOURCES = cellstate_test.cpp
cellstate_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

compositiontable_test_SOURCES = compositiontable_test.cpp
compositiontable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

ratetree_test_SOURCES = ratetree_test.cpp
ratetree_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

rategroup_test_SOURCES = rategroup_test.cpp
rategroup_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

hybridratecontainer_test_SOURCES = hybridratecontainer_test.cpp
hybridratecontainer_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

partialstrand_test_SOURCES = partialstrand_test.cpp
partialstrand_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

sitegroup_test_SOURCES = sitegroup_test.cpp
sitegroup_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

sequenceoccupation_test_SOURCES = sequenceoccupation_test.cpp
sequenceoccupation_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

freechemical_test_SOURCES = freechemical_test.cpp
freechemical_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

boundchemical_test_SOURCES = boundchemical_test.cpp
boundchemical_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

chemicalreaction_test_SOURCES = chemicalreaction_test.cpp
chemicalreaction_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la

boundunitlist_test_SOURCES = boundunitlist_test.cpp
boundunitlist_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libbipsim.la
