
/*! \file forwarddeclarations.h
 * \authors Marc Dinh, Stephan Fischer
 * \brief Contains forward declarations of all project classes.
 * 
 * This file contains all forward declarations of type class
 * ExampleClass;. It should be included in every project class, 
 * it allows for mutual inclusions (A is an attribute of B and B an attribute
 * of A) in a clean way without the need for additional code.
 */


#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H


// utility classes
template <typename T> class BiasedWheel;
template <class T> class Observable;
class RandomHandler;
class ChemicalLogger;
#ifdef HAVE_BOOST_SERIALIZATION
namespace boost { namespace serialization { class access; }}
#endif // HAVE_BOOST_SERIALIZATION

// factories
class SolverFactory;
class NaiveSolverFactory;
class ManualDispatchFactory;
class RateManagerFactory;
class NaiveRateManagerFactory;
class DependencyRateManagerFactory;
class GraphRateManagerFactory;
class RateContainerFactory;
class RateVectorFactory;
class RateTreeFactory;
class HybridRateContainerFactory;

// reaction classes
class BackwardReaction;
class BidirectionalReaction;
class ChemicalReaction;
class Complexation;
class ForwardReaction;
class Loading;
class Reaction;
class Release;
class SequenceBinding;
class Translocation;

// chemical entity classes
class Loader;
class BoundChemical;
class BoundUnit;
class BoundUnitList;
class Chemical;
class FreeChemical;
class ProcessiveChemical;

// site classes
class BindingSite;
class BindingSiteFamily;
class BindingSiteObserver;
class Site;
class SiteFamily;
class SiteObserver;

// sequence related classes
class ChemicalSequence;
class Segment;
class SequenceOccupation;
class SiteAvailability;
class SiteLocation;
class WatcherGroup;

// tables
class CompositionTable;
class DecodingTable;
class ProductTable;
class TransformationTable;

// parser, handler and factory classes
class EventHandler;
class EventFactory;
class Factory;
template <class T> class Handler;
class InputData;
class Parser;
class ReactionFactory;
class Simulation;
class SimulationParams;
class SimulatorInput;
class UnitFactory;

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
class RateValidity;

// rate containers
class RateContainer;
class FlyRateVector;
class RateVector;
class UpdatedTotalRateVector;

class RateTree;
class RateNode;
class SumNode;
class DummyNode;
class ReactionNode;

class HybridRateContainer;
class RateGroup;
class RateToken;

// generic containers
template <class T> class VectorQueue;
template <class T> class VectorStack;

// exceptions
class DependencyException;
class FormatException;
class ParserException;

// template typedefs
/**
 * @brief Class representing any entity that can be involved in a reaction.
 *
 * Reactant mainly provides an interface for handling reaction rate updates.
 * The type inheritance allows to list all components of a reaction as a list
 * of Reactant, while the implementation inheritance provides a way to communicate
 * with a DependencyRateManager automatically whenever a change in concentration occurs.
 */
typedef Observable<ConcentrationObserver> Reactant;

#endif // FORWARD_DECLARATIONS_H
