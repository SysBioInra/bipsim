

/**
 * @file reactionclassification.cpp
 * @brief Implementation of the ReactionClassification class.
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
#include "reactionclassification.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionClassification::ReactionClassification (void)
  : _current_size (0)
{
}

// Not needed for this class (use of default copy constructor) !
// ReactionClassification::ReactionClassification ( const ReactionClassification& other_reaction_classification );

ReactionClassification::~ReactionClassification (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
int ReactionClassification::create_new_class (double time_step)
{
  /** @pre time_step must be strictly positive or equal to ALWAYS_UPDATED. */
  REQUIRE ((time_step > 0) || (time_step == ALWAYS_UPDATED));

  // increase current number of classes
  ++_current_size;

  // create new list in the reaction vector
  _reaction_classification.resize (_current_size);

  // add time step in the _time_steps vector
  _time_steps.push_back (time_step);

  // return class identifier (index of the class in the vectors)
  return _current_size - 1;
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
// ReactionClassification& ReactionClassification::operator= ( const ReactionClassification& other_reaction_classification );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ReactionClassification::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
