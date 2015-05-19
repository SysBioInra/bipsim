

/**
 * @file terminationsite.cpp
 * @brief Implementation of the TerminationSite class.
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
#include "terminationsite.h"
#include "forwarddeclarations.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
TerminationSite::TerminationSite (void)
{
}

// Not needed for this class (use of default copy constructor) !
// TerminationSite::TerminationSite (TerminationSite& other_termination_site);

TerminationSite::~TerminationSite (void)
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
// TerminationSite& TerminationSite::operator= (TerminationSite& other_termination_site);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool TerminationSite::check_invariant (void)
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
