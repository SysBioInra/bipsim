

/**
 * @file addevent.h
 * @brief Header for the AddEvent class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef ADD_EVENT_H
#define ADD_EVENT_H

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
 * @brief Class representing events where chemicals are added.
 *
 * AddEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is added at some point in time. It inherits class Event.
 */
class AddEvent : public Event
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
   * @param quantity Amout of chemical to add.
   */
  AddEvent (double time, Chemical& target, int quantity);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // AddEvent (const AddEvent& other_add_event);

  /**
   * @brief Destructor.
   */
  virtual ~AddEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform event (add specified amount of chemical).
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
  // AddEvent& operator= (const AddEvent& other_add_event);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to add. */
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

#endif // ADD_EVENT_H
