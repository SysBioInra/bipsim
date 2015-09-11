

/**
 * @file eventhandler.cpp
 * @brief Implementation of the EventHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "eventhandler.h"
#include "eventparser.h"
#include "chemicalhandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double EventHandler::NO_EVENT_LEFT = std::numeric_limits<double>::infinity();

EventHandler::EventHandler (const std::string& event_file, const ChemicalHandler& chemical_handler)
{
  EventParser parser (event_file, chemical_handler);
  Event* new_event = parser.create_next_event();
  while (new_event != 0)
    {
      insert_event (new_event);
      new_event = parser.create_next_event();
    }
  
  _current_event = _events.begin();
}

// Not needed for this class (use of default copy constructor) !
// EventHandler::EventHandler ( const EventHandler& other_event_handler );

EventHandler::~EventHandler (void)
{
  // detroy events created by parsing
  for (std::list <Event*>::iterator event_it = _events.begin(); event_it != _events.end(); ++event_it)
    {
      delete *event_it;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//


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
// EventHandler& EventHandler::operator= ( const EventHandler& other_event_handler );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool EventHandler::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
void EventHandler::insert_event (Event* event)
{
  // find right position in the list
  std::list <Event*>::iterator event_it = _events.begin();
  while ((event_it != _events.end()) && ((*event_it)->time() < event->time()))
    {
      ++event_it;
    }

  // insert
  _events.insert (event_it, event);
}
