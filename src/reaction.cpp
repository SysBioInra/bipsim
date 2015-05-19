

/**
 * @file reaction.cpp
 * @brief Implementation of the Reaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "reaction.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Reaction::Reaction (void)
{
}

// Not needed for this class (use of default copy constructor) !
// Reaction::Reaction (Reaction& other_reaction);

Reaction::~Reaction (void)
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
// Reaction& Reaction::operator= (Reaction& other_reaction);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Reaction::check_invariant (void)
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
