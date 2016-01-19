

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
   * @brief Constructor
   * @param params Simulation parameters.
   * @param cell_state Reference to a cell state describing current state and
   *  reactions within the cell.
   */
  Solver (const SimulationParams& params, CellState& cell_state);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Solver (const Solver& other_solver);
  // /* @brief Assignment operator. */
  // Solver& operator= (const Solver& other_solver);
  /** @brief Destructor. */
  virtual ~Solver (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update system according to the reaction system during given time 
   *  step.
   * @param time_step Time during which the reactions should be integrated.
   */
  void solve (double time_step);

  /**
   * @brief Update system according to the reaction system until a reaction 
   *  happens.
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
   * @return Number of reactions that have occurred from the beginning of 
   *  simulation.
   */
  long long int number_reactions_performed (void) const;  
  
 protected:
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
   * @brief Accessor to the dependency graph between reactions.
   * @return DependencyGraph built during construction of the class
   *  establishing which reaction should be updated.
   */
  const DependencyGraph& dependency_graph (void) const;

  /**
   * @brief Accessor to the reactions.
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
  long long int _number_reactions_performed;

  /** @brief State of the cell. */
  CellState& _cell_state;

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

inline long long int Solver::number_reactions_performed (void) const 
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
