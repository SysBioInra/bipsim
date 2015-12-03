

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
void BindingSiteFamily::add (const BindingSite* binding_site)
{
  // store binding site
  _binding_sites.push_back (binding_site);

  // extend contribution vector
  _rate_contributions.extend (1);

  // create binding site observer
  int site_index = _rate_contributions.size()-1;
  _binding_site_observers.push_back 
    (new BindingSiteObserver(*binding_site, *this, site_index));
}

void BindingSiteFamily::update (int site_index, int number_available_sites)
{  
  _rate_contributions.set_rate
    (site_index,
     _binding_sites [site_index]->k_on() * number_available_sites);
      
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



// =================
//  Private Methods
// =================
//
