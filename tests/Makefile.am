
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################


# the program to build (the names of the final binaries)
bin_PROGRAMS = biasedwheel_test inputdata_test randomhandler_test

TESTS = $(bin_PROGRAMS)

basic_files = ../src/macros.h ../src/forwarddeclarations.h
biasedwheel_files = \
	../src/biasedwheel.cpp ../src/biasedwheel.h $(basic_files)
inputdata_files = \
	../src/inputdata.cpp ../src/inputdata.h $(basic_files)
randomhandler_files = \
	../src/randomhandler.cpp ../src/randomhandler.h $(biasedwheel_files)

biasedwheel_test_SOURCES = biasedwheel_test.cpp $(biasedwheel_files)
inputdata_test_SOURCES = inputdata_test.cpp $(inputdata_files)
randomhandler_test_SOURCES = randomhandler_test.cpp $(randomhandler_files)


###############################################################################
