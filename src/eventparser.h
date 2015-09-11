

/**
 * @file eventparser.h
 * @brief Header for the EventParser class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef EVENT_PARSER_H
#define EVENT_PARSER_H

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

/**
 * @brief Class that parses an event file.
 *
 * EventParser parses an input file, creates events contained
 * within the file and gives access to the list of created events
 * in chronological order.
 */
class EventParser
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param event_file Input file to read events from.
   * @param chemical_handler Handler used to access references to chemicals.
   */
  EventParser (const std::string& event_file, const ChemicalHandler& chemical_handler);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // EventParser (const EventParser& other_event_parser);

  /**
   * @brief Destructor.
   */
  ~EventParser (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Parse until next valid line and create corresponding event.
   * @return Pointer to event corresponding to next valid input line or 0 if end of file was
   *  reached. Object needs to be destroyed by the recipient.
   */
  Event* create_next_event (void);

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
  // EventParser& operator= (const EventParser& other_event_parser);

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
  /** @brief Handler used to access references to chemicals. */
  const ChemicalHandler& _chemical_handler;

  /** @brief Current line being parsed. */
  int _line;

  /** @brief File being parsed. */
  std::ifstream _file;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Check whether a line needs to be parsed.
   * @param line Line to check.
   * @return True if line only contains blank characters or is commented using the
   *  '#' character.
   */
  bool is_empty (const std::string& line);

  /**
   * @brief Check whether an event_tag is valid.
   * @param event_tag Tag to check.
   * @return True if tag corresponds to an existing event type.
   */
  bool is_tag_valid (const std::string& event_tag);
  
  /**
   * @brief Create event with given characteristics.
   * @param time Event time.
   * @param event_tag Tag indicating event type.
   * @param target Chemical targetted by event.
   * @param quantity Quantity indicating how chemical is affected by event.
   */
  Event* create_event (double time, const std::string& event_tag, Chemical& target, int quantity);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // EVENT_PARSER_H
