

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
  CRParticipant current_reactant;
  for (int i = 0; i < components.size(); ++i)
    {
      /** @pre Stoichiometries must be nonzero. */
      REQUIRE (stoichiometry [i] != 0);
      current_reactant.chemical = components[i];
      if (stoichiometry [i] < 0)
	{
	  current_reactant.stoichiometry = -stoichiometry[i];
	  _forward_reactants.push_back (components[i]);
	  _free_reactants.push_back (current_reactant);
	}
      else
	{
	  current_reactant.stoichiometry = stoichiometry[i];
	  _backward_reactants.push_back (components[i]);
	  _free_products.push_back (current_reactant);
	}
    }

  // handle bound chemicals
  if (forward_bound != 0) 
    { _forward_reactants.push_back (forward_bound); }
  if (backward_bound != 0) 
    { _backward_reactants.push_back (backward_bound); }
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
  std::vector <CRParticipant>::iterator reactant_it;
  for (reactant_it = _free_reactants.begin(); 
       reactant_it != _free_reactants.end(); ++reactant_it)
    { (reactant_it->chemical)->remove (reactant_it->stoichiometry); }

  for (reactant_it = _free_products.begin(); 
       reactant_it != _free_products.end(); ++reactant_it)
    { (reactant_it->chemical)->add (reactant_it->stoichiometry); }

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
  std::vector <CRParticipant>::iterator reactant_it;
  for (reactant_it = _free_reactants.begin(); 
       reactant_it != _free_reactants.end(); ++reactant_it)
    { (reactant_it->chemical)->add (reactant_it->stoichiometry); }

  for (reactant_it = _free_products.begin(); 
       reactant_it != _free_products.end(); ++reactant_it)
    { (reactant_it->chemical)->remove (reactant_it->stoichiometry); }

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
  for (std::vector <CRParticipant>::const_iterator 
	 reactant_it = _free_reactants.begin(); 
       reactant_it != _free_reactants.end(); ++reactant_it)
    {
      if ((reactant_it->chemical)->number() < reactant_it->stoichiometry)
	{ return false; }
    }
  if ((_bound_reactant != 0) && (_bound_reactant->number() < 1)) 
    { return false; }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (std::vector <CRParticipant>::const_iterator 
	 reactant_it = _free_products.begin(); 
       reactant_it != _free_products.end(); ++reactant_it)
    {
      if ((reactant_it->chemical)->number() < reactant_it->stoichiometry)
	{ return false; }
    }
  if ((_bound_product != 0) && (_bound_product->number() < 1)) 
    { return false; }
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
  for (std::vector <CRParticipant>::const_iterator 
	 reactant_it = _free_reactants.begin(); 
       reactant_it != _free_reactants.end(); ++reactant_it)
    {
      if ((reactant_it->chemical)->number() < reactant_it->stoichiometry)
	{ return 0; }
      rate *= (reactant_it->chemical)->number();
    }
  if (_bound_reactant != 0) { rate *= _bound_reactant->number(); }
  return rate;
}

double ChemicalReaction::compute_backward_rate (void) const
{
  /**
   * Backward rate is simply defined by r = k_-1 x product ( [product_i] ).
   * It is 0 if there are not enough reactants.
   */
  double rate = _k_m1;
  for (std::vector <CRParticipant>::const_iterator 
	 reactant_it = _free_products.begin(); 
       reactant_it != _free_products.end(); ++reactant_it)
    {
      if ((reactant_it->chemical)->number() < reactant_it->stoichiometry)
	{ return 0; }
      rate *= (reactant_it->chemical)->number();
    }
  if (_bound_product != 0) { rate *= _bound_product->number(); }
  return rate;
}
