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
 * @file constantrategroup.h
 * @brief Header for the ConstantRateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CONSTANT_RATE_GROUP_H
#define CONSTANT_RATE_GROUP_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reactiongroup.h"
#include "naiveratemanager.h"

/**
 * @brief Class simulating a group of reactions during a time step for which the
 *  reaction rates are supposed to be constant.
 *
 * Under constant rate hypothesis, it is very efficient to compute a chain of
 * reactions. Once the reactions and time step are given, ConstantRateGroup
 * computes the reaction schedule for the time step at once (limited to
 * MAX_NUMBER_REACTIONS to avoid high memory usage, scheduling is performed in 
 * several steps if necessary). Access to their timing and performing them is
 * therefore extremely quick.
 * This class inherits class ReactionGroup.
 * @sa ReactionGroup
 * @sa UpdatedRateGroup
 */
class ConstantRateGroup : public ReactionGroup
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param params Simulation parameters.
   * @param reactions Vector of reactions simulated under constant rate
   *  hypothesis.
   * @param initial_time Time at the beginning of simulation.
   * @param time_step Time interval during which reactions need to be performed.
   */
  ConstantRateGroup (const SimulationParams& params,
		     const std::vector<Reaction*>& reactions,
		     double initial_time, double time_step);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ConstantRateGroup (const ConstantRateGroup& other_group);
  // /* @brief Assignment operator */
  // ConstantRateGroup& operator= (const ConstantRateGroup& other_group);
  // /* @brief Destructor */
  // virtual ~ConstantRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from ReactionGroup
  void schedule_next_reaction (double time);
  void reinitialize (double time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from ReactionGroup
  double next_reaction_time (void) const;
  Reaction* next_reaction (void) const;

  // ==================
  //  Public Constants
  // ==================
  //

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Manager that stores the rates and updates them. */
  NaiveRateManager _rate_manager;

  /** @brief Time of next schedule reaction (OVERTIME if out of time step). */
  double _next_reaction_time;

  /** @brief Next scheduled reaction. */
  Reaction* _next_reaction;

  /** @brief End of time step. */
  double _final_time;

  /** @brief Time step for which the timings have been computed. */
  double _time_step;
};

// ======================
//  Inline declarations
// ======================
//
inline double ConstantRateGroup::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline Reaction* ConstantRateGroup::next_reaction (void) const
{
  return _next_reaction;
}

#endif // CONSTANT_RATE_GROUP_H
