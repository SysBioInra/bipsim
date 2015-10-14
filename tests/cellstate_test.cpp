/**
 * @file cellstate_test.cpp
 * @brief Unit testing for ProductTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/producttable.h"
#include "../src/chemicalsequence.h"
#include "../src/cellstate.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}


int main (int argc, char *argv[])
{ 
  CellState cell_state;

  TransformationTable* tt = new TransformationTable (1);
  tt->add_rule ("A","a"); tt->add_rule ("B","b"); tt->add_rule ("C","c");
  cell_state.store (tt,"tt");
  if (cell_state.find <TransformationTable> ("tt") != tt)
    { FAILURE ("Storage of transformation table failed."); }

  ProductTable* pt = new ProductTable (*tt);
  cell_state.store (pt,"pt");
  if (cell_state.find <ProductTable> ("pt") != pt)
    { FAILURE ("Storage of product table failed."); }
  
  ChemicalSequence* parent = new ChemicalSequence ("ABABAB"),
    *a = new ChemicalSequence ("a");  
  cell_state.store (parent,"parent");
  cell_state.store (a,"a");
  ChemicalSequence* parent_bis = cell_state.find <ChemicalSequence> ("parent");
  if (parent_bis != parent)
    { FAILURE ("Storage of chemical sequence failed."); }
  ChemicalSequence* a_bis = cell_state.find <ChemicalSequence> ("a");
  if (a_bis != a)
    { FAILURE ("Storage of chemical sequence failed."); }
  ProductTable* pt_bis = cell_state.find <ProductTable> ("pt");
  if (pt_bis != pt)
    { FAILURE ("Storage of product table failed."); }
  pt->add (*parent, 1, 1, *a);

  return EXIT_SUCCESS;
}
