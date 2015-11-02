

/**
 * @file bindingsiteobserver.cpp
 * @brief Implementation of the ClassName class.
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
#include "bindingsiteobserver.h"
#include "bindingsitefamily.h"

// ==========================
//  Constructors/Destructors
// ==========================
//

BindingSiteObserver::BindingSiteObserver (const BindingSite& site_to_watch,
					  BindingSiteFamily& family_to_notify,
					  int site_identifier)
  : _site_id (site_identifier)
  , _family (family_to_notify)
{
  site_to_watch.location().watch_site_availability
    (site_to_watch.first(), site_to_watch.last(), *this);
}

// Not needed for this class (use of default copy constructor) !
// BindingSiteObserver::BindingSiteObserver ( const BindingSiteObserver& other_binding_site_observer );

BindingSiteObserver::~BindingSiteObserver (void)
{
}


// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSiteObserver::update (int number_of_available_sites)
{
  /** @pre Number of available sites must be positive); */
  REQUIRE (number_of_available_sites >= 0);
  _family.update (_site_id, number_of_available_sites);
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
// Not needed for this class (use of default overloading) !
// BindingSiteObserver& BindingSiteObserver::operator= ( const BindingSiteObserver& other_binding_site_observer );


// =================
//  Private Methods
// =================
//
