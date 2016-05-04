

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
#include <vector> // std::vector


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

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
   * @brief Constructor
   * @param cell_state Object used to store existing units and reactions.
   * @param event_handler Object used to store existing events.
   */
  Parser (CellState& cell_state, EventHandler& event_handler);

 private:
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  /** @brief Copy constructor. */
  Parser (const Parser& other_parser);
  /** @brief Assignment operator. */
  Parser& operator= (const Parser& other_parser);

 public:
  /** @brief Destructor */
  ~Parser (void);

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

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Builders used to create new entities. */
  std::vector <Builder*> _builders;
};

// ======================
//  Inline declarations
// ======================
//

#endif // PARSER_H
