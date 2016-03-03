

/**
 * @file freeendbindingsitefactory.cpp
 * @brief Implementation of the ClassName class.
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
#include "freeendbindingsitefactory.h"
#include "chemicalsequence.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
FreeEndBindingSiteFactory::
FreeEndBindingSiteFactory (ChemicalSequence& location,
			   BindingSiteFamily& left_family,
			   BindingSiteFamily& right_family)
  : _location (location)
  , _left_family (left_family)
  , _right_family (right_family)
{}

// Not needed for this class (use of compiler-generated versions)
// ClassName::ClassName (const ClassName& other);
// ClassName& ClassName::operator= (const ClassName& other);
// ClassName::~ClassName (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
BindingSite* FreeEndBindingSiteFactory::create_left (int position) const
{
  return new BindingSite (_left_family, _location, position, 
			  position, 1, 1, position, false);
}

BindingSite* FreeEndBindingSiteFactory::create_right (int position) const
{
  return new BindingSite (_right_family, _location, position, 
			  position, 1, 1, position, false);
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
