

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

// ==========================
//  Constructors/Destructors
// ==========================
//
RateManager::RateManager (const std::vector <Reaction*>& reactions)
  : _reactions (reactions)
  , _rates (reactions.size(), 0)
  , _cumulated_rates (reactions.size(), 0)
  , _total_rate (0)
{
  compute_all_rates();
  cumulate_rates();
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
