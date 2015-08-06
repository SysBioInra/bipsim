

/**
 * @file solver.h
 * @brief Header for the Solver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SOLVER_H
#define SOLVER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list
#include <map> // std::map
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "dependencygraph.h"

/**
 * @brief Solver class for integrating reactions.
 *
 * The Solver class is a pure abstract class storing a list of reactions,
 * computing their rate and performing them in a random order as specified
 * by classes inheriting the Solver class.
 */
class Solver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param initial_time Initial simulation time.
   * @param cell_state Reference to a cell state describing current state and reactions within the cell.
   */
  Solver (double initial_time, CellState& cell_state);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Solver ( const Solver& other_solver );

  /**
   * @brief Destructor
   */
  virtual ~Solver (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update system according to the reaction system during given time step.
   * @param time_step Time during which the reactions should be integrated.
   */
  void solve (double time_step);

  /**
   * @brief Update system according to the reaction system until a reaction happens.
   */
  void go_to_next_reaction (void);
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to the simulation time.
   * @return Simulation time.
   */
  double time (void) const;

  /**
   * @brief Accessor to the number of reactions that have occurred.
   * @return Number of reactions that have occurred from the beginning of simulation.
   */
  int number_reactions_performed (void) const;  
  

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // Solver& operator= ( const Solver& other_solver );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

 protected:
  
  // ============
  //  Attributes
  // ============
  //
  /** @brief State of the cell. */
  CellState& _cell_state;

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Compute next reaction of the system.
   * 
   * The computation is actually done by classes inheriting
   * from Solver.
   * @return Time at which the reaction occurred.
   */
  virtual double compute_next_reaction (void) = 0;

  /**
   * @brief Accessor to the dependency graph between reactions (for use of inheriting classes).
   * 
   * @return Dependency graph built during construction of the class
   *  establishing which reaction should be updated.
   * @sa DependencyGraph
   */
  const DependencyGraph& dependency_graph (void) const;

  /**
   * @brief Accessor to the reactions (for use of inheriting classes).
   * 
   * @return Vector of reactions composing the system to integrate.
   */
  const std::vector<Reaction*>& reactions (void) const;
  

 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector of reactions to integrate. */
  std::vector< Reaction* > _reactions;

  /** @brief Dependency graph between relations. */
  DependencyGraph _dependency_graph;

  /** @brief Simulation time. */
  double _t;

  /** @brief Number of reactions that have been performed. */
  int _number_reactions_performed;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline double Solver::time (void) const 
{
  return _t;
}

inline int Solver::number_reactions_performed (void) const 
{
  return _number_reactions_performed;
}

inline const DependencyGraph& Solver::dependency_graph (void) const
{
  return _dependency_graph;
}

inline const std::vector<Reaction*>& Solver::reactions (void) const
{
  return _reactions;
}

#endif // SOLVER_H
