

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
#include "randomhandler.h"
#include "macros.h"



// ==========================
//  Constructors/Destructors
// ==========================
//

BindingSiteHandler::~BindingSiteHandler (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteHandler::create_site ( std::string family_name, Bindable& location, int position,
				       int length, double k_on, double k_off )
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
  BindingSite* binding_site = new BindingSite ( family_id, location, position, length, k_on, k_off );
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a list is automatically created
  _families[ family_id ].push_back ( binding_site );
}


// ============================
//  Public Methods - Accessors
// ============================
//

const BindingSite& BindingSiteHandler::get_random_available_site ( int family_id ) const
{
  // get the list of binding sites of the family
  SiteFamilyMap::const_iterator family = _families.find ( family_id );
  REQUIRE( family != _families.end() ); /** @pre family_id is already defined */
  const SiteList& family_site_list = family->second;
  
  // get the number of available sites for each binding site and store them in an 
  // vector
  int number_binding_sites = family_site_list.size();
  std::vector<int> number_available_sites ( number_binding_sites, 0 );
  
  SiteList::const_iterator site = family_site_list.begin();
  for ( int index = 0; site != family_site_list.end(); site++, index++ )
    {
      number_available_sites [ index ] = (*site)->number_available_sites();
    }

  // draw a site according to the number of available sites
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_index ( number_available_sites );

  // retrieve the corresponding binding site
  site = family_site_list.begin();
  for ( int index = 0; index < index_drawn; index++ ) { site++; }

  return *( dynamic_cast< const BindingSite* > ( *site ) );
}


double BindingSiteHandler::get_total_binding_rate_contribution ( int family_id ) const
{
  /**
   * Binding rate is generally defined by r = k_on x [A] x [B], where [A] is the concentration
   * of units to bind and [B] the concentration of binding sites. However, k_on varies from a
   * site to another so the total binding rate becomes
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * Here we need to compute the second part of the binding rate.
   */

  // retrieve binding site family
  SiteFamilyMap::const_iterator family = _families.find ( family_id );
  REQUIRE( family != _families.end() ); /** @pre family_id is already defined */
  const SiteList& family_site_list = family->second;
  
  // get the number of available sites and k_on for each binding site
  double rate_contribution = 0;
  SiteList::const_iterator site = family_site_list.begin();
  for ( int index = 0; site != family_site_list.end(); site++, index++ )
    {
      const BindingSite& binding_site = *dynamic_cast< const BindingSite* > (*site);
      rate_contribution += binding_site.k_on() * binding_site.number_available_sites();
    }

}

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
bool BindingSiteHandler::check_invariant ( void ) const
{
  bool result = SiteHandler::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
