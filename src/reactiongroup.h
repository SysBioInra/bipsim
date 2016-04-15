

/**
 * @file reactiongroup.h
 * @brief Header for the ReactionGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_GROUP_H
#define REACTION_GROUP_H

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
#include "macros.h" // REQUIRE

/**
 * @brief Class containing a group of reactions being simulated together.
 *
 * ReactionGroup is an abstract class that contains reactions that need to be 
 * simulated together and is able to perform them. It does not specify however
 * in what order and at what time they occur. This part of the work is left to
 * the classes that inherit ReactionGroup.
 */
class ReactionGroup
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // ReactionGroup (void) {}
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ReactionGroup (const ReactionGroup& other_reaction_group);
  // /* @brief Assignment operator. */
  // ReactionGroup& operator= (const ReactionGroup& other_reaction_group);
  /** @brief Destructor. */
  virtual ~ReactionGroup (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Recompute rates (if applicable) and schedule next reaction.
   * @param current_time Current simulation time.
   */
  virtual void schedule_next_reaction (double current_time) = 0;

  /**
   * @brief Reinitialize reaction timings with different initial_time.
   * @param time Current simulation time.
   */
  virtual void reinitialize (double time) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the next scheduled reaction time.
   * @return Next reaction time (INFINITY if no reaction left).
   */
  virtual double next_reaction_time (void) const = 0;

  /**
   * @brief Accessor to next scheduled reaction.
   * @return Next scheduled reaction (0 if no reaction could be scheduled).
   */
  virtual Reaction* next_reaction (void) const = 0;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_GROUP_H
