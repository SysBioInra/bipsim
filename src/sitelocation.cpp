

/**
 * @file sitelocation.cpp
 * @brief Implementation of the SiteLocation class.
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
#include "sitelocation.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SiteLocation::SiteLocation (void)
{
  _length = 1;
  _position = 1;
}

SiteLocation::SiteLocation ( int position, int length )
{
  _length = length;
  _position = position;
}

// Not needed for this class (use of default copy constructor) !
// SiteLocation::SiteLocation (SiteLocation& other_site_location);

SiteLocation::~SiteLocation (void)
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


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// SiteLocation& SiteLocation::operator= (SiteLocation& other_site_location);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool SiteLocation::check_invariant (void)
{
  bool result = ( _length > 0 ); /** Length must be positive. */
  result = result && ( _position > 0 ); /** Position must be positive. */
  return result;
}


// =================
//  Private Methods
// =================
//
