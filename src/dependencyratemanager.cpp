

/**
 * @file dependencyratemanager.cpp
 * @brief Implementation of the DependencyRateManager class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <algorithm> // std::fill
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "dependencyratemanager.h"
#include "reactant.h"
#include "reaction.h"
#include "rateinvalidator.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DependencyRateManager::DependencyRateManager (const SimulationParams& params,
					      const std::vector <Reaction*>& reactions)
  : RateManager (params, reactions)
  , _rate_validity (reactions.size())
{
  const std::vector <Reaction*>& _reactions = this->reactions();

  // loop through reactions and create observers
  for (int i = 0; i < _reactions.size(); ++i)
    {
      const std::vector <Reactant*>& reactants = _reactions [i]->reactants();
      for (std::vector <Reactant*>::const_iterator reactant_it = reactants.begin();
	   reactant_it != reactants.end(); ++reactant_it)
	{ (*reactant_it)->attach (_rate_validity.invalidator (i)); }
    }
}

// DependencyRateManager::DependencyRateManager (const DependencyRateManager& other);
// DependencyRateManager& DependencyRateManager::operator= (const DependencyRateManager& other);
// DependencyRateManager::~DependencyRateManager (void);

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
