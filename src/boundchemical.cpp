

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

BoundChemical::BoundChemical (void) {}

// Forbidden
// BoundChemical::BoundChemical (BoundChemical& other_bound_chemical);
// BoundChemical& BoundChemical::operator= (BoundChemical& other_bound_chemical);

BoundChemical::~BoundChemical (void)
{
  // delete remaining BoundUnits
  for (UnitFamilyMap::const_iterator family = _family_map.begin();
       family != _family_map.end(); family ++ )
    {
      const BoundUnitList& family_units = family->second;
      for (int i = 0; i < family_units.size(); ++i) { delete family_units [i]; }
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//

void BoundChemical::add_unit_at_site (const BindingSite& binding_site)
{
  add_unit (binding_site, binding_site.first(), binding_site.reading_frame());
}

void BoundChemical::add_unit_in_place_of (const BoundChemical& precursor)
{
  // here we add a new unit but we copy the characteristics of the focused unit
  // of the precursor species
  const BindingSite& binding_site = precursor.focused_unit_binding_site();
  int position = precursor.focused_unit_first();
  int reading_frame = precursor.focused_unit_reading_frame();
  
  add_unit (binding_site, position, reading_frame);  
}


void BoundChemical::focus_random_unit (void)
{
  /** @pre There must be at least one unit */
  REQUIRE (number() > 0); 

  // draw a random chemical
  int index_drawn = RandomHandler::instance().draw_uniform (0, number()-1);

  // find the corresponding unit in the map
  // first loop through the families until the family of the drawn index is found
  UnitFamilyMap::iterator family = _family_map.begin();
  BoundUnitList* family_units = &family->second;
  while (index_drawn >= family_units->size())
    {
      index_drawn -= family_units->size();
      ++family;
      family_units = &family->second;
    }

  // focus unit
  _focused_unit = (*family_units) [index_drawn];
}


void BoundChemical::focus_random_unit (int binding_site_family)
{
  // get the list of positions of chemicalts bound to requested family
  UnitFamilyMap::iterator family_it = _family_map.find (binding_site_family);
  /** @pre family_id is already defined */
  REQUIRE (family_it != _family_map.end()); 
  BoundUnitList& family_units = family_it->second;
  
  // focus unit
  _focused_unit = family_units
    [RandomHandler::instance().draw_uniform (0, family_units.size()-1)];
}



void BoundChemical::remove_focused_unit (void)
{
  int family_id = (_focused_unit->binding_site()).family();
  UnitFamilyMap::iterator family = _family_map.find (family_id);
  BoundUnitList& family_units = family->second;

  // remove chemical
  delete _focused_unit;
  family_units.erase (_focused_unit);
  remove (1);

  // if there are no more binding sites belonging to the family
  if (family_units.size() == 0) 
    {
      // erase the empty family list from the map
      _family_map.erase (family);
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//
int BoundChemical::number_bound_to_family (int binding_site_family) const
{
  // get the list of positions of chemicals bound to requested family
  UnitFamilyMap::const_iterator family = _family_map.find (binding_site_family);

  // we check whether there are indeed some chemicals in the list
  if (family != _family_map.end())
    {
      return (family->second).size();
    }
  else // there are no chemicals bound to the requested family
    {
      return 0;
    }
}

double BoundChemical::unbinding_rate_contribution (int binding_site_family) const
{
  /**
   * Unbinding rate is generally defined by   r = k_off x [A].
   * However here k_off varies from a binding site to another so it becomes
   * r_total = sum (r_i) = sum ( k_off_i x [A_i] )
   * Which is also the dot product between the vector of concentrations and the
   * vector of k_off. As we loop through individual units, this can even be seen
   * as a simple sum of unbinding rates (each k_off_i is counted the 
   * appropriate number of times).
   */

  // get the list of positions of chemicals bound to requested family
  UnitFamilyMap::const_iterator family = 
    _family_map.find (binding_site_family);

  // we check whether there are indeed some chemicals in the list
  if (family == _family_map.end()) { return 0; }
  else { return (family->second).unbinding_rate(); }
}

// =================
//  Private Methods
// =================
//
void BoundChemical::add_unit (const BindingSite& binding_site, int position, int reading_frame)
{
  add (1);
  _focused_unit = new BoundUnit (binding_site, position, reading_frame);
  _family_map [binding_site.family()].insert (_focused_unit);
}

void BoundChemical::print (std::ostream& output) const
{
  for (UnitFamilyMap::const_iterator family = _family_map.begin();
       family != _family_map.end(); family ++ )
    {
        const BoundUnitList& family_units = family->second;
	for (int i = 0; i < family_units.size(); ++i)
	  {
	    const BoundUnit* unit = family_units [i];
	    output << "Bound chemical initially bound at position "
		   << unit->binding_site().first()
		   << " and now at position "
		   << unit->current_position()
		   << "." << std::endl;
	  }
    }
}
