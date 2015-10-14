

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
#include "chemicalsequence.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSite::BindingSite (int family_id, ChemicalSequence& location, int position,
			  int length, double k_on, double k_off,
			  int reading_frame /*= NO_READING_FRAME*/)
  : Site (family_id, location, position, length)
  , _k_on (k_on)
  , _k_off (k_off)
  , _reading_frame (reading_frame)
{
  /** @pre Reading frame must be within site. */
  REQUIRE (reading_frame < position + length);
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

// =================
//  Private Methods
// =================
//
