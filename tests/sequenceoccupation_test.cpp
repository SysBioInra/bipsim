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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SequenceOccupation
#include <boost/test/unit_test.hpp>

#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "../src/sequenceoccupation.h"
#include "../src/boundchemical.h"
#include "../src/doublestrand.h"
#include "../src/freeendfactory.h"
#include "sitedispenser.h"

class SOSingleStrandL100
{
public:
  SOSingleStrandL100 (void) 
    : site_dispenser (100)
    , empty_occupation (100, 0)
  {}

public:
  SequenceOccupation empty_occupation;
  BoundChemical bound_element;
  SiteDispenser site_dispenser;
};

BOOST_FIXTURE_TEST_SUITE (WholeSequences, SOSingleStrandL100)
 
BOOST_AUTO_TEST_CASE (number_sites_emptyOccupation_returnsZero)
{
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 99), 0);
  for (int i = 0; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 0); }
}

BOOST_AUTO_TEST_CASE (number_sites_threeSequencesAdded_returnsThree)
{
  empty_occupation.add_sequence (3);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 99), 3);
  for (int i = 0; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 3); }
}

BOOST_AUTO_TEST_CASE (number_sites_threeSequencesAddedTwoRemoved_returnsOne)
{
  empty_occupation.add_sequence (3); empty_occupation.remove_sequence (2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 99), 1);
  for (int i = 0; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE (WholeSequencesAndBoundElements, SOSingleStrandL100)

BOOST_AUTO_TEST_CASE (number_sites_oneBoundElement_reflectsBoundElement)
{
  empty_occupation.add_sequence (1); 
  empty_occupation.add_element (bound_element, 50, 60);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 99), 0);
  for (int i = 0; i < 50; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 50; i < 61; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 0); }
  for (int i = 61; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 49), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 50), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (60, 99), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_twoBoundElements_reflectsBoundElement)
{
  empty_occupation.add_sequence (1); 
  empty_occupation.add_element (bound_element, 50, 60);
  empty_occupation.add_element (bound_element, 55, 65);
  for (int i = 0; i < 50; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 50; i < 66; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 0); }
  for (int i = 66; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  empty_occupation.add_sequence (1);
  for (int i = 0; i < 50; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
  for (int i = 50; i < 55; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 55; i < 61; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 0); }
  for (int i = 61; i < 66; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 66; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
}

BOOST_AUTO_TEST_CASE (number_sites_twoBoundElementsAddedRemoved_reflectsBoundElement)
{
  empty_occupation.add_sequence (2); 
  empty_occupation.add_element (bound_element, 50, 60);
  empty_occupation.add_element (bound_element, 55, 65);
  empty_occupation.remove_element (bound_element, 50, 60);
  for (int i = 0; i < 55; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
  for (int i = 55; i < 66; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 66; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
  empty_occupation.remove_element (bound_element, 55, 65);
  for (int i = 0; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
}

BOOST_AUTO_TEST_CASE (register_site_registerOneSite_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.register_site (bs);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (add_sequence_addTwoSequences_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.register_site (bs);
  bs.reset_update(); BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.add_sequence (2);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (remove_sequence_removeOneSequence_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.register_site (bs);
  empty_occupation.add_sequence (2);
  bs.reset_update(); BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.remove_sequence (1);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (add_sequence_addOneSequenceThreeSites_sitesUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.register_site (bs);
  MockBindingSite& bs2 = site_dispenser.new_site (15, 25);
  empty_occupation.register_site (bs2);
  MockBindingSite& bs3 = site_dispenser.new_site (30, 40);
  empty_occupation.register_site (bs3);
  bs.reset_update(); BOOST_REQUIRE (bs.was_updated() == false);
  bs2.reset_update(); BOOST_REQUIRE (bs2.was_updated() == false);
  bs3.reset_update(); BOOST_REQUIRE (bs3.was_updated() == false);
  empty_occupation.add_sequence (1);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
  BOOST_CHECK_EQUAL (bs3.was_updated(), true);
}

BOOST_AUTO_TEST_SUITE_END()

class SODoubleStrandL100
{
public:
  SODoubleStrandL100 (void) 
    : _sense (std::string (100, 'a'))
    , _antisense (std::string (100, 't'))
    , _ds (_sense, _antisense)
    , _fef (_sense, _antisense, _left, _right)
    , site_dispenser (100)
    , empty_occupation (100, 0)
  {
    empty_occupation.set_free_end_factory (_fef);
  }

  void extend_segment (int start, int end)
  {
    for (int i = start; i <= end; ++i) { empty_occupation.extend_segment (i); }
  }

private:
  ChemicalSequence _sense;
  ChemicalSequence _antisense;
  DoubleStrand _ds;
  BindingSiteFamily _left;
  BindingSiteFamily _right;
  FreeEndFactory _fef;

public:
  SequenceOccupation empty_occupation;
  SiteDispenser site_dispenser;
  BoundChemical bound_element;
  static const int L = 100;
};

BOOST_FIXTURE_TEST_SUITE (Segments, SODoubleStrandL100)

BOOST_AUTO_TEST_CASE (number_sites_SegmentSizeOne_reflectsSegmentOverlap)
{
  empty_occupation.start_segment (10);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (11, 11), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentZeroZero_reflectsSegmentOverlap)
{
  empty_occupation.start_segment (0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (1, 1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentLastBase_reflectsSegmentOverlap)
{
  empty_occupation.start_segment (L-1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-2, L-2), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentSizeEleven_reflectsSegmentOverlap)
{
  empty_occupation.start_segment (10); extend_segment (11, 20);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (11, 11), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 20), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 21), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_oneSegmentOneSequence_reflectsOccupation)
{
  empty_occupation.start_segment (10); extend_segment (11, 20);
  empty_occupation.add_sequence (1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (21, 21), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_twoOverlappingSegments_reflectsSegmentOverlap)
{
  empty_occupation.start_segment (10); extend_segment (11, 30);
  empty_occupation.start_segment (20); extend_segment (21, 40);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (19, 19), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (30, 30), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (31, 31), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (40, 40), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (41, 41), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 30), 2);
}

BOOST_AUTO_TEST_CASE (number_sites_oneSegmentOneBoundElement_reflectsOccupation)
{
  empty_occupation.start_segment (10); extend_segment (11, 30);
  empty_occupation.add_element (bound_element, 20, 25);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (19, 19), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (26, 26), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_oneSegmentTwoBoundElements_reflectsOccupation)
{
  empty_occupation.start_segment (10); extend_segment (11, 35);
  empty_occupation.add_element (bound_element, 15, 25);
  empty_occupation.add_element (bound_element, 20, 30);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (14, 14), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (15, 15), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (30, 30), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (31, 31), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_twoSegmentsTwoBoundElements_reflectsOccupation)
{
  empty_occupation.start_segment (10); extend_segment (11, 60);
  empty_occupation.start_segment (30); extend_segment (31, 80);
  empty_occupation.add_element (bound_element, 20, 50);
  empty_occupation.add_element (bound_element, 40, 70);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (19, 19), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (29, 29), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (30, 30), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (39, 39), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (40, 40), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (50, 50), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (51, 51), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (60, 60), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (61, 61), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (70, 70), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (71, 71), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_oneSegmentTwoBoundElementsAddedAndRemoved_reflectsSegment)
{
  empty_occupation.start_segment (10); extend_segment (11, 35);
  empty_occupation.add_element (bound_element, 15, 25);
  empty_occupation.add_element (bound_element, 20, 30);
  empty_occupation.remove_element (bound_element, 15, 25);
  empty_occupation.remove_element (bound_element, 20, 30); 
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (15, 15), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (35, 35), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_segmentsCoveringWholeSequence_returnsOne)
{
  empty_occupation.start_segment (10); extend_segment (11, 50);
  empty_occupation.start_segment (90); extend_segment (91, L-1);
  extend_segment (0, 9); extend_segment (51, 89);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 1);
}

BOOST_AUTO_TEST_CASE (register_moving_site_registerOneSite_updatesSite)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.register_moving_site (bs);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (start_segment_startOnSite_updatesSite)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (10, 20);
  MockBindingSite& bs3 = site_dispenser.new_site (21, 30);
  empty_occupation.register_site (bs);
  empty_occupation.register_moving_site (bs2);
  empty_occupation.register_site (bs3);
  bs.reset_update(); bs2.reset_update(); bs3.reset_update();
  empty_occupation.start_segment (10);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
  BOOST_CHECK_EQUAL (bs3.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (start_segment_startOnSite_updatesSite2)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (10, 20);
  MockBindingSite& bs3 = site_dispenser.new_site (21, 30);
  empty_occupation.register_site (bs);
  empty_occupation.register_moving_site (bs2);
  empty_occupation.register_site (bs3);
  bs.reset_update(); bs2.reset_update(); bs3.reset_update();
  empty_occupation.start_segment (20);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
  BOOST_CHECK_EQUAL (bs3.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (extend_segment_extendOnSite_updatesSite)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (15, 25);
  empty_occupation.register_site (bs);
  empty_occupation.register_moving_site (bs2);
  bs.reset_update(); bs2.reset_update();
  empty_occupation.start_segment (9);
  empty_occupation.extend_segment (10);
  BOOST_CHECK_EQUAL (bs.was_updated(), true); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
  extend_segment (11, 14);
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
  bs.reset_update();
  empty_occupation.extend_segment (15);
  BOOST_CHECK_EQUAL (bs.was_updated(), true); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
  extend_segment (16, 20); bs.reset_update(); bs2.reset_update();
  empty_occupation.extend_segment (21);
  BOOST_CHECK_EQUAL (bs.was_updated(), false); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_SUITE_END() 
