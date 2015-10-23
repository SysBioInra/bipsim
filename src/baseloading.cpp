

/**
 * @file binding.cpp
 * @brief Implementation of the BaseLoading class.
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
#include "baseloading.h"
#include "baseloader.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BaseLoading::BaseLoading (BaseLoader& base_loader)
  : _base_loader (base_loader)
{
  _reactants.push_back (&base_loader);
  const std::set<Chemical*> bases = base_loader.bases_loaded();
  _reactants.insert (_reactants.end(), bases.begin(), bases.end());
  const std::set<BoundChemical*> 
    occupied_states = base_loader.occupied_states();
  _products.insert (_products.end(), occupied_states.begin(), occupied_states.end());
}
 
// Not needed for this class (use of default copy constructor) !
// BaseLoading::BaseLoading (BaseLoading& other_base_loading);

BaseLoading::~BaseLoading (void)
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
bool BaseLoading::is_reaction_possible (void) const
{
  return (_base_loader.loading_rate() > 0);
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
// BaseLoading& BaseLoading::operator= (BaseLoading& other_base_loading);


// ===================
//  Protected Methods
// ===================
//
void BaseLoading::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  // Choose one of the base loaders randomly
  _base_loader.focus_random_unit_from_loading_rates();
  
  // Load the element corresponding to its target template
  _base_loader.focused_base_to_load().remove (1);
  // currently the base just disappears into thin air. It will show up eventually
  // when the elongated chemical is released so let us say it does virtually exist :)
  
  // Update the base loader status to occupied
  BoundChemical& occupied_loader = _base_loader.focused_occupied_state ();
  occupied_loader.add_unit_in_place_of (_base_loader);
  _base_loader.focused_unit_location().replace_bound_unit (_base_loader, occupied_loader);      
  _base_loader.remove_focused_unit();
}

double BaseLoading::compute_rate (void) const
{
  /**
   * Loading rate is generally defined by r = sum ( k_on_i x [A_i] x [B_i], where [A_i] is the
   * concentration of BaseLoader on template i and [B_i] the concentration of base i. k_on_i may vary from a
   * base to another. All this information is stored within the 
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * The concentration of units to bind is easy to get, the base loader handler can compute
   * the rest of the formula for us as it holds information about templates and base
   * concentrations.
   */
  return _base_loader.loading_rate();
}


void BaseLoading::print (std::ostream& output) const
{
  output << "BaseLoading reaction.";
}
