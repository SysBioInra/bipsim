

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
#include <numeric> // std::partial_sum
#include <cmath> // log

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

// Not needed for this class (use of compiler-generated versions)
// RandomHandler::RandomHandler (const RandomHandler& other_handler);
// RandomHandler& RandomHandler::operator= (RandomHandler& other_handler);
// RandomHandler::~RandomHandler (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

int RandomHandler::draw_index (const std::vector<int>& weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (weights.size() > 0); 
  std::vector<int> cumulated_weights (weights.size());
  std::partial_sum (weights.begin(), weights.end(), cumulated_weights.begin());
  /** @pre Total weight must be strictly positive. */
  REQUIRE (cumulated_weights.back() > 0);

  BiasedWheel<int> biased_wheel (cumulated_weights);
  boost::uniform_int<int> distribution (1, cumulated_weights.back());
  int result = biased_wheel.find_index 
    (distribution (RandomHandler::_generator));

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

  // we draw a number in the weight distribution
  BiasedWheel<double> biased_wheel (cumulated_weights);  
  boost::uniform_real<double> distribution (cumulated_weights.back()*1e-16, 
					    cumulated_weights.back());
  int result = biased_wheel.find_index
    (distribution (RandomHandler::_generator));
  
  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (weights[result] > 0);
  ENSURE ((result < weights.size()) && (result >= 0));
  return result;
}

int RandomHandler::draw_uniform (int a, int b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_int<int> distribution (a, b);
  return distribution (RandomHandler::_generator);
}

double RandomHandler::draw_uniform (double a, double b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_real<double> distribution (a, b);
  return distribution (RandomHandler::_generator);
}

double RandomHandler::draw_exponential ( double lambda )
{
  REQUIRE (lambda > 0); /** @pre lambda must be positive. */

  // we create the distribution and draw a number
  // boost implementation
  // TODO update boost libraries
  boost::uniform_01<double> distribution;
  return (-log (1 - distribution (RandomHandler::_generator)) / lambda);
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


// =================
//  Private Methods
// =================
//
