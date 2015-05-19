

/**
 * @file bindable.cpp
 * @brief Implementatino of the Bindable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "bindable.h"
#include "forwarddeclarations.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Bindable::Bindable (void)
{
  _length = 1;
}

// Not needed for this class (use of default copy constructor) !
// Bindable::Bindable (Bindable& other_bindable);

Bindable::~Bindable (void)
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
// Bindable& Bindable::operator= (Bindable& other_bindable);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Bindable::check_invariant (void)
{
  bool result = true;
  result = result && (_length > 0); /** Length must be positive (>0). */
  return result;
}


// =================
//  Private Methods
// =================
//
