/**
 * @file boundchemical_test.cpp
 * @brief Unit testing for BoundChemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BoundChemical
#include <boost/test/unit_test.hpp>
// #include <boost/test/floating_point_comparison.hpp>

// ==================
//  Project Includes
// ==================
//
#include "boundchemical.h"
#include "boundunit.h"
#include "mockrateinvalidator.h"
#include "sitedispenser.h"
#include "ratevalidity.h"
#include "templatefilter.h"
#include "familyfilter.h"

class BoundChemicalBaseTestsFixture
{
public:
  BoundChemicalBaseTestsFixture (void)
    : _site_dispenser (1)
    , bound_unit_1 (_site_dispenser.new_site (0,0))
    , bound_unit_2 (_site_dispenser.new_site (0,0))
  {}

private:
  SiteDispenser _site_dispenser;
  
public:
  BoundChemical bound_chemical;
  BoundUnit bound_unit_1;
  BoundUnit bound_unit_2;
};


BOOST_FIXTURE_TEST_SUITE (BaseTests, BoundChemicalBaseTestsFixture)
 
BOOST_AUTO_TEST_CASE (add_callAtCreation_returnsZero)
{
  BOOST_CHECK_EQUAL (bound_chemical.number(), 0);
}

BOOST_AUTO_TEST_CASE (add_addOneUnit_numberIsOne)
{
  bound_chemical.add (bound_unit_1);
  BOOST_CHECK_EQUAL (bound_chemical.number(), 1);
}

BOOST_AUTO_TEST_CASE (add_addTwoUnits_numberIsTwo)
{
  bound_chemical.add (bound_unit_1);
  bound_chemical.add (bound_unit_2);
  BOOST_CHECK_EQUAL (bound_chemical.number(), 2);
}

BOOST_AUTO_TEST_CASE (remove_addRemoveOneUnit_numberIsZero)
{
  bound_chemical.add (bound_unit_1);
  bound_chemical.remove (bound_unit_1);
  BOOST_CHECK_EQUAL (bound_chemical.number(), 0);
}

BOOST_AUTO_TEST_CASE (remove_addTwoRemoveOneUnit_numberIsOne)
{
  bound_chemical.add (bound_unit_1);
  bound_chemical.add (bound_unit_2);
  bound_chemical.remove (bound_unit_1);
  BOOST_CHECK_EQUAL (bound_chemical.number(), 1);
}

BOOST_AUTO_TEST_CASE (random_unit_addUnitOne_returnsUnitOne)
{
  bound_chemical.add (bound_unit_1);
  BOOST_CHECK_EQUAL (&(bound_chemical.random_unit()), &bound_unit_1);
}

BOOST_AUTO_TEST_CASE (random_unit_addUnitTwo_returnsUnitTwo)
{
  bound_chemical.add (bound_unit_2);
  BOOST_CHECK_EQUAL (&(bound_chemical.random_unit()), &bound_unit_2);
}

BOOST_AUTO_TEST_CASE (remove_addTwoUnitsRemoveSecond_randomReturnsFirst)
{
  bound_chemical.add (bound_unit_1);
  bound_chemical.add (bound_unit_2);
  bound_chemical.remove (bound_unit_2);
  BOOST_CHECK_EQUAL (&(bound_chemical.random_unit()), &bound_unit_1);
}

BOOST_AUTO_TEST_CASE (remove_addTwoUnitsRemoveFirst_randomReturnsSecond)
{
  bound_chemical.add (bound_unit_1);
  bound_chemical.add (bound_unit_2);
  bound_chemical.remove (bound_unit_1);
  BOOST_CHECK_EQUAL (&(bound_chemical.random_unit()), &bound_unit_2);
}
 
BOOST_AUTO_TEST_SUITE_END()

class BoundChemicalThreeObserversFixture
{
public:
  BoundChemicalThreeObserversFixture (void)
    : _site_dispenser (1)
    , bound_unit (_site_dispenser.new_site (0,0))
    , observers (3)
    , _rate_validity (3)
  {
    for (int i = 0; i < observers.size(); ++i)
      { 
	observers [i] = new MockRateInvalidator (_rate_validity, i); 
	bound_chemical.attach (*(observers[i]));
	BOOST_REQUIRE (observers[i]->was_updated() == false);
      }
  }
  
  ~BoundChemicalThreeObserversFixture (void)
  {
    for (int i = 0; i < observers.size(); ++i)
      { delete observers [i]; }
  }

private:
  SiteDispenser _site_dispenser;
  RateValidity _rate_validity;
  
public:
  BoundChemical bound_chemical;
  BoundUnit bound_unit;
  std::vector <MockRateInvalidator*> observers; 
};

BOOST_FIXTURE_TEST_SUITE (ObserverTest, BoundChemicalThreeObserversFixture)

BOOST_AUTO_TEST_CASE (add_calledOnce_observersUpdated)
{
  bound_chemical.add (bound_unit);
  for (int i = 0; i < 3; ++i)
    { BOOST_CHECK_EQUAL (observers[i]->was_updated(), true); }
}

BOOST_AUTO_TEST_CASE (remove_calledOnce_observersUpdated)
{
  bound_chemical.add (bound_unit);
  for (int i = 0; i < 3; ++i) { observers[i]->reset_update(); }
  bound_chemical.remove (bound_unit);
  for (int i = 0; i < 3; ++i)
    { BOOST_CHECK_EQUAL (observers[i]->was_updated(), true); }
}

BOOST_AUTO_TEST_SUITE_END()

class BoundChemicalFilterFixture
{
public:
  BoundChemicalFilterFixture (void)
    : _site_dispenser (1)
    , bound_unit (_site_dispenser.new_site (0,0))
  {}

private:
  SiteDispenser _site_dispenser;
  
public:
  BoundChemical bound_chemical;
  BoundUnit bound_unit;
};

BOOST_FIXTURE_TEST_SUITE (FilterTest, BoundChemicalFilterFixture)

BOOST_AUTO_TEST_SUITE_END()
