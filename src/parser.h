

/**
 * @file parser.h
 * @brief Header for the Parser class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PARSER_H
#define PARSER_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <sstream> // std::istreamstring
#include <list> // std::list


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "unitfactory.h"
#include "reactionfactory.h"
#include "eventfactory.h"

/**
 * @brief Class reading input data and creating the corresponding entities.
 *
 */
class Parser
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param cell_state Object used to store existing units and reactions.
   * @param event_handler Object used to store existing events.
   */
  Parser (CellState& cell_state, EventHandler& event_handler);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Parser ( const Parser& other_parser );

  /**
   * @brief Destructor
   */
  virtual ~Parser (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Load information about units and reactions.
   * @param input_data Data to parse.
   */
  void parse (InputData& input_data);

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
  //  * @brief Assignment operator
  //  */
  // Parser& operator= ( const Parser& other_parser );

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Factory used to create new units. */
  UnitFactory _unit_factory;
  
  /** @brief Factory used to create new reactions. */
  ReactionFactory _reaction_factory;

  /** @brief Factory used to create new events. */
  EventFactory _event_factory;  

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Loop through input data once.
   * @param input_data Data to parse.
   * @return True if at least one entity was successfully created.
   */
  bool loop_through_data (InputData& input_data);

  void display_dependency_errors (InputData& input_data);
};

// ======================
//  Inline declarations
// ======================
//

#endif // PARSER_H
