/**
 * @file sitegroup_test.cpp
 * @brief Unit testing for SiteGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SiteGroup
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <list>

// ==================
//  Project Includes
// ==================
//
#include "sitegroup.h"
#include "bindingsite.h"
#include "chemicalsequence.h"
#include "sitedispenser.h"

class SiteGroupL100
{
public:
  SiteGroupL100 (void) 
    : site_dispenser (100)
  {}  
  
public:
  SiteGroup empty_group;
  SiteDispenser site_dispenser;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, SiteGroupL100)
 
BOOST_AUTO_TEST_CASE (first_emptyGroup_returnsMax)
{
  BOOST_CHECK_EQUAL (empty_group.first(), std::numeric_limits <int>::max());
}

BOOST_AUTO_TEST_CASE (last_emptyGroup_returnsMinus1)
{
  BOOST_CHECK_EQUAL (empty_group.last(), -1);
}

BOOST_AUTO_TEST_CASE (first_GroupWithOneSite_returnsFirstBaseOfSite)
{
  empty_group.add_site (site_dispenser.new_site (10, 20));
  BOOST_CHECK_EQUAL (empty_group.first(), 10);
}

BOOST_AUTO_TEST_CASE (last_GroupWithOneSite_returnsLastBaseOfSite)
{
  empty_group.add_site (site_dispenser.new_site (10, 20));
  BOOST_CHECK_EQUAL (empty_group.last(), 20);
}

BOOST_AUTO_TEST_CASE (first_GroupWithTwoSites_returnsFirstBaseOfFirstSite)
{
  empty_group.add_site (site_dispenser.new_site (10, 20));
  empty_group.add_site (site_dispenser.new_site (20, 30));
  BOOST_CHECK_EQUAL (empty_group.first(), 10);
}

BOOST_AUTO_TEST_CASE (last_GroupWithTwoSites_returnsLatBaseOfLastSite)
{
  empty_group.add_site (site_dispenser.new_site (10, 20));
  empty_group.add_site (site_dispenser.new_site (20, 30));
  BOOST_CHECK_EQUAL (empty_group.last(), 30);
}

BOOST_AUTO_TEST_CASE (absorb_FusionTwoGroups_BoundsAreCorrect)
{
  SiteGroup group1; group1.add_site (site_dispenser.new_site (10, 20));
  group1.add_site (site_dispenser.new_site (50, 60));
  SiteGroup group2; group2.add_site (site_dispenser.new_site (70, 80));
  group2.add_site (site_dispenser.new_site (30, 40));
  group1.absorb (group2);
  BOOST_CHECK_EQUAL (group1.first(), 10);
  BOOST_CHECK_EQUAL (group1.last(), 80);
  BOOST_CHECK_EQUAL (group2.first(), std::numeric_limits <int>::max());
  BOOST_CHECK_EQUAL (group2.last(), -1);
}
 
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE (UpdateTests, SiteGroupL100)

BOOST_AUTO_TEST_CASE (update_all_oneSite_isUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  empty_group.add_site (bs); BOOST_CHECK_EQUAL (bs.was_updated(), false);
  empty_group.update_all(); BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_oneSiteUpdateOnSite_isUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  empty_group.add_site (bs); empty_group.update (10,20);
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_oneSiteUpdateCrossingSite_isUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  empty_group.add_site (bs); empty_group.update (15,25); 
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_oneSiteUpdateCrossingSite_isUpdated2)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  empty_group.add_site (bs); empty_group.update (0,30); 
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_oneSiteUpdateNonOverlapping_isNotUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  empty_group.add_site (bs); empty_group.update (0,9); 
  BOOST_CHECK_EQUAL (bs.was_updated(), false);
  empty_group.update (30,40); BOOST_CHECK_EQUAL (bs.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (update_all_twoSite_isUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  MockBindingSite& bs2 = site_dispenser.new_site (30,40);
  empty_group.add_site (bs); empty_group.add_site (bs2);  
  empty_group.update_all(); 
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_twoSitesUpdateOverlapsSiteTwo_siteTwoUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  MockBindingSite& bs2 = site_dispenser.new_site (30,40);
  empty_group.add_site (bs); empty_group.add_site (bs2);  
  empty_group.update (35, 45); 
  BOOST_CHECK_EQUAL (bs.was_updated(), false);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_twoSitesUpdateOverlapsBothSites_bothSitesUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  MockBindingSite& bs2 = site_dispenser.new_site (30,40);
  empty_group.add_site (bs); empty_group.add_site (bs2);  
  empty_group.update (15, 35); 
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_CASE (update_twoSitesUpdateNonOverlapping_noneUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,20);
  MockBindingSite& bs2 = site_dispenser.new_site (30,40);
  empty_group.add_site (bs); empty_group.add_site (bs2);  
  empty_group.update (21, 29); 
  BOOST_CHECK_EQUAL (bs.was_updated(), false);
  BOOST_CHECK_EQUAL (bs2.was_updated(), false);
}

BOOST_AUTO_TEST_CASE (update_twoOverlappingSitesUpdateInOverlap_bothUpdated)
{
  MockBindingSite& bs = site_dispenser.new_site (10,30);
  MockBindingSite& bs2 = site_dispenser.new_site (20,40);
  empty_group.add_site (bs); empty_group.add_site (bs2);  
  empty_group.update (21, 29); 
  BOOST_CHECK_EQUAL (bs.was_updated(), true);
  BOOST_CHECK_EQUAL (bs2.was_updated(), true);
}

BOOST_AUTO_TEST_SUITE_END()

