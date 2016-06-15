/**
 * @file vectorlist_test.cpp
 * @brief Unit testing for VectorList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE VectorList
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr
#include <list> // std::list


// ==================
//  Project Includes
// ==================
//
#include "vectorlist.h"

class IntVectorListFixture
{
public:
  bool check_content (std::list <int> ref)
  {
    for (int i = 0; i < empty_list.size(); ++i)
      {
	int search = empty_list() [i];
	// look for the corresponding element in ref
	for (std::list <int>::iterator it = ref.begin(); it != ref.end(); ++it)
	  { if (search == *it) { ref.erase (it); break; } }
      }
    return (ref.size() == 0);
  }
  
public:
  VectorList<int> empty_list;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, IntVectorListFixture)
 
BOOST_AUTO_TEST_CASE (size_EmptyList_returnsZero)
{
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (size_listSizeOne_returnsOne)
{
  empty_list.add (0);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (size_listSizeTwo_returnsTwo)
{
  empty_list.add (0);
  empty_list.add (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
}

BOOST_AUTO_TEST_CASE (random_unit_listSizeOne_returnsElementAdded)
{
  empty_list.add (0);
  BOOST_CHECK_EQUAL (empty_list.random_element(), 0);
}

BOOST_AUTO_TEST_CASE (parenthesisoperator_listSizeTwo_containsElementAdded)
{
  std::list <int> ref;
  empty_list.add (0); ref.push_back (0);
  empty_list.add (1); ref.push_back (1);
  BOOST_CHECK (check_content (ref));
}

BOOST_AUTO_TEST_CASE (remove_listSizeTwo_removesCorrectElement)
{
  empty_list.add (0);
  empty_list.add (1);
  empty_list.remove (0);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list() [0], 1);
}

BOOST_AUTO_TEST_CASE (remove_listSizeTwo_removesCorrectElement2)
{
  empty_list.add (0);
  empty_list.add (1);
  empty_list.remove (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  BOOST_CHECK_EQUAL (empty_list() [0], 0);
}

BOOST_AUTO_TEST_CASE (remove_listSizeOneRemoveNonContainedElement_sizeRemainsConstant)
{
  empty_list.add (0);
  empty_list.remove (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
}

BOOST_AUTO_TEST_CASE (remove_listTwoCopiesSameElementMultipleRemoves_sizeContinuallyDecreases)
{
  empty_list.add (1);
  empty_list.add (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 2);
  empty_list.remove (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 1);
  empty_list.remove (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}

BOOST_AUTO_TEST_CASE (remove_addFourElementsAndRemoveInRandomOrder_sizeIsZeroAtEnd)
{
  empty_list.add (0);
  empty_list.add (1);
  empty_list.add (2);
  empty_list.add (3);
  empty_list.remove (2);
  empty_list.remove (0);
  empty_list.remove (3);
  empty_list.remove (1);
  BOOST_CHECK_EQUAL (empty_list.size(), 0);
}
 
BOOST_AUTO_TEST_SUITE_END()
