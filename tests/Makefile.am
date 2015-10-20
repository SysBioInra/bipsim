
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################


# the programs to build (the names of the final binaries)
check_PROGRAMS = biasedwheel_test inputdata_test randomhandler_test \
	transformationtable_test producttable_test cellstate_test \
	compositiontable_test

# declare programs to be part of `make check`
TESTS = $(check_PROGRAMS)

# we use the convenience library created in src to link to the simulator
biasedwheel_test_SOURCES = biasedwheel_test.cpp
biasedwheel_test_LDADD = ../src/libsrc.a

inputdata_test_SOURCES = inputdata_test.cpp
inputdata_test_LDADD = ../src/libsrc.a

producttable_test_SOURCES = producttable_test.cpp
producttable_test_LDADD = ../src/libsrc.a

randomhandler_test_SOURCES = randomhandler_test.cpp
randomhandler_test_LDADD = ../src/libsrc.a

transformationtable_test_SOURCES = transformationtable_test.cpp
transformationtable_test_LDADD = ../src/libsrc.a

cellstate_test_SOURCES = cellstate_test.cpp
cellstate_test_LDADD = ../src/libsrc.a

compositiontable_test_SOURCES = compositiontable_test.cpp
compositiontable_test_LDADD = ../src/libsrc.a

###############################################################################
