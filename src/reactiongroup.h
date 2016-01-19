

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
  /**
   * @brief Constructor.
   * @param reactions A vector of reactions that need to be simulated together.
   */
  ReactionGroup (const std::vector<Reaction*>& reactions);

  // Not needed for this class (use of compiler-generated versions)
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
   * @brief Perform next scheduled reaction and schedule following reaction.
   * @return true if reaction was actually performed, false if there were not 
   *  enough reactants.
   */
  virtual bool perform_next_reaction (void) = 0;
   

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the next scheduled reaction time.
   * @return Next reaction time.
   *
   * For efficiency reasons, this function is not virtual, even though
   * its return value must in fact be specfied by classes inheriting from
   * ReactionGroup. If there is no scheduled reaction because it is beyond
   * the group's time scope, it should return ReactionGroup::OVERTIME.
   */
  double next_reaction_time (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Constant used to indicate that the next reaction is outside 
   *  simulated time step.
   */
  static const double OVERTIME; 

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //
  /** @brief Next scheduled reaction time. */
  double _next_reaction_time;

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Perform reaction with given index.
   * @param index Reaction index.
   */
  void perform_reaction (int index);

  /**
   * @brief Check whether there are enough reactants available for reaction with a specific index.
   * @param index Reaction index.
   * @return true if there are enough reactants.
   */
  bool is_reaction_possible (int index);

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reactions contained in the group. */
  std::vector<Reaction*> _reactions;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "reaction.h"

inline double ReactionGroup::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline void ReactionGroup::perform_reaction (int index)
{
  /** @pre Index must be in vector range. */
  REQUIRE ((index >= 0) && (index < _reactions.size()));
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (_reactions[index]->is_reaction_possible());
  _reactions [index]->perform();
}

inline bool ReactionGroup::is_reaction_possible (int index)
{
  /** @pre Index must be in vector range. */
  REQUIRE ((index >= 0) && (index < _reactions.size()));
  return _reactions[index]->is_reaction_possible();
}

#endif // REACTION_GROUP_H
