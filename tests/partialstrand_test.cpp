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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PartialStrand
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr
#include <sstream>
#include <string>
#include "utility.h"

// ==================
//  Project Includes
// ==================
//
#include "../src/partialstrand.h"

class StrandL10
{
public:
  StrandL10 (void) 
    : empty_strand (10)
  {
  }

  bool extend_segment (int start, int end)
  {
    for (int i = start; i < end; ++i) 
      { if (!empty_strand.occupy (i)) { return false; } }
    return true;
  }

public:
  PartialStrand empty_strand;

};

BOOST_FIXTURE_TEST_SUITE (BaseTests, StrandL10)
 
BOOST_AUTO_TEST_CASE (completed_emptyStrand_returnsFalse)
{
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds)
{
  BOOST_CHECK_EQUAL (empty_strand.occupy (0), true);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds2)
{
  BOOST_CHECK_EQUAL (empty_strand.occupy (5), true);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds3)
{
  BOOST_CHECK_EQUAL (empty_strand.occupy (9), true);
}

BOOST_AUTO_TEST_CASE (occupy_occupyTwiceSamePosition_fails)
{
  empty_strand.occupy (0);
  BOOST_CHECK_EQUAL (empty_strand.occupy (0), false);
}

BOOST_AUTO_TEST_CASE (occupy_multipleSegmentsOnEmptyStrand_succeeds)
{
  BOOST_CHECK_EQUAL (extend_segment (7, 10), true);
  BOOST_CHECK_EQUAL (extend_segment (0, 3), true);
  BOOST_CHECK_EQUAL (extend_segment (5, 7), true);
  BOOST_CHECK_EQUAL (extend_segment (3, 5), true);
}

BOOST_AUTO_TEST_CASE (occupy_multipleSegmentsOnEmptyStrand_succeeds2)
{
  BOOST_CHECK_EQUAL (extend_segment (8, 10), true);
  BOOST_CHECK_EQUAL (extend_segment (2, 5), true);
  empty_strand.occupy (6); 
  BOOST_CHECK_EQUAL (empty_strand.occupy (0), true);
  BOOST_CHECK_EQUAL (empty_strand.occupy (1), true);
  BOOST_CHECK_EQUAL (empty_strand.occupy (5), true);
  BOOST_CHECK_EQUAL (empty_strand.occupy (7), true);
}

BOOST_AUTO_TEST_CASE (completed_nonCoveringSegmentsOnEmptyStrand_returnsFalse)
{
  empty_strand.occupy (7); 
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  extend_segment (8, 10);
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  extend_segment (0, 3); 
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  extend_segment (5, 7);
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
}

BOOST_AUTO_TEST_CASE (completed_CoveringSegmentsOnEmptyStrand_returnsTrue)
{
  extend_segment (7, 10); extend_segment (0, 3);  
  extend_segment (5, 7);  extend_segment (3, 5);
  BOOST_CHECK_EQUAL (empty_strand.completed(), true);
}

BOOST_AUTO_TEST_CASE (completed_CoveringSegmentsOnEmptyStrand_returnsTrue2)
{
  for (int i = 9; i >= 0; --i) { empty_strand.occupy (i); }
  BOOST_CHECK_EQUAL (empty_strand.completed(), true);
}
 
BOOST_AUTO_TEST_SUITE_END()
 
