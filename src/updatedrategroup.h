

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
  // UpdatedRateGroup (const UpdatedRateGroup& other_update_rate_group);
  // /* @brief Assignment operator. */
  // UpdatedRateGroup& operator= (const UpdatedRateGroup& other_update_rate_group);
  // /* @brief Destructor. */
  // ~UpdatedRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from ReactionGroup
  bool perform_next_reaction (void);

  /**
   * @brief Recompute rates and reschedule next reaction.
   *
   * Because reaction rates are critical for the reactions contained in this
   * class, they need to be updated every time a reaction happens somewhere else
   * in the system. This function MUST be called every time a reaction occurs
   * ELSEWHERE. Else it should NOT be called, as it is computationnaly
   * expensive. For example, there is no need to call it just after calling
   * perform_next_reaction() as the latter already schedules the following
   * reaction.
   *
   * @param current_time Current simulation time.
   */
  void reschedule_next_reaction (double current_time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Manager that stores the rates and updates them. */
  DependencyRateManager _rate_manager;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//


#endif // UPDATED_RATE_GROUP_H
