

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
 * @brief UpdatedRateGroup enables simulation of a group of reactions for which the reaction
 *  rates cannot be supposed to be constant.
 *
 * UpdatedRateGroup provides the possibility to constantly update reaction rates for important
 * reactions or reactions with extremely varying rate. Computations are therefore extremely
 * expensive and reaction timings need to be updated every time a reaction occurs somewhere
 * else in the system.
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
   * @param reactions A vector of reactions simulated under constant rate hypothesis.
   * @param initial_time Time at the beginning of simulation.
   */
  UpdatedRateGroup (const std::vector<Reaction*>& reactions, double initial_time);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // UpdatedRateGroup ( const UpdatedRateGroup& other_update_rate_group );

  /**
   * @brief Destructor
   */
  virtual ~UpdatedRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform next scheduled reaction and schedule following reaction.
   */
  virtual void perform_next_reaction (void);

  /**
   * @brief Recompute rates and reschedule next reaction.
   *
   * Because reaction rates are critical for the reactions contained in this class,
   * they need to be updated every time a reaction happens somewhere else in the system.
   * This function MUST be called every time a reaction occurs ELSEWHERE. Else it should
   * NOT be called, as it is computationnaly expensive. For example, there is no need to 
   * call it just after calling perform_next_reaction() as the latter already schedules
   * the following reaction.
   *
   * @param current_time Current simulation time.
   */
  void reschedule_next_reaction (double current_time);


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
  // UpdatedRateGroup& operator= ( const UpdatedRateGroup& other_update_rate_group );

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
  /** @brief Manager that stores the rates and updates them. */
  DependencyRateManager _rate_manager;

  // =================
  //  Private Methods
  // =================
  //

  
  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // UPDATED_RATE_GROUP_H
