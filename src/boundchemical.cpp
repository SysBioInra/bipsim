

/**
 * @file boundchemical.cpp
 * @brief Implementation of the BoundChemical class.
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
#include "boundchemical.h"
#include "bindingsite.h"
#include "randomhandler.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
// Not needed for this class (use of default copy constructor) !
// BoundChemical::BoundChemical (BoundChemical& other_bound_chemical);

BoundChemical::~BoundChemical (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BoundChemical::handle_collision (void)
{
  std::cout << "Function" << __func__ << "remains to be defined in" << __FILE__ << __LINE__ << std::endl;
}

void BoundChemical::add_unit_at_site ( const BindingSite& binding_site )
{
  add ( 1 );

  _family_map[ binding_site.family() ].push_front( BoundUnit ( &binding_site, binding_site.position() ) );

  _focused_unit = _family_map[ binding_site.family() ].begin();
}

void BoundChemical::focus_random_unit ( void )
{
  REQUIRE( _number > 0 ); /** @pre There must be at least one unit */

  // draw a random chemical
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_uniform ( 0, _number-1 );

  // find the corresponding unit in the map
  int current_index = 0;
  // first loop through the families until the family of the drawn index is found
  UnitFamilyMap::iterator family = _family_map.begin();
  BoundUnitList& family_units = family->second;
  while ( current_index + family_units.size() <= index_drawn )
    {
      current_index += family_units.size();
      family++;
      family_units = family->second;
    }
  // then loop through units for the exact index
  BoundUnitList::iterator unit = family_units.begin();
  for ( int index = current_index; index < index_drawn; index++ )
    { 
      unit++; 
    }

  // focus unit
  _focused_unit = unit;
}


void BoundChemical::focus_random_unit ( int family_id )
{
  // get the list of positions of chemicalts bound to requested family
  UnitFamilyMap::iterator family = _family_map.find ( family_id );
  REQUIRE( family != _family_map.end() ); /** @pre family_id is already defined */
  BoundUnitList& family_units = family->second;
  
  // draw a random chemical to unbind
  int number_units = family_units.size();
  RandomHandler random_handler;
  int index_drawn = random_handler.draw_uniform ( 0, number_units-1 );

  // retrieve the corresponding position
  BoundUnitList::iterator unit = family_units.begin();
  for ( int index = 0; index < index_drawn; index++ )
    { 
      unit++; 
    }

  // focus unit
  _focused_unit = unit;
}

void BoundChemical::add_unit_in_place_of ( const BoundChemical& precursor )
{
  // here we add a new unit but we copy the characteristics of the focused unit
  // of the precursor species
  add ( 1 );
  const BindingSite& binding_site = precursor.focused_unit_binding_site();
  int family_id = binding_site.family();
  int position = precursor.focused_unit_position();

  _family_map[ family_id ].push_front( BoundUnit ( &binding_site, position ) );

  _focused_unit = _family_map[ family_id ].begin();
  
}


void BoundChemical::remove_focused_unit ( void )
{
  int family_id = (_focused_unit->first)->family();
  UnitFamilyMap::iterator family = _family_map.find ( family_id );
  BoundUnitList& family_units = family->second;

  // remove chemical
  family_units.erase ( _focused_unit );
  remove ( 1 );

  // if there are no more binding sites belonging to the family
  if ( family_units.size() == 0 ) 
    {
      // erase the empty family list from the map
      _family_map.erase ( family );
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//
int BoundChemical::number_bound_to_family ( int binding_site_family ) const
{
  // get the list of positions of chemicals bound to requested family
  UnitFamilyMap::const_iterator family = _family_map.find ( binding_site_family );

  // we check whether there are indeed some chemicals in the list
  if ( family != _family_map.end() )
    {
      return ( family->second ).size();
    }
  else // there are no chemicals bound to the requested family
    {
      return 0;
    }
}

double BoundChemical::get_total_unbinding_rate_contribution ( int binding_site_family ) const
{
  /**
   * Unbinding rate is generally defined by   r = k_off x [A].
   * However here k_off varies from a binding site to another so it becomes
   * r_total = sum ( r_i ) = sum ( k_off_i x [A_i] )
   * Which is also the dot product between the vector of concentrations and the vector
   * of k_off. As we loop through individual units, this can even be seen
   * as a simple sum of unbinding rates (each k_off_i is counted the appropriate number
   * of times).
   */

  // get the list of positions of chemicals bound to requested family
  UnitFamilyMap::const_iterator family = _family_map.find ( binding_site_family );

  // we check whether there are indeed some chemicals in the list
  if ( family == _family_map.end() ) // there are no chemicals bound to the requested family
    {
      return 0;
    }
  
  // we loop through the list and sum the k_off
  double r_total = 0;
  const BoundUnitList& family_units = family->second;
  for ( BoundUnitList::const_iterator unit = family_units.begin();
	unit != family_units.end() ; unit++ )
    {
      r_total += unit->first->k_off();
    }
  return r_total;
}

void BoundChemical::print ( std::ostream& output ) const
{
  for ( UnitFamilyMap::const_iterator family = _family_map.begin();
	family != _family_map.end(); family ++ )
    {
        const BoundUnitList& family_units = family->second;
	for ( BoundUnitList::const_iterator unit = family_units.begin();
	      unit != family_units.end(); unit++ )
	  {
	    output << "Bound chemical initially bound at position "
		   << unit->first->position()
		   << " and now at position "
		   << unit->second
		   << "." << std::endl;
	  }
    }
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
// BoundChemical& BoundChemical::operator= (BoundChemical& other_bound_chemical);


// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BoundChemical::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
