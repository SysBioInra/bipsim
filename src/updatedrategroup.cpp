

/**
 * @file updatedrategroup.cpp
 * @brief Implementation of the UpdatedRateGroup class.
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
#include "updatedrategroup.h"
#include "reaction.h"
#include "randomhandler.h"
#include "dependencyratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
UpdatedRateGroup::UpdatedRateGroup (const SimulationParams& params,
				    const std::vector<Reaction*>& reactions,
				    double initial_time)
  : ReactionGroup (reactions)
  , _rate_manager (params, reactions)
{
  reschedule_next_reaction (initial_time);
}

// Not needed for this class (use of default copy constructor) !
// UpdatedRateGroup::UpdatedRateGroup ( const UpdatedRateGroup& other_update_rate_group );

UpdatedRateGroup::~UpdatedRateGroup (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

bool UpdatedRateGroup::perform_next_reaction (void)
{
  // perform next scheduled reaction
  perform_reaction (_rate_manager.random_index());

  // schedule next reaction
  reschedule_next_reaction (_next_reaction_time);

  return true;
}


void UpdatedRateGroup::reschedule_next_reaction (double current_time)
{
  // update rates
  _rate_manager.update_rates();

  // compute reaction time
  _next_reaction_time = current_time + RandomHandler::instance().draw_exponential (_rate_manager.total_rate());
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
// UpdatedRateGroup& UpdatedRateGroup::operator= ( const UpdatedRateGroup& other_update_rate_group );


// =================
//  Private Methods
// =================
//
