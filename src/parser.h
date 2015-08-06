

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

/**
 * @brief Parser reads a file and creates the corresponding entities.
 *
 * The class loads units, sites and reactions separately to ensure that
 * they are created in the right order.
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
   */
  Parser (ChemicalHandler& chemical_handler, ReactionHandler& reaction_handler,
	  BindingSiteHandler& binding_site_handler, TerminationSiteHandler& termination_site_handler,
	  TableHandler& table_handler);

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
   * @brief Opens a file and loads information about units.
   * @param filename File to parse.
   */
  void parse_units ( const char* filename );

  /**
   * @brief Opens a file and loads information about reactions.
   * @param filename File to parse.
   */
  void parse_reactions ( const char* filename );

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Handler that creates chemicals. */
  ChemicalHandler& _chemical_handler;

  /** @brief Handler that creates chemical reactions. */
  ReactionHandler& _reaction_handler;

  /** @brief Handler that creates birnding sites. */
  BindingSiteHandler& _binding_site_handler;

  /** @brief Handler that creates termination sites. */
  TerminationSiteHandler& _termination_site_handler;

  /** @brief Handler that creates various tables. */
  TableHandler& _table_handler;

  /** @brief Number of times the file has been read. */
  int _read;

  /** @brief Line currently read. */
  int _line;

  /** @brief Lines to read again. */
  std::list< int > _lines_to_reread;
  

  // =================
  //  Private Methods
  // =================
  //
  /** @brief Read the start of a line and pass it over to the right unit parser. */
  void parse_unit_line (std::string& line);

  /** @brief Read the start of a line and pass it over to the right reaction parser. */
  void parse_reaction_line (std::string& line);

  /** @brief Parse a line describing a binding site. */
  void parse_binding_site ( std::istringstream& line_stream );

  /** @brief Parse a line describing a termination site. */
  void parse_termination_site ( std::istringstream& line_stream );

  /** @brief Parse a line describing a decoding table. */
  void parse_decoding_table ( std::istringstream& line_stream );

  /** @brief Parse a line describing a chemical. */
  void parse_chemical ( std::istringstream& line_stream );

  /** @brief Parse a line describing a bound chemical. */
  void parse_bound_chemical ( std::istringstream& line_stream );

  /** @brief Parse a line describing a base loader. */
  void parse_base_loader ( std::istringstream& line_stream );

  /** @brief Parse a line describing a chemical sequence. */
  void parse_chemical_sequence ( std::istringstream& line_stream );

  /** @brief Parse a line describing a processive chemical. */
  void parse_processive_chemical ( std::istringstream& line_stream );

  /** @brief Parse a line describing a base loading. */
  void parse_base_loading ( std::istringstream& line_stream );

  /** @brief Parse a line describing a chemical reaction. */
  void parse_chemical_reaction ( std::istringstream& line_stream );

  /** @brief Parse a line describing a complexation. */
  void parse_complexation ( std::istringstream& line_stream );

  /** @brief Parse a line describing an elongation. */
  void parse_elongation ( std::istringstream& line_stream );

  /** @brief Parse a line describing a binding. */
  void parse_binding ( std::istringstream& line_stream );

  /** @brief Parse a line describing a release. */
  void parse_release ( std::istringstream& line_stream );
};

// ======================
//  Inline declarations
// ======================
//

#endif // PARSER_H
