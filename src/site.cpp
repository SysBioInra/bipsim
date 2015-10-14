

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
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Site::Site (int family_id, ChemicalSequence& location, int position, int length)
  : _family (family_id)
  , _location (location)
  , _position (position)
  , _length (length)
{
  /** @pre Site must be within location.*/
  REQUIRE (location.is_out_of_bounds (position, length) == false);
}

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


// =================
//  Private Methods
// =================
//
