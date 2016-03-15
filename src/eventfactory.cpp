

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

#include "cellstate.h"
#include "eventhandler.h"
#include "event.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
EventFactory::EventFactory (CellState& cell_state, EventHandler& event_handler)
  : Factory (cell_state)
  , _event_handler (event_handler)
{
}


// Not needed for this class (use of compiler-generated versions)
// EventFactory::EventFactory (const EventFactory& other_event_factory);
// EventFactory& EventFactory::operator= (const EventFactory& other_factory);
// EventFactory::~EventFactory (void);

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
  
  double time = read <double> (line_stream);
  std::string event_tag = read <std::string> (line_stream);
  FreeChemical* target = fetch <FreeChemical> (line_stream);
  int quantity = read <int> (line_stream);

  // create event and return
  create_event (time, event_tag, *target, quantity);
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void EventFactory::create_event (double time, const std::string& event_tag, 
				 FreeChemical& target, int quantity)
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
