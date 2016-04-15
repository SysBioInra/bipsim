

/**
 * @file solver.cpp
 * @brief Implementation of the Solver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE() ENSURE()
#include "solver.h"
#include "reaction.h"
#include "dependencygraph.h"
#include "cellstate.h"
#include "simulationparams.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double Solver::INFINITY = std::numeric_limits<double>::infinity();

Solver::Solver (const SimulationParams& params, CellState& cell_state)
  : _reactions (cell_state.reactions())
  , _t (params.initial_time())
  , _number_reactions_performed (0)
{
}

// Not needed for this class (use of compiler generated versions)
// Solver::Solver (const Solver& other_solver);
// Solver& Solver::operator= (const Solver& other_solver);
// Solver::~Solver (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void Solver::solve (double time_step)
{  
  double final_time = _t + time_step;
  while (next_reaction_time() < final_time) { perform_next_reaction(); }
  _t = final_time;
}

void Solver::perform_next_reaction (void)
{
  REQUIRE (next_reaction_time() != INFINITY);
  // perform next reaction
  _t = next_reaction_time(); 
  next_reaction().perform();
  ++_number_reactions_performed;
  schedule_next_reaction();
}

void Solver::reschedule (double time)
{
  _t = time;
  reinitialize();
}

void Solver::set_volume (double time, double volume)
{
  _t = time;
  for (std::vector <Reaction*>::iterator it = _reactions.begin();
       it != _reactions.end(); ++it)
    { (*it)->handle_volume_change (volume); }
  reinitialize();
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
