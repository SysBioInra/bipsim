

/**
 * @file bindingsite.cpp
 * @brief Implementation of the BindingSite class.
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
#include "bindingsite.h"
#include "bindable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSite::BindingSite (void)
{
  _family = 0;
  _location = 0;
  _position = 1;
  _length = 1;
  _affinity = 0;
}

// Not needed for this class (use of default copy constructor) !
// BindingSite::BindingSite (BindingSite& other_binding_site);

BindingSite::~BindingSite (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//
int BindingSite::number_available_sites (void)
{
  REQUIRE( _location != 0 ); /** @pre Binding site location must be initializied */

  int result = _location->number_available_sites( _position, _length );

  ENSURE( result >= 0 ); /** @post Returned value >= 0 */

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
// BindingSite& BindingSite::operator= (BindingSite& other_binding_site);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BindingSite::check_invariant (void)
{
  bool result = ( _affinity >= 0); /** Affinity must be nonnegative. */
  result = result && ( _length > 0 ); /** Length must be positive. */
  result = result && ( _position > 0 ); /** Position must positive. */
  return result;
}


// =================
//  Private Methods
// =================
//
