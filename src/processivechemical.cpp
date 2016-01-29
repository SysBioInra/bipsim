

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
ProcessiveChemical::ProcessiveChemical (BoundChemical& stalled_form)
  :_stalled_form (stalled_form) 
{
}

// Not needed for this class (use of compiler generated versions)
// ProcessiveChemical::ProcessiveChemical (ProcessiveChemical& other_chemical);
// ProcessiveChemical& ProcessiveChemical::operator= (ProcessiveChemical& other_chemical);
// ProcessiveChemical::~ProcessiveChemical (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

void ProcessiveChemical::step_forward (int step_size)
{
  // move the focused chemical
  _focused_unit->move (step_size);
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool ProcessiveChemical::is_terminating (void)
{
  return focused_unit_location().is_termination_site
    (focused_unit_reading_frame(), _termination_sites);
}


// =================
//  Private Methods
// =================
//
