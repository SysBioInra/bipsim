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
    : table_ABCD (init_letters(), init_comp())
  {
  }

  template <typename Map>
  bool map_compare (Map const &lhs, Map const &rhs) {
    // No predicate needed because there is operator== for pairs already.
    return lhs.size() == rhs.size()
      && std::equal(lhs.begin(), lhs.end(),
		    rhs.begin());
  }

  FreeChemical A1, A2, BC1, B2, C2, C3, D1;
  CompositionTable table_ABCD;
  
private:
  std::vector <char> init_letters (void);
  std::vector <std::list <FreeChemical*> > init_comp (void);
};

std::vector <char> 
TableFourRecognizedLettersVaryingCompositions::init_letters (void)
{
  std::vector <char> letters (4);
  letters [0] = 'A'; letters [1] = 'B';  letters [2] = 'C'; letters [3] = 'D';
  return letters;
}

std::vector <std::list <FreeChemical*> > 
TableFourRecognizedLettersVaryingCompositions::init_comp (void)
{
  std::vector <std::list <FreeChemical*> > comp (4); 
  comp[0].push_back (&A1); comp[0].push_back (&A2);
  comp[1].push_back (&BC1); comp[1].push_back (&B2);
  comp[2].push_back (&BC1); comp[2].push_back (&C2); comp[2].push_back (&C3);
  comp[3].push_back (&D1);
  return comp;
}

BOOST_FIXTURE_TEST_SUITE (BaseTests, TableFourRecognizedLettersVaryingCompositions)
 
BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements)
{
  std::string sequence = "A"; std::map <FreeChemical*, int> expected_result;
  expected_result [&A1] = expected_result [&A2] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements2)
{
  std::string sequence = "B"; std::map <FreeChemical*, int> expected_result;
  expected_result [&BC1] = expected_result [&B2] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}
 
BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements3)
{
  std::string sequence = "C"; std::map <FreeChemical*, int> expected_result;
  expected_result [&BC1] = expected_result [&C2] = expected_result [&C3] = 1; 
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_singleLetterSequence_returnsBaseElements4)
{
  std::string sequence = "D"; std::map <FreeChemical*, int> expected_result;
  expected_result [&D1] = 1;
  BOOST_CHECK (map_compare (table_ABCD.composition (sequence), 
			    expected_result));
}

BOOST_AUTO_TEST_CASE (composition_fourLetterSequence_returnsEachBaseElement)
{
  std::string sequence = "DCBA"; std::map <FreeChemical*, int> expected_result;
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
