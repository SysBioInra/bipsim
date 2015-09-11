

/**
 * @file event.cpp
 * @brief Implementation of the Event class.
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
#include "event.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Event::Event (double time, Chemical& target)
  : _time (time)
  , _target (target)
{
  REQUIRE (time>0); /** @pre time must be positive. */
}

// Not needed for this class (use of default copy constructor) !
// Event::Event (const Event& other_class_name);

Event::~Event (void)
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
// Event& Event::operator= (const Event& other_class_name);


// =================
//  Private Methods
// =================
//
