

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

// ==========================
//  Constructors/Destructors
// ==========================
//
Solver::Solver (void)
  : _number_reactions (0)
  , _total_rate (0)
  , _t (0)
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
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Solver::add_reaction (Reaction& reaction)
{
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
  _reactions[_number_reactions-1] = &reaction;
}

void Solver::add_reaction_list (std::list<Reaction*>& reactions_to_add)
{
  int next_reaction_index = _number_reactions;

  // check whether vector size is exceeded
  _number_reactions += reactions_to_add.size();
  while ( _number_reactions > _reactions.size() )
    {
      // if yes resize vector size twofold
      _reactions.resize (_reactions.size()*2, 0);
      // same goes for rate vector
      _rates.resize (_rates.size()*2, 0);      
    }

  // add reactions to vector
  for ( std::list<Reaction*>::iterator next_reaction = reactions_to_add.begin();
	next_reaction != reactions_to_add.end();
	next_reaction++, next_reaction_index++ )
    {
      _reactions[next_reaction_index] = *next_reaction;
    }
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
void Solver::update_rates()
{
  _total_rate = 0;
  for ( int i = 0; i < _number_reactions; i++ )
    {
      _rates[2*i] = _reactions[i]->forward_rate();
      _rates[2*i+1] = _reactions[i]->backward_rate();
      _total_rate += _rates[2*i] + _rates[2*i+1];
    }
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
  int reaction_index = random_index / 2;
  
  // perform reaction
  if ( (random_index % 2) == 0 )
    {
      // std::cout << "f" << *_reactions[reaction_index] << " " << _rates[reaction_index*2] << std::endl;
      _reactions[reaction_index]->perform_forward();
    }
  else
    {
      // std::cout << *_reactions[reaction_index] << " " << _rates[reaction_index*2+1] << std::endl;
      _reactions[reaction_index]->perform_backward();
    }
}
