

/**
 * @file dependencygraph.cpp
 * @brief Implementation of the DependencyGraph class.
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
#include "dependencygraph.h"
#include "reaction.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DependencyGraph::DependencyGraph (const std::vector <Reaction*>& reactions)
  : _reactions (reactions)
{
  // we build the dependencies in two steps

  // STEP 1: the reaction vector provides us with a link from reactions to 
  // reactants (reaction->reactants). However, we are also interested in
  // the reverse component->reactions link. We start with building a map that
  // yields this relationship.

  // Map that stores the vector of reactions in which a reactant is involved
  std::map< const Reactant*, std::list<Reaction*> > reactant_to_reactions;

  // loop through reaction
  for (std::vector <Reaction*>::iterator reaction_it = _reactions.begin();
       reaction_it != _reactions.end(); ++reaction_it)
    {
      // gather reactants
      const std::list<Reactant*>& reactants = (*reaction_it)->reactants();

      // loop through reactants
      for (std::list<Reactant*>::const_iterator component = reactants.begin();
	   component != reactants.end(); component++)
	{
	  // store component to reaction relationship
	  reactant_to_reactions [*component].push_back (*reaction_it);
	}
    }

  // STEP 2: now we have the two following relationships
  // - reaction -> list<component>
  // - component -> list<reactions>
  // so we can easily define our dependency vector giving
  //   reaction -> list<reactions>

  // loop through reactions again
  for (std::vector <Reaction*>::iterator reaction_it = _reactions.begin();
       reaction_it != _reactions.end(); ++reaction_it)
    {
      // gather reactants of reaction
      const std::list<Reactant*>& reactants = (*reaction_it)->reactants();

      // loop through reactants
      for (std::list<Reactant*>::const_iterator component = reactants.begin();
	   component != reactants.end(); component++)
	{
	  // gather reactions the component is involved in
	  const std::list <Reaction*>& impacted_reactions = reactant_to_reactions [*component];

	  // insert them in dependencies map
	  _dependencies [*reaction_it].insert (impacted_reactions.begin(), impacted_reactions.end());
	}
    }

  // NOTE that _dependencies uses sets to store reactions so we need not worry about
  // duplicates...
}


DependencyGraph::~DependencyGraph (void)
{
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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool DependencyGraph::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
