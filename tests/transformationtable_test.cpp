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
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/transformationtable.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

bool transformation_test (TransformationTable& table, const std::string& input, const std::string& expected_result);

int main (int argc, char *argv[])
{ 
  // test with DNA to RNA transformation
  TransformationTable dna2rna (1);
  dna2rna.add_rule ("A", "U");
  dna2rna.add_rule ("C", "G");
  dna2rna.add_rule ("G", "C");
  dna2rna.add_rule ("T", "A");

  if (!transformation_test (dna2rna, "ACGTACGTACGT", "UGCAUGCAUGCA"))
    { FAILURE ("could not transform simple sequence successfully..."); }

  if (!transformation_test (dna2rna, "ACVGTACGTACGT", ""))
    { FAILURE ("could not transform simple sequence successfully..."); }

  // test with more complex transformation
  TransformationTable table (3);
  table.add_rule ("AAA", "1");
  table.add_rule ("BBB", "22");
  table.add_rule ("CCC", "333");
  table.add_rule ("DDD", "4444");

  if (!transformation_test (table, "AAABB", ""))
    { FAILURE ("could not transform simple sequence successfully..."); }

  if (!transformation_test (table, "DDDCCCBBBAAA", "4444333221"))
    { FAILURE ("could not transform simple sequence successfully..."); }

  if (!transformation_test (table, "", ""))
    { FAILURE ("could not transform simple sequence successfully..."); }
  

  return EXIT_SUCCESS;
}

bool transformation_test (TransformationTable& table, const std::string& input, const std::string& expected_result)
{
  if (table.transform (input) == expected_result) return true;
  else return false;
}
