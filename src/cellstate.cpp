

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
#include "cellstate.h"
#include "parser.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
CellState::CellState (void)
{
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
void CellState::store (SimulatorInput* element, const std::string& name /*= ""*/)
{
  // try to store element
  if (not (_site_handler.store (element, name)
	   || _binding_site_family_handler.store (element, name)
	   || _site_family_handler.store (element, name)
	   || _chemical_handler.store (element, name)
	   || _reaction_handler.store (element, name)
	   || _bireaction_handler.store (element, name)
	   || _composition_table_handler.store (element, name)
	   || _decoding_table_handler.store (element, name)
	   || _product_table_handler.store (element, name)
	   || _transformation_table_handler.store (element, name)))
    {
      // TODO throw error
      std::cerr << "Element could not be stored \n";
      delete element;
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//
int CellState::find_id (const std::string& name) const
{
  int result = _site_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _binding_site_family_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _site_family_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _chemical_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _reaction_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _bireaction_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _composition_table_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _decoding_table_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _product_table_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  result = _transformation_table_handler.find_id (name);
  if (result != NOT_FOUND) { return result; }

  return NOT_FOUND;
}  


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


// =================
//  Private Methods
// =================
//
