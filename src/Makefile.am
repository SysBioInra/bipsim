
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
	logger.h \
	chemicallogger.cpp chemicallogger.h \
	reactionlogger.cpp reactionlogger.h \
	doublestrandlogger.cpp doublestrandlogger.h

# factories
libsrc_a_SOURCES += \
	solverfactory.h ratemanagerfactory.h ratecontainerfactory.h \
	boundunitfactory.cpp boundunitfactory.h

# reaction sources
libsrc_a_SOURCES += \
	backwardreaction.h bidirectionalreaction.h \
	chemicalreaction.cpp chemicalreaction.h \
	doublestrandrecruitment.cpp doublestrandrecruitment.h \
	forwardreaction.h \
	loading.cpp loading.h \
	reaction.h \
	release.cpp release.h \
	sequencebinding.cpp sequencebinding.h \
	translocation.cpp translocation.h

# chemical entity sources
libsrc_a_SOURCES += \
	boundchemical.cpp boundchemical.h \
	boundunit.cpp boundunit.h \
	boundunitfilter.h \
	familyfilter.cpp familyfilter.h \
	templatefilter.cpp templatefilter.h \
	freechemical.h chemical.h reactant.h

# site sources
libsrc_a_SOURCES += \
	bindingsite.cpp bindingsite.h \
	bindingsitefamily.cpp bindingsitefamily.h \
	switch.h

# sequence related sources
libsrc_a_SOURCES += \
	chemicalsequence.cpp chemicalsequence.h \
	doublestrand.cpp doublestrand.h \
	partialstrand.cpp partialstrand.h \
	sequenceoccupation.cpp sequenceoccupation.h \
	sitegroup.cpp sitegroup.h 

# table sources
libsrc_a_SOURCES += \
	compositiontable.cpp compositiontable.h \
	loadingtable.cpp loadingtable.h \
	producttable.h \
	transformationtable.cpp transformationtable.h

# parser, handler and builder sources
libsrc_a_SOURCES += \
	builder.h \
	eventhandler.cpp eventhandler.h \
	eventbuilder.cpp eventbuilder.h event.h \
	inputline.h interpreter.h \
	handler.h \
	inputdata.cpp inputdata.h \
	parser.cpp parser.h \
	reactantbuilder.cpp reactantbuilder.h \
	reactionbuilder.cpp reactionbuilder.h \
	simulation.cpp simulation.h \
	simulationparams.cpp simulationparams.h \
	simulatorinput.h \
	tablebuilder.cpp tablebuilder.h 

# solver sources
libsrc_a_SOURCES += \
	cellstate.cpp cellstate.h \
	constantrategroup.cpp constantrategroup.h \
	manualdispatchsolver.cpp manualdispatchsolver.h \
	naivesolver.cpp naivesolver.h \
	reactiongroup.h \
	reactionclassification.cpp reactionclassification.h \
	solver.cpp solver.h \
	updatedrategroup.cpp updatedrategroup.h

# rate related classes
libsrc_a_SOURCES += \
	ratemanager.cpp ratemanager.h \
	naiveratemanager.cpp naiveratemanager.h \
	dependencyratemanager.cpp dependencyratemanager.h \
	rateinvalidator.h \
	ratevalidity.cpp ratevalidity.h

# rate containers
libsrc_a_SOURCES += \
	ratecontainer.h ratevector.h updatedtotalratevector.h \
	flyratevector.h \
	ratetree.cpp ratetree.h ratenode.h \
	hybridratecontainer.cpp hybridratecontainer.h  \
	rategroup.h ratetoken.h

# generic containers
libsrc_a_SOURCES += \
	vectorqueue.h vectorlist.h

# exceptions
libsrc_a_SOURCES += simulatorexception.h

###############################################################################
