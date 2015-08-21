

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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BindingSite::check_invariant (void) const
{
  bool result = Site::check_invariant()
    && ( _k_on > 0 ) /** on-rate must be positive. */
    && ( _k_off > 0 ); /** off-rate must be positive. */
  return result;
}


// =================
//  Private Methods
// =================
//
