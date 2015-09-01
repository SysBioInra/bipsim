

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
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "ratemanager.h"
#include "reaction.h"
#include "macros.h" // REQUIRE

/**
 * @brief The ReactionGroup class contains a group of reactions that need to be simulated at the same level.
 *
 * ReactionGroup is an abstract class that contains reactions that need to be simulated
 * together and is able to perform them. It does not specify however in what order and
 * at what time they occur. This part of the work is left to the classes that inherit 
 * from ReactionGroup.
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

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ReactionGroup ( const ReactionGroup& other_reaction_group );

  /**
   * @brief Destructor
   */
  virtual ~ReactionGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform next scheduled reaction and schedule following reaction.
   * @return true if reaction was actually performed, false if there were not enough reactants.
   *
   * This function is pure virtual, schedule is in fact specified
   * by classes inheriting from ReactionGroup.
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
  // ReactionGroup& operator= ( const ReactionGroup& other_reaction_group );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Constant used to indicate that the next reaction is outside simulated time step.
   */
  static const double OVERTIME; 

protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Next scheduled reaction time. */
  double _next_reaction_time;

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Perform reaction according to a rate value index.
   * @param rate_index The index should typically be specified according to a rate value
   *  that has been drawn in the _rates vector, NOT the reaction index in the _reactions vector.
   *  Because forward rate and backward rate are stored separately, the rate index specifies both
   *  the reaction to perform and the sense of reaction.
   */
  void perform_reaction (int rate_index);

  /**
   * @brief Check whether there are enough reactants available for reaction with a specific rate index.
   * @param rate_index The index should typically be specified according to a rate value
   *  that has been drawn in the _rates vector, NOT the reaction index in the _reactions vector.
   *  Because forward rate and backward rate are stored separately, the rate index specifies both
   *  the reaction to perform and the sense of reaction.
   * @return true if there are enough reactants.
   */
  bool is_reaction_possible (int rate_index);

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

  // ======================
  //  Forbidden Operations
  // ======================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline double ReactionGroup::next_reaction_time (void) const
{
  return _next_reaction_time;
}

inline void ReactionGroup::perform_reaction (int rate_index)
{
  // compute next reaction to perform
  Reaction& reaction = *_reactions [rate_index/2];
  
  // perform reaction
  if ( (rate_index % 2) == 0 )
    {
      /** @pre There must be enough reactants to perform reaction. */
      REQUIRE (reaction.is_forward_reaction_possible());
      reaction.perform_forward();
    }
  else
    {
      // see above
      REQUIRE (reaction.is_backward_reaction_possible());
      reaction.perform_backward();
    }
}

inline bool ReactionGroup::is_reaction_possible (int rate_index)
{
  // find reaction to perform
  Reaction& reaction = *_reactions [rate_index/2];
  
  // compute reactant availability
  if ( (rate_index % 2) == 0 )
    {
      return reaction.is_forward_reaction_possible();
    }
  else
    {
      return reaction.is_backward_reaction_possible();
    }
}

#endif // REACTION_GROUP_H
