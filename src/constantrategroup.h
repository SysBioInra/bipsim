

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
#include <limits> // std::numerical_limits<double>::infinity()

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reactiongroup.h"
#include "naiveratemanager.h"

/**
 * @brief ConstantRateGroup enables simulation of a group of reactions during a time step
 *  for which the reaction rates are supposed to be constant.
 *
 * Under constant rate hypothesis, it is very efficient to compute a chain of reactions.
 * Once the reactions and time step are given, ConstantRateGroup computes the reaction schedule
 * for the time step at once. Access to their timing and performing them is therefore extremely
 * quick.
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
   * @brief Constructor
   * @param reactions A vector of reactions simulated under constant rate hypothesis.
   * @param initial_time Time at the beginning of simulation.
   * @param time_step Time interval during which reactions need to be performed.
   */
  ConstantRateGroup (const std::vector<Reaction*>& reactions, double initial_time, double time_step);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ConstantRateGroup ( const ConstantRateGroup& other_constant_rate_group );

  /**
   * @brief Destructor
   */
  virtual ~ConstantRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform next scheduled reaction and schedule following reaction.
   */
  virtual bool perform_next_reaction (void);

  /**
   * @brief Reinitialize reaction timings with different initial_time.
   *
   * Reuses the reactions and time step specified at constructions, only
   * initial and final times change. Reaction rates are updated before timings
   * are computed.
   * @param initial_time Time at the beginning of simulation.
   */
  void reinitialize (double initial_time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  
  /**
   * @brief Accessor to final time of simulation.
   * @return Maximal time for which timings have been computed. If all reactions
   *  have been performed, next reaction time is OVERTIME, indicating that reaction
   *  timings greater than final_time need to be computed.
   */
  double final_time (void) const;

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
  // ConstantRateGroup& operator= ( const ConstantRateGroup& other_constant_rate_group );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Manager that stores the rates and updates them. */
  NaiveRateManager _rate_manager;

  /** @brief Time points at which reactions occur as determined during construction. */
  std::vector<double> _reaction_times;

  /** @brief Indices of the reaction rates that occur as determined during construction. */
  std::vector<int> _reaction_rate_indices;

  /** @brief Index pointing to the next reaction time and rate index to read. */
  int _next_index;

  /**
   * @brief Maximal time for which timings have been computed. If all reactions
   *  have been performed, next reaction time is OVERTIME, indicating that reaction
   *  timings greater than final_time need to be computed.
   */
  double _final_time;

  /* @brief Time step for which the timings have been computed. */
  double _time_step;

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
inline double ConstantRateGroup::final_time (void) const
{
  return _final_time;
}



#endif // CONSTANT_RATE_GROUP_H
