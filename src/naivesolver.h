

/**
 * @file naivesolver.h
 * @brief Header for the NaiveSolver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef NAIVE_SOLVER_H
#define NAIVE_SOLVER_H

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
#include "solver.h"

/**
 * @brief NaiveSolver class for integrating reactions according to a brute
 *  force Gillespie algorithm.
 *
 * The NaiveSolver class stores a list of reactions, computes their rate
 * and performs them in a random order according to their rate. Solving is
 * done in a stochastic way based on exponential distributions (Gillespie
 * algorithm).
 * This class inherits class Solver.
 */
class NaiveSolver : public Solver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param params Simulation parameters.
   * @param reactions Vector of reactions to integrate.
   */
  NaiveSolver (const SimulationParams& params,
	       const std::vector <Reaction*>& reactions);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  NaiveSolver (const NaiveSolver& other_naive_solver);
  /** @brief Assignment operator. */
  NaiveSolver& operator= (const NaiveSolver& other_naive_solver);

 public:
  /**
   * @brief Destructor
   */
  virtual ~NaiveSolver (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //  
  // Redefined from Solver
  double next_reaction_time (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Solver
  void schedule_next_reaction (void);
  Reaction& next_reaction (void) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Rate manager handling rates and rate updates. */
  RateManager* _rate_manager;

  /** @brief Next reaction. */
  Reaction* _next_reaction;

  /** @brief Next reaction time. */
  double _next_reaction_time;
};

// ======================
//  Inline declarations
// ======================
//
inline double NaiveSolver::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline Reaction& NaiveSolver::next_reaction (void) const
{
  /** @pre A reaction must be scheduled (i.e. next reaction time is finite). */
  REQUIRE (_next_reaction_time != INFINITY);
  return *_next_reaction;
}

#endif // NAIVE_SOLVER_H
