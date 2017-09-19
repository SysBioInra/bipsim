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
 * @file ratevalidity.cpp
 * @brief Implementation of the RateValidity class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "ratevalidity.h"
#include "rateinvalidator.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RateValidity::RateValidity (int number_rates)
  : _invalidated (number_rates, false)
  , _invalidators (number_rates)
{
  /** @pre number_rates must be positive. */
  REQUIRE (number_rates > 0);
  for (std::size_t i = 0; i < _invalidators.size(); ++i)
    { _invalidators[i] = new RateInvalidator (*this, i); }
}

// RateValidity::RateValidity (const RateValidity& other);
// RateValidity& RateValidity::operator= (const RateValidity& other);

RateValidity::~RateValidity (void)
{
  for (std::vector <RateInvalidator*>::iterator it = _invalidators.begin();
       it != _invalidators.end(); ++it)
    { delete *it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void RateValidity::extend (int extension_size)
{
  /** @pre extension_size must be positive. */
  REQUIRE (extension_size > 0);
  int old_size = _invalidated.size();
  int new_size = old_size + extension_size;
  _invalidated.resize (new_size, false);
  _invalidators.resize (new_size);
  for (int i = old_size; i < new_size; ++i)
    { _invalidators[i] = new RateInvalidator (*this, i); }
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
