
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


// utility classes
class IdentifiedList;
class RandomHandler;

// reaction classes
class BaseLoading;
class Binding;
class ChemicalReaction;
class Complexation;
class Elongation;
class ProcessiveChemical;
class Reaction;
class Release;

// chemical entity classes
class BaseLoader;
class BindingSite;
class BoundChemical;
class BoundUnit;
class Chemical;
class ChemicalSequence;
class DecodingTable;
class Site;
class SiteLocation;

// parser and handler classes
class BindingSiteHandler;
class ChemicalHandler;
class Parser;
class ReactionHandler;
class SiteHandler;
class TableHandler;
class TerminationSiteHandler;

// solver classes
class ConstantRateGroup;
class DependencyGraph;
class NaiveSolver;
class ReactionGroup;
class ReactionClassification;
class Solver;
class UpdatedRateGroup;

#endif // FORWARDDECLARATIONS_H
