

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
// Not needed for this class (use of default copy constructor) !
// Elongation::Elongation (Elongation& other_elongation);

Elongation::~Elongation( void )
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Elongation::perform_forward( void )
{
  // choose one unit to move randomly
  _processive_chemical.focus_random_unit ();
  // update position on its location
  _processive_chemical.focused_unit_location().move_bound_unit( _processive_chemical, _step_size );
  // move it
  _processive_chemical.step_forward ( _step_size );

  // check whether the unit has reached a termination site
  if ( _processive_chemical.is_terminating() == true )
    {
      // stall the processive chemical
      // create a stalled form of the chemical
      BoundChemical& stalled_form = _processive_chemical.stalled_form();
      stalled_form.add_unit_in_place_of ( _processive_chemical );
      // update location status
      _processive_chemical.focused_unit_location().replace_bound_unit ( _processive_chemical, stalled_form );
      // delete processive chemical
      _processive_chemical.remove_focused_unit();
    }
}

void Elongation::perform_backward( void )
{
  std::cerr << "Elongation reaction cannot be performed backwards..." << std::endl;
}


// ============================
//  Public Methods - Accessors
// ============================
//
double Elongation::forward_rate( void ) const
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
  return 0;
}

double Elongation::backward_rate( void ) const
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
  return 0;
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

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Elongation::check_invariant (void) const
{
  bool result = Reaction::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
