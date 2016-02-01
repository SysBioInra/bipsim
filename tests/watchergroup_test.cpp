/**
 * @file watchergroup_test.cpp
 * @brief Unit testing for WatcherGroup class.
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

// ==================
//  Project Includes
// ==================
//
#include "../src/watchergroup.h"
#include "../src/bindingsite.h"
#include "../src/siteavailability.h"
#include "utility.h"

class WatcherGroupTest : public UnitTest
{
public:
  WatcherGroupTest (int length) : _cs (std::string (length, 'A')) {}

  void test_add_site (int first, int last) 
  { 
    BindingSite dummy_bs (_bsf, _cs, first, last, 1, 1);
    SiteAvailability* sa = _wgroup.add_watcher (dummy_bs);
    test (&(sa->site()) == &dummy_bs, 
	  "SiteAvailability object incorrectly initialized");
    _watchers.push_back (sa);
  }

  void test_absorption (WatcherGroupTest& other_group)
  {
    _watchers.splice (_watchers.end(), other_group._watchers,
		      other_group._watchers.begin(), 
		      other_group._watchers.end());
    _wgroup.absorb (other_group._wgroup);
    other_group.test_watchers();
    test_watchers();
  }

  void test_watchers (void) const
  {
    test (compare_lists (_watchers, _wgroup.watchers()),
	  "Watcher list incorrect.");
  }

  void test_first (int position) const
  {
    test (_wgroup.first() == position, "First position of group is incorrect");
  }

  void test_last (int position) const
  {
    test (_wgroup.last() == position, "Last position of group is incorrect");
  }

private:
  WatcherGroup _wgroup;
  BindingSiteFamily _bsf;
  ChemicalSequence _cs;
  std::list <SiteAvailability*> _watchers;
};

int main (int argc, char *argv[])
{ 
  // Test adding sites
  {
    WatcherGroupTest test (100);
    test.test_first (std::numeric_limits <int>::max()); test.test_last (-1);
    // note that sites are in the [1, length] region 
    // while groups are in [0, length-1]
    test.test_add_site (10, 19); test.test_watchers(); 
    test.test_first (9); test.test_last (18);
    test.test_add_site (20, 39); test.test_watchers(); 
    test.test_first (9); test.test_last (38);
    test.test_add_site (10, 39); test.test_watchers(); 
    test.test_first (9); test.test_last (38);
    test.test_add_site (100, 100); test.test_watchers(); 
    test.test_first (9); test.test_last (99);
    test.test_add_site (51, 73); test.test_watchers(); 
    test.test_first (9); test.test_last (99);
    test.test_add_site (1, 3); test.test_watchers(); 
    test.test_first (0); test.test_last (99);
    test.test_add_site (1, 100); test.test_watchers(); 
    test.test_first (0); test.test_last (99);
    test.test_add_site (1, 100); test.test_watchers(); 
    test.test_first (0); test.test_last (99);
  }

  // Test adding sites
  {
    WatcherGroupTest test (100);
    // note that sites are in the [1, length] region 
    // while groups are in [0, length-1]
    test.test_add_site (1, 100); test.test_watchers(); 
    test.test_first (0); test.test_last (99);
  }

  // Test absorption
  {
    WatcherGroupTest test (100), test2 (100);
    test.test_add_site (6, 21); test.test_add_site (15, 31);
    test2.test_add_site (46, 51); test2.test_add_site (45, 48);
    test.test_absorption (test2);
    test.test_first (5); test.test_last (50);
    test2.test_first (std::numeric_limits <int>::max()); test2.test_last (-1);
    test2.test_add_site (53,90); test2.test_add_site (3,10);
    test.test_absorption (test2);
    test.test_first (2); test.test_last (89);
    test2.test_first (std::numeric_limits <int>::max()); test2.test_last (-1);
  }

  return EXIT_SUCCESS;
}

