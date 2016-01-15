
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

# convenience library containing all the source files (used by unit tests)
noinst_LIBRARIES = libsrc.a

# the program to build (the names of the final binaries)
bin_PROGRAMS = simulator

# list of sources for the 'simulator' binary
simulator_SOURCES = main.cpp
simulator_LDADD = libsrc.a

# list of sources for 'libsrc.la' convenience library
# main file and utility files
libsrc_a_SOURCES = \
	macros.h forwarddeclarations.h \
	biasedwheel.cpp biasedwheel.h \
	randomhandler.cpp randomhandler.h \
	observable.h \
	chemicallogger.cpp chemicallogger.h

# factories
libsrc_a_SOURCES += \
	solverfactory.h ratemanagerfactory.h ratecontainerfactory.h

# reaction sources
libsrc_a_SOURCES += \
	backwardreaction.h bidirectionalreaction.h \
	chemicalreaction.cpp chemicalreaction.h \
	complexation.cpp complexation.h \
	forwardreaction.h \
	loading.cpp loading.h \
	reaction.cpp reaction.h \
	release.cpp release.h \
	sequencebinding.cpp sequencebinding.h \
	translocation.cpp translocation.h

# chemical entity sources
libsrc_a_SOURCES += \
	loader.cpp loader.h \
	boundchemical.cpp boundchemical.h \
	boundunit.h boundunitlist.h \
	chemical.h \
	processivechemical.cpp processivechemical.h 

# site sources
libsrc_a_SOURCES += \
	bindingsite.h \
	bindingsitefamily.cpp bindingsitefamily.h \
	bindingsiteobserver.h \
	site.cpp site.h \
	sitefamily.h \
	siteobserver.h

# sequence related sources
libsrc_a_SOURCES += \
	chemicalsequence.cpp chemicalsequence.h \
	sequenceoccupation.cpp sequenceoccupation.h \
	sitelocation.h \
	siteavailability.cpp siteavailability.h \
	watchergroup.cpp watchergroup.h

# table sources
libsrc_a_SOURCES += \
	compositiontable.cpp compositiontable.h \
	decodingtable.cpp decodingtable.h \
	producttable.cpp producttable.h \
	transformationtable.cpp transformationtable.h

# parser, handler and factory sources
libsrc_a_SOURCES += \
	eventhandler.cpp eventhandler.h \
	eventfactory.cpp eventfactory.h event.h \
	factory.h \
	handler.h \
	inputdata.cpp inputdata.h \
	parser.cpp parser.h \
	reactionfactory.cpp reactionfactory.h \
	simulation.cpp simulation.h \
	simulationparams.cpp simulationparams.h \
	simulatorinput.h \
	unitfactory.cpp unitfactory.h

# solver sources
libsrc_a_SOURCES += \
	cellstate.cpp cellstate.h \
	constantrategroup.cpp constantrategroup.h \
	dependencygraph.cpp dependencygraph.h \
	manualdispatchsolver.cpp manualdispatchsolver.h \
	naivesolver.cpp naivesolver.h \
	reactiongroup.cpp reactiongroup.h \
	reactiongroupevent.h \
	reactionclassification.cpp reactionclassification.h \
	solver.cpp solver.h \
	updatedrategroup.cpp updatedrategroup.h

# rate related classes
libsrc_a_SOURCES += \
	ratemanager.cpp ratemanager.h \
	naiveratemanager.cpp naiveratemanager.h \
	dependencyratemanager.cpp dependencyratemanager.h \
	graphratemanager.cpp graphratemanager.h \
	concentrationobserver.h \
	reactionobserver.cpp reactionobserver.h \
	ratevalidity.h

# rate containers
libsrc_a_SOURCES += \
	ratecontainer.h ratevector.h updatedtotalratevector.h \
	flyratevector.h \
	ratetree.cpp ratetree.h \
	ratenode.h sumnode.h dummynode.h reactionnode.h \
	hybridratecontainer.cpp hybridratecontainer.h  \
	rategroup.h ratetoken.h

# generic containers
libsrc_a_SOURCES += \
	vectorqueue.h vectorstack.h

# exceptions
libsrc_a_SOURCES += \
	formatexception.h parserexception.h dependencyexception.h

###############################################################################
