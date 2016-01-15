

/**
 * @file eventfactory.cpp
 * @brief Implementation of the EventFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "eventfactory.h"

#include "simulatorexception.h"
#include "formatexception.h"
#include "parserexception.h"

#include "cellstate.h"
#include "eventhandler.h"
#include "event.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
EventFactory::EventFactory (CellState& cell_state, EventHandler& event_handler)
  : _cell_state (cell_state)
  , _event_handler (event_handler)
{
}


// Not needed for this class (use of default copy constructor) !
// EventFactory::EventFactory (const EventFactory& other_event_factory);

EventFactory::~EventFactory (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool EventFactory::handle (const std::string& line)
{
 // parse the first word and hand the rest of the line over to 
  // appropriate creator
  std::istringstream line_stream (line);  
  // first word of line must be "event"
  if (check_tag (line_stream, "event") == false) { return false; }
  
  // check that line is valid
  double time;
  std::string event_tag, target_name;
  int quantity;
  if (not (line_stream >> time >> event_tag >> target_name >> quantity))
    { throw FormatException(); }

  // get target reference
  Chemical* target = _cell_state.find<Chemical> (target_name);
  if (target == 0) { throw DependencyException (target_name); }

  // create event and return
  create_event (time, event_tag, *target, quantity);
  return true;
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
// EventFactory& EventFactory::operator= (const EventFactory& other_event_factory);


// =================
//  Private Methods
// =================
//
void EventFactory::create_event (double time, const std::string& event_tag, Chemical& target, int quantity)
{
  if (event_tag == "ADD")
    { _event_handler.store (new AddEvent (time, target, quantity)); }
  else if (event_tag == "REMOVE")
    { _event_handler.store (new RemoveEvent (time, target, quantity)); }
  else if (event_tag == "SET")
    { _event_handler.store (new SetEvent (time, target, quantity)); }
  else
    { throw ParserException ("unrecognized event type (" + event_tag + ")"); }
}
