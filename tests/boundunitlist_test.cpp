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
#include <list> // std::list

#include "../src/chemicalsequence.h"
#include "../src/bindingsite.h"
#include "../src/bindingsitefamily.h"
#include "../src/boundunitlist.h"
#include "../src/boundunit.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

bool check_content (const BoundUnitList& ulist, 
		    std::list <BoundUnit*> ref);

int main (int argc, char *argv[])
{ 
  ChemicalSequence cs ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  BindingSiteFamily dummy_family;
  BindingSite bs (dummy_family, cs, 1, 1, 1, 1);
  BoundUnitList ulist;
  std::list <BoundUnit*> ref;

  // test initial size
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }

  // test adding one element
  BoundUnit bu (bs, bs.first(), bs.reading_frame());
  ulist.insert (&bu);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  ref.push_back (&bu);
  if (!check_content (ulist, ref))
    { FAILURE ("First element of list not correctly initialized."); }

  // test adding a second element
  BoundUnit bu2 (bs, bs.first()+10, bs.reading_frame());
  ulist.insert (&bu2);
  if (ulist.size() != 2) { FAILURE ("Wrong number of elements."); }
  ref.push_back (&bu2);
  if (!check_content (ulist, ref))
    { FAILURE ("Second element of list not correctly initialized."); }
  
  // test erasing an element from position
  ref.remove (ulist [1]);
  ulist.erase (ulist [1]);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  if (!check_content (ulist, ref)) { FAILURE ("Wrong element erased."); }

  // test erasing an element from a stored pointer
  ulist.insert (&bu2);
  ulist.erase (&bu2);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  if (!check_content (ulist, ref)) { FAILURE ("Wrong element erased."); }

  // test erasing a non existing element
  ulist.erase (ulist [0]);
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }
  ulist.insert (&bu2);
  std::cout << "Warning expected: ";
  ulist.erase (&bu);
  if (ulist.size() != 1)
    { FAILURE ("Element was erased when it should not."); }
  
  // test storing multiple copies and deleting them
  ulist.insert (&bu2);
  if (ulist.size() != 2) { FAILURE ("Wrong number of elements."); }
  ulist.erase (&bu2);
  if (ulist.size() != 1) { FAILURE ("Wrong number of elements."); }
  ulist.erase (&bu2);
  if (ulist.size() != 0) { FAILURE ("Wrong number of elements."); }

  // add several elements and delete them
  BoundUnit bu3 (bs, bs.first(), bs.reading_frame());
  BoundUnit bu4 (bs, bs.first(), bs.reading_frame());
  ulist.insert (&bu);
  ulist.insert (&bu2);
  ulist.insert (&bu3);
  ulist.insert (&bu4);
  ulist.erase (&bu3);
  ulist.erase (&bu);
  ulist.erase (&bu4);
  ulist.erase (&bu2);

  return EXIT_SUCCESS;
}

bool check_content (const BoundUnitList& ulist, 
		    std::list <BoundUnit*> ref)
{
  for (int i = 0; i < ulist.size(); ++i)
    {
      const BoundUnit* search = ulist [i];
      // look for the corresponding element in ref
      for (std::list <BoundUnit*>::iterator bu_it = ref.begin();
	   bu_it != ref.end(); ++bu_it)
	{
	  if (search == *bu_it) { ref.erase (bu_it); break; }
	}
    }

  return (ref.size() == 0);
}
