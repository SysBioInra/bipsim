
/**
 * @file sequenceoccupation_test.cpp
 * @brief Unit testing for SequenceOccupation class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SequenceOccupation
#include <boost/test/unit_test.hpp>

// ==================
//  Project Includes
// ==================
//
#include "sequenceoccupation.h"
#include "doublestrand.h"
#include "sitedispenser.h"

class SOSingleStrandL100
{
public:
  SOSingleStrandL100 (void) 
    : site_dispenser (100)
    , empty_occupation (100)
  {}

public:
  SequenceOccupation empty_occupation;
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
  empty_occupation.add_element (50, 60);
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
  empty_occupation.add_element (50, 60);
  empty_occupation.add_element (55, 65);
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
  empty_occupation.add_element (50, 60);
  empty_occupation.add_element (55, 65);
  empty_occupation.remove_element (50, 60);
  for (int i = 0; i < 55; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
  for (int i = 55; i < 66; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 1); }
  for (int i = 66; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
  empty_occupation.remove_element (55, 65);
  for (int i = 0; i < 100; ++ i) 
    { BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (i, i), 2); }
}

BOOST_AUTO_TEST_CASE (watch_site_registerOneSite_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.watch_site (bs);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (add_sequence_addTwoSequences_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.watch_site (bs);
  bs.reset_update(); BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.add_sequence (2);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (remove_sequence_removeOneSequence_siteUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.watch_site (bs);
  empty_occupation.add_sequence (2);
  bs.reset_update(); BOOST_REQUIRE (bs.was_updated() == false);
  empty_occupation.remove_sequence (1);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (add_sequence_addOneSequenceThreeSites_sitesUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  empty_occupation.watch_site (bs);
  MockBindingSite& bs2 = site_dispenser.new_site (15, 25);
  empty_occupation.watch_site (bs2);
  MockBindingSite& bs3 = site_dispenser.new_site (30, 40);
  empty_occupation.watch_site (bs3);
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
    : site_dispenser (100)
    , empty_occupation (100)
  {
  }

  bool extend_strand (int strand_id, int start, int end)
  {
    for (int i = start; i <= end; ++i) 
      { 
	if (!empty_occupation.extend_strand (strand_id, i)) { return false; }
      }
    return true;
  }

public:
  SequenceOccupation empty_occupation;
  SiteDispenser site_dispenser;
  static const int L = 100;
};

BOOST_FIXTURE_TEST_SUITE (Segments, SODoubleStrandL100)

BOOST_AUTO_TEST_CASE (number_sites_SegmentSizeOne_reflectsSegmentOverlap)
{
  int strand_id = empty_occupation.partial_strand_id (10);
  empty_occupation.extend_strand (strand_id, 10);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (11, 11), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentZeroZero_reflectsSegmentOverlap)
{
  int strand_id = empty_occupation.partial_strand_id (0);
  empty_occupation.extend_strand (strand_id, 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (1, 1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentLastBase_reflectsSegmentOverlap)
{
  int strand_id = empty_occupation.partial_strand_id (L-1);
  empty_occupation.extend_strand (strand_id, L-1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, 0), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-2, L-2), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (L-1, L-1), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 0);
}

BOOST_AUTO_TEST_CASE (number_sites_SegmentSizeEleven_reflectsSegmentOverlap)
{
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 20));
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
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 20));
  empty_occupation.add_sequence (1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (9, 9), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (10, 10), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 2);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (21, 21), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_twoOverlappingSegments_reflectsSegmentOverlap)
{
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 30));
  int strand2 = empty_occupation.partial_strand_id (20); 
  BOOST_REQUIRE (extend_strand (strand2, 20, 40));
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
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 30));
  empty_occupation.add_element (20, 25);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (19, 19), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (26, 26), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_oneSegmentTwoBoundElements_reflectsOccupation)
{
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 11, 35));
  empty_occupation.add_element (15, 25);
  empty_occupation.add_element (20, 30);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (14, 14), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (15, 15), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (30, 30), 0);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (31, 31), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_twoSegmentsTwoBoundElements_reflectsOccupation)
{
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 60));
  int strand2 = empty_occupation.partial_strand_id (30); 
  BOOST_REQUIRE (extend_strand (strand2, 30, 80));
  empty_occupation.add_element (20, 50);
  empty_occupation.add_element (40, 70);
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
  int strand1 = empty_occupation.partial_strand_id (10);
  BOOST_REQUIRE (extend_strand (strand1, 10, 35));
  empty_occupation.add_element (15, 25);
  empty_occupation.add_element (20, 30);
  empty_occupation.remove_element (15, 25);
  empty_occupation.remove_element (20, 30); 
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (15, 15), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (20, 20), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (25, 25), 1);
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (35, 35), 1);
}

BOOST_AUTO_TEST_CASE (number_sites_segmentsCoveringWholeSequence_returnsOne)
{
  int strand1 = empty_occupation.partial_strand_id (10); 
  BOOST_REQUIRE (extend_strand (strand1, 10, 50));
  int strand2 = empty_occupation.partial_strand_id (90); 
  BOOST_REQUIRE (extend_strand (strand2, 90, L-1));
  BOOST_REQUIRE (extend_strand (strand2, 0, 9)); 
  BOOST_REQUIRE (extend_strand (strand1, 51, 89));
  BOOST_CHECK_EQUAL (empty_occupation.number_available_sites (0, L-1), 1);
}

BOOST_AUTO_TEST_CASE (extend_strand_startOnSite_updatesSite)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (21, 30);
  empty_occupation.watch_site (bs);
  empty_occupation.watch_site (bs2);
  bs.reset_update(); bs2.reset_update();
  int strand_id = empty_occupation.partial_strand_id (10);
  empty_occupation.extend_strand (strand_id, 10);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (extend_strand_startOnSite_updatesSite2)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (21, 30);
  empty_occupation.watch_site (bs);
  empty_occupation.watch_site (bs2);
  bs.reset_update(); bs2.reset_update();
  int strand_id = empty_occupation.partial_strand_id (20);
  empty_occupation.extend_strand (strand_id, 20);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (extend_strand_extendOnSite_updatesSite)
{
  MockBindingSite& bs = site_dispenser.new_site (10, 20);
  MockBindingSite& bs2 = site_dispenser.new_site (15, 25);
  empty_occupation.watch_site (bs);
  empty_occupation.watch_site (bs2);
  bs.reset_update(); bs2.reset_update();
  int strand1 = empty_occupation.partial_strand_id (9);
  empty_occupation.extend_strand (strand1, 9);
  BOOST_REQUIRE (empty_occupation.extend_strand (strand1, 10));
  BOOST_CHECK_EQUAL (bs.was_updated(), true); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
  BOOST_REQUIRE (extend_strand (strand1, 11, 14));
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
  bs.reset_update();
  BOOST_REQUIRE (empty_occupation.extend_strand (strand1, 15));
  BOOST_CHECK_EQUAL (bs.was_updated(), true); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
  BOOST_REQUIRE (extend_strand (strand1, 16, 20)); 
  bs.reset_update(); bs2.reset_update();
  BOOST_REQUIRE (empty_occupation.extend_strand (strand1, 21));
  BOOST_CHECK_EQUAL (bs.was_updated(), false); 
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_SUITE_END() 
