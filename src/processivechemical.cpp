

/**
 * @file processivechemical.cpp
 * @brief Implementation of the ProcessiveChemical class.
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
#include "processivechemical.h"
#include "randomhandler.h"
#include "chemicalsequence.h"
#include "bindingsite.h"
#include "site.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
// Not needed for this class (use of default copy constructor) !
// ProcessiveChemical::ProcessiveChemical (ProcessiveChemical& other_processive_chemical);

ProcessiveChemical::~ProcessiveChemical (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

void ProcessiveChemical::step_forward ( int step_size )
{
  // move the focused chemical
  _focused_unit->second += step_size;
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool ProcessiveChemical::is_terminating ( void )
{
  const Bindable& focused_unit_location = (_focused_unit->first)->location();
  return focused_unit_location.is_termination_site ( _focused_unit->second,
						     _termination_site_families );
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
// ProcessiveChemical& ProcessiveChemical::operator= (ProcessiveChemical& other_processive_chemical);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ProcessiveChemical::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
