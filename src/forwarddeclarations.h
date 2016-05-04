
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
class RandomHandler;
class ChemicalLogger;
class DoubleStrandLogger;
#ifdef HAVE_BOOST_SERIALIZATION
namespace boost { namespace serialization { class access; }}
#endif // HAVE_BOOST_SERIALIZATION

// factories
class BoundUnitFactory;
class SolverFactory;
class NaiveSolverFactory;
class ManualDispatchFactory;
class RateManagerFactory;
class NaiveRateManagerFactory;
class DependencyRateManagerFactory;
class RateContainerFactory;
class RateVectorFactory;
class RateTreeFactory;
class HybridRateContainerFactory;

// reaction classes
class BackwardReaction;
class BidirectionalReaction;
class ChemicalReaction;
class DoubleStrandLoading;
class DoubleStrandRecruitment;
class ForwardReaction;
class Loading;
class ProductLoading;
class Reaction;
class Release;
class SequenceBinding;
class Translocation;

// chemical entity classes
class Reactant;
class BoundChemical;
class BoundUnit;
class BoundUnitFilter;
class FamilyFilter;
class TemplateFilter;
class Chemical;
class FreeChemical;

// site classes
class BindingSite;
class BindingSiteFamily;
class FreeEndBindingSiteFactory;
class FreeEndHandler;
class Site;
class SiteFamily;

// sequence related classes
class ChemicalSequence;
class DoubleStrand;
class PartialStrand;
class Segment;
class SequenceOccupation;
class SiteGroup;

// tables
class CompositionTable;
class LoadingTable;
class ProductTable;
class TransformationTable;

// parser, handler and builder classes
template <class T> class BaseToken;
class Builder;
class EventHandler;
template <class T> class Handler;
class InputData;
class InputLine;
class Interpreter;
class Parser;
class Rule;
class Simulation;
class SimulationParams;
class SimulatorInput;
class TagToken;

// event classes
class AddEvent;
class Event;
class RemoveEvent;
class SetEvent;

// solver classes
class CellState;
class ConstantRateGroup;
class NaiveSolver;
class ManualDispatchSolver;
class ReactionGroup;
class ReactionClassification;
class Solver;
class UpdatedRateGroup;

// rate related classes
class RateManager;
class NaiveRateManager;
class DependencyRateManager;
class GraphRateManager;

class RateInvalidator;
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
template <class T> class VectorList;
template <class T> class VectorQueue;
template <class T> class VectorStack;

// exceptions
class DependencyException;
class FormatException;
class ParserException;


#endif // FORWARD_DECLARATIONS_H
