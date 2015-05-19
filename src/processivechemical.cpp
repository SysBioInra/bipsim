

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
#include "forwarddeclarations.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ProcessiveChemical::ProcessiveChemical (void)
{
}

// Not needed for this class (use of default copy constructor) !
// ProcessiveChemical::ProcessiveChemical (ProcessiveChemical& other_processive_chemical);

ProcessiveChemical::~ProcessiveChemical (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ProcessiveChemical::handle_out_of_bounds (void)
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
}

void ProcessiveChemical::handle_termination_site (void)
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
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
bool ProcessiveChemical::check_invariant (void)
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
