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
 * @file reactionclassification.cpp
 * @brief Implementation of the ReactionClassification class.
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
#include "reactionclassification.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionClassification::ReactionClassification (void)
  : _current_size (0)
{
}

// Not needed for this class (use of compiler generated versions)
// ReactionClassification::ReactionClassification (const ReactionClassification& other_classification);
// ReactionClassification& ReactionClassification::operator= (const ReactionClassification& other_classification);
// ReactionClassification::~ReactionClassification;

// ===========================
//  Public Methods - Commands
// ===========================
//
int ReactionClassification::create_new_class (double time_step)
{
  /** @pre time_step must be strictly positive or equal to ALWAYS_UPDATED. */
  REQUIRE ((time_step > 0) || (time_step == ALWAYS_UPDATED));

  // increase current number of classes
  ++_current_size;

  // create new list in the reaction vector
  _reaction_classification.resize (_current_size);

  // add time step in the _time_steps vector
  _time_steps.push_back (time_step);

  // return class identifier (index of the class in the vectors)
  return _current_size - 1;
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
const std::vector<Reaction*>& ReactionClassification::get_class (int class_id) const
{
  std::list< std::vector<Reaction*> >::const_iterator class_it = _reaction_classification.begin();
  int class_index = 0;
  while (class_index < class_id) { ++class_it, ++class_index; }
  return *class_it;
}

std::vector<Reaction*>& ReactionClassification::get_class (int class_id)
{
  std::list< std::vector<Reaction*> >::iterator class_it = _reaction_classification.begin();
  int class_index = 0;
  while (class_index < class_id) { ++class_it, ++class_index; }
  return *class_it;
}
