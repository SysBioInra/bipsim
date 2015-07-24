
/*! \file forwarddeclarations.h
 * \authors Marc Dinh, Stephan Fischer
 * \brief Contains forward declarations of all project classes.
 * 
 * This file contains all forward declarations of type class
 * ExampleClass;. It should be included in every project class, 
 * it allows for mutual inclusions (A is an attribute of B and B an attribute
 * of A) in a clean way without the need for additional code.
 */


#ifndef FORWARDDECLARATIONS_H
#define FORWARDDECLARATIONS_H

class BaseLoader;
class BaseLoading;
class Binding;
class BindingSite;
class BindingSiteHandler;
class BoundChemical;
class BoundUnit;
class Chemical;
class ChemicalHandler;
class ChemicalReaction;
class ChemicalSequence;
class Complexation;
class DecodingTable;
class DependencyGraph;
class Elongation;
class IdentifiedList;
class Parser;
class ProcessiveChemical;
class RandomHandler;
class Reaction;
class ReactionHandler;
class Release;
class Site;
class SiteHandler;
class SiteLocation;
class Solver;
class TableHandler;
class TerminationSiteHandler;

#endif // FORWARDDECLARATIONS_H
