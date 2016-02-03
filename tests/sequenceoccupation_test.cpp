/**
 * @file sequenceoccupation_test.cpp
 * @brief Unit testing for SequenceOccupation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "unittest.h"
#include "utility.h"

// ==================
//  Project Includes
// ==================
//
#include "../src/sequenceoccupation.h"
#include "../src/boundchemical.h"

class SequenceOccupationTest : public UnitTest
{
public:
  SequenceOccupationTest (int length) : _so (length, 0) {}

  void test_site_availability (int first, int last, int expected) 
  {
    test (_so.number_available_sites (first, last) == expected,
	  "Number of available sites is incorrect.");
  }

  void test_left_ends (const std::list <int>& expected)
  {
    test (compare_lists (_so.left_ends(), expected),
	  "Left ends incorrectly inferred.");
  }

  void test_right_ends (const std::list <int>& expected)
  {
    test (compare_lists (_so.right_ends(), expected),
	  "Right ends incorrectly inferred.");
  }

  SequenceOccupation& operator() (void) { return _so; }

private:
  SequenceOccupation _so;
};

int main (int argc, char *argv[])
{ 
  // test on whole sequences
  {
    SequenceOccupationTest test (100);
    test.test_site_availability (0, 99, 0);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 0); }
    test().add_sequence (3);
    test.test_site_availability (0, 99, 3);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 3); }
    test().remove_sequence (2);
    test.test_site_availability (0, 99, 1);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 1); }
    test().add_sequence (1);
    test().remove_sequence (2);
    test.test_site_availability (0, 99, 0);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 0); }
  }

  // test on whole sequences + bound elements
  {
    SequenceOccupationTest test (100);
    BoundChemical dummy;
    test().add_sequence (1);
    test().add_element (dummy, 50, 53);
    test.test_site_availability (0, 99, 0);
    for (int i = 0; i < 50; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 50; i < 54; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 54; i < 100; ++ i) { test.test_site_availability (i, i, 1); }
    test.test_site_availability (50, 53, 0);
    test.test_site_availability (0, 50, 0);
    test.test_site_availability (53, 99, 0);

    test().add_element (dummy, 52, 63);
    test.test_site_availability (52, 63, 0);
    for (int i = 0; i < 50; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 50; i < 64; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 64; i < 100; ++ i) { test.test_site_availability (i, i, 1); }

    test().add_sequence (1);
    test.test_site_availability (50, 53, 0);
    test.test_site_availability (52, 63, 0);
    for (int i = 0; i < 50; ++ i) { test.test_site_availability (i, i, 2); }
    for (int i = 50; i < 52; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 52; i < 54; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 54; i < 64; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 64; i < 100; ++ i) { test.test_site_availability (i, i, 2); }

    test().remove_element (dummy, 50, 53);
    test.test_site_availability (50, 53, 1);
    test.test_site_availability (52, 63, 1);
    for (int i = 0; i < 52; ++ i) { test.test_site_availability (i, i, 2); }
    for (int i = 52; i < 64; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 64; i < 100; ++ i) { test.test_site_availability (i, i, 2); }

    test().remove_element (dummy, 52, 63);
    test.test_site_availability (50, 53, 2);
    test.test_site_availability (52, 63, 2);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 2); }
  }

  // test with segments
  {
    SequenceOccupationTest test (100);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().add_sequence (1);
    for (int i = 0; i < 100; ++ i) { test.test_site_availability (i, i, 1); }

    test().start_new_segment (15);
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 1); }
    test.test_site_availability (15, 15, 2);
    for (int i = 16; i < 100; ++ i) { test.test_site_availability (i, i, 1); }

    test().start_new_segment (15);
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 1); }
    test.test_site_availability (15, 15, 3);
    for (int i = 16; i < 100; ++ i) { test.test_site_availability (i, i, 1); }

    for (int i = 16; i < 35; ++ i) { test().extend_segment (i); }
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 1); }
    test.test_site_availability (15, 15, 3);
    for (int i = 16; i < 35; ++ i) { test.test_site_availability (i, i, 2); }  
    for (int i = 35; i < 100; ++ i) { test.test_site_availability (i, i, 1); }  

    test().start_new_segment (85);
    for (int i = 86; i < 100; ++ i) { test().extend_segment (i); }
    for (int i = 0; i < 10; ++ i) { test().extend_segment (i); }
    for (int i = 0; i < 10; ++ i) { test.test_site_availability (i, i, 2); }
    for (int i = 10; i < 15; ++ i) { test.test_site_availability (i, i, 1); }
    test.test_site_availability (15, 15, 3);
    for (int i = 16; i < 35; ++ i) { test.test_site_availability (i, i, 2); }  
    for (int i = 35; i < 85; ++ i) { test.test_site_availability (i, i, 1); }  
    for (int i = 85; i < 100; ++ i) { test.test_site_availability (i, i, 2); }  

    for (int i = 10; i < 15; ++ i) { test().extend_segment (i); }
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 2); }
    test.test_site_availability (15, 15, 3);
    for (int i = 16; i < 35; ++ i) { test.test_site_availability (i, i, 2); }  
    for (int i = 35; i < 85; ++ i) { test.test_site_availability (i, i, 1); }  
    for (int i = 85; i < 100; ++ i) { test.test_site_availability (i, i, 2); }  
    
    for (int i = 35; i < 85; ++ i) { test().extend_segment (i); }
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 2); }
    test.test_site_availability (15, 15, 3);
    for (int i = 16; i < 100; ++ i) { test.test_site_availability (i, i, 2); } 

    test().remove_sequence (2);
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 0); }
    test.test_site_availability (15, 15, 1);
    for (int i = 16; i < 100; ++ i) { test.test_site_availability (i, i, 0); } 
  }

  // test with segments and bound elements
  {
    SequenceOccupationTest test (100);
    BoundChemical dummy;
    test().start_new_segment (15);
    for (int i = 16; i < 60; ++i) { test().extend_segment (i); }
    test().add_element (dummy, 50, 53);
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 15; i < 50; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 50; i < 54; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 54; i < 60; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 60; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().add_element (dummy, 10, 20);
    for (int i = 0; i < 21; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 21; i < 50; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 50; i < 54; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 54; i < 60; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 60; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().add_element (dummy, 40, 52);
    for (int i = 0; i < 21; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 21; i < 40; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 40; i < 54; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 54; i < 60; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 60; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().remove_element (dummy, 50, 53);
    for (int i = 0; i < 21; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 21; i < 40; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 40; i < 53; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 53; i < 60; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 60; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().start_new_segment (45);
    for (int i = 46; i < 70; ++i) { test().extend_segment (i); }
    for (int i = 0; i < 21; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 21; i < 40; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 40; i < 45; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 45; i < 53; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 53; i < 60; ++ i) { test.test_site_availability (i, i, 2); }
    for (int i = 60; i < 70; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 70; i < 100; ++ i) { test.test_site_availability (i, i, 0); }

    test().remove_element (dummy, 10, 20);
    test().remove_element (dummy, 40, 52);
    for (int i = 0; i < 15; ++ i) { test.test_site_availability (i, i, 0); }
    for (int i = 15; i < 45; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 45; i < 60; ++ i) { test.test_site_availability (i, i, 2); }
    for (int i = 60; i < 70; ++ i) { test.test_site_availability (i, i, 1); }
    for (int i = 70; i < 100; ++ i) { test.test_site_availability (i, i, 0); }
  }

  // test left and right ends
  {
    SequenceOccupationTest test (100);
    BoundChemical dummy;
    std::list <int> left_ends, right_ends;
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    test().add_sequence (1);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    test().start_new_segment (15);
    left_ends.push_back (15); right_ends.push_back (15);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);

    test().start_new_segment (15);
    left_ends.push_back (15); right_ends.push_back (15);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    
    for (int i = 16; i < 31; ++i) { test().extend_segment (i); }
    right_ends.back() = 30;
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
    
    test().start_new_segment (5);
    for (int i = 6; i < 15; ++i) { test().extend_segment (i); }
    for (int i = 16; i < 25; ++i) { test().extend_segment (i); }
    left_ends.back() = 5; right_ends.remove (15); right_ends.push_back (24);
    test.test_left_ends (left_ends); test.test_right_ends (right_ends);
  }

  return EXIT_SUCCESS;
}
