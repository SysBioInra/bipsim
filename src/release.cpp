

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
#include "producttable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Release::Release (BoundChemical& unit_to_release, std::vector<Chemical*>& other_components,
		  std::vector<int>& stoichiometry, double rate, ProductTable* product_table /*= 0*/)
  : _side_reaction (other_components, stoichiometry, rate, 0)
  , _unit_to_release (unit_to_release)
  , _product_table (product_table)
{
  _reactants.push_back (&_unit_to_release);

  // copy components from side reaction into parent component list
  _reactants.insert (_reactants.end(), _side_reaction.reactants().begin(), _side_reaction.reactants().end());
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
  _side_reaction.update_rates();
  _forward_rate = _unit_to_release.number()*_side_reaction.forward_rate();

  /** Backward reaction is impossible. Nothing to update (it remains always 0) */
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool Release::is_forward_reaction_possible (void) const
{
  return ((_side_reaction.is_forward_reaction_possible())
	  && (_unit_to_release.number() > 0));
}

bool Release::is_backward_reaction_possible (void) const
{
  return false;
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
  bool result = _side_reaction.check_invariant();
  return result;
}

// ===================
//  Protected Methods
// ===================
//
void Release::do_forward_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_forward_reaction_possible());

  _unit_to_release.focus_random_unit();
  _unit_to_release.focused_unit_location().unbind_unit (_unit_to_release);
  if (_product_table != 0)
    {
      ChemicalSequence* product =
	_product_table->product (_unit_to_release.focused_unit_binding_site().location(),
				 _unit_to_release.focused_unit_binding_site().position(),
				 _unit_to_release.focused_unit_position());
      if (product !=0) product->add(1);
      else std::cerr << "Unknown product\n";
    }

  _unit_to_release.remove_focused_unit();

  _side_reaction.perform_forward();
}

void Release::do_backward_reaction (void)
{
  std::cerr << "ERROR: release reaction should not be performed backwards." << std::endl;
}



// =================
//  Private Methods
// =================
//
