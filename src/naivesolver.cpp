

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

// ==========================
//  Constructors/Destructors
// ==========================
//
NaiveSolver::NaiveSolver (double initial_time, CellState& cell_state)
  : Solver (initial_time, cell_state)
    // there are 2 rates per reaction (forward and backward)
  , _rates (2*cell_state.reaction_list().size(),0)
  , _total_rate (0)
  , _last_reaction_index (-1)
{
}

// Not needed for this class (use of default copy constructor) !
// NaiveSolver::NaiveSolver ( const NaiveSolver& other_naive_solver );

NaiveSolver::~NaiveSolver (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void NaiveSolver::update_all_rates (void)
{
  // fist update system state
  _cell_state.update_all_binding_rate_contributions();

  // then update rates
  _total_rate = 0;
  int number_reactions = reactions().size();
  for ( int i = 0; i < number_reactions; i++ )
    {
      // update rates
      reactions() [i]->update_rates();

      // store the rates
      _rates[2*i] = reactions()[i]->forward_rate();
      _rates[2*i+1] = reactions()[i]->backward_rate();
      _total_rate += _rates[2*i] + _rates[2*i+1];
    }
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
void NaiveSolver::update_rates (void)
{
  if (_last_reaction_index == -1) // no reaction has been performed yet
    {
      update_all_rates();
      return;
    }
  
  // fist update system state if necessary
  const ChemicalSequence* modified_sequence = reactions()[_last_reaction_index]->last_chemical_sequence_involved();
  if (modified_sequence != 0)
    {
      _cell_state.update_binding_rate_contributions (*modified_sequence);
    }

  // then update rates
  // get reaction indices to update from dependency graph
  const std::set<int>& reactions_to_update =
    dependency_graph().reactions_to_update (_last_reaction_index);

  // update rates according to dependencies
  int reaction_index;
  for (std::set<int>::const_iterator reaction = reactions_to_update.begin();
       reaction != reactions_to_update.end(); reaction++)
    {
      reaction_index = *reaction;
      
      // update rates
      reactions() [reaction_index]->update_rates();

      // store the rates
      _rates[2*reaction_index] = reactions()[reaction_index]->forward_rate();
      _rates[2*reaction_index+1] = reactions()[reaction_index]->backward_rate();
    }

  // compute total rate
  _total_rate = 0;
  int number_rates = _rates.size();
  for (int i = 0; i < number_rates; i++) { _total_rate += _rates[i]; }
}


double NaiveSolver::compute_next_reaction (void)
{
  // update reaction rates
  update_rates();
  
  if (_total_rate <= 0)
    {
      std::cerr << "Warning: no reactions left to perform" << std::endl;
      // do not update time
      return time();
    }
  
  // compute next reaction to perform
  int random_index = _random_handler.draw_index (_rates);
  _last_reaction_index = random_index / 2;
  
  // perform reaction
  if ( (random_index % 2) == 0 )
    {
      // std::cout << "f" << reactions()[_last_reaction_index] << " " << _rates[_last_reaction_index*2] << std::endl;
      reactions()[_last_reaction_index]->perform_forward();
    }
  else
    {
      // std::cout << reactions()[_last_reaction_index] << " " << _rates[_last_reaction_index*2+1] << std::endl;
      reactions()[_last_reaction_index]->perform_backward();
    }
 
  // return updated time
  return time() + _random_handler.draw_exponential (_total_rate);
}
