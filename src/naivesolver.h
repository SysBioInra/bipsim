

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
#include <list> // std::list
#include <map> // std::map
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "solver.h"

/**
 * @brief NaiveSolver class for integrating reactions according to a brute
 * force Gillespie algorithm.
 *
 * The NaiveSolver class stores a list of reactions, computes their rate
 * and performs them in a random order according to their rate. Solving is
 * done in a stochastic way based on exponential distributions (Gillespie
 * algorithm).
 * This class inherits class Solver.
 * @sa Solver
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
   * @param cell_state Reference to a cell state describing current state and reactions within the cell.
   */
  NaiveSolver (const SimulationParams& params, CellState& cell_state);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // NaiveSolver ( const NaiveSolver& other_naive_solver );

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
  // NaiveSolver& operator= ( const NaiveSolver& other_naive_solver );


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Rate manager handling rates and rate updates. */
  RateManager* _rate_manager;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Compute next reaction based on current _rates.
   * @return Time at which the reaction occurred.
   */
  virtual double compute_next_reaction (void);
};

// ======================
//  Inline declarations
// ======================
//


#endif // NAIVE_SOLVER_H
