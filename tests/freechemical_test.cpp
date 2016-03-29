/**
 * @file freechemical_test.cpp
 * @brief Unit testing for FreeChemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE FreeChemical
#include <boost/test/unit_test.hpp>
// #include <boost/test/floating_point_comparison.hpp>

// ==================
//  Project Includes
// ==================
//
#include "../src/freechemical.h"
#include "mockrateinvalidator.h"
#include "../src/ratevalidity.h"


BOOST_AUTO_TEST_SUITE (BaseTests)
 
BOOST_AUTO_TEST_CASE (number_callAtCreation_returnsZero)
{
  FreeChemical test;
  BOOST_CHECK_EQUAL (test.number(), 0);
}

BOOST_AUTO_TEST_CASE (add_addOneMolecule_numberIsOne)
{
  FreeChemical test;
  test.add (1);
  BOOST_CHECK_EQUAL (test.number(), 1);
}

BOOST_AUTO_TEST_CASE (add_addZeroMolecules_numberIsZero)
{
  FreeChemical test;
  test.add (0);
  BOOST_CHECK_EQUAL (test.number(), 0);
}

BOOST_AUTO_TEST_CASE (add_addTenMolecules_numberIsTen)
{
  FreeChemical test;
  test.add (10);
  BOOST_CHECK_EQUAL (test.number(), 10);
}

BOOST_AUTO_TEST_CASE (remove_removeZeroMolecules_numberIsZero)
{
  FreeChemical test;
  test.remove (0);
  BOOST_CHECK_EQUAL (test.number(), 0);
}

BOOST_AUTO_TEST_CASE (remove_AddOneRemoveOne_numberIsZero)
{
  FreeChemical test;
  test.add (1);
  test.remove (1);
  BOOST_CHECK_EQUAL (test.number(), 0);
}

BOOST_AUTO_TEST_CASE (remove_AddTenRemoveOne_numberIsNine)
{
  FreeChemical test;
  test.add (10);
  test.remove (1);
  BOOST_CHECK_EQUAL (test.number(), 9);
}
 
BOOST_AUTO_TEST_SUITE_END()

class FreeChemicalThreeObserversFixture
{
public:
  FreeChemicalThreeObserversFixture (void)
    : observers (3)
    , _rate_validity (3)
  {
    for (int i = 0; i < observers.size(); ++i)
      { 
	observers [i] = new MockRateInvalidator (_rate_validity, i); 
	free_chemical.attach (*(observers[i]));
	BOOST_REQUIRE (observers[i]->was_updated() == false);
      }
  }
  
  ~FreeChemicalThreeObserversFixture (void)
  {
    for (int i = 0; i < observers.size(); ++i)
      { delete observers [i]; }
  }

  FreeChemical free_chemical;
  std::vector <MockRateInvalidator*> observers; 

private:
  RateValidity _rate_validity;
};

BOOST_FIXTURE_TEST_SUITE (ObserverTest, FreeChemicalThreeObserversFixture)

BOOST_AUTO_TEST_CASE (add_calledOnce_observersUpdated)
{
  free_chemical.add (1);
  for (int i = 0; i < 3; ++i)
    { BOOST_CHECK_EQUAL (observers[i]->was_updated(), true); }
}

BOOST_AUTO_TEST_CASE (remove_calledOnce_observersUpdated)
{
  free_chemical.add (1);
  for (int i = 0; i < 3; ++i) { observers[i]->reset_update(); }
  free_chemical.remove (1);
  for (int i = 0; i < 3; ++i)
    { BOOST_CHECK_EQUAL (observers[i]->was_updated(), true); }
}

BOOST_AUTO_TEST_SUITE_END()
