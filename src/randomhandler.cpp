

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
#include "macros.h" // REQUIRE ENSURE

// ==========================
//  Constructors/Destructors
// ==========================
//
boost::mt19937 RandomHandler::_generator;


// Not needed for this class (use of default copy constructor) !
// RandomHandler::RandomHandler (RandomHandler& other_random_handler);

RandomHandler::~RandomHandler (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
int RandomHandler::draw_index ( const std::vector<int>& weights )
{
  // we compute the total weight and strip all zero values out of the vector
  std::vector<int> cumulated_weights (weights);
  // this vector will allow us to find the orginal indices later
  std::vector<int> original_indices (weights.size(), 0);
  int real_number_items = cumulate_vector_and_strip (cumulated_weights, original_indices);
  REQUIRE( real_number_items > 0 ); /** @pre There must be positive at least one positive item. */
  REQUIRE( cumulated_weights[real_number_items-1] > 0 ); /** @pre There must be positive at least one positive item. */
  
  // we draw a number in the weight distribution
  boost::uniform_int<int> distribution ( 1, cumulated_weights[real_number_items-1] );
  int drawn_weight = distribution( RandomHandler::_generator );
  
  // we look for the corresponding index
  int result = original_indices [find_index (drawn_weight, cumulated_weights)];

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE( weights[result] > 0 );
  ENSURE( result < weights.size() );
  ENSURE( result >= 0 );
  return result;
}

int RandomHandler::draw_index ( const std::vector<double>& weights )
{
  // we compute the total weight and strip all zero values out of the vector
  std::vector<double> cumulated_weights (weights);
  // this vector will allow us to find the orginal indices later
  std::vector<int> original_indices (weights.size(), 0);
  int real_number_items = cumulate_vector_and_strip (cumulated_weights, original_indices);
  REQUIRE( real_number_items > 0 ); /** @pre There must be positive at least one positive item. */
  
  // we draw a number in the weight distribution
  boost::uniform_real<double> distribution ( 0, cumulated_weights[real_number_items-1] );
  double drawn_weight = distribution( RandomHandler::_generator );

  // we look for the corresponding index
  int result = original_indices [find_index (drawn_weight, cumulated_weights)];

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE( weights[result] > 0 );
  return result;
}

int RandomHandler::draw_uniform ( int a, int b )
{
  REQUIRE( a <= b ); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_int<int> distribution ( a, b );
  return distribution( RandomHandler::_generator );
}

double RandomHandler::draw_exponential ( double lambda )
{
  REQUIRE( lambda > 0 ); /** @pre lambda must be positive. */

  // we create the distribution and draw a number
  boost::uniform_01<double> distribution;
  return ( - log (1 - distribution (RandomHandler::_generator)) / lambda );
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

template<typename T> int RandomHandler::find_index ( T drawn_weight, const std::vector<T>& cumulated_weights )
{
  // The correct index verifies the following:
  //
  //  (1) cumulated_weights[i] >= drawn_weight
  //  (2) if i>0, cumulated_weights[i-1] < drawn_weight
  //
  // This is true because zero weights have been taken out, i.e. cumulated_weights are strictly increasing
  // and, importantly, all indices can theoretically be drawn (it would be a disaster to pick an index with
  // zero weight). If zero weights would be allowed, the above conditions would be globally fine. Consider
  // an example with zero weights, then the cumulated_weights may look like
  //
  //       index        0  1  2  3  4
  //       weights      2  1  0  0  1
  //       cumulate     2  3  3  3  4
  //
  // Suppose 3 is drawn, then we expect 1 to be the picked index. You can check that it is indeed the only
  // one that verifies the two above conditions.
  // Risks are only that a wrong index is picked for numerical reasons (typically involving double
  // comparisons but it does not even seem possible) or that the drawn_weight is exactly zero and the first
  // weight is also exactly zero, so it would be drawn, which would be unacceptable (generally unlikely, but
  // this time it definitely IS possible).

  // We use dichotomy to find the correct index
  if (cumulated_weights[0] >= drawn_weight) { return 0; }
  int min_index = 1;
  int max_index = cumulated_weights.size()-1;
  int i = (max_index+min_index)/2;
  while ( min_index != max_index )
    {
      if ( cumulated_weights[i] < drawn_weight ) // is condition (1) satisfied ?
	{
	  // if no, the index is bigger than i
	  min_index = i+1;
	  i = (max_index + min_index+1)/2;
	}
      else if ( cumulated_weights[i-1] >= drawn_weight ) // if yes, is condition (2) satisfied ?
	{
	  // if no, the index is smaller than i
	  max_index = i-1;
	  i = (max_index + min_index+1)/2;
	}
      else // if yes, we have found the right index !
	{
	  min_index = max_index = i;
	}
    }
  
  return min_index;
}

template<typename T> int RandomHandler::cumulate_vector_and_strip ( std::vector<T>& vector_to_cumulate, std::vector<int>& original_indices )
{
  int number_items = vector_to_cumulate.size();
  int real_number_items = 0;

  // we start by looking for the first positive value
  int original_index = 0;
  while ( ( vector_to_cumulate[original_index] <= 0 ) && ( original_index < number_items ) )
    { 
      original_index++;
    }

  // we check whether there were only nonpositive values
  if ( original_index == number_items ) { return 0; }

  // we set the first value
  vector_to_cumulate[real_number_items] = vector_to_cumulate[original_index];
  original_indices[real_number_items] = original_index;
  real_number_items++;
  original_index++;

  for ( ; original_index < number_items; original_index++ )
    { 
      if ( vector_to_cumulate[original_index] > 0 ) // skip all zero or negative values
	{
	  vector_to_cumulate[real_number_items] = vector_to_cumulate[real_number_items-1] + vector_to_cumulate[original_index];
	  original_indices[real_number_items] = original_index;
	  real_number_items++;
	}
    }

  // resize vectors
  vector_to_cumulate.resize (real_number_items);
  original_indices.resize (real_number_items);
  
  return real_number_items;
}
