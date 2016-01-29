/**
 * @file partialstrand_test.cpp
 * @brief Unit testing for PartialStrand class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "unittest.h"

// ==================
//  Project Includes
// ==================
//
#include "../src/partialstrand.h"


class PartialStrandTest : public UnitTest
{
public:
  PartialStrandTest (int length) : _length (length), _strand (length) {}

  void test_new (int position, bool expected)
  {
    test (_strand.start_new_segment (position) == expected,
	  "Segment extension does not work properly.");
  }

  void test_extension (int position, bool expected)
  {
    test (_strand.extend_segment (position) == expected,
	  "Segment extension does not work properly.");
  }

  void test_completed (bool expected)
  {
    test (_strand.completed() == expected,
	  "Strand uncompleted when it should be (or conversely).");
  }
  
private:
  PartialStrand _strand;
  int _length;
};

int main (int argc, char *argv[])
{ 
  // all extensions should fail.
  {
    PartialStrandTest test (10);
    test.test_completed (false);
    for (int i = 0; i < 10; ++i) { test.test_extension (i, false); }
    test.test_completed (false);
  }

  // all news should work
  {
    PartialStrandTest test (10);
    test.test_completed (false);
    for (int i = 0; i < 10; ++i) { test.test_new (i, true); }
    test.test_completed (true);
  }

  {
    PartialStrandTest test (10);
    // build a segment from 7 to 3
    test.test_new (7, true);
    test.test_extension (7, false);
    test.test_new (7, false);
    test.test_extension (0, false);
    for (int i = 8; i < 10; ++i) { test.test_extension (i, true); }
    for (int i = 0; i < 3; ++i) { test.test_extension (i, true); }
    test.test_completed (false);

    // build a segment from 5 to 7
    test.test_new (5, true); test.test_extension (6, true);
    
    // build a segment from 3 to 5
    test.test_new (3, true); test.test_extension (4, true);
    
    test.test_completed (true);
  }

  return EXIT_SUCCESS;
}

