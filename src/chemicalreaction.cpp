

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
				    std::vector<int>& stoichiometry, double forward_rate_constant,
				    double backward_rate_constant)
  : Reaction ()
  , _number_components (components.size())
  , _component_vector (components)
  , _stoichiometry (stoichiometry)
  , _k_1 (forward_rate_constant)
  , _k_m1 (backward_rate_constant)
  , _bound_product_index (_number_components)
  , _bound_reactant_index (_number_components)
{

  /** @pre Stoichiometry container size must match number of components. */
  REQUIRE( _stoichiometry.size() == _number_components );

  // fill in the component list
  for (int i = 0; i < _number_components; i++)
    {
      _components.push_back (_component_vector[i]);
    }

  // look for bound chemicals in the reaction
  compute_bound_component_indices();

  // move them at the end of the vector
  if (_bound_product_index < _number_components)
    {
      // swap contents
      int desired_index = _number_components-1;
      int tmp_stoichiometry = _stoichiometry[desired_index];
      Chemical* tmp_chemical = _component_vector[desired_index];
      _stoichiometry[desired_index] = _stoichiometry [_bound_product_index];
      _component_vector[desired_index] = _component_vector[_bound_product_index];
      _stoichiometry [_bound_product_index] = tmp_stoichiometry;
      _component_vector[_bound_product_index] = tmp_chemical;
      // update indices
      if (_bound_reactant_index == desired_index)
	{
	  _bound_reactant_index = _bound_product_index;
	}
      _bound_product_index = desired_index;
    }
  if (_bound_reactant_index < _number_components)
    {
      // swap contents
      int desired_index = _number_components-2;
      int tmp_stoichiometry = _stoichiometry[desired_index];
      Chemical* tmp_chemical = _component_vector[desired_index];
      _stoichiometry[desired_index] = _stoichiometry [_bound_reactant_index];
      _component_vector[desired_index] = _component_vector[_bound_reactant_index];
      _stoichiometry [_bound_reactant_index] = tmp_stoichiometry;
      _component_vector[_bound_reactant_index] = tmp_chemical;
      // update indices
      _bound_reactant_index = desired_index;
    }
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
  REQUIRE( ChemicalReaction::is_forward_reaction_possible() == true );
  
  // update free chemical numbers
  for (int i = 0; i < _bound_reactant_index; i++)
    {
      int variation = _stoichiometry[i];
      if ( variation > 0 )
	{
	  _component_vector[i]->add (variation);
	}
      else
	{
	  _component_vector[i]->remove (-variation);
	}
    }

  // update bound chemical number (if applicable)
  if (_bound_reactant_index < _number_components)
    {
      BoundChemical* product = static_cast<BoundChemical*> (_component_vector[_bound_product_index]);
      BoundChemical* reactant = static_cast<BoundChemical*> (_component_vector[_bound_reactant_index]);
      reactant->focus_random_unit();
      product->add_unit_in_place_of (*reactant);
      reactant->focused_unit_location().replace_bound_unit (*reactant, *product);      
      reactant->remove_focused_unit();
    }
}

void ChemicalReaction::perform_backward (void)
{
  /** @pre There must be enough components left to perform the reaction. */
  REQUIRE( is_backward_reaction_possible() == true );
  
  for (int i = 0; i < _number_components; i++)
    {
      int variation = _stoichiometry[i];
      if ( variation > 0 )
	{
	  _component_vector[i]->remove (variation);
	}
      else
	{
	  _component_vector[i]->add (-variation);
	}
    }

  // update bound chemical number (if applicable)
  if (_bound_reactant_index < _number_components)
    {
      BoundChemical* product = static_cast<BoundChemical*> (_component_vector[_bound_product_index]);
      BoundChemical* reactant = static_cast<BoundChemical*> (_component_vector[_bound_reactant_index]);
      product->focus_random_unit ();
      reactant->add_unit_in_place_of (*reactant);
      product->focused_unit_location().replace_bound_unit (*product, *reactant);      
      product->remove_focused_unit();
    }
}

void ChemicalReaction::print (std::ostream& output) const
{
  output << "Chemical reaction.";
}

