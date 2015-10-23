

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
   * @brief Constructor
   * @param reactions Vector of reactions simulated under constant rate
   *  hypothesis.
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


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Manager that stores the rates and updates them. */
  NaiveRateManager _rate_manager;

  /** @brief Time points at which reactions next scheduled reactions occur. */
  std::vector<double> _reaction_times;

  /** @brief Indices of the next scheduled reactions. */
  std::vector<int> _reaction_indices;

  /** @brief Index pointing to the next reaction time and rate index to read. */
  int _next_index;

  /**
   * @brief Maximal time for which timings should be computed. If all reactions
   *  have been performed, next reaction time is OVERTIME, indicating that reaction
   *  timings greater than final_time need to be computed.
   */
  double _final_time;

  /** @brief Time step for which the timings have been computed. */
  double _time_step;

  /** @brief Max number of reactions scheduled at once. */
  static const int MAX_NUMBER_REACTIONS = 1000000;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Proceed to next scheduled reaction.
   *
   * This function simply updates the index of _reaction_times and
   * _reaction_indices or reschedules a whole set of reactions if all reactions 
   * in these vectors have been performed.
   */
  void go_to_next_reaction (void);

  /**
   * @brief Accessor to the timing of the next scheduled reaction.
   * @return Next scheduled reaction timing.
   */
  double next_reaction_time (void);

  /**
   * @brief Schedule a new set of reactions following current_time.
   *
   * The new scheduled set is stored in the vectors _reaction_times and
   * _reaction_indices. At most MAX_NUMBER_REACTIONS are scheduled and if
   * the end of the time step is reached (i.e _final_time), an OVERTIME is added
   * in _reaction_times after the last valid reaction.
   */
  void schedule_next_reactions (double current_time);


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

inline void ConstantRateGroup::go_to_next_reaction (void)
{
  if (_next_index < (_reaction_times.size()-1))
    {
      ++_next_index;
    }
  else
    {
      schedule_next_reactions (_reaction_times [_next_index]);
    }
  _next_reaction_time = next_reaction_time();
}

inline double ConstantRateGroup::next_reaction_time (void)
{
  return _reaction_times [_next_index];
}


#endif // CONSTANT_RATE_GROUP_H
