

/**
 * @file hybridratecontainer.h
 * @brief Header for the HybridRateContainer class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef HYBRID_RATE_CONTAINER_H
#define HYBRID_RATE_CONTAINER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <cmath> // frexp()

// ==================
//  Project Includes
// ==================
//
#include "ratecontainer.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing rates using groups of increasing size.
 *
 * HybridRateContainer inherits from RateContainer. Its uses a hybrid strategy
 * for selecting a random rate. At every update it classes rates into groups
 * bounded by a minimal value and a maximal value that are typically powers of
 * 2. A classical drawing is used to determine in which group the next reaction
 * will happen. Then a reaction in the group is selected using a rejection
 * method, the rejection probability being kept small as we know the minimal
 * and maximal value of rates stored within the group.
 * @sa RateGroup.
 */
class HybridRateContainer : public RateContainer
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param number_rates Number of rates to store.
   * @param base_rate Rate used to delimit the first group. The first group
   *  should ideally collect only a very small number of rates.
   */
  HybridRateContainer (int number_rates, double base_rate);

  // Not needed for this rate (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // HybridRateContainer (const HybridRateContainer& other_rate_container);

  /**
   * @brief Destructor.
   */
  ~HybridRateContainer (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // inherited
  void update_cumulates (void);

  // inherited
  int random_index (void) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // inherited
  double total_rate (void) const;

  /**
   * @brief Accessor to current number of groups managed by the container.
   * @return Current number of groups managed by the container.
   */
  int number_groups (void) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  // inherited
  void set_rate (int index, double value);


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this rate (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // HybridRateContainer& operator= (const HybridRateContainer& other_rate_container);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Maxmimal rate of the first group. */
  double _base_rate;

  /** @brief Current maximal rate that can be handled by the container. */
  double _current_max_rate;

  /** @brief Vector of token representing rates that are passed to groups. */
  std::vector <RateToken*> _tokens;

  /** @brief Vector of groups. */
  std::vector <RateGroup*> _groups;

  /** @brief Tree used to store the total rates associated with each group. */
  RateTree* _rate_tree;

  /** @brief Stack storing group indices that need to be updated in the tree. */
  RateValidity* _update_stack;

  /** @brief Constant used to specify that a rate is not stored in any group. */
  static const int NULL_GROUP = -1;

  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Compute which group a value is in.
   * @param value Rate value.
   * @return Index of the group the rates belongs to. If the rate is exactly a
   *  value separating two groups, the larger index is returned.
   */
  int _rate_to_group (double value);

  /**
   * @brief Create new groups until a given value can be stored.
   * @param value Rate value that needs to be stored.
   */
  void _create_new_groups (double value);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
#include "ratetree.h"
#include "macros.h"
#include "rategroup.h"
#include "ratevalidity.h"
#include "ratetoken.h"

inline void HybridRateContainer::update_cumulates (void)
{
  while (!_update_stack->empty())
    {	
      int next = _update_stack->front();
      _rate_tree->set_rate (next, _groups [next]->total_rate());
      _update_stack->pop();
    }
  _rate_tree->update_cumulates();
}

inline int HybridRateContainer::random_index (void) const
{
  return _groups [_rate_tree->random_index()]->random_index();
}

inline double HybridRateContainer::total_rate (void) const
{ 
  return _rate_tree->total_rate();
}

inline int HybridRateContainer::number_groups (void) const
{ 
  return _groups.size();
}

inline void HybridRateContainer::set_rate (int index, double value)
{
  /** 
   * @pre Index must be consistent with number of rates provided at
   * construction.
   */
  REQUIRE ((index >= 0) && (index < _tokens.size()));
  
  RateToken* token = _tokens [index];
  
  // remove token from previous group
  if (token->group() != NULL_GROUP) 
    { 
      _groups [token->group()]->remove (token); 
      _update_stack->update (token->group());
    }
  
  // add to new group
  token->set_group (GroupKey(), _rate_to_group (value));
  if (token->group() != NULL_GROUP)
    { 
      _groups [token->group()]->insert (token, value);
      _update_stack->update (token->group());
    }
}

inline int HybridRateContainer::_rate_to_group (double value)
{
  if (value == 0) { return NULL_GROUP; }
  if (value <= _base_rate) { return 0; }
  if (value >= _current_max_rate)
    { _create_new_groups (value); return _groups.size()-1; }
  
  int group;
  frexp (value/_base_rate, &group);
  /** @post Index must be consistent with number of groups. */
  ENSURE ((group > 0) && (group < _groups.size()));
  return group;
}

#endif // HYBRID_RATE_CONTAINER_H
