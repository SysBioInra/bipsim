// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 



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
  , _current_max_rate (base_rate)
  , _tokens (number_rates)
  , _groups (1)
  , _group_container (1, false)
  , _update_stack (new RateValidity (1))
{
  // create one group to start with
  _groups [0] = new RateGroup (0, base_rate);
  
  // initialize tokens
  for (std::size_t i = 0; i < _tokens.size(); ++i)
    {
      _tokens [i] = new RateToken (i);
      _tokens [i]->set_group (GroupKey(), NULL_GROUP);
    }
}

// Forbidden
// HybridRateContainer::HybridRateContainer (const HybridRateContainer& other_container);
// HybridRateContainer& HybridRateContainer::operator= (const HybridRateContainer& other_container);

HybridRateContainer::~HybridRateContainer (void)
{
  // destroy groups, tokens and update stack
  for (std::size_t i = 0; i < _groups.size(); ++i) { delete _groups [i]; }
  for (std::size_t i = 0; i < _tokens.size(); ++i) { delete _tokens [i]; }
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
      new_update_stack->invalidate (_update_stack->front());
      _update_stack->pop();
    }
  delete _update_stack;
  _update_stack = new_update_stack;
}
