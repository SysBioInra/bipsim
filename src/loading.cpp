

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
#include "loading.h"
#include "loader.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Loading::Loading (Loader& loader)
  : _loader (loader)
{
  _reactants.push_back (&loader);
  const std::set<Chemical*> bases = loader.chemicals_loaded();
  _reactants.insert (_reactants.end(), bases.begin(), bases.end());
  const std::set<BoundChemical*> 
    occupied_states = loader.occupied_states();
  _products.insert (_products.end(), occupied_states.begin(), 
		    occupied_states.end());
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
bool Loading::is_reaction_possible (void) const
{
  return (_loader.loading_rate() > 0);
}

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

double Loading::compute_rate (void) const
{
  /**
   * Loading rate is generally defined by r = sum ( k_on_i x [A_i] x [B_i]),
   * where [A_i] is the concentration of Loader on template i and [B_i] the
   * concentration of chemical to load i. k_on_i may vary from a
   * template to another. All this information is stored within the
   * DecodingTable associated with the Loader. The concentration of units to
   * bind is easy to get, the loader can compute the rest of the formula for
   * us as it holds information about templates and base concentrations.
   */
  return _loader.loading_rate();
}


void Loading::print (std::ostream& output) const
{
  output << "Loading reaction.";
}
