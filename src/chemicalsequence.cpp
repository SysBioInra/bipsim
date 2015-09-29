

/**
 * @file chemicalsequence.cpp
 * @brief Implementation of the ChemicalSequence class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <algorithm> // std::fill

// ==================
//  Project Includes
// ==================
//
#include "chemicalsequence.h"
#include "sitelocation.h"
#include "siteavailability.h"
#include "siteobserver.h"
#include "boundchemical.h"
#include "processivechemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalSequence::ChemicalSequence (const std::string& sequence)
  : _sequence (sequence)
{
  _length = _sequence.size();
  
  // initialize occupancy map with zero values
  _occupancy_map.resize (_length, 0);
}

// Not needed for this class (use of default copy constructor) !
// ChemicalSequence::ChemicalSequence (ChemicalSequence& other_chemical_sequence);

ChemicalSequence::~ChemicalSequence (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalSequence::bind_unit ( const BoundChemical& chemical_to_bind )
{
  int position = chemical_to_bind.focused_unit_position();
  int length = chemical_to_bind.focused_unit_length();
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  int last_position = position + length - 1;
  REQUIRE( last_position <= _length );
  
  // add the reference and position to the chemicals map
  _chemical_map[ &chemical_to_bind ].push_back ( SiteLocation( position, length ) );
  
  // update occupancy status
  for ( int i = position; i <= last_position; i++ ) { _occupancy_map [ i-1 ] +=1; }

  // notify changes
  notify_site_availability();
}

void ChemicalSequence::unbind_unit ( const BoundChemical& chemical_to_unbind )
{
  int position = chemical_to_unbind.focused_unit_position();
  int length = chemical_to_unbind.focused_unit_length();
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  int last_position = position + length - 1;
  REQUIRE( last_position <= _length );

  // remove the reference and position from the chemicals map
  remove_reference_from_map ( chemical_to_unbind, position, length );

  // update occupancy status
  for ( int i = position; i <= last_position; i++ ) { _occupancy_map [ i-1 ] -=1; }

  // notify changes
  notify_site_availability();
}

void ChemicalSequence::replace_bound_unit ( const BoundChemical& old_chemical, const BoundChemical& new_chemical )
{
  int old_position = old_chemical.focused_unit_position();
  int old_length = old_chemical.focused_unit_length();
  int old_last_position = old_position + old_length - 1;
  int new_position = new_chemical.focused_unit_position();
  int new_length = new_chemical.focused_unit_length();
  int new_last_position = new_position + new_length - 1;

  // update occupancy status
  for ( int i = old_position; i <= old_last_position; i++ ) { _occupancy_map [ i-1 ] -=1; }
  for ( int i = new_position; i <= new_last_position; i++ ) { _occupancy_map [ i-1 ] +=1; }

  // notify changes
  notify_site_availability();

  // add the reference and position to the chemicals map
  remove_reference_from_map( old_chemical, old_position, old_length );
  _chemical_map[ &new_chemical ].push_back ( SiteLocation( new_position, new_length ) );
}

void ChemicalSequence::move_bound_unit ( ProcessiveChemical& chemical_to_move, int number_steps )
{
  int old_position = chemical_to_move.focused_unit_position();
  int new_position = old_position + number_steps;
  int length = chemical_to_move.focused_unit_length();
  int old_last_position = old_position + length - 1;
  int new_last_position = new_position + length - 1;

  // update occupancy status
  for ( int i = old_position; i <= old_last_position; i++ ) { _occupancy_map [ i-1 ] -=1; }
  for ( int i = new_position; i <= new_last_position; i++ ) { _occupancy_map [ i-1 ] +=1; }

  // notify changes
  notify_site_availability();
  
  // add the reference and position to the chemicals map
  remove_reference_from_map( chemical_to_move, old_position, length );
  _chemical_map[ &chemical_to_move ].push_back ( SiteLocation( new_position, length ) );
}

void ChemicalSequence::add ( int quantity )
{
  Chemical::add (quantity);

  // notify changes
  notify_site_availability();
}

void ChemicalSequence::remove ( int quantity )
{
  Chemical::remove (quantity);
  
  // notify changes
  notify_site_availability();
}

void ChemicalSequence::watch_site_availability (int position, int length, SiteObserver& site_observer)
{
  REQUIRE (position > 0); /** @pre Position must be positive. */
  REQUIRE (length > 0); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  REQUIRE (position + length - 1 <= _length);

  site_observer.update (0);
			
  // create site availability notifier
  _sites_to_watch.push_back (SiteAvailability (position, length, site_observer));

  // send first notification about site availability
  _sites_to_watch.back().notify (number(), _occupancy_map);
}


void ChemicalSequence::print (std::ostream& output) const
{
  output << "Chemical sequence of length " << _length << " with currently "
	 << number() << " molecules.";
}



// ============================
//  Public Methods - Accessors
// ============================
//
void ChemicalSequence::add_termination_site ( const Site& termination_site )
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

bool ChemicalSequence::is_termination_site ( int position,
					     const std::list<int>& termination_site_families ) const
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( position <= _length ); /** @pre Position must be smaller or equal to sequence length. */

  // if there is no site to check or no termination site at the position to enquire
  const std::map< int, std::list<int> >::const_iterator local_sites = _termination_sites.find( position );
  if ( ( termination_site_families.size() == 0 ) || ( local_sites == _termination_sites.end() ) )
    {
      return false;
    }
  
  // we loop through the list of termination sites to inspect
  // we place ourselves at the beginnig of the list
  std::list<int>::const_iterator termination_site_family = termination_site_families.begin();
  // we get start and end iterator to the list of sites at the current position on the sequence
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
// ChemicalSequence& ChemicalSequence::operator= (ChemicalSequence& other_chemical_sequence);

// =================
//  Private Methods
// =================
//
void ChemicalSequence::remove_reference_from_map ( const BoundChemical& chemical, int position, int length )
{
  SiteLocationList& location_list = _chemical_map[ &chemical ];
  REQUIRE( location_list.size() > 0 ); /** @pre There must be a chemical of this type in the map. */

  if ( location_list.size() == 1 ) // there is only one element in the list
    {
      // the list will be empty, we can remove it from the map
      // (no more chemical of the same type is bound to the sequence)
      _chemical_map.erase( &chemical );
    }
  else // at least one chemical of same type will be left on the sequence
    {
      // we erase a chemical at the right location
      SiteLocationList::iterator site_location = location_list.begin();
      while ( (( site_location->position() != position )
	       || ( site_location->length() != length ))
	      && (site_location != location_list.end()) )
	{ 
	  site_location++;
	}

      if ( site_location != location_list.end() )
	{
	  location_list.erase ( site_location );
	}
      else 
	{
	  std::cerr << "ERROR: trying to delete bound chemical " << &chemical << " that does not exist at position "
		    << position << " with length " << length << std::endl;
	  site_location = location_list.begin();
	  while (  (site_location != location_list.end()) )
	    { 
	      std::cout << site_location->position() << " " << site_location->length() << std::endl;
	      site_location++;
	    }
	}
    }
}

void ChemicalSequence::notify_site_availability (void)
{
  for (std::list<SiteAvailability>::iterator site_it = _sites_to_watch.begin();
       site_it != _sites_to_watch.end(); site_it++)
    {
      site_it->notify (number(), _occupancy_map);
    }
  
}
