

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
const double Solver::NO_REACTIONS_LEFT = 
  std::numeric_limits<double>::infinity();

Solver::Solver (const SimulationParams& params, CellState& cell_state)
  : _reactions (cell_state.reactions())
  , _cell_state (cell_state)
  , _dependency_graph (cell_state.reactions())
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
  bool reactions_left = true;
  while (reactions_left && (_t < final_time))
    {
      // compute next reaction
      reactions_left = go_to_next_reaction();
    }
  _t = final_time;
}

bool Solver::go_to_next_reaction (void)
{
  // perform next reaction
  double t = compute_next_reaction();
  if (t == NO_REACTIONS_LEFT) { return false; }
  _t = t;
  ++_number_reactions_performed;
  return true;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
