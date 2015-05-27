

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
ChemicalSequence::ChemicalSequence ( void )
{
  // intialize occupancy map with zero values
  _occupancy_map.resize( this->length(), 0 );
}

// Not needed for this class (use of default copy constructor) !
// ChemicalSequence::ChemicalSequence (ChemicalSequence& other_chemical_sequence);

ChemicalSequence::~ChemicalSequence ( void )
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
  REQUIRE( last_position <= this->length() );
  
  // add the reference and position to the chemicals map
  _chemical_map[ &chemical_to_bind ].push_back ( SiteLocation( position, length ) );
  
  // update occupancy status
  for ( int i = position; i <= last_position; i++ ) { _occupancy_map [ i-1 ] +=1; }
}

void ChemicalSequence::unbind_unit ( const BoundChemical& chemical_to_unbind )
{
  int position = chemical_to_unbind.focused_unit_position();
  int length = chemical_to_unbind.focused_unit_length();
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  int last_position = position + length - 1;
  REQUIRE( last_position <= this->length() );

  // remove the reference and position from the chemicals map
  remove_reference_from_map ( chemical_to_unbind, position, length );

  // update occupancy status
  for ( int i = position; i <= last_position; i++ ) { _occupancy_map [ i-1 ] -=1; }
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
  
  // add the reference and position to the chemicals map
  remove_reference_from_map( chemical_to_move, old_position, length );
  _chemical_map[ &chemical_to_move ].push_back ( SiteLocation( new_position, length ) );

  // TODO: handle collisions
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



// ============================
//  Public Methods - Accessors
// ============================
//
int ChemicalSequence::number_available_sites ( int position, int length ) const
{
  REQUIRE( position > 0 ); /** @pre Position must be positive. */
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  /** @pre Position and length must be consistent with sequence length. */
  REQUIRE( position + length - 1 <= this->length() );

  // check for the lowest occupancy status
  int max_occupied = 0;
  for ( int i = position; i < position + length; i++ )
    {
      if ( _occupancy_map [ i-1 ] > max_occupied ) { max_occupied = _occupancy_map [ i-1 ]; }
    }

  int result = 0;
  if ( max_occupied < this->number() ) { result = this->number() - max_occupied; }

  ENSURE( result >= 0 ); /** @post The number of available sites is nonnegative. */
  /** @post The number of available sites is smaller than the number of bearing sequences. */
  ENSURE( result <= this->number() );
  return result;
}

// ==========================
//  Public Methods - Setters
// ==========================
//
void ChemicalSequence::set_length ( int length )
{
  Bindable::set_length ( length );
  // set that every position on every chemical is available
  _occupancy_map.resize ( this->length() );
  std::fill ( _occupancy_map.begin(), _occupancy_map.end(), 0 );
}


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
  /** The invariants of parent classes must be verified. */
  bool result = Chemical::check_invariant(); 
  result = result && Bindable::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
void ChemicalSequence::remove_reference_from_map ( const BoundChemical& chemical, int position, int length )
{
  SiteLocationList& location_list = _chemical_map[ &chemical ];
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
      while ( ( site_location->position() != position )
	      || ( site_location->length() != length ))
	{ 
	  site_location++;
	}
      location_list.erase ( site_location );
    }
}
