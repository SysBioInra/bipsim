
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

# library containing all the source files
noinst_LTLIBRARIES = libbipsim.la

# the program to build (the names of the final binaries)
bin_PROGRAMS = bipsim

# list of sources for the 'bipsim' binary
bipsim_SOURCES = main.cpp
bipsim_LDADD = libbipsim.la

# list of sources for 'libbipsim.la' library
# utility files
libbipsim_la_SOURCES = \
	biasedwheel.cpp randomhandler.cpp chemicallogger.cpp \
	reactionlogger.cpp doublestrandlogger.cpp

# factories
libbipsim_la_SOURCES += boundunitfactory.cpp

# reaction sources
libbipsim_la_SOURCES += \
	chemicalreaction.cpp doublestrandrecruitment.cpp loading.cpp \
	release.cpp sequencebinding.cpp translocation.cpp

# chemical entity sources
libbipsim_la_SOURCES += \
	boundchemical.cpp boundunit.cpp familyfilter.cpp templatefilter.cpp

# site sources
libbipsim_la_SOURCES += bindingsite.cpp bindingsitefamily.cpp

# sequence related sources
libbipsim_la_SOURCES += \
	chemicalsequence.cpp doublestrand.cpp partialstrand.cpp \
	sequenceoccupation.cpp sitegroup.cpp

# table sources
libbipsim_la_SOURCES += \
	compositiontable.cpp loadingtable.cpp transformationtable.cpp

# parser, handler and builder sources
libbipsim_la_SOURCES += \
	eventhandler.cpp eventbuilder.cpp inputdata.cpp parser.cpp \
	reactantbuilder.cpp reactionbuilder.cpp simulation.cpp \
	simulationparams.cpp tablebuilder.cpp

# solver sources
libbipsim_la_SOURCES += \
	cellstate.cpp constantrategroup.cpp manualdispatchsolver.cpp \
	naivesolver.cpp reactionclassification.cpp solver.cpp \
	updatedrategroup.cpp

# rate related classes
libbipsim_la_SOURCES += \
	ratemanager.cpp naiveratemanager.cpp dependencyratemanager.cpp \
	ratevalidity.cpp

# rate containers
libbipsim_la_SOURCES += ratetree.cpp hybridratecontainer.cpp


# headers
# utility files
include_HEADERS = \
	biasedwheel.h randomhandler.h logger.h chemicallogger.h \
	reactionlogger.h doublestrandlogger.h forwarddeclarations.h

# factories
include_HEADERS += \
	solverfactory.h ratemanagerfactory.h ratecontainerfactory.h \
	boundunitfactory.h

# reaction sources
include_HEADERS += \
	backwardreaction.h bidirectionalreaction.h chemicalreaction.h \
	doublestrandrecruitment.h forwardreaction.h loading.h reaction.h \
	release.h sequencebinding.h translocation.h

# chemical entity sources
include_HEADERS += \
	boundchemical.h boundunit.h boundunitlist.h boundunitfilter.h \
	familyfilter.h templatefilter.h freechemical.h chemical.h reactant.h

# site sources
include_HEADERS += bindingsite.h bindingsitefamily.h switch.h

# sequence related sources
include_HEADERS += \
	chemicalsequence.h doublestrand.h partialstrand.h \
	sequenceoccupation.h sitegroup.h

# table sources
include_HEADERS += \
	compositiontable.h loadingtable.h producttable.h transformationtable.h

# parser, handler and builder sources
include_HEADERS += \
	builder.h eventhandler.h eventbuilder.h event.h \
	inputline.h interpreter.h handler.h inputdata.cpp inputdata.h \
	parser.h reactantbuilder.h reactionbuilder.h simulation.h \
	simulationparams.h simulatorinput.h tablebuilder.h

# solver sources
include_HEADERS += \
	cellstate.h constantrategroup.h manualdispatchsolver.h \
	naivesolver.cpp naivesolver.h reactiongroup.h reactionclassification.h \
	solver.h updatedrategroup.h

# rate related classes
include_HEADERS += \
	ratemanager.h naiveratemanager.h dependencyratemanager.h \
	rateinvalidator.h ratevalidity.h

# rate containers
include_HEADERS += \
	ratecontainer.h ratevector.h updatedtotalratevector.h \
	flyratevector.h ratetree.h ratenode.h hybridratecontainer.h  \
	rategroup.h ratetoken.h

# generic containers
include_HEADERS += vectorqueue.h

# exceptions
include_HEADERS += simulatorexception.h

###############################################################################
