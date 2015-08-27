
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
template <typename T> class BiasedWheel;
class IdentifiedList;
class RandomHandler;

// reaction classes
class BaseLoading;
class Binding;
class ChemicalReaction;
class Complexation;
class Elongation;
class Reaction;
class Release;

// chemical entity classes
class BaseLoader;
class BindingSiteFamily;
class BindingSite;
class BindingSiteObserver;
class BoundChemical;
class BoundUnit;
class Chemical;
class ChemicalSequence;
class DecodingTable;
class ProcessiveChemical;
class Reactant;
class Site;
class SiteLocation;
class SiteObserver;
class SiteAvailability;

// parser and handler classes
class BindingSiteHandler;
class ChemicalHandler;
class Parser;
class ReactionHandler;
class SiteHandler;
class TableHandler;
class TerminationSiteHandler;

// solver classes
class CellState;
class ConstantRateGroup;
class DependencyGraph;
class NaiveSolver;
class ManualDispatchSolver;
class ReactionGroup;
class ReactionGroupEvent;
class ReactionClassification;
class Solver;
class UpdatedRateGroup;

// rate related classes
class RateManager;
class NaiveRateManager;
class DependencyRateManager;

#endif // FORWARDDECLARATIONS_H
