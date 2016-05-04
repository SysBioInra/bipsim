

/**
 * @file eventbuilder.h
 * @brief Header for the EventBuilder class.
 * 
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
  Rule _add_format, _remove_format, _set_format;
  double _time;
  std::string _chemical;
  int _quantity;
};

// ======================
//  Inline declarations
// ======================
//

#endif // EVENT_BUILDER_H
