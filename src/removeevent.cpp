

/**
 * @file removeevent.cpp
 * @brief Implementation of the RemoveEvent class.
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
#include "removeevent.h"
#include "chemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RemoveEvent::RemoveEvent (double time, Chemical& target, int quantity)
  : Event (time, target)
  , _quantity (quantity)
{
  REQUIRE (quantity > 0); /** @pre Quantity must be strictly positive. */
}

// Not needed for this class (use of default copy constructor) !
// RemoveEvent::RemoveEvent (const RemoveEvent& other_remove_event);

RemoveEvent::~RemoveEvent (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void RemoveEvent::perform (void)
{
  if (_quantity < target().number())
    { target().remove (_quantity); }
  else
    { target().remove (target().number()); }
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
// RemoveEvent& RemoveEvent::operator= (const RemoveEvent& other_remove_event);


// =================
//  Private Methods
// =================
//
