

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
#include <map> // std::map
#include <list> // std::list

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
DependencyGraph::DependencyGraph (const std::vector< Reaction* > reactions)
  : _dependencies (reactions.size())
{
  // we build the dependencies in three steps

  // STEP 1: the reaction vector provides us with a link from reactions to 
  // components (reaction->components). However, we are also interested in
  // the reverse component->reactions link. We start with building a map that
  // yields this relationship.

  // Map that stores the list of reactions in which a chemical is involved
  std::map< const Chemical*, std::list<int> > chemical_to_reactions;

  // loop through reaction
  int number_reactions = reactions.size();
  for (int i = 0; i < number_reactions; ++i)
    {
      // gather components
      const std::list<Chemical*>& components = reactions[i]->components();

      // loop through components
      for (std::list<Chemical*>::const_iterator component = components.begin();
	   component != components.end(); component++)
	{
	  // store component to reaction relationship
	  chemical_to_reactions [*component].push_back (i);
	}
    }

  // STEP 2: now we have the two following relationships
  // - reaction[_index] -> list<component>
  // - component -> list<reaction_index>
  // so we can easily define our dependency vector giving
  //   reaction_index -> list<reaction_index>

  // loop through reactions again
  for (int i = 0; i < number_reactions; ++i)
    {
      // gather components of reaction i
      const std::list<Chemical*>& components = reactions[i]->components();

      // loop through components
      for (std::list<Chemical*>::const_iterator component = components.begin();
	   component != components.end(); component++)
	{
	  // gather reaction indices the component is involved in
	  const std::list<int>& reaction_indices = chemical_to_reactions [*component];

	  // loop through reaction indices and store them in the dependency vector
	  for (std::list<int>::const_iterator reaction = reaction_indices.begin();
	       reaction != reaction_indices.end(); ++reaction)
	    {
	      _dependencies[i].insert (*reaction);
	    }
	}
    }

  // NOTE that _dependencies uses sets to store indices so we need not worry about
  // duplicates...
}

// Not needed for this class (use of default copy constructor) !
// DependencyGraph::DependencyGraph ( const DependencyGraph& other_dependency_graph );

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
// Not needed for this class (use of default overloading) !
// DependencyGraph& DependencyGraph::operator= ( const DependencyGraph& other_dependency_graph );

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
