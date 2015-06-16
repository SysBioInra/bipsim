

/**
 * @file bindable.cpp
 * @brief Implementatino of the Bindable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "bindable.h"
#include "forwarddeclarations.h"
#include "site.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Bindable::Bindable (void)
  : _length (1)
{
}

// Not needed for this class (use of default copy constructor) !
// Bindable::Bindable (Bindable& other_bindable);

Bindable::~Bindable (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//
void Bindable::add_termination_site ( const Site& termination_site )
{
  // as a first approximation, we consider that reaching any base of the termination 
  // site sends a termination signal
  int first_position = termination_site.position();
  int last_position = first_position + termination_site.length();
  for ( int i = first_position; i < last_position; i++ )
    {
      _termination_sites[ i ].push_back ( termination_site.family() );
    }
}

bool Bindable::is_termination_site ( int position,
					     const std::list<int>& termination_site_families ) const
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( position <= _length ); /** @pre Position must be smaller or equal to bindable length. */

  // if there is no site to check or no termination site at the position to enquire
  const std::map< int, std::list<int> >::const_iterator local_sites = _termination_sites.find( position );
  if ( ( termination_site_families.size() == 0 ) || ( local_sites == _termination_sites.end() ) )
    {
      return false;
    }
  
  // we loop through the list of termination sites to inspect
  // we place ourselves at the beginnig of the list
  std::list<int>::const_iterator termination_site_family = termination_site_families.begin();
  // we get start and end iterator to the list of sites at the current position on the bindable
  std::list<int>::const_iterator local_sites_begin_iterator = local_sites->second.begin();
  std::list<int>::const_iterator local_sites_end_iterator = local_sites->second.end();
  // we check whether one of the local sites corresponds to one of the sites to inspect
  while ( termination_site_family != termination_site_families.end() )
    {
      std::list<int>::const_iterator local_site = local_sites_begin_iterator;
      while ( local_site != local_sites_end_iterator )
	{
	  if ( *termination_site_family == *local_site )
	    {
	      return true;
	    }
	  local_site++;
	}
      termination_site_family++;
    }

  // if we arrive here, all the tests were non conclusive
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
// Bindable& Bindable::operator= (Bindable& other_bindable);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Bindable::check_invariant ( void ) const
{
  bool result = true;
  result = result && (_length > 0); /** Length must be positive (>0). */
  return result;
}


// =================
//  Private Methods
// =================
//
