

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
ChemicalReaction::ChemicalReaction (const std::vector<FreeChemical*>& chemicals,
				    const std::vector<int>& stoichiometry,
				    const std::vector<int>& orders,
				    double forward_rate_constant,
				    double backward_rate_constant,
				    BoundChemical* forward_bound /*= 0*/,
				    BoundChemical* backward_bound /*= 0*/)
  : _k_1 (forward_rate_constant)
  , _k_m1 (backward_rate_constant)
  , _forward_constant (forward_rate_constant)
  , _backward_constant (backward_rate_constant)    
  , _bound_reactant (forward_bound)
  , _bound_product (backward_bound)
  , _forward_order (0)
  , _backward_order (0)
{

  /** @pre Stoichiometry and order container size must match 
   *   number of chemicals. */
  REQUIRE (stoichiometry.size() == chemicals.size());
  REQUIRE (orders.size() == chemicals.size());
  /** @pre k_1 must be positive. */
  REQUIRE (forward_rate_constant >= 0);
  /** @pre k_-1 must be positive. */
  REQUIRE (backward_rate_constant >= 0);
  /** @pre There must be exactly two bound chemicals or none. */
  REQUIRE (((forward_bound == 0) && (backward_bound == 0))
	   || ((forward_bound != 0) && (backward_bound != 0)));

  // fill in the reactant and stoichiometry vectors with free chemicals
  CRFree new_free;
  for (int i = 0; i < chemicals.size(); ++i)
    {
      /** @pre Stoichiometries must be nonzero. */
      REQUIRE (stoichiometry [i] != 0);
      /** @pre Orders must be positive. */
      REQUIRE (orders [i] >= 0);

      new_free.chemical = chemicals[i];
      new_free.order = orders[i];
      if (stoichiometry [i] < 0)
	{
	  new_free.stoichiometry = -stoichiometry[i];
	  _free_forward.push_back (new_free);
	  _forward_order += orders[i];
	  _forward_reactants.push_back (chemicals[i]);
	}
      else
	{
	  new_free.stoichiometry = stoichiometry[i];
	  _free_backward.push_back (new_free);
	  _backward_order += orders[i];
	  _backward_reactants.push_back (chemicals[i]);
	}
    }

  if (_bound_reactant != 0) 
    { 
      ++_forward_order; 
      _forward_reactants.push_back (_bound_reactant);
      ++_backward_order; 
      _backward_reactants.push_back (_bound_product);
    }
}

// Not needed for this class (use of compiler-generated versions)
// ChemicalReaction::ChemicalReaction (const ChemicalReaction& other);
// ChemicalReaction& ChemicalReaction::operator= (const ChemicalReaction& other);
// ChemicalReaction::~ChemicalReaction (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalReaction::perform_forward (void)
{
  /** @pre There must be enough chemicals left to perform the reaction. */
  REQUIRE (is_forward_reaction_possible() == true);
  
  // update free chemical numbers
  std::vector <CRFree>::iterator it = _free_forward.begin();
  for (; it != _free_forward.end(); ++it)
    { (it->chemical)->remove (it->stoichiometry); }
  for (it = _free_backward.begin(); it != _free_backward.end(); ++it)
    { (it->chemical)->add (it->stoichiometry); }

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
  /** @pre There must be enough chemicals left to perform the reaction. */
  REQUIRE (is_backward_reaction_possible() == true);
  
  // update free chemical numbers
  std::vector <CRFree>::iterator it = _free_forward.begin();
  for (; it != _free_forward.end(); ++it)
  { (it->chemical)->add (it->stoichiometry); }
  for (it = _free_backward.begin(); it != _free_backward.end(); ++it)
  { (it->chemical)->remove (it->stoichiometry); }

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
  for (std::vector <CRFree>::const_iterator 
  	 it = _free_forward.begin(); it != _free_forward.end(); ++it)
    {
      if ((it->chemical)->number() < it->stoichiometry) { return false; }
    }

  if ((_bound_reactant != 0) && (_bound_reactant->number() < 1)) 
    { return false; }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (std::vector <CRFree>::const_iterator 
  	 it = _free_backward.begin(); it != _free_backward.end(); ++it)
    {
      if ((it->chemical)->number() < it->stoichiometry) { return false; }
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
  if (is_forward_reaction_possible() == false) { return 0; }

  double rate = _forward_constant;
  for (std::vector <CRFree>::const_iterator 
  	 it = _free_forward.begin(); it != _free_forward.end(); ++it)
    { 
      rate *= contribution ((it->chemical)->number(), it->order);  
    }

  if (_bound_reactant != 0) { rate *= _bound_reactant->number(); }
  return rate;
}

double ChemicalReaction::compute_backward_rate (void) const
{
  if (is_backward_reaction_possible() == false) { return 0; }

  double rate = _backward_constant;
  for (std::vector <CRFree>::const_iterator 
  	 it = _free_backward.begin(); it != _free_backward.end(); ++it)
    { 
      rate *= contribution ((it->chemical)->number(), it->order);  
    }

  if (_bound_product != 0) { rate *= _bound_product->number(); }
  return rate;
}
