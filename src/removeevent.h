

/**
 * @file removeevent.h
 * @brief Header for the RemoveEvent class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REMOVE_EVENT_H
#define REMOVE_EVENT_H

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
 * @brief Class representing events where chemicals are removed.
 *
 * RemoveEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is removed at some point in time. It inherits class Event.
 */
class RemoveEvent : public Event
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
   * @param quantity Amout of chemical to remove.
   */
  RemoveEvent (double time, Chemical& target, int quantity);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RemoveEvent (const RemoveEvent& other_remove_event);

  /**
   * @brief Destructor.
   */
  virtual ~RemoveEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform event (remove specified amount of chemical).
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
  // RemoveEvent& operator= (const RemoveEvent& other_remove_event);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to remove. */
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

#endif // REMOVE_EVENT_H
