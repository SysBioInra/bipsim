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
#include "../src/chemicalsequence.h"
#include "../src/bindingsitefamily.h"
#include "../src/doublestrand.h"
#include "../src/freeendfactory.h"

class StrandL10
{
public:
  StrandL10 (void) 
    : _sense (std::string (10, 'a'))
    , _antisense (std::string (10, 't'))
    , _ds (_sense, _antisense)
    , _fef (_sense, _antisense, _left, _right)
    , empty_strand (10, _fef)
  {
  }

  bool extend_segment (int start, int end)
  {
    for (int i = start; i < end; ++i) 
      { if (!empty_strand.extend_segment (i)) { return false; } }
    return true;
  }

private:
  ChemicalSequence _sense;
  ChemicalSequence _antisense;
  DoubleStrand _ds;
  BindingSiteFamily _left;
  BindingSiteFamily _right;
  FreeEndFactory _fef;

public:
  PartialStrand empty_strand;

};

BOOST_FIXTURE_TEST_SUITE (BaseTests, StrandL10)
 
BOOST_AUTO_TEST_CASE (completed_emptyStrand_returnsFalse)
{
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
}

BOOST_AUTO_TEST_CASE (extend_emptyStrand_fails)
{
  for (int i = 0; i < 10; ++i) 
    { BOOST_CHECK_EQUAL (empty_strand.extend_segment (i), false); }
}

BOOST_AUTO_TEST_CASE (start_emptyStrand_succeeds)
{
  for (int i = 0; i < 10; ++i) 
    { BOOST_CHECK_EQUAL (empty_strand.start_segment (i), true); }
}

BOOST_AUTO_TEST_CASE (extend_multipleSegmentsOnEmptyStrand_succeeds)
{
  empty_strand.start_segment (7); 
  BOOST_CHECK_EQUAL (extend_segment (8, 10), true);
  BOOST_CHECK_EQUAL (extend_segment (0, 3), true);
  empty_strand.start_segment (5); 
  BOOST_CHECK_EQUAL (extend_segment (6, 7), true);
  BOOST_CHECK_EQUAL (extend_segment (3, 5), true);
}

BOOST_AUTO_TEST_CASE (extend_multipleSegmentsOnEmptyStrand_succeeds2)
{
  empty_strand.start_segment (8); 
  BOOST_CHECK_EQUAL (extend_segment (9, 10), true);
  empty_strand.start_segment (2); 
  BOOST_CHECK_EQUAL (extend_segment (3, 5), true);
  empty_strand.start_segment (6); 
  BOOST_CHECK_EQUAL (empty_strand.extend_segment (0), true);
  BOOST_CHECK_EQUAL (empty_strand.extend_segment (1), true);
  BOOST_CHECK_EQUAL (empty_strand.extend_segment (5), true);
  BOOST_CHECK_EQUAL (empty_strand.extend_segment (7), true);
}

BOOST_AUTO_TEST_CASE (completed_nonCoveringSegmentsOnEmptyStrand_returnsFalse)
{
  empty_strand.start_segment (7); 
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  extend_segment (7, 10);
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  extend_segment (0, 3); 
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
  empty_strand.start_segment (5); 
  extend_segment (6, 7);
  BOOST_CHECK_EQUAL (empty_strand.completed(), false);
}

BOOST_AUTO_TEST_CASE (completed_CoveringSegmentsOnEmptyStrand_returnsTrue)
{
  empty_strand.start_segment (7); extend_segment (8, 10);
  extend_segment (0, 3);  empty_strand.start_segment (5); 
  extend_segment (6, 7);  extend_segment (3, 5);
  BOOST_CHECK_EQUAL (empty_strand.completed(), true);
}
 
BOOST_AUTO_TEST_SUITE_END()
 
