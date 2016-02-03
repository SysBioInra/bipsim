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
#include <sstream>
#include <string>
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "unittest.h"
#include "utility.h"

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

  void test_left_ends (const std::list <int> expected)
  {
    test (compare_lists (_strand.left_ends(), expected),
	  "Left ends incorrectly inferred.");
  }

  void test_right_ends (const std::list <int> expected)
  {
    test (compare_lists (_strand.right_ends(), expected),
	  "Right ends incorrectly inferred.");
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
    std::list <int> left_ends, right_ends;
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // build a segment from 7 to 3
    test.test_new (7, true); 
    left_ends.push_back (7); right_ends.push_back (7);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    test.test_extension (7, false); 
    test.test_new (7, false);
    test.test_extension (0, false);
    for (int i = 8; i < 10; ++i) { test.test_extension (i, true); }
    for (int i = 0; i < 3; ++i) { test.test_extension (i, true); }
    right_ends.remove (7); right_ends.push_back (2);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    test.test_completed (false);

    // build a segment from 5 to 7
    test.test_new (5, true); test.test_extension (6, true);
    left_ends.remove (7); left_ends.push_back (5);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    
    // build a segment from 3 to 5
    test.test_new (3, true); test.test_extension (4, true);
    left_ends.clear(); right_ends.clear();
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    
    test.test_completed (true);
  }

  // test with multiple segments
  {
    PartialStrandTest test (10);
    std::list <int> left_ends, right_ends;
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // build a segment from 8 to 10
    test.test_new (8, true); 
    left_ends.push_back (8); right_ends.push_back (8);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    test.test_extension (9, true); 
    right_ends.remove (8); right_ends.push_back (9);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // build segment from 2 to 5
    test.test_new (2, true); 
    test.test_extension (3, true); test.test_extension (4, true);
    left_ends.push_back (2); right_ends.push_back (4);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // build segment at 6
    test.test_new (6, true); 
    left_ends.push_back (6); right_ends.push_back (6);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
 
    // extend segment at 0
    test.test_extension (0, true); 
    right_ends.remove (9); right_ends.push_back (0);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // ligate at 1
    test.test_extension (1, true); 
    left_ends.remove (2); right_ends.remove (0);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // ligate at 5
    test.test_extension (5, true); 
    left_ends.remove (6); right_ends.remove (4);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    // ligate at 7
    test.test_extension (7, true); 
    left_ends.remove (8); right_ends.remove (6);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
  }

  return EXIT_SUCCESS;
}

