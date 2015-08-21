

/**
 * @file siteavailability.cpp
 * @brief Implementation of the SiteAvailability class.
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
#include "siteavailability.h"
#include "siteobserver.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SiteAvailability::SiteAvailability (int first_position, int length, SiteObserver& site_observer)
  : _first (first_position)
  , _last (first_position+length-1)
  , _observer (site_observer)
  , _last_value_notified (-1)
{
}

// Not needed for this class (use of default copy constructor) !
// SiteAvailability::SiteAvailability ( const SiteAvailability& other_site_availability );

SiteAvailability::~SiteAvailability (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SiteAvailability::notify (int current_number_sequences, const std::vector<int>& current_occupancy)
{
  /** @pre Occupancy vector must be larger than last base to investigate. */
  REQUIRE (current_occupancy.size() >= _last);

  // check current occupancy
  int max_occupied = 0;
  for ( int i = _first; i < _last; i++ )
    {
      if ( current_occupancy [ i-1 ] > max_occupied ) { max_occupied = current_occupancy [ i-1 ]; }
    }

  // notify changes
  int number_sites_available = current_number_sequences - max_occupied;
  if (number_sites_available != _last_value_notified)
    {
      _observer.update (number_sites_available);
      _last_value_notified = number_sites_available;
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
// Not needed for this class (use of default overloading) !
// SiteAvailability& SiteAvailability::operator= ( const SiteAvailability& other_site_availability );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool SiteAvailability::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
