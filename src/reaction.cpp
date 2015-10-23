

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
  : _rate (0)
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

std::ostream& operator<< (std::ostream& output, const Reaction& reaction)
{
  reaction.print (output);
  return output;
}


// =================
//  Private Methods
// =================
//
