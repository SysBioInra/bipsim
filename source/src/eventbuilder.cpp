// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 



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
  , _dosage_format (TagToken ("event") + DblToken (_time) + TagToken ("DOSAGE") 
		    + DblToken (_dosage_step) + DblToken (_dosage_end)
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
  else if (_dosage_format.match (text_input))
    {
      FreeChemical& chemical = fetch <FreeChemical> (_chemical);
      for (double t = _time; t <= _dosage_end; t += _dosage_step)
	{
	  _event_handler.store (new SetEvent (t, chemical, _quantity));
	}
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
