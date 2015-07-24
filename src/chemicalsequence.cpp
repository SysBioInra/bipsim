

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
  _occupancy_map.resize( _length, 0 );
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
  for (int i = 0; i < _focus_area_start.size(); i++) { update_focus_area_occupancy (i); }
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
  for (int i = 0; i < _focus_area_start.size(); i++) { update_focus_area_occupancy (i); }
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
  for (int i = 0; i < _focus_area_start.size(); i++) { update_focus_area_occupancy (i); }

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
  for (int i = 0; i < _focus_area_start.size(); i++) { update_focus_area_occupancy (i); }
  
  // add the reference and position to the chemicals map
  remove_reference_from_map( chemical_to_move, old_position, length );
  _chemical_map[ &chemical_to_move ].push_back ( SiteLocation( new_position, length ) );
}

void ChemicalSequence::elongate_nascent (void)
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
}

void ChemicalSequence::terminate_nascent (void)
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
}

void ChemicalSequence::add ( int quantity )
{
  Chemical::add ( quantity );
}

void ChemicalSequence::remove ( int quantity )
{
  Chemical::remove ( quantity );
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
}

int ChemicalSequence::create_focus_area (int position, int length)
{
  REQUIRE (position > 0); /** @pre Position must be positive. */
  REQUIRE (length > 0); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  REQUIRE (position + length - 1 <= _length);

  int identifier = _focus_area_start.size();
  _focus_area_start.resize (identifier+1, position);
  _focus_area_end.resize (identifier+1, position+length);
  _focus_area_max_occupancy.resize (identifier+1, 0);
  
  update_focus_area_occupancy (identifier);

  return identifier;
}


void ChemicalSequence::print (std::ostream& output) const
{
  output << "Chemical sequence of length " << _length << " with currently "
	 << _number << " molecules.";
}



// ============================
//  Public Methods - Accessors
// ============================
//
int ChemicalSequence::number_available_sites ( int position, int length ) const
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  REQUIRE( position + length - 1 <= _length );
  
  // check for the lowest occupancy status
  int max_occupied = 0;
  for ( int i = position; i < position + length; i++ )
    {
      if ( _occupancy_map [ i-1 ] > max_occupied ) { max_occupied = _occupancy_map [ i-1 ]; }
    }
  
  int result = 0;
  if ( max_occupied < _number ) { result = _number - max_occupied; }
  
  ENSURE( result >= 0 ); /** @post The number of available sites is nonnegative. */
  /** @post The number of available sites is smaller than the number of bearing sequences. */
  ENSURE( result <= _number );
  return result;
}

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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ChemicalSequence::check_invariant (void) const
{
  bool result = 
    Chemical::check_invariant()   /** The invariant of parent class must be verified. */
    && (_length > 0); /** Length must be positive (>0). */
  return result;
}


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

void ChemicalSequence::update_focus_area_occupancy (int area_id)
{
  /** @pre Identifier must be within vector range. */
  REQUIRE (area_id >= 0);
  REQUIRE (area_id < _focus_area_max_occupancy.size());

  // check for the highest occupancy status
  int max_occupied = 0;
  int last_position = _focus_area_end [area_id];
  for ( int i = _focus_area_start [area_id]; i < last_position; i++ )
    {
      if ( _occupancy_map [ i-1 ] > max_occupied ) { max_occupied = _occupancy_map [ i-1 ]; }
    }
  
  _focus_area_max_occupancy [area_id] = max_occupied;
}
