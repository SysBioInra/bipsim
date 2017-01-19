

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
  : _base_volume (1)
{}

// Not needed for this class (use of compiler-generated versions)
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
	   || _switch_handler.store (element, name)
	   || _chemical_handler.store (element, name)
	   || _reaction_handler.store (element, name)
	   || _bireaction_handler.store (element, name)
	   || _composition_table_handler.store (element, name)
	   || _loading_table_handler.store (element, name)
	   || _product_table_handler.store (element, name)
	   || _transformation_table_handler.store (element, name)))
    {
      // TODO throw error
      std::cerr << "Element could not be stored \n";
      delete element;
    }
}

void CellState::
set_volume_parameters (double base_volume, 
		       const std::vector <std::string>& modifiers,
		       const std::vector <double>& modifier_weights)
{
  /** @pre Length of modifiers and weights must match. */
  REQUIRE (modifiers.size() == modifier_weights.size());
  _base_volume = base_volume;
  _volume_weights = modifier_weights;
  _volume_modifiers.clear();
  for (std::vector <std::string>::const_iterator it = modifiers.begin();
       it != modifiers.end(); ++it)
    { _volume_modifiers.push_back (&(fetch <Chemical> (*it))); }
  modify_volume();
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void CellState::modify_volume (void)
{
  double volume = _base_volume;
  for (int i = 0; i < _volume_modifiers.size(); ++i)
    { volume += _volume_weights[i] * _volume_modifiers[i]->number(); }
  /** @post Result must be positive. */
  ENSURE (volume > 0);
  for (std::vector <Reaction*>::const_iterator it = reactions().begin();
       it != reactions().end(); ++it)
    { (*it)->handle_volume_change (volume); }
}
