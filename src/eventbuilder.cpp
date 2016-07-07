

/**
 * @file eventbuilder.cpp
 * @brief Implementation of the EventBuilder class.
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
#include "eventbuilder.h"

#include "simulatorexception.h"

#include "cellstate.h"
#include "eventhandler.h"
#include "event.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
EventBuilder::EventBuilder (CellState& cell_state, EventHandler& event_handler)
  : Builder (cell_state)
  , _event_handler (event_handler)
  , _add_format (TagToken ("event") + DblToken (_time) + TagToken ("ADD") 
		 + StrToken (_chemical) + IntToken (_quantity))
  , _remove_format (TagToken ("event") + DblToken (_time) + TagToken ("REMOVE") 
		    + StrToken (_chemical) + IntToken (_quantity))
  , _set_format (TagToken ("event") + DblToken (_time) + TagToken ("SET") 
		 + StrToken (_chemical) + IntToken (_quantity))
{
}


// ===========================
//  Public Methods - Commands
// ===========================
//
bool EventBuilder::match (InputLine& text_input)
{
  if (_add_format.match (text_input))
    { 
      _event_handler.store 
	(new AddEvent (_time, fetch <FreeChemical> (_chemical), _quantity)); 
    }
  else if (_remove_format.match (text_input))
    { 
      _event_handler.store 
	(new RemoveEvent (_time, fetch <FreeChemical> (_chemical), _quantity)); 
    }
  else if (_set_format.match (text_input))
    { 
      _event_handler.store 
	(new SetEvent (_time, fetch <FreeChemical> (_chemical), _quantity)); 
    }
  else { return false; }
  return true;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
