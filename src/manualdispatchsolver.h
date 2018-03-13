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
 * @file manualdispatchsolver.h
 * @brief Header for the ManualDispatchSolver class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef MANUAL_DISPATCH_SOLVER_H
#define MANUAL_DISPATCH_SOLVER_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "solver.h"

/**
 * @brief Class integrating reactions with user-defined groups and time steps.
 *
 * ManualDispatchSolver integrates a system of reactions according to a user-
 * specified classification. This classification defines when rates should be
 * updated, allowing for efficient scheduling of reaction times.
 * This class inherits class Solver.
 * @sa ConstantRateGroup
 * @sa UpdatedRateGroup
 */
class ManualDispatchSolver : public Solver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param params Simulation parameters.
   * @param reaction_classification Classification specifying how reactions 
   *  should be integrated.
   */
  ManualDispatchSolver (const SimulationParams& params,
			const ReactionClassification& reaction_classification);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  ManualDispatchSolver (const ManualDispatchSolver& other_solver);
  /** @brief Assignment operator. */
  ManualDispatchSolver& operator= (const ManualDispatchSolver& other_solver);
  
 public:
  /**
   * @brief Destructor
   */
  ~ManualDispatchSolver (void);

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
  /** @brief Struct storing next scheduled event. */
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Solver
  void schedule_next_reaction (void);
  void reinitialize (void);
  Reaction& next_reaction (void) const;
  
  /**
   * @brief Insert event in event list.
   * @param time Time of event.
   * @param group Pointer to group performing event.
   */
  void insert_event (double time, ReactionGroup* group);

  /**
   * @brief Update first envent in event list.
   */
  void update_first_event (void);

  /**
   * @brief For UpdatedRateGroup, update rates and event.
   */
  void update_variable_event (void);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Next reaction. */
  Reaction* _next_reaction;

  /** @brief Next reaction time. */
  double _next_reaction_time;

  /** @brief Vector of groups with varying time steps. */
  std::vector <ConstantRateGroup*> _constant_rate_groups;

  /** @brief Vector of groups to update after every reaction. */
  std::vector <UpdatedRateGroup*> _updated_rate_groups;

  struct GroupEvent 
  {
    GroupEvent (double time = 0, ReactionGroup* group = 0)
    : time (time), group (group) {}
    double time;
    ReactionGroup* group;
  };

  /** @brief List of events (reaction or group update) sorted by time. */
  std::list <GroupEvent> _event_list;

  /** @brief Event for the next UpdatedRateGroup that triggers. */
  GroupEvent _variable_event;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // REQUIRE

inline double ManualDispatchSolver::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline Reaction& ManualDispatchSolver::next_reaction (void) const
{
  /** @pre A reaction must be scheduled (i.e. next reaction time is finite). */
  REQUIRE (_next_reaction_time != NO_REACTION_LEFT);
  return *_next_reaction;
}

#endif // MANUAL_DISPATCH_SOLVER_H
