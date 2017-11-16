
#########################
#         FLAGS         #
#########################
#
AM_CPPFLAGS = -I$(top_srcdir)/src -fPIC $(BOOST_CPPFLAGS) $(PYTHON_INCLUDES)
AM_LDFLAGS = $(BOOST_LDFLAGS)

#########################
# THE PROGRAMS TO BUILD #
#########################
#
# the programs to build (the names of the final binaries)
lib_LTLIBRARIES = bipsim.la
bipsim_la_LDFLAGS = -module -avoid-version

# we use the library created in src to link to the simulator
bipsim_la_SOURCES = export_bipsim.cpp
bipsim_la_LIBADD = $(top_srcdir)/src/libbipsim.la -l$(BOOST_PYTHON_LIB)
