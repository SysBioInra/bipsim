

/**
 * @file site.cpp
 * @brief Implementation of the Site class.
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
#include "site.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
// Not needed for this class (use of default copy constructor) !
// Site::Site (Site& other_site);

Site::~Site (void)
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
int Site::number_available_sites (void) const
{
  int result = _location.number_available_sites( _position, _length );

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
// Site& Site::operator= (Site& other_site);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Site::check_invariant (void) const
{
  bool result = _length > 0 /** Length must be positive. */
    && ( _position > 0 ); /** Position must positive. */
  return result;
}


// =================
//  Private Methods
// =================
//
