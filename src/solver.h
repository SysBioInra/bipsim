// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file solver.h
 * @brief Header for the Solver class.
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

/**
 * @brief Solver class for integrating reactions.
 * @details The Solver class is a pure abstract class storing a list of 
 * reactions,
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
   */
  Solver (const SimulationParams& params);

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
   * @brief Update system during given time step.
   * @param time_step Time during which the reactions should be integrated.
   */
  void solve (double time_step);

  /**
   * @brief Perform next scheduled reaction.
   */
  void perform_next_reaction (void);
  
  /**
   * @brief Reset simulation time and reschedule next reaction.
   * @param time New simulation time.
   */
  void reschedule (double time);

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
   * @brief Accessor to next reaction time.
   * @return Time of next scheduled reaction or NO_REACTION_LEFT if none.
   */
  virtual double next_reaction_time (void) const = 0;

  /**
   * @brief Accessor to the number of reactions that have occurred.
   * @return Number of reactions that have occurred from the beginning of 
   *  simulation.
   */
  long long int number_reactions_performed (void) const;  

  // ==================
  //  Public constants
  // ==================
  //  
  /** @brief Shortcut for double value representing infinity. */
  static const double NO_REACTION_LEFT;
  
 private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Schedule next reaction.
   */
  virtual void schedule_next_reaction (void) = 0;

  /**
   * @brief Reset system and reschedule next reaction.
   */
  virtual void reinitialize (void);

  /**
   * @brief Accessor to next reaction.
   * @return Reaction scheduled to be performed next.
   */
  virtual Reaction& next_reaction (void) const = 0;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Simulation time. */
  double _t;

  /** @brief Number of reactions that have been performed. */
  long long int _number_reactions_performed;
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

inline void Solver::reinitialize (void)
{
  schedule_next_reaction();
}

#endif // SOLVER_H
