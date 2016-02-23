

/**
 * @file freeendfactory.cpp
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
#include "freeendfactory.h"
#include "chemicalsequence.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
FreeEndFactory::FreeEndFactory (ChemicalSequence& location,
				ChemicalSequence& opposite_strand,
				BindingSiteFamily& left_family,
				BindingSiteFamily& right_family)
  : _location (location)
  , _opposite (opposite_strand)
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
BindingSite* FreeEndFactory::create_left (int position) const
{
  int c_position = _location.complementary (position);
  
  return new BindingSite (_left_family, _opposite, c_position, 
			  c_position, 1, 1, c_position, false);
}

BindingSite* FreeEndFactory::create_right (int position) const
{
  int c_position = _location.complementary (position);
  
  return new BindingSite (_right_family, _opposite, c_position, 
			  c_position, 1, 1, c_position, false);
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
