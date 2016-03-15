

/**
 * @file familyfilter.cpp
 * @brief Implementation of the FamilyFilter class.
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
#include "familyfilter.h"
#include "boundunit.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
FamilyFilter::FamilyFilter (const BindingSiteFamily& family)
  : _family (&family)
{
}

// Not needed for this class (use of compiler-generated versions)
// FamilyFilter::FamilyFilter (const FamilyFilter& other);
// FamilyFilter& FamilyFilter::operator= (const FamilyFilter& other);
// FamilyFilter::~FamilyFilter (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
inline void FamilyFilter::add (BoundUnit& unit)
{
  if ((&(unit.binding_site().family()) == _family)
      && (unit.binding_site().first() == unit.first())) 
    { _units.add (&unit); }
}

inline void FamilyFilter::remove (BoundUnit& unit)
{
  if (&(unit.binding_site().family()) == _family) { _units.remove (&unit); }
}


// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
