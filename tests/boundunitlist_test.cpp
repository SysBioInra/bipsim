/**
 * @file boundunitlist_test.cpp
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

#include "../src/chemicalsequence.h"
#include "../src/bindingsite.h"
#include "../src/boundunitlist.h"
#include "../src/boundunit.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}


int main (int argc, char *argv[])
{ 
  ChemicalSequence cs ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  BindingSite bs (0, cs, 1, 1, 1, 1);
  BoundUnitList ulist;

  // test initial size
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }

  // test adding one element
  BoundUnit bu (bs, bs.first(), bs.reading_frame());
  ulist.push_front (&bu);
  if (ulist [0] != &bu)
    { FAILURE ("First element of list not correctly initialized."); }
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }

  // test adding a second element
  BoundUnit bu2 (bs, bs.first()+10, bs.reading_frame());
  ulist.push_front (&bu2);
  if (ulist [0] != &bu2)
    { FAILURE ("First element of list not correctly initialized."); }
  if (ulist.size() != 2) { FAILURE ("Wrong number of elements."); }
  
  // test erasing an element from position
  ulist.erase (ulist [1]);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  if (ulist [0] != &bu2) { FAILURE ("Wrong element erased."); }

  // test erasing an element from a stored pointer
  ulist.push_front (&bu);
  ulist.erase (&bu2);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  if (ulist [0] != &bu) { FAILURE ("Wrong element erased."); }

  // test erasing first element of the list
  ulist.push_front (&bu2);
  ulist [1]; // change internal pointer to last element accessed
  ulist.erase (&bu2);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  if (ulist [0] != &bu) { FAILURE ("Wrong element erased."); }
  
  // test erasing a non existing element
  ulist.erase (&bu2);
  if (ulist.size() != 1) { FAILURE ("Element was erased when it should not."); }
  
  // test storing multiple copies and deleting them
  ulist.push_front (&bu);
  if (ulist.size() != 2) { FAILURE ("Wrong number of elements."); }
  ulist.erase (&bu);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  ulist.erase (&bu);
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }
  ulist.erase (&bu);
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }

  return EXIT_SUCCESS;
}
