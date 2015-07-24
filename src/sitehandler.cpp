

/**
 * @file sitehandler.cpp
 * @brief Implementation of the SiteHandler class.
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
#include "sitehandler.h"
#include "site.h"
#include "chemicalsequence.h"
#include "macros.h"



// ==========================
//  Constructors/Destructors
// ==========================
//
SiteHandler::SiteHandler ( const SiteHandler& other_site_handler )
{
  // copy the identifier list
  _family_ids = other_site_handler._family_ids;

  // copy the reference map
  copy_family_map ( other_site_handler );
}

SiteHandler::~SiteHandler (void)
{
  clear_sites ();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SiteHandler::create_site ( std::string family_name, ChemicalSequence& location,
				int position, int length )
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Site must fit on chemical sequence 
   *   (position+length <= location->length() ). */
  REQUIRE( position + length <= location.length() );

  // get family identifier
  if ( _family_ids.exists ( family_name ) == false )
    {
      _family_ids.create_id ( family_name );
    }
  int family_id = _family_ids.id ( family_name );

  // create site
  Site* site = new Site ( family_id, location, position, length );
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a list is automatically created
  _families[ family_id ].push_back ( site );
}

void SiteHandler::print ( void ) const
{
  for ( SiteFamilyMap::const_iterator family = _families.begin();
	family != _families.end(); family++ )
    {
      int family_id = family->first;
      std::string family_name = retrieve_name ( family_id );
      const SiteList& family_site_list = family->second;
      for ( SiteList::const_iterator site = family_site_list.begin();
	    site != family_site_list.end(); site++)
	{
	  std::cout << "Site from family \"" << family_name
		    << "\" (id " << family_id
		    << ") number " << *site
		    << " which has " << (*site)->number_available_sites()
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
SiteHandler& SiteHandler::operator= (SiteHandler& other_site_handler)
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
bool SiteHandler::check_invariant ( void ) const
{
  bool result = true;
  /** There is at least one site reference per family */
  for ( SiteFamilyMap::const_iterator it = _families.begin(); it != _families.end(); it++ )
    {
      result = result && ( (it->second).size() > 0 );
    }
  return result;
}


// =================
//  Private Methods
// =================
//
void SiteHandler::clear_sites ( void )
{
  SiteList* reference_list = 0;
  // we destroy all the sites
  for ( SiteFamilyMap::iterator family = _families.begin(); family != _families.end(); family++ )
    {
      // destroy all sites of the current family
      reference_list = &(family->second);
      for ( SiteList::iterator site = reference_list->begin();
	    site != reference_list->end(); site++ )
	{
	  delete *site;
	}
      // clear the list
      reference_list->clear();
    }

}

void SiteHandler::copy_family_map ( const SiteHandler& other_site_handler )
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;


  // we duplicate every site
  const SiteFamilyMap& other_families = other_site_handler._families;
  int family_id = 0;
  const SiteList* site_list = 0;
  for ( SiteFamilyMap::const_iterator family = other_families.begin();
	family != other_families.end(); 
	family++ )
    {
      // copy all sites of the current family
      family_id = family->first;
      site_list = &(family->second);
      for ( SiteList::const_iterator site = site_list->begin ();
	    site != site_list->end (); 
	    site++ )
	{
	  // CHECK THIS !!!! NOT FINISHED HERE !!!!
	  create_site ( family_id, *(*site) );
	}
    }
}

void SiteHandler::create_site ( int family_id,  const Site& site )
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
}
