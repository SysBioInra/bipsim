

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
#include "randomhandler.h"
#include "dependencygraph.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Solver::Solver (void)
  : _number_reactions (0)
  , _total_rate (0)
  , _t (0)
  , _number_reactions_performed (0)
  , _dependency_graph (0)
  , _last_reaction_index (-1)
{
  // we arbitrarily define the reaction vector to be of size 10
  // it will be resized when reactions are added
  _reactions.resize (10, 0);

  // there are 2 rates per reaction (forward and backward)
  _rates.resize (20, 0);
}

// Not needed for this class (use of default copy constructor) !
// Solver::Solver ( const Solver& other_solver );

Solver::~Solver (void)
{
  delete _dependency_graph;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Solver::add_reaction (Reaction& reaction)
{
  int reaction_index = _number_reactions;
  _number_reactions++;

  // check whether vector size is exceeded
  if ( _number_reactions > _reactions.size() )
    {
      // if yes resize vector size twofold
      _reactions.resize (_reactions.size()*2, 0);
      // same goes for rate vector
      _rates.resize (_rates.size()*2, 0);      
    }

  // add the reaction to the vector
  _reactions [reaction_index] = &reaction;

  // dependency graph is outdated: delete it
  delete _dependency_graph;
  _dependency_graph = 0;
}

void Solver::add_reaction_list (const std::list<Reaction*>& reactions_to_add)
{
  // add reactions to vector
  for (std::list<Reaction*>::const_iterator next_reaction = reactions_to_add.begin();
       next_reaction != reactions_to_add.end();
       next_reaction++)
    {
      add_reaction (**next_reaction);
    }
}

void Solver::update_all_rates (void)
{
  _total_rate = 0;
  for ( int i = 0; i < _number_reactions; i++ )
    {
      // update rates
      _reactions [i]->update_rates();

      // store the rates
      _rates[2*i] = _reactions[i]->forward_rate();
      _rates[2*i+1] = _reactions[i]->backward_rate();
      _total_rate += _rates[2*i] + _rates[2*i+1];
    }
}

void Solver::compute_dependencies (void)
{
  // check that dependencies have not been computed
  if (_dependency_graph == 0)
    {
      // if not, create a dependency graph
      _reactions.resize (_number_reactions);
      _dependency_graph = new DependencyGraph (_reactions);
    }
  // else simply ignore the request
}

void Solver::solve (double time_step)
{  
  /**
   * Solving is done in a stochastic way based on exponential distributions.
   * Basically, every reaction has some associated rate r_i (actually 2, forward
   * and backward). The next reaction timing can be seen as T_i -> Exp(r_i). Now, by
   * the properties of exponential distributions, if we suppose that reactions are
   * independent, we have min(T_i) -> Exp(sum(r_i)) and the probability that the next
   * reaction is reaction i is given by r_i / sum(r_i).
   */
  REQUIRE( _dependency_graph != 0 ); /** @pre Dependencies must be to date. */

  double final_time = _t + time_step;

  // update reaction rates and set next reaction time
  update_rates();
  set_next_reaction_time();
  
  while ( _t < final_time )
    {
      // compute next reaction
      compute_next_reaction();

      // update reaction rates and set next reaction time
      update_rates();
      if (_total_rate > 0)
	{
	  set_next_reaction_time();
	}
      else
	{
	  _t = final_time;
	}
    }
  _t = final_time;
}

void Solver::go_to_next_reaction (void)
{
  REQUIRE( _dependency_graph != 0 ); /** @pre Dependencies must be to date. */

  // update reaction rates
  update_rates();

 if (_total_rate > 0)
    {
      set_next_reaction_time();
      compute_next_reaction();
    }
  else
    {
      std::cerr << "Warning: no reactions left to perform" << std::endl;
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
void Solver::update_rates (void)
{
  REQUIRE( _dependency_graph != 0 ); /** @pre Dependencies must be to date. */

  if (_last_reaction_index == -1) // no reaction has been performed yet
    {
      update_all_rates();
      return;
    }

  // get reaction indices to update
  const std::set<int>& reactions_to_update =
    _dependency_graph->reactions_to_update (_last_reaction_index);

  // update rates according to dependencies
  int reaction_index;
  for (std::set<int>::const_iterator reaction = reactions_to_update.begin();
       reaction != reactions_to_update.end(); reaction++)
    {
      reaction_index = *reaction;
      
      // update rates
      _reactions [reaction_index]->update_rates();

      // store the rates
      _rates[2*reaction_index] = _reactions[reaction_index]->forward_rate();
      _rates[2*reaction_index+1] = _reactions[reaction_index]->backward_rate();
    }

  // compute total rate
  _total_rate = 0;
  for (int i = 0; i < 2*_number_reactions; i++) { _total_rate += _rates[i]; }
}

void Solver::set_next_reaction_time (void)
{
  REQUIRE( _total_rate > 0 );
  
  _t += _random_handler.draw_exponential (_total_rate);
}

void Solver::compute_next_reaction (void)
{
  // compute next reaction to perform
  int random_index = _random_handler.draw_index (_rates);
  _last_reaction_index = random_index / 2;
  
  // perform reaction
  if ( (random_index % 2) == 0 )
    {
      // std::cout << "f" << *_reactions[reaction_index] << " " << _rates[reaction_index*2] << std::endl;
      _reactions[_last_reaction_index]->perform_forward();
    }
  else
    {
      // std::cout << *_reactions[reaction_index] << " " << _rates[reaction_index*2+1] << std::endl;
      _reactions[_last_reaction_index]->perform_backward();
    }

  ++_number_reactions_performed;
}
