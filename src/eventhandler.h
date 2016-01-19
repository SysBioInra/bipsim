

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
 * @brief Class storing and handling simulation events.
 *
 * EventHandler stores user-defined events such as
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
   * @brief Default constructor.
   */
  EventHandler (void);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  EventHandler (const EventHandler& other_event_handler);
  /** @brief Assignment operator. */
  EventHandler& operator= ( const EventHandler& other_event_handler );
 public:

  /** @brief Destructor. */
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

  /**
   * @brief Store new event.
   * @param event Object created on the heap that should be managed and destroyed
   *  by EventHandler;
   */
  void store (Event* event);

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

inline void EventHandler::store (Event* event)
{
  insert_event (event);
  _current_event = _events.begin();
}

#endif // EVENT_HANDLER_H
