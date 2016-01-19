

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
#include <list> // std::list
#include <vector> //std::vector

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "vectorstack.h"

/**
 * @brief Class enabling handling of a pool of ConcentrationObserver.
 *
 * RateValidity handles a pool of ConcentrationObserver. Classes that inherit
 * it can add new observers or clear the whole pool. At creation, they specify
 * a target for the observer and an identifier the observer should send each
 * time it receives an update. This identifier is then transmitted via the 
 * update() function that needs to be implemented by the inheriting class.
 */
class RateValidity
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  RateValidity (int number_rates)
    : _invalidated (number_rates, false)
    , _update_stack (number_rates)
    {
    }

 private:
  // Forbidden
  /** @brief Copy constructor. */
  RateValidity (const RateValidity& other_rate_validity);
  /** @brief Assignment operator. */
  RateValidity& operator= (const RateValidity& other_rate_validity);

 public:
  /**
   * @brief Destructor.
   */
  ~RateValidity (void) { clear_observers(); }

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add an observer to the pool.
   * @param reactant Reactant to observer.
   * @param identifier Integer identifier of the observer. Should be passed
   *  to the parent each time an update occurs.
   */
  void add_observer (Reactant& reactant, int identifier);

  /**
   * @brief Clear the pool of observers.
   */
  void clear_observers (void);

  /**
   * @brief Function called by observers to notify an update.
   * @param identifier Identifier of the observer having received an update.
   */
  void update (int identifier)
  {
    /** @pre identifier must be consistent with number of rates to watch. */
    REQUIRE ((identifier >= 0) && (identifier < _invalidated.size()));
    if (_invalidated [identifier]) return;
    _update_stack.push (identifier);
    _invalidated [identifier] = true;
  }

  /**
   * @brief Remove next element from update stack.
   */
  void pop (void)
  { 
    /** @pre Update stack must not be empty. */
    REQUIRE (!empty());
    _invalidated [_update_stack.item()] = false;
    _update_stack.pop(); 
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to next element in update list.
   * @return Index of the element to update.
   */
  int front (void) const
  { 
    /** @pre Update stack must not be empty. */
    REQUIRE (!empty());
    return _update_stack.item();
  }

  /**
   * @brief Returns whether update list is empty.
   * @return True if there are no more elements to update.
   */
  bool empty (void) const { return _update_stack.empty(); }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Observer pool. */
  std::list <ConcentrationObserver*> _observers;

  /** @brief Flag telling if elements have already been stacked for update. */
  std::vector <int> _invalidated;

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
#include "concentrationobserver.h"

inline void RateValidity::add_observer (Reactant& reactant, int identifier)
{
  _observers.push_back (new ConcentrationObserver (*this, &reactant,
						   identifier));
}

inline void RateValidity::clear_observers (void)
{
  // unsubscribe reactant notifications by deleting observers
  for (std::list<ConcentrationObserver*>::iterator obs_it = _observers.begin();
       obs_it != _observers.end(); ++obs_it)
    {
      delete *obs_it; // this automatically detaches the observer
    }
  _observers.clear();
}

#endif // RATE_VALIDITY_H
