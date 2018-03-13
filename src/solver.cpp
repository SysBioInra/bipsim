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
 * @file solver.cpp
 * @brief Implementation of the Solver class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE() ENSURE()
#include "solver.h"
#include "reaction.h"
#include "cellstate.h"
#include "simulationparams.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double Solver::NO_REACTION_LEFT =
  std::numeric_limits<double>::infinity();

Solver::Solver (const SimulationParams& params)
  : _t (params.initial_time())
  , _number_reactions_performed (0)
{
}

// Not needed for this class (use of compiler generated versions)
// Solver::Solver (const Solver& other_solver);
// Solver& Solver::operator= (const Solver& other_solver);
// Solver::~Solver (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void Solver::solve (double time_step)
{  
  double final_time = _t + time_step;
  while (next_reaction_time() < final_time) { perform_next_reaction(); }
  _t = final_time;
}

void Solver::perform_next_reaction (void)
{
  REQUIRE (next_reaction_time() != NO_REACTION_LEFT);
  // perform next reaction
  _t = next_reaction_time(); 
  next_reaction().perform();
  ++_number_reactions_performed;
  schedule_next_reaction();
}

void Solver::reschedule (double time)
{
  _t = time;
  reinitialize();
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
