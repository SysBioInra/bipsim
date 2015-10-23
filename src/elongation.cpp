

/**
 * @file elongation.cpp
 * @brief Implementation of the Elongation class.
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
#include "elongation.h"
#include "processivechemical.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Elongation::Elongation (ProcessiveChemical& processive_chemical,
			BoundChemical& chemical_after_step,
			int step_size, double rate)
  : _processive_chemical (processive_chemical)
  , _chemical_after_step (chemical_after_step)
  , _step_size (step_size)
  , _rate (rate)
{
  /** @pre Rate must be positive. */
  REQUIRE (rate >= 0);
  /** @pre Step size must be strictly positive. */
  REQUIRE (step_size > 0);

  _reactants.push_back (&processive_chemical);
  _products.push_back (&chemical_after_step);
  _products.push_back (&_processive_chemical.stalled_form());
}
 
// Not needed for this class (use of default copy constructor) !
// Elongation::Elongation (Elongation& other_elongation);

Elongation::~Elongation( void )
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//
bool Elongation::is_reaction_possible (void) const
{
  return (_processive_chemical.number() > 0);
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
// Elongation& Elongation::operator= (Elongation& other_elongation);


// =================
//  Private Methods
// =================
//
void Elongation::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  bool stall = false;
  
  // choose one unit to move randomly
  _processive_chemical.focus_random_unit();
  
  // update position on location if it is possible
  ChemicalSequence& location = _processive_chemical.focused_unit_location();
  if ( not location.is_out_of_bounds (_processive_chemical.focused_unit_position() + _step_size,
				      _processive_chemical.focused_unit_length()))
    {
      // move the processive chemical (order is important here)
      location.move_bound_unit ( _processive_chemical, _step_size ); // first update location
      _processive_chemical.step_forward ( _step_size );
    }
  else 
    {
      std::cout << "out of bounds\n";
      stall = true;
    }

  // check whether the unit has reached a termination site
  if ( _processive_chemical.is_terminating() == true ) { stall = true; }

  // replace the chemical with its stalled form or its form after step
  if ( stall == true ) // stalled
    {
      // create a stalled form of the chemical
      BoundChemical& stalled_form = _processive_chemical.stalled_form();
      stalled_form.add_unit_in_place_of ( _processive_chemical );
      // update location status
      location.replace_bound_unit ( _processive_chemical, stalled_form );
      // delete processive chemical
      _processive_chemical.remove_focused_unit();
    }
  else // stepped
    {
      // transform the processive chemical in its form after stepping
      _chemical_after_step.add_unit_in_place_of (_processive_chemical); // first create _chemical_after_step
      _processive_chemical.focused_unit_location().replace_bound_unit (_processive_chemical, _chemical_after_step);      
      _processive_chemical.remove_focused_unit();
    }
}

double Elongation::compute_rate (void) const
{
  /**
   * Elongation rate is simply r = #(processive_chemical) * elongation_rate / step_size.
   */
  return (_rate * _processive_chemical.number()) / _step_size;
}

void Elongation::print (std::ostream& output) const
{
  output << "Elongation reaction.";
}
