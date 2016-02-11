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
#include <cmath>

#include "unittest.h"
#include "utility.h"

// ==================
//  Project Includes
// ==================
//
#include "../src/sequenceoccupation.h"
#include "../src/boundchemical.h"
#include "../src/bindingsitefamily.h"
#include "../src/bindingsite.h"
#include "../src/chemicalsequence.h"

class DummyBSF : public BindingSiteFamily
{
public:
  DummyBSF (int length) : _cs (std::string (length, 'a'), 0), _k_on (1) {}

  BindingSite* add_site (int first, int last) 
  {
    BindingSite* bs = new BindingSite (*this, _cs, first, last, _k_on, 1);
    _k_on *= _lvl;
    return bs;
  }

  int site_availability (int site_index)
  {
    int result = floor (total_binding_rate_contribution() + 0.5);
    for (int i = 0; i < site_index; ++i) { result /= _lvl; }
    return result % _lvl;
  }

private:
  ChemicalSequence _cs;
  int _k_on;
  static const int _lvl = 10;
};

class SequenceOccupationTest : public UnitTest
{
public:
  SequenceOccupationTest (int length) : _so (length, 0), _bsf (length) {}

  void test_watch_site (int first, int last)
  {
    BindingSite* bs = _bsf.add_site (first, last);
    _so.watch_site (*bs);
  }

  void test_unwatch_moving_site (const BindingSite& bs)
  {
    _so.unwatch_moving_site (bs);
  }
  

  void test_site_availability (int first, int last, int expected) 
  {    
    test (_so.number_available_sites (first, last) == expected,
	  "Number of available sites is incorrect.");
  }

  void test_site_update (int site_index, int expected)
  {
    test (_bsf.site_availability (site_index) == expected,
	  "Observer has not been updated correctly");
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
  DummyBSF _bsf;
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

  // test site watching
  {
    SequenceOccupationTest test (100);
    BoundChemical dummy;
    
    test.test_watch_site (10, 15);
    test.test_site_availability (10, 15, 0); test.test_site_update (0, 0);
    test().add_sequence (1);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test().add_sequence (4);
    test.test_site_availability (10, 15, 5); test.test_site_update (0, 5);
    test().remove_sequence (3);
    test.test_site_availability (10, 15, 2); test.test_site_update (0, 2);

    test.test_watch_site (14, 20);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);

    test.test_watch_site (80, 90);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);

    test().start_new_segment (9);
    for (int i = 10; i < 16; ++i) { test().extend_segment (i); }
    test.test_site_availability (10, 15, 3); test.test_site_update (0, 3);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);

    test().add_element (dummy, 12, 17);
    test.test_site_availability (10, 15, 2); test.test_site_update (0, 2);
    test.test_site_availability (14, 20, 1); test.test_site_update (1, 1);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);

    test().add_element (dummy, 5, 12);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 1); test.test_site_update (1, 1);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);

    for (int i = 16; i < 82; ++i) { test().extend_segment (i); }
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);
    
    test().start_new_segment (13);
    for (int i = 14; i < 25; ++i) { test().extend_segment (i); }
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 3); test.test_site_update (1, 3);
    test.test_site_availability (80, 90, 2); test.test_site_update (2, 2);

    test().add_element (dummy, 17, 82);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 1); test.test_site_update (2, 1);

    test().add_element (dummy, 85, 92);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 1); test.test_site_update (2, 1);

    test().add_element (dummy, 85, 92);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 0); test.test_site_update (2, 0);

    test().add_element (dummy, 85, 92);
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 0); test.test_site_update (2, 0);

    for (int i = 82; i < 100; ++i) { test().extend_segment (i); }
    test.test_site_availability (10, 15, 1); test.test_site_update (0, 1);
    test.test_site_availability (14, 20, 2); test.test_site_update (1, 2);
    test.test_site_availability (80, 90, 0); test.test_site_update (2, 0);    

    test().remove_element (dummy, 5, 12);
    test().remove_element (dummy, 12, 17);
    test().remove_element (dummy, 17, 82);
    test().remove_element (dummy, 85, 92);
    test().remove_element (dummy, 85, 92);
    test().remove_element (dummy, 85, 92);
    test.test_site_availability (10, 15, 3); test.test_site_update (0, 3);
    test.test_site_availability (14, 20, 4); test.test_site_update (1, 4);
    test.test_site_availability (80, 90, 3); test.test_site_update (2, 3);    
  }

  // TODO: test watching moving sites
  {
  }

  return EXIT_SUCCESS;
}
