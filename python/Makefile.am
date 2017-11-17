
#########################
#         FLAGS         #
#########################
#
AM_CPPFLAGS = -I$(top_srcdir)/src -fPIC $(BOOST_CPPFLAGS) $(PYTHON_CPPFLAGS)
AM_LDFLAGS =

#########################
# THE PROGRAMS TO BUILD #
#########################
#
# the programs to build (the names of the final binaries)
lib_LTLIBRARIES = bipsim.la
bipsim_la_LDFLAGS = -module -shared -avoid-version $(BOOST_LDFLAGS) -l$(BOOST_PYTHON_LIB) $(PYTHON_LIBS)
bipsim_la_SOURCES = export_bipsim.cpp
bipsim_la_LIBADD = $(top_srcdir)/src/libbipsim.la
