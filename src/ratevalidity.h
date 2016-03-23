

/**
 * @file ratevalidity.h
 * @brief Header for the RateValidity class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_VALIDITY_H
#define RATE_VALIDITY_H

// ==================
//  General Includes
// ==================
//
#include <vector> //std::vector

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "vectorstack.h"

/**
 * @brief Class monitoring validity of reaction rates.
 *
 * RateValidity is a stack storing indices of rates to update.
 */
class RateValidity
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param number_rates Number of rates to monitor.
   */
  RateValidity (int number_rates);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  RateValidity (const RateValidity& other);
  /** @brief Assignment operator. */
  RateValidity& operator= (const RateValidity& other);
 public:

  /** @brief Destructor. */
  ~RateValidity (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Mark rate as outdated.
   * @param identifier Identifier of rate to update.
   */
  void invalidate (int identifier);

  /**
   * @brief Remove next element from update stack.
   */
  void pop (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to next element in update stack.
   * @return Index of the element to update.
   */
  int front (void) const;

  /**
   * @brief Returns whether update list is empty.
   * @return True if there are no more elements to update.
   */
  bool empty (void) const;

  /**
   * @brief Accessor to observer used to invalidate specific rate.
   * @param identifier Identifier of rate to invalidate.
   * @return RateInvalidator observer that invalidates given rate upon update.
   */
  RateInvalidator& invalidator (int identifier) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Flag telling if elements have already been stacked for update. */
  std::vector <int> _invalidated;

  /** @brief Invalidators used as observers to invalidate a specific rate. */
  std::vector <RateInvalidator*> _invalidators;
  
  /** @brief Stack holding indices of elements to update. */
  VectorStack <int> _update_stack;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline void RateValidity::invalidate (int identifier)
{
  /** @pre identifier must be consistent with number of rates to watch. */
  REQUIRE ((identifier >= 0) && (identifier < _invalidated.size()));
  if (_invalidated [identifier]) return;
  _update_stack.push (identifier);
  _invalidated [identifier] = true;
}

inline void RateValidity::pop (void)
{ 
  /** @pre Update stack must not be empty. */
  REQUIRE (!empty());
  _invalidated [_update_stack.item()] = false;
  _update_stack.pop(); 
}

inline int RateValidity::front (void) const
{ 
  /** @pre Update stack must not be empty. */
  REQUIRE (!empty());
  return _update_stack.item();
}

inline bool RateValidity::empty (void) const
{
  return _update_stack.empty(); 
}

inline RateInvalidator& RateValidity::invalidator (int identifier) const
{
  /** @pre identifier must be consistent with number of rates to watch. */
  REQUIRE ((identifier >= 0) && (identifier < _invalidators.size()));
  return *(_invalidators [identifier]);
}

#endif // RATE_VALIDITY_H
