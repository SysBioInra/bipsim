

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
#include "randomhandler.h"

/**
 * @brief Solver class for integrating reactions.
 *
 * The Solver class stores a list of reactions, computes their rate
 * and performs them in a random order according to their rate.
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
   */
  Solver (void);

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
   * @brief Add reaction to reactions to integrate.
   * @param reaction Reaction to add.
   */
  void add_reaction (Reaction& reaction);

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


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Set simulation time.
   * @param time Time value.
   */
  void set_time (double time);

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
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Total number of reactions to integrate. */
  int _number_reactions;

  /** @brief Vector of reactions to integrate. */
  std::vector< Reaction* > _reactions;

  /** @brief Total reaction rate. */
  int _total_rate;

  /** @brief Vector of reaction rates. */
  std::vector< double > _rates;

  /** @brief Simulation time. */
  double _t;

  /** @brief Random handler used for determining next reaction. */
  RandomHandler _random_handler;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Fill the _rates vector with current reaction rates.
   */
  void update_rates (void);

  /**
   * @brief Update _t to the next reaction time based on current _rates.
   */
  void set_next_reaction_time (void);

  /**
   * @brief Compute next reaction based on current _rates.
   */
  void compute_next_reaction (void);
};

// ======================
//  Inline declarations
// ======================
//
inline double Solver::time (void) const 
{
  return _t;
}

inline void Solver::set_time (double time)
{
  _t = time;
}


#endif // SOLVER_H
