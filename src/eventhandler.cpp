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

// ==========================
//  Constructors/Destructors
// ==========================
//
const double EventHandler::NO_EVENT_LEFT = 
  std::numeric_limits<double>::infinity();

EventHandler::EventHandler (void)
{
  _current_event = _events.end();
}

// Forbidden
// EventHandler::EventHandler (const EventHandler& other_event_handler);
// EventHandler& EventHandler::operator= (const EventHandler& other_event_handler);

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
