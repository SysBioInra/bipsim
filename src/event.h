

/**
 * @file event.h
 * @brief Header for the Event class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef EVENT_H
#define EVENT_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class representing simulation events.
 *
 * Event is an abstract class used for user-defined events that happen
 * throughout simulation, such as adding/removing a given amount of a
 * target chemical at some point in time.
 */
class Event
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param time Time at which the event happens during simulation.
   * @param target Chemical impacted by the event.
   */
  Event (double time, Chemical& target);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // Event (const Event& other_event);

  /**
   * @brief Destructor.
   */
  virtual ~Event (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform event (implemented by children classes).
   */
  virtual void perform (void) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to event time.
   * @return Time at which event occurs.
   */
  double time (void);

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
  // Event& operator= (const Event& other_event);

 protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //
  /** 
   * @brief Accessor to the target of event.
   * @return Reference to event target.
   */
  Chemical& target (void);

 private:
  
  // ============
  //  Attributes
  // ============
  //
  /** @brief Time at which event occurs. */
  double _time;

  /** @brief Target impacted by event. */
  Chemical& _target;

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
#include "macros.h" // ENSURE

inline double Event::time (void)
{
  /** @post Returned value is positive. */
  ENSURE (_time >= 0);
  return _time;
}

inline Chemical& Event::target (void)
{
  return _target;
}

#endif // EVENT_H
