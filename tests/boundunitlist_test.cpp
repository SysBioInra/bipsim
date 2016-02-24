/**
 * @file boundunitlist_test.cpp
 * @brief Unit testing for BoundUnitList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BoundUnitList
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr
#include <list> // std::list


// ==================
//  Project Includes
// ==================
//
#include "../src/chemicalsequence.h"
#include "../src/bindingsite.h"
#include "../src/bindingsitefamily.h"
#include "../src/boundunitlist.h"
#include "../src/boundunit.h"

class EmptyListGenericBindingSite
{
public:
  EmptyListGenericBindingSite (void)
    : _cs (std::string (100, 'a'))
    , _bs (_bsf, _cs, 0, 0, 1, 1)
    , bound_unit_0 (_bs, 0, 0)
    , bound_unit_10 (_bs, 10, 10)
    , bound_unit_20 (_bs, 20, 20)
    , bound_unit_30 (_bs, 30, 30)
  {}
  
  bool check_content (std::list <BoundUnit*> ref)
  {
    for (int i = 0; i < empty_list.size(); ++i)
      {
	const BoundUnit* search = empty_list [i];
	// look for the corresponding element in ref
	for (std::list <BoundUnit*>::iterator bu_it = ref.begin();
	     bu_it != ref.end(); ++bu_it)
	  { if (search == *bu_it) { ref.erase (bu_it); break; } }
      }
    return (ref.size() == 0);
  }

private:
  ChemicalSequence _cs;
  BindingSiteFamily _bsf;
  BindingSite _bs;
  
public:
  BoundUnitList empty_list;
  BoundUnit bound_unit_0;
  BoundUnit bound_unit_10;
  BoundUnit bound_unit_20;
  BoundUnit bound_unit_30;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, EmptyListGenericBindingSite)
 
BOOST_AUTO_TEST_CASE (size_EmptyList_returnsZero)
{
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (size_listSizeOne_returnsOne)
{
  empty_list.insert (&bound_unit_0);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (size_listSizeTwo_returnsTwo)
{
  empty_list.insert (&bound_unit_0);
  empty_list.insert (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
}

BOOST_AUTO_TEST_CASE (bracketoperator_listSizeOne_returnsElementAdded)
{
  empty_list.insert (&bound_unit_0);
  BOOST_CHECK_EQUAL (empty_list [0], &bound_unit_0);
}

BOOST_AUTO_TEST_CASE (bracketoperator_listSizeTwo_containsElementAdded)
{
  std::list <BoundUnit*> ref;
  empty_list.insert (&bound_unit_0); ref.push_back (&bound_unit_0);
  empty_list.insert (&bound_unit_10); ref.push_back (&bound_unit_10);
  BOOST_CHECK (check_content (ref));
}

BOOST_AUTO_TEST_CASE (erase_listSizeTwo_erasesCorrectElement)
{
  empty_list.insert (&bound_unit_0);
  empty_list.insert (&bound_unit_10);
  empty_list.erase (&bound_unit_0);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list [0], &bound_unit_10);
}

BOOST_AUTO_TEST_CASE (erase_listSizeTwo_erasesCorrectElement2)
{
  empty_list.insert (&bound_unit_0);
  empty_list.insert (&bound_unit_10);
  empty_list.erase (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list [0], &bound_unit_0);
}

BOOST_AUTO_TEST_CASE (erase_listSizeOneEraseNonContainedElement_sizeRemainsConstant)
{
  empty_list.insert (&bound_unit_0);
  std::cout << "Warning expected: ";
  empty_list.erase (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (erase_listTwoCopiesSameElementMultipleErases_sizeContinuallyDecreases)
{
  empty_list.insert (&bound_unit_10);
  empty_list.insert (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
  empty_list.erase (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  empty_list.erase (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (erase_addFourElementsAndRemoveInRandomOrder_sizeIsZeroAtEnd)
{
  empty_list.insert (&bound_unit_0);
  empty_list.insert (&bound_unit_10);
  empty_list.insert (&bound_unit_20);
  empty_list.insert (&bound_unit_30);
  empty_list.erase (&bound_unit_20);
  empty_list.erase (&bound_unit_0);
  empty_list.erase (&bound_unit_30);
  empty_list.erase (&bound_unit_10);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}
 
BOOST_AUTO_TEST_SUITE_END()
