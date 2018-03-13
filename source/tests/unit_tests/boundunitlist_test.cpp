/**
 * @file vectorlist_test.cpp
 * @brief Unit testing for BoundUnitList class.
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
#include "chemicalsequence.h"
#include "boundunit.h"
#include "boundunitlist.h"

class BoundUnitListFixture
{
public:
  BoundUnitListFixture(void) : _dummy_seq("aaa") {}
  
  BoundUnit& create_unit (void)
  {
    return *(new BoundUnit (_dummy_seq, 0, 0, 0));
  }
  
  bool check_content (std::list <BoundUnit*> ref)
  {
    for (int i = 0; i < empty_list.size(); ++i)
      {
	BoundUnit* search = empty_list() [i];
	// look for the corresponding element in ref
	for (std::list <BoundUnit*>::iterator it = ref.begin(); it != ref.end(); ++it)
	  { if (search == *it) { ref.erase (it); break; } }
      }
    return (ref.size() == 0);
  }
  
private:
  ChemicalSequence _dummy_seq;

public:
  BoundUnitList empty_list;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, BoundUnitListFixture)
 
BOOST_AUTO_TEST_CASE (size_EmptyList_returnsZero)
{
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (size_listSizeOne_returnsOne)
{
  empty_list.add (create_unit());
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (size_listSizeTwo_returnsTwo)
{
  empty_list.add (create_unit());
  empty_list.add (create_unit());
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
}

BOOST_AUTO_TEST_CASE (random_unit_listSizeOne_returnsElementAdded)
{
  BoundUnit& unit1 = create_unit();
  empty_list.add (unit1);
  BOOST_CHECK_EQUAL (&empty_list.random_unit(), &unit1);
}

BOOST_AUTO_TEST_CASE (parenthesisoperator_listSizeTwo_containsElementAdded)
{
  std::list <BoundUnit*> ref;
  BoundUnit& unit1 = create_unit();
  BoundUnit& unit2 = create_unit();
  empty_list.add (unit1); ref.push_back (&unit1);
  empty_list.add (unit2); ref.push_back (&unit2);
  BOOST_CHECK (check_content (ref));
}

BOOST_AUTO_TEST_CASE (remove_listSizeTwo_removesCorrectElement)
{
  BoundUnit& unit1 = create_unit();
  BoundUnit& unit2 = create_unit();
  empty_list.add (unit1);
  empty_list.add (unit2);
  empty_list.remove (unit1);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list() [0], &unit2);
}

BOOST_AUTO_TEST_CASE (remove_listSizeTwo_removesCorrectElement2)
{
  BoundUnit& unit1 = create_unit();
  BoundUnit& unit2 = create_unit();
  empty_list.add (unit1);
  empty_list.add (unit2);
  empty_list.remove (unit2);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list() [0], &unit1);
}

BOOST_AUTO_TEST_CASE (remove_listSizeOneRemoveNonContainedElement_sizeRemainsConstant)
{
  BoundUnit& unit1 = create_unit();
  BoundUnit& unit2 = create_unit();
  empty_list.add (unit1);
  empty_list.remove (unit2);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (remove_listTwoCopiesSameElementMultipleRemoves_sizeContinuallyDecreases)
{
  BoundUnit& unit1 = create_unit();
  empty_list.add (unit1);
  empty_list.add (unit1);
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
  empty_list.remove (unit1);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  empty_list.remove (unit1);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (remove_addFourElementsAndRemoveInRandomOrder_sizeIsZeroAtEnd)
{
  BoundUnit& unit1 = create_unit();
  BoundUnit& unit2 = create_unit();
  BoundUnit& unit3 = create_unit();
  BoundUnit& unit4 = create_unit();
  empty_list.add (unit1);
  empty_list.add (unit2);
  empty_list.add (unit3);
  empty_list.add (unit4);
  empty_list.remove (unit3);
  empty_list.remove (unit1);
  empty_list.remove (unit4);
  empty_list.remove (unit2);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}
 
BOOST_AUTO_TEST_SUITE_END()
