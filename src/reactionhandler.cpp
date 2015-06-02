

/**
 * @file reactionhandler.cpp
 * @brief Implementation of the ReactionHandler class.
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
#include "reactionhandler.h"
#include "reaction.h"
#include "binding.h"
#include "chemicalreaction.h"
#include "complexation.h"
#include "elongation.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionHandler::ReactionHandler (void)
  :_last_reference (0)
{
}

// Not needed for this class (use of default copy constructor) !
// ReactionHandler::ReactionHandler ( const ReactionHandler& other_reaction_handler );

ReactionHandler::~ReactionHandler (void)
{
  std::list< Reaction* >::iterator reaction = _references.begin();
  while ( reaction != _references.end() )
    {
      delete *reaction;
      reaction++;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ReactionHandler::create_chemical_reaction (std::vector<Chemical*>& components,
						std::vector<int>& stoichiometry,
						double forward_rate_constant,
						double backward_rate_constant)
{
  ChemicalReaction* reaction = new ChemicalReaction (components, stoichiometry,
						     forward_rate_constant, 
						     backward_rate_constant);
  _references.push_back (reaction);
  _last_reference = reaction;
}

void ReactionHandler::create_binding (Chemical& unit_to_bind, BoundChemical& binding_result, int binding_site_family)
{
  Binding* reaction = new Binding (unit_to_bind, binding_result, binding_site_family);
  _references.push_back (reaction);
  _last_reference = reaction;
}

void ReactionHandler::create_complexation (Chemical& component_a, Chemical& component_b, Chemical& complex, double k_on, double k_off)
{
  Complexation* reaction = new Complexation (component_a, component_b, complex, k_on, k_off);
  _references.push_back (reaction);
  _last_reference = reaction;
}

void ReactionHandler::create_elongation (ProcessiveChemical& processive_chemical, int step_size, double rate)
{
  Elongation* reaction = new Elongation (processive_chemical, step_size, rate);
  _references.push_back (reaction);
  _last_reference = reaction;
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
// ReactionHandler& ReactionHandler::operator= ( const ReactionHandler& other_reaction_handler );

std::ostream& operator<< (std::ostream& output, const ReactionHandler& reaction_handler)
{
  output << "There are currently " << reaction_handler._references.size() << " reactions." << std::endl;
  for (std::list<Reaction*>::const_iterator next_reference = reaction_handler._references.begin();
       next_reference != reaction_handler._references.end();
       next_reference++ )
    {
      output << *(*next_reference) << std::endl;
    }
  
  return output;
}

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ReactionHandler::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
