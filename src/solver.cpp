

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
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE() ENSURE()
#include "solver.h"
#include "reaction.h"
#include "dependencygraph.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Solver::Solver (const std::list<Reaction*>& reactions)
  : _reactions (reactions.begin(), reactions.end())
  , _dependency_graph (_reactions)
  , _t (0)
  , _number_reactions_performed (0)
{
}

// Not needed for this class (use of default copy constructor) !
// Solver::Solver ( const Solver& other_solver );

Solver::~Solver (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

void Solver::solve (double time_step)
{  
  double final_time = _t + time_step;

  while ( _t < final_time )
    {
      // compute next reaction
      go_to_next_reaction();
    }
  _t = final_time;
}

void Solver::go_to_next_reaction (void)
{
  // perform next reaction
  _t = compute_next_reaction();

  // increase number of reactions performed
  ++_number_reactions_performed;
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
// Solver& Solver::operator= ( const Solver& other_solver );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Solver::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
