
/**
 * @file chemicalreaction.cpp
 * @brief Implementation of the ChemicalReaction class.
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
#include "boundunitfactory.h"
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
  : _forward_bound (forward_bound)
  , _backward_bound (backward_bound)
  , _k_1 (forward_rate_constant)
  , _k_m1 (backward_rate_constant)
  , _forward_constant (forward_rate_constant)
  , _backward_constant (backward_rate_constant)    
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
  /** @pre There can only be a bound product if there is a bound reactant. */
  REQUIRE ((is_forward_reaction_valid() || (_forward_constant == 0))
	   &&
	   (is_backward_reaction_valid() || (_backward_constant == 0)));
  
  // fill in reactant and stoichiometry vectors with free chemicals
  CRFree new_free;
  for (std::size_t i = 0; i < chemicals.size(); ++i)
    {
      /** @pre Orders must be positive. */
      REQUIRE (orders [i] >= 0);
      // ignore chemicals with zero stoichiometry
      if (stoichiometry[i] == 0) { continue; }
      
      new_free.chemical = chemicals[i];
      new_free.order = orders[i];
      if (stoichiometry [i] < 0)
	{
	  new_free.stoichiometry = -stoichiometry[i];
	  _free_forward.push_back (new_free);
	  _forward_order += orders[i];
	  if (orders[i] != 0)
	    { _forward_reactants.push_back (chemicals[i]); }
	}
      else
	{
	  new_free.stoichiometry = stoichiometry[i];
	  _free_backward.push_back (new_free);
	  _backward_order += orders[i];
	  if (orders[i] != 0)
	    { _backward_reactants.push_back (chemicals[i]); }
	}
    }

  if (_forward_bound != 0) 
    { 
      ++_forward_order; 
      _forward_reactants.push_back (_forward_bound);
      ++_backward_order; 
      _backward_reactants.push_back (_backward_bound);
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
  // if there are not enough reactants, do nothing
  if (!is_forward_reaction_possible()) { return; }
  
  // update free chemical numbers
  std::vector <CRFree>::iterator it = _free_forward.begin();
  for (; it != _free_forward.end(); ++it)
    { (it->chemical)->remove (it->stoichiometry); }
  for (it = _free_backward.begin(); it != _free_backward.end(); ++it)
    { (it->chemical)->add (it->stoichiometry); }

  // update bound chemical number (if applicable)
  if (_forward_bound != 0)
    {
      BoundUnit& unit = _forward_bound->random_unit();
      _forward_bound->remove (unit);
      if (_backward_bound != 0) { _backward_bound->add (unit); }
      else
	{
	  unit.location().unbind_unit (unit.first(), unit.last());
	  BoundUnitFactory::instance().free(unit);
	}
    }
}

void ChemicalReaction::perform_backward (void)
{
  // if there are not enough reactants, do nothing
  if (!is_backward_reaction_possible()) { return; }
  
  // update free chemical numbers
  std::vector <CRFree>::iterator it = _free_forward.begin();
  for (; it != _free_forward.end(); ++it)
  { (it->chemical)->add (it->stoichiometry); }
  for (it = _free_backward.begin(); it != _free_backward.end(); ++it)
  { (it->chemical)->remove (it->stoichiometry); }

  // update bound chemical number (if applicable)
  if (_forward_bound != 0)
    {
      BoundUnit& unit = _backward_bound->random_unit();
      _backward_bound->remove (unit);
      if (_forward_bound != 0) { _forward_bound->add (unit); }
      else
	{
	  unit.location().unbind_unit (unit.first(), unit.last());
	  BoundUnitFactory::instance().free(unit);
	}
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
    { if ((it->chemical)->number() < it->stoichiometry) { return false; } }

  if ((_forward_bound != 0) && (_forward_bound->number() < 1))
    { return false; }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (std::vector <CRFree>::const_iterator 
  	 it = _free_backward.begin(); it != _free_backward.end(); ++it)
    { if ((it->chemical)->number() < it->stoichiometry) { return false; } }

  if ((_backward_bound != 0) && (_backward_bound->number() < 1)) 
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

  if (_forward_bound != 0) { rate *= _forward_bound->number(); }
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

  if (_backward_bound != 0) { rate *= _backward_bound->number(); }
  return rate;
}
