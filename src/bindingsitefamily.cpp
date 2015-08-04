

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
#include "randomhandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteFamily::BindingSiteFamily (void)
{
}

// Not needed for this class (use of default copy constructor) !
// BindingSiteFamily::BindingSiteFamily ( const BindingSiteFamily& other_binding_site_family );

BindingSiteFamily::~BindingSiteFamily (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteFamily::update_rate_contributions (void)
{
  /**
   * Binding rate is generally defined by r = k_on x [A] x [B], where [A] is the concentration
   * of units to bind and [B] the concentration of binding sites. However, k_on varies from a
   * site to another so the total binding rate becomes
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * Here we need to compute the second part of the binding rate.
   */

  _total_rate_contribution = 0;
  // loop through binding sites
  std::vector<double>::iterator rate = _rate_contributions.begin();
  for (std::vector<const BindingSite*>::iterator site = _binding_sites.begin();
       site != _binding_sites.end(); ++site, ++rate)
    {
      // compute contribution from number of available sites and k_on for each binding site
      (*rate) = (*site)->rate_contribution();
      _total_rate_contribution += *rate;
    }
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
     if (_binding_sites[i]->number_available_sites() > 0) return true;
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
