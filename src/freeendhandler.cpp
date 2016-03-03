

/**
 * @file freeendhandler.cpp
 * @brief Implementation of the FreeEndHandler class.
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
#include "freeendhandler.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
FreeEndHandler::FreeEndHandler (ChemicalSequence* opposite_sequence /*= 0*/)
    : _opposite (opposite_sequence)
  {}

// Not needed for this class (use of compiler-generated versions)
// FreeEndHandler::FreeEndHandler (const FreeEndHandler& other);
// FreeEndHandler& FreeEndHandler::operator= (const FreeEndHandler& other);
// FreeEndHandler::~FreeEndHandler (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
BindingSite* FreeEndHandler::create_left (int position) const
{
  if (_opposite == 0) { return 0; }
  return &(_opposite->create_left_end_binding_site (position));
}

BindingSite* FreeEndHandler::create_right (int position) const
{
  if (_opposite == 0) { return 0; }
  return &(_opposite->create_right_end_binding_site (position));
}

void FreeEndHandler::remove (BindingSite* site) const
{
  if ((_opposite == 0) || (site == 0)) return;
  _opposite->remove_free_end_binding_site (*site);
}

void FreeEndHandler::set_opposite (ChemicalSequence& opposite_sequence)
{ 
  /** @pre Pairing must not have been defined before. */
  REQUIRE (_opposite == 0);
  _opposite = &opposite_sequence; 
}

void FreeEndHandler::break_pairing (void)
{ 
  _opposite = 0; 
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
