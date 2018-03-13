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
 * @file eventbuilder.h
 * @brief Header for the EventBuilder class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef EVENT_BUILDER_H
#define EVENT_BUILDER_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "builder.h"

/**
 * @brief Class creating events from text input.
 */
class EventBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object to read/write information about units and
   *  reactions.
   * @param event_handler Handler used to store event information.
   */
  EventBuilder (CellState& cell_state, EventHandler& event_handler);

  // redefined from Builder
  bool match (InputLine& text_input);

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief  Handler used to store event information. */
  EventHandler& _event_handler;

  // format and values read
  Rule _add_format, _remove_format, _set_format, _dosage_format;
  double _time, _dosage_step, _dosage_end;
  std::string _chemical;
  int _quantity;
};

// ======================
//  Inline declarations
// ======================
//

#endif // EVENT_BUILDER_H