void ChemicalReaction::update_rates (void)
{
  /**
   * Forward rate is simply defined by r = k_1 x product ( [reactant_i] ).
   * It is 0 if there are not enough reactants.
   */
  if (is_forward_reaction_possible() == true)
    {
      _forward_rate = _k_1;
      for (int i = 0; i < _number_components; i++)
	{
	  if ( _stoichiometry[i] < 0 )
	    {
	      _forward_rate *= _component_vector[i]->number();
	    }
	} 
    }
  else // forward reaction is not possible
    {
      _forward_rate = 0;
    }

  /**
   * Backward rate is simply defined by r = k_-1 x product ( [product_i] ).
   * It is 0 if there are not enough reactants.
   */
  if (is_backward_reaction_possible() == true)
    {
      _backward_rate = _k_m1;
      for (int i = 0; i < _number_components; i++)
	{
	  if ( _stoichiometry[i] > 0 )
	    {
	      _backward_rate *= _component_vector[i]->number();
	    }
	}  
    }
  else // backward reaction is not possible
    {
      _backward_rate = 0;
    }
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
// ChemicalReaction& ChemicalReaction::operator= ( const ChemicalReaction& other_chemical_reaction );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ChemicalReaction::check_invariant (void) const
{
  bool result = Reaction::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
bool ChemicalReaction::is_forward_reaction_possible (void) const
{
  for (int i = 0; i < _number_components; i++)
    {
      if (_component_vector[i]->number() < -_stoichiometry[i]) return false;
    }
  return true;
}

bool ChemicalReaction::is_backward_reaction_possible (void) const
{
  for (int i = 0; i < _number_components; i++)
    {
      if (_component_vector[i]->number() < _stoichiometry[i]) return false;
    }
  return true;
}

void ChemicalReaction::compute_bound_component_indices ( void )
{
  _bound_product_index = _number_components;
  _bound_reactant_index = _number_components;
  for (int i = 0; i < _number_components; i++)
    {
      BoundChemical* test = dynamic_cast< BoundChemical* > (_component_vector[i]);
      if ( test != 0 ) // true if a the chemical is a bound chemical !
	{
	  if ( _stoichiometry[i] > 0 ) // it is a product
	    {
	      // check whether a bound product has not already been defined
	      if ( _bound_product_index == _number_components )
		{
		  _bound_product_index = i;
		}
	      else
		{
		  std::cerr << "ERROR: reaction contains 2 or more bound products. "
			    << "Class can only handle one at the moment."
			    << std::endl;
		}
	    }
	  else // it is a reactant
	    {
	      // check whether a bound reactant has not already been defined
	      if ( _bound_reactant_index == _number_components )
		{
		  _bound_reactant_index = i;
		}
	      else
		{
		  std::cerr << "ERROR: reaction contains 2 or more bound reactants. "
			    << "Class can only handle one at the moment."
			    << std::endl;
		}
	    }
	}
    }

  // check that we have a pair of bound reactant AND product
  if ( (_bound_product_index < _number_components) || (_bound_reactant_index < _number_components) )
    {
      if ( (_bound_product_index == _number_components) || (_bound_reactant_index == _number_components) )
	{
	  std::cerr << "ERROR: reaction contains a bound reactant/product without its corresponding "
		    << "bound product/reactant counterpart (a chemical reaction cannot imply binding)."
		    << std::endl;
	}
    }
  
  // check that the stoichiometry is 1
  if ( (_bound_product_index < _number_components) && (_stoichiometry [_bound_product_index] != 1) )
    {
       std::cerr << "ERROR: trying to define a chemical reaction in which the stoichiometry of a bound "
		 << "product is not equal to 1 (" << _stoichiometry [_bound_product_index] << ")."
		 << std::endl;
    }

  if ( (_bound_reactant_index < _number_components) && (_stoichiometry [_bound_reactant_index] != -1) )
    {
       std::cerr <<  "ERROR: trying to define a chemical reaction in which the stoichiometry of a bound "
		 << "reactant is not equal to -1 (" << _stoichiometry [_bound_reactant_index] << ")."
		 << std::endl;
    }
}
