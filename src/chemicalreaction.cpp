

/**
 * @file chemicalreaction.cpp
 * @brief Implementation of the ChemicalReaction class.
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
#include "macros.h" // REQUIRE
#include "chemicalreaction.h"
#include "freechemical.h"
#include "chemicalsequence.h"
#include "boundchemical.h"
#include "boundunit.h"
#include "simulatorexception.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalReaction::ChemicalReaction (const std::vector<FreeChemical*>& components,
				    const std::vector<int>& stoichiometry,
				    double forward_rate_constant,
				    double backward_rate_constant,
				    BoundChemical* forward_bound /*= 0*/,
				    BoundChemical* backward_bound /*= 0*/)

  : _k_1 (forward_rate_constant)
  , _k_m1 (backward_rate_constant)
  , _bound_reactant (forward_bound)
  , _bound_product (backward_bound)
{

  /** @pre Stoichiometry container size must match number of components. */
  REQUIRE (stoichiometry.size() == components.size());
  /** @pre k_1 must be positive. */
  REQUIRE (forward_rate_constant >= 0);
  /** @pre k_-1 must be positive. */
  REQUIRE (backward_rate_constant >= 0);
  /** @pre There must be exactly two bound chemicals or none. */
  REQUIRE (((forward_bound == 0) && (backward_bound == 0))
	   || ((forward_bound != 0) && (backward_bound != 0)));

  // fill in the reactant and stoichiometry vectors with free chemicals
  for (int i = 0; i < components.size(); ++i)
    {
      /** @pre Stoichiometries must be nonzero. */
      REQUIRE (stoichiometry [i] != 0);
      if (stoichiometry [i] < 0)
	{
	  _forward_reactants.push_back (components[i]);
	  _forward_stoichiometry.push_back (-stoichiometry[i]);
	}
      else
	{
	  _backward_reactants.push_back (components[i]);
	  _backward_stoichiometry.push_back (stoichiometry[i]);
	}
    }
  _free_reactant_number = _forward_reactants.size();
  _free_product_number = _backward_reactants.size();

  // handle bound chemicals
  if (forward_bound != 0) 
    { 
      _forward_reactants.push_back (forward_bound); 
      _forward_stoichiometry.push_back (1);
    }
  if (backward_bound != 0) 
    {
      _backward_reactants.push_back (backward_bound); 
      _backward_stoichiometry.push_back (1);
    }
  _forward_reactant_number = _forward_reactants.size();
  _backward_reactant_number = _backward_reactants.size();
}

// Not needed for this class (use of compiler-generated versions)
// ChemicalReaction::ChemicalReaction (const ChemicalReaction& other_reaction);
// ChemicalReaction& ChemicalReaction::operator= (const ChemicalReaction& other_reaction);
// ChemicalReaction::~ChemicalReaction (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalReaction::perform_forward (void)
{
  /** @pre There must be enough components left to perform the reaction. */
  REQUIRE (is_forward_reaction_possible() == true);
  
  // update free chemical numbers
  for (int i = 0; i < _free_reactant_number; i++)
    { forward_chemical (i)->remove (_forward_stoichiometry[i]); }

  for (int i = 0; i < _free_product_number; i++)
    { backward_chemical (i)->add (_backward_stoichiometry[i]); }

  // update bound chemical number (if applicable)
  if (_bound_reactant != 0)
    {
      BoundUnit& unit = _bound_reactant->random_unit();
      _bound_reactant->remove (unit);
      _bound_product->add (unit);
    }
}

void ChemicalReaction::perform_backward (void)
{
  /** @pre There must be enough components left to perform the reaction. */
  REQUIRE (is_backward_reaction_possible() == true);
  
  // update free chemical numbers
  for (int i = 0; i < _free_reactant_number; i++)
    { forward_chemical (i)->add (_forward_stoichiometry[i]); }

  for (int i = 0; i < _free_product_number; i++)
    { backward_chemical (i)->remove (_backward_stoichiometry[i]); }

  // update bound chemical number (if applicable)
  if (_bound_reactant != 0)
    {
      BoundUnit& unit = _bound_product->random_unit();
      _bound_product->remove (unit);
      _bound_reactant->add (unit);
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//
bool ChemicalReaction::is_forward_reaction_possible (void) const
{
  for (int i = 0; i < _forward_reactant_number; i++)
    {
      if (static_cast <const Chemical*> (_forward_reactants [i])->number() 
	  < _forward_stoichiometry[i])
	{ return false; }
    }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (int i = 0; i < _backward_reactant_number; i++)
    {
      if (static_cast <const Chemical*> (_backward_reactants [i])->number() 
	  < _backward_stoichiometry[i])
	{ return false; }
    }
  return true;
}

// =================
//  Private Methods
// =================
//
void ChemicalReaction::print (std::ostream& output) const
{
  output << "Chemical reaction.";
}

double ChemicalReaction::compute_forward_rate (void) const
{
  /**
   * Forward rate is simply defined by r = k_1 x product ( [reactant_i] ).
   * It is 0 if there are not enough reactants.
   */
  double rate = _k_1;
  for (int i = 0; i < _forward_reactant_number; i++)
    {
      if (static_cast <const Chemical*> (_forward_reactants [i])->number() 
	  < _forward_stoichiometry[i])
	{ return 0; }
      rate *= static_cast <const Chemical*> (_forward_reactants [i])->number();
    } 
  return rate;
}

double ChemicalReaction::compute_backward_rate (void) const
{
  /**
   * Backward rate is simply defined by r = k_-1 x product ( [product_i] ).
   * It is 0 if there are not enough reactants.
   */
  double rate = _k_m1;
  for (int i = 0; i < _backward_reactant_number; i++)
    {
      if (static_cast <const Chemical*> (_backward_reactants [i])->number() 
	  < _backward_stoichiometry[i])
	{ return 0; }
      rate *= static_cast <const Chemical*> (_backward_reactants [i])->number();
    }
  return rate;
}
