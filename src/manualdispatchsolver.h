

/**
 * @file manualdispatchsolver.h
 * @brief Header for the ManualDispatchSolver class.
 * 
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


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "solver.h"
#include "reactiongroupevent.h"

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
   * @param initial_time Initial simulation time.
   * @param reactions List of reactions to integrate.
   * @param reaction_classification Classification specifying how reactions should be integrated.
   */
  ManualDispatchSolver (double initial_time, const std::list<Reaction*>& reactions, const ReactionClassification& reaction_classification);

  /**
   * @brief Destructor
   */
  virtual ~ManualDispatchSolver (void);

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector of reaction groups that enables integration according to different time steps.
   * @sa ReactionGroup
   * @sa ConstantRateGroup
   * @sa UpdatedRateGroup
   */
  std::vector<ReactionGroup*> _reaction_groups;

  /**
   * @brief List containing next scheduled event type for every group sorted by event time.
   * 
   * Event types is either performing a reaction or updating a group because it has expired.
   * @sa ConstantRateGroup
   * @sa ReactionGroupEvent
   */
  std::list<ReactionGroupEvent> _event_list;

  /**
   * @brief List containing indices of groups that should be updated after every reaction.
   */
  std::list<int> _updated_rate_group_indices;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Compute next reaction.
   * @return Time at which the reaction occurred.
   */
  virtual double compute_next_reaction (void);
  
  /**
   * @brief Add event in the event list for a specific reaction group.
   * @param group_index Index of the group for which an event should be added.
   */
  void add_event_for_group (int group_index);

  /**
   * @brief For UpdatedRateGroup, update rates, events and event list.
   * @param already_updated Index of the group that has already been updated.
   * @param current_time Current simulation time.
   */
  void update_variable_rate_events (int already_updated, double current_time);


  // ======================
  //  Forbidden Operations
  // ======================
  //
  /**
   * @brief Assignment operator forbidden (declared private).
   */
  ManualDispatchSolver& operator= ( const ManualDispatchSolver& other_manual_dispatch_solver );

  /**
   * @brief Copy constructor forbidden (declared private).
   */
  ManualDispatchSolver ( const ManualDispatchSolver& other_manual_dispatch_solver );


};

// ======================
//  Inline declarations
// ======================
//

#endif // MANUAL_DISPATCH_SOLVER_H
