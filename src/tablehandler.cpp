

/**
 * @file tablehandler.cpp
 * @brief Implementation of the TableHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "tablehandler.h"
#include "decodingtable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
TableHandler::TableHandler (void)
  : _last_id (NOT_FOUND)
  , _last_reference (0)
{
}

// Not needed for this class (use of default copy constructor) !
// TableHandler::TableHandler ( const TableHandler& other_table_handler );

TableHandler::~TableHandler (void)
{
  std::map< int, DecodingTable* >::iterator table = _references.begin();
  while ( table != _references.end() )
    {
      delete table->second;
      table++;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//

void TableHandler::create_decoding_table (std::string table_name, const std::list<std::string>& template_list, const std::list<Chemical*>& base_list, const std::list<double>& rate_list)
{
  /** @pre Table should not be defined already. */
  REQUIRE( _identifiers.exists (table_name) == false );
  REQUIRE( template_list.size() > 0 ); /** @pre There should be at least one element in the table. */
  REQUIRE( template_list.size() == base_list.size() ); /** @pre The number of templates and bases must be equal. */
  
  // get a new id for the entity
  _identifiers.create_id (table_name);
  _last_id = _identifiers.id (table_name);
  
  // create it and store the reference
  std::list<std::string>::const_iterator template_it = template_list.begin();
  std::list<Chemical*>::const_iterator base_it = base_list.begin();
  std::list<double>::const_iterator rate_it = rate_list.begin();
  int template_length = template_it->size();
  DecodingTable* new_table = new DecodingTable (template_length);
  while (template_it != template_list.end())
    {
      if (template_it->size() == template_length)
	{
	  new_table->add_template (*template_it, **base_it, *rate_it);
	}
      else
	{
	  std::cerr << "ERROR: trying to define a decoding table with templates of variable "
		    << "length." << std::endl;
	}
      template_it++; base_it++; rate_it++;
    }
  _references [_last_id] = new_table;
  _last_reference = new_table;
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
// TableHandler& TableHandler::operator= ( const TableHandler& other_table_handler );

std::ostream& operator<< (std::ostream& output, const TableHandler& table_handler)
{
  for (std::map<int, DecodingTable*>::const_iterator next_reference = table_handler._references.begin();
       next_reference != table_handler._references.end();
       next_reference++ )
    {
      output << table_handler._identifiers.name (next_reference->first) << ": "
	     << *(next_reference->second) << std::endl;
    }
  
  return output;
}

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool TableHandler::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
