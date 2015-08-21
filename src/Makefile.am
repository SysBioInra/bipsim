
###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = test

# list of sources for the 'test' binary
# main file and utility files
test_SOURCES = \
	main.cpp macros.h forwarddeclarations.h \
	identifiedlist.cpp identifiedlist.h \
	randomhandler.cpp randomhandler.h

# reaction sources
test_SOURCES += \
	baseloading.cpp baseloading.h \
	binding.cpp binding.h \
	chemicalreaction.cpp chemicalreaction.h \
	complexation.cpp complexation.h \
	elongation.cpp elongation.h \
	reaction.cpp reaction.h \
	release.cpp release.h

# chemical entity sources
test_SOURCES += \
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
	sitelocation.cpp sitelocation.h \
	siteobserver.h \
	siteavailability.cpp siteavailability.h

# parser and handler sources
test_SOURCES += \
	bindingsitehandler.cpp bindingsitehandler.h \
	chemicalhandler.cpp chemicalhandler.h \
	parser.cpp parser.h \
	reactionhandler.cpp reactionhandler.h \
	sitehandler.cpp sitehandler.h \
	tablehandler.cpp tablehandler.h \
	terminationsitehandler.cpp terminationsitehandler.h

# solver sources
test_SOURCES += \
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
test_SOURCES += \
	ratemanager.cpp ratemanager.h \
	naiveratemanager.cpp naiveratemanager.h \
	dependencyratemanager.cpp dependencyratemanager.h


###############################################################################
