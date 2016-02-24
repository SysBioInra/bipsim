/**
 * @file compositiontable_test.cpp
 * @brief Unit testing for CompositionTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CompositionTable
#include <boost/test/unit_test.hpp>

#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "../src/compositiontable.h"
#include "../src/freechemical.h"

class TableFourRecognizedLettersVaryingCompositions
{
public:
  TableFourRecognizedLettersVaryingCompositions (void)
  {
    std::list <Chemical*> comp; comp.push_back(&A1); comp.push_back(&A2);
    table_ABCD.add_rule ('A', comp);
    comp.clear(); comp.push_back(&BC1); comp.push_back(&B2);
    table_ABCD.add_rule ('B', comp);
    comp.clear(); comp.push_back(&BC1); comp.push_back(&C2); 
    comp.push_back(&C3);
    table_ABCD.add_rule ('C', comp);
    comp.clear(); comp.push_back(&D1);
    table_ABCD.add_rule ('D', comp);
  }

  template <typename Map>
  bool map_compare (Map const &lhs, Map const &rhs) {
    // No predicate needed because there is operator== for pairs already.
    return lhs.size() == rhs.size()
      && std::equal(lhs.begin(), lhs.end(),
		    rhs.begin());
  }

public:
  FreeChemical A1, A2, BC1, B2, C2, C3, D1;
  CompositionTable table_ABCD;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, TableFourRecognizedLettersVaryingCompositions)
 
BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements)
{
  std::string sequence = "A"; std::map <Chemical*, int> expected_result;
  expected_result [&A1] = expected_result [&A2] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements2)
{
  std::string sequence = "B"; std::map <Chemical*, int> expected_result;
  expected_result [&BC1] = expected_result [&B2] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}
 
BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements3)
{
  std::string sequence = "C"; std::map <Chemical*, int> expected_result;
  expected_result [&BC1] = expected_result [&C2] = expected_result [&C3] = 1; 
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements4)
{
  std::string sequence = "D"; std::map <Chemical*, int> expected_result;
  expected_result [&D1] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_fourLetterSequence_returnsEachBaseElement)
{
  std::string sequence = "DCBA"; std::map <Chemical*, int> expected_result;
  expected_result [&A1] = expected_result [&A2] = expected_result [&B2] = 1;
  expected_result [&BC1] = 2;
  expected_result [&C2] = expected_result [&C3] = expected_result [&D1] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_twoLongSequencesSameLetters_returnsSameComposition)
{
  std::string sequence1 = "AAAABBBCCDCCBBBAAAA";
  std::string sequence2 = "DCCCCBBBBBBAAAAAAAA";
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence1), 
			    table_ABCD.composition (sequence2)));
}

BOOST_AUTO_TEST_SUITE_END()
