

/**
 * @file naivesolver.cpp
 * @brief Implementation of the NaiveSolver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE() ENSURE()
#include "naivesolver.h"
#include "reaction.h"
#include "randomhandler.h"
#include "dependencygraph.h"
#include "cellstate.h"
#include "ratemanager.h"
#include "ratemanagerfactory.h"
#include "simulationparams.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
NaiveSolver::NaiveSolver (const SimulationParams& params, CellState& cell_state)
  : Solver (params, cell_state)
  , _rate_manager (0)
{
  _rate_manager = params.rate_manager_factory().
    create (params, cell_state.reactions(),
	    DependencyGraph (cell_state.reactions()));
}

// Forbidden
// NaiveSolver::NaiveSolver (const NaiveSolver& other_solver);
// NaiveSolver& NaiveSolver::operator= (const NaiveSolver& other_solver);

NaiveSolver::~NaiveSolver (void)
{
  delete _rate_manager;
}

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
void NaiveSolver::schedule_next_reaction (void)
{
  // update reaction rates
  _rate_manager->update_rates();
  //std::cout << time() << " " << number_reactions_performed() << "\n";
  //std::cout << (*_rate_manager) << "\n\n";
      
  if (_rate_manager->total_rate() > 0)
    {
      _next_reaction = &(_rate_manager->random_reaction());
      _next_reaction_time = time() + RandomHandler::instance().draw_exponential 
	(_rate_manager->total_rate());
    }
  else
    {
      std::cerr << "Warning: no reactions left to perform "
		<< "(total reaction rate = "
		<< _rate_manager->total_rate() << ")." << std::endl;
      _next_reaction = 0;
      _next_reaction_time = INFINITY;
    }  
}
