

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
  : _rate_manager (params, reactions)
  , _next_reaction_time (initial_time)
  , _next_reaction (0)
  , _final_time (initial_time + time_step)
  , _time_step (time_step)
{
  reinitialize (initial_time);
}

// Not needed for this class (use of compiler-generated versions)
// ConstantRateGroup::ConstantRateGroup (const ConstantRateGroup& other_group);
// ConstantRateGroup& ConstantRateGroup::operator= (const ConstantRateGroup& other_group);
// ConstantRateGroup::~ConstantRateGroup (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void ConstantRateGroup::schedule_next_reaction (double current_time)
{
  if (current_time >= _final_time) { reinitialize (current_time); }

  _next_reaction_time = current_time + 
    RandomHandler::instance().draw_exponential (_rate_manager.total_rate());
  if (_next_reaction_time < _final_time) 
    { 
      _next_reaction = &(_rate_manager.random_reaction()); 
    }
  else
    { 
      _next_reaction_time = _final_time; _next_reaction = 0;
    }
}

void ConstantRateGroup::reinitialize (double initial_time)
{
  _final_time = initial_time + _time_step;
  _rate_manager.update_rates();  
  schedule_next_reaction (initial_time);
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
