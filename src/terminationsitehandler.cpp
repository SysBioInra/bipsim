

/**
 * @file terminationsitehandler.cpp
 * @brief Implementation of the TerminationSiteHandler class.
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
#include "terminationsitehandler.h"
#include "bindable.h"
#include "macros.h"
#include "site.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
TerminationSiteHandler::~TerminationSiteHandler (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void TerminationSiteHandler::create_site ( std::string family_name, Bindable& location,
					   int position, int length )
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Site must fit on bindable element 
   *   (position+length <= location->length() ). */
  REQUIRE( position + length <= location.length() );

  // get family identifier
  if ( _family_ids.exists ( family_name ) == false )
    {
      _family_ids.create_id ( family_name );
    }
  int family_id = _family_ids.id ( family_name );

  // create site
  Site* site = new Site ( family_id, location, position, length );
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a list is automatically created
  _families[ family_id ].push_back ( site );

  // add the termination site on the bindable element
  location.add_termination_site ( *site );
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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool TerminationSiteHandler::check_invariant ( void ) const
{
  bool result = SiteHandler::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
