

/**
 * @file chemical.cpp
 * @brief Implementation of the Chemical class.
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
#include "chemical.h"
#include "forwarddeclarations.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Chemical::Chemical (void)
{
  _number = 0;
}

// Not needed for this class (use of default copy constructor) !
// Chemical::Chemical (Chemical& other_Chemical);

Chemical::~Chemical (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Chemical::print (std::ostream& output) const
{
  output << "Chemical with currently " << _number << " molecules.";
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
// Chemical& Chemical::operator= (Chemical& other_Chemical);


// =================
//  Private Methods
// =================
//
