

/**
 * @file pairedloading.cpp
 * @brief Implementation of the PairedLoading class.
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
#include "pairedloading.h"
#include "loader.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
PairedLoading::PairedLoading (Loader& loader)
  : LoadingBase (loader)
{}

// Not needed for this class (use of compiler-generated versions)
// PairedLoading::PairedLoading (const PairedLoading& other);
// PairedLoading& PairedLoading::operator= (const PairedLoading& other);
// PairedLoading::~PairedLoading (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//


// ===================
//  Protected Methods
// ===================
//
void PairedLoading::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  // Choose one of the loaders randomly
  _loader.focus_random_unit_from_loading_rates();
  
  // Load the element corresponding to its target template
  _loader.focused_chemical_to_load().remove (1);
  
  // Extend strand
  _loader.focused_unit_location().
    extend_strand (_loader.focused_unit_reading_frame());
  
  // Update the loader status to occupied
  BoundChemical& occupied_loader = _loader.focused_occupied_state();
  occupied_loader.add_unit_in_place_of (_loader);
  _loader.focused_unit_location().
    replace_bound_unit (_loader, occupied_loader);      
  _loader.remove_focused_unit();
}

void PairedLoading::print (std::ostream& output) const
{
  output << "Paired Loading";
}

