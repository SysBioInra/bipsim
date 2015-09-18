

/**
 * @file biasedwheel.cpp
 * @brief Implementation of the BiasedWheel class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <algorithm> // std::sort

// ==================
//  Project Includes
// ==================
//
#include "biasedwheel.h"
#include "macros.h" // REQUIRE

// ==========================
//  Constructors/Destructors
// ==========================
//
template <typename T>
BiasedWheel<T>::BiasedWheel (const std::vector<T>& weights)
{
  /** @pre All weights must be greater or equal to 0. */
  REQUIRE (check_weight_positivity (weights));

  /**
   * Input vector is not stored as such, instead all zero values are stripped
   * and a cumulative form is stored.
   */
  // for optimization purposes, we reserve enough memory for our vectors
  _cumulated_weights.reserve (weights.size());
  _original_indices.reserve (weights.size());
  cumulate_vector_and_strip (weights);
}

// Not needed for this class (use of default copy constructor) !
// BiasedWheel::BiasedWheel ( const BiasedWheel& other_biased_wheel );

template <typename T>
BiasedWheel<T>::~BiasedWheel (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
template <typename T>
int BiasedWheel<T>::find_index (T drawn_weight)
{
  /** 
   * @pre drawn_weight must be strictly (!) positive. Positive is not enough: if the weights are integer,
   *  the wheel would be strongly biased.
   */
  REQUIRE (drawn_weight > 0);
  /** 
   * @pre drawn_weight must be smaller or equal to total weight.
   */
  REQUIRE (drawn_weight <= _total_weight);

  // The correct index verifies the following:
  //
  //  (1) _cumulated_weights[i] >= drawn_weight
  //  (2) if i>0, _cumulated_weights[i-1] < drawn_weight
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

  /** We use dichotomy to find the correct index */
  if (_cumulated_weights[0] >= drawn_weight) { return _original_indices[0]; }
  int min_index = 1;
  int max_index = _cumulated_weights.size()-1;
  int i = (max_index+min_index)/2;
  while ( min_index != max_index )
    {
      if ( _cumulated_weights[i] < drawn_weight ) // is condition (1) satisfied ?
	{
	  // if no, the index is bigger than i
	  min_index = i+1;
	  i = (max_index + min_index+1)/2;
	}
      else if ( _cumulated_weights[i-1] >= drawn_weight ) // if yes, is condition (2) satisfied ?
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

  return _original_indices [min_index];
}
  
template <typename T>
std::vector<int> BiasedWheel<T>::find_multiple_indices (const std::vector<T>& drawn_weights)
{
  /** @pre All drawn_weights must be in the (0, _total_weight] range. */
  REQUIRE (check_drawn_weight_validity (drawn_weights));

  // the rules are the same as find_index, see above
  /** 
   * Here we do not use dichotomy for each drawn weight as it would be inefficient. Instead,
   * we simply go through the wheel and look how many drawn weights fall on the index we are
   * currently on. For this to be efficient, we begin by sorting all the drawn values.
   */
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
  double current_weight_to_find = 0;
  int current_cumulated_weight_index = 0;

  while (current_order < number_indices) // while there is a weight whose index is not found
    {
      current_weight_to_find = drawn_weights [weight_order [current_order]];

      // the right index is found if
      //  (1) cumulated_weights[i] >= drawn_weight
      //  (2) if i>0, cumulated_weights[i-1] < drawn_weight
      while (_cumulated_weights [current_cumulated_weight_index] < current_weight_to_find)
	{
	  // condition (1) not met: go to the next cumulated weight index
	  ++current_cumulated_weight_index;
	}
      // right index has been found (condition (2) is automatically met): store result
      // we need to be careful to store it at the right place: current_order does not
      // yield the INDEX of the element being compared, but its ORDER. 
      result [weight_order [current_order]] = _original_indices [current_cumulated_weight_index];

      // go to next element in increasing order
      ++current_order;
    }

  return result;
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
// BiasedWheel& BiasedWheel::operator= ( const BiasedWheel& other_biased_wheel );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
template <typename T>
bool BiasedWheel<T>::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//

// THIS VERSION MIGHT BE BETTER IF WE DO NOT HAVE TO ALLOCATE MEMORY SYSTEMATICALLY
// (E.G. IF A BIASED WHEEL IS REUSED FOR A DIFFERENT VECTOR)
// template<typename T>
// void BiasedWheel<T>::cumulate_vector_and_strip (const std::vector<T>& vector_to_cumulate)
// {
//   int number_items = vector_to_cumulate.size();
//   int real_number_items = 0;

//   // we start by looking for the first positive value
//   int original_index = 0;
//   while ( ( vector_to_cumulate[original_index] <= 0 ) && ( original_index < number_items ) )
//     { 
//       original_index++;
//     }

//   // we set the first value
//   _cumulated_weights [real_number_items] = vector_to_cumulate [original_index];
//   _original_indices [real_number_items] = original_index;
//   real_number_items++;
//   original_index++;

//   for ( ; original_index < number_items; original_index++ )
//     { 
//       if ( vector_to_cumulate[original_index] > 0 ) // skip all zero or negative values
// 	{
// 	  _cumulated_weights [real_number_items] = _cumulated_weights [real_number_items-1] + vector_to_cumulate [original_index];
// 	  _original_indices [real_number_items] = original_index;
// 	  real_number_items++;
// 	}
//     }

//   // resize vectors
//   _cumulated_weights.resize (real_number_items);
//   _original_indices.resize (real_number_items);
  
//   _total_weight = _cumulated_weights.back();
// }

template<typename T>
void BiasedWheel<T>::cumulate_vector_and_strip (const std::vector<T>& vector_to_cumulate)
{
  int number_items = vector_to_cumulate.size();

  // we start by looking for the first positive value
  int original_index = 0;
  while ( ( vector_to_cumulate[original_index] <= 0 ) && ( original_index < number_items ) )
    { 
      original_index++;
    }

  // we set the first value
  _cumulated_weights.push_back (vector_to_cumulate[original_index]);
  _original_indices.push_back (original_index);
  original_index++;

  for ( ; original_index < number_items; original_index++ )
    { 
      if ( vector_to_cumulate[original_index] > 0 ) // skip all zero or negative values
	{
	  _cumulated_weights.push_back (_cumulated_weights.back() + vector_to_cumulate[original_index]);
	  _original_indices.push_back (original_index);
	}
    }

  _total_weight = _cumulated_weights.back();
}


template<typename T>
std::vector<int> BiasedWheel<T>::sorted_indices (const std::vector<T>& vector_to_sort)
{
  CompareValues compare_vector_to_sort_values (vector_to_sort);

  // generate an unsorted index vector
  int number_values = vector_to_sort.size();
  std::vector<int> result (number_values);
  for (int i = 0; i < number_values; ++i) { result [i] = i; }

  // sort indices based on associated values
  std::sort (result.begin(), result.end(), compare_vector_to_sort_values);

  return result;
}

template <typename T>
bool BiasedWheel<T>::check_weight_positivity (const std::vector<T>& v)
{
  for (typename std::vector<T>::const_iterator weight_it = v.begin();
       weight_it != v.end(); ++weight_it)
    {
      if (*weight_it < 0) return false;
    }
  return true;
}

template <typename T>
bool BiasedWheel<T>::check_drawn_weight_validity (const std::vector<T>& v)
{
  for (typename std::vector<T>::const_iterator weight_it = v.begin();
       weight_it != v.end(); ++weight_it)
    {
      if ((*weight_it <= 0) || (*weight_it > _total_weight))
	{
	  std::cerr << "drawn weight = " << *weight_it << ", total weight = " << _total_weight << std::endl;
	  return false;
	}
    }
  return true;
}

// useful declarations
/**
 * @brief BiasedWheel where weights are integers.
 */
template class BiasedWheel<int>;
/**
 * @brief BiasedWheel where weights are real values.
 */
template class BiasedWheel<double>;
