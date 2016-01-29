

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

// Not needed for this class (use of compiler-generated versions)
// CellState::CellState (void);
// CellState::CellState (const CellState& other_cell_state);
// CellState& CellState::operator= (const CellState& other_cell_state);
// CellState::~CellState (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void CellState::store (SimulatorInput* element,
		       const std::string& name /*= ""*/)
{
  // try to store element
  if (not (_site_handler.store (element, name)
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

// =================
//  Private Methods
// =================
//
