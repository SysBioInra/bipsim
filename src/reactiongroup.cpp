

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
#include <limits> // std::numeric_limits

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


// ===================
//  Protected Methods
// ===================
//


// =================
//  Private Methods
// =================
//
