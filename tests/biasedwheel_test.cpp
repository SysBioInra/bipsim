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

