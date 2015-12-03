

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
BindingSite::BindingSite (int family_id, ChemicalSequence& location, int first,
			  int last, double k_on, double k_off,
			  int reading_frame /*= NO_READING_FRAME*/)
  : Site (family_id, location, first, last)
  , _k_on (k_on)
  , _k_off (k_off)
  , _reading_frame (reading_frame)
{
  /** @pre If defined, reading frame must be within site. */
  REQUIRE ((reading_frame == NO_READING_FRAME)
	   || ((reading_frame >= first) && (reading_frame <= last)));
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
