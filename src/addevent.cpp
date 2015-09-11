

/**
 * @file addevent.cpp
 * @brief Implementation of the AddEvent class.
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
#include "addevent.h"
#include "chemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
AddEvent::AddEvent (double time, Chemical& target, int quantity)
  : Event (time, target)
  , _quantity (quantity)
{
  REQUIRE (quantity > 0); /** @pre Quantity must be strictly positive. */
}

// Not needed for this class (use of default copy constructor) !
// AddEvent::AddEvent (const AddEvent& other_add_event);

AddEvent::~AddEvent (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void AddEvent::perform (void)
{
  target().add (_quantity);
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
// AddEvent& AddEvent::operator= (const AddEvent& other_add_event);



// =================
//  Private Methods
// =================
//
