

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
ReactionGroup::ReactionGroup (const std::vector<Reaction*>& reactions)
  : _reactions (reactions)
  , _number_reactions (reactions.size())
  , _next_reaction_time (0)
  , _rates (2*reactions.size(), 0)
  , _total_rate (0)
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
void ReactionGroup::update_all_rates (void)
{
  _total_rate = 0;
  for ( int i = 0; i < _number_reactions; i++ )
    {
      // update rates
      _reactions [i]->update_rates();

      // store the rates
      _rates[2*i] = _reactions[i]->forward_rate();
      _rates[2*i+1] = _reactions[i]->backward_rate();
      _total_rate += _rates[2*i] + _rates[2*i+1];
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
  int reaction_index = rate_index / 2;
  
  // perform reaction
  if ( (rate_index % 2) == 0 )
    {
      _reactions[reaction_index]->perform_forward();
    }
  else
    {
      _reactions[reaction_index]->perform_backward();
    }
}

// =================
//  Private Methods
// =================
//
