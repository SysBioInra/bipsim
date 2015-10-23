

/**
 * @file complexation.cpp
 * @brief Implementation of the Complexation class.
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
#include "chemical.h"
#include "boundchemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Complexation::Complexation (Chemical& component_a, Chemical& component_b,
			    Chemical& complex, double k_on, double k_off)
  : _component_a (component_a)
  , _component_b (component_b)
  , _complex (complex)
  , _k_on (k_on)
  , _k_off (k_off)
  , _bound_component (0)
{
  /** @pre k_on must be positive. */
  REQUIRE (k_on >= 0);
  /** @pre k_off must be positive. */
  REQUIRE (k_off >= 0);

  // fill in component list
  _forward_reactants.push_back (&component_a);
  _forward_reactants.push_back (&component_b);
  _backward_reactants.push_back (&complex);

  look_for_bound_components();
}

// Not needed for this class (use of default copy constructor) !
// Complexation::Complexation ( const Complexation& other_complexation );

Complexation::~Complexation (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Complexation::perform_forward (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_forward_reaction_possible());

  if (_bound_component == 0)
    {
      _component_a.remove (1);
      _component_b.remove (1);
      _complex.add (1);
    }
  else
    {
      BoundChemical* complex = static_cast<BoundChemical*> (&_complex);
      BoundChemical* bound_component = 0;
      if (_bound_component == 1)
	{
	  bound_component = static_cast<BoundChemical*> (&_component_a);
	  _component_b.remove (1);
	}
      else 
	{
	  bound_component = static_cast<BoundChemical*> (&_component_b);
	  _component_a.remove (1);
	}
      bound_component->focus_random_unit();
      complex->add_unit_in_place_of (*bound_component);
      bound_component->focused_unit_location().replace_bound_unit (*bound_component, *complex);      
      bound_component->remove_focused_unit();
    }
}

void Complexation::perform_backward (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_backward_reaction_possible());

  if (_bound_component == 0)
    {
      _component_a.add (1);
      _component_b.add (1);
      _complex.remove (1);
    }
  else
    {
      BoundChemical* complex = static_cast<BoundChemical*> (&_complex);
      BoundChemical* bound_component = 0;
      if (_bound_component == 1)
	{
	  bound_component = static_cast<BoundChemical*> (&_component_a);
	  _component_b.add (1);
	}
      else 
	{
	  bound_component = static_cast<BoundChemical*> (&_component_b);
	  _component_a.add (1);
	}
      complex->focus_random_unit();
      bound_component->add_unit_in_place_of (*complex);
      complex->focused_unit_location().replace_bound_unit (*complex, *bound_component);      
      complex->remove_focused_unit();
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool Complexation::is_forward_reaction_possible (void) const
{
  return ((_component_a.number() > 0) && (_component_b.number() > 0));
}

bool Complexation::is_backward_reaction_possible (void) const
{
  return (_complex.number() > 0);
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
// Complexation& Complexation::operator= ( const Complexation& other_complexation );


// ===================
//  Protected Methods
// ===================
//

// =================
//  Private Methods
// =================
//
double Complexation::compute_forward_rate (void) const
{
  /**
   * Complexation rate is simply defined by r = k_on x [A] x [B].
   */
  return _k_on * _component_a.number() * _component_b.number();
}

double Complexation::compute_backward_rate (void) const
{
  /**
   * Backward complexation rate is simply defined by r = k_off x [AB].
   */
  return _k_off * _complex.number();
}

void Complexation::print (std::ostream& output) const
{
  output << "Complexation reaction.";
}

void Complexation::look_for_bound_components (void)
{
  _bound_component = 0;

  // test component a
  BoundChemical* test = dynamic_cast< BoundChemical* > (&_component_a);
  if (test != 0) { _bound_component = 1; }

  // test component b
  test = dynamic_cast< BoundChemical* > (&_component_b);
  if (test != 0)
    {
      if (_bound_component == 0) { _bound_component = 2; }
      else
	{
	  std::cerr << "ERROR: Trying to define a complexation reaction with 2"
		    << " bound components. Program can currently only handle"
		    << " one."
		    << std::endl;
	}
    }

  // test complex
  test = dynamic_cast< BoundChemical* > (&_complex);
  if ( (test == 0) && (_bound_component != 0)
       || (test != 0) && (_bound_component == 0) )
    {
      std::cerr << "ERROR: Trying to define a complexation with a bound"
		<< " component/complex without corresponding bound"
		<< " complex/component on the other side."
		<< std::endl;
    }      
}
