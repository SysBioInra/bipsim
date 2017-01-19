

/**
 * @file boundchemical.cpp
 * @brief Implementation of the BoundChemical class.
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
#include "boundunit.h"
#include "boundunitfilter.h"
#include "switch.h"

// ==========================
//  Constructors/Destructors
// ==========================
//

BoundChemical::BoundChemical (void)
{
}

// BoundChemical::BoundChemical (BoundChemical& other);
// BoundChemical& BoundChemical::operator= (BoundChemical& other);
// BoundChemical::~BoundChemical (void)

// ===========================
//  Public Methods - Commands
// ===========================
//

void BoundChemical::add (BoundUnit& unit)
{
  // before adding, check if unit is on a switch
  for (std::list <Switch*>::iterator switch_it = _switches.begin();
       switch_it != _switches.end(); ++switch_it)
    {
      if (unit.location().is_switch_site (unit.reading_frame(), (*switch_it)->id()))
	  {
	    // apply switch and return
	    (*switch_it)->output().add (unit);
	    return;
	  }
    }

  // no switching happened: add unit to collection
  Chemical::add (1);
  { _units.add (&unit); }
  for (std::list <BoundUnitFilter*>::iterator filter_it = _filters.begin();
       filter_it != _filters.end(); ++filter_it)
    { (*filter_it)->add (unit); }
}

void BoundChemical::remove (BoundUnit& unit)
{
  Chemical::remove (1);
  { _units.remove (&unit); }
  for (std::list <BoundUnitFilter*>::iterator filter_it = _filters.begin();
       filter_it != _filters.end(); ++filter_it)
    { (*filter_it)->remove (unit); }
}

void BoundChemical::add_switch (Switch& switch_)
{
  /** @pre Input of switch must be current BoundChemical. */
  REQUIRE (&switch_.input() == this);
  
  _switches.push_back (&switch_);
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void BoundChemical::print (std::ostream& output) const
{
  output << "Bound chemical";
  for (int i = 0; i < _units.size(); ++i)
    {
      BoundUnit* unit = _units()[i];
      output << " at position " 
	     << unit->first() << "." << std::endl;
    }
}
