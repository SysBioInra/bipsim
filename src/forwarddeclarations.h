
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
class ChemicalLogger;
#ifdef HAVE_BOOST_SERIALIZATION
namespace boost { namespace serialization { class access; }}
#endif // HAVE_BOOST_SERIALIZATION

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
class Site;
class SiteLocation;
class SiteObserver;
class SiteAvailability;

// parser and handler classes
class BindingSiteHandler;
class ChemicalHandler;
class EventHandler;
class EventParser;
class Parser;
class ReactionHandler;
class Simulation;
class SiteHandler;
class TableHandler;
class TerminationSiteHandler;

// event classes
class AddEvent;
class Event;
class RemoveEvent;
class SetEvent;

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
class GraphRateManager;
class ConcentrationObserver;
class ReactionObserver;

// template classes
template <class T> class Observable;
/**
 * @brief Class representing any entity that can be involved in a reaction.
 *
 * Reactant mainly provides an interface for handling reaction rate updates.
 * The type inheritance allows to list all components of a reaction as a list
 * of Reactant, while the implementation inheritance provides a way to communicate
 * with a DependencyRateManager automatically whenever a change in concentration occurs.
 */
typedef Observable<ConcentrationObserver> Reactant;


#endif // FORWARDDECLARATIONS_H
