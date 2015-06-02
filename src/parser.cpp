

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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "parser.h"
#include "bindable.h"
#include "chemical.h"
#include "boundchemical.h"
#include "processivechemical.h"
#include "chemicalhandler.h"
#include "reactionhandler.h"
#include "bindingsitehandler.h"
#include "terminationsitehandler.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
Parser::Parser (ChemicalHandler& chemical_handler, ReactionHandler& reaction_handler,
		BindingSiteHandler& binding_site_handler, TerminationSiteHandler& termination_site_handler)
  : _chemical_handler (chemical_handler)
  , _reaction_handler (reaction_handler)
  , _binding_site_handler (binding_site_handler)
  , _termination_site_handler (termination_site_handler)
  , _read (0)
  , _line (0)
{
}

// Not needed for this class (use of default copy constructor) !
// Parser::Parser ( const Parser& other_parser );

Parser::~Parser (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Parser::parse_units ( const char* filename )
{
  std::ifstream file (filename);
  std::string line;

  // we read the file twice to solve undefined references to chemicals
  // defined after they are first referenced

  // first loopthrough
  _read = 1;
  _line = 0;
  _lines_to_reread.clear();
  while (std::getline (file, line))
    {
      _line++;
      parse_unit_line (line);
    }

  // rewind file and reread undefined lines
  file.clear();
  file.seekg(0); // rewind
  _read = 2;
  _line = 0;
  std::list< int >::iterator next_line = _lines_to_reread.begin();
  while (next_line != _lines_to_reread.end())
    {
      while (_line != *next_line)
	{
	  std::getline (file, line);
	  _line++;
	}
      parse_unit_line (line);
      next_line++;
    }
  
  file.close ();
}

void Parser::parse_reactions ( const char* filename )
{
  std::ifstream file (filename);
  std::string line;

  _read = 1;
  _line = 0;
  while (std::getline (file, line))
    {
      _line++;
      parse_reaction_line (line);
    }

  file.close ();
}


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
// Parser& Parser::operator= ( const Parser& other_parser );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Parser::check_invariant (void) const
{
  bool result = true;
  return result;
}

// =================
//  Private Methods
// =================
//
void Parser::parse_unit_line (std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate parser
  std::string word;
  std::string CL ("Chemical");
  std::string BS ("BindingSite");
  std::string TS ("TerminationSite");
  std::string BC ("BoundChemical");
  std::string CS ("ChemicalSequence");
  std::string PC ("ProcessiveChemical");
  std::istringstream line_stream (line);
  if (line_stream >> word)
    {
      if (word==std::string("unit"))
	{
	  line_stream >> word;
	  if ( word == BS )
	    {
	      parse_binding_site (line_stream);
	    }
	  else if ( word == CL )
	    {
	      parse_chemical (line_stream);
	    }
	  else if ( word == TS )
	    {
	      parse_termination_site (line_stream);
	    }
	  else if ( word == BC )
	    {
	      parse_bound_chemical (line_stream);
	    }
	  else if ( word == CS )
	    {
	      parse_chemical_sequence (line_stream);
	    }
	  else if ( word == PC )
	    {
	      parse_processive_chemical (line_stream);
	    }
	  else
	    {
	      std::cerr << "ERROR (line " << _line << "): " << word
			<< " does NOT define a unit."
			<< std::endl;
	    }
	}
    }
}

void Parser::parse_reaction_line (std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate parser
  std::string word;
  std::string CR ("ChemicalReaction");
  std::string CN ("Complexation");
  std::string BG ("Binding");
  std::string EN ("Elongation");
  std::istringstream line_stream (line);
  if (line_stream >> word)
    {
      if (word==std::string("reaction"))
	{
	  line_stream >> word;
	  if ( word == CR )
	    {
	      parse_chemical_reaction (line_stream);
	    }
	  else if ( word == CN )
	    {
	      parse_complexation (line_stream);
	    }
	  else if ( word == BG )
	    {
	      parse_binding (line_stream);
	    }
	  else if ( word == EN )
	    {
	      parse_elongation (line_stream);
	    }
	  else
	    {
	      std::cerr << "ERROR (line " << _line << "): " << word
			<< " does NOT define a unit."
			<< std::endl;
	    }
	}
    }
}


void Parser::parse_binding_site ( std::istringstream& line_stream )
{
  // read base data
  std::string family_name;
  std::string location;
  int position;
  int length;
  double k_on;
  double k_off;
  if ( not (line_stream >> family_name >> location >> position >> length >> k_on >> k_off) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check whether the location is known
  Bindable* bindable = 0;
  if ( _chemical_handler.exists (location) )
    {
      bindable = dynamic_cast<Bindable*> (&_chemical_handler.reference (location));
      if (bindable == 0)
	{
	  std::cerr << "ERROR (line " << _line << "): chemical " << location
		    << " is not bindable and cannot be used to define" 
		    << " a binding site." << std::endl;
	  return;	  
	}
    }
  else
    {
      // if not, we need to reread the file (maybe the location is defined later)
      if (_read > 1) // we check that we indeed read the file only once
	{
	  std::cerr << "ERROR (line " << _line << "): trying to define binding site " << family_name
		    << " with unknown location " << location
		    << ". Please define the location explicitly somewhere in the "
		    << "input file." << std::endl;
	}
      else
	{
	  // we add the line to the list of lines to read again
	  _lines_to_reread.push_back (_line);
	}
      return; // stop parsing in any case
    }

  _binding_site_handler.create_site (family_name, *bindable, position, length, k_on, k_off);
}


void Parser::parse_termination_site ( std::istringstream& line_stream )
{
  // read base data
  std::string family_name;
  std::string location;
  int position;
  int length;
  if ( not (line_stream >> family_name >> location >> position >> length) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check whether the location is known
  Bindable* bindable = 0;
  if ( _chemical_handler.exists (location) )
    {
      bindable = dynamic_cast<Bindable*> (&_chemical_handler.reference (location));
      if (bindable == 0)
	{
	  std::cerr << "ERROR (line " << _line << "): chemical " << location
		    << " is not bindable and cannot be used to define" 
		    << " a termination site." << std::endl;
	  return;	  
	}
    }
  else
    {
      // if not, we need to reread the file (maybe the location is defined later)
      if (_read > 1) // we check that we indeed read the file only once
	{
	  std::cerr << "ERROR (line " << _line << "): trying to define termination site " << family_name
		    << " with unknown location " << location
		    << ". Please define the location explicitly somewhere in the "
		    << "input file." << std::endl;
	}
      else
	{
	  // we add the line to the list of lines to read again
	  _lines_to_reread.push_back (_line);
	}
      return; // stop parsing in any case
    }

  _termination_site_handler.create_site (family_name, *bindable, position, length);
}

void Parser::parse_chemical ( std::istringstream& line_stream )
{
  // read base data
  std::string name;
  if ( not (line_stream >> name ) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  int initial_quantity;
  if ( not (line_stream >> initial_quantity))
    {
      initial_quantity = 0;
    }

  if (not _chemical_handler.exists (name) )
    {
      _chemical_handler.create_chemical (name, initial_quantity);
    }
}

void Parser::parse_chemical_sequence ( std::istringstream& line_stream )
{
  // read base data
  std::string name;
  int length;
  if ( not (line_stream >> name >> length) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  int initial_quantity;
  if ( not (line_stream >> initial_quantity))
    {
      initial_quantity = 0;
    }

  if (not _chemical_handler.exists (name) )
    {
      _chemical_handler.create_chemical_sequence (name, length, initial_quantity);
    }
}

void Parser::parse_bound_chemical ( std::istringstream& line_stream )
{
  // read base data
  std::string name;
  if ( not (line_stream >> name) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  int initial_quantity;
  if ( not (line_stream >> initial_quantity))
    {
      initial_quantity = 0;
    }

  if (not _chemical_handler.exists (name) )
    {
      _chemical_handler.create_bound_chemical (name, initial_quantity);
    }
}

void Parser::parse_processive_chemical ( std::istringstream& line_stream )
{
  // read base data
  std::string name;
  std::string stalled_name;
  if ( not (line_stream >> name >> stalled_name) )
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check whether the stalled form is known
  if ( not _chemical_handler.exists (stalled_name) )
    {
      // if not, we need to reread the file (maybe the stalled form is defined later)
      if (_read > 1) // we check that we indeed read the file only once
	{
	  std::cerr << "ERROR (line " << _line << "): trying to define processive chemical " << name
		    << " with unknown stalled form " << stalled_name
		    << ". Please define the stalled form explicitly somewhere in the "
		    << "input file." << std::endl;
	}
      else
	{
	  // we add the line to the list of lines to read again
	  _lines_to_reread.push_back (_line);
	}
      return; // stop parsing in any case
    }

  if (not _chemical_handler.exists (name) )
    {
      _chemical_handler.create_processive_chemical (name, stalled_name, 0);
    }

  // parse termination sites
  std::string site_name;
  ProcessiveChemical* chemical = dynamic_cast< ProcessiveChemical* > (&_chemical_handler.reference (name));
  while (line_stream >> site_name)
    {
      // check whether termination site is already known
      int site_id = _termination_site_handler.retrieve_id (site_name);
      if ( site_id != TerminationSiteHandler::NOT_FOUND )
	{
	  chemical->add_recognized_termination_site (site_id);
	}
      else
	{
	  // if not, we need to reread the file (maybe the site is defined later)
	  if (_read > 1) // we check that we indeed read the file only once
	    {
	      std::cerr << "ERROR (line " << _line << "): trying to define processive chemical " << name
			<< " with unknown termination site " << site_name
			<< ". Please define the termination site explicitly somewhere in the "
			<< "input file." << std::endl;
	    }
	  else
	    {
	      // we add the line to the list of lines to read again
	      _lines_to_reread.push_back (_line);
	    }
	}
    }
}


void Parser::parse_chemical_reaction ( std::istringstream& line_stream )
{
  // read base data
  std::list< Chemical* > chemicals;
  std::list< int > stoichiometries;
  std::string chemical;
  int stoichiometry;
  while (line_stream >> chemical >> stoichiometry)
    {
      // check that the chemical is known
      if (_chemical_handler.exists (chemical))
	{
	  chemicals.push_back (&_chemical_handler.reference(chemical));
	  stoichiometries.push_back (stoichiometry);
	}
      else
	{
	  std::cerr << "ERROR (line " << _line << "): unknown chemical " << chemical
		    << " in chemical reaction." << std::endl;
	  return;
	}
    }
  double k_1, k_m1;
  if ((chemicals.size() == 0) || (not (line_stream >> k_1 >> k_m1)))
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // transform lists in vectors
  std::vector< Chemical* > chemicals_vector (chemicals.size(), 0);
  int i = 0;
  for ( std::list<Chemical*>::iterator next_chemical = chemicals.begin();
	next_chemical != chemicals.end();
	next_chemical++, i++ )
    {
      chemicals_vector[i] = *next_chemical;
    }
  std::vector< int > stoichiometries_vector (stoichiometries.size(), 0);
  i = 0;
  for ( std::list<int>::iterator next_coef = stoichiometries.begin();
	next_coef != stoichiometries.end();
	next_coef++, i++ )
    {
      stoichiometries_vector[i] = *next_coef;
    }
  

  // create reaction
  _reaction_handler.create_chemical_reaction (chemicals_vector, stoichiometries_vector, k_1, k_m1);
}


void Parser::parse_complexation ( std::istringstream& line_stream )
{
  // read base data
  std::string component_a;
  std::string component_b;
  std::string complex;
  double k_on;
  double k_off;
  if (not (line_stream >> component_a >> component_b >> complex >> k_on >> k_off))
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check that the chemicals are known
  if (not _chemical_handler.exists (component_a))
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << component_a << std::endl;
      return;
    }
  if (not _chemical_handler.exists (component_b))
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << component_b << std::endl;
      return;
    }
  if (not _chemical_handler.exists (complex))
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << complex << std::endl;
      return;
    }

  // create reaction
  _reaction_handler.create_complexation ( _chemical_handler.reference(component_a),
					  _chemical_handler.reference(component_b),
					  _chemical_handler.reference(complex),
					  k_on, k_off );
}

void Parser::parse_elongation ( std::istringstream& line_stream )
{
  // read base data
  std::string chemical;
  int step_size ;
  double rate;
  if (not (line_stream >> chemical >> step_size >> rate))
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check that the chemical is valid
  ProcessiveChemical* processive_chemical = 0;
  if (_chemical_handler.exists (chemical))
    {
      processive_chemical =
	dynamic_cast< ProcessiveChemical* > (&_chemical_handler.reference(chemical));
      if (processive_chemical == 0)
	{
	  std::cerr << "ERROR (line " << _line << "): chemical " << chemical
		    << " is not a processive chemical and cannot be used to define" 
		    << " an elongation reaction." << std::endl;
	  return;	  
	}
    }
  else
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << chemical << std::endl;
      return;
    }

  // create reaction
  _reaction_handler.create_elongation (*processive_chemical, step_size, rate);
}

void Parser::parse_binding ( std::istringstream& line_stream )
{
  // read base data
  std::string unit_to_bind;
  std::string binding_result;
  std::string binding_site;
  if (not (line_stream >> unit_to_bind >> binding_result >> binding_site))
    {
      std::cerr << "ERROR (line " << _line << "): corrupt data, ignoring line." << std::endl;
      return;
    }

  // check that the data is valid
  BoundChemical* bound_chemical = 0;
  int binding_site_family;
  if (not _chemical_handler.exists (unit_to_bind))
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << unit_to_bind << std::endl;
      return;
    }
  if (_chemical_handler.exists (binding_result))
    {
      bound_chemical =
	dynamic_cast< BoundChemical* > (&_chemical_handler.reference(binding_result));
      if (bound_chemical == 0)
	{
	  std::cerr << "ERROR (line " << _line << "): chemical " << binding_result
		    << " is not a bound chemical and cannot be used to define" 
		    << " a binding result ." << std::endl;
	  return;	  
	}
    }
  else
    {
      std::cerr << "ERROR (line " << _line << "): unknown chemical " << binding_result << std::endl;
      return;
    }
  if (_binding_site_handler.exists (binding_site))
    {
      binding_site_family = _binding_site_handler.retrieve_id (binding_site);
    }
  else
    {
      std::cerr << "ERROR (line " << _line << "): unknown binding site " << binding_site << std::endl;
      return;
    }

  // create reaction
  _reaction_handler.create_binding (_chemical_handler.reference(unit_to_bind),
				    *bound_chemical, binding_site_family);
}
