

/**
 * @file bindingsitehandler.cpp
 * @brief Implementation of the BindingSiteHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <vector>  // std::vector

// ==================
//  Project Includes
// ==================
//
#include "bindingsitehandler.h"
#include "bindingsite.h"
#include "bindable.h"
#include "boundchemical.h"
#include "randomhandler.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteHandler::BindingSiteHandler (void)
{
}

BindingSiteHandler::BindingSiteHandler (BindingSiteHandler& other_binding_site_handler)
{
  // copy the identifier list
  _family_ids = other_binding_site_handler._family_ids;

  // copy the reference map
  copy_reference_map ( other_binding_site_handler );
}

BindingSiteHandler::~BindingSiteHandler (void)
{
  clear_binding_sites ();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteHandler::create_binding_site ( std::string family_name, Bindable& location, int position,
			     int length, double affinity )
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Site must fit on bindable element 
   *   (position+length <= location->length() ). */
  REQUIRE( position + length <= location.length() );

  // get family identifier
  if ( _family_ids.exists ( family_name ) == false )
    {
      _family_ids.create_id ( family_name );
    }
  int family_id = _family_ids.id ( family_name );

  // create binding site
  BindingSite* binding_site = new BindingSite;
  binding_site->set_family ( family_id );
  binding_site->set_physical_context ( location, position, length );
  binding_site->set_affinity ( affinity );
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a list is automatically created
  _references[ family_id ].push_back ( binding_site );
}

void BindingSiteHandler::add_unit_on_random_available_site ( int family_id, BoundChemical& unit_to_bind )
{
  // get the list of binding sites of the family
  ReferenceMap::iterator family = _references.find ( family_id );
  REQUIRE( family != _references.end() ); /** @pre family_id is already defined */
  BindingSiteList& family_reference_list = family->second;
  
  // get the number of available sites for each binding site and store them in an 
  // vector
  int number_binding_sites = family_reference_list.size();
  std::vector<int> number_available_sites ( number_binding_sites, 0 );
  
  BindingSiteList::iterator binding_site = family_reference_list.begin();
  for ( int index = 0; binding_site != family_reference_list.end(); binding_site++, index++ )
    {
      number_available_sites [ index ] = (*binding_site)->number_available_sites();
    }
  

  // draw a site according to the number of available sites
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_index ( number_available_sites );

  // retrieve the corresponding binding site
  binding_site = family_reference_list.begin();
  for ( int index = 0; index < index_drawn; index++ ) { binding_site++; }

  // add element and perform binding
  unit_to_bind.add( 1, *(*binding_site) );
  (*binding_site)->bind_unit( unit_to_bind );
}

void BindingSiteHandler::print ( void )
{
  for ( ReferenceMap::iterator family = _references.begin();
	family != _references.end(); family++ )
    {
      int family_id = family->first;
      std::string family_name = retrieve_name ( family_id );
      BindingSiteList& family_reference_list = family->second;
      for ( BindingSiteList::iterator binding_site = family_reference_list.begin();
	    binding_site != family_reference_list.end(); binding_site++)
	{
	  std::cout << "Binding site from family \"" << family_name
		    << "\" (id " << family_id
		    << ") number " << *binding_site
		    << " which has " << (*binding_site)->number_available_sites()
		    << " available sites." << std::endl;
	}
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
BindingSiteHandler& BindingSiteHandler::operator= (BindingSiteHandler& other_binding_site_handler)
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
}

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BindingSiteHandler::check_invariant ( void )
{
  bool result = true;
  /** There is at least one binding site reference per family */
  for ( ReferenceMap::iterator it = _references.begin(); it != _references.end(); it++ )
    {
      result = result && ( (it->second).size() > 0 );
    }
  return result;
}


// =================
//  Private Methods
// =================
//
void BindingSiteHandler::clear_binding_sites ( void )
{
  BindingSiteList* reference_list = 0;
  // we destroy all the binding sites
  for ( ReferenceMap::iterator family = _references.begin(); family != _references.end(); family++ )
    {
      // destroy all binding sites of the current family
      reference_list = &(family->second);
      for ( BindingSiteList::iterator binding_site = reference_list->begin();
	    binding_site != reference_list->end(); binding_site++ )
	{
	  delete *binding_site;
	}
      // clear the list
      reference_list->clear();
    }

}

void BindingSiteHandler::copy_reference_map ( BindingSiteHandler& other_binding_site_handler )
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;


  // we duplicate every binding site
  ReferenceMap& other_references = other_binding_site_handler._references;
  int family_id = 0;
  BindingSiteList* reference_list = 0;
  for ( ReferenceMap::iterator family = other_references.begin();
	family != other_references.end(); 
	family++ )
    {
      // copy all binding sites of the current family
      family_id = family->first;
      reference_list = &(family->second);
      for ( BindingSiteList::iterator binding_site = reference_list->begin ();
	    binding_site != reference_list->end (); 
	    binding_site++ )
	{
	  // CHECK THIS !!!! NOT FINISHED HERE !!!!
	  create_binding_site ( family_id, *(*binding_site) );
	}
    }
}

void BindingSiteHandler::create_binding_site ( int family_id,  BindingSite& binding_site )
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
}
