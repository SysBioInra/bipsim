

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
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE
#include "chemicalreaction.h"
#include "chemical.h"
#include "boundchemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalReaction::ChemicalReaction (std::vector<Chemical*>& components,
				    std::vector<int>& stoichiometry,
				    double forward_rate_constant,
				    double backward_rate_constant)
  : _k_1 (forward_rate_constant)
  , _k_m1 (backward_rate_constant)
{

  /** @pre Stoichiometry container size must match number of components. */
  REQUIRE (stoichiometry.size() == components.size());
  /** @pre k_1 must be positive. */
  REQUIRE (forward_rate_constant >= 0);
  /** @pre k_-1 must be positive. */
  REQUIRE (backward_rate_constant >= 0);

  // fill in the reactant and stoichiometry vectors
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

  // look for bound chemicals in the reaction
  isolate_bound_components();

  /** @post Total number of components must be conserved. */
  ENSURE (_forward_reactants.size() + _backward_reactants.size()
	  == components.size());

  /** @post There are either no bound chemical in the reaction or one on each
   *   side of the reaction. */
  ENSURE (((_bound_reactant == 0) && (_bound_product == 0))
	  || ((_bound_reactant != 0) && (_bound_product != 0)));

  /** @post Sum of free and bound reactants must match total number of
   *   reactants */
  ENSURE (_free_reactant_number == 
	  _forward_reactants.size() - (_bound_reactant != 0));

  /** @post Sum of free and bound products must match total number of
   *   products */
  ENSURE (_free_product_number == 
	  _backward_reactants.size() - (_bound_product != 0));
}

// Not needed for this class (use of default copy constructor) !
// ChemicalReaction::ChemicalReaction ( const ChemicalReaction& other_chemical_reaction );

ChemicalReaction::~ChemicalReaction (void)
{
}

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
      _bound_reactant->focus_random_unit();
      _bound_product->add_unit_in_place_of (*_bound_reactant);
      _bound_reactant->focused_unit_location().replace_bound_unit (*_bound_reactant, *_bound_product);      
      _bound_reactant->remove_focused_unit();
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
      _bound_product->focus_random_unit ();
      _bound_reactant->add_unit_in_place_of (*_bound_product);
      _bound_product->focused_unit_location().replace_bound_unit (*_bound_product, *_bound_reactant);      
      _bound_product->remove_focused_unit();
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//
bool ChemicalReaction::is_forward_reaction_possible (void) const
{
  for (int i = 0; i < forward_reactants().size(); i++)
    {
      if (forward_chemical(i)->number() < _forward_stoichiometry[i])
	{ return false; }
    }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (int i = 0; i < backward_reactants().size(); i++)
    {
      if (backward_chemical(i)->number() < _backward_stoichiometry[i])
	{ return false; }
    }
  return true;
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
// ChemicalReaction& ChemicalReaction::operator= ( const ChemicalReaction& other_chemical_reaction );


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
  for (int i = 0; i < _forward_reactants.size(); i++)
    {
      if (forward_chemical(i)->number() < _forward_stoichiometry[i])
	{ return 0; }
      rate *= forward_chemical(i)->number();
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
  for (int i = 0; i < _backward_reactants.size(); i++)
    {
      if (backward_chemical(i)->number() < _backward_stoichiometry[i])
	{ return 0; }
      rate *= backward_chemical(i)->number();
    }
  return rate;
}


void ChemicalReaction::isolate_bound_components (void)
{
  // look for a bound reactant
  _bound_reactant = 0;
  int bound_reactant_index = 0;
  for (int i = 0; i < _forward_reactants.size(); i++)
    {
      BoundChemical* test
	= dynamic_cast <BoundChemical*> (_forward_reactants[i]);
      if (test != 0) // true if chemical is a bound chemical
	{
	  // check whether a bound product has not already been defined
	  if (_bound_reactant == 0)
	    {
	      _bound_reactant = test;
	      bound_reactant_index = i;
	    }
	  else
	    {
	      std::cerr << "ERROR: reaction contains 2 or more bound reactants."
			<< " Class can only handle one at the moment."
			<< std::endl;
	      REQUIRE (false); // TODO throw error
	    }
	}
    }
      
  // look for a bound product
  _bound_product = 0;
  int bound_product_index = 0;
 for (int i = 0; i < _backward_reactants.size(); i++)
    {
      BoundChemical* test
	= dynamic_cast <BoundChemical*> (_backward_reactants[i]);
      if (test != 0) // true if chemical is a bound chemical
	{
	  // check whether a bound reactant has not already been defined
	  if (_bound_product == 0)
	    {
	      _bound_product = test;
	      bound_product_index = i;
	    }
	  else
	    {
	      std::cerr << "ERROR: reaction contains 2 or more bound products."
			<< " Class can only handle one at the moment."
			<< std::endl;
	      REQUIRE (false); // TODO throw error
	    }
	}
    }
 
 // check that we have a pair of bound reactant AND product
 if (((_bound_reactant == 0) && (_bound_product != 0))
     || ((_bound_reactant != 0) && (_bound_product == 0)))
   {
     std::cerr << "ERROR: reaction contains a bound reactant/product without"
	       << " its corresponding bound product/reactant counterpart (a"
	       << " chemical reaction cannot imply binding)."
	       << std::endl;
     REQUIRE (false); // TODO throw error
   }

 // check that the stoichiometry is 1
 if ((_bound_reactant != 0)
     && (_forward_stoichiometry [bound_reactant_index] != 1))
    {
      std::cerr << "ERROR: trying to define a chemical reaction in which the"
		<< " stoichiometry of a bound reactant is not equal to 1 ("
		<< _forward_stoichiometry [bound_reactant_index] << ")."
		<< std::endl;
    }
 if ((_bound_product != 0)
     && (_backward_stoichiometry [bound_product_index] != 1))
    {
      std::cerr << "ERROR: trying to define a chemical reaction in which the"
		<< " stoichiometry of a bound product is not equal to 1 ("
		<< _backward_stoichiometry [bound_product_index] << ")."
		<< std::endl;
    }

 // move bound reactant at the end of reactants if applicable
 if (_bound_reactant == 0)
   {
     _free_reactant_number = _forward_reactants.size();
   }
 else
   {
     int s = _forward_reactants.size();
     _free_reactant_number = s-1;
     _forward_reactants [bound_reactant_index] = _forward_reactants [s-1];
     _forward_stoichiometry [bound_reactant_index] =
       _forward_stoichiometry [s-1];
     _forward_reactants [s-1] = _bound_reactant;
     _forward_stoichiometry [s-1] = 1;
   }

 // move bound product at the end of products if applicable
 if (_bound_product == 0)
   {
     _free_product_number = _backward_reactants.size();
   }
 else
   {
     int s = _backward_reactants.size();
     _free_product_number = s-1;
     _backward_reactants [bound_product_index] = _backward_reactants [s-1];
     _backward_stoichiometry [bound_product_index] =
       _backward_stoichiometry [s-1];
     _backward_reactants [s-1] = _bound_product;
     _backward_stoichiometry [s-1] = 1;
   }
}
