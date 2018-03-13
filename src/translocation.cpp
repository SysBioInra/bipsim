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
 * @file translocation.cpp
 * @brief Implementation of the Translocation class.
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
#include "macros.h" // REQUIRE
#include "config.h" // DISPLAY_WARNINGS

#include "translocation.h"
#include "boundchemical.h"
#include "boundunit.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Translocation::Translocation (BoundChemical& processive_chemical,
			      BoundChemical& chemical_after_step,
			      BoundChemical& stalled_form,
			      int step_size, double rate)
  : _processive_chemical (processive_chemical)
  , _chemical_after_step (chemical_after_step)
  , _stalled_form (stalled_form)
  , _step_size (step_size)
  , _rate (rate)
  , _volume_constant (rate)
{
  /** @pre Rate must be positive. */
  REQUIRE (rate >= 0);
  /** @pre Step size must be strictly positive. */
  REQUIRE (step_size > 0);

  _reactants.push_back (&processive_chemical);
  _products.push_back (&chemical_after_step);
  _products.push_back (&stalled_form);
}
 
// Not needed for this class (use of compiler generated versions)
// Translocation::Translocation (Translocation& other);
// Translocation& Translocation::operator= (Translocation& other);
// Translocation::~Translocation (void);

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
void Translocation::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  bool stall = false;
  
  // choose one unit to move randomly
  BoundUnit& unit = _processive_chemical.random_unit();
  
  // update position on location if it is possible
  int new_first = unit.first() + _step_size;
  int new_last = unit.last() + _step_size;
  if (!unit.location().is_out_of_bounds (new_first, new_last))
    {
      unit.location().unbind_unit (unit.first(), unit.last()); 
      unit.move (_step_size);
      unit.location().bind_unit (unit.first(), unit.last());
    }
  else 
    { 
      stall = true; 
#ifdef DISPLAY_WARNINGS
      std::cerr << "out of bounds\n"; 
#endif
    }
  
  // replace the chemical with its stalled form or its form after step
  _processive_chemical.remove (unit);
  if (stall == false) { _chemical_after_step.add (unit); }
  else  { _stalled_form.add (unit); }
}


double Translocation::compute_rate (void) const
{
  /**
   * Translocation rate is simply
   *  r = #(processive_chemical) * translocation_rate.
   */
  return _volume_constant * _processive_chemical.number();
}

void Translocation::print (std::ostream& output) const
{
  output << "Translocation reaction.";
}
