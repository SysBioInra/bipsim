

/**
 * @file eventhandler.h
 * @brief Header for the EventHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class reading and handling simulation events.
 *
 * EventHandler reads and performs user-defined events such as
 * adding/removing a chemical at a given time.
 */
class EventHandler
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param event_file File containing events to perform.
   * @param chemical_handler Handler providing references to the chemicals to modify.
   */
  EventHandler (const std::string& event_file, const ChemicalHandler& chemical_handler);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // EventHandler ( const EventHandler& other_event_handler );

  /**
   * @brief Destructor.
   */
  ~EventHandler (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform next scheduled event.
   */
  void perform_event (void);

  /**
   * @brief Ignore next scheduled event.
   */
  void ignore_event (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to next event time.
   * @return Time at which the next event is supposed to occur. Returns
   *  EventHandler::NO_EVENT_LEFT if there is no event left.
   */
  double next_event_time (void);

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
  // EventHandler& operator= ( const EventHandler& other_event_handler );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Next event time if there is no event left.
   */
  static const double NO_EVENT_LEFT;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief List of events performed during simulation in chronological order. */
  std::list <Event*> _events;

  /** @brief Iterator to the current event. */
  std::list <Event*>::iterator _current_event;
  

  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Insert an event in event list according to chronological order.
   * @param event Pointer to event to insert.
   */
  void insert_event (Event* event);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
#include "event.h"

inline void EventHandler::ignore_event (void)
{
  /** @pre There must be at least one event left. */
  REQUIRE (_current_event != _events.end());
  ++_current_event;
}

inline void EventHandler::perform_event (void)
{
  /** @pre There must be at least one event left. */
  REQUIRE (_current_event != _events.end());
  (*_current_event)->perform();
  ++_current_event;
}

inline double EventHandler::next_event_time (void)
{
  if (_current_event == _events.end())
    { return NO_EVENT_LEFT; }
  else 
    { return (*_current_event)->time(); }
}

#endif // EVENT_HANDLER_H
