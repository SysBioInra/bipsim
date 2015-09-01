

/**
 * @file observable.h
 * @brief Header for the Observable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

// ==================
//  General Includes
// ==================
//
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Generic class that accept observers and sends them notifications.
 *
 * Observable<T> accepts observers of type T and sends them notifications
 * via their update() function. Children can use the notify_change() function
 * to trigger the notification system.
 */

template <class T>
class Observable
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  Observable (void) {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // Observable ( const Observable& other_observable );

  /**
   * @brief Destructor.
   */
  virtual ~Observable (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Attach a DependencyRateManager for notification when concentration changes.
   * @param rate_manager RateManager to add to observer list.
   */
  void attach (T& observer)
  {
    _observers.insert (&observer);
  }

  /**
   * @brief Detach a previously added RateManager from observer list.
   * @param rate_manager RateManager to remove from observer list.
   */
  void detach (T& observer)
  {
    _observers.erase (&observer);
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


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
  //  * @brief Assignment operator.
  //  */
  // Observable& operator= ( const Observable& other_observable );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const {return true;}

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Notify all observers that concentration has changed.
   */
  void notify_change (void)
  {
    for (typename std::set <T*>::iterator obs_it = _observers.begin();
	 obs_it != _observers.end(); obs_it++)
      {
	(*obs_it)->update();
      }
  }


 private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief List of observers to notify when concentration changes.
   */
  std::set <T*> _observers;

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

#endif // OBSERVABLE_H
