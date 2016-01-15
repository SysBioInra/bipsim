

/**
 * @file dependencyratemanager.cpp
 * @brief Implementation of the DependencyRateManager class.
 * 
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
#include "concentrationobserver.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DependencyRateManager::DependencyRateManager (const SimulationParams& params,
					      const std::vector <Reaction*>& reactions)
  : RateManager (params, reactions)
  , _rate_validity (reactions.size())
{
  create_dependencies();
}

// Not needed for this class (use of compiler-generated versions)
// DependencyRateManager::DependencyRateManager (const DependencyRateManager& other_manager);
// DependencyRateManager& DependencyRateManager::operator= (const DependencyRateManager& other_manager);
// DependencyRateManager::~DependencyRateManager (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void DependencyRateManager::update_rates (void)
{
  while (!_rate_validity.empty())
    {
      update_reaction (_rate_validity.front());
      _rate_validity.pop();
    }
  cumulate_rates();
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
void DependencyRateManager::create_dependencies (void)
{
  // loop through reactions and create observers
  for (int i = 0; i < reactions().size(); ++i)
    {
      const std::vector <Reactant*>& reactants = reactions() [i]->reactants();
      for (std::vector <Reactant*>::const_iterator reactant_it = reactants.begin();
	   reactant_it != reactants.end(); ++reactant_it)
	{
	  _rate_validity.add_observer (**reactant_it, i);
	}
    }
}
