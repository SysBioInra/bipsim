
/**
 * @file bindingsitefamily.cpp
 * @brief Implementation of the BindingSiteFamily class.
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
#include "bindingsitefamily.h"
#include "bindingsite.h"
#include "randomhandler.h"
#include "macros.h"
#include "ratevalidity.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteFamily::BindingSiteFamily (void) 
  : _rate_validity_size (10)
  , _rate_validity (0)
{
  _rate_validity = new RateValidity (_rate_validity_size);
}

// Forbidden
// BindingSiteFamily::BindingSiteFamily (const BindingSiteFamily& other_family);
// BindingSiteFamily& BindingSiteFamily::operator= ( const BindingSiteFamily& other_family );

BindingSiteFamily::~BindingSiteFamily (void)
{
  delete _rate_validity;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteFamily::add (BindingSite* site)
{
  // store binding site
  _binding_sites.push_back (site);

  // extend contribution vector
  _rate_validity->extend (1);
  _rate_contributions.extend (1);

  // configure update
  site->set_update_id (_rate_contributions.size()-1);
  site->location().watch_site (*site);
}

void BindingSiteFamily::update (int site_index)
{  
  /** @pre site_index must be within family range. */
  REQUIRE ((site_index >= 0) && (site_index < _rate_contributions.size()));

  _rate_validity->invalidate (site_index);
      
  // notify change to rate managers
  notify_change();
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool BindingSiteFamily::is_site_available (void) const
{
  update_rates();
  // we loop through sites and return as soon as we find that is available
  int number_sites = _binding_sites.size();
  for (int i = 0; i < number_sites; ++i)
   {
     if (_rate_contributions [i] > 0) return true;
   }

  // if we arrive here, it means that all sites are occupied
  return false;
}

bool BindingSiteFamily::contains (const BindingSite* site) const
{
  for (std::vector<BindingSite*>::const_iterator bs_it = _binding_sites.begin();
       bs_it != _binding_sites.end(); ++bs_it)
    { if (*bs_it == site) { return true; } }
  return false;
}

// =================
//  Private Methods
// =================
//
void BindingSiteFamily::update_rates (void) const
{
  while (!_rate_validity->empty())
    {
      int site_index = _rate_validity->front();
      _rate_contributions.set_rate 
	(site_index, _binding_sites [site_index]->binding_rate());
      _rate_validity->pop();
    }
}
