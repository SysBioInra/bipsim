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
 * @file updatedrategroup.h
 * @brief Header for the UpdatedRateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef UPDATED_RATE_GROUP_H
#define UPDATED_RATE_GROUP_H

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
#include "dependencyratemanager.h"

/**
 * @brief Class simulating a group of reactions for which the reaction rates
 *  cannot be supposed to be constant.
 *
 * UpdatedRateGroup provides the possibility to constantly update reaction rates
 * for important reactions or reactions with extremely varying rate.
 * Computations are therefore extremely expensive and reaction timings need to
 * be updated every time a reaction occurs somewhere else in the system.
 * This class inherits class ReactionGroup.
 * @sa ReactionGroup
 * @sa ConstantRateGroup
 */
class UpdatedRateGroup : public ReactionGroup
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param params Simulation parameters.
   * @param reactions A vector of reactions simulated under constant rate 
   *  hypothesis.
   * @param initial_time Time at the beginning of simulation.
   */
  UpdatedRateGroup (const SimulationParams& params,
		    const std::vector<Reaction*>& reactions,
		    double initial_time);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // UpdatedRateGroup (const UpdatedRateGroup& other);
  // /* @brief Assignment operator. */
  // UpdatedRateGroup& operator= (const UpdatedRateGroup& other);
  // /* @brief Destructor. */
  // ~UpdatedRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from RateGroup
  void schedule_next_reaction (double current_time);
  void reinitialize (double time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from ReactionGroup
  double next_reaction_time (void) const;
  Reaction* next_reaction (void) const;

  // ==================
  //  Public constants
  // ==================
  //  
  /** @brief Shortcut for double value representing infinity. */
  static const double NO_REACTIONS_LEFT;

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
  DependencyRateManager _rate_manager;

  /** @brief Next reaction. */
  Reaction* _next_reaction;

  /** @brief Next reaction time. */
  double _next_reaction_time;
};

// ======================
//  Inline declarations
// ======================
//
inline double UpdatedRateGroup::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline Reaction* UpdatedRateGroup::next_reaction (void) const
{
  return _next_reaction;
}


#endif // UPDATED_RATE_GROUP_H
