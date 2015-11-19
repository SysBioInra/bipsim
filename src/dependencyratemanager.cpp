

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
DependencyRateManager::DependencyRateManager (const std::vector <Reaction*>& reactions)
  : RateManager (reactions)
  , _reactions_to_update (reactions.size(), false)
{
  create_dependencies();
}

// Not needed for this class (use of default copy constructor) !
// DependencyRateManager::DependencyRateManager ( const DependencyRateManager& other_dependency_rate_manager );

DependencyRateManager::~DependencyRateManager (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void DependencyRateManager::update_rates (void)
{
  int reaction_index = 0;
  for (std::vector <bool>::iterator reaction_it = _reactions_to_update.begin();
       reaction_it != _reactions_to_update.end();
       ++reaction_index, ++reaction_it)
    {
      if (*reaction_it == true)
  	{ 
  	  update_reaction (reaction_index);
  	  *reaction_it = false;
  	}
    }
  cumulate_rates();
}


void DependencyRateManager::manage (const std::vector <Reaction*>& reactions)
{
  clear_observers();
  // parent class handles both _reactions and _rates updates
  RateManager::manage (reactions);
  create_dependencies();

  std::fill (_reactions_to_update.begin(), _reactions_to_update.end(), false);
  _reactions_to_update.resize (reactions.size(), false);
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
// DependencyRateManager& DependencyRateManager::operator= ( const DependencyRateManager& other_dependency_rate_manager );


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
	  add_observer (**reactant_it, i);
	}
    }
}
