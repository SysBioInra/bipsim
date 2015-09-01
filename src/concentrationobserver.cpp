

/**
 * @file concentrationobserver.cpp
 * @brief Implementation of the ConcentrationObserver class.
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
#include "concentrationobserver.h"
#include "reactant.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ConcentrationObserver::ConcentrationObserver (DependencyRateManager& parent, Reactant* reactant_to_observe, const std::list<int>& message)
  : _parent (parent)
  , _reactant (reactant_to_observe)
  , _message (message)
{
  _reactant->attach (*this);
}

// Not needed for this class (use of default copy constructor) !
// ConcentrationObserver::ConcentrationObserver ( const ConcentrationObserver& other_concentration_observer );

ConcentrationObserver::~ConcentrationObserver (void)
{
  _reactant->detach (*this);
}

// ===========================
//  Public Methods - Commands
// ===========================
//


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
// ConcentrationObserver& ConcentrationObserver::operator= ( const ConcentrationObserver& other_concentration_observer );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ConcentrationObserver::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
