/**
 * @file biasedwheel_test.cpp
 * @brief Unit testing for BiasedWheel class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <cmath> // fabs
#include <numeric> // std::partial_sum

#include "../src/biasedwheel.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

template <typename T>
std::ostream& operator<< (std::ostream& output, const std::vector<T>& v)
{
  output << "[ ";
  for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
      output << *it << " ";
    }
  output << "]";
  return output;
}

int main (int argc, char *argv[])
{ 
  int vector_size = 10;
  std::vector<int> int_zeros (vector_size, 0);
  std::vector<int> int_test;
  std::vector <int> int_cumulated;
  std::vector<double> double_zeros (vector_size, 0);
  std::vector<double> double_test;
  std::vector <double> double_cumulated;

  // test vector [ 1 0 .... 0 1 ]
  // expected result [ 1 1 ... 1 2 ]
  {
    int_test = int_zeros; int_test[0] = 1; int_test[vector_size-1] = 1;
    std::vector<int> expected_result (int_test.size(),1);
    expected_result[int_test.size()-1] = 2;
    int_cumulated.resize (int_test.size());
    std::partial_sum (int_test.begin(), int_test.end(), int_cumulated.begin());
    BiasedWheel<int> bw (int_cumulated);
    if (bw.cumulated_weights() != expected_result)
      FAILURE ("Accumulation of the weight vector incorrect.");
  }

  // constructor should accumulate values of the vector
  // test vector [ 1 1 1 1 1 1 1 1 1 1 ]
  // expected result = [ 1 2 3 4 5 6 7 8 9 10 ]
  {
    std::vector<int> test (10, 1); 
    std::vector<int> expected_result (10); 
    for (int i = 0; i < expected_result.size(); ++i) expected_result[i] = i+1;
    int_cumulated.resize (test.size());
    std::partial_sum (test.begin(), test.end(), int_cumulated.begin());
    BiasedWheel<int> bw (int_cumulated);
    if (bw.cumulated_weights() != expected_result)
      FAILURE ("Accumulation of the weight vector incorrect.");    
    if (bw.total_weight() != 10)
      FAILURE ("Accumulation of the weight vector incorrect.");    
  }

  // constructor should accumulate values of the vector
  // test vector [ 0 1 0 1 0 1 0 1 0 1 ]
  // expected result [ 0 1 1 2 2 3 3 4 4 5 ]
  {
    std::vector<int> test (10, 0);
    std::vector<int> expected_result (10, 0);
    for (int i = 0; i < 5; ++i)
      {
	test [2*i+1] = 1;
	expected_result[2*i] = i;
	expected_result[2*i+1] = i+1;
      }
    int_cumulated.resize (test.size());
    std::partial_sum (test.begin(), test.end(), int_cumulated.begin());
    BiasedWheel<int> bw (int_cumulated);
    if (bw.cumulated_weights() != expected_result)
      FAILURE ("Accumulation+stripping of the weight vector incorrect.");    
    if (bw.total_weight() != 5)
      FAILURE ("Accumulation of the weight vector incorrect.");    
  }

  // test vector [ 0 1 0 1 0 1 0 1 0 1 ]
  // expected result [ 0 1 1 2 2 3 3 4 4 5 ]
  {
    std::vector<double> test (10, 0);
    std::vector<double> expected_result (10, 0);
    for (int i = 0; i < 5; ++i)
      {
	test [2*i+1] = 1;
	expected_result[2*i] = i;
	expected_result[2*i+1] = i+1;
      }
    double_cumulated.resize (test.size());
    std::partial_sum (test.begin(), test.end(), double_cumulated.begin());
    BiasedWheel<double> bw (double_cumulated);
    if (fabs (bw.total_weight() - 5.0) > 1e-15 )
      FAILURE ("Accumulation of the weight vector incorrect.");    
  }

  // find index should be able to find the only positive weight index as long as weight parameter is valid
  // test vectors [ 0 ... i+1 ... 0 ] with i+1 at position i
  // expected result i
  for (int i = 0; i < vector_size; ++i)
    {
      int_test = int_zeros; int_test [i] = i+1;
      int_cumulated.resize (int_test.size());
      std::partial_sum (int_test.begin(), int_test.end(), int_cumulated.begin());
      BiasedWheel<int> bw (int_cumulated);
      if (bw.find_index (1) != i)
	  FAILURE ("Finding index of only positive weight failed.");
      if (bw.find_index (i+1) != i)
	  FAILURE ("Finding index of only positive weight failed.");
    }
  
  // find indices should be able to find the only positive weight index as long as weight parameter is valid
  // test vectors [ 0 ... i+1 ... 0 ] with i+1 at position i
  // expected result [ i i ... i ] of length n
  for (int i = 0; i < vector_size; ++i)
    {
      int n = 10;
      int_test = int_zeros; int_test [i] = i+1;
      int_cumulated.resize (int_test.size());
      std::partial_sum (int_test.begin(), int_test.end(), int_cumulated.begin());
      BiasedWheel<int> bw (int_cumulated);
      if (bw.find_multiple_indices (std::vector <int> (n,1)) != std::vector <int> (n,i))
	  FAILURE ("Finding index of only positive weight multiple times failed.");
      if (bw.find_multiple_indices (std::vector <int> (n,i+1)) != std::vector <int> (n,i))
	  FAILURE ("Finding index of only positive weight multiple times failed.");
    }
  

  return EXIT_SUCCESS;
}

