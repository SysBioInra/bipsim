

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
#include <numeric> // std::accumulate

// ==================
//  Project Includes
// ==================
//
#include "ratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RateManager::RateManager (const std::vector <Reaction*>& reactions)
  : _reactions (reactions)
  , _rates (reactions.size(), 0)
  , _total_rate (0)
{
  compute_all_rates();
}

// Not needed for this class (use of default copy constructor) !
// RateManager::RateManager ( const RateManager& other_rate_manager );

RateManager::~RateManager (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void RateManager::manage (const std::vector <Reaction*>& reactions)
{
  _reactions = reactions;
  _rates.resize (reactions.size(),0);

  compute_all_rates();
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
// RateManager& RateManager::operator= ( const RateManager& other_rate_manager );


// =================
//  Private Methods
// =================
//
void RateManager::compute_all_rates (void)
{
  for (int i = 0; i < _reactions.size(); ++i) { update_reaction (i); }
}

void RateManager::compute_total_rate (void)
{
  // accumulate (start, end, initial value)
  _total_rate = std::accumulate (_rates.begin(), _rates.end(), 0);
}
