

/**
 * @file eventfactory.h
 * @brief Header for the EventFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list
#include <string> // std::string
#include <fstream> // std::ifstream

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "factory.h"

/**
 * @brief Class used to create events used by simulator.
 *
 * EventFactory automatically recognizes which reaction to create and stores it
 * in an EventHandler object. It inherits abstract class Factory.
 */
class EventFactory : public Factory
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param cell_state Object to read/write information about units and
   *  reactions.
   * @param event_handler Handler used to store event information.
   */
  EventFactory (CellState& cell_state, EventHandler& event_handler);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // EventFactory (const EventFactory& other_event_factory);
  // /* @brief Assignment operator. */
  // EventFactory& operator= (const EventFactory& other_event_factory);
  // /* @brief Destructor. */
  // ~EventFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Factory
  bool handle (const std::string& line);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // =================
  //  Private Methods
  // =================
  //  
  /**
   * @brief Create and store event with given characteristics.
   * @param time Event time.
   * @param event_tag Tag indicating event type.
   * @param target FreeChemical targetted by event.
   * @param quantity Quantity indicating how chemical is affected by event.
   * @return True if event was successfully created and stored.
   */
  void create_event (double time, const std::string& event_tag, 
		     FreeChemical& target, int quantity);

  // ============
  //  Attributes
  // ============
  //
  /** @brief  Handler used to store event information. */
  EventHandler& _event_handler;
};

// ======================
//  Inline declarations
// ======================
//

#endif // EVENT_FACTORY_H
