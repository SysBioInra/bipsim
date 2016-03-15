

/**
 * @file sequencebinding.cpp
 * @brief Implementation of the SequenceBinding class.
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
#include "sequencebinding.h"
#include "bindingsite.h"
#include "bindingsitefamily.h"
#include "freechemical.h"
#include "boundchemical.h"
#include "boundunit.h"
#include "boundunitfactory.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceBinding::SequenceBinding (FreeChemical& unit_to_bind,
				  BoundChemical& binding_result,
				  BindingSiteFamily& binding_site_family)
  : _unit_to_bind (unit_to_bind)
  , _binding_result (binding_result)
  , _family (binding_site_family)
  , _family_filter (_family)
{
  _forward_reactants.push_back (&unit_to_bind);
  _forward_reactants.push_back (&binding_site_family);
  _backward_reactants.push_back (&binding_result);
  _backward_reactants.push_back (&binding_site_family);

  _binding_result.add_filter (_family_filter);
}
 
// Forbidden
// SequenceBinding::SequenceBinding (SequenceBinding& other_binding);
// SequenceBinding& SequenceBinding::operator= (SequenceBinding& other_binding);

SequenceBinding::~SequenceBinding (void)
{
  _binding_result.remove_filter (_family_filter);
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SequenceBinding::perform_forward (void)
{
  /** @pre There is at least one element to bind. */
  REQUIRE (is_forward_reaction_possible()); 

  // Number of free elements is updated.
  _unit_to_bind.remove (1);
  
  // A binding site in the family is randomly chosen and occupied by a newly
  // created binding result
  const BindingSite& site = _family.random_available_site();
  _binding_result.add (BoundUnitFactory::instance().create (site));
  site.location().bind_unit (site.first(), site.last());
}

void SequenceBinding::perform_backward (void)
{
  /** @pre There is at least one element to unbind. */
  REQUIRE (is_backward_reaction_possible()); 

  // Number of free elements is updated.
  _unit_to_bind.add (1);

  // A unit bound through a site in the family is randomly chosen
  BoundUnit& unit = _family_filter.random_unit();

  // remove unit
  unit.location().unbind_unit (unit.first(), unit.last());
  _binding_result.remove (unit);
  BoundUnitFactory::instance().free (unit);
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool SequenceBinding::is_forward_reaction_possible (void) const
{
  return ((_unit_to_bind.number() > 0) && (_family.is_site_available()));
}

bool SequenceBinding::is_backward_reaction_possible (void) const
{
  return (_family_filter.number() > 0);
}

// =================
//  Private Methods
// =================
//
double SequenceBinding::compute_forward_rate (void) const
{
  /**
   * SequenceBinding rate is generally defined by r = k_on x [A] x [B],
   * where [A] is the concentration of units_to_bind and [B] the concentration
   * of binding sites. However, k_on varies from a site to another so the total
   * binding rate becomes
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * The concentration of units to bind is easy to get, the binding site
   * family can compute the rest of the formula for us as it holds information
   * about binding rates and binding site concentrations.
   */
  return _unit_to_bind.number() * _family.total_binding_rate();
}

double SequenceBinding::compute_backward_rate (void) const
{
  /**
   * Unbinding rate is generally defined by r = k_off x [A], where [A] is the 
   * concentration of binding_result. However here k_off varies from a binding
   * site to another so it becomes
   *         r_total = sum ( r_i ) = sum ( k_off_i x [A_i] )
   * Which is also the dot product between the vector of concentrations of 
   * binding_result bound to every single binding sites and the vector of 
   * corresponding unbinding rates.
   */
  return _family_filter.total_unbinding_rate();
}


void SequenceBinding::print (std::ostream& output) const
{
  output << "SequenceBinding reaction.";
}

