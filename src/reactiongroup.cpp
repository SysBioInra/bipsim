

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


// =================
//  Protected Methods
// =================
//

void ReactionGroup::perform_reaction (int rate_index)
{
  // compute next reaction to perform
  Reaction& reaction = *_reactions [rate_index/2];
  
  // perform reaction
  if ( (rate_index % 2) == 0 )
    {
      if (reaction.is_forward_reaction_possible())
	{
	  reaction.perform_forward();
	}
    }
  else
    {
      if (reaction.is_backward_reaction_possible())
	{
	  reaction.perform_backward();
	}
    }
}

// =================
//  Private Methods
// =================
//
