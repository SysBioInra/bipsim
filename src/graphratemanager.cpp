

/**
 * @file graphratemanager.cpp
 * @brief Implementation of the GraphRateManager class.
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
#include "graphratemanager.h"
#include "reaction.h"
#include "reactionobserver.h"
#include "dependencygraph.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
GraphRateManager::GraphRateManager (const std::vector <Reaction*>& reactions, const DependencyGraph& dependency_graph)
  : RateManager (reactions)
  , _reactions_to_update (reactions.size(), false)
{
  create_observers (dependency_graph);
}

// Not needed for this class (use of default copy constructor) !
// GraphRateManager::GraphRateManager ( const GraphRateManager& other_graph_rate_manager );

GraphRateManager::~GraphRateManager (void)
{
  clear_observers();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void GraphRateManager::update_rates (void)
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

void GraphRateManager::update (const std::list<int>& reactions_to_update)
{
  // just mark reactions to update in update table
  for (std::list <int>::const_iterator reaction_it = reactions_to_update.begin();
       reaction_it != reactions_to_update.end(); ++reaction_it)
    {
      _reactions_to_update [*reaction_it] = true;
    }
}

void GraphRateManager::manage (const std::vector <Reaction*>& reactions, const DependencyGraph& dependency_graph)
{
  clear_observers();
  // parent class handles both _reactions and _rates updates
  RateManager::manage (reactions);
  create_observers (dependency_graph);

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
// GraphRateManager& GraphRateManager::operator= ( const GraphRateManager& other_graph_rate_manager );


// =================
//  Private Methods
// =================
//
void GraphRateManager::create_observers (const DependencyGraph& dependency_graph)
{
  // create index map that yield Reaction -> index
  std::map <Reaction*, int> reaction_to_index;
  for (int i = 0; i < reactions().size(); ++i) { reaction_to_index [reactions() [i]] = i; }

  // loop through all reactions (not only those managed by the rate manager)
  const std::vector<Reaction*>& all_reactions = dependency_graph.reactions();
  for (std::vector <Reaction*>::const_iterator reaction_it = all_reactions.begin();
       reaction_it != all_reactions.end(); ++reaction_it)
    {
      // see what reactions are impacted by current reaction under focus
      std::list<int> dependencies =
	convert_to_indices (reaction_to_index, dependency_graph.reactions_to_update (*reaction_it));

      // if one of the impacted reactions is handled by the rate manager,
      // the original reaction needs to be observed
      if (!dependencies.empty())
	{
	  _reaction_observers.push_back (new ReactionObserver (*this, *reaction_it, dependencies));
	}
    }
}

void GraphRateManager::clear_observers (void)
{
  // unsubscribe reactant notifications by deleting observers
  for (std::list<ReactionObserver*>::iterator obs_it = _reaction_observers.begin();
       obs_it != _reaction_observers.end(); ++obs_it)
    {
      delete *obs_it;
    }
  _reaction_observers.clear();
}

std::list<int> GraphRateManager::convert_to_indices (const std::map <Reaction*, int>& reaction_to_index, const std::set<Reaction*>& reactions)
{
  std::list <int> result;
  std::map <Reaction*, int>::const_iterator search_result;
  for (std::set<Reaction*>::const_iterator reaction_it = reactions.begin();
       reaction_it != reactions.end(); ++reaction_it)
    {
      search_result = reaction_to_index.find (*reaction_it);
      if (search_result != reaction_to_index.end())
	{
	  result.push_back (search_result->second);
	}
    }

  return result;
}
