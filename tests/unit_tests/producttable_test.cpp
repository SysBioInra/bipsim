/**
 * @file producttable_test.cpp
 * @brief Unit testing for ProductTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ProductTable
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "producttable.h"
#include "chemicalsequence.h"

class AaTTFactory
{
public:
  static TransformationTable* make (void)
  {
    std::vector <std::string> input, output;
    input.push_back ("A"); output.push_back ("a");
    input.push_back ("B"); output.push_back ("b");
    input.push_back ("C"); output.push_back ("c");
    return new TransformationTable (input, output);
  }
};

class ABCStringFactory
{
public:
  static std::string make (int number)
  {
    std::string result;
    for (int i = 0; i < number; ++i) { result += "ABC"; }
    return result;
  }
};

class ParentL60
{
public:
  ParentL60 (void)
    : parent_ABC (ABCStringFactory::make (20))
    , _tt (AaTTFactory::make())
    , table_Aa_Bb_Cc (*_tt)
  {
  }
  
  ~ParentL60 (void) { delete _tt; }

private:
  TransformationTable* _tt;

public:
  ChemicalSequence parent_ABC;
  ProductTable table_Aa_Bb_Cc;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, ParentL60)
 
BOOST_AUTO_TEST_CASE (product_noProduct_returnsNullPointer)
{
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 1, 10),
		     (ChemicalSequence*) 0);
}

BOOST_AUTO_TEST_CASE (product_oneProduct_returnsAddedProduct)
{
  ChemicalSequence cs (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 1, 10, cs);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 1, 10), &cs);
}

BOOST_AUTO_TEST_CASE (product_oneProductWrongBasesSpecified_returnsNullPointer)
{
  ChemicalSequence cs (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 1, 10, cs);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 1, 15), 
		     (ChemicalSequence*) 0);
}

BOOST_AUTO_TEST_CASE (product_threeProducts_returnsCorrectProduct)
{
  ChemicalSequence cs (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 1, 10, cs);
  ChemicalSequence cs2 (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 11, 20, cs2);
  ChemicalSequence cs3 (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 11, 30, cs3);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 11, 20), &cs2);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 11, 30), &cs3);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 1, 10), &cs);
}

BOOST_AUTO_TEST_CASE (product_oneProductOverwritten_returnsSecondProduct)
{
  ChemicalSequence cs (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 1, 10, cs);
  ChemicalSequence cs2 (std::string (10, 'a')); 
  table_Aa_Bb_Cc.add (parent_ABC, 1, 10, cs2);
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc.product (parent_ABC, 1, 10), &cs2);
}

BOOST_AUTO_TEST_CASE (generate_child_sequence_oneBase_returnsCorrectString)
{
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc. generate_child_sequence (parent_ABC, 0, 0),
		     "a");
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc. generate_child_sequence (parent_ABC, 1, 1),
		     "b");
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc. generate_child_sequence (parent_ABC, 2, 2),
		     "c");
}

BOOST_AUTO_TEST_CASE (generate_child_sequence_severalBases_returnsCorrectString)
{
  BOOST_CHECK_EQUAL (table_Aa_Bb_Cc. generate_child_sequence (parent_ABC, 10, 20), 
		     "bcabcabcabc");
}

BOOST_AUTO_TEST_SUITE_END()
