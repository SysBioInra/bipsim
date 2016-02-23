

/**
 * @file complexation.cpp
 * @brief Implementation of the Complexation class and classes
 *  implementating the ComplexationImp class.
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
#include "complexation.h"
#include "complexationimp.h"
#include "chemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "simulatorexception.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Complexation::Complexation (Chemical& component_a, Chemical& component_b,
			    Chemical& complex, double k_on, double k_off)
  : _k_on (k_on)
  , _k_off (k_off)
{
  /** @pre k_on must be positive. */
  REQUIRE (k_on >= 0);
  /** @pre k_off must be positive. */
  REQUIRE (k_off >= 0);

  // fill in component list
  _forward_reactants.push_back (&component_a);
  _forward_reactants.push_back (&component_b);
  _backward_reactants.push_back (&complex);

  // create implementation based on the presence of bound elements
  BoundChemical* test_a = dynamic_cast <BoundChemical*> (&component_a);
  BoundChemical* test_b = dynamic_cast <BoundChemical*> (&component_b);
  BoundChemical* test_complex = dynamic_cast <BoundChemical*> (&complex);

  // case where there is no bound chemical
  if ((test_a == 0) && (test_b == 0) && (test_complex == 0))
    { _imp = new FreeComplexationImp (component_a, component_b, complex); }
  else
    {
      // else we expect that the complex and exactly one of the 
      // two components are bound
      if ((test_complex == 0) || ((test_a == 0) && (test_b == 0)))
	{
	  throw ParserException ("Trying to define a complexation with"
				 " a bound component/complex without"
				 " corresponding bound complex/component on"
				 " the other side");
	}
      if ((test_a != 0) && (test_b != 0))
	{
	  throw ParserException ("Trying to define a complexation "
				 "reaction with 2 bound components. Program"
				 " can currently only handle one");
	}
      
      if (test_a != 0) 
	{ 
	  _imp = new BoundComplexationImp (*test_a, component_b, *test_complex);
	}
      else
	{
	  _imp = new BoundComplexationImp (*test_b, component_a, *test_complex);
	}
    }
}

FreeComplexationImp::FreeComplexationImp (Chemical& component_a, 
					  Chemical& component_b, 
					  Chemical& complex)
  : ComplexationImp (component_a, component_b, complex)
{
  /** @pre component_A must not be a BoundChemical. */
  REQUIRE (dynamic_cast <BoundChemical*> (&component_a) == 0);
  /** @pre component_B must not be a BoundChemical. */
  REQUIRE (dynamic_cast <BoundChemical*> (&component_b) == 0);
  /** @pre complex must not be a BoundChemical. */
  REQUIRE (dynamic_cast <BoundChemical*> (&complex) == 0);
}

BoundComplexationImp::BoundComplexationImp (BoundChemical& component_a, 
					    Chemical& component_b,
					    BoundChemical& complex)
  : ComplexationImp (component_a, component_b, complex)
{
  /** @pre component_B must not be a BoundChemical. */
  REQUIRE (dynamic_cast <BoundChemical*> (&component_b) == 0);
}

// Forbidden
// Complexation::Complexation (const Complexation& other_complexation);
// Complexation& Complexation::operator= (const Complexation& other_complexation);

Complexation::~Complexation (void)
{
  delete _imp;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Complexation::perform_forward (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_forward_reaction_possible());

  _imp->perform_forward();
}

void FreeComplexationImp::perform_forward (void)
{
  component_a().remove (1);
  component_b().remove (1);
  complex().add (1);
}

void BoundComplexationImp::perform_forward (void)
{
  _free_component().remove (1);
  _bound_component().focus_random_unit();
  _bound_complex().add_unit_in_place_of (_bound_component());
  _bound_component().focused_unit_location().
    replace_bound_unit (_bound_component(), _bound_complex());
  _bound_component().remove_focused_unit();
}

void Complexation::perform_backward (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_backward_reaction_possible());

  _imp->perform_backward();
}

void FreeComplexationImp::perform_backward (void)
{
  component_a().add (1);
  component_b().add (1);
  complex().remove (1);
}

void BoundComplexationImp::perform_backward (void)
{
  _free_component().add (1);
  _bound_complex().focus_random_unit();
  _bound_component().add_unit_in_place_of (_bound_complex());
  _bound_complex().focused_unit_location().
    replace_bound_unit (_bound_complex(), _bound_component());
  _bound_complex().remove_focused_unit();
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool Complexation::is_forward_reaction_possible (void) const
{
  return ((_imp->component_a().number() > 0) 
	  && (_imp->component_b().number() > 0));
}

bool Complexation::is_backward_reaction_possible (void) const
{
  return (_imp->complex().number() > 0);
}

// =================
//  Private Methods
// =================
//
double Complexation::compute_forward_rate (void) const
{
  /**
   * Complexation rate is simply defined by r = k_on x [A] x [B].
   */
  return _k_on * _imp->component_a().number() * _imp->component_b().number();
}

double Complexation::compute_backward_rate (void) const
{
  /**
   * Backward complexation rate is simply defined by r = k_off x [AB].
   */
  return _k_off * _imp->complex().number();
}

void Complexation::print (std::ostream& output) const
{
  output << "Complexation reaction.";
}
