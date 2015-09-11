

/**
 * @file setevent.cpp
 * @brief Implementation of the SetEvent class.
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
#include "setevent.h"
#include "chemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SetEvent::SetEvent (double time, Chemical& target, int quantity)
  : Event (time, target)
  , _quantity (quantity)
{
  REQUIRE (quantity > 0); /** @pre Quantity must be strictly positive. */
}

// Not needed for this class (use of default copy constructor) !
// SetEvent::SetEvent (const SetEvent& other_set_event);

SetEvent::~SetEvent (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SetEvent::perform (void)
{
  int difference = _quantity - target().number();
  if (difference > 0)
    {
      target().add (difference);
    }
  else if (difference < 0)
    {
      target().remove (-difference);
    }
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
// SetEvent& SetEvent::operator= (const SetEvent& other_set_event);

// =================
//  Private Methods
// =================
//
