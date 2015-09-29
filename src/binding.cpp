

/**
 * @file binding.cpp
 * @brief Implementation of the Binding class.
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
#include "binding.h"
#include "bindingsite.h"
#include "bindingsitefamily.h"
#include "boundchemical.h"
#include "chemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Binding::Binding ( Chemical& unit_to_bind, BoundChemical& binding_result, BindingSiteFamily& binding_site_family, int binding_site_family_id)
  : Reaction()
  , _unit_to_bind (unit_to_bind)
  , _binding_result (binding_result)
  , _binding_site_family (binding_site_family)
  , _binding_site_family_id (binding_site_family_id)
{
  _reactants.push_back (&unit_to_bind);
  _reactants.push_back (&binding_result);
  _reactants.push_back (&binding_site_family);
}
 
// Not needed for this class (use of default copy constructor) !
// Binding::Binding (Binding& other_Binding);

Binding::~Binding( void )
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Binding::update_rates ( void )
{
  /**
   * Binding rate is generally defined by r = k_on x [A] x [B], where [A] is the concentration
   * of units_to_bind and [B] the concentration of binding sites. However, k_on varies from a
   * site to another so the total binding rate becomes
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * The concentration of units to bind is easy to get, the binding site handler can compute
   * the rest of the formula for us as it holds information about binding rates and binding site
   * concentrations.
   */
  _forward_rate = _unit_to_bind.number() * _binding_site_family.total_binding_rate_contribution();

  /**
   * Unbinding rate is generally defined by r = k_off x [A], where [A] is the concentration
   * of binding_result. However here k_off varies from a binding site to another so it becomes
   * r_total = sum ( r_i ) = sum ( k_off_i x [A_i] )
   * Which is also the dot product between the vector of concentrations of binding_result bound
   * to every single binding sites and the vector of corresponding unbinding rates.
   */
  _backward_rate = _binding_result.get_total_unbinding_rate_contribution (_binding_site_family_id);
}


void Binding::print (std::ostream& output) const
{
  output << "Binding reaction.";
}


// ============================
//  Public Methods - Accessors
// ============================
//
bool Binding::is_forward_reaction_possible (void) const
{
  return (_unit_to_bind.number() > 0);
}

bool Binding::is_backward_reaction_possible (void) const
{
  return (_binding_result.number() > 0);
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
// Binding& Binding::operator= (Binding& other_Binding);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Binding::check_invariant (void) const
{
  bool result = Reaction::check_invariant();
  return result;
}


// ===================
//  Protected Methods
// ===================
//
void Binding::do_forward_reaction (void)
{
  REQUIRE (is_forward_reaction_possible()); /** @pre There is at least one element to bind. */

  // Number of free elements is updated.
  _unit_to_bind.remove (1);
  
  // A binding site in the family is randomly chosen and occupied by a newly created binding result
  const BindingSite& binding_site = _binding_site_family.get_random_available_site();
  _binding_result.add_unit_at_site (binding_site);
  binding_site.location().bind_unit (_binding_result);
}

void Binding::do_backward_reaction (void)
{
  REQUIRE (is_backward_reaction_possible()); /** @pre There is at least one element to unbind. */

  // Number of free elements is updated.
  _unit_to_bind.add (1);

  // A unit bound through a site in the family is randomly chosen
   _binding_result.focus_random_unit (_binding_site_family_id);

  // remove unit
  _binding_result.focused_unit_location().unbind_unit (_binding_result);
  _binding_result.remove_focused_unit();

}
