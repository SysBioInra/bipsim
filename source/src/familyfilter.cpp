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
 * @file familyfilter.cpp
 * @brief Implementation of the FamilyFilter class.
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
#include "familyfilter.h"
#include "boundunit.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
FamilyFilter::FamilyFilter (const BindingSiteFamily& family)
  : _family (&family)
{
}

// Not needed for this class (use of compiler-generated versions)
// FamilyFilter::FamilyFilter (const FamilyFilter& other);
// FamilyFilter& FamilyFilter::operator= (const FamilyFilter& other);
// FamilyFilter::~FamilyFilter (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
inline void FamilyFilter::add (BoundUnit& unit)
{
  if ((unit.binding_site() != 0)
      && (&(unit.binding_site()->family()) == _family)
      && (unit.binding_site()->first() == unit.first())) 
    { _units.add (unit); }
}

inline void FamilyFilter::remove (BoundUnit& unit)
{
  if ((unit.binding_site() != 0)
      && (&(unit.binding_site()->family()) == _family))
    { _units.remove (unit); }
}


// ============================
//  Public Methods - Accessors
// ============================
//

double FamilyFilter::total_unbinding_rate (void) const
{
  // we loop through the list and sum the k_off
  double r_total = 0;
  for (std::vector <BoundUnit*>::const_iterator it = _units().begin();
       it != _units().end(); ++it)
    { r_total += (*it)->binding_site()->k_off(); }
  return r_total;
}

// =================
//  Private Methods
// =================
//
