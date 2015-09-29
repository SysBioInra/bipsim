
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = simulator

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)
 
# list of sources for the 'simulator' binary
# main file and utility files
simulator_SOURCES = \
	main.cpp macros.h forwarddeclarations.h \
	biasedwheel.cpp biasedwheel.h \
	randomhandler.cpp randomhandler.h \
	observable.h \
	chemicallogger.cpp chemicallogger.h

# reaction sources
simulator_SOURCES += \
	baseloading.cpp baseloading.h \
	binding.cpp binding.h \
	chemicalreaction.cpp chemicalreaction.h \
	complexation.cpp complexation.h \
	elongation.cpp elongation.h \
	reaction.cpp reaction.h \
	release.cpp release.h

# chemical entity sources
simulator_SOURCES += \
	baseloader.cpp baseloader.h \
	bindingsite.cpp bindingsite.h \
	bindingsitefamily.cpp bindingsitefamily.h \
	bindingsiteobserver.cpp bindingsiteobserver.h \
	boundchemical.cpp boundchemical.h \
	boundunit.h \
	chemical.cpp chemical.h \
	chemicalsequence.cpp chemicalsequence.h \
	decodingtable.cpp decodingtable.h \
	processivechemical.cpp processivechemical.h \
	site.cpp site.h \
	sitefamily.h \
	sitelocation.cpp sitelocation.h \
	siteobserver.h \
	siteavailability.cpp siteavailability.h

# parser, handler and factory sources
simulator_SOURCES += \
	eventhandler.cpp eventhandler.h \
	eventfactory.cpp eventfactory.h \
	factory.h \
	handler.h \
	inputdata.cpp inputdata.h \
	parser.cpp parser.h \
	reactionfactory.cpp reactionfactory.h \
	simulation.cpp simulation.h \
	simulatorinput.h \
	unitfactory.cpp unitfactory.h

# event sources
simulator_SOURCES += \
	addevent.cpp addevent.h \
	event.cpp event.h \
	removeevent.cpp removeevent.h \
	setevent.cpp setevent.h

# solver sources
simulator_SOURCES += \
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
simulator_SOURCES += \
	ratemanager.cpp ratemanager.h \
	naiveratemanager.cpp naiveratemanager.h \
	dependencyratemanager.cpp dependencyratemanager.h \
	graphratemanager.cpp graphratemanager.h \
	concentrationobserver.cpp concentrationobserver.h \
	reactionobserver.cpp reactionobserver.h


###############################################################################
