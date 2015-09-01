

/**
 * @file reactiongroup.cpp
 * @brief Implementation of the ReactionGroup class.
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
#include "reactiongroup.h"
#include "reaction.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double ReactionGroup::OVERTIME = std::numeric_limits<double>::infinity();

ReactionGroup::ReactionGroup (const std::vector<Reaction*>& reactions)
  : _reactions (reactions)
  , _next_reaction_time (0)
{
}

// Not needed for this class (use of default copy constructor) !
// ReactionGroup::ReactionGroup ( const ReactionGroup& other_reaction_group );

ReactionGroup::~ReactionGroup (void)
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
// ReactionGroup& ReactionGroup::operator= ( const ReactionGroup& other_reaction_group );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ReactionGroup::check_invariant (void) const
{
  bool result = true;
  return result;
}


// ===================
//  Protected Methods
// ===================
//


// =================
//  Private Methods
// =================
//
