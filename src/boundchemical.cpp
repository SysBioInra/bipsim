

/**
 * @file boundchemical.cpp
 * @brief Implementation of the BoundChemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "boundchemical.h"
#include "bindingsite.h"
#include "randomhandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BoundChemical::BoundChemical (void)
{
}

// Not needed for this class (use of default copy constructor) !
// BoundChemical::BoundChemical (BoundChemical& other_bound_chemical);

BoundChemical::~BoundChemical (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BoundChemical::handle_collision (void)
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
}

void BoundChemical::add ( int quantity, BindingSite& binding_site )
{
  REQUIRE( quantity > 0 ); /** @pre quantity must be positive. */

  Chemical::add ( quantity );
  for ( int i = 0; i < quantity; i++ )
    {
      _binding_sites[ binding_site.family() ].push_back ( &binding_site );
    }
}

void BoundChemical::unbind_random ( int family_id )
{
  // get the list of binding sites of the family to which the chemical is bound
  ReferenceMap::iterator family = _binding_sites.find ( family_id );
  REQUIRE( family != _binding_sites.end() ); /** @pre family_id is already defined */
  BindingSiteList& family_reference_list = family->second;
  
  // draw a random chemical to unbind
  int number_binding_sites = family_reference_list.size();
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_uniform ( 0, number_binding_sites-1 );

  // retrieve the corresponding binding site
  BindingSiteList::iterator binding_site = family_reference_list.begin();
  for ( int index = 0; index < index_drawn; index++ )
    { 
      binding_site++; 
    }

  // perform unbinding
  (*binding_site)->unbind_unit( *this );

  // remove chemical
  family_reference_list.erase ( binding_site );
  Chemical::remove ( 1 );
  // if there are no more binding sites belonging to the family
  if ( number_binding_sites - 1 == 0 ) 
    {
      // erase the empty family list from the map
      _binding_sites.erase ( family );
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// BoundChemical& BoundChemical::operator= (BoundChemical& other_bound_chemical);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BoundChemical::check_invariant (void)
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
