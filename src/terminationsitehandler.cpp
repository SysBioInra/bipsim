

/**
 * @file terminationsitehandler.cpp
 * @brief Implementation of the TerminationSiteHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "terminationsitehandler.h"
#include "macros.h"
#include "site.h"
#include "chemicalsequence.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
TerminationSiteHandler::~TerminationSiteHandler (void)
{
  clear_sites();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void TerminationSiteHandler::create_site ( std::string family_name, ChemicalSequence& location,
					   int position, int length )
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Site must fit on bindable element 
   *   (position+length <= location->length() ). */
  REQUIRE( position + length <= location.length() );

  int family_id = get_or_create_family_identifier ( family_name );

  // create site
  Site* site = new Site ( family_id, location, position, length );
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a list is automatically created
  _families[ family_id ].push_back ( site );

  // add the termination site on the bindable element
  location.add_termination_site ( *site );
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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool TerminationSiteHandler::check_invariant ( void ) const
{
  bool result = SiteHandler::check_invariant();
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
void TerminationSiteHandler::clear_sites ( void )
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

void TerminationSiteHandler::print (std::ostream& output) const
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
	  output << "Termination site from family \"" << family_name
		 << "\" (id " << family_id
		 << ") number " << *site << std::endl;
	}
    }
}

