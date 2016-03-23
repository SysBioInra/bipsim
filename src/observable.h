

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
#include <list> // std::list

#ifdef HAVE_BOOST_SERIALIZATION
#include <boost/serialization/set.hpp>
#endif // HAVE_BOOST_SERIALIZATION

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Astract class that accept observers and sends them notifications.
 *
 * Observable accepts RateInvalidator and sends them notifications
 * via their update() function. Children can use the notify_change() function
 * to trigger the notification system.
 */
class Observable
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Default constructor. */
  Observable (void) {}

 private:
  // Forbidden
  /** @brief Copy constructor. */
  Observable (const Observable& other);
  /** @brief Assignment operator. */
  Observable& operator= (const Observable& other);
 public:

  // /* @brief Destructor. */
  // virtual ~Observable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Attach an observer for notification when a chage occurs.
   * @param observer RateInvalidator to add to observer list.
   */
  void attach (RateInvalidator& observer);

  /**
   * @brief Detach a previously added observer from observer list.
   * @param observer RateInvalidator to remove from observer list.
   */
  void detach (RateInvalidator& observer);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /** @brief Notify all observers. */
  void notify_change (void);


 private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Set of observers to notify. */
  std::list <RateInvalidator*> _observers;

#ifdef HAVE_BOOST_SERIALIZATION
  // ===============
  //  Serialization
  // ===============
  friend class boost::serialization::access;

  template<class Archive>
    void serialize (Archive& ar, const unsigned int version)
  {
    ar & _observers;
  }
#endif // HAVE_BOOST_SERIALIZATION
};

// ======================
//  Inline declarations
// ======================
//
#include "rateinvalidator.h"

inline void Observable::attach (RateInvalidator& observer) 
{
  _observers.push_back (&observer); 
}

inline void Observable::detach (RateInvalidator& observer) 
{
  _observers.remove (&observer);
}

inline void Observable::notify_change (void)
{
  for (std::list <RateInvalidator*>::iterator obs_it = _observers.begin();
       obs_it != _observers.end(); ++obs_it)
    { (*obs_it)->update(); }
}


#endif // OBSERVABLE_H
