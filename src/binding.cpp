

/**
 * @file binding.cpp
 * @brief Implementation of the Binding class.
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
#include "macros.h" // REQUIRE()
#include "binding.h"
#include "bindingsite.h"
#include "bindingsitehandler.h"
#include "boundchemical.h"
#include "chemical.h"

BindingSiteHandler *Binding::_binding_site_handler = 0;

// ==========================
//  Constructors/Destructors
// ==========================
//
Binding::Binding( void )
  :Reaction()
{
  _unit_to_bind = 0;
  _binding_result = 0;
  _binding_site_family = 0;
}

// Not needed for this class (use of default copy constructor) !
// Binding::Binding (Binding& other_Binding);

Binding::~Binding( void )
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Binding::perform_forward( void )
{
  REQUIRE( _unit_to_bind != 0 ); /** @pre Unit to bind has been initialized. */
  REQUIRE( Binding::_binding_site_handler != 0 ); /** @pre Binding site handler has been initialized. */
  REQUIRE( _binding_result != 0 ); /** @pre Binding result has been initialized. */
  REQUIRE( _binding_site_family != 0 ); /** @pre Binding site family has been initialized. */
  REQUIRE( _unit_to_bind->number() > 0 ); /** @pre There is at least one element to bind. */

  // Number of free  elements is updated.
  _unit_to_bind->remove( 1 );
  
  // A binding site in the family is randomly chosen and occupied.
  Binding::_binding_site_handler->add_unit_on_random_available_site( _binding_site_family, *_binding_result );
}

void Binding::perform_backward( void )
{
  REQUIRE( _unit_to_bind != 0 ); /** @pre Unit to bind has been initialized. */
  REQUIRE( _binding_site_handler != 0 ); /** @pre Binding site handler has been initialized. */
  REQUIRE( _binding_result != 0 ); /** @pre Binding result has been initialized. */
  REQUIRE( _binding_site_family != 0 ); /** @pre Binding site family has been initialized. */
  REQUIRE( _binding_result->number() > 0 ); /** @pre There is at least one element to unbind. */

  // Number of free elements is updated.
  _unit_to_bind->add( 1 );

  // A unit bound through a site in the family is randomly chosen and removed.
  _binding_result->unbind_random( _binding_site_family );
  
}


// ============================
//  Public Methods - Accessors
// ============================
//
double Binding::forward_rate( void )
{
  std::cout << "Function " << __func__ << " remains to be defined in " << __FILE__ << __LINE__ << std::endl;
  return 0;
}

double Binding::backward_rate( void )
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
// Binding& Binding::operator= (Binding& other_Binding);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Binding::check_invariant (void)
{
  bool result = Reaction::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
