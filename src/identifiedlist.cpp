

/**
 * @file identifiedlist.cpp
 * @brief Implementation of the IdentifiedList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "identifiedlist.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
// Not needed for this class (use of default copy constructor) !
// IdentifiedList::IdentifiedList (IdentifiedList& other_identified_list);

IdentifiedList::~IdentifiedList (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void IdentifiedList::create_id ( std::string element_name )
{
  int old_size = _identifiers.size();

  // try adding family to the map
  _identifiers.insert ( std::pair<std::string,int> ( element_name, _last_attributed_id + 1 ) );

  // if it was already in the map, nothing was done,
  // so we only update the last attribute id if the size has changed
  _last_attributed_id += _identifiers.size() - old_size;
}


// ============================
//  Public Methods - Accessors
// ============================
//
int IdentifiedList::id ( std::string element_name) const
{
  // We initialize the return value to "not found"
  int result = IdentifiedList::NOT_FOUND;

  // Look for element_name in the map
  IdMap::const_iterator it = _identifiers.find ( element_name );
  if ( it != _identifiers.end() ) { result = it->second; }

  return result;
}

bool IdentifiedList::exists (std::string element_name) const
{
  // Look for element_name in the map
  return ( _identifiers.count ( element_name ) > 0 );
}

std::string IdentifiedList::name (int element_id) const
{
  std::string result = "";

  // Look for element_id in the map
  bool found = false;
  IdMap::const_iterator it = _identifiers.begin ();
  while ( ( found == false ) &&  ( it != _identifiers.end() ) )
    {
      if ( it->second == element_id )
	{
	  found = true;
	  result = it->first;
	}
      it++;
    }

  return result;
}


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// IdentifiedList& IdentifiedList::operator= (IdentifiedList& other_identified_list);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool IdentifiedList::check_invariant (void) const
{
  bool result = true;
  return result; /** No invariant. */
}


// =================
//  Private Methods
// =================
//
