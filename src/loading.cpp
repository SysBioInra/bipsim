

/**
 * @file loading.cpp
 * @brief Implementation of the Loading class.
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
#include "macros.h" // REQUIRE()
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "loading.h"
#include "loader.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Loading::Loading (Loader& loader)
  : LoadingBase (loader)
{
}
 
// Not needed for this class (use of compiler generated versions)
// Loading::Loading (Loading& other_loading);
// Loading& Loading::operator= (Loading& other_loading);
// Loading::~Loading (void);

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
void Loading::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  // Choose one of the loaders randomly
  _loader.focus_random_unit_from_loading_rates();
  
  // Load the element corresponding to its target template
  _loader.focused_chemical_to_load().remove (1);
  // currently the loaded chemical just disappears into thin air. It will show
  // up eventually when the elongated chemical is released so let us say it does
  // virtually exist :)
  
  // Update the loader status to occupied
  BoundChemical& occupied_loader = _loader.focused_occupied_state ();
  occupied_loader.add_unit_in_place_of (_loader);
  _loader.focused_unit_location().replace_bound_unit (_loader, occupied_loader);      
  _loader.remove_focused_unit();
}

void Loading::print (std::ostream& output) const
{
  output << "Loading";
}
