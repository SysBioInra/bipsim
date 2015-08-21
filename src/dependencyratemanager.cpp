

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

// ==================
//  Project Includes
// ==================
//
#include "dependencyratemanager.h"
#include "reactant.h"

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
  clear_dependencies();
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
  compute_total_rate();
}

void DependencyRateManager::update (Reactant* reactant)
{
  // just append reactions to update in update list
  std::list <int>& reactions_to_add = _reactant_to_reactions [reactant];
  for (std::list <int>::iterator reaction_it = reactions_to_add.begin();
       reaction_it != reactions_to_add.end(); ++reaction_it)
    {
      _reactions_to_update [*reaction_it] = true;
    }
}

void DependencyRateManager::manage (const std::vector <Reaction*>& reactions)
{
  clear_dependencies();
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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool DependencyRateManager::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
void DependencyRateManager::create_dependencies (void)
{
  // loop through reactions and create dependency map
  for (int i = 0; i < reactions().size(); ++i)
    {
      const std::list <Reactant*>& reactants = reactions() [i]->reactants();
      for (std::list <Reactant*>::const_iterator reactant_it = reactants.begin();
	   reactant_it != reactants.end(); ++reactant_it)
	{
	  // add the Reactant* -> reaction index dependency
	  _reactant_to_reactions [*reactant_it].push_back (i);
	}
    }

  // subscribe to reactants
  for (std::map <Reactant*, std::list<int> >::iterator reactant_it = _reactant_to_reactions.begin();
       reactant_it != _reactant_to_reactions.end(); ++reactant_it)
    {
      (reactant_it->first)->attach (*this);
    }
}

void DependencyRateManager::clear_dependencies (void)
{
  // first unsubscribe reactant notifications
  for (std::map <Reactant*, std::list<int> >::iterator reactant_it = _reactant_to_reactions.begin();
       reactant_it != _reactant_to_reactions.end(); ++reactant_it)
    {
      (reactant_it->first)->detach (*this);
    }

  // clear map
  _reactant_to_reactions.clear();
}
