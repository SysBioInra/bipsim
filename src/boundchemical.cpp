

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
  const std::list<int>* local_switches
    = unit.location().switch_sites (unit.reading_frame());
  // if some local switch applies to unit, apply and return
  if (local_switches)
  {
    for (std::list <int>::const_iterator switch_it = local_switches->begin();
	 switch_it != local_switches->end(); ++switch_it)
    {
      std::map <int, Switch*>::iterator result = _switches.find(*switch_it);
      if (result != _switches.end())
      {
	result->second->output().add (unit); return;
      }
    }
  }
  
  // no switching happened: add unit to collection
  Chemical::add (1);
  _units.add (unit);
  for (std::list <BoundUnitFilter*>::iterator filter_it = _filters.begin();
       filter_it != _filters.end(); ++filter_it)
    { (*filter_it)->add (unit); }
}

void BoundChemical::remove (BoundUnit& unit)
{
  Chemical::remove (1);
  _units.remove (unit);
  for (std::list <BoundUnitFilter*>::iterator filter_it = _filters.begin();
       filter_it != _filters.end(); ++filter_it)
    { (*filter_it)->remove (unit); }
}

void BoundChemical::add_switch (Switch& switch_)
{
  /** @pre Input of switch must be current BoundChemical. */
  REQUIRE (&switch_.input() == this);
  
  _switches[switch_.id()] = &switch_;
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
