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
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/producttable.h"
#include "../src/chemicalsequence.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}


int main (int argc, char *argv[])
{ 
  TransformationTable dummy (1);
  dummy.add_rule ("A","a");
  dummy.add_rule ("B","b");
  dummy.add_rule ("C","c");
  dummy.add_rule ("m","x");
  dummy.add_rule ("i","o");

  ProductTable table (dummy);
  ChemicalSequence a ("A"), b ("B"), c("C");
  ChemicalSequence parent ("mimimi");
  
  table.add (parent, 1, 10, a);
  table.add (parent, 150, 200, b);
  table.add (parent, 9, 17, c);
  if (table.product (parent, 1, 10) != &a)
    { FAILURE ("could not find product in table."); }
  if (table.product (parent, 9, 17) != &c)
    { FAILURE ("could not find product in table."); }
  if (table.product (parent, 150, 200) != &b)
    { FAILURE ("could not find product in table."); }
  if (table.product (parent, 15, 18) != 0)
    { FAILURE ("found non-existing product in table."); }

  table.add (parent, 1, 10, c);
  if (table.product (parent, 1, 10) != &c) { FAILURE ("rewrite failed."); }  

  if (table.generate_child_sequence (a, 1, 1) != "a")
    { FAILURE ("Conversion to child failed."); }  
  if (table.generate_child_sequence (b, 1, 1) != "b")
    { FAILURE ("Conversion to child failed."); }  
  if (table.generate_child_sequence (c, 1, 1) != "c")
    { FAILURE ("Conversion to child failed."); }  
  if (table.generate_child_sequence (parent, 1, 6) != "xoxoxo")
    { FAILURE ("Conversion to child failed."); }  

  return EXIT_SUCCESS;
}
