

/**
 * @file ratemanager.cpp
 * @brief Implementation of the RateManager class.
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
#include "ratemanager.h"
#include "simulationparams.h"
#include "ratecontainerfactory.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RateManager::RateManager (const SimulationParams& params,
			  const std::vector <Reaction*>& reactions)
  : _reactions (reactions)
{
  _rates = params.rate_container_factory().create (params, reactions.size());

  compute_all_rates();
  cumulate_rates();
}

// Forbidden
// RateManager::RateManager (const RateManager& other_rate_manager);
// RateManager& RateManager::operator= (const RateManager& other_rate_manager);

RateManager::~RateManager (void)
{
  delete _rates;
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
