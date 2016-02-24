/**
 * @file transformationtable_test.cpp
 * @brief Unit testing for TransformationTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TransformationTable
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "../src/transformationtable.h"

class TTDNA2RNA
{
public:
  TTDNA2RNA (void)
    : table_dna2rna (1)
  {
    table_dna2rna.add_rule ("A", "U");
    table_dna2rna.add_rule ("C", "G");
    table_dna2rna.add_rule ("G", "C");
    table_dna2rna.add_rule ("T", "A");
  }

  TransformationTable table_dna2rna;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, TTDNA2RNA)
 
BOOST_AUTO_TEST_CASE (transform_emptyString_returnsEmptyString)
{
  BOOST_CHECK_EQUAL (table_dna2rna.transform (""), "");
}

BOOST_AUTO_TEST_CASE (transform_simpleValidSequence_returnsCorrectTransformation)
{
  std::string input = "ACGTACGTACGT";
  std::string expected = "UGCAUGCAUGCA";
  BOOST_CHECK_EQUAL (table_dna2rna.transform (input), expected);
}

BOOST_AUTO_TEST_CASE (transform_simpleInvalidSequence_returnsEmptyString)
{
  std::string input = "ACVGTACGTACGT";
  std::string expected = "";
  BOOST_CHECK_EQUAL (table_dna2rna.transform (input), expected);
}
 
BOOST_AUTO_TEST_SUITE_END()

class ComplexTT
{
public:
  ComplexTT (void)
    : table_motif_L3 (3)
  {
    table_motif_L3.add_rule ("AAA", "1");
    table_motif_L3.add_rule ("BBB", "22");
    table_motif_L3.add_rule ("CCC", "333");
    table_motif_L3.add_rule ("DDD", "4444");
  }

  TransformationTable table_motif_L3;
};

BOOST_FIXTURE_TEST_SUITE (BaseTestsMotifLength3, ComplexTT)

BOOST_AUTO_TEST_CASE (transform_invalidSequenceLength_returnsEmptyString)
{
  BOOST_CHECK_EQUAL (table_motif_L3.transform ("AAABB"), "");
}

BOOST_AUTO_TEST_CASE (transform_simpleValidSequence_returnsCorrectTransformation2)
{
  BOOST_CHECK_EQUAL (table_motif_L3.transform ("DDDCCCBBBAAA"), 
		     "4444333221");
}

BOOST_AUTO_TEST_SUITE_END()
