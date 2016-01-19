

/**
 * @file parser.cpp
 * @brief Implementation of the Parser class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <fstream> // std::ifstream
#include <sstream> // std::istringstream
#include <string> // std::string
#include <stdexcept> // std::runtime_error

// ==================
//  Project Includes
// ==================
//
#include "parser.h"
#include "factory.h"
#include "cellstate.h"
#include "inputdata.h"
#include "simulatorexception.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Parser::Parser (CellState& cell_state, EventHandler& event_handler)
  : _unit_factory (cell_state)
  , _reaction_factory (cell_state)
  , _event_factory (cell_state, event_handler)
{
}

// Not needed for this class (use of compiler generated versions)
// Parser::Parser (const Parser& other_parser);
// Parser& Parser::operator= (const Parser& other_parser);
// Parser::~Parser (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void Parser::parse (InputData& input_data)
{
  // we loop through the data until no creation takes place anymore
  // the idea is that as long as dependencies may not have been resolved
  // we need to try to create everything again
  std::cout << "Parsing data..." << std::endl;
  while (loop_through_data (input_data) == true) {}
  if (display_dependency_errors (input_data))
    {
      throw std::runtime_error ("corrupted input data");
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
bool Parser::loop_through_data (InputData& input_data)
{
  bool entity_created = false;
  bool success = false;
  input_data.rewind();
  while (input_data.is_eof()==false)
    {
      try
	{
	  success = _unit_factory.handle (input_data.line())
	    || _reaction_factory.handle (input_data.line())
	    || _event_factory.handle (input_data.line());

	  if (success)
	    {
	      entity_created = true;
	      input_data.mark_line_as_treated();
	    }
	}
      catch (const ParserException& error)
	{
	  std::cerr << "PARSING ERROR (file " << input_data.file_name()
		    << ", line " << input_data.line_number() << "): "
		    << error.what() << "." << std::endl;
	  input_data.mark_line_as_treated();
	}
      catch (const DependencyException& error)
	{
	  // ignore dependency errors as some entities/reactions have not been
	  // created yet
	}
      
      input_data.go_next();
    }

  return entity_created;
}

bool Parser::display_dependency_errors (InputData& input_data)
{
  bool error_occurred = false;
  input_data.rewind();
  while (input_data.is_eof()==false)
    {
      try
	{
	  if (not (_unit_factory.handle (input_data.line())
		   || _reaction_factory.handle (input_data.line())
		   || _event_factory.handle (input_data.line())))
	    {
	      error_occurred = true;
	      std::cerr << "ERROR (file " << input_data.file_name()
			<< ", line " << input_data.line_number() << "): "
			<< "line does not make sense." << std::endl;
	      input_data.mark_line_as_treated();
	    }
	}
      catch (const DependencyException& error)
	{
	  error_occurred = true;
	  std::cerr << "DEPENDENCY ERROR (file " << input_data.file_name()
		    << ", line " << input_data.line_number() << "): "
		    << error.what() << "." << std::endl;
	  input_data.mark_line_as_treated();
	}
      
      input_data.go_next();
    }
  return error_occurred;
}
