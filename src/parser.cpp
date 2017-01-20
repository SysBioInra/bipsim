

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
#include <stdexcept> // std::runtime_error

// ==================
//  Project Includes
// ==================
//
#include "parser.h"
#include "cellstate.h"
#include "inputdata.h"
#include "simulatorexception.h"
#include "reactionbuilder.h"
#include "reactantbuilder.h"
#include "tablebuilder.h"
#include "eventbuilder.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Parser::Parser (CellState& cell_state, EventHandler& event_handler)
{
  // reactant builder
  _builders.push_back (new BindingSiteBuilder (cell_state));
  _builders.push_back (new SwitchBuilder (cell_state));
  _builders.push_back (new SwitchSiteBuilder (cell_state));
  _builders.push_back (new FreeChemicalBuilder (cell_state));
  _builders.push_back (new BoundChemicalBuilder (cell_state));
  _builders.push_back (new ChemicalSequenceBuilder (cell_state));
  _builders.push_back (new DoubleStrandBuilder (cell_state));
  // table builder
  _builders.push_back (new CompositionTableBuilder (cell_state));
  _builders.push_back (new LoadingTableBuilder (cell_state));
  _builders.push_back (new ProductTableBuilder (cell_state));
  _builders.push_back (new TransformationTableBuilder (cell_state));
  // reaction builder
  _builders.push_back (new ChemicalReactionBuilder (cell_state));
  _builders.push_back (new ProductLoadingBuilder (cell_state));
  _builders.push_back (new DoubleStrandLoadingBuilder (cell_state));
  _builders.push_back (new TranslocationBuilder (cell_state));
  _builders.push_back (new SequenceBindingBuilder (cell_state));
  _builders.push_back (new ReleaseBuilder (cell_state));
  _builders.push_back (new DegradationBuilder (cell_state));
  _builders.push_back (new DoubleStrandRecruitmentBuilder (cell_state));
  // event builder
  _builders.push_back (new EventBuilder (cell_state, event_handler));
}

// Parser::Parser (const Parser& other_parser);
// Parser& Parser::operator= (const Parser& other_parser);

Parser::~Parser (void)
{
  for (std::vector <Builder*>::iterator it = _builders.begin();
       it != _builders.end(); ++it)
    { delete *it; }
}

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
  bool entity_created = true;
  bool display_dependency_errors = false;
  bool dependency_errors = false;
  while (entity_created || display_dependency_errors)
    {
      input_data.rewind();
      entity_created = false;
      while (input_data.is_eof()==false)
	{
	  try
	    {
	      bool success = false;
	      std::vector <Builder*>::iterator it = _builders.begin();
	      while (success == false && it != _builders.end())
		{ success = (*it)->match (input_data.line()); ++it; }
	      if (success)
		{ entity_created = true; input_data.mark_line_as_treated(); }
	      else 
		{ throw FormatException(); }
	    }
	  catch (const ParserException& error)
	    {
	      std::ostringstream msg;
	      msg << "PARSING ERROR (file " << input_data.file_name()
		  << ", line " << input_data.line_number() << "): "
		  << error.what();
	      input_data.mark_line_as_treated();
	      throw ParserException (msg.str());
	    }
	  catch (const DependencyException& error)
	    {
	      if (display_dependency_errors)
		{
		  dependency_errors = true;
		  std::cerr << "DEPENDENCY ERROR (file " 
			    << input_data.file_name() << ", line " 
			    << input_data.line_number() << "): "
			    << error.what() << "." << std::endl;
		  input_data.mark_line_as_treated();
		}
	    }
	  input_data.go_next();
	}
      if (entity_created == false) 
	{ display_dependency_errors = !display_dependency_errors; }
    }
  if (dependency_errors)
    { throw std::runtime_error ("could not solve dependencies."); }
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
