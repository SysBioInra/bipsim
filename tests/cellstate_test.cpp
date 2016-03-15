/**
 * @file cellstate_test.cpp
 * @brief Unit testing for CellState class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CellState
#include <boost/test/unit_test.hpp>

// ==================
//  Project Includes
// ==================
//
#include "../src/producttable.h"
#include "../src/chemicalsequence.h"
#include "../src/cellstate.h"

class BaseElements
{
public:
  BaseElements (void)
    : chemical_sequence_1 (new ChemicalSequence ("AAA"))
    , chemical_sequence_2 (new ChemicalSequence ("BBB"))
  {
    std::vector <std::string> input, output;
    input.push_back ("A"); output.push_back ("a");
    transformation_table = new TransformationTable (input, output);
    product_table = new ProductTable (*transformation_table);
  }

  TransformationTable* transformation_table;
  ProductTable* product_table;
  ChemicalSequence* chemical_sequence_1;
  ChemicalSequence* chemical_sequence_2;
  CellState empty_cell_state;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, BaseElements)
 
BOOST_AUTO_TEST_CASE (store_find_storeTransformationTable_findsStoredElement)
{
  empty_cell_state.store (transformation_table, "tt");
  BOOST_CHECK_EQUAL (empty_cell_state.find <TransformationTable> ("tt"),
		     transformation_table);
}

BOOST_AUTO_TEST_CASE (store_find_storeProductTable_findsStoredElement)
{
  empty_cell_state.store (product_table, "pt");
  BOOST_CHECK_EQUAL (empty_cell_state.find <ProductTable> ("pt"),
		     product_table);
}

BOOST_AUTO_TEST_CASE (store_find_storeChemicalSequence_findsStoredElement)
{
  empty_cell_state.store (chemical_sequence_1, "cs");
  BOOST_CHECK_EQUAL (empty_cell_state.find <ChemicalSequence> ("cs"),
		     chemical_sequence_1);
}

BOOST_AUTO_TEST_CASE (store_find_storeMultipleElements_findsStoredElements)
{
  empty_cell_state.store (product_table, "pt");
  empty_cell_state.store (chemical_sequence_1, "cs1");
  empty_cell_state.store (chemical_sequence_2, "cs2");
  BOOST_CHECK_EQUAL (empty_cell_state.find <ChemicalSequence> ("cs1"),
		     chemical_sequence_1);
  BOOST_CHECK_EQUAL (empty_cell_state.find <ProductTable> ("pt"),
		     product_table);
  BOOST_CHECK_EQUAL (empty_cell_state.find <ChemicalSequence> ("cs2"),
		     chemical_sequence_2);
}
 
BOOST_AUTO_TEST_SUITE_END()
  
