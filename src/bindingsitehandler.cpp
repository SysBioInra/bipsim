

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
#include "chemicalsequence.h"
#include "randomhandler.h"
#include "macros.h"



// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteHandler::BindingSiteHandler (void)
{
}

BindingSiteHandler::~BindingSiteHandler (void)
{
  clear_sites();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteHandler::create_site (std::string family_name, ChemicalSequence& location, int position,
				      int length, double k_on, double k_off,
				      int reading_frame /*= NO_READING_FRAME*/)
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Site must fit on bindable element 
   *   (position+length <= location->length() ). */
  REQUIRE( position + length <= location.length() );

  // get family identifier
  int family_id = get_or_create_family_identifier (family_name);

  // create binding site
  BindingSite* binding_site = new BindingSite (family_id, location, position, length, k_on, k_off, reading_frame);
  _binding_site_list.push_back (binding_site);
  
  // add reference to the corresponding family list
  // if family_id is not in the map, a family is automatically created
  _families[ family_id ].add_binding_site ( binding_site );
}

void BindingSiteHandler::update_all_binding_rate_contributions (void)
{
  // we simply call the update rate function for every family in the map
  std::map<int,BindingSiteFamily>::iterator family = _families.begin();
  while (family != _families.end())
    {
      (family->second).update_rate_contributions();
      ++family;
    }
}

void BindingSiteHandler::update_binding_rate_contributions (const ChemicalSequence& modified_sequence)
{
  std::list<BindingSite*>& modified_binding_sites = _location_map [&modified_sequence];

  for (std::list<BindingSite*>::iterator site = modified_binding_sites.begin();
       site != modified_binding_sites.end(); ++site)
    {
      _families [(*site)->family()].update_rate_contribution(*site);
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
void BindingSiteHandler::print (std::ostream& output) const
{
}

void BindingSiteHandler::clear_sites (void)
{
  std::list<BindingSite*>::iterator site = _binding_site_list.begin();
  while (site != _binding_site_list.end())
    {
      delete *site;
      ++site;
    }
}
