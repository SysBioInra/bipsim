

/**
 * @file cellstate.cpp
 * @brief Implementation of the CellState class.
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
#include "binding.h"
#include "cellstate.h"
#include "parser.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
CellState::CellState (const char* filename)
{
  // link binding reactions with binding site handler
  Binding::set_binding_site_handler (_binding_site_handler);

  // open new parser and load file
  Parser parser (_chemical_handler, _reaction_handler, _binding_site_handler, _termination_site_handler, _table_handler);
  parser.parse_units (filename);
  parser.parse_reactions (filename);
}

// Not needed for this class (use of default copy constructor) !
// CellState::CellState ( const CellState& other_cell_state );

CellState::~CellState (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// CellState& CellState::operator= ( const CellState& other_cell_state );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool CellState::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
