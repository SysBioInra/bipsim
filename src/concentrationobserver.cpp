

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
ConcentrationObserver::ConcentrationObserver (RateValidity& parent,
					      Reactant* reactant,
					      int identifier)
  : _parent (parent)
  , _reactant (reactant)
  , _message (identifier)
{
  _reactant->attach (*this);
}

// Not needed for this class (use of default copy constructor) !
// ConcentrationObserver::ConcentrationObserver ( const ConcentrationObserver& other_concentration_observer );

ConcentrationObserver::~ConcentrationObserver (void)
{
  if (_reactant != 0) _reactant->detach (*this);
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



// =================
//  Private Methods
// =================
//
