

/**
 * @file naiveratemanager.cpp
 * @brief Implementation of the NaiveRateManager class.
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
#include "naiveratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
NaiveRateManager::NaiveRateManager (const std::vector <Reaction*>& reactions)
  : RateManager (reactions)
{
}

// Not needed for this class (use of default copy constructor) !
// NaiveRateManager::NaiveRateManager ( const NaiveRateManager& other_naive_rate_manager );

NaiveRateManager::~NaiveRateManager (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void NaiveRateManager::update_rates (void)
{
  compute_all_rates();
  cumulate_rates();
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
// NaiveRateManager& NaiveRateManager::operator= ( const NaiveRateManager& other_naive_rate_manager );


// =================
//  Private Methods
// =================
//
