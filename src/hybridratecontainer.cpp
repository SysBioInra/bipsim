

/**
 * @file hybridratecontainer.cpp
 * @brief Implementation of the ClassName class.
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
#include "hybridratecontainer.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
HybridRateContainer::HybridRateContainer (int number_rates, double base_rate)
  : _base_rate (base_rate)
  , _tokens (number_rates)
  , _groups (1)
  , _update_stack (new RateValidity (1))
  , _group_container (1, false)
  , _current_max_rate (base_rate)
{
  // create one group to start with
  _groups [0] = new RateGroup (0, base_rate);
  
  // initialize tokens
  for (int i = 0; i < _tokens.size(); ++i)
    {
      _tokens [i] = new RateToken (i);
      _tokens [i]->set_group (GroupKey(), NULL_GROUP);
    }
}

// Not needed for this class (use of default copy constructor) !
// ClassName::ClassName (const ClassName& other_class_name);

HybridRateContainer::~HybridRateContainer (void)
{
  // destroy groups
  for (int i = 0; i < _groups.size(); ++i) { delete _groups [i]; }
  
  // destroy tokens
  for (int i = 0; i < _tokens.size(); ++i) { delete _tokens [i]; }
  
  // destroy update stack
  delete _update_stack;
}


// ===========================
//  Public Methods - Commands
// ===========================
//


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
// ClassName& ClassName::operator= (const ClassName& other_class_name);


// =================
//  Private Methods
// =================
//
void HybridRateContainer::_create_new_groups (double value)
{
  // create new groups
  while (_current_max_rate <= value)
    {
      _groups.push_back (new RateGroup (_current_max_rate, 
					2*_current_max_rate));
      _current_max_rate *= 2;
    }
  
  // extend group container
  _group_container.extend (_groups.size() - _group_container.size ());

  // create new update stack
  RateValidity* new_update_stack = new RateValidity (_groups.size());
  while (!_update_stack->empty())
    {
      new_update_stack->update (_update_stack->front());
      _update_stack->pop();
    }
  delete _update_stack;
  _update_stack = new_update_stack;
}
