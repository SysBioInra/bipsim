

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
SiteAvailability::SiteAvailability (int first, int last, 
				    SiteObserver& site_observer)
  : _first (first)
  , _last (last)
  , _observer (site_observer)
  , _last_value_notified (-1)
{
  /** @pre First position must be positive. */
  REQUIRE (first >= 0);
  /** @pre Last position must be greater than first. */
  REQUIRE (last >= first);
}

// Not needed for this class (use of compiler generated versions)
// SiteAvailability::SiteAvailability (const SiteAvailability& other_site_availability);
// SiteAvailability& SiteAvailability::operator= (const SiteAvailability& other_site_availability);
// SiteAvailability::~SiteAvailability (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void SiteAvailability::notify (int a, int b, int current_number_sequences,
			       const std::vector<int>& current_occupancy)
{
  /** @pre Occupancy vector must be larger than last base to investigate. */
  REQUIRE (current_occupancy.size() >= _last);

  // check if site was affected by change
  if (((_first < a) && (_last < a))
       || ((_first > b) && (_last > b))) return; // site not affected by change

  // check current occupancy
  int max_occupied = 0;
  for ( int i = _first; i < _last; i++ )
    {
      if ( current_occupancy [i] > max_occupied )
	{ max_occupied = current_occupancy [i]; }
    }

  // notify changes
  int number_sites_available = current_number_sequences - max_occupied;
  if (number_sites_available < 0) number_sites_available = 0;
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

// =================
//  Private Methods
// =================
//
