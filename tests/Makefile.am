
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

# the programs to build (the names of the final binaries)
check_PROGRAMS = biasedwheel_test inputdata_test randomhandler_test \
	transformationtable_test producttable_test cellstate_test \
	compositiontable_test ratetree_test boundunitlist_test \
	rategroup_test hybridratecontainer_test partialstrand_test \
	sitegroup_test sequenceoccupation_test 

# declare programs to be part of `make check`
TESTS = $(check_PROGRAMS)

# we use the convenience library created in src to link to the simulator
biasedwheel_test_SOURCES = biasedwheel_test.cpp
biasedwheel_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

inputdata_test_SOURCES = inputdata_test.cpp
inputdata_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

producttable_test_SOURCES = producttable_test.cpp
producttable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

randomhandler_test_SOURCES = randomhandler_test.cpp
randomhandler_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

transformationtable_test_SOURCES = transformationtable_test.cpp
transformationtable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

cellstate_test_SOURCES = cellstate_test.cpp
cellstate_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

compositiontable_test_SOURCES = compositiontable_test.cpp
compositiontable_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

ratetree_test_SOURCES = ratetree_test.cpp
ratetree_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

boundunitlist_test_SOURCES = boundunitlist_test.cpp
boundunitlist_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

rategroup_test_SOURCES = rategroup_test.cpp
rategroup_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

hybridratecontainer_test_SOURCES = hybridratecontainer_test.cpp
hybridratecontainer_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

partialstrand_test_SOURCES = partialstrand_test.cpp
partialstrand_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

sitegroup_test_SOURCES = sitegroup_test.cpp
sitegroup_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

sequenceoccupation_test_SOURCES = sequenceoccupation_test.cpp
sequenceoccupation_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) ../src/libsrc.a

###############################################################################
