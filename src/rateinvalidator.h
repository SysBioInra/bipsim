

/**
 * @file rateinvalidator.h
 * @brief Header for the RateInvalidator class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_INVALIDATOR_H
#define RATE_INVALIDATOR_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class noticing parent when rate changes.
 *
 * RateInvalidator uses an observer pattern to receive notifications from 
 * other objects used to invalidate a reaction rate stored in a RateValidity
 * object.
 */
class RateInvalidator
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param parent Reference to the parent to warn when a change occurs.
   * @param rate_index Index of rate to invalidate when a change occurs.
   */
  RateInvalidator (RateValidity& parent, int rate_index);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  RateInvalidator (const RateInvalidator& other);
  /** @brief Assignment operator. */
  RateInvalidator& operator= (const RateInvalidator& other);
 public:

  // /* @brief Destructor. */
  // ~RateInvalidator (void);    

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Notify observer that a change affecting its rate occurred.
   */
  void update (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent to warn when rate changes. */
  RateValidity& _parent;
  
  /** @brief Message to pass along with rate change notifications. */
  int _index;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "ratevalidity.h"

inline
RateInvalidator::RateInvalidator (RateValidity& parent, int rate_index)
  : _parent (parent)
  , _index (rate_index)
{
}

inline void RateInvalidator::update (void)
{
  _parent.invalidate (_index);
}

 
#endif // RATE_INVALIDATOR_H
