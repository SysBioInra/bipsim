/**
 * @file siteavailability_test.cpp
 * @brief Unit testing for SiteAvailability class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <cmath> // fabs

#include "unittest.h"

// ==================
//  Project Includes
// ==================
//
#include "../src/siteavailability.h"
#include "../src/bindingsite.h"

class SiteAvailabilityTest : public UnitTest
{
public:
  SiteAvailabilityTest (int first, int last) 
    : _cs (std::string (last, 'a'))
    , _bs (_bsf, _cs, first, last, 1, 1)
    , _sa (_bs)
  { test (&(_sa.site()) == &_bs, "Wrong initialization."); }
  
  void test_notify (int number)
  {
    _sa.notify (number);
    test (fabs (_bsf.total_binding_rate_contribution() - number) < 1e-15,
	  "Notification failed.");
  }
  
  void test_is_affected (int a, int b, bool expected)
  {
    test (_sa.is_affected (_cs.relative (a), _cs.relative (b)) == expected,
	  "Considers that site is affected when it is not (or conversely).");
  }

private:
  BindingSiteFamily _bsf;
  ChemicalSequence _cs;
  BindingSite _bs;
  SiteAvailability _sa;
};

int main (int argc, char *argv[])
{ 
  {
    SiteAvailabilityTest test (10, 15);
    test.test_notify (0);
    test.test_notify (10);
    test.test_notify (20);
    test.test_notify (0);
    test.test_is_affected (10, 15, true);
    test.test_is_affected (0, 10, true);
    test.test_is_affected (10, 10, true);
    test.test_is_affected (0, 9, false);
    test.test_is_affected (0, 20, true);
    test.test_is_affected (11, 11, true);
    test.test_is_affected (11, 20, true);
    test.test_is_affected (15, 20, true);
    test.test_is_affected (16, 20, false);
  }

  return EXIT_SUCCESS;
}

