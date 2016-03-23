
/**
 * @file ratevalidity.cpp
 * @brief Implementation of the RateValidity class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "ratevalidity.h"
#include "rateinvalidator.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RateValidity::RateValidity (int number_rates)
  : _invalidated (number_rates, false)
  , _update_stack (number_rates)
  , _invalidators (number_rates)
{
  for (int i = 0; i < _invalidators.size(); ++i)
    { _invalidators[i] = new RateInvalidator (*this, i); }
}

// RateValidity::RateValidity (const RateValidity& other);
// RateValidity& RateValidity::operator= (const RateValidity& other);

RateValidity::~RateValidity (void)
{
  for (std::vector <RateInvalidator*>::iterator it = _invalidators.begin();
       it != _invalidators.end(); ++it)
    { delete *it; }
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
