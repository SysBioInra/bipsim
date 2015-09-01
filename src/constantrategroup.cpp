

/**
 * @file constantrategroup.cpp
 * @brief Implementation of the ConstantRateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <vector> // std::vector
#include <algorithm> // std::sort

// ==================
//  Project Includes
// ==================
//
#include "constantrategroup.h"
#include "randomhandler.h"
#include "macros.h" // REQUIRE
#include "naiveratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ConstantRateGroup::ConstantRateGroup (const std::vector<Reaction*>& reactions, double initial_time, double time_step)
  : ReactionGroup (reactions)
  , _next_index (0)
  , _final_time (initial_time + time_step)
  , _time_step (time_step)
  , _rate_manager (reactions)
{
  reinitialize (initial_time);
}

// Not needed for this class (use of default copy constructor) !
// ConstantRateGroup::ConstantRateGroup ( const ConstantRateGroup& other_constant_rate_group );

ConstantRateGroup::~ConstantRateGroup (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool ConstantRateGroup::perform_next_reaction (void)
{
  /** @pre _next_reaction_time must not have reached OVERTIME (extending beyond valid time step). */
  REQUIRE (_next_reaction_time != OVERTIME);

  // perform next scheduled reaction if possible
  bool reaction_performed = false;
  int next_reaction_index = _reaction_rate_indices [_next_index];

  if (is_reaction_possible (next_reaction_index))
    {
      perform_reaction (next_reaction_index);
      reaction_performed = true;
    }

  // schedule next reaction
  ++_next_index;
  _next_reaction_time = _reaction_times [_next_index];

  return reaction_performed;
}

void ConstantRateGroup::reinitialize (double initial_time)
{
  // first compute the current rates of the reactions
  _rate_manager.update_rates();
  
  // compute all reactions timings
  // reactions times are normally given by successive exponential distributions. But equivalently,
  // number of reactions is also given by a Poisson distribution with rate (total_rate x time_step).
  // asymptotically, using the Poisson distribution is more efficient so here we go:
  int number_reactions = RandomHandler::instance().draw_poisson (_rate_manager.total_rate()*_time_step);
  // the reaction timings can now be obtained by drawing number_reactions uniform distributions
  // along the time interval:
  _final_time = initial_time + _time_step;
  _reaction_times.resize (number_reactions+1, 0);
  for (int i = 0; i < number_reactions; ++i)
    {
      _reaction_times [i] = RandomHandler::instance().draw_uniform (initial_time, _final_time);
    }
  // however we need to sort the timings:
  std::sort (_reaction_times.begin(), --_reaction_times.end());
  // we append an OVERTIME after the last reaction
  _reaction_times[number_reactions] = OVERTIME;

  // compute reaction indices
  _reaction_rate_indices = RandomHandler::instance().draw_multiple_indices (_rate_manager.rates(), number_reactions);

  // update _next_reaction_time
  _next_reaction_time = _reaction_times [0];
  _next_index = 0;
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
// ConstantRateGroup& ConstantRateGroup::operator= ( const ConstantRateGroup& other_constant_rate_group );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ConstantRateGroup::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
