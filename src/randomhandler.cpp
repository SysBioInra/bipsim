

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
#include <numeric> //std::partial_sum


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

int RandomHandler::draw_index (const std::vector<int>& weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (weights.size() > 0); 
  std::vector<double> cumulated_weights (weights.size());
  std::partial_sum (weights.begin(), weights.end(), cumulated_weights.begin());
  /** @pre Total weight must be strictly positive. */
  REQUIRE (cumulated_weights.back() > 0);

  int result = draw_index_cumulated (cumulated_weights);

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (weights[result] > 0);
  ENSURE ((result < weights.size()) && (result >= 0));
  return result;
}

int RandomHandler::draw_index (const std::vector<double>& weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (weights.size() > 0); 
  std::vector<double> cumulated_weights (weights.size());
  std::partial_sum (weights.begin(), weights.end(), cumulated_weights.begin());
  /** @pre Total weight must be strictly positive. */
  REQUIRE (cumulated_weights.back() > 0);

  int result = draw_index_cumulated (cumulated_weights);

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (weights[result] > 0);
  ENSURE ((result < weights.size()) && (result >= 0));
  return result;
}

int RandomHandler::draw_index_cumulated (const std::vector<int>& cumulated_weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (cumulated_weights.size() > 0); 

  // we draw a number in the weight distribution
  BiasedWheel<int> biased_wheel (cumulated_weights);
  boost::uniform_int<int> distribution (1, biased_wheel.total_weight());
  int result = biased_wheel.find_index 
    (distribution (RandomHandler::_generator));

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (cumulated_weights[result] > 0);
  ENSURE ((result < cumulated_weights.size()) && (result >= 0));
  return result;
}

int RandomHandler::draw_index_cumulated (const std::vector<double>& cumulated_weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (cumulated_weights.size() > 0); 

  // we draw a number in the weight distribution
  BiasedWheel<double> biased_wheel (cumulated_weights);  
  boost::uniform_real<double> distribution (biased_wheel.total_weight()*1e-16, biased_wheel.total_weight());
  int result = biased_wheel.find_index
    (distribution (RandomHandler::_generator));

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (cumulated_weights[result] > 0);
  ENSURE ((result < cumulated_weights.size()) && (result >= 0));
  return result;
}

std::vector<int> RandomHandler::draw_multiple_indices_cumulated (const std::vector<double>& cumulated_weights, int number_indices)
{
  /** @pre There must be at least one item. */
  REQUIRE (cumulated_weights.size() > 0);

  // we draw number_indices values in the weight distribution
  BiasedWheel<double> biased_wheel (cumulated_weights);
  boost::uniform_real<double> distribution (biased_wheel.total_weight()*1e-16, biased_wheel.total_weight());
  std::vector<double> drawn_weights (number_indices, 0);
  for (int i = 0; i < number_indices; ++i)
    {
      drawn_weights [i] = distribution (RandomHandler::_generator);
    }

  // we look for the corresponding indices
  std::vector<int> result = biased_wheel.find_multiple_indices (drawn_weights);
  return result;
}

int RandomHandler::draw_uniform (int a, int b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_int<int> distribution ( a, b );
  return distribution( RandomHandler::_generator );
}

double RandomHandler::draw_uniform (double a, double b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_real<double> distribution ( a, b );
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
  double m_mean = -lambda;
  double log_product = 0;
  for(int m = 0; ; ++m)
    {
      log_product += log (distribution (RandomHandler::_generator));
      if (log_product <= m_mean) { return m; }
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


// =================
//  Private Methods
// =================
//
