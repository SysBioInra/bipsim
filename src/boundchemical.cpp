

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
#include "boundunit.h"
#include "boundunitfilter.h"


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
      output << " initially bound at position "
	     << unit->binding_site().first() << " and now at position " 
	     << unit->first() << "." << std::endl;
    }
}
