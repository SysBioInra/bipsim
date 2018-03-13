
/**
 * @file partialstrand_test.cpp
 * @brief Unit testing for PartialStrand class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PartialStrand
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include "utility.h"

// ==================
//  Project Includes
// ==================
//
#include "partialstrand.h"

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
      { 
	if (empty_strand.occupied (i)) { return false; } 
	empty_strand.occupy (i);
      }
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

BOOST_AUTO_TEST_CASE (empty_emptyStrand_returnsTrue)
{
  BOOST_CHECK_EQUAL (empty_strand.empty(), true);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds)
{
  empty_strand.occupy (0);
  BOOST_CHECK_EQUAL (empty_strand.occupied (0), true);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds2)
{
  empty_strand.occupy (5);
  BOOST_CHECK_EQUAL (empty_strand.occupied (5), true);
}

BOOST_AUTO_TEST_CASE (occupy_emptyStrand_succeeds3)
{
  empty_strand.occupy (9);
  BOOST_CHECK_EQUAL (empty_strand.occupied (9), true);
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
  BOOST_CHECK_EQUAL (empty_strand.occupied (6), false);
  empty_strand.occupy (6); 
  BOOST_CHECK_EQUAL (empty_strand.occupied (0), false);
  empty_strand.occupy (0);
  BOOST_CHECK_EQUAL (empty_strand.occupied (1), false);
  empty_strand.occupy (1);
  BOOST_CHECK_EQUAL (empty_strand.occupied (5), false);
  empty_strand.occupy (5);
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

BOOST_AUTO_TEST_CASE (segments_oneSegment_returnsSegmentBounds)
{
  for (int i = 0; i <= 5 ; ++i) { empty_strand.occupy (i); }
  std::vector <int> result = empty_strand.segments();
  BOOST_CHECK_EQUAL (result.size(), 2);
  BOOST_CHECK_EQUAL (result[0], 0);
  BOOST_CHECK_EQUAL (result[1], 5);
}

BOOST_AUTO_TEST_CASE (segments_twoDisjointSegments_returnsSegmentBounds)
{
  for (int i = 0; i <= 1 ; ++i) { empty_strand.occupy (i); }
  for (int i = 5; i <= 6 ; ++i) { empty_strand.occupy (i); }
  std::vector <int> result = empty_strand.segments();
  BOOST_CHECK_EQUAL (result.size(), 4);
  BOOST_CHECK_EQUAL (result[0], 0);
  BOOST_CHECK_EQUAL (result[1], 1);
  BOOST_CHECK_EQUAL (result[2], 5);
  BOOST_CHECK_EQUAL (result[3], 6);
}

BOOST_AUTO_TEST_CASE (segments_twoJoiningSegments_returnsSegmentBounds)
{
  for (int i = 4; i <= 5 ; ++i) { empty_strand.occupy (i); }
  for (int i = 6; i <= 7 ; ++i) { empty_strand.occupy (i); }
  std::vector <int> result = empty_strand.segments();
  BOOST_CHECK_EQUAL (result.size(), 2);
  BOOST_CHECK_EQUAL (result[0], 4);
  BOOST_CHECK_EQUAL (result[1], 7);
}
 
BOOST_AUTO_TEST_SUITE_END()
 
