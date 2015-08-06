

/**
 * @file release.cpp
 * @brief Implementation of the Release class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE
#include "release.h"
#include "chemical.h"
#include "boundchemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Release::Release (BoundChemical& unit_to_release, std::vector<Chemical*>& other_components,
		  std::vector<int>& stoichiometry, double rate)
  : ChemicalReaction (other_components, stoichiometry, rate, 0)
  , _unit_to_release (unit_to_release)
{
  _components.push_back (&_unit_to_release);
}

// Not needed for this class (use of default copy constructor) !
// Release::Release ( const Release& other_release );

Release::~Release (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Release::perform_forward (void)
{
  _unit_to_release.focus_random_unit();

  // update last chemical sequence involved
  _last_chemical_sequence_involved = &_unit_to_release.focused_unit_location();

  _unit_to_release.focused_unit_location().unbind_unit (_unit_to_release);
  _unit_to_release.release();

  ChemicalReaction::perform_forward();
}

void Release::perform_backward (void)
{
  std::cerr << "ERROR: release reaction should not be performed backwards." << std::endl;
}

void Release::print (std::ostream& output) const
{
  output << "Release reaction.";
}

void Release::update_rates (void)
{
  /**
   * Forward rate is simply defined by r = k_1 x product ( [reactant_i] ).
   * It is 0 if there are not enough reactants.
   */
  ChemicalReaction::update_rates();
  _forward_rate = _unit_to_release.number()*ChemicalReaction::forward_rate();

  /** Backward reaction is impossible. Nothing to update (it remains always 0) */
}

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
// Release& Release::operator= ( const Release& other_release );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Release::check_invariant (void) const
{
  bool result = ChemicalReaction::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
bool Release::is_forward_reaction_possible (void) const
{
  return (ChemicalReaction::is_forward_reaction_possible())
    && (_unit_to_release.number() > 0);
}

bool Release::is_backward_reaction_possible (void) const
{
  return false;
}
