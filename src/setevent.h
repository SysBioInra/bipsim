

/**
 * @file setevent.h
 * @brief Header for the SetEvent class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SET_EVENT_H
#define SET_EVENT_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "event.h"

/**
 * @brief Class representing events where amount of chemical is set.
 *
 * SetEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is set at some point in time. It inherits class Event.
 */
class SetEvent : public Event
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
   * @param quantity Amout of chemical to set.
   */
  SetEvent (double time, Chemical& target, int quantity);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SetEvent (const SetEvent& other_set_event);

  /**
   * @brief Destructor.
   */
  virtual ~SetEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform event (set specified amount of chemical).
   */
  virtual void perform (void);

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
  // SetEvent& operator= (const SetEvent& other_set_event);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to set. */
  int _quantity;

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

#endif // SET_EVENT_H
