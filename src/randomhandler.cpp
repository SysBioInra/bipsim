

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

#include <algorithm> // std::sort

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

RandomHandler::RandomHandler (void)
{
  _generator.seed (0);
}


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

std::vector<int> RandomHandler::draw_multiple_indices ( const std::vector<double>& weights, int number_indices )
{
  // we compute the total weight and strip all zero values out of the vector
  std::vector<double> cumulated_weights (weights);
  // this vector will allow us to find the orginal indices later
  std::vector<int> original_indices (weights.size(), 0);
  int real_number_items = cumulate_vector_and_strip (cumulated_weights, original_indices);
  REQUIRE( real_number_items > 0 ); /** @pre There must be positive at least one positive item. */
  
  // we draw number_indices values in the weight distribution
  boost::uniform_real<double> distribution ( 0, cumulated_weights[real_number_items-1] );
  std::vector<double> drawn_weights (number_indices, 0);
  for (int i = 0; i < number_indices; ++i)
    {
      drawn_weights [i] = distribution (RandomHandler::_generator);
    }

  // we look for the corresponding indices
  std::vector<int> drawn_indices = find_multiple_indices (drawn_weights, cumulated_weights);

  // we convert it back to original indices
  std::vector<int> result (number_indices, 0);
  for (int i = 0; i < number_indices; ++i)
    {
      result [i] = original_indices [drawn_indices [i]];
      /** @post The weights associated to the drawn indices must be positive. */
      ENSURE (weights [result [i]] > 0);
    }

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

template<typename T> std::vector<int> RandomHandler::find_multiple_indices (const std::vector<T>& drawn_weights, const std::vector<T>& cumulated_weights)
{
  // the principle is the same as find_index, see above
  int number_indices = drawn_weights.size();
  std::vector<int> result (number_indices);

  // first we need to know the order of the weights that have been drawn
  std::vector<int> weight_order = sorted_indices (drawn_weights);
  // we get a vector pointing to progressively bigger weights:
  //  weight_order[0] gives the index of the smallest element
  //  weight_order[1] gives the index of the second smallest element etc.
  // in shord weight_order associates an order to the index of the element like this
  //   weight_order [i] -> index of ith element

  // there is a chance that the smallest element is pretty small, so we do not use
  // dichotomy here, we simply go from the start and look for the index of the smallest element
  int current_order = 0;
  double current_weight_to_find = drawn_weights [weight_order [current_order]];
  int current_cumulated_weight_index = 0;

  while (current_order < number_indices) // while there is a weight whose index is not found
    {
      // the right index is found if
      //  (1) cumulated_weights[i] >= drawn_weight
      //  (2) if i>0, cumulated_weights[i-1] < drawn_weight
      while (cumulated_weights [current_cumulated_weight_index] < current_weight_to_find)
	{
	  // condition (1) not met: go to the next cumulated weight index
	  ++current_cumulated_weight_index;
	}
      // right index has been found (condition (2) is automatically met): store result
      // we need to be careful to store it at the right place: current_order does not
      // yield the INDEX of the element being compared, but its ORDER. 
      result [weight_order [current_order]] = current_cumulated_weight_index;

      // go to next element in increasing order
      ++current_order;
      current_weight_to_find = drawn_weights [weight_order [current_order]];
    }

  return result;
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


template<typename T> std::vector<int> RandomHandler::sorted_indices (const std::vector<T>& vector_to_sort)
{
  CompareValues<T> compare_vector_to_sort_values (vector_to_sort);

  // generate an unsorted index vector
  int number_values = vector_to_sort.size();
  std::vector<int> result (number_values);
  for (int i = 0; i < number_values; ++i) { result [i] = i; }

  // sort indices based on associated values
  std::sort (result.begin(), result.end(), compare_vector_to_sort_values);

  return result;
}
