

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
   *
   * This function is pure virtual, schedule is in fact specified
   * by classes inheriting from ReactionGroup.
   */
  virtual void perform_next_reaction (void) = 0;

  /**
   * @brief Update all the reaction rates.
   * @sa Reaction
   */
  void update_all_rates (void);   
   

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
  double next_reaction_time (void);

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
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  static const double OVERTIME; 

protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Next scheduled reaction time. */
  double _next_reaction_time;

  // =================
  //  Protected Methods
  // =================
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
   * @brief Accessor to the rate vector.
   * @return Vector containing forward and backward rates of the reactions. More precisely, the
   *  rates of reaction with index i occupy indices 2i (forward rate) and 2i+1 (backward rate).
   *  Vector size is thus twice as big as the reaction vector.
   */
  const std::vector<double>& rates (void);

  /**
   * @brief Accessor to total rate.
   * @return Total reaction rate at the last update timing.
   */
  double total_rate (void);

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Reactions contained in the group. */
  std::vector<Reaction*> _reactions;

  /** @brief Number of reactions contained in the group. */
  int _number_reactions;

  /** @brief Reaction rates. There are two rates for every reaction: a forward rate and a backward rate. */
  std::vector<double> _rates;

  /** @brief Total reaction rate. */
  double _total_rate;

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
inline double ReactionGroup::next_reaction_time (void)
{
  return _next_reaction_time;
}

inline const std::vector<double>& ReactionGroup::rates (void)
{
  return _rates;
}

inline double ReactionGroup::total_rate (void)
{
  return _total_rate;
}

#endif // REACTION_GROUP_H
