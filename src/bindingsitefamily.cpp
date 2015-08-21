

/**
 * @file bindingsitefamily.cpp
 * @brief Implementation of the BindingSiteFamily class.
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
#include "bindingsitefamily.h"
#include "bindingsiteobserver.h"
#include "randomhandler.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteFamily::BindingSiteFamily (void)
 : _total_rate_contribution (0)
{
}

// Not needed for this class (use of default copy constructor) !
// BindingSiteFamily::BindingSiteFamily ( const BindingSiteFamily& other_binding_site_family );

BindingSiteFamily::~BindingSiteFamily (void)
{
  for (std::list<BindingSiteObserver*>::iterator bso_it = _binding_site_observers.begin();
       bso_it != _binding_site_observers.end(); bso_it++)
    {
      delete *bso_it;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteFamily::add_binding_site (const BindingSite* binding_site)
{
  // store binding site
  _binding_sites.push_back (binding_site);

  // extend contribution vector
  _rate_contributions.push_back (0);

  // create binding site observer
  int site_index = _rate_contributions.size()-1;
  _binding_site_observers.push_back (new BindingSiteObserver(*binding_site, *this, site_index));
}

void BindingSiteFamily::update (int site_index, int number_available_sites)
{  
  double new_contribution = _binding_sites [site_index]->k_on() * number_available_sites;

  // update total rate contribution by substracting old contribution and adding new one
  _total_rate_contribution = _total_rate_contribution - _rate_contributions[site_index] + new_contribution;
      
  // update contribution in the contribution table
  _rate_contributions [site_index] = new_contribution;

  // check for numerical issues
  if (_total_rate_contribution < 0) { compute_total_rate_contribution(); }

  // notify change to rate managers
  notify_concentration_change();
}


// ============================
//  Public Methods - Accessors
// ============================
//
const BindingSite& BindingSiteFamily::get_random_available_site (void) const
{
  /**
   * We draw a site according to the rate contributions, which is k_on * site_availability.
   */
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_index (_rate_contributions);

  return *(_binding_sites [index_drawn]);
}

bool BindingSiteFamily::is_site_available (void) const
{
  // we loop through sites and return as soon as we find that is available
  int number_sites = _binding_sites.size();
  for (int i = 0; i < number_sites; ++i)
   {
     if (_rate_contributions[i] > 0) return true;
   }

  // if we arrive here, it means that all sites are occupied
  return false;
}


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// BindingSiteFamily& BindingSiteFamily::operator= ( const BindingSiteFamily& other_binding_site_family );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BindingSiteFamily::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
void BindingSiteFamily::compute_total_rate_contribution (void)
{
  // recompute the total rate contribution from the contribution table
  _total_rate_contribution = 0;
  // loop through binding sites
  for (std::vector<double>::iterator rate = _rate_contributions.begin();
       rate != _rate_contributions.end(); ++rate)
    {
      _total_rate_contribution += *rate;
    }
}
