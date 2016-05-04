
/**
 * @file boundunitfactory.cpp
 * @brief Implementation of the BoundUnitFactory class.
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
#include "boundunitfactory.h"
#include "boundunit.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BoundUnitFactory BoundUnitFactory::_instance;

BoundUnitFactory::BoundUnitFactory (void)
{}

// BoundUnitFactory::BoundUnitFactory (const BoundUnitFactory& other);
// BoundUnitFactory& BoundUnitFactory::operator= (const BoundUnitFactory& other);

BoundUnitFactory::~BoundUnitFactory (void)
{
  for (std::list <BoundUnit*>::iterator unit_it = _unused.begin();
       unit_it != _unused.end(); ++unit_it)
    {
      _created.remove (*unit_it);
    }
  for (std::list <BoundUnit*>::iterator unit_it = _created.begin();
       unit_it != _created.end(); ++unit_it)
    {
      delete *unit_it;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
BoundUnit& BoundUnitFactory::create (ChemicalSequence& location, 
				     int first, int last, int reading_frame)
{
  BoundUnit* unit;
  if (_unused.empty()) 
    { 
      unit = new BoundUnit (location, first, last, reading_frame); 
      _created.push_back (unit);
    }
  else 
    {
      unit = _unused.back();
      _unused.pop_back();
      unit->rebind (location, first, last, reading_frame);
    }
  return *unit;
}

BoundUnit& BoundUnitFactory::create (const BindingSite& site)
{
  BoundUnit* unit;
  if (_unused.empty()) 
    { 
      unit = new BoundUnit (site); 
      _created.push_back (unit);
    }
  else 
    {
      unit = _unused.back();
      _unused.pop_back();
      unit->rebind (site);
    }
  return *unit;
}

void BoundUnitFactory::free (BoundUnit& unit)
{
  _unused.push_back (&unit);
} 

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
