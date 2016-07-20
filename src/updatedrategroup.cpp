
/**
 * @file updatedrategroup.cpp
 * @brief Implementation of the UpdatedRateGroup class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "updatedrategroup.h"
#include "reaction.h"
#include "randomhandler.h"
#include "dependencyratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double UpdatedRateGroup::INF = 
    std::numeric_limits<double>::infinity();

UpdatedRateGroup::UpdatedRateGroup (const SimulationParams& params,
				    const std::vector<Reaction*>& reactions,
				    double initial_time)
  : _rate_manager (params, reactions)
  , _next_reaction (0)
  , _next_reaction_time (initial_time)
{
  schedule_next_reaction (initial_time);
}

// Not needed for this class (use of default copy constructor) !
// UpdatedRateGroup::UpdatedRateGroup (const UpdatedRateGroup& other_update_rate_group);
// UpdatedRateGroup& UpdatedRateGroup::operator= (const UpdatedRateGroup& other_update_rate_group);
// UpdatedRateGroup::~UpdatedRateGroup (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void UpdatedRateGroup::schedule_next_reaction (double current_time)
{
  _rate_manager.update_rates();
  
  if (_rate_manager.total_rate() > 0)
    {
      _next_reaction = &(_rate_manager.random_reaction());
      _next_reaction_time = current_time + 
	RandomHandler::instance().draw_exponential (_rate_manager.total_rate());
    }
  else
    {
      _next_reaction = 0;
      _next_reaction_time = INF;
    }
}

void UpdatedRateGroup::reinitialize (double time)
{
  _rate_manager.compute_all_rates();
  schedule_next_reaction (time);
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
