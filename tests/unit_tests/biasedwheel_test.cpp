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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BiasedWheel
#include <boost/test/unit_test.hpp>

#include <vector> // std::vector
#include <iostream> // std::cerr
#include <cmath> // fabs
#include <numeric> // std::partial_sum


// ==================
//  Project Includes
// ==================
//
#include "biasedwheel.h"

template <typename T>
std::ostream& operator<< (std::ostream& output, const std::vector<T>& v)
{
  output << "[ ";
  for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    { output << *it << " "; }
  output << "]";
  return output;
}

class VectorsSize10
{
public:
  VectorsSize10 (void) 
    : vector_size (10)
    , int_zeros (vector_size, 0)
    , int_cumulated (vector_size)
  {}

  void cumulate_int_zeros (void)
  { 
    std::partial_sum (int_zeros.begin(), int_zeros.end(), 
		      int_cumulated.begin());
  }

  int vector_size;
  std::vector<int> int_zeros;
  std::vector<int> int_cumulated;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, VectorsSize10)
 
BOOST_AUTO_TEST_CASE (find_index_oneNonZeroIntegerValue_returnsIndexOfNonZeroValue)
{
  int_zeros [0] = 10; cumulate_int_zeros(); BiasedWheel<int> bw (int_cumulated);
  BOOST_CHECK_EQUAL (bw.find_index (1), 0);
  BOOST_CHECK_EQUAL (bw.find_index (10), 0);
}

BOOST_AUTO_TEST_CASE (find_index_oneNonZeroIntegerValue_returnsIndexOfNonZeroValue2)
{
  int_zeros [5] = 10; cumulate_int_zeros(); BiasedWheel<int> bw (int_cumulated);
  BOOST_CHECK_EQUAL (bw.find_index (1), 5);
  BOOST_CHECK_EQUAL (bw.find_index (10), 5);
}

BOOST_AUTO_TEST_CASE (find_index_oneNonZeroIntegerValue_returnsIndexOfNonZeroValue3)
{
  int_zeros [9] = 10; cumulate_int_zeros(); BiasedWheel<int> bw (int_cumulated);
  BOOST_CHECK_EQUAL (bw.find_index (1), 9);
  BOOST_CHECK_EQUAL (bw.find_index (10), 9);
}

BOOST_AUTO_TEST_CASE (find_multiple_indices_oneNonZeroIntegerValueFindTenIndices_returnsIndexOfNonZeroValueTenTimes)
{
  int n = 10;
  int_zeros [0] = 10; cumulate_int_zeros(); BiasedWheel<int> bw (int_cumulated);
  BOOST_CHECK (bw.find_multiple_indices (std::vector <int> (n, 1)) 
	       == std::vector <int> (n, 0));
  BOOST_CHECK (bw.find_multiple_indices (std::vector <int> (n, 10))
	       == std::vector <int> (n, 0));
}

BOOST_AUTO_TEST_CASE (find_multiple_indices_oneNonZeroIntegerValueFindTenIndices_returnsIndexOfNonZeroValueTenTimes2)
{
  int n = 10;
  int_zeros [5] = 10; cumulate_int_zeros(); BiasedWheel<int> bw (int_cumulated);
  BOOST_CHECK (bw.find_multiple_indices (std::vector <int> (n, 1))
	       == std::vector <int> (n, 5));
  BOOST_CHECK (bw.find_multiple_indices (std::vector <int> (n, 10))
	       == std::vector <int> (n, 5));
}
 
BOOST_AUTO_TEST_SUITE_END()
