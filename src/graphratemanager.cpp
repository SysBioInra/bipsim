

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
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "graphratemanager.h"
#include "reaction.h"
#include "dependencygraph.h"
#include "rateinvalidator.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
GraphRateManager::GraphRateManager (const SimulationParams& params,
				    const std::vector <Reaction*>& reactions,
				    const DependencyGraph& dependency_graph)
  : RateManager (params, reactions)
  , _rate_validity (reactions.size())
{
  // create index map that yield Reaction -> index
  const std::vector <Reaction*>& _reactions = this->reactions();
  std::map <Reaction*, int> reaction_to_index;
  for (int i = 0; i < _reactions.size(); ++i) 
    { reaction_to_index [_reactions [i]] = i; }

  // loop through all reactions (not only those managed by the rate manager)
  const std::vector<Reaction*>& all_reactions = dependency_graph.reactions();
  std::map <Reaction*, int>::const_iterator search_result;
  for (std::vector <Reaction*>::const_iterator reaction_it 
	 = all_reactions.begin();
       reaction_it != all_reactions.end(); ++reaction_it)
    {
      // add invalidators for reaction rates impacted by current reaction 
      // under focus
      const std::set <Reaction*>& impacted 
	= dependency_graph.reactions_to_update (*reaction_it);
      for (std::set <Reaction*>::const_iterator impacted_it = impacted.begin();
	   impacted_it != impacted.end(); ++impacted_it)
	{
	  // look if impacted reaction is handled by manager
	  search_result = reaction_to_index.find (*impacted_it);
	  // if yes, add invalidator on original reaction
	  if (search_result != reaction_to_index.end())
	    {
	      (*reaction_it)->attach 
		(_rate_validity.invalidator (search_result->second));
	    }
	}
    }
}

// Forbidden
// GraphRateManager::GraphRateManager (const GraphRateManager& other_manager);
// GraphRateManager& GraphRateManager::operator= (const GraphRateManager& other_graph_rate_manager);

// GraphRateManager::~GraphRateManager (void)

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
