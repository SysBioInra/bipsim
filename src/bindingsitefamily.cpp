

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
#include "bindingsite.h"
#include "randomhandler.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteFamily::BindingSiteFamily (void) {}

// Forbidden
// BindingSiteFamily::BindingSiteFamily (const BindingSiteFamily& other_family);
// BindingSiteFamily& BindingSiteFamily::operator= ( const BindingSiteFamily& other_family );
// BindingSiteFamily::~BindingSiteFamily (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteFamily::add (BindingSite* binding_site)
{
  // store binding site
  _binding_sites.push_back (binding_site);

  // extend contribution vector
  _rate_contributions.extend (1);

  // configure update message to be index in the rate vector
  binding_site->set_update_message (_rate_contributions.size()-1);
}

void BindingSiteFamily::update (int site_index, int rate_contribution)
{  
  _rate_contributions.set_rate (site_index, rate_contribution);
      
  // notify change to rate managers
  notify_change();
}


// ============================
//  Public Methods - Accessors
// ============================
//
bool BindingSiteFamily::is_site_available (void) const
{
  // we loop through sites and return as soon as we find that is available
  int number_sites = _binding_sites.size();
  for (int i = 0; i < number_sites; ++i)
   {
     if (_rate_contributions [i] > 0) return true;
   }

  // if we arrive here, it means that all sites are occupied
  return false;
}


// =================
//  Private Methods
// =================
//
