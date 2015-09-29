

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
   * @param cell_sate Object to read/write information about units and reactions.
   * @param event_handler Handler used to store event information.
   */
  EventFactory (CellState& cell_state, EventHandler& event_handler);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // EventFactory (const EventFactory& other_event_factory);

  /**
   * @brief Destructor.
   */
  ~EventFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create entity from line.
   * @param line Text line to create entity from.
   * @return True if an entity was successfully created.
   */
  bool handle (const std::string& line);

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
  // /*
  //  * @brief Assignment operator.
  //  */
  // EventFactory& operator= (const EventFactory& other_event_factory);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief  Object to read/write information about units and reactions. */
  CellState& _cell_state;

  /** @brief  Handler used to store event information. */
  EventHandler& _event_handler;

  // =================
  //  Private Methods
  // =================
  //  
  /**
   * @brief Create and store event with given characteristics.
   * @param time Event time.
   * @param event_tag Tag indicating event type.
   * @param target Chemical targetted by event.
   * @param quantity Quantity indicating how chemical is affected by event.
   * @return True if event was successfully created and stored.
   */
  bool create_event (double time, const std::string& event_tag, Chemical& target, int quantity);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // EVENT_FACTORY_H
