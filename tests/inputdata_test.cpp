/**
 * @file inputdata_test.cpp
 * @brief Unit testing for InputData class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/inputdata.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}


int main (int argc, char *argv[])
{ 
  std::list <std::string> files_to_read;
  files_to_read.push_back ("input/input1.in");
  files_to_read.push_back ("input/input3.in");
  files_to_read.push_back ("input/input2.in");

  InputData test (files_to_read);
  int i = 0;
  while (!test.is_eof())
    {
      std::cout << test.line() << "\n";
      if (i%2) test.mark_line_as_treated();
      test.go_next();
      ++i;
    }

  test.write_warnings (std::cout);

  test.rewind();
  while (!test.is_eof())
    {
      std::cout << test.line() << "\n";
      test.go_next();
    }

  return EXIT_SUCCESS;
}

