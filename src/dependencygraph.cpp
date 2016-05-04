

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
  // reactants and products  (reaction->reactants+products). 
  // However, we need the reverse reactant->reactions link. 
  // We start with building a map that yields this relationship.
  std::map <const Reactant*, std::list<Reaction*> > reactant_to_reactions;
  for (std::vector <Reaction*>::iterator reaction_it = _reactions.begin();
       reaction_it != _reactions.end(); ++reaction_it)
    {
      // Note that products do not change reaction rate
      const std::vector<Reactant*>& reactants = (*reaction_it)->reactants();
      for (std::vector<Reactant*>::const_iterator it = reactants.begin();
	   it != reactants.end(); ++it)
	{ reactant_to_reactions [*it].push_back (*reaction_it); }
    }

  // STEP 2: now we have the two following relationships
  // - reaction -> vector <reactants> and vector <products>
  // - chemical -> list <reactions>
  // so we can easily define our dependency vector giving
  //   reaction -> vector <reactions>
  for (std::vector <Reaction*>::iterator reaction_it = _reactions.begin();
       reaction_it != _reactions.end(); ++reaction_it)
    {
      for (int i = 0; i < 2; ++i)
	{
	  // gather reactants and products of reaction
	  const std::vector<Reactant*>& components = 
	    (i==0)? (*reaction_it)->reactants(): (*reaction_it)->products();
	  
	  // insert reactions the component is involved in in dependency map
	  for (std::vector<Reactant*>::const_iterator it = components.begin();
	       it != components.end(); ++it)
	    {
	      const std::list <Reaction*>& 
		impacted_reactions = reactant_to_reactions [*it];
	      _dependencies [*reaction_it].insert 
		(impacted_reactions.begin(), impacted_reactions.end());
	    }
	}
    }

  // NOTE that _dependencies uses sets to store reactions so we need not 
  // worry about duplicates...
}


// Not needed for this class (use of compiler-generated versions)
// DependencyGraph::DependencyGraph (const DependencyGraph& other_graph);
// DependencyGraph& DependencyGraph::operator= (const DependencyGraph& other_graph);
// DependencyGraph::~DependencyGraph (void) {}

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
