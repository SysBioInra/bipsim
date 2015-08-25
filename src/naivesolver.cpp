

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
#include "naiveratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
NaiveSolver::NaiveSolver (double initial_time, CellState& cell_state)
  : Solver (initial_time, cell_state)
  , _rate_manager (0)
{
  _rate_manager = new NaiveRateManager (reactions());
}

// Not needed for this class (use of default copy constructor) !
// NaiveSolver::NaiveSolver ( const NaiveSolver& other_naive_solver );

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


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// NaiveSolver& NaiveSolver::operator= ( const NaiveSolver& other_naive_solver );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool NaiveSolver::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//

double NaiveSolver::compute_next_reaction (void)
{
  // update reaction rates
  _rate_manager->update_rates();
  
  if (_rate_manager->total_rate() <= 0)
    {
      std::cerr << "Warning: no reactions left to perform" << std::endl;
      // do not update time
      return time();
    }
  
  // compute next reaction to perform
  int random_index = RandomHandler::instance().draw_index (_rate_manager->rates());
  int reaction_index = random_index / 2;
  
  // perform reaction
  if ( (random_index % 2) == 0 )
    {
      // std::cout << "f" << reactions()[reaction_index] << " " << _rates[_last_reaction_index*2] << std::endl;
      reactions()[reaction_index]->perform_forward();
    }
  else
    {
      // std::cout << reactions()[reaction_index] << " " << _rates[_last_reaction_index*2+1] << std::endl;
      reactions()[reaction_index]->perform_backward();
    }
 
  // return updated time
  return time() + RandomHandler::instance().draw_exponential (_rate_manager->total_rate());
}
