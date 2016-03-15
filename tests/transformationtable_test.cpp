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

#include <vector>
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "../src/transformationtable.h"

class TTDNA2RNA
{
public:
  TTDNA2RNA (void)
    : table_dna2rna (input_motifs, output_motifs)
  {
  }

  TransformationTable table_dna2rna;
  
private:
  static std::vector <std::string> input_motifs;
  static std::vector <std::string> output_motifs;
};

std::vector <std::string> init_input (void)
{
  std::vector <std::string> result;
  result.push_back ("A"); result.push_back ("C"); 
  result.push_back ("G"); result.push_back ("T"); 
  return result;
}
std::vector <std::string> TTDNA2RNA::input_motifs (init_input()); 

std::vector <std::string> init_output (void)
{
  std::vector <std::string> result;
  result.push_back ("U"); result.push_back ("G");
  result.push_back ("C"); result.push_back ("A");
  return result;
}
std::vector <std::string> TTDNA2RNA::output_motifs (init_output());

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
    : table_motif_L3 (input_motifs, output_motifs)
  {
  }

  TransformationTable table_motif_L3;

private:
  static std::vector <std::string> input_motifs;
  static std::vector <std::string> output_motifs;
};

std::vector <std::string> init_input_2 (void)
{
  std::vector <std::string> result;
  result.push_back ("AAA"); result.push_back ("BBB"); 
  result.push_back ("CCC"); result.push_back ("DDD"); 
  return result;
}
std::vector <std::string> ComplexTT::input_motifs (init_input_2()); 

std::vector <std::string> init_output_2 (void)
{
  std::vector <std::string> result;
  result.push_back ("1"); result.push_back ("22");
  result.push_back ("333"); result.push_back ("4444");
  return result;
}
std::vector <std::string> ComplexTT::output_motifs (init_output_2());

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
