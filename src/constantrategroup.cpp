

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
#include <limits> // std::numerical_limits<double>::infinity()
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
ConstantRateGroup::ConstantRateGroup (const SimulationParams& params,
				      const std::vector<Reaction*>& reactions,
				      double initial_time, double time_step)
  : ReactionGroup (reactions)
  , _reaction_times (MAX_NUMBER_REACTIONS)
  , _next_index (0)
  , _final_time (initial_time + time_step)
  , _time_step (time_step)
  , _rate_manager (params, reactions)
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
  /** @pre _next_reaction_time must not have reached OVERTIME (extending beyond
   *   valid time step). */
  REQUIRE (_next_reaction_time != OVERTIME);

  // perform next scheduled reaction if possible
  bool reaction_performed = false;
  int next_reaction_index = _reaction_indices [_next_index];

  if (is_reaction_possible (next_reaction_index))
    {
      perform_reaction (next_reaction_index);
      reaction_performed = true;
    }

  // schedule next reaction
  go_to_next_reaction();

  return reaction_performed;
}

void ConstantRateGroup::reinitialize (double initial_time)
{
  _final_time = initial_time + _time_step;

  // first compute the current rates of the reactions
  _rate_manager.update_rates();  

  // compute next reaction timings
  schedule_next_reactions (initial_time);
  _next_reaction_time = next_reaction_time();
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


// =================
//  Private Methods
// =================
//
void ConstantRateGroup::schedule_next_reactions (double current_time)
{
  /** @pre current_time must be smaller than _final_time. */
  REQUIRE (current_time < _final_time); 
  double total_rate = _rate_manager.total_rate();
   
  int number_reactions = 0;
  double relative_reaction_time = 
    RandomHandler::instance().draw_exponential (total_rate);

  while ((current_time + relative_reaction_time < _final_time)
	 && (number_reactions < MAX_NUMBER_REACTIONS))
    {
      _reaction_times [number_reactions] = 
	current_time + relative_reaction_time;
      ++number_reactions;
      relative_reaction_time += 
	RandomHandler::instance().draw_exponential (total_rate);
    }

  // we append an OVERTIME after the last reaction if final_time was reached
  if ((number_reactions < MAX_NUMBER_REACTIONS) 
      && (current_time + relative_reaction_time >= _final_time))
    {
      _reaction_times [number_reactions] = OVERTIME;
    }

  // std::cout << "scheduled " << number_reactions << " reactions at time "
  // << current_time << "(rate is " << total_rate << ")" << std::endl;

  // compute reaction indices
  _reaction_indices.clear();
  for (int i = 0; i < number_reactions; ++i)
    {
      _reaction_indices.push_back (_rate_manager.random_index());
    }

  _next_index = 0;
}

