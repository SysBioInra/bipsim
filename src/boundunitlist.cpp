

/**
 * @file boundunitlist.cpp
 * @brief Implementation of the BoundUnitList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "boundunitlist.h"
#include "bindingsite.h"
#include "boundunit.h"
#include "randomhandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BoundUnitList::BoundUnitList (void)
 : _current (0)
{ 
}

// BoundUnitList::BoundUnitList (const BoundUnitList& other);
// BoundUnitList& BoundUnitList::operator= (const BoundUnitList& other);
// BoundUnitList::~BoundUnitList (void);

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//
double BoundUnitList::unbinding_rate (void) const
{
  // we loop through the list and sum the k_off
  double r_total = 0;
  for (std::vector <BoundUnit*>::const_iterator unit_it = _v.begin();
       unit_it != _v.end(); ++unit_it)
    { r_total += (*unit_it)->binding_site().k_off(); }
  return r_total;
}

BoundUnit& BoundUnitList::random_unit (void) const
{
  /** @pre List must not be empty. */
  REQUIRE (_v.size() > 0);
  _current = RandomHandler::instance().draw_uniform (0, _v.size()-1);
  return *(_v [_current]);
}

// =================
//  Private Methods
// =================
//
