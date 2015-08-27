

/**
 * @file randomhandler.cpp
 * @brief Implementation of the RandomHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <boost/random/uniform_int.hpp> // boost::uniform_int
#include <boost/random/uniform_real.hpp> // boost::uniform_real
#include <boost/random/uniform_01.hpp> // boost::uniform_01
#include <boost/random/exponential_distribution.hpp> // boost::exponential_distribution


// ==================
//  Project Includes
// ==================
//
#include "randomhandler.h"
#include "biasedwheel.h"
#include "macros.h" // REQUIRE ENSURE

// ==========================
//  Constructors/Destructors
// ==========================
//
RandomHandler RandomHandler::_instance;

RandomHandler::RandomHandler (void)
{
  _generator.seed (0);
}


RandomHandler::~RandomHandler (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
int RandomHandler::draw_index ( const std::vector<int>& weights )
{
  REQUIRE( weights.size() > 0 ); /** @pre There must be at least one item. */

  // we create a biased wheel
  BiasedWheel<int> biased_wheel (weights);

  REQUIRE( biased_wheel.total_weight() > 0 ); /** @pre Total weight must be strictly positive. */
  
  // we draw a number in the weight distribution
  boost::uniform_int<int> distribution (1, biased_wheel.total_weight());
  int drawn_weight = distribution( RandomHandler::_generator );
  
  // we look for the corresponding index
  int result = biased_wheel.find_index (drawn_weight);

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE( weights[result] > 0 );
  ENSURE( result < weights.size() );
  ENSURE( result >= 0 );
  return result;
}

int RandomHandler::draw_index ( const std::vector<double>& weights )
{
  REQUIRE( weights.size() > 0 ); /** @pre There must be at least one item. */

  // we create a biased wheel
  BiasedWheel<double> biased_wheel (weights);

  REQUIRE( biased_wheel.total_weight() > 0 ); /** @pre Total weight must be strictly positive. */
  
  // we draw a number in the weight distribution
  boost::uniform_real<double> distribution (0, biased_wheel.total_weight());
  double drawn_weight = distribution( RandomHandler::_generator );

  // we look for the corresponding index
  int result = biased_wheel.find_index (drawn_weight);

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE( weights[result] > 0 );
  ENSURE( result < weights.size() );
  ENSURE( result >= 0 );
  return result;
}

std::vector<int> RandomHandler::draw_multiple_indices ( const std::vector<double>& weights, int number_indices )
{
  REQUIRE( weights.size() > 0 ); /** @pre There must be at least one item. */

  // we create a biased wheel
  BiasedWheel<double> biased_wheel (weights);

  REQUIRE( biased_wheel.total_weight() > 0 ); /** @pre Total weight must be strictly positive. */
  
  // we draw number_indices values in the weight distribution
  boost::uniform_real<double> distribution (0, biased_wheel.total_weight());
  std::vector<double> drawn_weights (number_indices, 0);
  for (int i = 0; i < number_indices; ++i)
    {
      drawn_weights [i] = distribution (RandomHandler::_generator);
    }

  // we look for the corresponding indices
  std::vector<int> result = biased_wheel.find_multiple_indices (drawn_weights);

  return result;
}

int RandomHandler::draw_uniform ( int a, int b )
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_int<int> distribution ( a, b );
  return distribution( RandomHandler::_generator );
}

double RandomHandler::draw_exponential ( double lambda )
{
  REQUIRE (lambda > 0); /** @pre lambda must be positive. */

  // we create the distribution and draw a number
  // boost implementation
  // TODO update boost libraries
  boost::uniform_01<double> distribution;
  return ( - log (1 - distribution (RandomHandler::_generator)) / lambda );
}


int RandomHandler::draw_poisson ( double lambda )
{
  REQUIRE( lambda > 0 ); /** @pre lambda must be positive. */

  // we create the distribution and draw a number
  // boost implementation
  // TODO update boost libraries
  boost::uniform_01<double> distribution;
  double exp_mean = exp (-lambda);
  double product = 1;
  for(int m = 0; ; ++m)
    {
      product *= distribution (RandomHandler::_generator);
      if (product <= exp_mean) { return m; }
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
// RandomHandler& RandomHandler::operator= (RandomHandler& other_random_handler);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool RandomHandler::check_invariant ( void ) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
