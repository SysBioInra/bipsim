/**
 * @file randomhandler_test.cpp
 * @brief Unit testing for RandomHandler class.
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

#include "../src/randomhandler.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

int main (int argc, char *argv[])
{ 
  int vector_size = 10;
  std::vector<int> int_zeros (vector_size, 0);
  std::vector<int> int_test;
  std::vector<double> double_zeros (vector_size, 0);
  std::vector<double> double_test;

  // draw_index should abort when total weight <= 0
  // RandomHandler::instance().draw_index (std::vector<int>(3,0));

  // draw_index should return the only non-zero weighted index
  for (int i = 0; i < vector_size; ++i)
    {
      int_test = int_zeros; int_test[i] = i + 1;
      if (RandomHandler::instance().draw_index (int_test) != i)
	FAILURE ("draw_index did not return only non-zero weighted index");
    }

  // draw_index should return the only non-zero weighted index
  for (int i = 0; i < vector_size; ++i)
    {
      double_test = double_zeros; double_test[i] = i + 1.1;
      if (RandomHandler::instance().draw_index (double_test) != i)
	FAILURE ("draw_index did not return only non-zero weighted index");
    }

  // draw_multiple_indices should return n times the only non-zero weighted index
  int n = 10;
  for (int i = 0; i < vector_size; ++i)
    {
      double_test = double_zeros; double_test[i] = i+1.1;
      if (RandomHandler::instance().draw_multiple_indices (double_test,n) != std::vector<int> (n,i))
	{
	  FAILURE ("draw_multiple_indices did not return only non-zero weighted index");
	}
    }
  
  // draw_uniform with min = max = a should return a
  int a = 7;
  if (RandomHandler::instance().draw_uniform (a,a) != a)
    FAILURE ("draw_uniform (a,a) did not return a");

  // draw_uniform should abort if a 
  // RandomHandler::instance().draw_uniform (3,1);

  // should abort
  // RandomHandler::draw_exponential (0);

      

  return EXIT_SUCCESS;
}

