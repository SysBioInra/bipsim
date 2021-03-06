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
 * @file naiveratemanager.cpp
 * @brief Implementation of the NaiveRateManager class.
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
#include "naiveratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
NaiveRateManager::NaiveRateManager (const SimulationParams& params,
				    const std::vector <Reaction*>& reactions)
  : RateManager (params, reactions)
{
}

// Not needed for this class (use of compiler generated versions)
// NaiveRateManager::NaiveRateManager (const NaiveRateManager& other_manager);
// NaiveRateManager& NaiveRateManager::operator= (const NaiveRateManager& other_manager);
// NaiveRateManager::~NaiveRateManager (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void NaiveRateManager::update_rates (void)
{
  compute_all_rates();
  cumulate_rates();
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
