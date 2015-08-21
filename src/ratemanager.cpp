

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
  , _rates (2*reactions.size(), 0)
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

  // one forwarde rate + one backward rate for each reaction
  _rates.resize (2*reactions.size(),0);

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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool RateManager::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
void RateManager::compute_all_rates (void)
{
  _total_rate = 0;
  for (int i = 0; i < _reactions.size(); ++i)
    {
      update_reaction (i);
      _total_rate += _rates [2*i];
      _total_rate += _rates [2*i+1];
    }
}

void RateManager::compute_total_rate (void)
{
  // accumulate (start, end, initial value)
  _total_rate = std::accumulate (_rates.begin(), _rates.end(), 0);
}
