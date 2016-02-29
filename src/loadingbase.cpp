

/**
 * @file loadingbase.cpp
 * @brief Implementation of the LoadingBase class.
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
#include "loadingbase.h"
#include "loader.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
LoadingBase::LoadingBase (Loader& loader)
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
// LoadingBase::LoadingBase (LoadingBase& other_loading);
// LoadingBase& LoadingBase::operator= (LoadingBase& other_loading);
// LoadingBase::~LoadingBase (void);

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//
bool LoadingBase::is_reaction_possible (void) const
{
  return (_loader.loading_rate() > 0);
}

double LoadingBase::compute_rate (void) const
{
  /**
   * LoadingBase rate is generally defined by r = sum ( k_on_i x [A_i] x [B_i]),
   * where [A_i] is the concentration of Loader on template i and [B_i] the
   * concentration of chemical to load i. k_on_i may vary from a
   * template to another. All this information is stored within the
   * DecodingTable associated with the Loader. The concentration of units to
   * bind is easy to get, the loader can compute the rest of the formula for
   * us as it holds information about templates and base concentrations.
   */
  return _loader.loading_rate();
}

